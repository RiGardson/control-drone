#include "stm32f10x.h"

void I2C_config (void);
void Delay (uint32_t DLms);
void I2C2_DeInit(void);
void Start (void);
void Stop (void);
void SendADDR_Wite (uint8_t DevADDR);
void SendDATA (uint8_t data);
void SendLASTDATA (uint8_t data);
void SendADDR_Read(void);//uint8_t DevADDR);
void ReStart (void);

