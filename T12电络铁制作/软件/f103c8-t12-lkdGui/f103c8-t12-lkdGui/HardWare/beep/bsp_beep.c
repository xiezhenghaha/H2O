/**
 ******************************************************************************
 * @file    bsp_beep.c
 * @author  Zxp
 * @version V1.0.0
 * @date    2021-06-02
 * @brief   蜂鸣器驱动  
 * @History Version     Date        Author      Note
 *          1.0.0       2021-06-02  ZXP         初始版本
 *          1.1.0       2021-06-07  ZXP         修改函数名,以符合命名规范
 *          1.2.0       2021-07-04  ZXP         添加调节音量功能
 ******************************************************************************
 */

#include "bsp_beep.h"

#define BEEP_TIMx TIM4
#define BEEP_GPIO_PORT GPIOB               /* GPIO端口 */
#define BEEP_GPIO_CLK RCC_APB2Periph_GPIOB /* GPIO端口时钟 */
#define BEEP_GPIO_PIN GPIO_Pin_6

/**
 * @brief  初始化控制BEEP的IO
 * @param  无
 * @retval 无
 */
 void BSP_BeepSetDuty(int32_t duty);
void BSP_InitBeep(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    uint16_t usPeriod;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB2PeriphClockCmd(BEEP_GPIO_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin = BEEP_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BEEP_GPIO_PORT, &GPIO_InitStructure);

    /* 频率为72000000/(9999+1)/(7+1) = 900Hz */
    usPeriod = 9999;
    TIM_TimeBaseStructure.TIM_Period = usPeriod;
    TIM_TimeBaseStructure.TIM_Prescaler = 15;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(BEEP_TIMx, &TIM_TimeBaseStructure);

    TIM_OCStructInit(&TIM_OCInitStructure); /* 初始化结构体成员 */

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable; /* only for TIM1 and TIM8. */
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;      /* only for TIM1 and TIM8. */
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;     /* only for TIM1 and TIM8. */
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;   /* only for TIM1 and TIM8. */

    TIM_OC1Init(BEEP_TIMx, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(BEEP_TIMx, TIM_OCPreload_Enable);

    TIM_Cmd(BEEP_TIMx, ENABLE);

    BSP_BeepSetDuty(0);
}

/**
 * @brief   设置蜂鸣器的PWM的占空比
 * @param   duty: 要输出的占空比.数值是正常占空比的一百倍,如5000表示50.00%的占空比
 * @return  None
 */
void BSP_BeepSetDuty(int32_t duty)
{
    duty = BSP_LimitInt(duty, 0, 100);
    BEEP_TIMx->CCR1 = BEEP_TIMx->ARR * (duty / 100.0);
}

void BSP_BeepSetOn(BeepTypeDef *pBeep, uint32_t ts)
{
    pBeep->state = true;
    pBeep->timeStamp = ts;
    if (pBeep->mute == true)  // 静音
    {
        BSP_BeepSetDuty(0);
    }
    else
    {
        BSP_BeepSetDuty(pBeep->volume);
    }
}

void BSP_BeepSetOff(BeepTypeDef *pBeep, uint32_t ts)
{
    pBeep->state = false;
    BSP_BeepSetDuty(0);
}
