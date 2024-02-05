#ifndef __W25Q128_H
#define __W25Q128_H

#include "stm32f10x.h"                  // Device header
#include "SPI.h"
#include "sys.h"
#include <time.h>

#define W25Q128_WRITE_ENABLE                     0x06
#define W25Q128_WRITE_DISABLE                    0x04
#define W25Q128_READ_STATUS_REGISTER_1           0x05
#define W25Q128_READ_STATUS_REGISTER_2           0x35
#define W25Q128_WRITE_STATUS_REGISTER            0x01
#define W25Q128_PAGE_PROGRAM                     0x02
#define W25Q128_QUAD_PAGE_PROGRAM                0x32
#define W25Q128_BLOCK_ERASE_64KB                 0xD8
#define W25Q128_BLOCK_ERASE_32KB                 0x52
#define W25Q128_SECTOR_ERASE_4KB                 0x20
#define W25Q128_CHIP_ERASE                       0xC7
#define W25Q128_ERASE_SUSPEND                    0x75
#define W25Q128_ERASE_RESUME                     0x7A
#define W25Q128_POWER_DOWN                       0xB9
#define W25Q128_HIGH_PERFORMANCE_MODE            0xA3
#define W25Q128_CONTINUOUS_READ_MODE_RESET       0xFF
#define W25Q128_RELEASE_POWER_DOWN_HPM_DEVICE_ID 0xAB
#define W25Q128_MANUFACTURER_DEVICE_ID           0x90
#define W25Q128_READ_UNIQUE_ID                   0x4B
#define W25Q128_JEDEC_ID                         0x9F
#define W25Q128_READ_DATA                        0x03
#define W25Q128_FAST_READ                        0x0B
#define W25Q128_FAST_READ_DUAL_OUTPUT            0x3B
#define W25Q128_FAST_READ_DUAL_IO                0xBB
#define W25Q128_FAST_READ_QUAD_OUTPUT            0x6B
#define W25Q128_FAST_READ_QUAD_IO                0xEB
#define W25Q128_OCTAL_WORD_READ_QUAD_IO          0xE3

#define W25Q128_DUMMY_BYTE                       0xFF

#define W25Q128_MAX_BLOCK 240

void W25Q128_Init(void);
void W25Q128_ReadID(uint8_t *MID, uint16_t *DID);
void W25Q128_WriteEnable(void);
void W25Q128_WaitBusy(void);
void W25Q128_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count);
void W25Q128_SectorErase(uint32_t Address);
void W25Q128_BlockErase(uint32_t Address);
void W25Q128_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count);
uint8_t W25Q128_GetNewestBlock(void);
uint32_t W25Q128_GetBlockTimeStamp(uint8_t block_index);
void W25Q128_WriteBlockTimeStamp(uint32_t Address,time_t  time_cnt);

#endif
