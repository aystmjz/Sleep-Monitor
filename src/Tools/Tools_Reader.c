#include "Tools_Reader.h"

struct Option_Class Reader_Option_List[];
extern char Temp_str[40];

struct Tools_Reader_Class {
    uint16_t Year;
    uint16_t Mon;
    uint16_t Day;
    uint16_t Hour;
    uint16_t Min;
    uint16_t Sec;
    struct tm Time;
    struct Menu_Class MU;
} Reader;

struct Option_Class Reader_Option_List[] = {
    {"<<<"},
    {"关闭", Menu_TurnOffMenu},
    {"年:%d", Disable_MUReadKey_Mouse, &Reader.Year},
    {"月:%d", Disable_MUReadKey_Mouse, &Reader.Mon},
    {"日:%d", Disable_MUReadKey_Mouse, &Reader.Day},
    {"时:%d", Disable_MUReadKey_Mouse, &Reader.Hour},
    {"分:%d", Disable_MUReadKey_Mouse, &Reader.Min},
    {"秒:%d", Disable_MUReadKey_Mouse, &Reader.Sec},
    {".."},
};
char Temp_st[40];
void Tools_Reader(void)
{
    static struct Menu_Class Menu_ReaderMenu = {single, 1};
    uint8_t Key_Temp;
    int8_t Encoder_Temp;
    uint8_t data[16];
    uint32_t block_index;
    if (Menu_ReaderMenu.isInit) {
        Menu_MenuClassInit(&Menu_ReaderMenu, Reader_Option_List);
        DS3231_ReadTime();
        Reader.Year         = Time_Year;
        Reader.Mon          = Time_Mon;
        Reader.Day          = Time_Day;
        Reader.Hour         = Time_Hour;
        Reader.Min          = Time_Min;
        Reader.Sec          = Time_Sec / 10 * 10;
        Reader.Time.tm_year = Time_Year - 1900;
        Reader.Time.tm_mon  = Time_Mon - 1;
        Reader.Time.tm_mday = Time_Day;
        Reader.Time.tm_hour = Time_Hour;
        Reader.Time.tm_min  = Time_Min;
        Reader.Time.tm_sec  = Time_Sec;
    }

    if (Menu_ReaderMenu.isReadKey == 0) // 如果菜单不读取按键则代表抓住了选项;
    {
        Key_Temp     = Menu_EnterEvent();
        Encoder_Temp = Menu_RollEvent();
        if (Encoder_Temp) {
            switch (Menu_ReaderMenu.Cat_i) // 抓住的选项
            {
                case 2:
                    if (Check_date(Reader.Year + Encoder_Temp, Reader.Mon, Reader.Day))
                        Reader.Year += Encoder_Temp;
                    break;
                case 3:
                    if (Check_date(Reader.Year, Reader.Mon + Encoder_Temp, Reader.Day))
                        Reader.Mon += Encoder_Temp;
                    break;
                case 4:
                    if (Check_date(Reader.Year, Reader.Mon, Reader.Day + Encoder_Temp))
                        Reader.Day += Encoder_Temp;
                    break;
                case 5:
                    if ((Reader.Hour + Encoder_Temp) < 0)
                        Reader.Hour = 23;
                    else if ((Reader.Hour + Encoder_Temp) > 23)
                        Reader.Hour = 0;
                    else
                        Reader.Hour += Encoder_Temp;
                    break;
                case 6:
                    if ((Reader.Min + Encoder_Temp) < 0)
                        Reader.Min = 59;
                    else if ((Reader.Min + Encoder_Temp) > 59)
                        Reader.Min = 0;
                    else
                        Reader.Min += Encoder_Temp;
                    break;
                case 7:
                    if ((Reader.Sec + Encoder_Temp * 10) < 0) {
                        Reader.Sec = 50;
                        if (Reader.Min != 0) Reader.Min--;
                    } else if ((Reader.Sec + Encoder_Temp * 10) > 50) {
                        Reader.Sec = 0;
                        if (Reader.Min != 59) Reader.Min++;
                    } else
                        Reader.Sec += Encoder_Temp * 10;
                    break;
                default:
                    break;
            }
            Reader.Time.tm_year = Reader.Year - 1900;
            Reader.Time.tm_mon  = Reader.Mon - 1;
            Reader.Time.tm_mday = Reader.Day;
            Reader.Time.tm_sec  = Reader.Sec;

            block_index = W25Q128_SearchBlock(mktime(&Reader.Time) - 8 * 60 * 60);
            if (block_index) {
                W25Q128_ReadData((block_index << 16) | TimeDate_ToAddress(Reader.Hour, Reader.Min, Reader.Sec),
                                 data, DATA_LEN);
                if ((data[5] != 0 || data[6] != 0) && data[0] == 0) {
                    sprintf(Temp_str, "Lux=%d.%d                ", data[5], data[6]);
                    LCD_ShowString(20, 20, (const uint8_t *)Temp_str);
                } else {
                    sprintf(Temp_str, "No Date                  ");
                    LCD_ShowString(20, 20, (const uint8_t *)Temp_str);
                }
                if ((data[1] != 0 || data[2] != 0) && data[0] == 0) {
                    sprintf(Temp_str, "Temp=%d.%d  Hum=%d.%d    ", data[1], data[2], data[3], data[4]);
                    LCD_ShowString(20, 40, (const uint8_t *)Temp_str);
                } else {
                    sprintf(Temp_str, "No Date                  ");
                    LCD_ShowString(20, 40, (const uint8_t *)Temp_str);
                }
                if ((data[7] != 0 || data[8] != 0) && data[0] == 0) {
                    sprintf(Temp_str, "eco2=%d  tvoc=%d     ", (uint16_t)data[7] << 8 | data[8], (uint16_t)data[9] << 8 | data[10]);
                    LCD_ShowString(20, 60, (const uint8_t *)Temp_str);
                } else {
                    sprintf(Temp_str, "No Date                  ");
                    LCD_ShowString(20, 60, (const uint8_t *)Temp_str);
                }

                sprintf(Temp_st, "block_index=%d\r\n", block_index);
                Debug_printf(Temp_st);
                for (uint8_t i = 0; i < DATA_LEN; i++) {
                    sprintf(Temp_st, "data%d:  %d\r\n", i, data[i]);
                    Debug_printf(Temp_st);
                }
            } else {
                sprintf(Temp_str, "No Date In The Day           ");
                LCD_ShowString(20, 20, (const uint8_t *)Temp_str);
                sprintf(Temp_str, "                             ");
                LCD_ShowString(20, 40, (const uint8_t *)Temp_str);
                LCD_ShowString(20, 60, (const uint8_t *)Temp_str);
            }

            // sprintf(Temp_st, "TimeStamp:  %d\r\n", mktime(&Reader.Time));
            // Debug_printf(Temp_st);
        }
        if (Key_Temp) {
            Menu_ReaderMenu.isReadKey = 1;
            Menu_ReaderMenu.Cur_Style = Menu_Global.Cur_Style;
            sprintf(Temp_str, "                           ");
            LCD_ShowString(20, 20, (const uint8_t *)Temp_str);
            LCD_ShowString(20, 40, (const uint8_t *)Temp_str);
            LCD_ShowString(20, 60, (const uint8_t *)Temp_str);
        }
    } else {
        Reader.MU.Cur_Style = Menu_Global.Cur_Style;
    }

    Menu_RunWindow(&Menu_ReaderMenu);
}
