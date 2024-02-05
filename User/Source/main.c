#include "stm32f4xx.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "retarget.h"
#include "bsp_usart.h"
#include "bsp_systick.h"
#include "bsp_beep.h"

// #define Exp_1 // ʵ��һ ����LED
// #define Exp_2 // ʵ��� �������
// #define Exp_3 // ʵ���� ����ͨ��
// #define Exp_4 // ʵ���� ������

void Delay(__IO u32 nCount);

static void Show_Message(void);

int main(void) {
    /*ʵ��һ ����LED*/
#ifdef Exp_1
    LED_GPIO_Config(); // LED�˿ڳ�ʼ��
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
        /* ������ʾ������������� */
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

    /*ʵ��� �������*/
#ifdef Exp_2
    LED_GPIO_Config(); // LED�˿ڳ�ʼ��
    Key_GPIO_Config(); // ������ʼ��
    while(1) {
        if (Key_Scan(KEY1_GPIO_PORT, KEY1_PIN) == KEY_ON) {
            LED1_TOGGLE; // LED1��ת
        }
        if (Key_Scan(KEY2_GPIO_PORT, KEY2_PIN) == KEY_ON) {
            LED2_TOGGLE; // LED2��ת
        }
    }
#endif

    /*ʵ���� ����ͨ��*/
#ifdef Exp_3
    LED_GPIO_Config(); // LED�˿ڳ�ʼ��
    USARTx_Config(); // ��ʼ�� USART ����ģʽΪ 115200 8-N-1
    RetargetInit(USART1); // �����ض���
    Show_Message(); // ��ʾ��Ϣ
    char ch;
    while(1) {
        ch = getchar(); // ��ȡ�ַ�ָ��
        printf("���յ��ַ���%c\n", ch);
        /* �����ַ�ָ�����RGB�ʵ���ɫ */
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
                /* �������ָ��ָ���ַ�����ӡ��ʾ��Ϣ */
                Show_Message();
                break;
        }
    }
#endif

    /*ʵ���� ������*/
#ifdef Exp_4
    BEEP_GPIO_Config(); // �������˿ڳ�ʼ��
    while(1) {
        BEEP_ON;
        Delay(0xFFFFFF);
        BEEP_OFF;
        Delay(0xFFFFFF);
    }
#endif


}

/**
 * @brief  ��ʱ����
 * @param nCount
 */
void Delay(__IO uint32_t nCount) {
    for (; nCount != 0; nCount--);
}

/**
  * @brief  ��ӡָ��������ʾ��Ϣ
  * @param  none
  * @retval none
  */
static void Show_Message(void) {
    printf("\r\n   ����һ��ͨ������ͨ��ָ�����RGB�ʵ�ʵ�� \n");
    printf("ʹ��  USART1  ����Ϊ��%d 8-N-1 \n", USARTx_BAUDRATE);
    printf("������ӵ�ָ������RGB�ʵ���ɫ��ָ���Ӧ���£�\n");
    printf("   ָ��   ------ �ʵ���ɫ \n");
    printf("     1    ------    �� \n");
    printf("     2    ------    �� \n");
    printf("     3    ------    �� \n");
    printf("     4    ------    �� \n");
    printf("     5    ------    �� \n");
    printf("     6    ------    �� \n");
    printf("     7    ------    �� \n");
    printf("     8    ------    �� \n");
}