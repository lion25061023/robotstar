#ifndef __mpu6050__H
#define __mpu6050__H

void mpu6050_writereg(uint8_t regaddress, uint8_t data);

uint8_t mpu6050_receivereg(uint8_t regaddress);
uint8_t mpu6050_getid(void);
void mpu6050_init(void);
void mpu6050_getdata (int16_t*AccX, int16_t *AccY, int16_t *AccZ, 
				      int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ);


uint8_t mpu6050_write_len(uint8_t addr, uint8_t reg,uint8_t len,uint8_t *Buf);
uint8_t mpu_read_len(uint8_t addr, uint8_t reg,uint8_t len,uint8_t *Buf);
void MPU6050_DMPInit(void);
uint8_t MPU6050_ReadDMP(float *Pitch, float *Roll, float *Yaw);
#endif


