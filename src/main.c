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
char *topic = "aystmjz/topic/hxd";
#include "oled.h"
#include "bmp.h"
#include "LED.h"
u8 ref = 0;                 // 刷新显示
u16 vx = 15542, vy = 11165; // 比例因子，此值除以1000之后表示多少个AD值代表一个像素点
u16 chx = 140, chy = 146;   // 默认像素点坐标为0时的AD起始值
void xianshi()              // 显示信息
{
    u16 lx, ly;
    BACK_COLOR  = WHITE;
    POINT_COLOR = RED;
    showhanzi(10, 0, 3); // 电
    showhanzi(45, 0, 4); // 子

    LCD_ShowString(10, 35, "1.3 IPS-SPI 240*320");
    LCD_ShowString(10, 55, "LCD_W:");
    LCD_ShowNum(70, 55, LCD_W, 3);
    LCD_ShowString(110, 55, "LCD_H:");
    LCD_ShowNum(160, 55, LCD_H, 3);
    lx = 10;
    ly = 75;
}

#define Image_X 132
#define Image_Y 150
#define Image_L 50

void showimage() // 显示40*40图片
{

    Address_set(Image_L, Image_L, Image_L + Image_X - 1, Image_L + Image_Y - 1); // 坐标设置
    for (int i = 0; i < 39600 / 2; i++) {

        LCD_WR_DATA8(gImage_qmx[i * 2 + 1]);
        LCD_WR_DATA8(gImage_qmx[i * 2]);
    }
    ref = 0;
}

void showimage_() // 显示40*40图片
{
    int i, j, k;

    for (k = 2; k < 6; k++) {
        for (j = 0; j < 6; j++) {
            Address_set(40 * j, 40 * k, 40 * j + 39, 40 * k + 39); // 坐标设置
            for (i = 0; i < 1600; i++) {

                LCD_WR_DATA8(image[i * 2 + 1]);
                LCD_WR_DATA8(image[i * 2]);
            }
        }
    }
    ref = 0;
}

uint8_t TEST = 0;
MessageTypeDef Data;

void Pub_Data(char *topic)
{
    cJSON *Data_Json = NULL;
    Data_Json        = cJSON_CreateObject();
    cJSON_AddNumberToObject(Data_Json, "Temp", Data.Temp);
    cJSON_AddNumberToObject(Data_Json, "Hum", Data.Hum);
    cJSON_AddNumberToObject(Data_Json, "Light", Data.Light);
    if (Data.Beep)
        cJSON_AddTrueToObject(Data_Json, "Beep");
    else
        cJSON_AddFalseToObject(Data_Json, "Beep");
    if (Data.Led)
        cJSON_AddTrueToObject(Data_Json, "Led");
    else
        cJSON_AddFalseToObject(Data_Json, "Led");
    Pub_Json(topic, Data_Json);
    cJSON_Delete(Data_Json);
}

extern uint16_t Battery;
extern uint8_t BatteryFlag;

extern uint8_t TIME[7]; // 秒(0)分(1)时(2)日(3)月(4)周(5)年(6)

typedef struct {
    uint16_t eco2;
    uint16_t tvoc;
    uint8_t status;
    uint8_t error_id;
    uint16_t raw_data;
} CCS811_Data_t;

CCS811_Data_t CCS;


uint16_t temp1         = 0;

void CheckData()
{	float Temperature, Max = -40, Min = 400, Sum = 0;
	uint8_t y_p = 0,data_buf[1550] = {0};
    if (MLX90640_CheckData(data_buf)) {
        for (int8_t y = 23; y >= 0; y--) {
            uint8_t x_p = 0;
            for (uint8_t x = 4; x < 64 + 4; x += 2) {
                int16_t Temp = ((int16_t)data_buf[x + 1 + 64 * y] << 8 | data_buf[x + 64 * y]);
                Temperature  = (float)Temp / 100;
                if (Temperature < Min) Min = Temperature;
                if (Temperature > Max) Max = Temperature;
                Sum += Temperature;
                TempData.Raw[x_p][y_p] = Temperature;
                x_p++;
            }
            y_p++;
        }
		TempData.Max=Max;
		TempData.Min=Min;
        TempData.Average = Sum / (32 * 24);
        TempData.Target  = (TempData.Raw[16][12] + TempData.Raw[15][12] + TempData.Raw[16][11] + TempData.Raw[15][11]) / 4;
	}
}

int main(void)
{
    int16_t Encoder_Num = 0;
    Delay_ms(50);
    Uart_Init(115200);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
    Debug_printf("Debug_print OK\r\n");

    Encoder_Init();
    Key_Init();
    W25Q128_Init();
    CCS811_Init();
    Lcd_Init();       // 初始化TFT
    LCD_Clear(WHITE); // 清屏
    BACK_COLOR = BLACK;

    POINT_COLOR = WHITE;
    MLX90640_SendInitCMD();
    Debug_printf("OK!!!\r\n");
    // xianshi(); //显示信息
    // showimage(); //显示40*40图片

    while (1) {
        Delay_ms(1000);
    	CheckData();

        for (uint8_t i = 0; i < 24; i++) {
            for (uint8_t j = 0; j < 32; j++) {
                if (TempData.Raw[j][i] >= 30) {
                    Debug_printf(" #");
                } else
                    Debug_printf(" ");
                Delay_ms(2);
            }
            Debug_printf(" \r\n");
        }

        Delay_ms(2);
        Debug_printf(" \r\n++++++++++++++++++++++++++++++++++++++++++++\r\n");
        char str[20] = {0};
        sprintf(str, "Max=%.2f Min=%.2f Average=%.2f Target=%.2f\r\n", TempData.Max, TempData.Min, TempData.Average, TempData.Target);
        Debug_printf(str);
    }

    LED_Init();
    DS3231_Init();
    // EC800_Init();
    // MQTT_Init();
    showimage();
    BACK_COLOR  = WHITE;
    POINT_COLOR = DARKBLUE;

    uint8_t CCS811_BUF[12];
    uint8_t CCS811_Information[10];
    uint8_t CCS811_temp = 0x5a;
    uint8_t CCS811_MeasureMode, CCS811_Status, CCS811_Error_ID;

    CCS811_Wake();                                                   // nWAKE pin is asserted at least 50μs before the transaction and kept asserted throughout,nWAKE pin is active low
    CCS811_Information[0] = CCS811_ReadByte(HW_ID_REG);              // Read CCS's information  ,ID
    CCS811_MultiReadByte(0x23, &CCS811_Information[1], 2);           // FW_Boot_Version
    CCS811_MultiReadByte(0x24, &CCS811_Information[3], 2);           // FW_App_Version
    CCS811_Status = CCS811_ReadByte(STATUS_REG);                     // Firstly the status register is read and the APP_VALID flag is checked.
    if (CCS811_Status & 0x10) CCS811_WriteByte(APP_START_REG, 0x00); // Used to transition the CCS811 state from boot to application mode, a write with no data is required.
    CCS811_Status      = CCS811_ReadByte(STATUS_REG);
    CCS811_MeasureMode = CCS811_ReadByte(MEAS_MODE_REG);
    CCS811_WriteByte(MEAS_MODE_REG, 0x20); // Write Measure Mode Register,sensor measurement every second,no interrupt
    CCS811_Sleep();

    while (1) {
        Delay_ms(500);
        LED1_Turn();
        Encoder_Num = Encoder_Get();
        // TIME[0]=DS3231_ReadByte(0x04);
        LCD_ShowNum(100, 200, Encoder_Num, 5);

        LCD_ShowNum(210, 0, Get_BatteryLevel(), 2);
        if (BatteryFlag) {
            BatteryFlag = 0;
            Update_BatteryLevel(Get_ADC());
        }

        CCS811_Wake(); // nWAKE pin is asserted at least 50μs before the transaction and kept asserted throughout,nWAKE pin is active low
        CCS811_Status == CCS811_ReadByte(STATUS_REG);
        CCS811_Error_ID == CCS811_ReadByte(ERROR_ID_REG);
        // CCS811_WriteByte(MEAS_MODE_REG, DRIVE_MODE_1SEC);
        CCS811_MeasureMode = CCS811_ReadByte(MEAS_MODE_REG);
        CCS811_MultiReadByte(ALG_RESULT_DATA, CCS811_BUF, 8);
        CCS811_Information[0] = CCS811_ReadByte(HW_ID_REG); // Read CCS's information  ,ID
        CCS811_Sleep();
        CCS.eco2 = (uint16_t)CCS811_BUF[0] * 256 + CCS811_BUF[1];
        CCS.tvoc = (uint16_t)CCS811_BUF[2] * 256 + CCS811_BUF[3];
        LCD_ShowNum(0, 0, CCS.eco2, 4);
        LCD_ShowNum(60, 0, CCS.tvoc, 4);
        LCD_ShowNum(120, 0, CCS811_Information[0], 4);
        // CCS811_Information[0] = 0;
    }

    while (1) {

        Delay_ms(500);
        LED1_Turn();
        Encoder_Num = Encoder_Get();
        DS3231_ReadTime();

        // TIME[0]=DS3231_ReadByte(0x04);
        LCD_ShowNum(160, 75, Encoder_Num, 5);

        LCD_ShowNum(160, 95, Key_Get(), 3);
        LCD_ShowNum(160, 115, Battery * 4, 4);
        LCD_ShowNum(160, 135, Battery_calculate(Battery * 4), 2);

        char len[20];
        sprintf(len, "TIME[0]:%d\n", TIME[0]);
        Debug_printf(len);
        LCD_ShowNum(20, 135, TIME[0], 2);
        LCD_ShowNum(40, 135, TIME[1], 2);
        LCD_ShowNum(60, 135, TIME[2], 2);
        LCD_ShowNum(80, 135, TIME[3], 2);
        LCD_ShowNum(100, 135, TIME[4], 2);
        LCD_ShowNum(120, 135, TIME[5], 2);
        LCD_ShowNum(140, 135, TIME[6], 2);
        uint8_t M;
        uint16_t D;
        uint8_t data[5], num[5] = {12, 22, 13};
        W25Q128_ReadID(&M, &D);
        LCD_ShowNum(120, 155, M, 2);
        LCD_ShowNum(140, 155, D, 2);
        W25Q128_SectorErase(0x000000);
        Delay_ms(20);
        W25Q128_PageProgram(0x000000, num, 3);
        Delay_ms(20);
        W25Q128_ReadData(0x000000, data, 3);
        LCD_ShowNum(100, 175, num[0], 2);
        LCD_ShowNum(120, 175, num[1], 2);
        LCD_ShowNum(140, 175, num[2], 2);
        LCD_ShowNum(100, 195, data[0], 2);
        LCD_ShowNum(120, 195, data[1], 2);
        LCD_ShowNum(140, 195, data[2], 2);

        if (BatteryFlag) {
            BatteryFlag = 0;
            Update_BatteryLevel(Get_ADC());
        }
        // Pub_Data(topic);
    }
}

void TIM2_IRQHandler(void)
{
    static uint16_t Key_Counter = 0, Battery_Counter = 0;
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) {
        Key_Counter++;
        Battery_Counter++;
        if (Key_Counter >= 50) {
            Key_Counter = 0;
            Key_Entry();
        }
        if (Battery_Counter >= 5000) {
            Battery_Counter = 0;
            BatteryFlag     = 1;
        }
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}

/*

char* topic="aystmjz/topic/hxd";

 int mmain(void)
 {



        int temp,humi;
        uint32_t CT_data[2];
        //delay_init();	    	 //延时函数初始化
        NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
        LED_Init();		  		//初始化与LED连接的硬件接口
        uart_init(115200);//串口1初始化，打印调试信息
        uart2_init(115200);//串口2初始化，对接EC800S

    printf("OK\r\n");



//IWDG_Init(7,625);//8S开门狗复位一次

        printf("+++");
        Delay_ms(500);
        printf("+++");
        Delay_ms(500);
    I2C_Bus_Init();
        while(!AHT20_Init())//判断AHT20是否存在
        {
            Uart1_SendStr("AHT20 ERROR!\r\n");
            Delay_ms(500);
        }
        EC800_Init(); //EC800初始化,设置为命令模式TCP
      MQTT_Init();
      cJSON* Data_Json = NULL;
        while(1)
        {
            //读取 AHT20 传感器数据
            while(AHT20_Read_Cal_Enable() == 0)
            {
                    AHT20_Init();//如果为0再使能一次
                    Delay_ms(30);
            }
            AHT20_Read_CTdata(CT_data);  //读取温度和湿度
            humi = CT_data[0] * 1000 / 1024 / 1024;  //计算得到湿度值（放大了10倍,如果c1=523，表示现在湿度为52.3%）
            temp = CT_data[1] * 200 *10 / 1024 / 1024 - 500;//计算得到温度值（放大了10倍，如果t1=245，表示现在温度为24.5℃）

    Data_Json = cJSON_CreateObject();
    cJSON_AddNumberToObject(Data_Json, "Temp", temp/10);
    cJSON_AddNumberToObject(Data_Json, "Hum", humi/10);
    cJSON_AddNumberToObject(Data_Json, "Light", 6340);
    cJSON_AddFalseToObject(Data_Json, "Led");
    cJSON_AddTrueToObject(Data_Json, "Beep");

            //Pub_Msgdata(temp/10,humi/10);
            Pub_Json(topic,Data_Json);
            cJSON_Delete(Data_Json);
            Delay_ms(500);

            IWDG_Feed();

        }






}

*/
