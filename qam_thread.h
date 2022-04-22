#ifndef QAM_WORKTHREAD_H
#define QAM_WORKTHREAD_H

#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include <QElapsedTimer>
#include <ringbuffer.h>
#include <message_box.h>

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

private:
    void run() override;
    void QAM_Decoder();

    static const uint32_t DATA_SIZE = (256*1024);

//    uint8_t AdcDataBuffer[DATA_SIZE];
//    uint32_t Length = 0;
    double Length;

    bool m_quit = false;

    QElapsedTimer timer;

    enum {
        CORRECT = 0,    // warning_satus = 0 input array correct
        WARNING_1 = 1,  // warning_satus = 1 not enough sample in the end array
        WARNING_2 = 2,  // warning_satus = 2 all or more than 0.2 array equal 0
        WARNING_3 = 3,  // warning_satus = 3 start array sample equal 0 less than 0.2
        WARNING_4 = 4,  // warning_satus = 4 kostyl check failed
    } HsEwlReceive_WarningStatus;
};

#endif // QAM_WORKTHREAD_H
