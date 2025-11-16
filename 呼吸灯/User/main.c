#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "pwm.h"
#include "Serial.h"
#include "Timer.h"
#include <stdio.h>
#include <string.h> 

//频率调整参数(先不考虑亮度吧)
int led_fre;
char order_fr[100];
char led_fr_str[4];
int led_fr_num[4];
//亮度调整参数
int led_light;
char order[100];
char led_str_num[4];
int led_num[4];
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
		pwm_setcompare1(1000);
		pwm_setcompare2(1000);
		pwm_setcompare3(1000);
		pwm_setcompare4(1000);
		if (Serial_RxFlag == 1)		//如果接收到数据包
		{
			if (Serial_RxPacket[4]=='l')
			{
				for (int i=0;i<4;i++)
			{
				led_num[i]=0;
			}
			
			sscanf(Serial_RxPacket,"%[^:]:%d,%s",order,&led_light,led_str_num);
			
			int len=strlen(led_str_num);
			for (int i=0;i<len;i++)
			{
				if (led_str_num[i]=='1')
				{
					led_num[0]=1;
				}
				else if(led_str_num[i]=='2')
				{
					led_num[1]=1;
				}
				else if(led_str_num[i]=='3')
				{
					led_num[2]=1;
				}
				else if(led_str_num[i]=='4')
				{
					led_num[3]=1;
				}
				
				
				
			}
			for (int i=0;i<4;i++)
			{
				if (led_num[i]==1)
				{
					if (i==0) pwm_setcompare1(led_light);
					if (i==1) pwm_setcompare2(led_light);
					if (i==2) pwm_setcompare3(led_light);
					if (i==3) pwm_setcompare4(led_light);
					OLED_ShowString(i+1,3,"    ");
					OLED_ShowNum(i+1,3,led_light,4);
					pwm_setcompare1(led_light);
				}
			}
			}
			
			else if (Serial_RxPacket[4]=='f')
			{
//				for (int i=0;i<4;i++)
//				{
//				led_fr_num[i]=0;
//				}
				OLED_ShowNum(1,8,led_fr_num[0],1);
				OLED_ShowString(3,7,led_fr_str);
				sscanf(Serial_RxPacket,"%[^:]:%d,%s",order,&led_fre,led_fr_str);
				int len=strlen(led_fr_str);
				for (int i=0;i<len;i++)
				{
				if (led_fr_str[i]=='1')
				{
					led_fr_num[0]=1;
				}
				else if(led_fr_str[i]=='2')
				{
					led_fr_num[1]=1;
				}
				else if(led_fr_str[i]=='3')
				{
					led_fr_num[2]=1;
				}
				else if(led_fr_str[i]=='4')
				{
					led_fr_num[3]=1;
				}
				
				
				
				}
				
				
			}
			
				Serial_RxFlag = 0;	
		}
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
			if (count<=led_fre)
			{
				for (int i=0;i<4;i++)
				{
					if (led_fr_num[i]==1)
					{
						if (i==0) pwm_setcompare1(1000);
						if (i==1) pwm_setcompare2(1000);
						if (i==2) pwm_setcompare3(1000);
						if (i==3) pwm_setcompare4(1000);
						OLED_ShowString(i+1,3,"    ");
						OLED_ShowNum(i+1,3,led_fre,4);
						
						
					}
				}
			}
			
		}
		else
		{
			count=0;
			for (int i=0;i<4;i++)
				{
					if (led_fr_num[i]==1)
					{
						if (i==0) pwm_setcompare1(0);
						if (i==1) pwm_setcompare2(0);
						if (i==2) pwm_setcompare3(0);
						if (i==3) pwm_setcompare4(0);
						
					}
				}
		}
		
		//手动清除中断标志位
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	}
 }
