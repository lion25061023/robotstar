#include "stm32f10x.h"                  // Device header

void pwm_init(void)
{
	//初始化gpio
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//开启GPIOA的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2  | GPIO_Pin_3 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);						//将PA1和PA2引脚初始化为推挽输出
	
	/*设置GPIO初始化后的默认电平*/
	GPIO_SetBits(GPIOA, GPIO_Pin_0);
	GPIO_SetBits(GPIOA, GPIO_Pin_1);	
	GPIO_SetBits(GPIOA, GPIO_Pin_2);	
	GPIO_SetBits(GPIOA, GPIO_Pin_3);		
	//使能APB1时钟的tim外设

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	//配置时钟源为内部时钟
	TIM_InternalClockConfig(TIM2);
	//定义初始化内部时钟的结构体
	TIM_TimeBaseInitTypeDef  TIM_TimeBasestructure;
	//分频采样的分频数
	TIM_TimeBasestructure.TIM_ClockDivision=TIM_CKD_DIV1 ;
	//计数模式（向上计数）
	TIM_TimeBasestructure.TIM_CounterMode=TIM_CounterMode_Up   ;
	//计数个数，目标值，arr
	
	TIM_TimeBasestructure.TIM_Period=1000-1;
	//预分频（可以理解为把72mhz分成10000个频道？）psc
	
	TIM_TimeBasestructure.TIM_Prescaler=72-1;
	//重复计数器（高级计数器才有）（不懂）
	TIM_TimeBasestructure.TIM_RepetitionCounter=0;
	//初始化tim2计时器
	TIM_TimeBaseInit(TIM2, &TIM_TimeBasestructure);
	//通道和pin口有对应关系
	TIM_OCInitTypeDef TIM_Initstructure;
	//给所有参数赋初始值
	TIM_OCStructInit(&TIM_Initstructure); 
	
	
	//配置输出比较的8种模式，此处选pwm1
	TIM_Initstructure.TIM_OCMode =TIM_OCMode_PWM1     ;
	//选极性
	TIM_Initstructure.TIM_OCPolarity=TIM_OCPolarity_High  ;
	//输出使能
	TIM_Initstructure.TIM_OutputState=TIM_OutputState_Enable ;
	//CCR的值
	TIM_Initstructure.TIM_Pulse=0;
	TIM_OC1Init(TIM2,&TIM_Initstructure);
	TIM_OC2Init(TIM2,&TIM_Initstructure);
	TIM_OC3Init(TIM2,&TIM_Initstructure);
	TIM_OC4Init(TIM2,&TIM_Initstructure);
	
	
	
	TIM_Cmd(TIM2,ENABLE);


}
void pwm_setcompare1(uint16_t compare)
{
		//设置CCR寄存器的值
	
	TIM_SetCompare1(TIM2,compare);
	
}
void pwm_setcompare2(uint16_t compare)
{
		//设置CCR寄存器的值
	
	TIM_SetCompare2(TIM2,compare);
	
}
void pwm_setcompare3(uint16_t compare)
{
		//设置CCR寄存器的值
	
	TIM_SetCompare3(TIM2,compare);
	
}
void pwm_setcompare4(uint16_t compare)
{
		//设置CCR寄存器的值
	
	TIM_SetCompare4(TIM2,compare);
}
