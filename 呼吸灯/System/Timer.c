#include "stm32f10x.h"                  // Device header
void Timer_3_init(void)
{
	//使能APB1时钟的tim外设

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	//配置时钟源为内部时钟
	TIM_InternalClockConfig(TIM3);
	//定义初始化内部时钟的结构体
	TIM_TimeBaseInitTypeDef  TIM_TimeBasestructure;
	//分频采样的分频数
	TIM_TimeBasestructure.TIM_ClockDivision=TIM_CKD_DIV1 ;
	//计数模式（向上计数）
	TIM_TimeBasestructure.TIM_CounterMode=TIM_CounterMode_Up   ;
	//计数个数，目标值，arr
	
	TIM_TimeBasestructure.TIM_Period=100-1;
	//预分频（可以理解为把72mhz分成10000个频道？）psc
	
	TIM_TimeBasestructure.TIM_Prescaler=7200-1;
	//重复计数器（高级计数器才有）（不懂）
	TIM_TimeBasestructure.TIM_RepetitionCounter=0;
	//初始化tim3计时器
	TIM_TimeBaseInit(TIM3, &TIM_TimeBasestructure);
	
	
	
	//开启更新中断到nvic的通路？？
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef  NVIC_InitSTRUCTURE;
	//tim2在nvic的通道
	NVIC_InitSTRUCTURE.NVIC_IRQChannel= TIM3_IRQn; 
	NVIC_InitSTRUCTURE.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitSTRUCTURE.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitSTRUCTURE.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitSTRUCTURE);
	//启动定时器
	TIM_Cmd(TIM3,ENABLE);
	
	
}

////系统自带的中断服务函数，如果发生中断则跳转至该函数执行相应程序（如..）
//void TIM3_IRQHandler (void)
//{
//	//判断是否为更新中断引发的中断
//	if (TIM_GetITStatus(TIM3,TIM_IT_Update)==SET)
//	{
//		
//		//手动清除中断标志位
//		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
//	}
//}
