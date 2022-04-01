#ifndef QAM_WORKTHREAD_H
#define QAM_WORKTHREAD_H

#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include <QElapsedTimer>
#include <ringbuffer.h>

class QamThread : public QThread
{
    Q_OBJECT

signals:
    void consolePutData(const QString &data, quint8 priority);

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
};

#endif // QAM_WORKTHREAD_H
