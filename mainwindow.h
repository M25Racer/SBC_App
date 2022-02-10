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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <message_box.h>
#include <usb_workthread.h>

QT_BEGIN_NAMESPACE

class QLabel;

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class Console;
class SettingsDialog;
class MessageBox;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    bool openSerialPort();
    void closeSerialPort();
    void about();
    void readDataSerialPort();
    void transmitDataSerialPort(const uint8_t *p_data, const int length);
    void postTxDataSTM(const uint8_t *p_data, const int length);
    void handleError(QSerialPort::SerialPortError error);
    void consolePutData(const QString &data, quint8 priority);
    void timeoutSerialPortRx();
    void timeoutSerialPortReconnect();
    void timeoutUsbInitCallback();
    void usbInitTimeoutStart(const int timeout_ms);

private:
    void initActionsConnections();

private:
    void showStatusMessage(const QString &message);
    void serialPortRxCleanup();

    Ui::MainWindow *m_ui = nullptr;
    QLabel *m_status = nullptr;

    Console *m_console = nullptr;
    QSerialPort *m_serial = nullptr;
    MessageBox *m_message_box = nullptr;
    UsbWorkThread m_usb_thread;

    QByteArray TtyUserRxBuffer;

    quint64 TtyUserRxBuffer_MaxSize = 4096;
    quint64 TtyUserRxBuffer_len = 0;

    QTimer *timerRxTimeout;
    QTimer *timerReconnect;
    QTimer *timerUsbPoll;                               // todo Probably we should move USB to separate thread
    QTimer *timerUsbInit;
    bool timeoutSerialPort = false;

    // const int timeoutSerialPortRx_ms = 1;//10;              // timeout for serial port receiver (max time between rx packets)
    const int timeoutSerialPortReconnect_ms = 1000;

    uint8_t quick_answer[2] = {0x00, 0x1d};
    bool transmit_quick_answer = false;

    struct timeval tv = {0, 0};
};

#endif // MAINWINDOW_H
