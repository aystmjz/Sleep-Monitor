#ifndef __RED_H
#define __RED_H 
#include "sys.h" 
#include "delay.h"
 
#define RDATA 	PAin(15)	 	//�������������
 
//����ң��ʶ����(ID),ÿ��ң�����ĸ�ֵ��������һ��,��Ҳ��һ����.
//����ѡ�õ�ң����ʶ����Ϊ0
#define REMOTE_ID 0      		   
 
extern u8 RmtCnt;			//�������µĴ���
 
void Remote_Init(void);    	//���⴫��������ͷ���ų�ʼ��
u8 Remote_Scan(void);	    
#endif