#include "remote.h"

// 0: 空闲状态
// 1: 起始状态
// 2: 解码状态
static uint8_t State      = 0;
static uint8_t RepeatFlag = 0, DateIndex = 0, ReadyFlag = 0;
static uint16_t Time;
static uint16_t Address, Command;
static uint8_t AddressData[REMOTE_ADDRESS_LEN - 1];
static uint8_t CommandData[REMOTE_COMMAND_LEN - 1];
uint8_t Remote_RepeatCounter = 0;

// 红外遥控初始化 设置IO以及定时器2的输入捕获
void Remote_Init(void)
{
    // 使能AFIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    // TIM2部分重映射
    GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 使能PORTB时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  // TIM4 时钟使能

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15;     // PA15 输入
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_OD; // 上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA, GPIO_Pin_15);

    TIM_TimeBaseStructure.TIM_Period        = 20000;              // 设定计数器自动重装值 最大10ms溢出
    TIM_TimeBaseStructure.TIM_Prescaler     = (72 - 1);           // 预分频器,1M的计数频率,1us加1.
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;       // 设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up; // TIM向上计数模式

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); // 根据指定的参数初始化TIMx

    TIM_ICInitStructure.TIM_Channel     = TIM_Channel_1;          // 选择输入端 IC1映射到TI1上
    TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Falling; // 上升沿捕获
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; // 配置输入分频,不分频
    TIM_ICInitStructure.TIM_ICFilter    = 0x03;           // IC1F=0011 配置输入滤波器 8个定时器时钟周期滤波
    TIM_ICInit(TIM2, &TIM_ICInitStructure);               // 初始化定时器输入捕获通道

    TIM_Cmd(TIM2, ENABLE); // 使能定时器2

    NVIC_InitStructure.NVIC_IRQChannel                   = TIM2_IRQn; // TIM2中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;         // 先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 3;         // 从优先级3级
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;    // IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);                                   // 根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

    TIM_ITConfig(TIM2, TIM_IT_Update | TIM_IT_CC1, ENABLE); // 允许更新中断 ,允许CC4IE捕获中断
}

// 定时器2中断服务程序
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) // 计时器更新中断
    {
        State = REST;
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
    if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET) // 捕获下降沿中断                                                断
    {
        LED_Turn();
        TIM_SetCounter(TIM2, 0);
        switch (State) { // 状态机
            case REST:
                State = BEGIN;
                break;
            case BEGIN:
                Time = TIM_GetCapture1(TIM2);
                if (Time > (START_TIME - ERROR_TIME) && Time < (START_TIME + ERROR_TIME)) { // 识别起始信号
                    State                = RUN;
                    DateIndex            = 0;
                    RepeatFlag           = 0;
                    Remote_RepeatCounter = 0;
                } else if (RepeatFlag && Time > (REPEAT_TIME - ERROR_TIME) && Time < (REPEAT_TIME + ERROR_TIME)) { // 识别重复信号
                    Remote_RepeatCounter++;
                    State = REST;
                } else // 错误处理
                    State = REST;
                break;
            case RUN:
                Time      = TIM_GetCapture1(TIM2);
                ReadyFlag = 0;
                if (Time > (LOW_TIME - ERROR_TIME) && Time < (LOW_TIME + ERROR_TIME)) { // 识别低电平
                    if (DateIndex >= REMOTE_ADDRESS_LEN) {
                        CommandData[DateIndex - REMOTE_ADDRESS_LEN] = 0;
                        DateIndex++;
                    } else {
                        AddressData[DateIndex] = 0;
                        DateIndex++;
                    }
                } else if (Time > (HIGH_TIME - ERROR_TIME) && Time < (HIGH_TIME + ERROR_TIME)) { // 识别高电平
                    if (DateIndex >= REMOTE_ADDRESS_LEN) {
                        CommandData[DateIndex - REMOTE_ADDRESS_LEN] = 1;
                        DateIndex++;
                    } else {
                        AddressData[DateIndex] = 1;
                        DateIndex++;
                    }
                } else { // 错误处理
                    DateIndex = 0;
                    State     = REST;
                }
                if (DateIndex >= (REMOTE_ADDRESS_LEN + REMOTE_COMMAND_LEN)) { // 完整接收
                    DateIndex  = 0;
                    RepeatFlag = 1;
                    ReadyFlag  = 1;
                    State      = REST;
                }
                break;
        }
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
    }
}

uint8_t Remote_Verify(void)
{
    uint16_t address = 0, in_address = 0;
    uint16_t command = 0, in_command = 0;
    in_address |= 0xffff << (REMOTE_COMMAND_LEN / 2); // 补位
    in_command |= 0xffff << (REMOTE_COMMAND_LEN / 2);
    for (uint8_t i = 0; i < (REMOTE_ADDRESS_LEN / 2); i++) { // 数据处理
        address |= AddressData[i] << i;
        in_address |= AddressData[i + (REMOTE_ADDRESS_LEN / 2)] << i;
    }
    for (uint8_t i = 0; i < (REMOTE_COMMAND_LEN / 2); i++) {
        command |= CommandData[i] << i;
        in_command |= CommandData[i + (REMOTE_COMMAND_LEN / 2)] << i;
    }
    if (REMOTE_ADDRESS_VERIFY && address != (uint16_t)~in_address) return 0; // 校验
    if (REMOTE_COMMAND_VERIFY && command != (uint16_t)~in_command) return 0;
    Address = address;
    Command = command;
    return 1;
}

uint16_t Remote_GetAddress(void)
{
    return Address;
}

uint16_t Remote_GetCommand(void)
{
    if (ReadyFlag) { // 接收完成
        ReadyFlag = 0;
        if (Remote_Verify()) {                               // 数据处理校验
            if (!(REMOTE_ID_VERIFY) || Address == REMOTE_ID) // 遥控器ID校验
                return Command;
        }
    }
    return 0;
}