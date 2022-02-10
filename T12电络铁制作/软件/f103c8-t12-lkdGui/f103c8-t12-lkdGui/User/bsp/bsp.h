/**
 ******************************************************************************
 * @file    bsp.h
 * @author  Zxp
 * @version V1.0.0
 * @date    2021-06-10
 * @brief   底层驱动模块所有的h文件的汇总文件。
 ******************************************************************************
 */

#ifndef _BSP_H_
#define _BSP_H

#define STM32_V4
//#define STM32_X2

/* 检查是否定义了开发板型号 */
#if !defined(STM32_V4) && !defined(STM32_X2)
#error "Please define the board model : STM32_X2 or STM32_V4"
#endif

/* 定义 BSP 版本号 */
#define __STM32F1_BSP_VERSION "1.1"

/* CPU空闲时执行的函数 */
//#define CPU_IDLE()		bsp_Idle()

#define USE_FreeRTOS 1

#if USE_FreeRTOS == 1
#include "FreeRTOS.h"
#include "task.h"
#define DISABLE_INT() taskENTER_CRITICAL()
#define ENABLE_INT() taskEXIT_CRITICAL()
#else
/* 开关全局中断的宏 */
#define ENABLE_INT() __set_PRIMASK(0)  /* 使能全局中断 */
#define DISABLE_INT() __set_PRIMASK(1) /* 禁止全局中断 */
#endif

/* 这个宏仅用于调试阶段排错 */
#define BSP_Printf printf
//#define BSP_Printf(...)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stm32f10x.h"

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/*
    EXTI9_5_IRQHandler 的中断服务程序分散在几个独立的 bsp文件中。
    需要整合到 stm32f4xx_it.c 中。

    定义下面行表示EXTI9_5_IRQHandler入口函数集中放到 stm32f4xx_it.c。
*/
#define EXTI9_5_ISR_MOVE_OUT

#define DEBUG_GPS_TO_COM1 /* 打印GPS数据到串口1 */

/* 通过取消注释或者添加注释的方式控制是否包含底层驱动模块 */
//#include "BSP_Config.h"
#include "UICode.h"
#include "bsp_adc.h"
#include "bsp_beep.h"
#include "bsp_dwt.h"
#include "bsp_kalman.h"
#include "bsp_key.h"
#include "bsp_led.h"
#include "bsp_multi_button.h"
#include "bsp_pid.h"
#include "bsp_pwm.h"
#include "bsp_rotaryEncoder.h"
#include "bsp_rwflash.h"
#include "bsp_ssdoled.h"
#include "bsp_tim_pwm.h"
#include "bsp_timer.h"
#include "bsp_uart_fifo.h"
#include "bsp_usart.h"
#include "lkdGui.h"
#include "main.h"

#define CLK_CALU(x) ((uint32_t)(0x01 << ((((uint32_t)(x)&0xff00) >> 8) / 4)))
#ifndef MIN
#define MIN(x, y) ((((int32_t)(x)) < ((int32_t)(y))) ? (x) : (y))
#endif  //min
#ifndef MAX
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#endif  //max
#ifndef ABS
#define ABS(X) ((X) > 0 ? (X) : -(X))
#endif

/* 提供给其他C文件调用的函数 */
void bsp_Init(void);
void bsp_Idle(void);
void BSP_Tick_Init(void);
float BSP_LimitFloat(float val, float min, float max);
int32_t BSP_LimitInt(int32_t val, int32_t min, int32_t max);
float map(float value, float fromLow, float fromHigh, float toLow, float toHigh);
void BSP_SaveParameters(void);
void BSP_SaveDataLoop(void);

#endif

/***************************** END OF FILE **********************************/
