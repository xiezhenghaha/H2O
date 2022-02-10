/**
 ******************************************************************************
 * @file    bsp.h
 * @author  Zxp
 * @version V1.0.0
 * @date    2021-06-10
 * @brief   �ײ�����ģ�����е�h�ļ��Ļ����ļ���
 ******************************************************************************
 */

#ifndef _BSP_H_
#define _BSP_H

#define STM32_V4
//#define STM32_X2

/* ����Ƿ����˿������ͺ� */
#if !defined(STM32_V4) && !defined(STM32_X2)
#error "Please define the board model : STM32_X2 or STM32_V4"
#endif

/* ���� BSP �汾�� */
#define __STM32F1_BSP_VERSION "1.1"

/* CPU����ʱִ�еĺ��� */
//#define CPU_IDLE()		bsp_Idle()

#define USE_FreeRTOS 1

#if USE_FreeRTOS == 1
#include "FreeRTOS.h"
#include "task.h"
#define DISABLE_INT() taskENTER_CRITICAL()
#define ENABLE_INT() taskEXIT_CRITICAL()
#else
/* ����ȫ���жϵĺ� */
#define ENABLE_INT() __set_PRIMASK(0)  /* ʹ��ȫ���ж� */
#define DISABLE_INT() __set_PRIMASK(1) /* ��ֹȫ���ж� */
#endif

/* ���������ڵ��Խ׶��Ŵ� */
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
    EXTI9_5_IRQHandler ���жϷ�������ɢ�ڼ��������� bsp�ļ��С�
    ��Ҫ���ϵ� stm32f4xx_it.c �С�

    ���������б�ʾEXTI9_5_IRQHandler��ں������зŵ� stm32f4xx_it.c��
*/
#define EXTI9_5_ISR_MOVE_OUT

#define DEBUG_GPS_TO_COM1 /* ��ӡGPS���ݵ�����1 */

/* ͨ��ȡ��ע�ͻ������ע�͵ķ�ʽ�����Ƿ�����ײ�����ģ�� */
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

/* �ṩ������C�ļ����õĺ��� */
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
