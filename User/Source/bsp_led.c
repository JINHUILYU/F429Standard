#include "bsp_led.h"

/**
 * @brief  LED配置初始化
 * @param  none
 * @retval none
 */
void LED_GPIO_Config(void) {
    /*定义一个 GPIO_InitTypeDef 类型的结构体*/
    GPIO_InitTypeDef GPIO_InitStructure;

    /* 使能 LED 相关的 GPIO 外设时钟 */
    RCC_AHB1PeriphClockCmd(LED1_GPIO_CLK, ENABLE);
    RCC_AHB1PeriphClockCmd(LED2_GPIO_CLK, ENABLE);
    RCC_AHB1PeriphClockCmd(LED3_GPIO_CLK, ENABLE);

    /* 选择要控制的 GPIO 引脚 */
    GPIO_InitStructure.GPIO_Pin = LED1_PIN;

    /* 设置引脚模式为输出模式 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

    /* 设置引脚的输出类型为推挽输出 */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

    /* 设置引脚为上拉模式 */
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

    /* 设置引脚速率为 2MHz */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

    /* 调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO */
    GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);

    /* 选择要控制的GPIO引脚 */
    GPIO_InitStructure.GPIO_Pin = LED2_PIN;
    GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);

    /* 选择要控制的GPIO引脚 */
    GPIO_InitStructure.GPIO_Pin = LED3_PIN;
    GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);

    /* 关闭RGB灯 */
    LED_RGBOFF;
}
