

#include "as5600.h"

#include "TIM6_Delay.h"

void Read_AS5600_register(uint8_t Address, uint8_t *Data, uint8_t size){

	I2C1->CR1 |= I2C_CR1_ACK;
	I2C1_Start();
	I2C1_Address(AS5600_SLAVE_ADDRESS_WRITE);
	I2C1->DR = Address;
	while (!(I2C1->SR1 & I2C_SR1_TXE)){};  // wait for TXE bit to set

	//I2C1->CR1 |= I2C_CR1_ACK;
	//I2C1_Stop();

	I2C1_Start();
	I2C1_Address(AS5600_SLAVE_ADDRESS_READ);


	uint8_t i;
	for(i=0;i<size;i++)
	{
	  if(i<(size-1))
	  {
		  while (!(I2C1->SR1 & I2C_SR1_RXNE)){};
		  Data[size-i-1] = I2C1->DR;
	  }
	  else
	  {
	    I2C1->CR1 &= ~I2C_CR1_ACK;
	    I2C1_Stop();
	    while (!READ_BIT(I2C1->SR1, I2C_SR1_RXNE)){};
	    Data[size-i-1] = I2C1->DR;
	  }
	}
}

void Write_AS5600_register(uint8_t Address, uint8_t *Data, uint8_t size){
	I2C1->CR1 |= I2C_CR1_ACK;
	I2C1_Start();
	I2C1_Address(AS5600_SLAVE_ADDRESS_WRITE);
	I2C1->DR = Address;
	while (!(I2C1->SR1 & I2C_SR1_TXE));  // wait for TXE bit to set

	uint8_t i;
	for(i=0;i<size;i++){
		while (!(I2C1->SR1 & I2C_SR1_TXE));
		I2C1->DR = Data[size-i-1];  // send data
	}
	while (!(I2C1->SR1 & I2C_SR1_BTF));  // wait for BTF to set
	I2C1_Stop();
}

/*for(i=0;i<size;i++)
{
  if(i<(size-1))
  {
	  while (!(I2C1->SR1 & I2C_SR1_RXNE)){};
	  Data[size-i-1] = I2C1->DR;
  }
  else
  {
    I2C1->CR1 &= ~I2C_CR1_ACK;
    I2C1_Stop();
    while (!READ_BIT(I2C1->SR1, I2C_SR1_RXNE)){};
    Data[size-i-1] = I2C1->DR;
  }
}*/
/*
	while (size)
		{
			size--;

			while (!(I2C1->SR1 & I2C_SR1_RXNE));  // wait for RxNE bit to set
			if (size == 0) I2C1->CR1 &= ~I2C_CR1_ACK;// clear ACK bit
			Data[size] = I2C1->DR;  // Read data
			//
		}
//
		//while (!(I2C1->SR1 & I2C_SR1_BTF));  // wait for BTF to set
		//I2C1->CR1 &= ~I2C_CR1_ACK;
	//I2C1_Stop();*/
