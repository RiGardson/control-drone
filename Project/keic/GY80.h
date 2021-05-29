#include "stm32f10x.h"
#include "I2c.h"

void Write_byte (uint8_t DevADDR, uint8_t Mem_Rigister, uint8_t Data);
void READ_bytes(uint8_t DevADDR, uint8_t Mem_Rigister, uint8_t *Data);
