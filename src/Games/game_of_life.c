#include "game_of_life.h"

uint8_t Nodes_Cache[8][120]; // 计算缓存

void Update_Display(void) // 上传屏幕
{
    memcpy(OLED_DisplayBuf, Nodes_Cache, 1024);
}

void Point_life(uint8_t X, uint8_t Y) // 写点生
{
    Nodes_Cache[(Y / 8)][X] |= (0x01 << (Y % 8));
}

void Point_death(uint8_t X, uint8_t Y) // 写点死
{
    Nodes_Cache[(Y / 8)][X] &= ~(0x01 << (Y % 8));
}

uint8_t CountPointRound(uint8_t X, uint8_t Y) // 统计点周围细胞数量
{
    return (
        OLED_GetPoint(X - 1, Y - 1) + OLED_GetPoint(X, Y - 1) + OLED_GetPoint(X + 1, Y - 1) +
        OLED_GetPoint(X - 1, Y) + OLED_GetPoint(X + 1, Y) +
        OLED_GetPoint(X - 1, Y + 1) + OLED_GetPoint(X, Y + 1) + OLED_GetPoint(X + 1, Y + 1));
}

void Game_Of_Life_Turn(void) // 刷新游戏回合
{
    uint8_t x, y;
    uint8_t Count;

    for (y = 0; y < 64; y++) {
        for (x = 0; x < 120; x++) {
            Count = CountPointRound(x, y);
            if (OLED_GetPoint(x, y)) {
                if (Count < 2 || Count > 3) {
                    Point_death(x, y);
                }
            } else {
                if (Count == 3) {
                    Point_life(x, y);
                }
            }
        }
    }
    Update_Display();
}

void Game_Of_Life_Seed(int16_t seed) // 游戏初始化种子
{
    srand(seed);
    for (uint8_t i = 0; i < 8; i++)
        for (uint8_t j = 0; j < 120; j++) {
            Nodes_Cache[i][j] = rand();
        }
    Update_Display();
}

void Game_Of_Life_Play(void) // 游戏开始
{
    uint8_t x1 = 8, x2 = 16, y1 = 32, y2 = 32;
    int8_t shift = -1;

    int16_t Angle = 7;
    float px = 72, py = 32;

    uint8_t Key_Temp;

    OLED_Clear();
    Game_Of_Life_Seed(DS3231_GetTimeStamp()); // 放置种子

    while (1) {
        Game_Of_Life_Turn();

        if (shift > 0) {
            y2 += Menu_RollEvent() * 8;
        } else {
            x2 += Menu_RollEvent() * 8;
        }
        x2 %= 120;
        y2 %= 64;
        OLED_DrawLine(x1, y1, x2, y2);
        if ((x2 - x1) > 1) {
            x1 += (x2 - x1) / 8 + 1;
        } else if ((x2 - x1) < -1) {
            x1 += (x2 - x1) / 8 - 1;
        } else {
            x1 = x2;
        }
        if ((y2 - y1) > 1) {
            y1 += (y2 - y1) / 2 + 1;
        } else if ((y2 - y1) < -1) {
            y1 += (y2 - y1) / 2 - 1;
        } else {
            y1 = y2;
        }

        OLED_Rotation_C_P(64, 32, &px, &py, Angle);
        OLED_DrawLine(64, 32, px + 0.5, py + 0.5);

        OLED_Update();
        Key_Temp = Menu_EnterEvent();
        if (Key_Temp == 1) { shift = -shift; }
        if (Key_Temp == 2) { return; }
    }
}
