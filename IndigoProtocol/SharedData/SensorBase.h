#pragma once

namespace sensor_base
{
    const uint32_t ID = 0xFF;
    const uint16_t DEFAULT_PERIOD_MS = 1000;
}

namespace sensor_base
{
    enum etState : uint8_t
    {
        DISABLED            = 0x00,
        TEMPERATURE_ENABLED = 0x01,
        VOLTAGE_ENABLED     = 0x02,

        DEFAULT = TEMPERATURE_ENABLED | VOLTAGE_ENABLED
    };

    struct ScenarioConfig
    {
        uint16_t _periodMs; ///< Period in milliseconds
        uint8_t _state; ///< Sensor state

        ScenarioConfig() :
            _periodMs(DEFAULT_PERIOD_MS),
            _state(etState::DEFAULT)
        {}
    };

    enum etChannelID
    {
        Temperature = 0,
        Voltage     = 1,
    };
};
