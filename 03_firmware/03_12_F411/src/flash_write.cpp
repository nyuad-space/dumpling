#include "flash_write.h"
#include "pinout.h"
#include "global.h"

FlashLogger::FlashLogger()
    : flashSpi_(FLASH_MOSI, FLASH_MISO, FLASH_SCK),
      flashTransport_(FLASH_CS, flashSpi_),
      flash_(&flashTransport_)
{
}

bool FlashLogger::beginForLog()
{
    mounted_ = false;
    fileReady_ = false;

    flashSpi_.begin();

    // check hardware init & fs access
    if (!flash_.begin() || !fatfs_.begin(&flash_))
        return false;
    mounted_ = true;

    // get file handle to log + add header
    file_ = fatfs_.open(logPath, FILE_WRITE);
    if (!file_)
        return false;
    if (file_.size() == 0)
    {
        file_.println("timestamp_ms,ax_mps2,ay_mps2,az_mps2,gx_rads,gy_rads,gz_rads,temp_c");
        file_.flush();
    }
    fileReady_ = true;
    return true;
}
bool FlashLogger::beginForExport()
{
    mounted_ = false;
    fileReady_ = false;

    flashSpi_.begin();

    // check hardware init & fs access
    if (!flash_.begin() || !fatfs_.begin(&flash_))
        return false;
    mounted_ = true;

    // check temp file handle for reading
    File32 readFile = fatfs_.open(logPath, FILE_READ);
    if (!readFile)
        return false;
    readFile.close();

    fileReady_ = true;
    return true;
}
bool FlashLogger::append(const LSM6DSOsample &sample)
{
    // ensure file ready
    if (!mounted_ || !fileReady_ || !file_)
        return false;
    // place file cursor to EoF before writing
    if (!file_.seek(file_.size()))
        return false;

    file_.print(sample.timestamp_ms);
    file_.print(',');
    file_.print(sample.ax_mps2, 6);
    file_.print(',');
    file_.print(sample.ay_mps2, 6);
    file_.print(',');
    file_.print(sample.az_mps2, 6);
    file_.print(',');

    file_.print(sample.gx_rads, 6);
    file_.print(',');
    file_.print(sample.gy_rads, 6);
    file_.print(',');
    file_.print(sample.gz_rads, 6);
    file_.print(',');

    file_.println(sample.temp_c, 6);
    file_.flush();

    return true;
}
void FlashLogger::printStatus(Stream &out) const
{
    out.print("flash_mounted=");
    out.print(mounted_ ? "true" : "false");
    out.print(", file_ready==");
    out.println(fileReady_ ? "true" : "false");
}

void FlashLogger::exportLogToSerial(Stream &out)
{
    // ensure file ready
    if (!mounted_ || !fileReady_)
    {
        out.println("ERROR: flash log not ready");
        return;
    }

    // get file handle to read
    File32 readFile = fatfs_.open(logPath, FILE_READ);
    if (!readFile)
    {
        out.println("ERROR: failed to open csv");
        return;
    }

    // export marker + contents
    out.println("EXPORT_BEGIN");
    while (readFile.available())
        out.write(readFile.read());
    out.println();
    out.println("EXPORT_END");

    readFile.close();
}