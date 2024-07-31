#include "stm32f4xx.h"                  // Device header
static volatile int pin =14; // define pin variable
static volatile int i;
void EXTI0_IRQHandler(void); // define interrupt handler

void EXTI0_IRQHandler(void){
	EXTI->PR |= EXTI_PR_PR0; // clean PR register
	GPIOD->ODR &= ~(1UL<<pin); // light off the current LED
	if(pin==14)
		pin = 15;
	else
		pin = 14;
	GPIOD->ODR |= (1UL<<pin); // light on the next LED
}
static void wait(){ // define wait function
	for(i=0;i<1000000;i++);
}
int main(){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIODEN; // activate clock signal of A and D ports
	GPIOD->MODER |= GPIO_MODER_MODER15_0 | GPIO_MODER_MODER14_0; // set pin 14 and 15 of D port to output mod
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; // activate system configuration controller clock signal
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0; // trigger interrupt from A port
	EXTI->IMR |= EXTI_IMR_IM0; // trigger interrupt from pin 0
	EXTI->RTSR |= EXTI_RTSR_TR0; // trigger interrupt with button click
	NVIC_EnableIRQ(EXTI0_IRQn); // activate interrupt 6
	while(1){
		GPIOD->ODR |= (1UL<<pin); // light on the current LED
		wait();
		GPIOD->ODR &= ~(1UL<<pin); // light off the current LED
		wait();
	}
}
