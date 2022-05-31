#ifndef SRP_MOD_PROTOCOL_H
#define SRP_MOD_PROTOCOL_H

#include "stdint.h"

#define MOD_SRP_PROTOCOL_HEADER_SIZE        (3 + 0) // 1 byte addr, 2 bytes length
#define MOD_SRP_PROTOCOL_CRC_SIZE           (2)
#define MOD_SRP_PROTOCOL_DATA_ECC_MIN_SIZE  (0)
#define MASTER_ADDR_SIZE                    (1)     // 'master address = 0x4d' size

#define MOD_SRP_MIN_VALID_LENGTH (MOD_SRP_PROTOCOL_HEADER_SIZE + MOD_SRP_PROTOCOL_CRC_SIZE + MOD_SRP_PROTOCOL_DATA_ECC_MIN_SIZE + MASTER_ADDR_SIZE)

#pragma pack(push, 1)

typedef enum
{
    ENUM_FRAME_START = 0,
    ENUM_FRAME_STOP	= 1
} enum_line_frame_flag;

typedef struct
{
    uint8_t frame_flag 	: 1;
    uint8_t frame_id	: 7;
} start_header_t;

typedef struct
{
    uint16_t len;
    uint16_t crc16;
} stop_header_t;

#pragma pack(pop)

#endif // SRP_MOD_PROTOCOL_H
