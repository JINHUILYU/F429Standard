#include "bsp_basic_tim.h"

/**
 * @brief  基本定时器 TIMx, x[6, 7]中断优先级配置
 * @param  none
 * @retval none
 */
static void TIMx_NVIC_Configuration(void) {
    NVIC_InitTypeDef NVIC_InitStructure;
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    // 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM_IRQn;
    // 设置抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    // 设置子优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
 * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
 * 另外三个成员是通用定时器和高级定时器才有.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         都有
 * TIM_CounterMode	     TIMx,x[6,7]没有，其他都有（基本定时器）
 * TIM_Period            都有
 * TIM_ClockDivision     TIMx,x[6,7]没有，其他都有(基本定时器)
 * TIM_RepetitionCounter TIMx,x[1,8]才有(高级定时器)
 *-----------------------------------------------------------------------------
 */
static void TIM_Mode_Config(void) {
    // 开启TIM6_CLK
    RCC_APB1PeriphClockCmd(BASIC_TIM_CLK, ENABLE);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    // 选择内部时钟作为定时器的时钟源
    TIM_InternalClockConfig(BASIC_TIM);

    // 时钟分频
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    // 计数器计数模式，向上计数
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    /* 累计 TIM_Period个后产生一个更新或者中断*/
    // 当定时器从0计数到9999，即为10000次，为一个定时周期
    TIM_TimeBaseStructure.TIM_Period = 10000 - 1;

    // 定时器时钟源TIMxCLK = 2 * PCLK1
    //			   PCLK1 = HCLK / 4
    //				=> TIMxCLK = HCLK / 2 = SystemCoreClock / 2 = 90MHz
    // 设定定时器频率为 = TIMxCLK / (TIM_Prescaler + 1) = 10000Hz
    // 在10k的频率下计10000个数为一个定时周期，所以定时周期为1s
    TIM_TimeBaseStructure.TIM_Prescaler = 9000 - 1;

    // 高级定时器的重复计数器，基本定时器不用设置
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

    // 初始化定时器TIM6时基单元
    TIM_TimeBaseInit(BASIC_TIM, &TIM_TimeBaseStructure);

    // 清除定时器TIM6更新中断标志位，防止开启定时器后立刻产生中断
    TIM_ClearFlag(BASIC_TIM, TIM_FLAG_Update);

    // 开启定时器TIM6更新中断
    TIM_ITConfig(BASIC_TIM, TIM_IT_Update, ENABLE);

    // 使能定时器TIM6
    TIM_Cmd(BASIC_TIM, ENABLE);
}

/**
  * @brief  初始化基本定时器定时，1s产生一次中断
  * @param  none
  * @retval none
  */
void TIMx_Configuration(void) {
    TIMx_NVIC_Configuration();
    TIM_Mode_Config();
}
