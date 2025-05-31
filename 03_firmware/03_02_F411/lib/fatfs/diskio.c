#include <Adafruit_SPIFlash.h>

extern Adafruit_SPIFlash flash;

extern "C"
{

#include "fatfs/diskio.h"

    DSTATUS disk_status(BYTE pdrv)
    {
        return 0;
    }

    DSTATUS disk_initialize(BYTE pdrv)
    {
        return 0;
    }

    DRESULT disk_read(BYTE pdrv, BYTE *buff, DWORD sector, UINT count)
    {
        return flash.readBlocks(sector, buff, count) ? RES_OK : RES_ERROR;
    }

    DRESULT disk_write(BYTE pdrv, const BYTE *buff, DWORD sector, UINT count)
    {
        return flash.writeBlocks(sector, buff, count) ? RES_OK : RES_ERROR;
    }

    DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void *buff)
    {
        switch (cmd)
        {
        case CTRL_SYNC:
            flash.syncBlocks();
            return RES_OK;
        case GET_SECTOR_COUNT:
            *((DWORD *)buff) = flash.size() / 512;
            return RES_OK;
        case GET_SECTOR_SIZE:
            *((WORD *)buff) = 512;
            return RES_OK;
        case GET_BLOCK_SIZE:
            *((DWORD *)buff) = 8;
            return RES_OK;
        default:
            return RES_PARERR;
        }
    }
}
