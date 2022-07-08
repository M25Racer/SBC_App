#include "qam_thread.h"
#include "usb_global.h"
//#include "qam_decoder/qam_decoder.h"
#include "qam_decoder/HS_EWL_DEMOD_QAM.h"
//#include "qam_decoder/HS_EWL_DEMOD_QAM_emxAPI.h"
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
//#include "rtwtypes.h"
//#include "omp.h"

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
static int16_t FrameErrorAdcBuffer[10][USB_MAX_DATA_SIZE];

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
double qam_symbols_data_real[275];
double qam_symbols_data_imag[275];
creal_T qam_symbols_data[255];
int qam_symbols_size;
double byte_data[255];
int byte_data_size;

static uint8_t data_decoded[128*1024];      // data decoded from all qam packet

double ref_sin[] = { 0, 1.205367e-01, 2.393157e-01, 3.546049e-01, 4.647232e-01, 5.680647e-01, 6.631227e-01, 7.485107e-01, 8.229839e-01, 8.854560e-01, 9.350162e-01, 9.709418e-01, 9.927089e-01, 1, 9.927089e-01, 9.709418e-01, 9.350162e-01, 8.854560e-01, 8.229839e-01, 7.485107e-01, 6.631227e-01, 5.680647e-01, 4.647232e-01, 3.546049e-01, 2.393157e-01, 1.205367e-01, 1.224647e-16, -1.205367e-01, -2.393157e-01, -3.546049e-01, -4.647232e-01, -5.680647e-01, -6.631227e-01, -7.485107e-01, -8.229839e-01, -8.854560e-01, -9.350162e-01, -9.709418e-01, -9.927089e-01, -1, -9.927089e-01, -9.709418e-01, -9.350162e-01, -8.854560e-01, -8.229839e-01, -7.485107e-01, -6.631227e-01, -5.680647e-01, -4.647232e-01, -3.546049e-01, -2.393157e-01, -1.205367e-01 };
double ref_cos[] = { 1, 9.927089e-01, 9.709418e-01, 9.350162e-01, 8.854560e-01, 8.229839e-01, 7.485107e-01, 6.631227e-01, 5.680647e-01, 4.647232e-01, 3.546049e-01, 2.393157e-01, 1.205367e-01, 6.123234e-17, -1.205367e-01, -2.393157e-01, -3.546049e-01, -4.647232e-01, -5.680647e-01, -6.631227e-01, -7.485107e-01, -8.229839e-01, -8.854560e-01, -9.350162e-01, -9.709418e-01, -9.927089e-01, -1, -9.927089e-01, -9.709418e-01, -9.350162e-01, -8.854560e-01, -8.229839e-01, -7.485107e-01, -6.631227e-01, -5.680647e-01, -4.647232e-01, -3.546049e-01, -2.393157e-01, -1.205367e-01, -1.836970e-16, 1.205367e-01, 2.393157e-01, 3.546049e-01, 4.647232e-01, 5.680647e-01, 6.631227e-01, 7.485107e-01, 8.229839e-01, 8.854560e-01, 9.350162e-01, 9.709418e-01, 9.927089e-01 };

// Debug variables
qint64 elapsed_all = 0;
qint64 elapsed_all_saved = 0;

// Function Declarations
static void argInit_1x14000_real_T(double result[14000]);
static void argInit_1x33000_real_T(double result[33000]);
static void argInit_1x450000_real_T(double result[450000]);
static void argInit_1x57820_real_T(double result[57820]);
static double argInit_real_T();
static void main_HS_EWL_DEMOD_QAM();
static void main_HS_EWL_FREQ_ACQ();
static void main_HS_EWL_FREQ_EST_FOR_SWEEP();
static void main_HS_EWL_TR_FUN_EST();

// Function Definitions
//
// Arguments    : double result[14000]
// Return Type  : void
//
static void argInit_1x14000_real_T(double result[14000])
{
  // Loop over the array to initialize each element.
  for (int idx1 = 0; idx1 < 14000; idx1++) {
    // Set the value of the array element.
    // Change this value to the value that the application requires.
    result[idx1] = argInit_real_T();
  }
}

//
// Arguments    : double result[33000]
// Return Type  : void
//
static void argInit_1x33000_real_T(double result[33000])
{
  // Loop over the array to initialize each element.
  for (int idx1 = 0; idx1 < 33000; idx1++) {
    // Set the value of the array element.
    // Change this value to the value that the application requires.
    result[idx1] = argInit_real_T();
  }
}

//
// Arguments    : double result[450000]
// Return Type  : void
//
static void argInit_1x450000_real_T(double result[450000])
{
  // Loop over the array to initialize each element.
  for (int idx1 = 0; idx1 < 450000; idx1++) {
    // Set the value of the array element.
    // Change this value to the value that the application requires.
    result[idx1] = argInit_real_T();
  }
}

//
// Arguments    : double result[57820]
// Return Type  : void
//
static void argInit_1x57820_real_T(double result[57820])
{
  // Loop over the array to initialize each element.
  for (int idx1 = 0; idx1 < 57820; idx1++) {
    // Set the value of the array element.
    // Change this value to the value that the application requires.
    result[idx1] = argInit_real_T();
  }
}

//
// Arguments    : void
// Return Type  : double
//
static double argInit_real_T()
{
  return 0.0;
}

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
    mode = 1;
    mutex.unlock();
}

void QamThread::QAM_Decoder()
{
    bool crc_error = false;
    bool last_frame_received = false;
    double *signal = (double*)&Signal;
    double len = Length;

//    for(uint32_t i = 0; i < 50000; ++i)
//        Signal[i] = i;

    peformance_timer.start();
//    emxArray_real_T *data_qam256 = NULL;
//    data_qam256 = emxCreateWrapper_real_T(signal, 1, len);

    HS_EWL_FREQ_ACQ(signal, len, Fs, f0, sps, mode, preamble_len,
                message_len, QAM_order, preamble_QAM_symbol, &index_data, &len_data, (double*)&f_est_data,
                &warning_status);

//    HS_EWL_FREQ_ACQ(data_qam256, len, Fs, f0, sps, mode, preamble_len,
//                      message_len, QAM_order, preamble_QAM_symbol, &index_data, &len_data, (double *)&f_est_data,
//                      *(int(*)[1]) & f_est_size, &warning_status);

    if(warning_status != 4)
    {
//        HS_EWL_DEMOD_QAM(data_qam256, index_data, len_data, f_est_data,
//                     Fs, sps, preamble_QAM_symbol,
//                     QAM_order, qam_symbols_data, &qam_symbols_size,
//                     byte_data, &byte_data_size);

        HS_EWL_DEMOD_QAM(signal, len, index_data, len_data, f_est_data,
            Fs, sps, preamble_QAM_symbol,
            QAM_order, qam_symbols_data_real, qam_symbols_data_imag,
            byte_data);

        // Data parsing
        // Convert decoded data from 'double' to 'uint8', copy to data_decoded[] buffer
        for(uint8_t i = 0; i < TxPacketDataSize; ++i)
            frame_decoded[i] = (uint8_t)byte_data[i];

        // Parse 'frame' tail
        frame_tail_nlast_t *tail = (frame_tail_nlast_t*)&frame_decoded[TxPacketDataSize - sizeof(frame_tail_nlast_t)];

        // Check 'frame' CRC
        uint8_t crc8 = calc_crc8(frame_decoded, TxPacketDataSize - 1);

        if(crc8 != tail->crc8)
        {
            crc_error = true;
            emit consolePutData(QString("HS frame parsing crc error\n"), 1);
        }
        else
        {
            crc_error = false;

            const uint32_t data_size_not_last_frame = TxPacketDataSize - sizeof(frame_tail_nlast_t);    // valuable data in 'not last' frame
            const uint32_t data_size_last_frame = TxPacketDataSize - sizeof(frame_tail_last_t);         // valuable data in 'last' frame

            // Not last frame
            if(tail->frame_flag == ENUM_FRAME_NOT_LAST)
            {
                //emit consolePutData(QString("Not last frame #%1, continue receive\n").arg(tail->frame_id), 1);

                if(sizeof(data_decoded) < tail->frame_id * data_size_not_last_frame + data_size_not_last_frame)
                {
                    emit consolePutData(QString("Error wrong frame number %1 or too too long continuous data receive\n").arg(tail->frame_id), 1);
                }
                else
                {
                    // Copy decoded frame to data_decoded buffer
                    for(uint8_t i = 0; i < data_size_not_last_frame; ++i)
                        data_decoded[tail->frame_id * data_size_not_last_frame + i] = (uint8_t)byte_data[i];
                }
            }
            // Last frame
            else // tail->frame_flag == ENUM_FRAME_LAST
            {
                last_frame_received = true;
                frame_tail_last_t *tail_last = (frame_tail_last_t*)&frame_decoded[TxPacketDataSize - sizeof(frame_tail_last_t)];

                if(sizeof(data_decoded) < tail->frame_id * data_size_not_last_frame + data_size_last_frame)
                {
                    emit consolePutData(QString("Error wrong frame number %1 or too too long continuous data receive\n").arg(tail->frame_id), 1);
                }
                else
                {
                    // Check data length
                    if(tail_last->len < MOD_SRP_MIN_VALID_LENGTH_NEW)
                    {
                        // Error length is too short
                        emit consolePutData(QString("HS data parsing error, packet data length is too short %1\n").arg(tail_last->len), 1);
                    }
                    else if(tail_last->len > (tail->frame_id * data_size_not_last_frame + TxPacketDataSize - sizeof(frame_tail_last_t)))
                    {
                        // Error length is too long
                        emit consolePutData(QString("HS data parsing error, packet data length is too long %1\n").arg(tail_last->len), 1);
                    }
                    else
                    {
                        // Length is ok
                        // Copy decoded frame to data_decoded buffer (just copy everything, do not calculate data length in last frame)
                        for(uint8_t i = 0; i < data_size_last_frame; ++i)
                            data_decoded[tail->frame_id * data_size_not_last_frame + i] = (uint8_t)byte_data[i];

                        // TODO - transmit later, when all QAM threads will be ready
                        // Transmit decoded data to PC
                        emit postTxDataToSerialPort((uint8_t*)&data_decoded[MASTER_ADDR_SIZE], tail_last->len - MASTER_ADDR_SIZE);

                        emit consolePutData(QString("Last frame #%1, data length %2\n").arg(tail_last->tail.frame_id).arg(tail_last->len), 1);
                    }
                }
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

//#ifdef QT_DEBUG
    if(crc_error || warning_status != CORRECT)
    {
        for(uint32_t i = 0; i < Length; ++i)
        {
            double val = Signal[i];
            FrameErrorAdcBuffer[n_error_frame][i] = (int16_t)val;
        }

        emit consoleFrameErrorFile(FrameErrorAdcBuffer[n_error_frame], Length);

        if(++n_error_frame == 10)
            n_error_frame = 0;
    }
//#endif

    // Debug
    qint64 elapsed = peformance_timer.elapsed();
    elapsed_all += elapsed;

    emit consolePutData(QString("QAM decoder finished for length %1: freq = %2 Hz, elapsed time = %3 ms\n").arg(Length).arg(f_est_data).arg(elapsed), 1);

    if(last_frame_received)
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
