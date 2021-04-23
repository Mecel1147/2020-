#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "key.h"  
#include "gy906.h"
#include "oled.h"
#include "cs.h"
#include "beep.h"
#include "myiic.h"
/*  
IO��ע��
OLED	SDA  PB11
					SCL	 PB10
		������	trigger		PB6
						ECHO 			PB7
		MLX90614	SDA			PB9
							SCL 		PB8
	����		 			LED1	 PB3	
  ���		 			LED2   PB5	
	�¶�					LED0   PB15
	����					LED3			 PB1	
							PB13	--
							PB14 	++
		BEEP 			PB12		
		tx				PA9
		RX				PA10
		����			PB0
						*/


 u16 distance_Data;//���������
 	int buff;//�¶ȴ���������
	int buff1;
	  u16 s;//�¶�С��ʮ��λ������ʾ
		u16 s1;
    u16 g;//�¶�С�����ְٷ�λ	 
int key=35;	//�¶���ֵ
int temp;	//�¶�С�����ֻ���
int face;	//�沿����
int Hex2Dec(int Hex);//16����ת10����
void FACE(void);//�沿�����жϺ���
void key_judge(void);//��ֵ���ú���
void OLED_Show(void );//OLED��ʾ����
	
int main(void)
{ 
	u8 len;
	u16 i=0;
	BEEP_Init();	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);    //��ʼ����ʱ����
	uart_init(115200);	//��ʼ�����ڲ�����Ϊ115200
	uart2_init(115200);//����2��ʼ��
	TIM2_Configuration(5000,419);//��ʱ��2��ʼ��
	LED_Init();					//��ʼ��LED 
	
	KEY_Init(); 				//������ʼ��  
	IIC_Init();					//OLED IIC��ʼ��
	LED2=1;
	delay_ms(1000);
	LED2=0;
	SMBus_Init();				//MLX914 IIC��ʼ��
	OLED_Init();				//OLED ��ʼ��
	GPIO_Configuration();//������IO�ڳ�ʼ��
	TIM3_Int_Init(5000,8399);  //	��ʱ��3��ʼ��	84Mhz/8400=10khz  10^-4*5000=500MS
 	

	
	while(1)
	{
		OLED_Show();	//OLED����ʾ�����¶ȵ���Ϣ
		//buff1=buff+2;
		len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
		USART_RX_STA=0;					//���ڱ�־λ����
		face = Hex2Dec(USART_RX_BUF[2]);//�������յ���16������ת��Ϊ10����
		//LED3=1;

		FACE();
		key_judge();
		//printf("%d \n",buff);
		//�����жϽ�����ֵ����
		
		if(buff1>=key)//�¶ȳ�����ֵ��������LED����
		{
			BEEP=1;
			LED0=1;
			delay_ms(500);
			BEEP=0;
			
			
		}
		
		if(buff1<key)LED0=0;//�¶�С����ֵLEDϨ��
		delay_ms (100);
	} 	    
}


	int Hex2Dec(int Hex)
{
    int dec=0,temp=0;
    temp= Hex & 0x0f;  //
    dec = (Hex >> 4) *10 + temp;//��4λ��10+����λ
    return dec;
}
void OLED_Show(void )
	{
		
		OLED_ShowCHinese(1,0,16);		//����
		OLED_ShowCHinese(17,0,17);
		OLED_ShowChar(35,1,':',20);
		OLED_ShowString(75,1,"mm",0);	
		OLED_ShowNum(100,0,350,3,13);		
		//OLED_ShowString(104,1,"400",20);
		//OLED_ShowNum(99,1,400,3,5);
		
		
		OLED_ShowCHinese(1,2,18);		//�¶�
		OLED_ShowCHinese(17,2,19);
		OLED_ShowCHinese(89,1,20);
		OLED_ShowChar(35,3,':',20);	
		OLED_ShowChar(72,3,'.',0);
		
		if(PB0==0)
		{
			buff1=buff+2;
			s1=s+5;
				
		}
		else if(PB0==1)
		{
			buff1=buff;
			s1=s;
		}
		OLED_ShowNum(40,2,buff1,4,16);
			OLED_ShowNum(78,3,s1,1,1);
		OLED_ShowChar(93,3,'C',20);
//		OLED_ShowNum(78,3,s,1,1);		
		OLED_ShowNum(85,3,g,1,0);
		OLED_ShowNum(100,2,key,3,13);
		
		s = temp/10%10;
    g = temp%10;

		OLED_ShowNum(40,0,distance_Data,4,16);
		//OLED_ShowString(1,4,"test",4);
	}
void key_judge(void)
{
	if(PB14==0)
		{
				
			delay_ms(10);//��������
			if(PB14==0)
			{
				key++;
			}
		}
			if(PB13==0)
		{
				
			delay_ms(10);
			if(PB13==0)
			{
				key--;
			}
		}
}

void FACE(void)
{
	int times=3;
	if(face==1)
	{
		int i;
		for(i=0;i<3;i++)
		OLED_ShowCHinese(i*17,5,3*(face-1)+22+i);
		OLED_ShowString(50,5,"welcome",0);
	}
	if(face==2)
	{
		int i;
		for(i=0;i<3;i++)
		OLED_ShowCHinese(i*17,5,3*(1-1)+22+i);
		OLED_ShowString(50,5,"welcome",0);
	}
	if(face==3)
	{
		int i;
		for(i=0;i<3;i++)
		OLED_ShowCHinese(i*17,5,3*(face-1)+22+i);
		OLED_ShowString(50,5,"welcome",0);
	}
		if(face==5)
	{
		int i;
		for(i=0;i<3;i++)
		OLED_ShowCHinese(i*17,5,3*(face-1)+22+i);
		OLED_ShowString(50,5,"error  ",0);
			BEEP=1;
			LED2=1;
			delay_ms(500);
			BEEP=0;
			LED2=0;
	}
	if(face==4)
	{
		OLED_ShowString(50,6,"MASK   ",0);
			//BEEP=1;
			LED3=1;
			delay_ms(500);
			BEEP=0;
			LED3=0;
	}
	if(face==6)
	{
		int i;
		for(i=0;i<3;i++)
		OLED_ShowCHinese(i*17,5,3*(face-1)+22+i);
		OLED_ShowString(50,5,"welcome",0);
}
		if(face==7)
	{
		int i;
		for(i=0;i<3;i++)
		OLED_ShowCHinese(i*17,5,3*(face-1)+22+i);
		OLED_ShowString(50,5,"welcome",0);

	}
	if(face==8)
	{
		int i;
		for(i=0;i<3;i++)
		OLED_ShowCHinese(i*17,5,3*(face-1)+22+i);
		OLED_ShowString(50,5,"welcome",0);
	}
}
	
/*

void FACE(void)
{
	int times=3;
	if(face==1)
	{
		int i;
		for(i=0;i<3;i++)
		OLED_ShowCHinese(i*17,5,3*(face-1)+22+i);
		OLED_ShowString(50,5,"welcome",0);
	}
	if(face==2)
	{
		int i;
		for(i=0;i<3;i++)
		OLED_ShowCHinese(i*17,5,3*(face-1)+22+i);
		OLED_ShowString(50,5,"welcome",0);
	}
	if(face==3)
	{
		int i;
		for(i=0;i<3;i++)
		OLED_ShowCHinese(i*17,5,3*(face-1)+22+i);
		OLED_ShowString(50,5,"welcome",0);
	}
		if(face==5)
	{
		int i;
		for(i=0;i<3;i++)
		OLED_ShowCHinese(i*17,5,3*(face-1)+22+i);
		OLED_ShowString(50,5,"error  ",0);
		do{
			BEEP=1;
			LED2=1;
			delay_ms(500);
			BEEP=0;
			LED2=0;
			}while(times--);
	}
	if(face==4)
	{
		OLED_ShowString(50,6,"MASK   ",0);
	do{
			BEEP=1;
			LED3=1;
			delay_ms(500);
			BEEP=0;
			LED3=0;
			}while(times--);
	}
}
*/