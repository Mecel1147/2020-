#ifndef __CS_H
#define __CS_H
#include "sys.h"
void GPIO_Configuration();
void TIM2_Configuration(u16 arr,u16 psc);
u16 get_Diatance(void);
void TIM2_IRQHandler(void);
void LED_Init(void);
u16 num_handle(u16 distance);
void TIM3_Int_Init(u16 arr,u16 psc);
#define PB3	 PBout(3)
#define PB5		PBout(5)
#define PB15	PBout(15)
#define PB1	PBout(1)
#define PB13	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)
#define PB14	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)
#define PB0	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)

 #endif