#include "DMA.h"

// OLED DMA
void DMA1_Init(DMA_Channel_TypeDef *DMA_CHx, uint32_t cpar, uint32_t cmar, uint16_t cndtr)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_DeInit(DMA_CHx);
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;                        // dma 要搬运到的外设地址
    DMA_InitStructure.DMA_MemoryBaseAddr     = cmar;                        // dma要搬运的内存的地址
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralDST;       // 搬运方向， 从内存到外设
    DMA_InitStructure.DMA_BufferSize         = cndtr;                       // 要搬运的内存的大小
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;   //  传输过程中外设的基地址不变
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;        // 传输过程中内存地址递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // 数据宽度为八位
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;     // 数据宽度为八位
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;             // 正常传输模式
    DMA_InitStructure.DMA_Priority           = DMA_Priority_Medium;         // 优先级设置
    DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;             // 没有内存到内存的传输
    DMA_Init(DMA_CHx, &DMA_InitStructure);
}

// 使能dma1的通道3，因为spi输出对应的是此通道
void DMA1_Transfer(DMA_Channel_TypeDef *DMA_CHx, uint16_t CounterSize)
{
    DMA_Cmd(DMA_CHx, DISABLE);
    DMA_SetCurrDataCounter(DMA_CHx, CounterSize);
    DMA_Cmd(DMA_CHx, ENABLE);
}
