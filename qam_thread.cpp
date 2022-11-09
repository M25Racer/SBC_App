#include "qam_thread.h"
#include "usb_global.h"
#include "qam_decoder/qam_decoder.h"
#include "qam_decoder/HS_EWL_DEMOD_QAM.h"
#include "qam_decoder/HS_EWL_DEMOD_QAM_terminate.h"
#include "qam_decoder/HS_EWL_DEMOD_QAM_types.h"
#include "qam_decoder/HS_EWL_FREQ_ACQ.h"
#include "qam_decoder/HS_EWL_FREQ_EST_FOR_SWEEP.h"
#include "qam_decoder/HS_EWL_TR_FUN_EST.h"
#include <QElapsedTimer>
#include "qam_decoder/signal_sweep.h"
#include "qam_decoder/signal.h"
#include "qam_decoder/rt_nonfinite.h"
#include "crc8.h"
#include "crc16.h"
#include "srp_mod_protocol.h"
#include "rs_decoder.h"
#include "qam_decoder/rtwtypes.h"
#include "atomic_vars.h"
#include "statistics.h"

/* Extern global variables */
extern RingBuffer *m_ring;              // ring data buffer (ADC data) for QAM decoder
extern QWaitCondition ringNotEmpty;
extern QMutex m_mutex_ring_wait;
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
double data[14040];
//double index_data;
double len_data;
double f_est_data;//estimated frequency
//int f_est_size;

//  output var for HS_EWL_DEMOD_QAM
//creal_T qam_symbols_data[255];
int qam_symbols_size;
double byte_data[255];
int byte_data_size;

double qam_symbols_real[275];
double qam_symbols_imag[275];
double start_inf_data;

#define DATA_DECODED_SIZE           (128*1024)
#define N_DATA_DECODED_BUFFERS      20

static uint8_t data_decoded[N_DATA_DECODED_BUFFERS][DATA_DECODED_SIZE];      // data decoded from all qam packet
static uint8_t n_data_buf = 0;

double ref_cos[] = {1, 9.927089e-01, 9.709418e-01, 9.350162e-01, 8.854560e-01, 8.229839e-01, 7.485107e-01, 6.631227e-01, 5.680647e-01, 4.647232e-01, 3.546049e-01, 2.393157e-01, 1.205367e-01, 6.123234e-17, -1.205367e-01, -2.393157e-01, -3.546049e-01, -4.647232e-01, -5.680647e-01, -6.631227e-01, -7.485107e-01, -8.229839e-01, -8.854560e-01, -9.350162e-01, -9.709418e-01, -9.927089e-01, -1, -9.927089e-01, -9.709418e-01, -9.350162e-01, -8.854560e-01, -8.229839e-01, -7.485107e-01, -6.631227e-01, -5.680647e-01, -4.647232e-01, -3.546049e-01, -2.393157e-01, -1.205367e-01, -1.836970e-16, 1.205367e-01, 2.393157e-01, 3.546049e-01, 4.647232e-01, 5.680647e-01, 6.631227e-01, 7.485107e-01, 8.229839e-01, 8.854560e-01, 9.350162e-01, 9.709418e-01, 9.927089e-01};
double ref_sin[] = {0, 1.205367e-01, 2.393157e-01, 3.546049e-01, 4.647232e-01, 5.680647e-01, 6.631227e-01, 7.485107e-01, 8.229839e-01, 8.854560e-01, 9.350162e-01, 9.709418e-01, 9.927089e-01, 1, 9.927089e-01, 9.709418e-01, 9.350162e-01, 8.854560e-01, 8.229839e-01, 7.485107e-01, 6.631227e-01, 5.680647e-01, 4.647232e-01, 3.546049e-01, 2.393157e-01, 1.205367e-01, 1.224647e-16, -1.205367e-01, -2.393157e-01, -3.546049e-01, -4.647232e-01, -5.680647e-01, -6.631227e-01, -7.485107e-01, -8.229839e-01, -8.854560e-01, -9.350162e-01, -9.709418e-01, -9.927089e-01, -1, -9.927089e-01, -9.709418e-01, -9.350162e-01, -8.854560e-01, -8.229839e-01, -7.485107e-01, -6.631227e-01, -5.680647e-01, -4.647232e-01, -3.546049e-01, -2.393157e-01, -1.205367e-01};

// Debug variables
qint64 elapsed_all = 0;
qint64 elapsed_all_saved = 0;

// Reed Solomon decoder variables
/* generator polynomial */
unsigned char gs[nn-K1+1];

QamThread::QamThread(QObject *parent) :
    QThread(parent)
{
}

QamThread::~QamThread()
{
    m_mutex_ring_wait.lock();
    m_quit = true;
    ringNotEmpty.wakeOne();
    m_mutex_ring_wait.unlock();
    wait();
}

void QamThread::run()
{
    bool res = false;

    // RS decoder init
    generate_gf();
    gen_poly(T1, gs);
    //gen_poly(Ts2, gs2);

    while(!m_quit)
    {
        m_mutex_ring_wait.lock();
        if(!m_ring->DataAvailable())
            ringNotEmpty.wait(&m_mutex_ring_wait);    // Wait condition unlocks mutex before 'wait', and will lock it again just after 'wait' is complete
        m_mutex_ring_wait.unlock();

        m_qamDecodedDataAvailable = true;             // Some data available for QAM decoder

        res = m_ring->GetDouble(Signal, &Length);

        if(!res)
        {
            emit consolePutData("Error ring buffer get() returned false\n", 1);
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
    int rs_decode_flag = -1;
    qint64 rs_elapsed = 0;

    // ERROR status
    int HS_EWL_FREQ_ACQ_error_status;
    int HS_EWL_DEMOD_QAM_error_status;

    bool crc_error = true;
    bool last_frame_received = false;
    double *signal = (double*)&Signal;
    double len = Length;

//    //test
//    for(uint32_t i = 0; i < 13291; ++i)
//    {
//        Signal[i] = signal_test[i];
//    }
//    len = 13291;

    peformance_timer.start();

    HS_EWL_FREQ_ACQ_error_status = HS_EWL_FREQ_ACQ(signal, len, Fs, f0, sps, mode, preamble_len,
                        message_len, data, &len_data, (double*)&f_est_data, &warning_status);

    if(HS_EWL_FREQ_ACQ_error_status == 0)
    {
        HS_EWL_DEMOD_QAM_error_status = HS_EWL_DEMOD_QAM(data, len_data, f_est_data, Fs, qam_symbols_real,
                                                        qam_symbols_imag, byte_data, &start_inf_data);
        switch(HS_EWL_DEMOD_QAM_error_status)
        {
            case 1: // input data LEN <= 0
                emit consolePutData("HS_EWL_DEMOD_QAM_error_status: input data LEN <= 0\n", 1);
                break;
            case 2: // incorrect input sample freq for lagrange_resamp func
                emit consolePutData("HS_EWL_DEMOD_QAM_error_status: incorrect input sample freq for lagrange_resamp func\n", 1);
                break;
            default:
            case 0: // OK
                break;
        }

        // Data parsing
        // Convert decoded data from 'double' to 'uint8', copy to data_decoded[] buffer

        for(uint8_t i = 0; i < TxPacketRsCodesSize + TxPacketDataSize; ++i)
            frame_decoded[i] = (uint8_t)byte_data[i];

        // Parse 'frame' tail
        frame_tail_nlast_t *tail = (frame_tail_nlast_t*)&frame_decoded[TxPacketRsCodesSize + TxPacketDataSize - sizeof(frame_tail_nlast_t)];

        // Check 'frame' CRC
        uint8_t crc8 = calc_crc8(frame_decoded + TxPacketRsCodesSize, TxPacketDataSize - 1);

        if(crc8 != tail->crc8)
        {
            crc_error = true;
            //emit consolePutData(QString("HS frame parsing crc error, attempting to correct data with rs decoder\n"), 1);

            peformance_timer2.start();

            // Zero padding before decode
            memset(frame_decoded + TxPacketRsCodesSize + TxPacketDataSize, 0x00, sizeof(frame_decoded) - TxPacketRsCodesSize - TxPacketDataSize);

            rs_decode_flag = decode_rs1(frame_decoded);

            if(rs_decode_flag < 2)
            {
                // Check crc again
                crc8 = calc_crc8(frame_decoded + TxPacketRsCodesSize, TxPacketDataSize - 1);
            }

            rs_elapsed = peformance_timer2.elapsed();
        }

        if(crc8 == tail->crc8)
        {
            crc_statistics_good_crc_received();

            crc_error = false;

            const uint32_t data_size_not_last_frame = TxPacketDataSize - sizeof(frame_tail_nlast_t);    // valuable data in 'not last' frame
            const uint32_t data_size_last_frame = TxPacketDataSize - sizeof(frame_tail_last_t);         // valuable data in 'last' frame

            // Not last frame
            if(tail->frame_flag == ENUM_FRAME_NOT_LAST)
            {
                //emit consolePutData(QString("Not last frame #%1, continue receive\n").arg(tail->frame_id), 1);

                if(DATA_DECODED_SIZE < tail->frame_id * data_size_not_last_frame + data_size_not_last_frame)
                {
                    emit consolePutData(QString("Error wrong frame number %1 or too too long continuous data receive\n").arg(tail->frame_id), 1);
                }
                else
                {
                    // Copy decoded frame to data_decoded buffer
                    for(uint8_t i = 0; i < data_size_not_last_frame; ++i)
                        data_decoded[n_data_buf][tail->frame_id * data_size_not_last_frame + i] = (uint8_t)frame_decoded[TxPacketRsCodesSize + i];
                }
            }
            // Last frame
            else // tail->frame_flag == ENUM_FRAME_LAST
            {
                last_frame_received = true;
                frame_tail_last_t *tail_last = (frame_tail_last_t*)&frame_decoded[TxPacketRsCodesSize + TxPacketDataSize - sizeof(frame_tail_last_t)];

                if(DATA_DECODED_SIZE < tail->frame_id * data_size_not_last_frame + data_size_last_frame)
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
                            data_decoded[n_data_buf][tail->frame_id * data_size_not_last_frame + i] = (uint8_t)frame_decoded[TxPacketRsCodesSize + i];

                        // Transmit decoded data to PC
                        emit postTxDataToSerialPort((uint8_t*)&data_decoded[n_data_buf][MASTER_ADDR_SIZE], tail_last->len - MASTER_ADDR_SIZE);
                        m_qamDecodedDataAvailable = false;  // No data for QAM decoder
                        if(++n_data_buf == N_DATA_DECODED_BUFFERS)
                            n_data_buf = 0;

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

        if(crc8 != tail->crc8)
        {
            crc_statistics_bad_crc_received();
            emit consolePutData(QString("CRC error, rs decoder failed to correct data\n"), 1);
        }
    }
    
    else
    {
        HS_EWL_DEMOD_QAM_error_status = -1; // HS_EWL_DEMOD_QAM not use
    }

    // RS decoder debug information
    switch(rs_decode_flag)
    {
        case 0:
            /* no non-zero syndromes => no errors: output received codeword */
            emit consolePutData(QString("RS decoder finished in %1 ms: no errors\n").arg(rs_elapsed), 1);
            break;

        case 1:
            emit consolePutData(QString("RS decoder finished in %1 ms: errors corrected\n").arg(rs_elapsed), 1);
            break;

        case 2:
        case 3:
            emit consolePutData(QString("RS decoder finished in %1 ms: unable to correct\n").arg(rs_elapsed), 1);
            break;

        default:
            break;
    }

    if(rs_decode_flag == -1)
        rs_statistics_add_no_correction();
    else if(rs_decode_flag < 2)
        rs_statistics_add_correction();

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

        emit consoleFrameErrorFile(FrameErrorAdcBuffer[n_error_frame], Length, 0);

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

//    HS_EWL_DEMOD_QAM_terminate();
}
