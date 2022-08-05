#pragma once

/*!
 \brief Статус сенсора на каждую выборку.
 */
#pragma pack(push, 1)
struct SensorStatus
{
    uint8_t sync;
    uint16_t voltage;
};
#pragma pack(pop)
