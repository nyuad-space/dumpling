#include "flash_write.h"
#include "pinout.h"
#include "global.h"

FlashLogger::FlashLogger()
    : flashSpi_(FLASH_MOSI, FLASH_MISO, FLASH_SCK),
      flashTransport_(FLASH_CS, flashSpi_),
      flash_(&flashTransport_)
{
}
bool FlashLogger::begin()
{
    flashSpi_.begin();

    if (!flash_.begin() || !fatfs_.begin(&flash_))
        return false;

    mounted_ = true;
    return writeHeaderIfNeeded();
}
bool FlashLogger::append(const LSM6DSOsample &sample)
{
    // ensure file ready
    if (!mounted_ || !writeHeaderIfNeeded() || !openLogFileIfNeeded())
        return false;

    file_.seek(file_.size());
    file_.print(sample.timestamp_ms);
    file_.print(',');
    file_.print(sample.ax_mps2, 3);
    file_.print(',');
    file_.print(sample.ay_mps2, 3);
    file_.print(',');
    file_.print(sample.az_mps2, 3);
    file_.print(',');

    file_.print(sample.gx_rads, 3);
    file_.print(',');
    file_.print(sample.gy_rads, 3);
    file_.print(',');
    file_.print(sample.gz_rads, 3);
    file_.print(',');

    file_.println(sample.temp_c, 3);
    file_.flush();

    return true;
}
void FlashLogger::printStatus(Stream &out) const
{
    out.print("flash_mounted=");
    out.print(mounted_ ? "true" : "false");
    out.print(", header_checked=");
    out.println(headerChecked_ ? "true" : "false");
}

bool FlashLogger::openLogFileIfNeeded()
{
    if (file_)
        return true;
    file_ = fatfs_.open(LOG_FILENAME, FILE_WRITE);
    if (!file_)
        return false;
    return true;
}
bool FlashLogger::writeHeaderIfNeeded()
{
    if (headerChecked_)
        return true;
    if (!openLogFileIfNeeded())
        return false;
    if (file_.size() == 0)
    {
        file_.println("timestamp_ms,ax_mps2,ay_mps2,az_mps2,gx_rads,gy_rads,gz_rads,temp_c");
        file_.flush();
    }
    headerChecked_ = true;
    return true;
}