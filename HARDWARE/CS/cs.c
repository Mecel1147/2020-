#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "cs.h"
#include "led.h"
#include "gy906.h"
extern u16 distance_Data;

void GPIO_Configuration()
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  /*echo*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  /*trig*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_15|GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_13|GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void TIM2_Configuration(u16 arr, u16 psc)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructrue;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  TIM_DeInit(TIM2);
  
  TIM_TimeBaseStructrue.TIM_Period = arr;                   //????
  TIM_TimeBaseStructrue.TIM_Prescaler = psc;                //????
  TIM_TimeBaseStructrue.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructrue.TIM_CounterMode = TIM_CounterMode_Up;
  
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructrue);
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  
  NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;              //???3??
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; //?????1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01;        //?????3
  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
  
  NVIC_Init(&NVIC_InitStructure);                           
  
  TIM_Cmd(TIM2, DISABLE); 
}
void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}
int t=0;
extern	int buff;
extern	int temp;
//��ʱ��3�жϷ�����
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
	{
		t++;
		distance_Data=num_handle(get_Diatance());
		//if(t==3)
		{
		buff=1.0*SMBus_ReadTemp();
		temp=SMBus_ReadTemp()*100.0;
			t=0;
		}
			LED1=!LED1;
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
}

u16 TIM2_Flag;
u16 get_Diatance()//���������
{
  int distance = 0;
  u16 TIM = 0;
  TIM_Cmd(TIM2, ENABLE);           
  
  GPIO_SetBits(GPIOB, GPIO_Pin_6);  
  delay_us(30);
  GPIO_ResetBits(GPIOB, GPIO_Pin_6);
 
  while((!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7))&&TIM2_Flag==0);  
  TIM2->CNT = 0;              
  while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)&&TIM2_Flag==0);    
  TIM_Cmd(TIM2, DISABLE);    
  
  if(TIM2_Flag==1)
    TIM2_Flag = 0;
  
  TIM = TIM_GetCounter(TIM2);
  distance = TIM*0.85;
  return distance;
}
u16 num_handle(u16 distance) //���ݴ�����
{
		  u16 diatance_Data;
    u16 q;
    u16 b;
    u16 s;
    u16 g;
		diatance_Data = get_Diatance();  
    q = diatance_Data/1000;
    b = diatance_Data/100%10;
    s = diatance_Data/10%10;
    g = diatance_Data%10;
    diatance_Data = q*1000+b*100+s*10+g;
		return diatance_Data;
}

void TIM2_IRQHandler(void)//�жϷ�����
{
  if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
  {
		
		
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    TIM2_Flag=1;
  }
}