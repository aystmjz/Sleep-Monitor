#include "usart.h"

// 加入以下代码,支持printf函数,而不需要选择use MicroLIB

#if 1

#pragma import(__use_no_semihosting) // 标准库需要的支持函数
struct __FILE {
    int handle;
};
FILE __stdout;
void _sys_exit(int x) // 定义_sys_exit()以避免使用半主机模式
{
    x = x;
}
int fputc(int ch, FILE *f) // 重定义fputc函数
{
    while ((USART2->SR & 0X40) == 0) {} // 循环发送,直到发送完毕
    USART2->DR = (u8)ch;
    return ch;
}

#endif

char RxCounter, RxBuffer[USART_REC_LEN]; // 接收缓冲

/// @brief 初始化串口1(MLX)
/// @param bound 波特率
void uart1_init(uint32_t bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
    USART_DeInit(USART1);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP; // 复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel                   = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;      // 抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 3;      // 子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE; // IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);

    USART_InitStructure.USART_BaudRate            = bound;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;            // 字长为8位数据格式
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;               // 一个停止位
    USART_InitStructure.USART_Parity              = USART_Parity_No;                // 无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件数据流控制
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;  // 收发模式
    USART_Init(USART1, &USART_InitStructure);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);
}

/// @brief 初始化串口2(EC)
/// @param bound 波特率
void uart2_init(uint32_t bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    USART_DeInit(USART2); // 复位串口2
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP; // 复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel                   = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;      // 抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;      // 子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE; // IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);

    USART_InitStructure.USART_BaudRate            = bound;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;            // 字长为8位数据格式
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;               // 一个停止位
    USART_InitStructure.USART_Parity              = USART_Parity_No;                // 无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件数据流控制
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;  // 收发模式
    USART_Init(USART2, &USART_InitStructure);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART2, ENABLE);
}

/// @brief 初始化串口3(DEBUG)
/// @param bound 波特率
void uart3_init(uint32_t bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    USART_DeInit(USART3); // 复位串口3

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP; // 复用推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel                   = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;      // 抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2;      // 子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE; // IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);

    USART_InitStructure.USART_BaudRate            = bound;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;            // 字长为8位数据格式
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;               // 一个停止位
    USART_InitStructure.USART_Parity              = USART_Parity_No;                // 无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件数据流控制
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;  // 收发模式
    USART_Init(USART3, &USART_InitStructure);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART3, ENABLE);
}

/// @brief 初始化总串口
/// @param bound 波特率
void Uart_Init(uint32_t bound)
{
    uart1_init(bound);
    uart2_init(bound);
    uart3_init(bound);
}

// 串口2打印数据
void Uart2_SendDATA(u8 *SendBuf, u8 len)
{
    u8 i;
    for (i = 0; i < len; i++) {
        while ((USART2->SR & 0X40) == 0) {} // 等待发送完成
        USART2->DR = (u8)SendBuf[i];
    }
}

// DEBUG串口打印
void Uart3_SendStr(char *SendBuf)
{
    while (*SendBuf) {
        while ((USART3->SR & 0X40) == 0) {} // 等待发送完成
        USART3->DR = (u8)*SendBuf;
        SendBuf++;
    }
}

//MLX串口发送一个字节数据
void USART1_Send_byte(uint8_t byte)
{
    while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);//等待发送完成
    USART2->DR=byte;
}
//MLX串口发送多字节数据
void USART1_Send_bytes(uint8_t *Buffer, uint8_t Length)
{
    uint8_t i=0;
    while(i<Length)
    {
        USART1_Send_byte(Buffer[i++]);
    }
}

extern uint8_t RX_BUF[1550];
uint8_t rebuf[1550] = {0};
// MLX串口中断
void USART1_IRQHandler(void)
{
    static uint16_t i             = 0;
    static uint16_t receiveLength = 0;
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        rebuf[i++] = USART_ReceiveData(USART1);
        if (rebuf[0] != 0x5a) // 判断帧头
            i = 0;
        if ((i == 2) && (rebuf[1] != 0x5a)) // 判断帧头
            i = 0;
        if (i == 4 && (rebuf[3] == 0x06)) {
            receiveLength = ((uint16_t)rebuf[3] << 8) | rebuf[2];
        }
        if (i > 4) // 当i计数值=5时，功能字节接受完毕，数据长度字节接收完毕
        {
            if (i == receiveLength + 6) {

                memcpy(RX_BUF, rebuf, i);
                //stata = 1;
                i     = 0;
            }
        }
        USART_ClearFlag(USART1, USART_FLAG_RXNE); // 清中断标志
    }
}

// MC串口中断
void USART2_IRQHandler(void)
{

    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
        RxBuffer[RxCounter++] = USART_ReceiveData(USART2);
    }
}

// DEBUG串口中断
void USART3_IRQHandler(void)
{
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
        while ((USART2->SR & 0X40) == 0) {}
        USART2->DR = (u8)USART_ReceiveData(USART3);
        ;
    }
}
