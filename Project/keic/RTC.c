#include "RTC.h"

void RTC_Init (void){
	//HSE/128=62,5kz
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;//enable the power interface clocks
	RCC->APB1ENR |= RCC_APB1ENR_BKPEN;//enable the backup interface clocks
	
	
	RCC->BDCR |= RCC_BDCR_BDRST;//Resets the entire Backup domain
															//The RTC_PRL, RTC_ALR, RTC_CNT, and RTC_DIV registers are reset only by a Backup Domain reset
	while((RCC->BDCR & RCC_BDCR_BDRST) != RCC_BDCR_BDRST);
	RCC->BDCR &= ~RCC_BDCR_BDRST;
	while((RCC->BDCR & RCC_BDCR_BDRST) == RCC_BDCR_BDRST);//clear bit Resets the entire Backup domain
	
	PWR->CR |= PWR_CR_DBP; // enable access to the Backup registers and RTC.
	RCC->BDCR |= RCC_BDCR_RTCEN;//RTC clock enable
	
	
	RCC->BDCR |= RCC_BDCR_RTCSEL_HSE;//HSE oscillator clock divided by 128 used as RTC clock
		
	/*
Configuration procedure
1. Poll RTOFF, wait until its value goes to ‘1’
2. Set the CNF bit to enter configuration mode
3. Write to one or more RTC registers
4. Clear the CNF bit to exit configuration mode
5. Poll RTOFF, wait until its value goes to ‘1’ to check the end of the write operation.
The write operation only executes when the CNF bit is cleared; it takes at least three
RTCCLK cycles to complete.
	*/
	while ((RTC->CRL & RTC_CRL_RTOFF) != RTC_CRL_RTOFF);
	RTC->CRL |= RTC_CRL_CNF;
	while ((RTC->CRL & RTC_CRL_CNF) != RTC_CRL_CNF);
	
	RTC->PRLH = (uint16_t)0x00; //PRL = 1 => PERIOD 32 mcs
	RTC->PRLL = (uint16_t)0x01;
	
	RTC->CRL &= ~RTC_CRL_CNF;
	while ((RTC->CRL & RTC_CRL_CNF) == RTC_CRL_CNF);
	//while ((RTC->CRL & RTC_CRL_RTOFF) != RTC_CRL_RTOFF);
	//RTC->CRH |= RTC_CRH_OWIE;//Overflow interrupt enable
	//RTC->CRH |= RTC_CRH_ALRIE;//Alarm interrupt enable
	//RTC->CRH |= RTC_CRH_SECIE;//Second interrupt enable
}

void read_couter (uint32_t couter){
	couter = ((RTC->CNTH << 16)|RTC->CNTL);
}


