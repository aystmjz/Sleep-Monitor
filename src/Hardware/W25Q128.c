#include "W25Q128.h"

void W25Q128_Init(void)
{
    W25Q128_SPI_Init();
}

void W25Q128_ReadID(uint8_t *MID, uint16_t *DID)
{
    W25Q128_SPI_Start();
    W25Q128_SPI_SwapByte(W25Q128_JEDEC_ID);
    *MID = W25Q128_SPI_SwapByte(W25Q128_DUMMY_BYTE);
    *DID = W25Q128_SPI_SwapByte(W25Q128_DUMMY_BYTE);
    *DID <<= 8;
    *DID |= W25Q128_SPI_SwapByte(W25Q128_DUMMY_BYTE);
    W25Q128_SPI_Stop();
}

void W25Q128_WriteEnable(void)
{
    W25Q128_SPI_Start();
    W25Q128_SPI_SwapByte(W25Q128_WRITE_ENABLE);
    W25Q128_SPI_Stop();
}

void W25Q128_WaitBusy(void)
{
    uint32_t Timeout;
    W25Q128_SPI_Start();
    W25Q128_SPI_SwapByte(W25Q128_READ_STATUS_REGISTER_1);
    Timeout = 100000;
    while ((W25Q128_SPI_SwapByte(W25Q128_DUMMY_BYTE) & 0x01) == 0x01) {
        Timeout--;
        if (Timeout == 0) {
            break;
        }
    }
    W25Q128_SPI_Stop();
}

void W25Q128_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count)
{
    W25Q128_WaitBusy();
    W25Q128_WriteEnable();

    W25Q128_SPI_Start();
    W25Q128_SPI_SwapByte(W25Q128_PAGE_PROGRAM);
    W25Q128_SPI_SwapByte(Address >> 16);
    W25Q128_SPI_SwapByte(Address >> 8);
    W25Q128_SPI_SwapByte(Address);
    for (uint16_t i = 0; i < Count; i++) {
        W25Q128_SPI_SwapByte(DataArray[i]);
    }
    W25Q128_SPI_Stop();
}

void W25Q128_SectorErase(uint32_t Address)
{
    W25Q128_WaitBusy();
    W25Q128_WriteEnable();

    W25Q128_SPI_Start();
    W25Q128_SPI_SwapByte(W25Q128_SECTOR_ERASE_4KB);
    W25Q128_SPI_SwapByte(Address >> 16);
    W25Q128_SPI_SwapByte(Address >> 8);
    W25Q128_SPI_SwapByte(Address);
    W25Q128_SPI_Stop();
}

void W25Q128_BlockErase(uint32_t Address)
{
    W25Q128_WaitBusy();
    W25Q128_WriteEnable();

    W25Q128_SPI_Start();
    W25Q128_SPI_SwapByte(W25Q128_BLOCK_ERASE_64KB);
    W25Q128_SPI_SwapByte(Address >> 16);
    W25Q128_SPI_SwapByte(Address >> 8);
    W25Q128_SPI_SwapByte(Address);
    W25Q128_SPI_Stop();
}

void W25Q128_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count)
{
    W25Q128_SPI_Start();
    W25Q128_SPI_SwapByte(W25Q128_READ_DATA);
    W25Q128_SPI_SwapByte(Address >> 16);
    W25Q128_SPI_SwapByte(Address >> 8);
    W25Q128_SPI_SwapByte(Address);
    for (uint32_t i = 0; i < Count; i++) {
        DataArray[i] = W25Q128_SPI_SwapByte(W25Q128_DUMMY_BYTE);
    }
    W25Q128_SPI_Stop();
}

uint8_t W25Q128_GetNewestBlock(void)
{
    uint8_t block_index = 1, time_temp[4];
    uint32_t time_cnt   = 0;
    for (uint32_t i = 1; i <= W25Q128_MAX_BLOCK; i++) {
        uint32_t temp = 0;
        W25Q128_ReadData(i << 16, time_temp, 4);
        for (uint8_t j = 0; j < 4; j++) {
            temp |= (uint32_t)time_temp[j] << (j * 8);
        }
        if (temp != 0xffffffff && temp >= time_cnt) {
            time_cnt    = temp;
            block_index = i;
        }
    }
    return block_index;
}

uint32_t W25Q128_GetBlockTimeStamp(uint8_t block_index)
{
    uint8_t time_temp[4];
    uint32_t time_cnt = 0;

    W25Q128_ReadData((uint32_t)block_index << 16, time_temp, 4);
    for (uint8_t i = 0; i < 4; i++) {
        time_cnt |= (uint32_t)time_temp[i] << (i * 8);
    }
    // LCD_ShowNum(40, 100, time_temp[0], 3);
    // LCD_ShowNum(40, 120, time_temp[1], 3);
    // LCD_ShowNum(40, 140, time_temp[2], 3);
    // LCD_ShowNum(40, 160, time_temp[3], 3);
    return time_cnt;
}

void W25Q128_WriteBlockTimeStamp(uint32_t Address, time_t time_cnt)
{
    uint8_t temp[4];
    temp[3] = (time_cnt >> 24) & 0xff;
    temp[2] = (time_cnt >> 16) & 0xff;
    temp[1] = (time_cnt >> 8) & 0xff;
    temp[0] = (time_cnt) & 0xff;
    W25Q128_PageProgram(Address, temp, 4);
}
