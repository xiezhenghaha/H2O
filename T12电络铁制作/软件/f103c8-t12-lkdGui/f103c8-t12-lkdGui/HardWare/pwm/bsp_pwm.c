/**
 ******************************************************************************
 * @file    bsp_pwm.c
 * @author  Zxp
 * @version V1.0.1
 * @date    2021-06-02
 * @brief   设置PWM输出
 ******************************************************************************
 */

#include "includes.h"

/**
 * @brief   设置加热控制PWM输出
 * @param   duty: 初始占空比.数值是正常占空比的一百倍,如5000表示50.00%的占空比
 * @return  None
 */
void bsp_InitHeatPwm(int32_t duty)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    uint16_t usPeriod;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    duty = BSP_LimitInt(duty, 0, 10000);

    /* 配置GPIO */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; /* 复用功能 */
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* 频率为72000000/(9999+1)/(7+1) = 900Hz */
    usPeriod = 9999;
    TIM_TimeBaseStructure.TIM_Period = usPeriod;
    TIM_TimeBaseStructure.TIM_Prescaler = 7;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_OCStructInit(&TIM_OCInitStructure); /* 初始化结构体成员 */

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = usPeriod * (duty / 10000.0);
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable; /* only for TIM1 and TIM8. */
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;      /* only for TIM1 and TIM8. */
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;     /* only for TIM1 and TIM8. */
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;   /* only for TIM1 and TIM8. */

    TIM_OC3Init(TIM3, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

    // TIM_ARRPreloadConfig(TIM3, ENABLE);

    TIM_Cmd(TIM3, ENABLE);
}

/**
 * @brief   设置加热的PWM的占空比
 * @param   duty: 要输出的占空比.数值是正常占空比的一百倍,如5000表示50.00%的占空比
 * @return  None
 */
void BSP_SetHeatDuty(int32_t duty)
{
    duty = BSP_LimitInt(duty, 0, 10000);
    TIM3->CCR3 = TIM3->ARR * (duty / 10000.0);
}

/**
 * @brief   关闭加热PWM输出,同时置低对应的引脚以彻底关闭输出
 * @param   None
 * @return  None
 */
void BSP_SetHeatOff(void)
{
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Disable);
    GPIO_ResetBits(GPIOB, GPIO_Pin_0);
}

/**
 * @brief   开启加热PWM输出
 * @param   None
 * @return  None
 */
void BSP_SetHeatOn(void)
{
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
}
