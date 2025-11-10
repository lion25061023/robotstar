#include "stm32f10x.h"                  // Device header
#include "Delay.h"
void my_i2c_w_scl(uint8_t bitvalue)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_10,(BitAction)bitvalue);
	Delay_us(10);
}
void my_i2c_w_sda(uint8_t bitvalue)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_11,(BitAction)bitvalue);
	Delay_us(10);
}
uint8_t  my_i2c_r_sda(void)
{
	uint8_t bitvalue;
	bitvalue=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11);
	Delay_us(10);
	return bitvalue;
}
void my_i2c_init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 |GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);		
	GPIO_SetBits( GPIOB, GPIO_Pin_10 | GPIO_Pin_11);
}


void my_i2c_start()
{
	my_i2c_w_sda(1);
	my_i2c_w_scl(1);
	my_i2c_w_sda(0);
	my_i2c_w_scl(0);
	
	
}

void my_i2c_stop()
{
	my_i2c_w_sda(0);
	my_i2c_w_scl(1);
	my_i2c_w_sda(1);
}

void my_i2c_sendbyte(uint8_t byte)
{
	uint8_t i;
	for (i=0;i<8;i++)
	{
		my_i2c_w_sda(byte & (0x80>>i));
		my_i2c_w_scl(1);
		my_i2c_w_scl(0);
	}
}

uint8_t my_i2c_receivebyte(void)
{
	uint8_t i,byte=0x00;
	my_i2c_w_sda(1);
	for (i=0;i<8;i++)
	{
		my_i2c_w_scl(1);
		if (my_i2c_r_sda()==1)
		{
			byte |= (0x80>>i);
		}
		my_i2c_w_scl(0);
	}
	return byte;
}

void my_i2c_sendackbit(uint8_t ackbit)
{
	
	
		my_i2c_w_sda(ackbit);
		my_i2c_w_scl(1);
		my_i2c_w_scl(0);
	
}

uint8_t my_i2c_receiveackbit(void)
{
	uint8_t ackbit;
	my_i2c_w_sda(1);
		
	my_i2c_w_scl(1);
	ackbit=my_i2c_r_sda();
	my_i2c_w_scl(0);
	return ackbit;
	
}

