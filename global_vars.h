#ifndef GLOBAL_VARS_H
#define GLOBAL_VARS_H

#include <QCoreApplication>
#include <QMutex>
#include "agc_algorithm.h"

typedef enum
{
    FREQ_EST_IDLE = 0,  //todo don't needed
    FREQ_EST_START = 1,
    FREQ_EST_WORK_IN_PROGRESS = 2,
    FREQ_EST_COMPLETE = 3,
} TFreqEstState;

typedef enum
{
    SWEEP_IDLE = 0,
    SWEEP_START = 1,
    SWEEP_WORK_IN_PROGRESS = 2,
    SWEEP_COMPLETE = 3,
} TSweepState;

void Set_AGC_State(uint8_t new_state);
uint8_t Get_AGC_State();

void Set_Special_Command_SIN600(bool s);
bool Get_Special_Command_SIN600();

void Set_Special_Command_Sweep(bool s);
bool Get_Special_Command_Sweep();

void Set_FreqEstState(TFreqEstState s);
TFreqEstState Get_FreqEstState();

void Set_SweepState(TSweepState s);
TSweepState Get_SweepState();

#endif // GLOBAL_VARS_H
