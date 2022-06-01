/*
 * CRC8.h
 */

#ifndef CRC8_H_
#define CRC8_H_

#include "stdint-gcc.h"

uint8_t calc_crc8(const uint8_t *array, uint16_t len);

bool check_crc8(const uint8_t *array, uint16_t len, uint8_t crc);

uint16_t add_crc8(uint8_t *array, uint16_t len);

#endif /* CRC8_H_ */
