#pragma once

constexpr unsigned long SAMPLE_INTERVAL_MS = 10; // 100Hz

enum class OperatingMode
{
    BenchDebug, // sensor -> MCU -> text on terminal
    LiveCsv,    // sensor -> MCU -> CSV stream (-> launch python app -> live dashboard + optional save .csv)
    FlightLog,  // sensor -> MCU -> on-board flash
    ExportLog   // on-board flash -> MCU (-> launch python export tool -> .csv on laptop)
};

// --- CHANGE MODE HERE ---
constexpr OperatingMode MODE = OperatingMode::LiveCsv; // main.cpp uses the derived constants from this

// --- Derived Modes ---
constexpr bool SENSOR_READ_MODE =
    (MODE == OperatingMode::BenchDebug) ||
    (MODE == OperatingMode::LiveCsv) ||
    (MODE == OperatingMode::FlightLog);

constexpr bool FLASH_LOG_MODE =
    (MODE == OperatingMode::FlightLog);

constexpr bool FLASH_EXPORT_MODE =
    (MODE == OperatingMode::ExportLog); // read-only

enum class SerialOutMode
{
    Debug,
    Csv
};

constexpr SerialOutMode SERIAL_OUT_MODE =
    (MODE == OperatingMode::LiveCsv)
        ? SerialOutMode::Csv
        : SerialOutMode::Debug;
