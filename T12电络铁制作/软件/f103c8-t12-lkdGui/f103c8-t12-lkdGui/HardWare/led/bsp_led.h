/**
 ******************************************************************************
 * @file    bsp_led.h
 * @author  Zxp
 * @version V1.1.0
 * @date    2021-02-07
 * @brief   LED驱动头文件
 * @History Version     Date        Author      Note
 *          1.0.0       2021-02-07  ZXP         初始版本
 *          1.1.0       2021-06-07  ZXP         修改函数名,以符合命名规范
 ******************************************************************************
 */

#ifndef __LED_H
#define __LED_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32f10x.h"

/* 定义LED连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的LED引脚 */
#define LED1_GPIO_PORT GPIOB               /* GPIO端口 */
#define LED1_GPIO_CLK RCC_APB2Periph_GPIOB /* GPIO端口时钟 */
#define LED1_GPIO_PIN GPIO_Pin_1

    void BSP_InitLed(void);
    void BSP_LedSetOn(void);
    void BSP_LedSetOff(void);
    void BSP_LedToggle(void);

#ifdef __cplusplus
}
#endif

#endif /* __LED_H */
