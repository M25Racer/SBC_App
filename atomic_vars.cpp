#include <QCoreApplication>
#include "agc_algorithm.h"
#include "atomic_vars.h"

/* Global atomic variables */
QAtomicInteger<uint8_t> mod_status_reg;

QAtomicInteger<bool> m_qamDecodedDataAvailable = false;

QAtomicInteger<quint8> m_statusAutoCfgPredistortion = 0x00;

QAtomicInteger<uint8_t> AgcStateGlobal = AGC_OK;

QAtomicInteger<bool> common_special_command = false;
QAtomicInteger<bool> sin600_command = false;
QAtomicInteger<bool> sweep_command = false;
QAtomicInteger<bool> sweep_record_to_file = false;

QAtomicInteger<uint8_t> FreqEstState = TFreqEstState::FREQ_EST_IDLE;
QAtomicInteger<uint8_t> SweepState = TSweepState::SWEEP_IDLE;

QAtomicInteger<uint8_t> m_rxHighSpeedStatistics = 0;
QAtomicInteger<uint16_t> m_ReedSolomonCorrectionsCounter = 0;
QAtomicInteger<uint16_t> m_ReedSolomonFailsCounter = 0;
QAtomicInteger<uint16_t> m_QamFramesCounter = 0;
QAtomicInteger<uint16_t> m_CrcErrorsCounter = 0;

QAtomicInteger<uint8_t> m_SrpMode = HS_280_MODE; // current speed mode

QAtomicInteger<bool> m_ShowAdvancedLogs = false;
