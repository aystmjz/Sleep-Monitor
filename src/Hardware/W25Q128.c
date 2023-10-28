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
