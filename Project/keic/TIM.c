#include "TIM.h"

//use tim1 -> sample time t=100 de chay dong co 
//										and T=10 ms for PID

void sample_time (uint16_t t){// t tinh bang don vi ms
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN ;//enable TIM1; AHB=1;APB1=1
	TIM1->CR1 |= TIM_CR1_ARPE; // TIMx_ARR register is buffered
	//TIM1->DIER &= ~TIM_DIER_UIE;
	
	TIM1->PSC = 71;
	TIM1->ARR = t;
	TIM1->CR1 &= ~ TIM_CR1_CMS ;// Edge-aligned mode. The counter counts up or down depending on the direction bit(DIR).
	TIM1->CR1 &= ~TIM_CR1_DIR;//UPCOUNTING MODE
	TIM1->CR1 &= ~TIM_CR1_UDIS;
	TIM1->SR &= ~TIM_CR1_URS;
	TIM1->SR &= ~TIM_SR_UIF;//clear update flag
	TIM1->DIER |= TIM_DIER_UIE;//UEV-update event enable
	//TIM1->DIER |= TIM_DIER_TIE;//.Trigger interrupt enable
	TIM1->CR1 |= TIM_CR1_CEN;//COUNTER ENABLE
	
}

//TIM4 CH1,CH2,CH3,CH4->controll motor 1,2,3,4
void all_motor_config (void){
	//----------GPIO CONFIG------------------
	//PB6 TIM4_CH1; PB7 TIM4_CH2; PB8_CH3; PB9 TIM4_CH4;
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	GPIOB->CRL &= ~GPIO_CRL_CNF6;GPIOB->CRH &= ~GPIO_CRL_CNF7;
	GPIOB->CRH &= ~GPIO_CRH_CNF8;GPIOB->CRH &= ~GPIO_CRH_CNF9;
	// Alternate function output Push-pull
	GPIOB->CRL |= GPIO_CRL_CNF6_1;GPIOB->CRL |= GPIO_CRL_CNF7_1;
	GPIOB->CRH |= GPIO_CRH_CNF8_1;GPIOB->CRH |= GPIO_CRH_CNF9_1;
	//output-2Hz
	GPIOB->CRL |= GPIO_CRL_MODE6_1;GPIOB->CRL |= GPIO_CRL_MODE7_1;
	GPIOB->CRH |= GPIO_CRH_MODE8_1;GPIOB->CRH |= GPIO_CRH_MODE9_1;
	
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	//---------------TIM4 CONFIG-------------------
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN ;//enable TIM4; (APB1 = AHB/2=36)*2=72
	TIM1->CR1 |= TIM_CR1_ARPE; // TIMx_ARR register is buffered
	
	TIM4->PSC = 35;
	TIM4->ARR = 40000;
	TIM4->CR1 &= ~ TIM_CR1_CMS ;// Edge-aligned mode. The counter counts up or down depending on the direction bit(DIR).
	TIM4->CR1 &= ~TIM_CR1_DIR;//UPCOUNTING MODE
	//----------------CH1-------------------------
	TIM4->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 ;// 110 -> PWM mode 1
	TIM4->CCER |= TIM_CCER_CC1E; // ENABLE COMPARE 1 OUTPUT ENABLE
	TIM4->CCER &= ~TIM_CCER_CC1P; //OC1 active high
	TIM4->CCMR1 &= ~TIM_CCMR1_CC1S; //CC1 channel is configured as output
	TIM4->CCMR1 |= TIM_CCMR1_OC1PE;  //Output Compare 1 preload enable
	TIM4->CCMR1 |= TIM_CCMR1_OC1FE;//Output Compare 1 fast enable
	//-------------------CH2--------------------
	TIM4->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 ;// 110 -> PWM mode 1
	TIM4->CCER |= TIM_CCER_CC2E; // ENABLE COMPARE 2 OUTPUT ENABLE
	TIM4->CCER &= ~TIM_CCER_CC2P; //OC2 active high
	TIM4->CCMR1 &= ~TIM_CCMR1_CC2S; //CC2 channel is configured as output
	TIM4->CCMR1 |= TIM_CCMR1_OC2PE;  //Output Compare 2 preload enable
	TIM4->CCMR1 |= TIM_CCMR1_OC2FE;//Output Compare 2 fast enable
	//----------------------CH3---------------------------	?
	TIM4->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 ;// 110 -> PWM mode 1
	TIM4->CCER |= TIM_CCER_CC3E; // ENABLE COMPARE 3 OUTPUT ENABLE
	TIM4->CCER &= ~TIM_CCER_CC3P; //OC3 active high
	TIM4->CCMR2 &= ~TIM_CCMR2_CC3S; //CC3 channel is configured as output
	TIM4->CCMR2 |= TIM_CCMR2_OC3PE;  //Output Compare 3 preload enable
	TIM4->CCMR2 |= TIM_CCMR2_OC3FE;//Output Compare 3 fast enable
	//-----------------------CH4----------------------
	TIM4->CCMR2 |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1 ;// 110 -> PWM mode 1
	TIM4->CCER |= TIM_CCER_CC4E; // ENABLE COMPARE 4 OUTPUT ENABLE
	TIM4->CCER &= ~TIM_CCER_CC4P; //OC4 active high
	TIM4->CCMR2 &= ~TIM_CCMR2_CC4S; //CC4 channel is configured as output
	TIM4->CCMR2 |= TIM_CCMR2_OC4PE;  //Output Compare 4 preload enable
	TIM4->CCMR2 |= TIM_CCMR2_OC4FE;//Output Compare 4 fast enable


//	TIM2->CR1 &= ~TIM_CR1_UDIS;// UPDATE ENABLE
	TIM4->EGR |= TIM_EGR_UG;// UPDATE ENABLE ( DEM TU O -> VALUE ARR)
	TIM4->BDTR |= TIM_BDTR_MOE;//Main output enable
	TIM4->BDTR |= TIM_BDTR_AOE;//Automatic output enable
		
//		
  NVIC_EnableIRQ(TIM1_CC_IRQn);//Interrupt compare enable
	TIM4->CR1 |= TIM_CR1_CEN;//COUNTER ENABLE
//
}
