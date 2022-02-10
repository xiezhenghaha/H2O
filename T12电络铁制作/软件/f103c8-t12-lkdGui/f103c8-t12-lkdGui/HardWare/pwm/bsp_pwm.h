/**
 ******************************************************************************
 * @file    bsp_pwm.h
 * @author  Zxp
 * @version V1.0.1
 * @date    2021-06-02
 * @brief   PWM输出驱动头文件
 ******************************************************************************
*/
#ifndef __BSP_PWM_H_
#define __BSP_PWM_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32f10x.h"

    void bsp_InitHeatPwm(int32_t duty);
    void BSP_SetHeatDuty(int32_t duty);
    void BSP_SetHeatOff(void);
    void BSP_SetHeatOn(void);

#ifdef __cplusplus
}
#endif

#endif  // #ifndef __BSP_PWM_H_
