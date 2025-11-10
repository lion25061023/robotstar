#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "pwm.h"
#include "Serial.h"
#include "Timer.h"
#include <stdio.h>
#include <string.h> 
uint16_t i;

int led_num;
int led_light;
int led_fre;
int bi=50;
char order[100];
int main(void)
{
	
	OLED_Init();
	pwm_init();
	Serial_Init();
	Timer_3_init();
	for (int i=1;i<=4;i++)
	{
		OLED_ShowNum(i,1,i,1);
		OLED_ShowNum(i,3,0,4);
	}
	
	while (1)
	{
		if (Serial_RxFlag == 1)		//如果接收到数据包
		{
			sscanf(Serial_RxPacket,"%[^:]:%d,%d",order,&led_num,&led_light);
			
			
			
			
			if (led_num==1)
			{
				OLED_ShowString(1,3,"    ");
				pwm_setcompare1(led_light);
			}
			else if (led_num==2)
			{
				OLED_ShowString(2,3,"    ");
				pwm_setcompare2(led_light);
			}
			else if (led_num==3)
			{
				OLED_ShowString(3,3,"    ");
				pwm_setcompare3(led_light);
			}
			else if (led_num==4)
			{
				OLED_ShowString(4,3,"    ");
				pwm_setcompare4(led_light);
			}
			OLED_ShowNum(led_num,1,led_num,1);
			OLED_ShowNum(led_num,3,led_light,4);
			Serial_RxFlag = 0;
			
		}
//		for (i=0;i<=100;i++)
//		{
//			pwm_setcompare1(i);
//			pwm_setcompare2(i);
//			pwm_setcompare3(i);
//			pwm_setcompare4(i);
//			Delay_ms(20);
//			
//		}
//		for (i=0;i<100;i++)
//			{
//			pwm_setcompare1(100-i);
//			pwm_setcompare2(100-i);
//			pwm_setcompare3(100-i);
//			pwm_setcompare4(100-i);
//			Delay_ms(20);
//			}
		
		
		
	}
}

uint8_t count;
//系统自带的中断服务函数，如果发生中断则跳转至该函数执行相应程序（如..）
//目前频率100hz
void TIM3_IRQHandler (void)
{
	//判断是否为更新中断引发的中断
	
	if (TIM_GetITStatus(TIM3,TIM_IT_Update)==SET)
	{
		count++;
		if (count<=100)
		{
			if (count<=bi)
			{
				if (led_num==1)
				{
					pwm_setcompare1(led_light);
				}
				else if (led_num==2)
				{
					pwm_setcompare2(led_light);
				}
				else if (led_num==3)
				{
					pwm_setcompare3(led_light);
				}
				else if (led_num==4)
				{
					pwm_setcompare4(led_light);
				}
			}
			else
			{
				if (led_num==1)
				{
					pwm_setcompare1(0);
				}
				else if (led_num==2)
				{
					pwm_setcompare2(0);
				}
				else if (led_num==3)
				{
					pwm_setcompare3(0);
				}
				else if (led_num==4)
				{
					pwm_setcompare4(0);
				}
				
			}
		}
		else
		{
			count=0;
		}
		
		//手动清除中断标志位
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	}
}
