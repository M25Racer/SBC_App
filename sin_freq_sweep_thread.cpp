#include "sin_freq_sweep_thread.h"
#include "usb_global.h"
#include "qam_common.h"
#include "qam_decoder/qam_decoder.h"
#include "qam_decoder/HS_EWL_DEMOD_QAM.h"
#include "qam_decoder/HS_EWL_DEMOD_QAM_terminate.h"
#include "qam_decoder/HS_EWL_DEMOD_QAM_types.h"
#include "qam_decoder/HS_EWL_FREQ_ACQ.h"
#include "qam_decoder/HS_EWL_FREQ_EST_FOR_SWEEP.h"
#include "qam_decoder/HS_EWL_TR_FUN_EST.h"
#include <QElapsedTimer>
#include "qam_decoder/math_sweep.h"
#include "qam_decoder/rt_nonfinite.h"

#include "crc16.h"
#include "srp_mod_protocol.h"
#include "atomic_vars.h"

using namespace QAM_Common;

/* Extern global variables */
extern QWaitCondition sinFreqSweepBufNotEmpty;
extern QMutex m_mutex_sweep_thread;
extern double f0;   // carrier frequency

/* Private variables */
static double SignalSin[USB_MAX_DATA_SIZE];
static double SignalSweep[USB_MAX_DATA_SIZE];
static int16_t SignalBuffer[2][USB_MAX_DATA_SIZE];

// output var HS_EWL_TR_FUN_EST
double gain_data[2048];
double phase_data[2048];
float gain_data_float[2048];
float phase_data_float[2048];
static double sweep_warning_status;
static double shift_for_qam_data;
uint16_t shift_for_qam_data_int;

// output var HS_EWL_FREQ_EST_FOR_SWEEP
static double f_opt;
static double ph_opt;
static double sweep_freq_warning_status;

/* Global variables */
uint8_t SinFreqEstDataBuffer[USB_MAX_DATA_SIZE];
uint8_t SweepDataBuffer[USB_MAX_DATA_SIZE];
uint32_t SinFreqEstDataLength = 0;
uint32_t SweepDataLength = 0;

SinFreqSweepThread::SinFreqSweepThread(QObject *parent) :
    QThread(parent)
{
}

SinFreqSweepThread::~SinFreqSweepThread()
{
    m_mutex_sweep_thread.lock();
    m_quit = true;
    SinFreqEstDataLength = 0;
    SweepDataLength = 0;
    sinFreqSweepBufNotEmpty.wakeOne();
    m_mutex_sweep_thread.unlock();

    wait();
}

void SinFreqSweepThread::run()
{
    bool res = false;
    bool flagCalcSinFreq = false;
    bool flagCalcSweep = false;

    while(!m_quit)
    {
        m_mutex_sweep_thread.lock();
        sinFreqSweepBufNotEmpty.wait(&m_mutex_sweep_thread); // Wait condition unlocks mutex before 'wait', and will lock it again just after 'wait' is complete

        if(SinFreqEstDataLength)
        {
            res = ConvertToDouble(SinFreqEstDataBuffer, SinFreqEstDataLength, SignalSin, &LengthSin);
            SinFreqEstDataLength = 0;
            flagCalcSinFreq = true;
        }

        if(SweepDataLength)
        {
            res = ConvertToDouble(SweepDataBuffer, SweepDataLength, SignalSweep, &LengthSweep);
            SweepDataLength = 0;
            flagCalcSweep = true;
        }

        m_mutex_sweep_thread.unlock();

        if(flagCalcSinFreq)
        {
            flagCalcSinFreq = false;
            if(res)
            {
                for(uint32_t i = 0; i < LengthSin; ++i)
                {
                    double val = SignalSin[i];
                    SignalBuffer[0][i] = (int16_t)val;
                 }
                emit consolePutAdcDataSpecial(SignalBuffer[0], LengthSin, 1);
                FreqEstimateForSweep();
            }
        }

        if(flagCalcSweep)
        {
            flagCalcSweep = false;
            if(res)
            {
                for(uint32_t i = 0; i < LengthSweep; ++i)
                {
                    double val = SignalSweep[i];
                    SignalBuffer[1][i] = (int16_t)val;
                }

                if(sweep_record_to_file)
                {
                    // Save 'sweep' to file, do not calculate
                    sweep_record_to_file = false;
                    emit consolePutAdcDataSpecial(SignalBuffer[1], LengthSweep, 3);
                }
                else
                {
                    emit consolePutAdcDataSpecial(SignalBuffer[1], LengthSweep, 2);
                    Sweep();
                }
            }
        }
    }
}

bool SinFreqSweepThread::ConvertToDouble(uint8_t *p_data_in, uint32_t length_in, double *p_data_out, double *p_length)
{
    // Check pointers
    if(p_data_out == nullptr || p_length == nullptr)
    {
        emit consolePutData("Error ring buffer get: data pointer or length pointer = null", 2);
        return false;
    }

    if(!length_in)
        return false;

    uint16_t value;
    uint64_t j = 0;

    for(uint32_t i = 0; i < length_in; i+=2)
    {
        value = p_data_in[i];
        value |= ((uint16_t)p_data_in[i+1]) << 8;
        p_data_out[j++] = double((int16_t)value);
    }

    *p_length = length_in/2;

    return true;
}

void SinFreqSweepThread::FreqEstimateForSweep()
{
    emit consolePutData(QString("QAM sin frequency estimate for sweep starting, length %1\n").arg(LengthSin), 2);

    FreqEstState = TFreqEstState::FREQ_EST_WORK_IN_PROGRESS;

    double *sine = (double*)&SignalSin;
    double sine_len = LengthSin;

    peformance_timer.start();

    HS_EWL_FREQ_EST_FOR_SWEEP(sine, sine_len, Fs, f_sine, period_amount, sine_sps,
                    &f_opt, &ph_opt, &sweep_freq_warning_status);

    int sweep_warning_status_int = int(sweep_freq_warning_status);

    switch(sweep_warning_status_int)
    {
        case 0: // sweep_freq_warning_status = 0;% OK input array
            //emit consolePutData(QString("Sweep freq: OK input array\n"), 2);
            // Save frequency
            f0 = f_opt;
            emit consolePutData(QString("Saving carrier frequency f0 = %1\n").arg(f0), 2);
            break;
        case 1:
            emit consolePutData(QString("Sweep freq warning: all or more than 33% input array equal 0\n"), 2);
            break;
        case 2:
            emit consolePutData(QString("Sweep freq warning: path of input array equal 0, less than 33%\n"), 2);
            break;
        default:
            emit consolePutData(QString("Sweep freq warning: unknown status %1\n").arg(sweep_warning_status_int), 2);
            break;
    }

    emit consolePutData(QString("Sweep freq f_opt = %1, elapsed time = %2 ms\n").arg(f_opt).arg(peformance_timer.elapsed()), 2);
    FreqEstState = TFreqEstState::FREQ_EST_COMPLETE;
}

void SinFreqSweepThread::Sweep()
{
    emit consolePutData(QString("QAM sweep starting, length %1\n").arg(LengthSweep), 2);

    SweepState = TSweepState::SWEEP_WORK_IN_PROGRESS;

    peformance_timer.start();

    double *sweep = (double*)&SignalSweep;

    HS_EWL_TR_FUN_EST(sweep, math_sweep, Fs, f_opt, f_sine, pream_sps,
                     gain_data, phase_data,&shift_for_qam_data,
                     &sweep_warning_status);

    // Convert to float
    for(uint16_t i = 0; i < 2048; ++i)
    {
        gain_data_float[i] = float(gain_data[i]);
        phase_data_float[i] = float(phase_data[i]);
    }

    // Convert 'shift' to uint16_t
    shift_for_qam_data_int = uint16_t(shift_for_qam_data);

    int sweep_warning_status_int = int(sweep_warning_status);

    switch(sweep_warning_status_int)
    {
        case 0: // sweep_warning_status   = 0; %input array OK
            //emit consolePutData(QString("Sweep: input array OK\n"), 2);
            break;
        case 1: // sweep_warning_status   = 1; %input sine singal freq or sample freq equal 0
            emit consolePutData(QString("Sweep warning: input sine singal freq or sample freq equal 0\n"), 2);
            break;
        case 2: // sweep_warning_status   = 2; %not enough len input array len < 80%
            emit consolePutData(QString("Sweep warning: not enough len input array len < 80%\n"), 2);
            break;
        case 3: // sweep_warning_status   = 3; % len input array > 80% or < 100%
            emit consolePutData(QString("Sweep warning: len input array > 80% or < 100%\n"), 2);
            break;
        case 4: // sweep_warning_status   = 4; %no found sweep preamble(ampMod)
            emit consolePutData(QString("Sweep warning: no found sweep preamble(ampMod)\n"), 2);
            break;
        case 5: // sweep_warning_status   = 5; %all input array equal 0
            emit consolePutData(QString("Sweep warning: all input array equal 0\n"), 2);
            break;
        default:
            emit consolePutData(QString("Sweep warning: unknown status %1\n").arg(sweep_warning_status_int), 2);
            break;
    }

    emit consolePutData(QString("Sweep elapsed time = %1 ms\n").arg(peformance_timer.elapsed()), 2);
    emit consolePutData(QString("Sweep shift_for_qam_data = %1\n").arg(shift_for_qam_data_int), 2);

    SweepState = TSweepState::SWEEP_COMPLETE;
}
