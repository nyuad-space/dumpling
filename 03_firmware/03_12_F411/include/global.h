#pragma once

constexpr unsigned long SAMPLE_INTERVAL_MS = 10;

constexpr bool FLASH_LOG_MODE = true;
constexpr bool FLASH_DUMP_MODE = true;
constexpr bool SENSOR_READ_MODE = false;

enum class SerialOutMode
{
    Debug, // Serial monitor
    Csv    // App stream
};

constexpr SerialOutMode SERIAL_OUT_MODE = SerialOutMode::Csv;
