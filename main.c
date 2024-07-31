#include "stm32f4xx.h"                  // Device header
static volatile int pin =14;
static volatile int i;
void EXTI0_IRQHandler(void);

void EXTI0_IRQHandler(void){
	EXTI->PR |= EXTI_PR_PR0;
	GPIOD->ODR &= ~(1UL<<pin);
	if(pin==14)
		pin = 15;
	else
		pin = 14;
	GPIOD->ODR |= (1UL<<pin);
}
static void wait(){
	for(i=0;i<1000000;i++);
}
int main(){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIODEN;
	GPIOD->MODER |= GPIO_MODER_MODER15_0 | GPIO_MODER_MODER14_0;
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0;
	EXTI->IMR |= EXTI_IMR_IM0;
	EXTI->RTSR |= EXTI_RTSR_TR0;
	NVIC_EnableIRQ(EXTI0_IRQn);
	while(1){
		GPIOD->ODR |= (1UL<<pin);
		wait();
		GPIOD->ODR &= ~(1UL<<pin);
		wait();
	}
}
