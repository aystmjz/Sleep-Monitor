#include "MLX90640.h"
uint8_t RX_BUF[1550]= {0};
uint8_t CMD_SAVE[4]={0xA5,0x56,0x02,0xFD};
uint8_t CMD_AUTO[4]={0xA5,0x35,0x02,0xDC};

TempDataTypeDef TempData;

//校验和检查
uint8_t MLX90640_CheckData(uint8_t *data)
{
    uint16_t sum=0,length=0,i=0;
    uint16_t temp=0;
    length=((uint16_t )RX_BUF[3]<<8)|RX_BUF[2]+6;
    if(length>1544)//超过上传数据
        return 0;
    for(i=0; i<length-2; i=i+2)
    {
        temp=((uint16_t )RX_BUF[i+1]<<8)|RX_BUF[i];
        sum+=temp;
    }
    temp=((uint16_t )RX_BUF[1543]<<8)|RX_BUF[1542];
    if(sum==temp)
    {
        memcpy(data,RX_BUF,length);
        return 1;
    }
    else
        return 0;
}

void MLX90640_SendInitCMD()
{
    USART1_Send_bytes(CMD_AUTO,4);//发送自动输出指令
    Delay_ms(50);
    USART1_Send_bytes(CMD_SAVE,4);//保存
    Delay_ms(50);
}

void MLX90640_SendCMD(uint8_t *CMD)
{
    USART1_Send_bytes(CMD,4);
    Delay_ms(50);
}


void MLX90640_Init()
{
    MLX90640_SendInitCMD();
    uart1_init(11520);
}
