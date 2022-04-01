#include "qam_thread.h"
#include "usb_global.h"
#include "qam_decoder/qam_decoder.h"
#include "qam_decoder/HS_EWL_FREQ_ACQ.h"
#include "qam_decoder/HS_EWL_FREQ_ACQ_emxAPI.h"
#include "qam_decoder/HS_EWL_FREQ_ACQ_terminate.h"
#include "qam_decoder/HS_EWL_FREQ_ACQ_types.h"
#include "qam_decoder/rt_nonfinite.h"

/* Extern global variables */
extern RingBuffer *m_ring;              // ring data buffer (ADC data) for QAM decoder
extern QWaitCondition ringNotEmpty;
extern QMutex m_mutex;

//uint8_t AdcDataBufferQT[USB_MAX_DATA_SIZE];
static double Signal[USB_MAX_DATA_SIZE];

static double Fs = 1832061.0687022900763358778625954; //280000;//ADC sample rate
//Fs = 1,832,061.0687022900763358778625954
static double f0 = 35000;//carrier freq
static double mode = 1;//1-both stages enabled, 0-only sevond stage
static double preamble_len = 20;//preamble length
static double message_len = 255;
static double QAM_order = 256;
static double preamble_QAM_symbol = 128;//QAM symbol used in preamble
static double f_est_data;//estimated frequency
static int f_est_size;


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

        //res = m_ring->Get(AdcDataBufferQT, &Length);
        res = m_ring->GetDouble(Signal, &Length);

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
    emit consolePutData(QString("QAM decoder starting, length %1\n").arg(Length), 1);
    //    double len = SIG_LEN;

    double *signal = (double*)&Signal;
    double len = Length;

    emxArray_real_T *in_data = NULL;
    in_data = emxCreateWrapper_real_T(signal, 1, len);

    timer.start();
    HS_EWL_FREQ_ACQ(in_data, len, Fs,
                    f0, mode, preamble_len,
                    message_len, QAM_order, preamble_QAM_symbol,
                    (double *)&f_est_data,
                    *(int(*)[1]) & f_est_size);
    //qDebug() << "freq = " << f_est_data << "Hz" << "elapsed time = " << timer.elapsed() << "ms";
    emit consolePutData(QString("freq = %1 Hz, elapsed time = %2 ms\n").arg(f_est_data).arg(timer.elapsed()), 1);
}
