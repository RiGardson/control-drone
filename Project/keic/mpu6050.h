#include "stm32f10x.h"
#include "I2c.h"

#define WHO_AM_I_REG 0x75
#define PWR_MGMT_1_REG 0x6B
#define SMPLRT_DIV_REG 0x19
#define ACCEL_CONFIG_REG 0x1C
#define ACCEL_XOUT_H_REG 0x3B
#define TEMP_OUT_H_REG 0x41
#define GYRO_CONFIG_REG 0x1B
#define GYRO_XOUT_H_REG 0x43
#define MPU6050_ADDR 0xD0

void Delay (uint32_t DLms);
void Write_byte (uint8_t DevADDR, uint8_t Mem_Rigister, uint8_t Data);
void READ_bytes(uint8_t DevADDR, uint8_t Mem_Rigister, uint8_t *Data);
void mpu_config (void);

