/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "console.h"
#include "../SRP_HS_USB_PROTOCOL/SRP_HS_USB_Protocol.h"

#include <QLabel>
#include <QMessageBox>
#include <QTimer>
#include <QThread>
#include "crc16.h"
#include "global_vars.h"
#include "synchro_watcher.h"
#include "indigo_base_protocol.h"

// Extern variables
extern QElapsedTimer profiler_timer;
extern RingBuffer *m_ring;
extern qint64 elapsed_all_saved;
extern IndigoBaseProtocolBuilderV1 g_frame_builder;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow),
    m_status(new QLabel),
    m_console(new Console),
    m_serial(new QSerialPort(this)),
    m_message_box(new CMessageBox)
{
    m_ui->setupUi(this);

    m_console->setEnabled(false);
    m_console->setReadOnly(true);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(m_console);
    m_ui->tab_1->setLayout(layout);

    //m_ui->actionConnect->setEnabled(true);
    //m_ui->actionDisconnect->setEnabled(false);
    //m_ui->actionQuit->setEnabled(true);
    //m_ui->actionConfigure->setEnabled(true);

    m_ui->statusBar->addWidget(m_status);

    initActionsConnections();

    connect(m_serial, &QSerialPort::errorOccurred, this, &MainWindow::handleError);
    connect(m_serial, &QSerialPort::readyRead, this, &MainWindow::readDataSerialPort);
    connect(m_message_box, &CMessageBox::postData, this, &MainWindow::transmitDataSerialPort);
    connect(m_message_box, &CMessageBox::postDataToStm32H7, this, &MainWindow::postTxDataSTM);
    connect(m_message_box, &CMessageBox::calculatePredistortionTablesStart, this, &MainWindow::calculatePredistortionTablesStart);
    connect(&m_usb_thread, &UsbWorkThread::postTxDataToSerialPort, this, &MainWindow::transmitDataSerialPort, Qt::ConnectionType::QueuedConnection);
    connect(&m_usb_thread, &UsbWorkThread::postWaitToSerialPort, this, &MainWindow::transmitWaitToSerialPort, Qt::ConnectionType::QueuedConnection);
    connect(&m_usb_thread, &UsbWorkThread::consolePutData, this, &MainWindow::consolePutData, Qt::ConnectionType::QueuedConnection);
    connect(&m_usb_thread, &UsbWorkThread::usbInitTimeoutStart, this, &MainWindow::usbInitTimeoutStart, Qt::ConnectionType::QueuedConnection);
    connect(&m_usb_thread, &UsbWorkThread::consoleAdcFile, this, &MainWindow::consoleAdcData, Qt::ConnectionType::QueuedConnection);
    connect(&m_usb_thread, &UsbWorkThread::hsDataReceived, this, &MainWindow::usbHsDataReceived, Qt::ConnectionType::QueuedConnection);
    //connect(&m_usb_thread, &UsbWorkThread::postDataToStm32H7, this, &MainWindow::postTxDataSTM);
    connect(&m_qam_thread, &QamThread::consolePutData, this, &MainWindow::consolePutData, Qt::ConnectionType::QueuedConnection);
    connect(&m_qam_thread, &QamThread::consoleFrameErrorFile, this, &MainWindow::consoleFrameErrorData, Qt::ConnectionType::QueuedConnection);
    connect(&m_qam_thread, &QamThread::postTxDataToSerialPort, this, &MainWindow::transmitDataSerialPort, Qt::ConnectionType::QueuedConnection);
    connect(&m_freq_sweep_thread, &FreqSweepThread::consolePutData, this, &MainWindow::consolePutData, Qt::ConnectionType::QueuedConnection);
    connect(&m_mod_tx_thread, &ModTransmitterThread::consolePutData, this, &MainWindow::consolePutData, Qt::ConnectionType::QueuedConnection);
    connect(&m_mod_tx_thread, &ModTransmitterThread::postDataToStm32H7, this, &MainWindow::postTxDataSTM, Qt::ConnectionType::QueuedConnection);
    connect(&m_mod_tx_thread, &ModTransmitterThread::sendCommandToSTM32, this, &MainWindow::sendCommandToSTM32, Qt::ConnectionType::QueuedConnection);
    connect(&m_mod_tx_thread, &ModTransmitterThread::qamDecoderReset, this, &MainWindow::qamDecoderReset, Qt::ConnectionType::QueuedConnection);
    connect(&m_mod_tx_thread, &ModTransmitterThread::setStatus, this, &MainWindow::setAutoCfgStatus, Qt::ConnectionType::QueuedConnection);

    m_console->putData("SBC Application\n", 1);
    m_console->putData("Opening serial port...\n", 1);

    // Clear serial port tx & rx buffer
    TtyUserRxBuffer.clear();

    // Timers
    // Serial port rx timeout timer
    timerSpRxTimeout = new QTimer();
    timerSpRxTimeout->setSingleShot(true);
    connect(timerSpRxTimeout, SIGNAL(timeout()), this, SLOT(timeoutSerialPortRx()));

    // Reconnect to serial port in case of error
    timerSpReconnect = new QTimer();
    timerSpReconnect->setSingleShot(true);
    connect(timerSpReconnect, SIGNAL(timeout()), this, SLOT(timeoutSerialPortReconnect()));

    // Usb initialization timer
    timerUsbInit = new QTimer();
    timerUsbInit->setSingleShot(true);
    connect(timerUsbInit, SIGNAL(timeout()), this, SLOT(timeoutUsbInitCallback()));

    // Indigo base protocol initialization
    g_frame_builder.Initialize();
    g_frame_builder.SetNotify(frame_builded_cb, this);

    openSerialPort();

    // LibUsb initialization, init usb device
    /* Get libusb version */
    const struct libusb_version *version;
    version = libusb_get_version();
    m_console->putData(QString("libusb version: %1.%2.%3.%4\n")
                       .arg(version->major).arg(version->minor)
                       .arg(version->micro).arg(version->nano), 1);

    // Start timer for continuous usb initialization attempts
    timerUsbInit->start(m_usb_thread.timeoutUsbInit_ms);

    // Start QAM decoder thread
    m_qam_thread.start();

    // Start QAM frequency estimate for sweep thread
    m_freq_sweep_thread.start();

    // Start MOD commands transmitter thread
    m_mod_tx_thread.start();

    // Timeout timer for MOD answers
    timerModAnswerTimeout = new QTimer(this);
    timerModAnswerTimeout->setSingleShot(true);
    timerModAnswerTimeout->connect(&m_mod_tx_thread, SIGNAL(startAnswerTimeoutTimer(int)), SLOT(start(int)), Qt::ConnectionType::QueuedConnection);
    timerModAnswerTimeout->connect(&m_mod_tx_thread, SIGNAL(stopAnswerTimeoutTimer()), SLOT(stop()), Qt::ConnectionType::QueuedConnection);

    m_mod_tx_thread.connect(timerModAnswerTimeout, SIGNAL(timeout()), SLOT(timeoutAnswer()));
}

MainWindow::~MainWindow()
{
    timerUsbInit->stop();
    timerSpReconnect->stop();
    timerSpRxTimeout->stop();

    closeSerialPort();
    m_usb_thread.USB_Deinit();

    m_console->Close();
    delete m_ui;
}

void MainWindow::consolePutData(const QString &data, quint8 priority)
{
    if(m_console == nullptr)
        return;

    m_console->putData(data, priority);
}

void MainWindow::consoleAdcData(const quint8 *p_data, quint32 size)
{
    if(m_console == nullptr)
        return;

    m_console->putDataAdc(p_data, size);
}

void MainWindow::consoleFrameErrorData(const qint16 *p_data, quint32 len)
{
    if(m_console == nullptr)
        return;

    m_console->putFrameErrorData(p_data, len);
}

bool MainWindow::openSerialPort()
{
    struct Settings {
        QString name;
        qint32 baudRate;
        QString stringBaudRate;
        QSerialPort::DataBits dataBits;
        QString stringDataBits;
        QSerialPort::Parity parity;
        QString stringParity;
        QSerialPort::StopBits stopBits;
        QString stringStopBits;
        QSerialPort::FlowControl flowControl;
        QString stringFlowControl;
        bool localEchoEnabled;
    };

    Settings settings;
    settings.name = "ttyGS0";
    settings.baudRate = 460800;
    settings.stringBaudRate = "460800";
    //settings.baudRate = 9600;
    //settings.stringBaudRate = "9600";
    settings.dataBits = QSerialPort::Data8;
    settings.flowControl = QSerialPort::NoFlowControl;
    ///settings.localEchoEnabled = true;
    settings.localEchoEnabled = false;
    settings.parity = QSerialPort::NoParity;
    settings.stopBits = QSerialPort::OneStop;
    settings.stringDataBits = "8";
    settings.stringFlowControl= "None";
    settings.stringParity = "None";
    settings.stringStopBits = "1";

    const Settings p = settings;
    m_serial->setPortName(p.name);
    m_serial->setBaudRate(p.baudRate);
    m_serial->setDataBits(p.dataBits);
    m_serial->setParity(p.parity);
    m_serial->setStopBits(p.stopBits);
    m_serial->setFlowControl(p.flowControl);

    if (m_serial->open(QIODevice::ReadWrite))
    {
        m_console->setEnabled(true);
        //m_console->setLocalEchoEnabled(p.localEchoEnabled);
        m_ui->actionConnect->setEnabled(false);
        m_ui->actionDisconnect->setEnabled(true);
        m_ui->actionConfigure->setEnabled(false);

        m_console->putData(tr("Connected to %1 : %2, %3, %4, %5, %6\n")
                           .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                           .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl), 1);
        return true;
    }
    else
    {
        QByteArray str_b;
        str_b.append(tr("Error "));
        str_b.append(m_serial->errorString());
        str_b.append("\n");
        m_console->putData(str_b, 1);
        return false;
    }
}

void MainWindow::closeSerialPort()
{
    if (m_serial->isOpen())
        m_serial->close();
    m_console->setEnabled(false);
    m_ui->actionConnect->setEnabled(true);
    m_ui->actionDisconnect->setEnabled(false);
    m_ui->actionConfigure->setEnabled(true);
    //showStatusMessage(tr("Disconnected"));
    m_console->putData("Serial port disconnected\n", 1);
}

void MainWindow::timeoutUsbInitCallback()
{
    m_usb_thread.USB_Init();
}

void MainWindow::usbInitTimeoutStart(int timeout_ms)
{
    timerUsbInit->start(timeout_ms);
}

void MainWindow::timeoutSerialPortReconnect()
{
    m_console->putData("SP Trying to reconnect to a serial port...\n", 1);
    if(!openSerialPort())
    {
        // Start timer again
        timerSpReconnect->start(timeoutSerialPortReconnect_ms);
    }
}

void MainWindow::timeoutSerialPortRx()
{
    m_console->putData("SP receive timeout\n", 0);
    parseDataSerialPort();
}

void MainWindow::serialPortRxCleanup()
{
    TtyUserRxBuffer_len = 0;
    TtyUserRxBuffer.clear();
}

void MainWindow::readDataSerialPort()
{
    // Serial port data received (from PC)
    profiler_timer.start();

    // Drop any USB received data to prevent old packets been transmitted to serial port (to PC)
    if(!m_mod_tx_thread.m_AutoConfigurationMode)
    {
        //todo
        //if(!syncIsSuspendedTimeInProgress())
        m_usb_thread.usb_receiver_drop = true;
        m_qam_thread.data_drop = true;
    }

    timerSpRxTimeout->stop();

    qint64 length_rx = m_serial->bytesAvailable();

    while(length_rx)
    {
        if(TtyUserRxBuffer_len)
            m_console->putData("SP Received " + QString::number(TtyUserRxBuffer_len) + " + " + QString::number(length_rx) + " bytes\n", 0);
        else
        {
            m_console->putData("SP Received " + QString::number(length_rx) + " bytes\n", 0);
//            if(length_rx == 269)
//            {
//                timerSpRxTimeout->stop();   // debug remove
//            }
        }

        TtyUserRxBuffer.append(m_serial->readAll());
        TtyUserRxBuffer_len += length_rx;
        length_rx = m_serial->bytesAvailable();
    }

    // Continue receive
    // Start receive timeout
    timerSpRxTimeout->start(timeoutSerialPortRx_ms);
}

void MainWindow::parseDataSerialPort()
{
//    profiler_timer.start();
    uint8_t addr = TtyUserRxBuffer.at(0);

    // If package for listed tools, response with 'quick answer'
    if (addr == m_message_box->PT_ADDR ||
        addr == m_message_box->SILS_ADDR ||
        addr == m_message_box->SFBS_ADDR ||
        addr == m_message_box->NAV_ADDR ||
        addr == m_message_box->ReCap_ADDR ||
        addr == m_message_box->XYC_ADDR ||
        addr == m_message_box->HEX_ADDR ||
        addr == m_message_box->Ind1h_ADDR)
    {
        quick_answer[0] = addr;
        transmit_quick_answer = true;
    }
    // If package for SRP2
    else if (addr == m_message_box->SRP2_ADDR)
    {
        // Check length
        if(TtyUserRxBuffer_len >= TtyUserRxBuffer_MaxSize)
            TtyUserRxBuffer_len = TtyUserRxBuffer_MaxSize;

        // len < minimum packet size
        if(TtyUserRxBuffer_len < m_message_box->TX_HEADER_LENGTH + m_message_box->DATA_ADDRESS_LENGTH + m_message_box->CRC_LENGTH)
        {
            m_console->putData("SP Broken packet from PC to SRP: length is too short\n", 1);
            serialPortRxCleanup();

            // Unlock USB receiver
            m_usb_thread.usb_receiver_drop = false;
            m_qam_thread.data_drop = false;
            m_ring->Clear();
            return;
        }

        // Check crc
        uint16_t crc16 = uint16_t(TtyUserRxBuffer.at(TtyUserRxBuffer_len - 2) & 0xFF) | (uint16_t(TtyUserRxBuffer.at(TtyUserRxBuffer_len - 1)) << 8);

        const uint8_t *p_data = (const uint8_t*)TtyUserRxBuffer.data();
        if(calc_crc16(p_data + 1, TtyUserRxBuffer_len-1-m_message_box->CRC_LENGTH) != crc16)
        {
            m_console->putData("SP Broken packet from PC to SRP: crc error\n", 1);
            serialPortRxCleanup();

            // Unlock USB receiver
            m_usb_thread.usb_receiver_drop = false;
            m_qam_thread.data_drop = false;
            m_ring->Clear();
            return;
        }

        // Crc ok, continue to message box
        if(m_message_box->message_box_srp((uint8_t*)TtyUserRxBuffer.data(), uint16_t(TtyUserRxBuffer_len), m_message_box->MASTER_ADDR, m_message_box->SRP2_ADDR))
        {
            // Unlock USB receiver
            m_usb_thread.usb_receiver_drop = false;
            m_qam_thread.data_drop = false;
            m_ring->Clear();
        }
        else
        {
            // data is being transmitted to STM32H7, usb_receiver_drop flag will be reset after the transfer
            m_qam_thread.data_drop = false;
            m_ring->Clear();
        }

        serialPortRxCleanup();
        return;
    }

    // Data for other tools

    // Check if synchronization 'suspended time' is in progress
    if(syncIsSuspendedTimeInProgress())
    {
        uint16_t suspended_time_left = syncGetTime();
        if(suspended_time_left)
        {
            m_console->putData("Sync suspended time is in progress: answer with 'CmdWaitAgainSend'\n", 1);

            // Answer with 'wait' (indigo base protocol answer)
            command_sync_wait_creator(suspended_time_left, EnumCmdWaitAgainSend);

            // Unlock USB receiver
            m_usb_thread.usb_receiver_drop = false;
            m_qam_thread.data_drop = false;
            m_ring->Clear();
            return;
        }
        else
        {
            // Uncomment if we get rid of 'USB_CMD_SYNCHRO_STOP'
            //syncSetSuspendedTimeInProgress(false, 0);
        }
    }

    // Check length
    if(TtyUserRxBuffer_len > TtyUserRxBuffer_MaxSize)
    {
        TtyUserRxBuffer_len = TtyUserRxBuffer_MaxSize;
        m_console->putData("SP Warning: received length is too big\n", 1);
    }

    m_qam_thread.data_drop = false;
    m_ring->Clear();

    // Transmit to STM32H7
    postTxDataSTM((uint8_t*)TtyUserRxBuffer.data(), int(TtyUserRxBuffer_len));
    serialPortRxCleanup();

    if(transmit_quick_answer)
    {
        transmit_quick_answer = false;
        transmitDataSerialPort(quick_answer, sizeof(quick_answer));
    }
}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    // TODO other errors?
    if (error == QSerialPort::ResourceError)
    {
        m_console->putData("SP Critical Error: " + m_serial->errorString() + "\n", 1);
        closeSerialPort();

        // Start timer to periodically try to reconnect
        timerSpReconnect->start(timeoutSerialPortReconnect_ms);
    }
}

void MainWindow::initActionsConnections()
{
    connect(m_ui->actionAbout, &QAction::triggered, this, &MainWindow::about);
    connect(m_ui->actionAboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);
    connect(m_ui->actionLogsFlush, &QAction::triggered, this, &MainWindow::logFileFlush);
    connect(m_ui->actionLogsOpenFile, &QAction::triggered, this, &MainWindow::logFileOpen);
    connect(m_ui->actionSend_HS_command_GET_STATUS, &QAction::triggered, this, &MainWindow::sendHsCommandGetStatus);
    connect(m_ui->actionSend_HS_command_GET_DATA, &QAction::triggered, this, &MainWindow::sendHsCommandGetData);
    connect(m_ui->actionGET_DATA_SIZE, &QAction::triggered, this, &MainWindow::sendHsCommandGetDataSize);
    connect(m_ui->actionADC_START, &QAction::triggered, this, &MainWindow::sendHsCommandAdcStart);
    connect(m_ui->actionSend_AGC_Start, &QAction::triggered, this, &MainWindow::sendHsCommandAgcStart);
    connect(m_ui->actionADC_START_for_SIN_600, &QAction::triggered, this, &MainWindow::sendHsCommandAdcStart2);
    connect(m_ui->actionADC_START_for_Sweep, &QAction::triggered, this, &MainWindow::sendHsCommandAdcStart3);
    connect(m_ui->actionTx_phase_gain_tables_to_MOD, &QAction::triggered, this, &MainWindow::sendPredistortionTables);
}

void MainWindow::showStatusMessage(const QString &message)
{
    m_status->setText(message);
}

// Serial port to PC
// Asynchronous transmit
void MainWindow::transmitDataSerialPort(const uint8_t *p_data, int length)
{
    m_serial->write((const char*)p_data, length);
    m_serial->flush();  // Sending the buffered data immediately to the serial port

    // Logs
#ifdef QT_DEBUG
    m_console->putData("SP Transmit " + QString::number(length) + " bytes\n", 0);
#endif

    qint64 pt_elapsed = profiler_timer.elapsed();
    m_console->putData(QString("Profiler timer elapsed %1 # transmit to sp end, without qam %2\n").arg(pt_elapsed).arg(pt_elapsed - elapsed_all_saved), 1);

    // todo - change slot from 'transmitDataSerialPort' to 'ModAnswerReceived' ?
    // Transmit data to 'm_mod_tx_thread' (MOD transmit & receive commands sequence)
    m_mod_tx_thread.ModAnswerReceived(p_data, length);
}

// USB to STM32H7 board
void MainWindow::postTxDataSTM(const uint8_t *p_data, const int length)
{
    int len = length;

    if(p_data == nullptr)
    {
        m_console->putData("Error postTxDataToSTM32H7 reported p_data = NULL\n", 1);
        return;
    }

    USBheader_t header;

    if(len + sizeof(header) > int(sizeof(m_usb_thread.UserTxBuffer)))
    {
        m_console->putData("Error postTxDataToSTM32H7 reported length is too long\n", 1);
        len = sizeof(m_usb_thread.UserTxBuffer);
    }

    if(len > int(sizeof(USB_Protocol::data)))
    {
        m_console->putData("Error postTxDataToSTM32H7 reported length is too long\n", 1);
        len = sizeof(USB_Protocol::data);
    }

    // Cancel ongoing transfer (if any)
    m_usb_thread.USB_StopTransmit();

    m_usb_thread.UserTxBuffer_len = len + sizeof(header);

    header.packet_length = m_usb_thread.UserTxBuffer_len;
    header.type = SRP_LS_DATA;
    header.cmd = 0;  // Don't care

    memcpy(m_usb_thread.UserTxBuffer, (uint8_t*)&header, sizeof(header));
    memcpy(m_usb_thread.UserTxBuffer + sizeof(header), p_data, len);

    m_console->putData("Transmit to H7 " + QString::number(m_usb_thread.UserTxBuffer_len) + " bytes\n", 0);
    m_usb_thread.start_transmit = true;
}

void MainWindow::transmitWaitToSerialPort()
{
    uint16_t suspended_time_left = syncGetTime();
    if(suspended_time_left)
    {
        command_sync_wait_creator(suspended_time_left, EnumCmdWaitRead);
    }
}

// Actions
void MainWindow::about()
{
    QMessageBox::about(this, tr("About SBC Application"),
                       tr("The <b>SBC Application</b> example demonstrates how to "
                          "use the Qt Serial Port module in modern GUI applications "
                          "using Qt, with a menu bar, toolbars, and a status bar."));
}

void MainWindow::logFileFlush()
{
    m_console->fileFlush();
    QMessageBox::information(this, tr("Info"), tr("Log file has been flushed!"));
}

void MainWindow::logFileOpen()
{
    m_console->fileOpen();
}

void MainWindow::sendCommandToSTM32(quint8 command, const quint8 *p_data, quint32 data_size)
{
    if(data_size && p_data == nullptr)
    {
        m_console->putData("Error sendCommandToSTM32sendCommandToSTM32 wrong parameters\n", 1);
        return;
    }

    if(command == USB_CMD_AGC_START)
        m_usb_thread.agc_is_active = true;

    m_usb_thread.sendHsCommand(command, data_size, p_data);
}

void MainWindow::usbHsDataReceived()
{
    m_mod_tx_thread.ModAnswerDataReceived();
}

// Debug HS command from GUI
void MainWindow::sendHsCommandGetStatus()
{
    // Get status
    m_usb_thread.sendHsCommand(USB_CMD_GET_STATUS, 0, nullptr);
}

void MainWindow::sendHsCommandGetData()
{
    m_console->putData(QString("Send HS command GET_DATA, requested data size %1\n").arg(m_usb_thread.stm32_ready_data_size), 1);
    m_usb_thread.sendHsCommand(USB_CMD_GET_DATA, 4, (uint8_t*)&m_usb_thread.stm32_ready_data_size);
}

void MainWindow::sendHsCommandGetDataSize()
{
    m_usb_thread.sendHsCommand(USB_CMD_GET_DATA_SIZE, 0, nullptr);
}

void MainWindow::sendHsCommandAdcStart()
{
    uint32_t adc_data_length = 440000;
    m_usb_thread.sendHsCommand(USB_CMD_ADC_START, 4, (uint8_t*)&adc_data_length);
}

void MainWindow::sendHsCommandAdcStart2()
{
    Set_Special_Command_SIN600(true);

    uint32_t adc_data_length = 31400;
    m_usb_thread.sendHsCommand(USB_CMD_ADC_START, 4, (uint8_t*)&adc_data_length);
}

void MainWindow::sendHsCommandAdcStart3()
{
    Set_Special_Command_Sweep(true);

    uint32_t adc_data_length = 440000;
    m_usb_thread.sendHsCommand(USB_CMD_ADC_START, 4, (uint8_t*)&adc_data_length);
}

void MainWindow::sendHsCommandAgcStart()
{
    sendCommandToSTM32(USB_CMD_AGC_START, nullptr, 0);
}

void MainWindow::sendPredistortionTables()
{
    m_console->putData("Starting of transmit phase & gain tables to MOD\n", 1);

    m_mod_tx_thread.ModStartTransmitPhaseGain();
}

void MainWindow::calculatePredistortionTablesStart()
{
    m_mod_tx_thread.calculatePredistortionTablesStart();
}

void MainWindow::qamDecoderReset()
{
    m_qam_thread.SetFirstPassFlag();
}

void MainWindow::setAutoCfgStatus(quint8 status)
{
    m_message_box->setStatusAutoCfgPredistortion(status);
}

bool MainWindow::frame_builded_cb(const void *param, const uint8_t *buffer, uint32_t size)
{
    MainWindow *self = (MainWindow*)param;

    if(!self)
        return false;

    self->transmitDataSerialPort(buffer, size);

    return true;
}
