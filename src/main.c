#include "Delay.h"
#include "stm32f10x.h" // Device header
#include "usart.h"
#include "math.h"
#include "stdio.h"
#include "stm32f10x_flash.h"
#include "stdlib.h"
#include "string.h"
#include "wdg.h"
#include "EC800.h"
#include "cJSON.h"
#include "Key.h"
#include "Encoder.h"
#include "DS3231.h"
#include "Battery.h"
#include "W25Q128.h"
#include "CCS811.h"
#include "MLX90640.h"
#include "lcd.h"
#include "remote.h"
// #include "bmp.h"
#include "LED.h"
#include <time.h>

char str[100];

uint8_t SelectReset_Flag;
uint8_t EncoderRun_Flag;
uint8_t PubData_Flag;
uint8_t Select_State = 2;
uint8_t Method       = 1;

uint8_t FPS_MLX90640         = 0;
uint8_t FPS_MLX90640_Counter = 0;

const uint8_t Method_Index[MethodNUM][3] = {{7, 8, 1}, {7, 8, 2}, {9, 10, 1}, {9, 10, 2}, {8, 11, 1}, {8, 11, 2}, {8, 11, 3}, {12, 13, 0}};

void MLX90640_RefreshMethod(ConverMethod method, uint16_t color)
{
    BACK_COLOR  = WHITE;
    POINT_COLOR = color;
    LCD_ShowSymbol(184, 212, Method_Index[method][0]);
    LCD_ShowSymbol(200, 212, Method_Index[method][1]);
    if (Method_Index[method][2])
        LCD_ShowNum(216, 212, Method_Index[method][2], 1);
    else
        LCD_Fill(216, 212, 224, 228, WHITE);
    TempPseColor_Init(method);
    Show_PseColorBar(0, 0);
}

uint8_t W25Q128_UserData[2]; // 发射率，显示方式

#define USER_DATA_ADDERSS 0xFF0000

void W25Q128_ReadUserData()
{
    W25Q128_ReadData(USER_DATA_ADDERSS, W25Q128_UserData, 2);
    Method     = W25Q128_UserData[0];
    Emissivity = W25Q128_UserData[1];
}

void W25Q128_WriteUserData()
{
    W25Q128_SectorErase(USER_DATA_ADDERSS);
    Delay_ms(20);
    W25Q128_UserData[0] = Method;
    W25Q128_UserData[1] = Emissivity;
    W25Q128_PageProgram(USER_DATA_ADDERSS, W25Q128_UserData, 2);
}

void Key_RefreshSelect()
{
    Select_State++;
    Select_State %= 3;
    BACK_COLOR = WHITE;
    switch (Select_State) {
        case 0:
            POINT_COLOR = BLACK;
            LCD_ShowSymbol(120, 212, EmissivitySymbol1);
            LCD_ShowSymbol(136, 212, EmissivitySymbol2);
            LCD_ShowSymbol(152, 212, EmissivitySymbol3);
            MLX90640_RefreshMethod((ConverMethod)Method, BLACK);
            W25Q128_WriteUserData();
            break;
        case 1:
            POINT_COLOR = RED;
            LCD_ShowSymbol(120, 212, EmissivitySymbol1);
            LCD_ShowSymbol(136, 212, EmissivitySymbol2);
            LCD_ShowSymbol(152, 212, EmissivitySymbol3);
            MLX90640_RefreshMethod((ConverMethod)Method, BLACK);

            break;
        case 2:
            POINT_COLOR = BLACK;
            LCD_ShowSymbol(120, 212, EmissivitySymbol1);
            LCD_ShowSymbol(136, 212, EmissivitySymbol2);
            LCD_ShowSymbol(152, 212, EmissivitySymbol3);
            MLX90640_RefreshMethod((ConverMethod)Method, RED);

            break;
    }
}

void Encoder_Action(int16_t num)
{
    static int16_t lastNum;
    if (num != lastNum) {
        EncoderRun_Flag = 1;
        switch (Select_State) {
            case 1:
                Emissivity += (num - lastNum) * 5;
                if (Emissivity < 5) Emissivity = 100;
                if (Emissivity > 100) Emissivity = 5;
                MLX90640_SetEmissivity(Emissivity);
                break;

            case 2:
                Method += num - lastNum;
                Method %= MethodNUM;
                MLX90640_RefreshMethod((ConverMethod)Method, RED);
                break;
            default:
                break;
        }

        lastNum = num;
    }
    EncoderRun_Flag = 0;
}

void Battery_Refresh()
{
    Battery_Flag = 0;
    BACK_COLOR   = WHITE;
    POINT_COLOR  = BLACK;
    if (Battery_UpdateLevel(EC800_GetADC())) {
        LCD_ShowSymbol(48, 212, BatterySymbol3);
        LCD_ShowSymbol(64, 212, BatterySymbol4);
        LCD_ShowSymbol(80, 212, BatterySymbol5);
        LCD_ShowSymbol(96, 212, BatterySymbol1);
    } else {
        LCD_ShowSymbol(48, 212, BatterySymbol1);
        LCD_ShowSymbol(64, 212, BatterySymbol2);
        LCD_ShowNum(80, 212, Battery_GetLevel(), 2);
        LCD_ShowSymbol(96, 212, PercentSymbol);
    }
}

// sprintf(str, "Max=%.2d Min=%.2d Average=%.2d Target=%.2d\r\n", TempData.Max / 100, TempData.Min / 100, TempData.Average / 100, TempData.Target / 100);
// Debug_printf(str);

char *Time_Str;
char s[20];
uint16_t Key;


int main(void)
{

    Delay_ms(100);
    Uart_Init(115200);
    Debug_printf("Debug_print OK\r\n");
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
    LED_Init();
    Encoder_Init();
    Key_Init();
    W25Q128_Init();
    // DS3231_Init();
    // CCS811_Init();
    LCD_Init(); // 初始化TFT
    W25Q128_ReadUserData();
    LCD_Clear(WHITE); // 清屏
    // EC800_Init();
    // MQTT_Init();
    MLX90640_SendInitCMD();
    TempPseColor_Init(GCM_Pseudo2);
    Show_PseColorBar(0, 0);
    Key_RefreshSelect();
    Debug_printf("InitOK!\r\n");
    Remote_Init(REMOTE_Common_Verify);
    DS3231_Init();
    while (1)
    {


LCD_ShowNum(40, 40,Time_Year,3);
LCD_ShowNum(80, 40,Time_Mon,3);
LCD_ShowNum(120, 40,Time_Day,3);

LCD_ShowNum(40, 60,Time_Week,3);

LCD_ShowNum(40, 80,Time_Hour,3);
LCD_ShowNum(80, 80,Time_Min,3);
LCD_ShowNum(120, 80,Time_Sec,3);
DS3231_ReadTime();


Time_Str=asctime(&Time_Date);
LCD_ShowString(20,100,Time_Str);

time_t time_cnt=DS3231_GetTimeStamp();
Time_Str=ctime(&time_cnt);
LCD_ShowString(20,120,Time_Str);

sprintf(s,"%d",DS3231_GetTimeStamp());
LCD_ShowString(20,140,s);


Delay_ms(300);
    }
//秒(0)分(1)时(2)日(3)月(4)周(5)年(6)
while (1)
{
            if (Key_Get()) {
            Remote_Transmit(REMOTE_ID,123);
        }
        LCD_ShowString(30, 20,"Command");
        LCD_ShowNum(40, 40,Remote_GetCommand(),3);
        Delay_ms(300);

}


    while (1) {

        LED_Turn();
        TIM_Cmd(TIM3, ENABLE);
        if (Key_Get()) {
            Key_RefreshSelect();
            Delay_ms(300);
        }

        Key = Remote_GetCommand();
        if (Key) { Encoder_Set(Key); }

        Encoder_Action(Encoder_Get());

        if (MLX90640_RefreshData()) {
            Show_TempRaw(8, 208);
            Show_TempBilinearInter(0, BAR, &TempData);
            FPS_MLX90640_Counter++;
        }

        if (Battery_Flag) Battery_Refresh();

        if (SelectReset_Flag) {
            SelectReset_Flag = 0;
            Key_RefreshSelect();
        }

        if (PubData_Flag) {
            PubData_Flag = 0;
            Data.Temp    = TempData.Target / 100;
            // Pub_Data(MQTT_Topic);
        }
    }
}

void TIM1_UP_IRQHandler(void)
{
    static uint16_t key_Counter = 0, battery_Counter = 0, select_Counter = 0, pubDat_Counter = 0;
    static uint16_t fps_MLX90640_Counter = 0;
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET) {
        key_Counter++;
        battery_Counter++;
        pubDat_Counter++;
        fps_MLX90640_Counter++;
        if (Select_State && !EncoderRun_Flag)
            select_Counter++;
        else
            select_Counter = 0;
        if (key_Counter >= 5) {
            key_Counter = 0;
            Key_Entry();
        }
        if (battery_Counter >= 1000) {
            battery_Counter = 0;
            Battery_Flag    = 1;
        }
        if (select_Counter >= 500) {
            select_Counter   = 0;
            Select_State     = 2;
            SelectReset_Flag = 1;
        }
        if (pubDat_Counter >= 500) {
            pubDat_Counter = 0;
            PubData_Flag   = 1;
        }
        if (fps_MLX90640_Counter >= 100) {
            fps_MLX90640_Counter = 0;
            FPS_MLX90640         = FPS_MLX90640_Counter;
            FPS_MLX90640_Counter = 0;
        }
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
}
