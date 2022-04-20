#include "qam_thread.h"
#include "usb_global.h"
#include "qam_decoder/qam_decoder.h"
#include "qam_decoder/HS_EWL_FREQ_ACQ.h"
#include "qam_decoder/HS_EWL_RECEIVE.h"
#include "qam_decoder/HS_EWL_RECEIVE_emxAPI.h"
#include "qam_decoder/HS_EWL_RECEIVE_terminate.h"
#include "qam_decoder/HS_EWL_RECEIVE_types.h"
#include "qam_decoder/rt_nonfinite.h"
#include "qam_decoder/signal.h"
#include "crc16.h"

/* Extern global variables */
extern RingBuffer *m_ring;              // ring data buffer (ADC data) for QAM decoder
extern QWaitCondition ringNotEmpty;
extern QMutex m_mutex;

static double Signal[USB_MAX_DATA_SIZE];

static double Fs = 1832061;//280000;//ADC sample rate
static double f0 = 35000;//carrier freq
static double sps = qRound(Fs/f0);//sample per symbol
static double mode = 1;//1-both stages enabled, 0-only second stage
static double preamble_len = 20;//preamble length
static double message_len = 255;
static double QAM_order = 256;
static double preamble_QAM_symbol = 128;//QAM symbol used in preamble

creal_T qam_symbols_data[255];
int qam_symbols_size;
double byte_data[255];
int byte_data_size;

double f_est_data;//estimated frequency
int f_est_size;

static const uint32_t TxPacketDataSize = 212;
static const uint32_t TxPacketDataOffset = 23;
uint8_t data_decoded[TxPacketDataSize];


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
    static bool first_pass = true;

    emit consolePutData(QString("QAM decoder starting, length %1\n").arg(Length), 1);

    //double len = SIG_LEN;
    //double *signal = (double*)&signal_test;

    double *signal = (double*)&Signal;
    double len = 15000;

    emxArray_real_T *in_data = NULL;
    in_data = emxCreateWrapper_real_T(signal, 1, len);
    timer.start();
    HS_EWL_RECEIVE(in_data, len, Fs,
                    f0, sps, mode, preamble_len,
                    message_len, QAM_order, preamble_QAM_symbol,
                    qam_symbols_data, *(int(*)[1]) & qam_symbols_size, byte_data, *(int(*)[1]) & byte_data_size,
                    (double *)&f_est_data,
                    *(int(*)[1]) & f_est_size);

    if(first_pass)
    {
        first_pass = false;
        f0 = f_est_data;
        mode = 0;
    }

//    qDebug() << "freq = " << f_est_data << "Hz" << "elapsed time = " << timer.elapsed() << "ms";
//    for(int i=0;i<byte_data_size;i++){
//        qDebug() << byte_data[i] - byte_array[i];
//    }
//    //    qDebug() << "byte_data[" <<0<< "]= " << byte_data_size;
//    qDebug() << "re_qam_symbol[" << qam_symbols_size-1 << "] =" << qam_symbols_data[qam_symbols_size-1].re;
//    qDebug() << "im_qam_symbol[" << qam_symbols_size-1 << "] =" << qam_symbols_data[qam_symbols_size-1].im;

    // Convert decoded data from 'double' to 'uint8'
    for(uint8_t i = 0; i < TxPacketDataSize; ++i)
        //data_decoded[i] = (uint8_t)byte_data[TxPacketDataOffset + i];
        data_decoded[i] = (uint8_t)byte_data[i];

    // Parse decoded data
    if(data_decoded[0] == MessageBox::SRP_ADDR)
    {
        uint16_t packet_length = (uint16_t)data_decoded[1] | ((uint16_t)data_decoded[2] << 8);

        //todo
        if(packet_length > 212)
            packet_length = 212;

        // Check crc
        //todo
        uint16_t crc = ((uint16_t)data_decoded[packet_length - 1] << 8)
                       | (uint16_t)data_decoded[packet_length - 2];

        bool res = check_crc16(data_decoded, packet_length - 2, crc);

        if(res)
        {
            // Transmit decoded data to PC
            emit postTxDataToSerialPort((uint8_t*)&data_decoded[3], packet_length);
        }
        else
        {
            emit consolePutData(QString("HS data parsing crc error, packet length %1\n").arg(packet_length), 1);
        }
    }

    // Debug information
    emit consolePutData(QString("freq = %1 Hz, elapsed time = %2 ms\n").arg(f_est_data).arg(timer.elapsed()), 1);

    QString s;
    for(int i = 0; i < byte_data_size; i++)
    {
        s.append(QString("%1 ").arg(byte_data[i] - byte_array[i]));
    }
    s.append("\n");
    emit consolePutData(s, 0);

    HS_EWL_RECEIVE_terminate();
}
