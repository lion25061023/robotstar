#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"
#include "MPU6050_Reg.h"
#include <math.h>
#include "inv_mpu_dmp_motion_driver.h"
#include "inv_mpu.h"
#include "Serial.h"

#define mpu6050_address 0xD0
#define DEFAULT_MPU_HZ   (100) //输出速度
#define q30        1073741824.0f  //q30格式，long转float时的除数


static signed char gyro_orientation[9] = { 1,  0,  0,
                                           0,  1,  0,
                                           0,  0,  1};
float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;
unsigned long sensor_timestamp;
short gyro[3], accel[3], sensors;
unsigned char more;
long quat[4];
										   
void mpu6050_writereg(uint8_t regaddress, uint8_t data)
{
	my_i2c_start();
	my_i2c_sendbyte(mpu6050_address);
	my_i2c_receiveackbit();
	my_i2c_sendbyte(regaddress);
	my_i2c_receiveackbit();
	my_i2c_sendbyte(data);
	my_i2c_receiveackbit();
	my_i2c_stop();
}




uint8_t mpu6050_receivereg(uint8_t regaddress)
{
	uint8_t data;
	my_i2c_start();
	my_i2c_sendbyte(mpu6050_address);
	my_i2c_receiveackbit();
	my_i2c_sendbyte(regaddress);
	my_i2c_receiveackbit();
	
	my_i2c_start();
	my_i2c_sendbyte(mpu6050_address |0x01);
	my_i2c_receiveackbit();
	data= my_i2c_receivebyte();
	my_i2c_sendackbit(1);
	my_i2c_stop();
	
	return data;
	
		
}
void mpu6050_init()
{
	my_i2c_init();
	//这些数据写入还是挺复杂的，建议翻一下手册，看看每一位的含义
	 mpu6050_writereg(MPU6050_PWR_MGMT_1, 0x01);
	 mpu6050_writereg(MPU6050_PWR_MGMT_2, 0x00);
	 mpu6050_writereg(MPU6050_SMPLRT_DIV, 0x09);
	 mpu6050_writereg(MPU6050_CONFIG, 0x06);
	 mpu6050_writereg(MPU6050_GYRO_CONFIG, 0x18);
	 mpu6050_writereg(MPU6050_ACCEL_CONFIG, 0x18);

}

void mpu6050_getdata (int16_t*AccX, int16_t *AccY, int16_t *AccZ, 
				      int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	uint8_t DataH, DataL;
	
	DataH =  mpu6050_receivereg(MPU6050_ACCEL_XOUT_H);
	DataL =  mpu6050_receivereg(MPU6050_ACCEL_XOUT_L);
	*AccX = (DataH << 8) | DataL;
	
	DataH =  mpu6050_receivereg(MPU6050_ACCEL_YOUT_H);
	DataL = mpu6050_receivereg(MPU6050_ACCEL_YOUT_L);
	*AccY = (DataH << 8) | DataL;
	
	DataH = mpu6050_receivereg(MPU6050_ACCEL_ZOUT_H);
	DataL =  mpu6050_receivereg(MPU6050_ACCEL_ZOUT_L);
	*AccZ = (DataH << 8) | DataL;
	
	DataH =  mpu6050_receivereg(MPU6050_GYRO_XOUT_H);
	DataL =  mpu6050_receivereg(MPU6050_GYRO_XOUT_L);
	*GyroX = (DataH << 8) | DataL;
	
	DataH =  mpu6050_receivereg(MPU6050_GYRO_YOUT_H);
	DataL = mpu6050_receivereg(MPU6050_GYRO_YOUT_L);
	*GyroY = (DataH << 8) | DataL;
	
	DataH =  mpu6050_receivereg(MPU6050_GYRO_ZOUT_H);
	DataL =  mpu6050_receivereg(MPU6050_GYRO_ZOUT_L);
	*GyroZ = (DataH << 8) | DataL;

}
uint8_t mpu6050_getid()
{
	return  mpu6050_receivereg(MPU6050_WHO_AM_I);
}


uint8_t mpu6050_write_len(uint8_t addr, uint8_t reg,uint8_t len,uint8_t *Buf)
{
	my_i2c_start();
	my_i2c_sendbyte((addr <<1) | 0x00);//发送器件地址并写
	if(my_i2c_receiveackbit())    
	{
		my_i2c_stop();
		return 1;
	}
	my_i2c_sendbyte(reg); //写寄存器地址
	if(my_i2c_receiveackbit())    
	{
		my_i2c_stop();
		return 1;
	}
	while (len--)
	{
		my_i2c_sendbyte(*Buf++);
		if(my_i2c_receiveackbit())    
		{
			my_i2c_stop();
			return 1;
		}
		
	}
	my_i2c_stop();
	return 0;

	
}

uint8_t mpu_read_len(uint8_t addr, uint8_t reg,uint8_t len,uint8_t *Buf)
{
	my_i2c_start();
	my_i2c_sendbyte((addr <<1) | 0x00);//发送器件地址并写
	if(my_i2c_receiveackbit())    
	{
		my_i2c_stop();
		return 1;
	}
	my_i2c_sendbyte(reg); //写寄存器地址
	if(my_i2c_receiveackbit())    
	{
		my_i2c_stop();
		return 1;
	}
	my_i2c_start();
	my_i2c_sendbyte((addr <<1) | 0x01);
	if(my_i2c_receiveackbit())    
	{
		my_i2c_stop();
		return 1;
	}
	while(len--)
	{
		*Buf++ = my_i2c_receivebyte();
		if(len)
		{
			my_i2c_sendackbit(0);
		}
		else
		{
			my_i2c_sendackbit(1);
		}
	
	}
	my_i2c_stop();
	return 0;
	
}

void MPU6050_DMPInit(void)
{
	uint8_t res = 0;
    
	my_i2c_init();
    res = mpu_init();
    if(!res)
    {
        Serial_Printf("mpu initialization complete ......\r\n");
        
		//ʨ׃̹ѨҪքԫِǷ
        res = mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL);
        if(!res)
            Serial_Printf("mpu_set_sensor complete ......\r\n");
        else
            Serial_Printf("mpu_set_sensor come across error ......\r\n");
		
        //ʨ׃FIFO
        res = mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL);
        if(!res)
            Serial_Printf("mpu_configure_fifo complete ......\r\n");
        else
            Serial_Printf("mpu_configure_fifo come across error ......\r\n");
        
		//ʨ׃ӉҹÊ
        res = mpu_set_sample_rate(DEFAULT_MPU_HZ);
        if(!res)
            Serial_Printf("mpu_set_sample_rate complete ......\r\n");
        else
            Serial_Printf("mpu_set_sample_rate come across error ......\r\n");
        
		//ݓ՘DMPڌݾ
        res = dmp_load_motion_driver_firmware();
        if(!res)
            Serial_Printf("dmp_load_motion_driver_firmware complete ......\r\n");
        else
            Serial_Printf("dmp_load_motion_driver_firmware come across error ......\r\n");
        
		//ʨ׃ΓÝӇ׽в
        res = dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation));
        if(!res)
            Serial_Printf("dmp_set_orientation complete ......\r\n");
        else
            Serial_Printf("dmp_set_orientation come across error ......\r\n");
        
		//ʨ׃DMP٦Ŝ
        res = dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |	              
              DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
              DMP_FEATURE_GYRO_CAL);
        if(!res)
            Serial_Printf("dmp_enable_feature complete ......\r\n");
        else
            Serial_Printf("dmp_enable_feature come across error ......\r\n");
        
		//ʨ׃DMPˤԶ̙Ê(خճһӬڽ200Hz)
        res = dmp_set_fifo_rate(DEFAULT_MPU_HZ);
        if(!res)
            Serial_Printf("dmp_set_fifo_rate complete ......\r\n");
        else
            Serial_Printf("dmp_set_fifo_rate come across error ......\r\n");
        
		//ؔݬ
        res = run_self_test();
        if(!res)
            Serial_Printf("mpu_run_self_test complete ......\r\n");
        else
            Serial_Printf("mpu_run_self_test come across error ......\r\n");
        
		//ʹŜDMP
        res = mpu_set_dmp_state(1);
        if(!res)
            Serial_Printf("mpu_set_dmp_state complete ......\r\n");
        else
            Serial_Printf("mpu_set_dmp_state come across error ......\r\n");
    }
    else
    {
        Serial_Printf("mpu initialization come across error ......\r\n");
        while(1);
    }
}

/**
  * @brief  ܱȡDMPԦm۳˽ߝگ˽
  * @param  Pitch ةҶއìޫ׈:0.1£ì׶Χ:-90.0£ ~ +90.0£
  * @param  Roll  ۡڶއìޫ׈:0.1£ì׶Χ:-180.0£~ +180.0£
  * @param  yaw   ƫڽއìޫ׈:0.1£ì׶Χ:-180.0£~ +180.0£
  * @retval ׵ܘ0ҭʾֽӣìǤ̻ҭʾʧќ
  */
uint8_t MPU6050_ReadDMP(float *Pitch, float *Roll, float *Yaw)
{	
	if(dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors, &more))  return 1;	
	if(sensors & INV_WXYZ_QUAT)
	{    
        q0 = quat[0] / q30;    //q30ٱʽתۻΪء֣˽
        q1 = quat[1] / q30;
        q2 = quat[2] / q30;
        q3 = quat[3] / q30;
        //݆̣փսةҶއbۡڶއۍƫڽއ
        *Pitch = asin(-2 * q1 * q3 + 2 * q0 * q2) * 57.3; 	
        *Roll  = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1) * 57.3;
        *Yaw   = atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2 * q2 - 2 * q3 * q3 + 1) * 57.3;
    }else  return 2;
    return 0;
}
