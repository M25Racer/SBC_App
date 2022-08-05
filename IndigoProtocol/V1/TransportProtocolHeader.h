#pragma once

#pragma pack(push, 1)
struct ToolAddress
{
    uint8_t      tool_id;
    uint16_t     serial_num;
};
#pragma pack(pop)

/*
* Заголовок транспортного уровня
*/
#pragma pack(push, 1)
struct TransportProtocolHeaderV1
{
    uint8_t       version;
    uint8_t       header_length;
    ToolAddress   address_source;
    ToolAddress   address_destination;
    uint8_t       frame_num;
    uint8_t       flags;
    uint32_t      data_length;
    uint32_t      data_offset;
    uint8_t       crc8_header;
    uint16_t      data_crc;
};
#pragma pack(pop)