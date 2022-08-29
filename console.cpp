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

#include "console.h"

#include <QScrollBar>
#include <QDesktopServices>
#include <QFileInfo>
//#include <QDir>

Console::Console(QWidget *parent) :
    QPlainTextEdit(parent)
{
    document()->setMaximumBlockCount(100);
    QPalette p = palette();
//    p.setColor(QPalette::Base, Qt::lightGray);
//    p.setColor(QPalette::Text, Qt::black);
    p.setColor(QPalette::Base, Qt::GlobalColor::black);
    p.setColor(QPalette::Text, QColor(255,99,71));
    setPalette(p);

    // Set the logging file
    m_logFile.reset(new QFile("SBC_log_"+ QDateTime::currentDateTime().toString("yyyy-MM-dd_hh.mm.ss.zzz") + ".txt"));
    // Open the file logging
    m_logFile.data()->open(QFile::Append | QFile::Text);

    // Open stream file writes
    out.setDevice(m_logFile.data());

#ifdef QT_DEBUG
    // Set the adc data file
    for(uint8_t i = 0; i < 20; ++i)
    {
        m_adcFile[i].reset(new QFile("SBC_adc_data" + QString::number(i) + "_" + QDateTime::currentDateTime().toString("yyyy-MM-dd_hh.mm.ss.zzz") + ".txt"));
        // Open the file logging
        m_adcFile[i].data()->open(QFile::Append | QFile::Text);

        // Open stream file writes
        outAdc[i].setDevice(m_adcFile[i].data());
    }
#endif
    // Set the adc data file for frame errors
    m_frameErrorFile.reset(new QFile("Frame_errors_adc_data_" + QDateTime::currentDateTime().toString("yyyy-MM-dd_hh.mm.ss.zzz") + ".txt"));
    // Open the file logging
    m_frameErrorFile.data()->open(QFile::Append | QFile::Text);

    // Open stream file writes
    outFrameErrorAdc.setDevice(m_frameErrorFile.data());
}

void Console::putData(const QString &data, uint8_t priority)
{
#ifdef QT_DEBUG
    ++priority;
#endif

    if(priority)
    {
        insertPlainText(data);

        QScrollBar *bar = verticalScrollBar();
        bar->setValue(bar->maximum());
    }

    // Log to file

    // Write the date of recording
    out << QDateTime::currentDateTime().toString("hh:mm:ss.zzz ");

    // Write to the output category of the message and the message itself
    out << data;// << endl;
    //out.flush();    // Clear the buffered data
}

void Console::putDataAdc(const quint8 *p_data, quint32 size)
{
    QString data;
    uint16_t value;

    for(uint64_t i = 0; i < size; i+=2)
    {
        value = p_data[i];
        value |= ((uint16_t)p_data[i+1]) << 8;

        data.append(QString("%1\n").arg((int16_t)value));
    }

    // Choose adc data file to write to

    m_adcFile[n_file]->resize(0);   // Clear file
    outAdc[n_file] << data;
    outAdc[n_file].flush();    // Clear the buffered data

    if(++n_file >= 20)
        n_file = 0;
}

void Console::putFrameErrorData(const qint16 *p_data, quint32 len)
{
    QString data;

    data.append(QString("%1 // =================================== Frame #%2, len %3 ===================================\n").arg((int16_t)p_data[0]).arg(n_frame++).arg(len));

    for(uint64_t i = 1; i < len; ++i)
    {
        data.append(QString("%1\n").arg((int16_t)p_data[i]));
    }

    //m_frameErrorFile->resize(0);    // Clear file
    outFrameErrorAdc << data;
    outFrameErrorAdc.flush();       // Clear the buffered data
}

void Console::fileFlush()
{
    putData("Force log file flush by user\n", 1);
    out.flush();    // Clear the buffered data
}

void Console::fileOpen()
{
    // Find file path
    QFileInfo info;
    info.setFile(m_logFile->fileName());
    info.makeAbsolute();

    putData(QString("Opening log file: ") + info.filePath() + QString("\n"), 1);

    // Flush before open
    fileFlush();

    // Open file with external program
    QDesktopServices::openUrl(QUrl::fromLocalFile(info.filePath()));
}

void Console::Close()
{
    out.flush();                    // Clear the buffered data
    m_logFile->close();             // Close file

#ifdef QT_DEBUG
    for(uint8_t i = 0; i < 20; ++i)
    {
        outAdc[i].flush();          // Clear the buffered data
        m_adcFile[i]->close();      // Close file
    }
#endif

    outFrameErrorAdc.flush();       // Clear the buffered data
    m_frameErrorFile->close();      // Close file
}
