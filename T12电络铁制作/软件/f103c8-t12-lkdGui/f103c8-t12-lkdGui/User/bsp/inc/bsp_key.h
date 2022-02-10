/**
 ******************************************************************************
 * @file    bsp_key.h
 * @author  Zxp
 * @version V1.0.0
 * @date    2021-06-29
 * @brief   �ֱ��𶯿�������
 ******************************************************************************
*/

#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#define HANDLE_SW_RCC_CLK RCC_APB2Periph_GPIOA
#define HANDLE_SW_GPIO_PORT GPIOA
#define HANDLE_SW_GPIO_PIN GPIO_Pin_0

void BSP_InitKey(void);

#endif
