#ifndef STATISTICS_H
#define STATISTICS_H

#include <QCoreApplication>

#define MAX_STATISTICS 100

#pragma pack(push, 1)

typedef struct {
    uint16_t good_synhro;
    uint16_t good_crc;
} data_quality;

#pragma pack(pop)

void statisics_reset();

void good_synchro_received();

void good_crc_received();

void bad_synchro_received();

void bad_crc_received();

data_quality get_data_statistics();

#endif // STATISTICS_H
