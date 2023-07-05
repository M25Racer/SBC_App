#ifndef ATOMIC_VARS_H
#define ATOMIC_VARS_H

#include <QCoreApplication>

typedef enum : uint8_t
{
    FREQ_EST_IDLE = 0,  //todo don't needed
    FREQ_EST_START = 1,
    FREQ_EST_WORK_IN_PROGRESS = 2,
    FREQ_EST_COMPLETE = 3,
} TFreqEstState;

typedef enum : uint8_t
{
    SWEEP_IDLE = 0,
    SWEEP_START = 1,
    SWEEP_WORK_IN_PROGRESS = 2,
    SWEEP_COMPLETE = 3,
} TSweepState;

typedef enum : uint8_t
{
    LS_MODE = 0x00,
    HS_210_MODE = 0x01,
    HS_280_MODE = 0x02
} TSrpMode;

/* Global atomic variables */
extern QAtomicInteger<uint8_t> mod_status_reg;

extern QAtomicInteger<bool> m_qamDecodedDataAvailable;
extern QAtomicInteger<quint8> m_statusAutoCfgPredistortion;
extern QAtomicInteger<uint8_t> AgcStateGlobal;

extern QAtomicInteger<bool> common_special_command;
extern QAtomicInteger<bool> sin600_command;
extern QAtomicInteger<bool> sweep_command;

extern QAtomicInteger<uint8_t> FreqEstState;
extern QAtomicInteger<uint8_t> SweepState;

extern QAtomicInteger<uint8_t> m_rxHighSpeedStatistics;         // Increments each QAM frame with good crc, decrements each bad crc. Limited to [0..CRC_MAX_STATISTICS]
extern QAtomicInteger<uint16_t> m_ReedSolomonCorrections;       // Infinite counter of rs corrections, rolls over, 0..65535

extern QAtomicInteger<uint8_t> m_SrpMode;   // current speed mode

#endif // ATOMIC_VARS_H
