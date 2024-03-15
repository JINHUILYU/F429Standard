//#define Exp_1 // 实验一 点亮LED
//#define Exp_2 // 实验二 按键检测
//#define Exp_3 // 实验三 串口通信
//#define Exp_4 // 实验四 串口通信(中断接收模式)
//#define Exp_5 // 实验五 蜂鸣器
//#define Exp_6 // 实验六 外部中断
//#define Exp_7 // 实验七 SysTick定时器
//#define Exp_8 // 实验八 PWM呼吸灯
//#define Exp_9 // 实验九 PWM呼吸灯(江科大)
//#define Exp_10 // 实验十 基本定时器
//#define Exp_11 // 实验十一 高级定时器
#define Exp_12 // 实验十二 红外遥控接收

#ifdef Exp_1
#include "bsp_led.h"
#endif

#ifdef Exp_2
#include "bsp_led.h"
#include "bsp_key.h"
#endif

#ifdef Exp_3
#include "bsp_led.h"
#include "bsp_usart.h"
#include "retarget.h"

static void Show_Message(void);
#endif

#ifdef Exp_4
#include "bsp_debug_usart.h"
#include "retarget.h"
#endif

#ifdef Exp_5
#include "bsp_beep.h"
#endif

#ifdef Exp_6
#include "bsp_led.h"
#include "bsp_exti.h"
#endif

#ifdef Exp_7
#include "bsp_led.h"
#include "bsp_SysTick.h"
#endif

#ifdef Exp_8
#include "bsp_breath_led.h"

#define SOFT_DELAY() Delay(0x3FFFFFF);
#endif

#ifdef Exp_9
#include "bsp_SysTick.h"
#include "PWM.h"
#endif

#ifdef Exp_10
#include "bsp_led.h"
#include "bsp_basic_tim.h"
#endif

#ifdef Exp_11
#include "bsp_led.h"
#include "bsp_advanced_tim.h"
#endif

#ifdef Exp_12

#include "bsp_led.h"
#include "bsp_irda.h"
#include "bsp_debug_usart.h"
#include "bsp_SysTick.h"
#include "retarget.h"

#define CLI() __set_PRIMASK(1)        /* 关闭总中断 */
#define SEI() __set_PRIMASK(0)        /* 开放总中断 */

extern uint8_t frame_flag;
extern uint8_t isr_cnt;
extern uint8_t frame_cnt;
#endif

// 该变量在定时器中断服务函数中使用，用于控制各通道的输出
// 修改该变量的值可直接改变呼吸灯的颜色
// 变量格式：RGB888
__IO uint32_t rgb_color = 0xFF00FF;

void Delay(__IO u32 nCount);

int main(void) {
    /*实验一 点亮LED*/
#ifdef Exp_1
    LED_GPIO_Config(); // LED端口初始化
    while(1) {
        LED1(ON);
        Delay(0xFFFFFF);
        LED1(OFF);

        LED2(ON);
        Delay(0xFFFFFF);
        LED2(OFF);

        LED3(ON);
        Delay(0xFFFFFF);
        LED3(OFF);
        /* 轮流显示红绿蓝黄紫青白 */
        LED_RED;
        Delay(0xFFFFFF);

        LED_GREEN;
        Delay(0xFFFFFF);

        LED_BLUE;
        Delay(0xFFFFFF);

        LED_YELLOW;
        Delay(0xFFFFFF);

        LED_PURPLE;
        Delay(0xFFFFFF);

        LED_CYAN;
        Delay(0xFFFFFF);

        LED_WHITE;
        Delay(0xFFFFFF);

        LED_RGBOFF;
        Delay(0xFFFFFF);
    }
#endif

    /*实验二 按键检测*/
#ifdef Exp_2
    LED_GPIO_Config(); // LED端口初始化
    Key_GPIO_Config(); // 按键初始化
    while(1) {
        if (Key_Scan(KEY1_GPIO_PORT, KEY1_PIN) == KEY_ON) {
            LED1_TOGGLE; // LED1翻转
        }
        if (Key_Scan(KEY2_GPIO_PORT, KEY2_PIN) == KEY_ON) {
            LED2_TOGGLE; // LED2翻转
        }
    }
#endif

    /*实验三 串口通信*/
#ifdef Exp_3
    LED_GPIO_Config(); // LED端口初始化
    USARTx_Config(); // 初始化 USART 配置模式为 115200 8-N-1
    RetargetInit(USART1); // 串口重定向
    Show_Message(); // 显示信息
    char ch;
    while(1) {
        ch = getchar(); // 获取字符指令
        printf("接收到字符：%c\n", ch);
        /* 根据字符指令控制RGB彩灯颜色 */
        switch (ch) {
            case '1': LED_RED;
                break;
            case '2': LED_GREEN;
                break;
            case '3': LED_BLUE;
                break;
            case '4': LED_YELLOW;
                break;
            case '5': LED_PURPLE;
                break;
            case '6': LED_CYAN;
                break;
            case '7': LED_WHITE;
                break;
            case '8': LED_RGBOFF;
                break;
            default:
                /* 如果不是指定指令字符，打印提示信息 */
                Show_Message();
                break;
        }
    }
#endif

    /*实验四 串口通信(中断接收模式)*/
#ifdef Exp_4
    Debug_USART_Config(); // 初始化 DEBUG_USART 配置模式为 115200 8-N-1
    RetargetInit(DEBUG_USART); // 串口重定向
    Usart_SendString(DEBUG_USART, "串口中断接收回显实验\n");
    printf("串口中断接收回显实验\n");
    while(1) {}
#endif

    /*实验五 蜂鸣器*/
#ifdef Exp_5
    BEEP_GPIO_Config(); // 蜂鸣器端口初始化
    while(1) {
        BEEP_ON;
        Delay(0xFFFFFF);
        BEEP_OFF;
        Delay(0xFFFFFF);
    }
#endif

    /*实验六 外部中断*/
#ifdef Exp_6
    LED_GPIO_Config(); // LED端口初始化
    /*
     * 初始化EXTI中断，按下按键会触发中断，触发中断会进入stm32f4xx_it.c文件中的函数
     * KEY1_IRQHandler和KEY2_IRQHandler，处理中断，反转LED灯。
    */
    EXTI_Key_Config();
    // 等待中断触发
    while(1) {}
#endif

    /*实验七 SysTick定时器*/
#ifdef Exp_7
    LED_GPIO_Config(); // LED端口初始化
    /*
     * 配置SysTick 为10us中断一次,时间到后触发定时中断，
     * 进入stm32fxx_it.c文件的SysTick_Handler处理，通过数中断次数计时
     */
    SysTick_Init();
    while(1) {
        LED_RED;
        Delay_us(100000); // 1s 10000 * 10us = 1000ms
        LED_GREEN;
        Delay_ms(1000); // 1s
        LED_BLUE;
        Delay_ms(1000); // 1s
    }
#endif

    /*实验八 PWM呼吸灯*/
#ifdef Exp_8
    /* 初始化呼吸灯 */
    BreathLED_Config();

    while (1) {
        // 可动态修改颜色，使用各种颜色的呼吸灯
        rgb_color = 0xFF00FF;
        SOFT_DELAY();

        rgb_color = 0x8080ff;
        SOFT_DELAY();

        rgb_color = 0xff8000;
        SOFT_DELAY();

        rgb_color = 0xffc90e;
        SOFT_DELAY();
    }
#endif

    /*实验九 PWM呼吸灯(江科大)*/
#ifdef Exp_9
    PWM_Init();

    while (1) {
        for (int i = 0; i <= 100; i++) {
            PWM_SetCompare1(i);
            Delay(0x3FFFF);
        }
        for (int i = 100; i >= 0; i--) {
            PWM_SetCompare1(i);
            Delay(0x3FFFF);
        }
    }
#endif

    /*实验十 基本定时器(TIM6)*/
#ifdef Exp_10
    LED_GPIO_Config(); // LED端口初始化
    /* 初始化基本定时器定时，1s产生一次中断 */
    TIMx_Configuration();
    while(1) {}
#endif

    /*实验十一 高级定时器(TIM2)*/
#ifdef Exp_11
#endif

    /*实验十二 红外遥控接收*/
#ifdef Exp_12
    uint8_t key_val;

    /* config the led */
    LED_GPIO_Config();
    LED1_ON;

    /* 配置SysTick 为10us中断一次 */
    SysTick_Init();

    /* 重新配置SysTick的中断优先级为最高，要不然SysTick延时中断抢占不了IO EXTI中断
     * 因为SysTick初始化时默认配置的优先级是最低的
     * 或者当你用其他定时器做延时的时候，要配置定时器的优先级高于IO EXTI中断的优先级
     */
    NVIC_SetPriority(SysTick_IRQn, 0);


    /* USART config 115200 8-N-1 */
    Debug_USART_Config();
    RetargetInit(DEBUG_USART); // 串口重定向
    printf("\r\n 这是一个红外遥控发射与接收实验 \r\n");

    /* 初始化红外接收头CP1838用到的IO */
    IrDa_Init();

    while (1) {
        if (frame_flag == 1) /* 一帧红外数据接收完成 */
        {
            key_val = IrDa_Process();
            printf("\r\n key_val=%d \r\n", key_val);
            printf("\r\n 按键次数frame_cnt=%d \r\n", frame_cnt);
            printf("\r\n 中断次数isr_cnt=%d \r\n", isr_cnt);

            /* 不同的遥控器面板对应不同的键值，需要实际测量 */
            switch (key_val) {
                case 0: LED1_TOGGLE;
                    printf("\r\n key_val=%d \r\n", key_val);
                    printf("\r\n Error \r\n");
                    break;

                case 162: // POWER
                LED1_TOGGLE;
                    printf("\r\n key_val=%d \r\n", key_val);
                    printf("\r\n POWER \r\n");
                    break;

                case 226: // MENU
                LED1_TOGGLE;
                    printf("\r\n key_val=%d \r\n", key_val);
                    printf("\r\n MENU \r\n");
                    break;

                case 34: // TEST
                LED1_TOGGLE;
                    printf("\r\n key_val=%d \r\n", key_val);
                    printf("\r\n TEST \r\n");
                    break;

                case 2: // +
                LED1_TOGGLE;
                    printf("\r\n key_val=%d \r\n", key_val);
                    printf("\r\n + \r\n");
                    break;

                case 194: // RETURN
                LED1_TOGGLE;
                    printf("\r\n key_val=%d \r\n", key_val);
                    printf("\r\n RETURN \r\n");
                    break;

                case 224: // |<<
                LED1_TOGGLE;
                    printf("\r\n key_val=%d \r\n", key_val);
                    printf("\r\n |<< \r\n");
                    break;

                case 168: // >
                LED1_TOGGLE;
                    printf("\r\n key_val=%d \r\n", key_val);
                    printf("\r\n > \r\n");
                    break;

                case 144: // >>|
                LED1_TOGGLE;
                    printf("\r\n key_val=%d \r\n", key_val);
                    printf("\r\n >>| \r\n");
                    break;

                case 104: // 0
                LED1_TOGGLE;
                    printf("\r\n key_val=%d \r\n", key_val);
                    printf("\r\n 0 \r\n");
                    break;

                case 152: // -
                LED1_TOGGLE;
                    printf("\r\n key_val=%d \r\n", key_val);
                    printf("\r\n - \r\n");
                    break;

                case 176: // C
                LED1_TOGGLE;
                    printf("\r\n key_val=%d \r\n", key_val);
                    printf("\r\n C \r\n");
                    break;

                case 48: // 1
                LED1_TOGGLE;
                    printf("\r\n key_val=%d \r\n", key_val);
                    printf("\r\n 1 \r\n");
                    break;

                case 24: // 2
                LED1_TOGGLE;
                    printf("\r\n key_val=%d \r\n", key_val);
                    printf("\r\n 2 \r\n");
                    break;

                case 122: // 3
                LED1_TOGGLE;
                    printf("\r\n key_val=%d \r\n", key_val);
                    printf("\r\n 3 \r\n");
                    break;

                case 16: // 4
                LED1_TOGGLE;
                    printf("\r\n key_val=%d \r\n", key_val);
                    printf("\r\n 4 \r\n");
                    break;

                case 56: // 5
                LED1_TOGGLE;
                    printf("\r\n key_val=%d \r\n", key_val);
                    printf("\r\n 5 \r\n");
                    break;

                case 90: // 6
                LED1_TOGGLE;
                    printf("\r\n key_val=%d \r\n", key_val);
                    printf("\r\n 6 \r\n");
                    break;

                case 66: // 7
                LED1_TOGGLE;
                    printf("\r\n key_val=%d \r\n", key_val);
                    printf("\r\n 7 \r\n");
                    break;

                case 74: // 8
                LED1_TOGGLE;
                    printf("\r\n key_val=%d \r\n", key_val);
                    printf("\r\n 8 \r\n");
                    break;

                case 82: // 9
                LED1_TOGGLE;
                    printf("\r\n key_val=%d \r\n", key_val);
                    printf("\r\n 9 \r\n");
                    break;

                default:
                    break;
            }
        }
    }
#endif
}

/**
 * @brief Delay function
 * @param nCount
 * @retval none
 */
void Delay(__IO uint32_t nCount) {
    for (; nCount != 0; nCount--);
}

#ifdef Exp_3
/**
  * @brief  Print information
  * @param  none
  * @retval none
  */
static void Show_Message(void) {
    printf("\r\n   这是一个通过串口通信指令控制RGB彩灯实验 \n");
    printf("使用  USART1  参数为：%d 8-N-1 \n", USARTx_BAUDRATE);
    printf("开发板接到指令后控制RGB彩灯颜色，指令对应如下：\n");
    printf("   指令    ------ 彩灯颜色 \n");
    printf("     1    ------    红 \n");
    printf("     2    ------    绿 \n");
    printf("     3    ------    蓝 \n");
    printf("     4    ------    黄 \n");
    printf("     5    ------    紫 \n");
    printf("     6    ------    青 \n");
    printf("     7    ------    白 \n");
    printf("     8    ------    灭 \n");
}
#endif