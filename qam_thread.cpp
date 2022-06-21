#include "qam_thread.h"
#include "usb_global.h"
#include "qam_decoder/qam_decoder.h"
#include "qam_decoder/HS_EWL_DEMOD_QAM.h"
#include "qam_decoder/HS_EWL_DEMOD_QAM_emxAPI.h"
#include "qam_decoder/HS_EWL_DEMOD_QAM_terminate.h"
#include "qam_decoder/HS_EWL_DEMOD_QAM_types.h"
#include "qam_decoder/HS_EWL_FREQ_ACQ.h"
#include "qam_decoder/HS_EWL_FREQ_EST_FOR_SWEEP.h"
#include "qam_decoder/HS_EWL_TR_FUN_EST.h"
#include <QElapsedTimer>
#include "qam_decoder/signal_sweep.h"
#include "qam_decoder/signal.h"
//#include "qam_decoder/math_sweep.h"
#include "qam_decoder/rt_nonfinite.h"

#include "crc8.h"
#include "crc16.h"
#include "srp_mod_protocol.h"

/* Extern global variables */
extern RingBuffer *m_ring;              // ring data buffer (ADC data) for QAM decoder
extern QWaitCondition ringNotEmpty;
extern QMutex m_mutex_ring;
extern QElapsedTimer profiler_timer;
/* Private variables */
static double Signal[USB_MAX_DATA_SIZE];

//double Fs = 1832061;//280000;//ADC sample rate
double f0 = 35000;//carrier freq
double sps = round(Fs/f0);//sample per symbol
double mode = 1;//1-both stages enabled, 0-only sevond stage

//  output var for HS_EWL_FREQ_ACQ
double warning_status;
double index_data;
double len_data;
double f_est_data;//estimated frequency
int f_est_size;

//  output var for HS_EWL_DEMOD_QAM
creal_T qam_symbols_data[255];
int qam_symbols_size;
double byte_data[255];
int byte_data_size;

//static uint8_t data_decoded[TxPacketDataSize];      // data decoded from single qam packet
static uint8_t data_decoded[128*1024];      // data decoded from single qam packet
//static uint8_t data_accum_buffer[128*1024];         // data buffer for storing decoded data from multiple packets
//static uint32_t data_accum_length = 0;              // data length in data_accum_buffer (for multiple packets)
static uint32_t data_offset = 0;

double ref_cos[] = {1, 9.927089e-01, 9.709418e-01, 9.350162e-01, 8.854560e-01, 8.229839e-01, 7.485107e-01, 6.631227e-01, 5.680647e-01, 4.647232e-01, 3.546049e-01, 2.393157e-01, 1.205367e-01, 6.123234e-17, -1.205367e-01, -2.393157e-01, -3.546049e-01, -4.647232e-01, -5.680647e-01, -6.631227e-01, -7.485107e-01, -8.229839e-01, -8.854560e-01, -9.350162e-01, -9.709418e-01, -9.927089e-01, -1, -9.927089e-01, -9.709418e-01, -9.350162e-01, -8.854560e-01, -8.229839e-01, -7.485107e-01, -6.631227e-01, -5.680647e-01, -4.647232e-01, -3.546049e-01, -2.393157e-01, -1.205367e-01, -1.836970e-16, 1.205367e-01, 2.393157e-01, 3.546049e-01, 4.647232e-01, 5.680647e-01, 6.631227e-01, 7.485107e-01, 8.229839e-01, 8.854560e-01, 9.350162e-01, 9.709418e-01, 9.927089e-01};
double ref_sin[] = {0, 1.205367e-01, 2.393157e-01, 3.546049e-01, 4.647232e-01, 5.680647e-01, 6.631227e-01, 7.485107e-01, 8.229839e-01, 8.854560e-01, 9.350162e-01, 9.709418e-01, 9.927089e-01, 1, 9.927089e-01, 9.709418e-01, 9.350162e-01, 8.854560e-01, 8.229839e-01, 7.485107e-01, 6.631227e-01, 5.680647e-01, 4.647232e-01, 3.546049e-01, 2.393157e-01, 1.205367e-01, 1.224647e-16, -1.205367e-01, -2.393157e-01, -3.546049e-01, -4.647232e-01, -5.680647e-01, -6.631227e-01, -7.485107e-01, -8.229839e-01, -8.854560e-01, -9.350162e-01, -9.709418e-01, -9.927089e-01, -1, -9.927089e-01, -9.709418e-01, -9.350162e-01, -8.854560e-01, -8.229839e-01, -7.485107e-01, -6.631227e-01, -5.680647e-01, -4.647232e-01, -3.546049e-01, -2.393157e-01, -1.205367e-01};

// Debug variables
qint64 elapsed_all = 0;
qint64 elapsed_all_saved = 0;

QamThread::QamThread(QObject *parent) :
    QThread(parent)
{
}

QamThread::~QamThread()
{
    m_mutex_ring.lock();
    m_quit = true;
    ringNotEmpty.wakeOne();
    m_mutex_ring.unlock();
    wait();
}

void QamThread::run()
{
    bool res = false;

    while(!m_quit)
    {
        m_mutex_ring.lock();
        if(!m_ring->DataAvailable())
            ringNotEmpty.wait(&m_mutex_ring);    // Wait condition unlocks mutex before 'wait', and will lock it again just after 'wait' is complete
        m_mutex_ring.unlock();

        res = m_ring->GetDouble(Signal, &Length);

        if(!res)
        {
            emit consolePutData("Error ring buffer get() returned false\n", 1);
            continue;
        }

        if(data_drop)
        {
            data_offset = 0;
            emit consolePutData("Dropping received QAM data\n", 1);
            continue;
        }

        QAM_Decoder();
    }
}

void QamThread::SetFirstPassFlag()
{
    emit consolePutData("QAM decoder reset (calcluate and save new frequency next time)\n", 1);
    mutex.lock();
    m_QamDecoderFirstPassFlag = true;
    mutex.unlock();
}

void QamThread::QAM_Decoder()
{
    double *signal = (double*)&Signal;
    double len = Length;

//    for(uint32_t i = 0; i < 50000; ++i)
//        Signal[i] = i;

    peformance_timer.start();
    emxArray_real_T *data_qam256 = NULL;
    data_qam256 = emxCreateWrapper_real_T(signal, 1, len);

    HS_EWL_FREQ_ACQ(data_qam256, len, Fs, f0, sps, mode, preamble_len,
                      message_len, QAM_order, preamble_QAM_symbol, &index_data, &len_data, (double *)&f_est_data,
                      *(int(*)[1]) & f_est_size, &warning_status);

    if(warning_status != 4)
    {
        HS_EWL_DEMOD_QAM(data_qam256, index_data, len_data, f_est_data,
                     Fs, sps, preamble_QAM_symbol,
                     QAM_order, qam_symbols_data, &qam_symbols_size,
                     byte_data, &byte_data_size);

        // Data parsing
        // If receive timeout elapsed
        if(data_timeout_tim.hasExpired(qam_rx_timeout_ms))
        {
            if(data_offset != 0)
                emit consolePutData(QString("Error: HS multiframe rx timeout expired\n"), 1);
            data_offset = 0;
        }

        // Restart receive timeout
        data_timeout_tim.restart();

        if(data_offset + TxPacketDataSize > sizeof(data_decoded))
        {
            emit consolePutData(QString("Error: HS too long continuous data received %1\n").arg(data_offset), 1);
            data_offset = 0;
        }

        // Convert decoded data from 'double' to 'uint8', copy to data_decoded[] buffer
        for(uint8_t i = 0; i < TxPacketDataSize; ++i)
            data_decoded[data_offset + i] = (uint8_t)byte_data[i];

        // Parse decoded data
        // Parse 'frame' tail
        frame_tail_nlast_t *tail = (frame_tail_nlast_t*)&data_decoded[data_offset + TxPacketDataSize - sizeof(frame_tail_nlast_t)];

        // Check 'frame' CRC
        uint8_t crc8 = calc_crc8((uint8_t*)&data_decoded[data_offset], TxPacketDataSize - 1);

        if(crc8 != tail->crc8)
        {
            emit consolePutData(QString("HS frame parsing crc error\n"), 1);
            data_offset = 0;
        }
        else
        {
            if(tail->frame_flag == ENUM_FRAME_NOT_LAST)
            {
                data_offset += TxPacketDataSize - sizeof(frame_tail_nlast_t);
//                emit consolePutData(QString("Not last frame #%1, continue receive\n").arg(tail->frame_id), 1);
            }
            else // tail->frame_flag == ENUM_FRAME_LAST
            {
                frame_tail_last_t *tail_last = (frame_tail_last_t*)&data_decoded[data_offset + TxPacketDataSize - sizeof(frame_tail_last_t)];

                // Check data length
                if(tail_last->len < MOD_SRP_MIN_VALID_LENGTH_NEW)
                {
                    // Error length is too short
                    emit consolePutData(QString("HS data parsing error, packet data length is too short %1\n").arg(tail_last->len), 1);
                }
                else if(tail_last->len > (data_offset + TxPacketDataSize - sizeof(frame_tail_nlast_t)))
                {
                    // Error length is too long
                    emit consolePutData(QString("HS data parsing error, packet data length is too long %1\n").arg(tail_last->len), 1);
                }
                else
                {
                    // Transmit decoded data to PC
                    emit postTxDataToSerialPort((uint8_t*)&data_decoded[MASTER_ADDR_SIZE], tail_last->len - MASTER_ADDR_SIZE);
                }

                emit consolePutData(QString("Last frame #%1, data length %2\n").arg(tail_last->tail.frame_id).arg(tail_last->len), 1);

                // Reset offset
                data_offset = 0;
            }

            mutex.lock();
            if(m_QamDecoderFirstPassFlag)
            {
                m_QamDecoderFirstPassFlag = false;
                f0 = f_est_data;
                mode = 0;
            }
            mutex.unlock();
        }
    }

    // Debug information
    int r = int(warning_status);

    switch(r)
    {
        case CORRECT:
            // warning_status = 0 input array correct
            break;
        case WARNING_1:
            // warning_status = 1 not enough sample in the end array
            emit consolePutData("warning_status = 1: not enough sample in the end array\n", 1);
            break;
        case WARNING_2:
            // warning_status = 2 all or more than 0.2 array equal 0
            emit consolePutData("warning_status = 2: all or more than 0.2 array equal 0\n", 1);
            break;
        case WARNING_3:
            // warning_status = 3 start array sample equal 0 less than 0
            emit consolePutData("warning_status = 3: start array sample equal 0 less than 0\n", 1);
            break;
        case WARNING_4:
            // warning_status = 4 f_est == 0
            emit consolePutData("warning_status = 4: error probably wrong f_est\n", 1);
            break;
    };

    // Debug
    qint64 elapsed = peformance_timer.elapsed();
    elapsed_all += elapsed;

    emit consolePutData(QString("QAM decoder finished for length %1: freq = %2 Hz, elapsed time = %3 ms\n").arg(Length).arg(f_est_data).arg(elapsed), 1);

    if(data_offset == 0)
    {
        emit consolePutData(QString("Elapsed all qam = %1\n").arg(elapsed_all), 1);
        elapsed_all_saved = elapsed_all;
        elapsed_all = 0;
    }

//    QString s;
//    for(int i = 0; i < byte_data_size; i++)
//    {
//        s.append(QString("%1 ").arg(byte_data[i] - byte_array[i]));
//    }
//    s.append("\n");
//    emit consolePutData(s, 0);

//    HS_EWL_DEMOD_QAM_terminate();
}
