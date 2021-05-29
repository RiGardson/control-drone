#include "I2C.h"


void I2C_config (void){

	//GPIO PB10-SCL;PB11-SDA :Alternate function open drain------------------------
	RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;	
	I2C2->CR1 &= ~I2C_CR1_PE;//
	I2C2_DeInit();
	
	I2C2->CR2 |= 0x24;// 36 Mhz
	I2C2->TRISE = 0x0C;//(SCL rise time of mpu6050=300ns)
	 

	I2C2->CCR |= I2C_CCR_FS;
	I2C2->CCR &= ~I2C_CCR_DUTY;//T(high):T(low)=1:2
  I2C2->CCR |= 0x1F;// dec 31
	I2C2->CR1 &= ~I2C_CR1_SMBUS; // I2C MODE
	//
	I2C2->CR1 &= ~I2C_CR1_NOSTRETCH;
	I2C2->OAR1 &= ~I2C_OAR1_ADDMODE;//7-bit slave address 
	I2C2->OAR2 &= ~I2C_OAR2_ENDUAL;//DISABLE DUAL MODE
	I2C2->CR1 &= ~I2C_CR1_ENGC;//General call DISable
	I2C2->SR2 |= I2C_CR1_STOP;
	
	I2C2->CR1 |= I2C_CR1_PE;//
	while ((I2C2->CR1 & I2C_CR1_PE) != I2C_CR1_PE );
	//END CONFIG---------------------------------------------------
}

void I2C2_DeInit(void){
	I2C2->CR1  = 0;
	I2C2->CR2  = 0;
	I2C2->OAR1 = 0;
	I2C2->OAR2 = 0;
	I2C2->DR   = 0;
	I2C2->SR1  = 0;
	I2C2->SR2  = 0;
	I2C2->CCR  = 0;
	I2C2->CR1  = 0x02;
};

void Start (void){
	I2C2->CR1 |= I2C_CR1_ACK;
	while ((I2C2->SR2 & I2C_SR2_BUSY) == I2C_SR2_BUSY  );// wait BUSY = 0
	I2C2->CR1 |= I2C_CR1_START;//S
	while ((I2C2->SR1 & I2C_SR1_SB ) != I2C_SR1_SB);//WAIT SB = 1//EV5
	(void) I2C1->SR1;
}

void Stop (void){
	I2C2->CR1 |= I2C_CR1_STOP;
	I2C2->CR1 &= ~I2C_CR1_ACK;
}

void SendADDR_Wite (uint8_t DevADDR){
	I2C2->DR = DevADDR;
	while ((I2C2->SR1&I2C_SR1_ADDR) != I2C_SR1_ADDR);//EV6
	(void)(I2C2->SR1|I2C2->SR2);
  while ((I2C2->SR1 & I2C_SR1_TXE) != I2C_SR1_TXE);	
}

void SendDATA (uint8_t data){
  I2C2->DR =data;
	while ((I2C2->SR1 & I2C_SR1_TXE) != I2C_SR1_TXE);//EV6
}

void SendLASTDATA (uint8_t data){
	I2C2->DR = data;
	while ((I2C2->SR1 & I2C_SR1_BTF) != I2C_SR1_BTF);//EV6
}

void SendADDR_Read(void){
	I2C2->DR = 0xD1; //(DevADDR|I2C_OAR1_ADD0);
	while ((I2C2->SR1&I2C_SR1_ADDR) != I2C_SR1_ADDR);
	(void)I2C2->SR1;
	(void)I2C2->SR2;
}

void ReStart (void){
	I2C2->CR1 |= I2C_CR1_START;//S
	while ((I2C2->SR1 & I2C_SR1_SB ) != I2C_SR1_SB);//WAIT SB = 1//EV5
  (void)I2C2->SR1;//read SR1 => ADDR =0
}



