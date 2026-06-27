#pragma once

constexpr unsigned long SAMPLE_INTERVAL_MS = 10;

enum class OperatingMode
{
    BenchDebug, // human-readable
    LiveCsv,    // live stream to python
    FlightLog,  // on-board logging
    ExportLog   // post-flight export
};

// --- CHANGE MODE HERE ---
// main.cpp uses the derived constants from this
constexpr OperatingMode MODE = OperatingMode::LiveCsv;

enum class SerialOutMode
{
    Debug,
    Csv
};

constexpr bool SENSOR_READ_MODE =
    (MODE == OperatingMode::BenchDebug) ||
    (MODE == OperatingMode::LiveCsv) ||
    (MODE == OperatingMode::FlightLog);

constexpr bool FLASH_LOG_MODE =
    (MODE == OperatingMode::FlightLog);

constexpr bool FLASH_EXPORT_MODE =
    (MODE == OperatingMode::ExportLog); // read-only

constexpr SerialOutMode SERIAL_OUT_MODE =
    (MODE == OperatingMode::LiveCsv)
        ? SerialOutMode::Csv
        : SerialOutMode::Debug;
