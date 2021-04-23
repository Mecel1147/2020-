#ifndef __GY906_H
#define __GY906_H
#include "sys.h" 
	   		 
//-- MLX90614 Command Mode --
#define RAM 0x00 //RAM����
#define EEPROM 0x20 //EEPROM����
#define MODE 0x60 //����ģʽ
#define EXITMODE 0x61 //�˳�����ģʽ
#define READFLAG 0xf0 //����־
#define SLEEP 0xff //˯��ģʽ
#define RD 0x01 //������
#define WR 0x00 //д����
#define ACK_SUCCESS 0x01
#define ACK_FAIL 0x00
//void SMBUS_SDA_H(void);
//void SMBUS_SCK_H(void);
//void SMBUS_SDA_L(void);
//void SMBUS_SCK_L(void);
void SMBus_StartBit(void);
void SMBus_StopBit(void);
void SMBus_SendBit(u8);
u8 SMBus_SendByte(u8);
u8 SMBus_ReceiveBit(void);
u8 SMBus_ReceiveByte(u8);
void SMBus_Delay(u16);
void SMBus_Init(void);
u16 SMBus_ReadMemory(u8, u8);
u8 PEC_Calculation(u8*);
float SMBus_ReadTemp(void); //��ȡ�¶�ֵ
void SMBus_Init(void);
void SMBUS_SDA_H(void);
void SMBUS_SCK_H(void);
void SMBUS_SDA_L(void);
void SMBUS_SCK_L(void);
void SMBus_SDA_INMode();
void SMBus_SDA_OUTMode();
#endif
