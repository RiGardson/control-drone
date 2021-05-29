#include "main.h"
#include "stdio.h"
#include "math.h"
float z=0;
uint16_t i=0;//bien danh cho ham for 
//const float Pi = 3.14159265;
uint8_t Data[6];
int16_t ACCEL_Xout,ACCEL_Yout,ACCEL_Zout;//cac truc ACC
int16_t GYRO_Xout ,GYRO_Yout ,GYRO_Zout ;//cac truc GYRO
int16_t set_x,set_y,set_z;//gia tri nhap vao tu o dieu khien

uint32_t T = 6000 ;									//sample time 40 us /1000000 =s
int n=1;													//bien dem so lan da chay PID
// PID 
float ampha , betal, gamma;
uint16_t ux , ux1;//xem lai khai bao phu hop voi gia tri dau ra pwm
uint16_t uy , uy1;
uint16_t uz , uz1;

float ex , ex1, ex2;//error
float ey , ey1, ey2;
float ez , ez1, ez2;

float P ,I ,D,delta;

//bien phuc vu tinh detal t khi tinh goc
float detal_t = 0;
uint32_t t=0,t1=0,t2=0;					
float goc_x=0.00,goc_y=0.00,goc_z=0.00;
float goc_x1=0.00,goc_y1=0.00,goc_z1=0.00;
float goc_x2=0.00,goc_y2=0.00,goc_z2=0.00;
int32_t SUM_X_A=0,SUM_Y_A=0,SUM_Z_A=0,SUM_X_G=0,SUM_Y_G=0,SUM_Z_G=0;
int32_t XA0,YA0,ZA0,XG0,YG0,ZG0;

uint32_t Thrust =0 ; 
// cu 1000us se doc gia tri moi 
//50 us de doc gia tri MPU+tinhs toan chay PID
float proportion;//ti le dau ra u va PWM
uint32_t u1,u2,u3,u4;


int main (void){
	__disable_irq();
	
	RCC_config(); 
	GPIO();
	I2C_config();
	mpu_config();
	SysTick_Config(72);//thoi gian couter++ = 1ms
	NVIC_SetPriority(SysTick_IRQn,0);
	all_motor_config();
	
	sample_time(T);
	NVIC_EnableIRQ(TIM1_UP_IRQn);
	NVIC_SetPriority(TIM1_UP_IRQn,1);//uu tien ngat =1 sau systick=0
	
	ampha=2*T*P+I*T*T+2*D;
	betal=T*T-4*D-2*T*P;
	gamma=2*D;
	delta=(T/1000000*2);
	
	for(i=0;i<4000;i++){
	READ_bytes(MPU6050_ADDR, ACCEL_XOUT_H_REG, Data);		
	ACCEL_Xout = ((uint16_t) (Data[0]<<8 | Data[1]));
	ACCEL_Yout = ((uint16_t) (Data[2]<<8 | Data[3]));
	ACCEL_Zout = ((uint16_t) (Data[4]<<8 | Data[5]));
	SUM_X_A += ACCEL_Xout; 
	SUM_Y_A += ACCEL_Yout; 
	SUM_Z_A += (ACCEL_Zout-16383); 
	READ_bytes(MPU6050_ADDR, GYRO_XOUT_H_REG, Data);
	GYRO_Xout = ((int16_t) (Data[0]<<8 | Data[1])) ;
	GYRO_Yout = ((int16_t) (Data[2]<<8 | Data[3])) ;
	GYRO_Zout = ((int16_t) (Data[4]<<8 | Data[5])) ;
	SUM_X_G += GYRO_Xout; 
	SUM_Y_G += GYRO_Yout; 
	SUM_Z_G += GYRO_Zout;
}
	XA0=(int)(SUM_X_A/4000);YA0=(int)(SUM_Y_A/4000);ZA0=(int)(SUM_Z_A/4000);
	XG0=(int)(SUM_X_G/4000);YG0=(int)(SUM_Y_G/4000);ZG0=(int)(SUM_Z_G/4000);
	
	t2=t;//thoi gian bat dau chuong trinh loop


	__enable_irq();
		
	while (1){
	
	}
}

//------ham tinh thoi gian--------
double	fun_detal_t (void){
	t1=t2;
	t2=t;
	return (t2-t1)/1000000.00;
}

//-------------Code interrupt--------------------------------------------------
//interrupt sample time
//ngat TIM1 de cap dap ung dau ra+100ms doc cam bien
void TIM1_UP_IRQHandler (void){
	TIM1->SR &= ~TIM_SR_UIF;//clear flag interrupt
	//----------------
	//code 
	if (n==20) {
		n=0;
//I)setpoint
		
//reset value
	ex1=0;ex2=0;
	ey1=0;ey2=0;
	ez1=0;ez2=0;
	
}
	else{ n++;
//PID	
//II)current value 
	
	READ_bytes(MPU6050_ADDR, ACCEL_XOUT_H_REG, Data);		
	ACCEL_Xout = ((uint16_t) (Data[0]<<8 | Data[1]))-XA0;
	ACCEL_Yout = ((uint16_t) (Data[2]<<8 | Data[3]))-YA0;
	ACCEL_Zout = ((uint16_t) (Data[4]<<8 | Data[5]))-ZA0;
	
	READ_bytes(MPU6050_ADDR, GYRO_XOUT_H_REG, Data);
	GYRO_Xout = ((int16_t) (Data[0]<<8 | Data[1])) -XG0;
	GYRO_Yout = ((int16_t) (Data[2]<<8 | Data[3])) -YG0;
	GYRO_Zout = ((int16_t) (Data[4]<<8 | Data[5])) -ZG0;
	
	detal_t = fun_detal_t();
		
	goc_x1 += (GYRO_Xout/131.00)*detal_t;
	goc_y1 += (GYRO_Yout/131.00)*detal_t;
	goc_z1 += (GYRO_Zout/131.00)*detal_t;
		
	goc_x2=atan2f(ACCEL_Yout,ACCEL_Zout)*57.2957795;
	goc_y2=atan2f(-ACCEL_Xout,ACCEL_Zout)*57.2957795;
		
	goc_x=goc_x1*0.97+goc_x2*0.03;//
	goc_y=goc_y1*0.97+goc_y2*0.03;//
	z=atan2f(1.00,1.00)*57.2957795;
	
//tinh gia tri dau ra
		ux1=ux;
		ex2=ex1;
		ex1=ex;
		ex=set_x-goc_x;
		ux=(ampha*ex+betal*ex+gamma*ex2+ux1)/delta;
		
		uy1=uy;
		ey2=ey1;
		ey1=ey;
		ey=set_y-goc_y;
		uy=(ampha*ey+betal*ey+gamma*ey2+uy1)/delta;
		
		uz1=uz;
		ez2=ez1;
		ez1=ez;
		ez=set_z-goc_z;
		uz=(ampha*ez+betal*ez+gamma*ez2+uz1)/delta;
		
		
		TIM4->CCR1 = ux*proportion;
		
	}
	//end code
}


//systick interrupt
void SysTick_Handler(void)
{
	SysTick->CTRL &= ~SysTick_CTRL_COUNTFLAG_Pos;
	t++;
}


