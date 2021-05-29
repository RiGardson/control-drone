#include "TIM6_Delay.h"

void Delay(uint8_t Delay){    //Delay = delay_time(in ms) * 2
	TIM6_Denit();

	RCC->APB1ENR &= ~RCC_APB1ENR_TIM6EN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

	TIM6->PSC = 0xA40F; //f = 2kHz
	TIM6->ARR = Delay;

	TIM6->CR1 |= TIM_CR1_CEN;

	while(!(TIM6->SR & TIM_SR_UIF));
	TIM6_Denit();

}

void TIM6_Denit(void){
	TIM6->SR = 0;
	TIM6->CR1 = 0;
	TIM6->CR2 = 0;
	TIM6->DIER = 0;
	TIM6->EGR = 0;
	TIM6->PSC = 0;
	TIM6->ARR = 0xFFF;
}
