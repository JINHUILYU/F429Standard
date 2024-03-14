#include "stm32f4xx.h"                  // Device header

/**
  * 函    数：PWM初始化
  * 参    数：无
  * 返 回 值：无
  */
void PWM_Init(void)
{
	/*开启时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);

    GPIO_PinAFConfig(GPIOH,  GPIO_PinSource10, GPIO_AF_TIM5);
    GPIO_PinAFConfig(GPIOH,  GPIO_PinSource11, GPIO_AF_TIM5);
    GPIO_PinAFConfig(GPIOH,  GPIO_PinSource12, GPIO_AF_TIM5);

    /*GPIO初始化*/
    // RED
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOH, &GPIO_InitStructure);

    // GREEN
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_Init(GPIOH, &GPIO_InitStructure);

    // BLUE
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_Init(GPIOH, &GPIO_InitStructure);

    /*配置时钟源*/
	TIM_InternalClockConfig(TIM5); // 选择TIM5为内部时钟，若不调用此函数，TIM默认也为内部时钟
	
	/*时基单元初始化*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				// 定义结构体变量
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     // 时钟分频，选择不分频，此参数用于配置滤波器时钟，不影响时基单元功能
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 计数器模式，选择向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 512 - 1;					// 计数周期，即ARR的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 12 - 1;				// 预分频器，即PSC的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            // 重复计数器，高级定时器才会用到
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStructure);
	
	/*输出比较初始化*/
	TIM_OCInitTypeDef TIM_OCInitStructure;							// 定义结构体变量
	TIM_OCStructInit(&TIM_OCInitStructure);		    // 结构体初始化，若结构体没有完整赋值，则最好执行此函数，给结构体所有成员都赋一个默认值，避免结构体初值不确定的问题
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				// 输出比较模式，选择PWM模式1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;		// 输出极性 当定时器计数值小于CCR1_Val时为低电平 LED灯亮
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	// 输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;								// 初始的CCR值
	TIM_OC1Init(TIM5, &TIM_OCInitStructure);
	
	/*TIM使能*/
	TIM_Cmd(TIM5, ENABLE); // 使能TIM5，定时器开始运行
}

/**
  * 函    数：PWM设置CCR
  * 参    数：Compare 要写入的CCR的值，范围：0~100
  * 返 回 值：无
  * 注意事项：CCR和ARR共同决定占空比，此函数仅设置CCR的值，并不直接是占空比
  *           占空比Duty = CCR / (ARR + 1)
  */
void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM5, Compare);		//设置CCR1的值
}
