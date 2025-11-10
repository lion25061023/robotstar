#ifndef __MyI2C__H
#define __MyI2C__H
void my_i2c_w_scl(uint8_t bitvalue);

void my_i2c_w_sda(uint8_t bitvalue);

void my_i2c_init(void);

void my_i2c_start(void);
void my_i2c_stop(void);
void my_i2c_sendbyte(uint8_t byte);
uint8_t my_i2c_receivebyte(void);
void my_i2c_sendackbit(uint8_t ackbit);
uint8_t my_i2c_receiveackbit(void);

	
	

#endif

