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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow),
    m_status(new QLabel),
    m_console(new Console)
{
    m_ui->setupUi(this);
    m_console->setEnabled(false);
    m_console->setReadOnly(true);
    setCentralWidget(m_console);

    //m_ui->actionConnect->setEnabled(true);
    //m_ui->actionDisconnect->setEnabled(false);
    //m_ui->actionQuit->setEnabled(true);
    //m_ui->actionConfigure->setEnabled(true);

    m_ui->statusBar->addWidget(m_status);

    initActionsConnections();

    connect(&m_usb_thread, &UsbWorkThread::consolePutData, this, &MainWindow::consolePutData, Qt::ConnectionType::QueuedConnection);
    connect(&m_usb_thread, &UsbWorkThread::usbInitTimeoutStart, this, &MainWindow::usbInitTimeoutStart, Qt::ConnectionType::QueuedConnection);

    m_console->putData("SBC Application\n", 1);

    // Timers
    // Serial port rx timeout timer
//    timerRxTimeout = new QTimer();
//    timerRxTimeout->setSingleShot(true);
//    connect(timerRxTimeout, SIGNAL(timeout()), this, SLOT(timeoutSerialPortRx()));

    // Usb initialization timer
    timerUsbInit = new QTimer();
    timerUsbInit->setSingleShot(true);
    connect(timerUsbInit, SIGNAL(timeout()), this, SLOT(timeoutUsbInitCallback()));

    // LibUsb initialization, init usb device
    /* Get libusb version */
    const struct libusb_version *version;
    version = libusb_get_version();
    m_console->putData(QString("libusb version: %1.%2.%3.%4\n")
                       .arg(version->major).arg(version->minor)
                       .arg(version->micro).arg(version->nano), 1);

    // Start timer for continuous usb initialization attempts
    timerUsbInit->start(m_usb_thread.timeoutUsbInit_ms);
}

MainWindow::~MainWindow()
{
    timerUsbInit->stop();

    m_usb_thread.closeSerialPort();
    m_usb_thread.end();

    m_console->Close();
    delete m_ui;
}

void MainWindow::consolePutData(const QString &data, quint8 priority)
{
    if(m_console == nullptr)
        return;

    m_console->putData(data, priority);
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About SBC Application"),
                       tr("The <b>SBC Application</b> example demonstrates how to "
                          "use the Qt Serial Port module in modern GUI applications "
                          "using Qt, with a menu bar, toolbars, and a status bar."));
}

void MainWindow::timeoutUsbInitCallback()
{
    m_usb_thread.USB_Init();

    if(m_usb_thread.s == UsbWorkThread::INIT_END)
    {
        // Start LibUSB polling thread
        m_usb_thread.USB_ThreadStart();
    }
}

void MainWindow::usbInitTimeoutStart(const int timeout_ms)
{
    timerUsbInit->start(timeout_ms);
}

void MainWindow::initActionsConnections()
{
    connect(m_ui->actionAbout, &QAction::triggered, this, &MainWindow::about);
    connect(m_ui->actionAboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);
}

void MainWindow::showStatusMessage(const QString &message)
{
    m_status->setText(message);
}
