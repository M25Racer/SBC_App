#include "global_vars.h"

static QMutex mutex_agc;
static QMutex mutex_sin600;
static QMutex mutex_sweep;
static QMutex mutex_fest;

static uint8_t AgcState = AGC_OK;

static bool sin600_command = false;
static bool sweep_command = false;

static TFreqEstState FreqEstState = FREQ_EST_IDLE;
static TSweepState SweepState = SWEEP_IDLE;

void Set_AGC_State(uint8_t new_state)
{
    mutex_agc.lock();
    AgcState = new_state;
    mutex_agc.unlock();
}

uint8_t Get_AGC_State()
{
    mutex_agc.lock();
    uint8_t s = AgcState;
    mutex_agc.unlock();
    return s;
}

void Set_Special_Command_SIN600(bool s)
{
    mutex_sin600.lock();
    sin600_command = s;
    mutex_sin600.unlock();
}

bool Get_Special_Command_SIN600()
{
    mutex_sin600.lock();
    bool s = sin600_command;
    mutex_sin600.unlock();
    return s;
}

void Set_Special_Command_Sweep(bool s)
{
    mutex_sweep.lock();
    sweep_command = s;
    mutex_sweep.unlock();
}

bool Get_Special_Command_Sweep()
{
    mutex_sweep.lock();
    bool s = sweep_command;
    mutex_sweep.unlock();
    return s;
}

void Set_FreqEstState(TFreqEstState s)
{
    mutex_fest.lock();
    FreqEstState = s;
    mutex_fest.unlock();
}

TFreqEstState Get_FreqEstState()
{
    TFreqEstState s;
    mutex_fest.lock();
    s = FreqEstState;
    mutex_fest.unlock();
    return s;
}

void Set_SweepState(TSweepState s)
{
    mutex_sweep.lock();
    SweepState = s;
    mutex_sweep.unlock();
}

TSweepState Get_SweepState()
{
    TSweepState s;
    mutex_sweep.lock();
    s = SweepState;
    mutex_sweep.unlock();
    return s;
}
