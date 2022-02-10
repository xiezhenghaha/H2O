/**
 ******************************************************************************
 * @file    bsp_key.c
 * @author  Zxp
 * @version V1.0.0
 * @date    2021-06-29
 * @brief   �ֱ��𶯿�������
 ******************************************************************************
 */

#include "bsp.h"

void BSP_InitKey(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  //�ⲿ�жϣ���Ҫʹ��AFIOʱ��
    RCC_APB2PeriphClockCmd(HANDLE_SW_RCC_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; /* ���븡��ģʽ */
    GPIO_InitStructure.GPIO_Pin = HANDLE_SW_GPIO_PIN;
    GPIO_Init(HANDLE_SW_GPIO_PORT, &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //�����ش���
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);  //����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;              //ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0f;  //��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;         //�����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure);
}
