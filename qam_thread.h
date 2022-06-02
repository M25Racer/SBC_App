#ifndef QAM_WORKTHREAD_H
#define QAM_WORKTHREAD_H

#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include <QElapsedTimer>
#include <ringbuffer.h>
#include <message_box.h>
#include "qam_common.h"

using namespace QAM_Common;

class MessageBox;

class QamThread : public QThread
{
    Q_OBJECT

signals:
    void consolePutData(const QString &data, quint8 priority);
    void postTxDataToSerialPort(const uint8_t *p_data, int len);

public slots:

public:
    explicit QamThread(QObject *parent = nullptr);
    ~QamThread();

    bool m_quit = false;

private:
    void run() override;
    void QAM_Decoder();

    static const uint32_t DATA_SIZE = (256*1024);

//    uint8_t AdcDataBuffer[DATA_SIZE];
//    uint32_t Length = 0;
    double Length;

    QElapsedTimer peformance_timer;     // timer for QAM decoder performance measurements
    QElapsedTimer data_timeout_tim;     // data receive timeout timer fro multi frame\packet QAM transfers

    const qint64 qam_rx_timeout_ms = 100;// rx timeout between QAM packets when decoding multi frame packet, ms

    enum {
        CORRECT = 0,    // warning_satus = 0 input array correct
        WARNING_1 = 1,  // warning_satus = 1 not enough sample in the end array
        WARNING_2 = 2,  // warning_satus = 2 all or more than 0.2 array equal 0
        WARNING_3 = 3,  // warning_satus = 3 start array sample equal 0 less than 0.2
        WARNING_4 = 4,  // warning_satus = 4 kostyl check failed
    } HsEwlReceive_WarningStatus;

    // QAM data related sizes & offsets
    static const uint32_t TxPacketDataSize = 212;
    static const uint32_t TxPacketDataOffset = 23;

    bool m_QamDecoderFirstPassFlag = true;
};

#endif // QAM_WORKTHREAD_H
