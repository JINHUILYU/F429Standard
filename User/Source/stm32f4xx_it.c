/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    06-March-2015
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "bsp_led.h"
#include "bsp_exti.h"
#include "bsp_breath_led.h"
#include "bsp_basic_tim.h"
#include "bsp_irda.h"

extern uint32_t frame_data;
extern uint8_t  frame_cnt;
extern uint8_t  frame_flag;

uint8_t isr_cnt;  /* 用于计算进了多少次中断 */

extern void TimingDelay_Decrement(void);

/** @addtogroup STM32F429I_DISCOVERY_Examples
  * @{
  */

/** @addtogroup FMC_SDRAM
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void) {
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void) {
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1) {}
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void) {
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1) {}
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void) {
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1) {}
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void) {
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1) {}
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void) {}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void) {}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void) {}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void) {
    TimingDelay_Decrement();
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f429_439xx.s).                         */
/******************************************************************************/

/**
  * @}
  */

/**
  * @}
  */
/*
 * @brief  外部中断0服务函数，对应按键KEY1，当KEY1按下时，LED1翻转(RED)
 * @param  none
 * @retval none
 */
void KEY1_IRQHandler(void) {
    //确保是否产生了EXTI Line中断
    if (EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET) {
        // LED1 取反
        LED1_TOGGLE;
        //清除中断标志位
        EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);
    }
}

/*
 * @brief  外部中断15_10服务函数，对应按键KEY2，当KEY2按下时，LED2翻转(GREEN)
 * @param  none
 * @retval none
 */
void KEY2_IRQHandler(void) {
    //确保是否产生了EXTI Line中断
    if (EXTI_GetITStatus(KEY2_INT_EXTI_LINE) != RESET) {
        // LED2 取反
        LED2_TOGGLE;
        //清除中断标志位
        EXTI_ClearITPendingBit(KEY2_INT_EXTI_LINE);
    }
}

/**
  * @brief  This function handles TIM interrupt request.
  * @param  None
  * @retval None
  */
extern uint16_t indexWave[];

extern __IO uint32_t rgb_color;

void  BRE_TIM_IRQHandler(void) {
    static uint16_t pwm_index = 0;            //用于PWM查表
    static uint16_t period_cnt = 0;        //用于计算周期数
    static uint16_t amplitude_cnt = 0;    //用于计算幅值等级

    if (TIM_GetITStatus(BRE_TIM, TIM_IT_Update) != RESET)    //TIM_IT_Update
    {
        amplitude_cnt++;

        //每个PWM表中的每个元素有AMPLITUDE_CLASS个等级，
        //每增加一级多输出一次脉冲，即PWM表中的元素多使用一次
        //使用256次，根据RGB颜色分量设置通道输出
        if (amplitude_cnt > (AMPLITUDE_CLASS - 1)) {
            period_cnt++;

            //每个PWM表中的每个元素使用period_class次
            if (period_cnt > period_class) {

                pwm_index++;                                                //标志PWM表指向下一个元素

                //若PWM表已到达结尾，重新指向表头
                if (pwm_index >= POINT_NUM) {
                    pwm_index = 0;
                }

                period_cnt = 0;                                            //重置周期计数标志
            }

            amplitude_cnt = 0;                                            //重置幅值计数标志
        } else {
            //每个PWM表中的每个元素有AMPLITUDE_CLASS个等级，
            //每增加一级多输出一次脉冲，即PWM表中的元素多使用一次

            //根据RGB颜色分量值，设置各个通道是否输出当前的PWM表元素表示的亮度
            //红
            if (((rgb_color & 0xFF0000) >> 16) >= amplitude_cnt)
                BRE_TIM->BRE_RED_CCRx = indexWave[pwm_index];    //根据PWM表修改定时器的比较寄存器值
            else
                BRE_TIM->BRE_RED_CCRx = 0;        //比较寄存器值为0，通道输出高电平，该通道LED灯灭

            //绿
            if (((rgb_color & 0x00FF00) >> 8) >= amplitude_cnt)
                BRE_TIM->BRE_GREEN_CCRx = indexWave[pwm_index];    //根据PWM表修改定时器的比较寄存器值
            else
                BRE_TIM->BRE_GREEN_CCRx = 0;    //比较寄存器值为0，通道输出高电平，该通道LED灯灭

            //蓝
            if ((rgb_color & 0x0000FF) >= amplitude_cnt)
                BRE_TIM->BRE_BLUE_CCRx = indexWave[pwm_index];    //根据PWM表修改定时器的比较寄存器值
            else
                BRE_TIM->BRE_BLUE_CCRx = 0;        //比较寄存器值为0，通道输出高电平，该通道LED灯灭

        }

        TIM_ClearITPendingBit(BRE_TIM, TIM_IT_Update);    //必须要清除中断标志位
    }
}

/**
 * @brief  This function handles TIM interrupt request.
 * @param  None
 * @retval None
 */
void BASIC_TIM_IRQHandler(void) {
    if (TIM_GetITStatus(BASIC_TIM, TIM_IT_Update) != RESET) {
        LED1_TOGGLE; // LED1翻转
        TIM_ClearITPendingBit(BASIC_TIM, TIM_IT_Update); // 清除中断标志位
    }
}

// IO 线中断, 接红外接收头的数据管脚
void IRDA_EXTI_IRQHANDLER_FUN(void)
{
    uint8_t pulse_time = 0;
    uint8_t leader_code_flag = 0; /* 引导码标志位，当引导码出现时，表示一帧数据开始 */
    uint8_t irda_data = 0;        /* 数据暂存位 */

    if(EXTI_GetITStatus(IRDA_EXTI_LINE) != RESET) /* 确保是否产生了EXTI Line中断 */
    {
        while(1)
        {
            if( IrDa_DATA_IN()== SET )        /* 只测量高电平的时间 */
            {
                pulse_time = Get_Pulse_Time();

                /* >=5ms 不是有用信号 当出现干扰或者连发码时，也会break跳出while(1)循环 */
                if( pulse_time >= 250 )
                {
                    break; /* 跳出while(1)循环 */
                }

                if(pulse_time>=200 && pulse_time<250)         /* 获得前导位 4ms~4.5ms */
                {
                    leader_code_flag = 1;
                }
                else if(pulse_time>=10 && pulse_time<50)      /* 0.56ms: 0.2ms~1ms */
                {
                    irda_data = 0;
                }
                else if(pulse_time>=50 && pulse_time<100)     /* 1.68ms：1ms~2ms */
                {
                    irda_data =1 ;
                }
                else if( pulse_time>=100 && pulse_time<=200 ) /* 2.1ms：2ms~4ms */
                {/* 连发码，在第二次中断出现 */
                    frame_flag = 1;               /* 一帧数据接收完成 */
                    frame_cnt++;                  /* 按键次数加1 */
                    isr_cnt ++;                   /* 进中断一次加1 */
                    break;                        /* 跳出while(1)循环 */
                }

                if( leader_code_flag == 1 )
                {/* 在第一次中断中完成 */
                    frame_data <<= 1;
                    frame_data += irda_data;
                    frame_cnt = 0;
                    isr_cnt = 1;
                }
            }
        }// while(1)
        EXTI_ClearITPendingBit(IRDA_EXTI_LINE);     //清除中断标志位
        //LED2_TOGGLE;
    }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
