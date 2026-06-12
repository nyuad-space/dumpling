#pragma once

constexpr unsigned long SAMPLE_INTERVAL_MS = 10;

constexpr bool FLASH_LOG_MODE = true;
constexpr bool SENSOR_READ_MODE = false;
constexpr bool FLASH_EXPORT_MODE = true; // re-flash for export mode
// TODO: one firmware for both logging and export (controlled by F405?)

enum class SerialOutMode
{
    Debug, // Serial monitor
    Csv    // App stream
};

constexpr SerialOutMode SERIAL_OUT_MODE = SerialOutMode::Csv;
