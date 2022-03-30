#include "qam_thread.h"

/* Extern global variables */
extern RingBuffer *m_ring;              // ring data buffer (ADC data) for QAM decoder
extern QWaitCondition ringNotEmpty;
extern QMutex m_mutex;

uint8_t AdcDataBufferQT[10*1024*1024];

QamThread::QamThread(QObject *parent) :
    QThread(parent)
{
}

QamThread::~QamThread()
{
    m_mutex.lock();
    m_quit = true;
    ringNotEmpty.wakeOne();
    m_mutex.unlock();
    wait();
}

void QamThread::run()
{
    bool res = false;

    while(!m_quit)
    {
        m_mutex.lock();
        if(!m_ring->DataAvailable())
            ringNotEmpty.wait(&m_mutex);    // Wait condition unlocks mutex before 'wait', and will lock it again just after 'wait' is complete
        m_mutex.unlock();

        res = m_ring->Get(AdcDataBufferQT, &Length);

        if(res)
        {
            QAM_Decoder();
        }
        else
        {
            emit consolePutData("Error ring buffer get returned false\n", 1);
        }
    }
}

void QamThread::QAM_Decoder()
{
    emit consolePutData(QString("QAM decoder working, length %1\n").arg(Length), 1);
    QThread::msleep(100);
}
