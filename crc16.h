#ifndef CRC16_H
#define CRC16_H

#include <QCoreApplication>

uint16_t calc_crc16( const uint8_t *array, uint16_t len);

bool check_crc16( const uint8_t *array, uint16_t len, uint16_t crc);

uint16_t add_crc16(uint8_t *array, uint16_t len);

uint16_t calc_crc16_xmodem( const uint8_t *array, uint16_t len );

bool check_crc16_xmodem(const uint8_t *array, uint16_t len, uint16_t crc);

uint16_t add_crc16_xmodem(uint8_t *array, uint16_t len);

#endif // CRC16_H
