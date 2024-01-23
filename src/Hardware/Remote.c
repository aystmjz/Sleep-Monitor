#include "remote.h"

// 0: ����״̬
// 1: ��ʼ״̬
// 2: ����״̬
static uint8_t State      = 0;
static uint8_t RepeatFlag = 0, DateIndex = 0, ReadyFlag = 0;
static uint16_t Time;
static uint16_t Address, Command;
static uint8_t AddressData[REMOTE_ADDRESS_LEN - 1];
static uint8_t CommandData[REMOTE_COMMAND_LEN - 1];
uint8_t Remote_RepeatCounter = 0;

// ����ң�س�ʼ�� ����IO�Լ���ʱ��2�����벶��
void Remote_Init(void)
{
    // ʹ��AFIOʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    // TIM2������ӳ��
    GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // ʹ��PORTBʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  // TIM4 ʱ��ʹ��

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15;     // PA15 ����
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_OD; // ��������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA, GPIO_Pin_15);

    TIM_TimeBaseStructure.TIM_Period        = 20000;              // �趨�������Զ���װֵ ���10ms���
    TIM_TimeBaseStructure.TIM_Prescaler     = (72 - 1);           // Ԥ��Ƶ��,1M�ļ���Ƶ��,1us��1.
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;       // ����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up; // TIM���ϼ���ģʽ

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); // ����ָ���Ĳ�����ʼ��TIMx

    TIM_ICInitStructure.TIM_Channel     = TIM_Channel_1;          // ѡ������� IC1ӳ�䵽TI1��
    TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Falling; // �����ز���
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; // ���������Ƶ,����Ƶ
    TIM_ICInitStructure.TIM_ICFilter    = 0x03;           // IC1F=0011 ���������˲��� 8����ʱ��ʱ�������˲�
    TIM_ICInit(TIM2, &TIM_ICInitStructure);               // ��ʼ����ʱ�����벶��ͨ��

    TIM_Cmd(TIM2, ENABLE); // ʹ�ܶ�ʱ��2

    NVIC_InitStructure.NVIC_IRQChannel                   = TIM2_IRQn; // TIM2�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;         // ��ռ���ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 3;         // �����ȼ�3��
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;    // IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);                                   // ����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

    TIM_ITConfig(TIM2, TIM_IT_Update | TIM_IT_CC1, ENABLE); // ��������ж� ,����CC4IE�����ж�
}

// ��ʱ��2�жϷ������
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) // ��ʱ�������ж�
    {
        State = REST;
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
    if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET) // �����½����ж�                                                ��
    {
        LED_Turn();
        TIM_SetCounter(TIM2, 0);
        switch (State) { // ״̬��
            case REST:
                State = BEGIN;
                break;
            case BEGIN:
                Time = TIM_GetCapture1(TIM2);
                if (Time > (START_TIME - ERROR_TIME) && Time < (START_TIME + ERROR_TIME)) { // ʶ����ʼ�ź�
                    State                = RUN;
                    DateIndex            = 0;
                    RepeatFlag           = 0;
                    Remote_RepeatCounter = 0;
                } else if (RepeatFlag && Time > (REPEAT_TIME - ERROR_TIME) && Time < (REPEAT_TIME + ERROR_TIME)) { // ʶ���ظ��ź�
                    Remote_RepeatCounter++;
                    State = REST;
                } else // ������
                    State = REST;
                break;
            case RUN:
                Time      = TIM_GetCapture1(TIM2);
                ReadyFlag = 0;
                if (Time > (LOW_TIME - ERROR_TIME) && Time < (LOW_TIME + ERROR_TIME)) { // ʶ��͵�ƽ
                    if (DateIndex >= REMOTE_ADDRESS_LEN) {
                        CommandData[DateIndex - REMOTE_ADDRESS_LEN] = 0;
                        DateIndex++;
                    } else {
                        AddressData[DateIndex] = 0;
                        DateIndex++;
                    }
                } else if (Time > (HIGH_TIME - ERROR_TIME) && Time < (HIGH_TIME + ERROR_TIME)) { // ʶ��ߵ�ƽ
                    if (DateIndex >= REMOTE_ADDRESS_LEN) {
                        CommandData[DateIndex - REMOTE_ADDRESS_LEN] = 1;
                        DateIndex++;
                    } else {
                        AddressData[DateIndex] = 1;
                        DateIndex++;
                    }
                } else { // ������
                    DateIndex = 0;
                    State     = REST;
                }
                if (DateIndex >= (REMOTE_ADDRESS_LEN + REMOTE_COMMAND_LEN)) { // ��������
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
    in_address |= 0xffff << (REMOTE_COMMAND_LEN / 2); // ��λ
    in_command |= 0xffff << (REMOTE_COMMAND_LEN / 2);
    for (uint8_t i = 0; i < (REMOTE_ADDRESS_LEN / 2); i++) { // ���ݴ���
        address |= AddressData[i] << i;
        in_address |= AddressData[i + (REMOTE_ADDRESS_LEN / 2)] << i;
    }
    for (uint8_t i = 0; i < (REMOTE_COMMAND_LEN / 2); i++) {
        command |= CommandData[i] << i;
        in_command |= CommandData[i + (REMOTE_COMMAND_LEN / 2)] << i;
    }
    if (REMOTE_ADDRESS_VERIFY && address != (uint16_t)~in_address) return 0; // У��
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
    if (ReadyFlag) { // �������
        ReadyFlag = 0;
        if (Remote_Verify()) {                               // ���ݴ���У��
            if (!(REMOTE_ID_VERIFY) || Address == REMOTE_ID) // ң����IDУ��
                return Command;
        }
    }
    return 0;
}