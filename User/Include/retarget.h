//
// Created by Lyu on 2024/2/4.
//

#ifndef __RETARGET_H__
#define __RETARGET_H__

#include <stdio.h>
#include "stm32f4xx_usart.h"

#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

#ifdef __GNUC__
#define GETCHAR_PROTOTYPE int __io_getchar (void)
#else
#define GETCHAR_PROTOTYPE int fgetc(FILE * f)
#endif /* __GNUC__ */

/**
 * @brief 注册重定向串口
 * @param usartx 需要重定向输入输出的串口
 */
void RetargetInit(USART_TypeDef *huart);

#endif //__RETARGET_H__
