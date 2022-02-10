/**
 ******************************************************************************
 * @file    bsp_led.h
 * @author  Zxp
 * @version V1.1.0
 * @date    2021-02-07
 * @brief   LED����ͷ�ļ�
 * @History Version     Date        Author      Note
 *          1.0.0       2021-02-07  ZXP         ��ʼ�汾
 *          1.1.0       2021-06-07  ZXP         �޸ĺ�����,�Է��������淶
 ******************************************************************************
 */

#ifndef __LED_H
#define __LED_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32f10x.h"

/* ����LED���ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����Ĵ��뼴�ɸı���Ƶ�LED���� */
#define LED1_GPIO_PORT GPIOB               /* GPIO�˿� */
#define LED1_GPIO_CLK RCC_APB2Periph_GPIOB /* GPIO�˿�ʱ�� */
#define LED1_GPIO_PIN GPIO_Pin_1

    void BSP_InitLed(void);
    void BSP_LedSetOn(void);
    void BSP_LedSetOff(void);
    void BSP_LedToggle(void);

#ifdef __cplusplus
}
#endif

#endif /* __LED_H */
