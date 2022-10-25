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

QAtomicInteger<uint8_t> FreqEstState = TFreqEstState::FREQ_EST_IDLE;
QAtomicInteger<uint8_t> SweepState = TSweepState::SWEEP_IDLE;