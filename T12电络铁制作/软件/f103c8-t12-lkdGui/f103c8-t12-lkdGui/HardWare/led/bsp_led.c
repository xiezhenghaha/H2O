/**
 ******************************************************************************
 * @file    bsp_led.c
 * @author  Zxp
 * @version V1.1.0
 * @date    2021-02-07
 * @brief   LED����
 * @History Version     Date        Author      Note
 *          1.0.0       2021-02-07  ZXP         ��ʼ�汾
 *          1.1.0       2021-06-07  ZXP         �޸ĺ�����,�Է��������淶
 ******************************************************************************
 */

#include "bsp_led.h"

/**
 * @brief  ��ʼ������LED��IO
 * @param  ��
 * @retval ��
 */
void BSP_InitLed(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(LED1_GPIO_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);

    BSP_LedSetOff();
}

void BSP_LedSetOn(void)
{
    GPIO_ResetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
}

void BSP_LedSetOff(void)
{
    GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
}

void BSP_LedToggle(void)
{
    LED1_GPIO_PORT->ODR ^= LED1_GPIO_PIN;
}

/*********************************************END OF FILE**********************/
