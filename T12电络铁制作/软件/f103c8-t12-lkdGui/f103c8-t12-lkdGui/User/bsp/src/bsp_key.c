/**
 ******************************************************************************
 * @file    bsp_key.c
 * @author  Zxp
 * @version V1.0.0
 * @date    2021-06-29
 * @brief   手柄震动开关驱动
 ******************************************************************************
 */

#include "bsp.h"

void BSP_InitKey(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  //外部中断，需要使能AFIO时钟
    RCC_APB2PeriphClockCmd(HANDLE_SW_RCC_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; /* 输入浮空模式 */
    GPIO_InitStructure.GPIO_Pin = HANDLE_SW_GPIO_PIN;
    GPIO_Init(HANDLE_SW_GPIO_PORT, &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //上升沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);  //根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;              //使能按键所在的外部中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0f;  //抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;         //子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);
}
