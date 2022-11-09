#ifndef STATISTICS_H
#define STATISTICS_H

#include <QCoreApplication>

#define CRC_MAX_STATISTICS 100
#define RS_STATISTICS_QUEUE_SIZE 100

void statisics_reset();

void crc_statistics_good_crc_received();
void crc_statistics_bad_crc_received();

void rs_statistics_add_correction();
void rs_statistics_add_no_correction();

uint8_t rs_calculate_statistics();

#endif // STATISTICS_H
