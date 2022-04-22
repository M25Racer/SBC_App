#ifndef SRP_MOD_PROTOCOL_H
#define SRP_MOD_PROTOCOL_H

#define MOD_SRP_PROTOCOL_HEADER_SIZE        (3 + 0) // 1 byte addr, 2 bytes length
#define MOD_SRP_PROTOCOL_CRC_SIZE           (2)
#define MOD_SRP_PROTOCOL_DATA_ECC_MIN_SIZE  (0)
#define MASTER_ADDR_SIZE                    (1)     // 'master address = 0x4d' size

#define MOD_SRP_MIN_VALID_LENGTH (MOD_SRP_PROTOCOL_HEADER_SIZE + MOD_SRP_PROTOCOL_CRC_SIZE + MOD_SRP_PROTOCOL_DATA_ECC_MIN_SIZE + MASTER_ADDR_SIZE)

#endif // SRP_MOD_PROTOCOL_H
