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

#include "crc16.h"
#include "srp_mod_protocol.h"

/* Extern global variables */
extern RingBuffer *m_ring;              // ring data buffer (ADC data) for QAM decoder
extern QWaitCondition ringNotEmpty;
extern QMutex m_mutex;
extern QElapsedTimer profiler_timer;
/* Private variables */
static double Signal[USB_MAX_DATA_SIZE];

//double Fs = 1832061;//280000;//ADC sample rate
double f0 = 35000;//carrier freq
double sps = round(Fs/f0);//sample per symbol
double mode = 1;//1-both stages enabled, 0-only sevond stage
//double preamble_len = 20;//preamble length
//double message_len = 255;
//double QAM_order = 256;
//double preamble_QAM_symbol = 128;//QAM symbol used in preamble
////  input var for sweep
//double f_sine = 35000;
//double period_amount = 500;
//double sine_sps = Fs/f_sine;
//double f_pream = 2000;
//double Fs_math_sweep = 280000;
//double pream_sps = Fs_math_sweep/f_pream;
////  output var for HS_EWL_FREQ_ACQ
//double warning_status;
//double index_data;
//double len_data;
//double f_est_data;//estimated frequency
//int f_est_size;
////  output var for HS_EWL_DEMOD_QAM
//creal_T qam_symbols_data[255];
//int qam_symbols_size;
//double byte_data[255];
//int byte_data_size;
////  output var HS_EWL_FREQ_EST_FOR_SWEEP
//double f_opt;
//double ph_opt;
//double sweep_freq_warning_status;
////  output var HS_EWL_TR_FUN_EST
//double  gain_data[2048];
//double  phase_data[2048];
//int     gain_size[2];
//int     phase_size[2];
//double  sweep_warning_status;

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

////  output var HS_EWL_FREQ_EST_FOR_SWEEP
//double f_opt;
//double ph_opt;
//double sweep_freq_warning_status;

////  output var HS_EWL_TR_FUN_EST
//double  gain_data[2048];
//double  phase_data[2048];
//int     gain_size[2];
//int     phase_size[2];
//double  sweep_warning_status;


//static double Fs = 1832061;//280000;//ADC sample rate
//static double f0 = 35000;//carrier freq
//static double sps = qRound(Fs/f0);//sample per symbol
//static double mode = 1;//1-both stages enabled, 0-only second stage
//static double preamble_len = 20;//preamble length
//static double message_len = 255;
//static double QAM_order = 256;
//static double preamble_QAM_symbol = 128;//QAM symbol used in preamble
//static double warning_status;

//static creal_T qam_symbols_data[255];
//static int qam_symbols_size;
//static double byte_data[255];
//static int byte_data_size;

//static double f_est_data;//estimated frequency
//static int f_est_size;

//// QAM data related sizes & offsets
//static const uint32_t TxPacketDataSize = 212;
//static const uint32_t TxPacketDataOffset = 23;

//static uint8_t data_decoded[TxPacketDataSize];      // data decoded from single qam packet
static uint8_t data_decoded[128*1024];      // data decoded from single qam packet
//static uint8_t data_accum_buffer[128*1024];         // data buffer for storing decoded data from multiple packets
//static uint32_t data_accum_length = 0;              // data length in data_accum_buffer (for multiple packets)
static uint32_t data_offset = 0;

//bool kostyl_check_error = false;    // TODO

double ref_cos[] = {1, 9.927089e-01, 9.709418e-01, 9.350162e-01, 8.854560e-01, 8.229839e-01, 7.485107e-01, 6.631227e-01, 5.680647e-01, 4.647232e-01, 3.546049e-01, 2.393157e-01, 1.205367e-01, 6.123234e-17, -1.205367e-01, -2.393157e-01, -3.546049e-01, -4.647232e-01, -5.680647e-01, -6.631227e-01, -7.485107e-01, -8.229839e-01, -8.854560e-01, -9.350162e-01, -9.709418e-01, -9.927089e-01, -1, -9.927089e-01, -9.709418e-01, -9.350162e-01, -8.854560e-01, -8.229839e-01, -7.485107e-01, -6.631227e-01, -5.680647e-01, -4.647232e-01, -3.546049e-01, -2.393157e-01, -1.205367e-01, -1.836970e-16, 1.205367e-01, 2.393157e-01, 3.546049e-01, 4.647232e-01, 5.680647e-01, 6.631227e-01, 7.485107e-01, 8.229839e-01, 8.854560e-01, 9.350162e-01, 9.709418e-01, 9.927089e-01};
double ref_sin[] = {0, 1.205367e-01, 2.393157e-01, 3.546049e-01, 4.647232e-01, 5.680647e-01, 6.631227e-01, 7.485107e-01, 8.229839e-01, 8.854560e-01, 9.350162e-01, 9.709418e-01, 9.927089e-01, 1, 9.927089e-01, 9.709418e-01, 9.350162e-01, 8.854560e-01, 8.229839e-01, 7.485107e-01, 6.631227e-01, 5.680647e-01, 4.647232e-01, 3.546049e-01, 2.393157e-01, 1.205367e-01, 1.224647e-16, -1.205367e-01, -2.393157e-01, -3.546049e-01, -4.647232e-01, -5.680647e-01, -6.631227e-01, -7.485107e-01, -8.229839e-01, -8.854560e-01, -9.350162e-01, -9.709418e-01, -9.927089e-01, -1, -9.927089e-01, -9.709418e-01, -9.350162e-01, -8.854560e-01, -8.229839e-01, -7.485107e-01, -6.631227e-01, -5.680647e-01, -4.647232e-01, -3.546049e-01, -2.393157e-01, -1.205367e-01};



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

    HS_EWL_DEMOD_QAM(data_qam256, index_data, len_data, f_est_data,
                     Fs, sps, preamble_QAM_symbol,
                     QAM_order, qam_symbols_data, &qam_symbols_size,
                     byte_data, &byte_data_size);

//    if(kostyl_check_error)
//        warning_status = 4;

    if(first_pass)
    {
        first_pass = false;
        f0 = f_est_data;
        mode = 0;
    }

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
    if(data_decoded[0] == MessageBox::SRP_ADDR)
    {
        uint16_t packet_length = (uint16_t)data_decoded[1] | ((uint16_t)data_decoded[2] << 8);

        if(packet_length < MOD_SRP_MIN_VALID_LENGTH)
        {
            emit consolePutData(QString("HS data parsing error, packet length is too short %1\n").arg(packet_length), 1);
            packet_length = MOD_SRP_MIN_VALID_LENGTH;
        }

        data_offset += TxPacketDataSize;

        // Whole data received?
        if(data_offset >= packet_length)
        {
            data_offset = 0;

            // Check crc
            uint16_t crc = ((uint16_t)data_decoded[packet_length - 1] << 8)
                           | (uint16_t)data_decoded[packet_length - 2];

            bool res = check_crc16(data_decoded, packet_length - 2, crc);

            if(res)
            {
                // Transmit decoded data to PC
                packet_length -= MOD_SRP_PROTOCOL_HEADER_SIZE + MOD_SRP_PROTOCOL_CRC_SIZE + MASTER_ADDR_SIZE;
                emit postTxDataToSerialPort((uint8_t*)&data_decoded[MOD_SRP_PROTOCOL_HEADER_SIZE + MASTER_ADDR_SIZE], packet_length);
                //emit consolePutData(QString("Profiler timer elapsed %1 # data to serial port\n").arg(profiler_timer.elapsed()), 1);
            }
            else
            {
                emit consolePutData(QString("HS data parsing crc error, packet length %1\n").arg(packet_length), 1);
            }
        }
    }
    else
    {
        emit consolePutData(QString("HS data parsing error, addr != SRP\n"), 1);
        data_offset = 0;
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
            emit consolePutData("warning_status = 1 not enough sample in the end array\n", 1);
            break;
        case WARNING_2:
            // warning_status = 2 all or more than 0.2 array equal 0
            emit consolePutData("warning_status = 2 all or more than 0.2 array equal 0\n", 1);
            break;
        case WARNING_3:
            // warning_status = 3 start array sample equal 0 less than 0
            emit consolePutData("warning_status = 3 start array sample equal 0 less than 0\n", 1);
            break;
        case WARNING_4:
            // warning_status = 4 kostyl check error
            emit consolePutData("warning_status = 4 kostyl check error\n", 1);
            break;
    };

    emit consolePutData(QString("freq = %1 Hz, elapsed time = %2 ms\n").arg(f_est_data).arg(peformance_timer.elapsed()), 1);

//    QString s;
//    for(int i = 0; i < byte_data_size; i++)
//    {
//        s.append(QString("%1 ").arg(byte_data[i] - byte_array[i]));
//    }
//    s.append("\n");
//    emit consolePutData(s, 0);

    //HS_EWL_RECEIVE_terminate();
}
