#include "stm32f4xx.h"                  // Device header


int main(void)

{
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; // включим тактирование порта
        GPIOD->MODER = 0x55000000; // включим ножки 12,13,14,15 на выход
        GPIOD->OTYPER = 0; //подтянем резистор ко всем ножкам порта
        GPIOD->OSPEEDR = 0; //установим скорость LOW на все лапки порта
        GPIOD->ODR = 0xF000; // зажгем все 4 диода
	
while(1)
{
//	int i;
//	GPIOD->ODR = 0x8000;
//	for(i=0;i<500000;i++){}
//	GPIOD->ODR = 0x4000;
//	for(i=0;i<1000000;i++){}
//	GPIOD->ODR = 0x2000;
//	for(i=0;i<1500000;i++){}
//	GPIOD->ODR = 0x1000;
//	for(i=0;i<2000000;i++){}
}

}