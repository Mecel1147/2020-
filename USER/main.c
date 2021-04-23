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
IO口注释
OLED	SDA  PB11
					SCL	 PB10
		超声波	trigger		PB6
						ECHO 			PB7
		MLX90614	SDA			PB9
							SCL 		PB8
	程序		 			LED1	 PB3	
  身份		 			LED2   PB5	
	温度					LED0   PB15
	口罩					LED3			 PB1	
							PB13	--
							PB14 	++
		BEEP 			PB12		
		tx				PA9
		RX				PA10
		补偿			PB0
						*/


 u16 distance_Data;//超声波测距
 	int buff;//温度传感器缓存
	int buff1;
	  u16 s;//温度小数十分位部分显示
		u16 s1;
    u16 g;//温度小数部分百分位	 
int key=35;	//温度阈值
int temp;	//温度小数部分缓存
int face;	//面部数据
int Hex2Dec(int Hex);//16进制转10进制
void FACE(void);//面部数据判断函数
void key_judge(void);//阈值设置函数
void OLED_Show(void );//OLED显示函数
	
int main(void)
{ 
	u8 len;
	u16 i=0;
	BEEP_Init();	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);    //初始化延时函数
	uart_init(115200);	//初始化串口波特率为115200
	uart2_init(115200);//串口2初始化
	TIM2_Configuration(5000,419);//定时器2初始化
	LED_Init();					//初始化LED 
	
	KEY_Init(); 				//按键初始化  
	IIC_Init();					//OLED IIC初始化
	LED2=1;
	delay_ms(1000);
	LED2=0;
	SMBus_Init();				//MLX914 IIC初始化
	OLED_Init();				//OLED 初始化
	GPIO_Configuration();//超声波IO口初始化
	TIM3_Int_Init(5000,8399);  //	定时器3初始化	84Mhz/8400=10khz  10^-4*5000=500MS
 	

	
	while(1)
	{
		OLED_Show();	//OLED上显示距离温度等信息
		//buff1=buff+2;
		len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
		USART_RX_STA=0;					//串口标志位清零
		face = Hex2Dec(USART_RX_BUF[2]);//将串口收到的16进制数转换为10进制
		//LED3=1;

		FACE();
		key_judge();
		//printf("%d \n",buff);
		//按键判断进行阈值设置
		
		if(buff1>=key)//温度超过阈值蜂鸣器和LED报警
		{
			BEEP=1;
			LED0=1;
			delay_ms(500);
			BEEP=0;
			
			
		}
		
		if(buff1<key)LED0=0;//温度小于阈值LED熄灭
		delay_ms (100);
	} 	    
}


	int Hex2Dec(int Hex)
{
    int dec=0,temp=0;
    temp= Hex & 0x0f;  //
    dec = (Hex >> 4) *10 + temp;//高4位×10+低四位
    return dec;
}
void OLED_Show(void )
	{
		
		OLED_ShowCHinese(1,0,16);		//距离
		OLED_ShowCHinese(17,0,17);
		OLED_ShowChar(35,1,':',20);
		OLED_ShowString(75,1,"mm",0);	
		OLED_ShowNum(100,0,350,3,13);		
		//OLED_ShowString(104,1,"400",20);
		//OLED_ShowNum(99,1,400,3,5);
		
		
		OLED_ShowCHinese(1,2,18);		//温度
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
				
			delay_ms(10);//按键消抖
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