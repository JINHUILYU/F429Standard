#include "stm32f4xx.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "retarget.h"
#include "bsp_usart.h"
#include "bsp_systick.h"
#include "bsp_beep.h"

// #define Exp_1 // 实验一 点亮LED
// #define Exp_2 // 实验二 按键检测
// #define Exp_3 // 实验三 串口通信
// #define Exp_4 // 实验四 蜂鸣器

void Delay(__IO u32 nCount);

static void Show_Message(void);

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

    /*实验四 蜂鸣器*/
#ifdef Exp_4
    BEEP_GPIO_Config(); // 蜂鸣器端口初始化
    while(1) {
        BEEP_ON;
        Delay(0xFFFFFF);
        BEEP_OFF;
        Delay(0xFFFFFF);
    }
#endif


}

/**
 * @brief  延时函数
 * @param nCount
 */
void Delay(__IO uint32_t nCount) {
    for (; nCount != 0; nCount--);
}

/**
  * @brief  打印指令输入提示信息
  * @param  none
  * @retval none
  */
static void Show_Message(void) {
    printf("\r\n   这是一个通过串口通信指令控制RGB彩灯实验 \n");
    printf("使用  USART1  参数为：%d 8-N-1 \n", USARTx_BAUDRATE);
    printf("开发板接到指令后控制RGB彩灯颜色，指令对应如下：\n");
    printf("   指令   ------ 彩灯颜色 \n");
    printf("     1    ------    红 \n");
    printf("     2    ------    绿 \n");
    printf("     3    ------    蓝 \n");
    printf("     4    ------    黄 \n");
    printf("     5    ------    紫 \n");
    printf("     6    ------    青 \n");
    printf("     7    ------    白 \n");
    printf("     8    ------    灭 \n");
}