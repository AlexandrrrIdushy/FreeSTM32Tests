#include "stm32f4xx.h"                  // Device header

void delay(uint32_t count)
{
	for(int i = 0; i < count; i ++)
	{}
}


int main(void)

{
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//включим тактирование порта D	
	GPIO_InitTypeDef InitD; //для светодиодов	
	InitD.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4| GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	InitD.GPIO_Mode = GPIO_Mode_OUT;
	InitD.GPIO_OType = GPIO_OType_PP;
	InitD.GPIO_Speed = GPIO_Speed_2MHz;
	InitD.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &InitD);//инициализация ног
//	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; // 

	
	
 //кнопка
 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//включим тактирование порта A		
 GPIO_InitTypeDef InitA0; //для кнопки
 InitA0.GPIO_Pin = GPIO_Pin_0;
 InitA0.GPIO_Mode = GPIO_Mode_IN;
 InitA0.GPIO_OType = GPIO_OType_OD;
 InitA0.GPIO_Speed = GPIO_Speed_2MHz;
 InitA0.GPIO_PuPd = GPIO_PuPd_NOPULL;
 GPIO_Init(GPIOA,&InitA0); //инициализация ножек порта

	
while(1)
{
  if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==1) //кнопка нажата
  {	
  GPIO_SetBits(GPIOD, GPIO_Pin_0);
   delay(1000000);
   GPIO_ResetBits(GPIOD, GPIO_Pin_0);
   GPIO_SetBits(GPIOD, GPIO_Pin_1);
   delay(1000000);
   GPIO_ResetBits(GPIOD, GPIO_Pin_1);
   GPIO_SetBits(GPIOD, GPIO_Pin_2);
   delay(1000000);
   GPIO_ResetBits(GPIOD, GPIO_Pin_2);
   GPIO_SetBits(GPIOD, GPIO_Pin_3);
   delay(1000000);
   GPIO_ResetBits(GPIOD, GPIO_Pin_3);
   GPIO_SetBits(GPIOD, GPIO_Pin_4);
   delay(1000000);
   GPIO_ResetBits(GPIOD, GPIO_Pin_4);
   GPIO_SetBits(GPIOD, GPIO_Pin_5);
   delay(1000000);
   GPIO_ResetBits(GPIOD, GPIO_Pin_5);
   GPIO_SetBits(GPIOD, GPIO_Pin_6);
   delay(1000000);
   GPIO_ResetBits(GPIOD, GPIO_Pin_6);
   GPIO_SetBits(GPIOD, GPIO_Pin_7);
   delay(1000000);
   GPIO_ResetBits(GPIOD, GPIO_Pin_7);
   GPIO_SetBits(GPIOD, GPIO_Pin_8);
   delay(1000000);
   GPIO_ResetBits(GPIOD, GPIO_Pin_8);
   GPIO_SetBits(GPIOD, GPIO_Pin_9);
   delay(1000000);
   GPIO_ResetBits(GPIOD, GPIO_Pin_9);
	  }
  else GPIO_ResetBits(GPIOD,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|
  GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);
 } 
}

