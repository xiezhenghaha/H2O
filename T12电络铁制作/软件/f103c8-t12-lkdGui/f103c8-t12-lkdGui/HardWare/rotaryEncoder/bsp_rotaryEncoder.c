/**
 ******************************************************************************
 * @file    bsp_rotaryEncoder.c
 * @author  Zxp
 * @version V1.0.0
 * @date    2021-06-10
 * @brief   旋转编码器驱动
 ******************************************************************************
 */

#include "bsp_rotaryEncoder.h"

void BSP_InitRotaryEncoder(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  //外部中断，需要使能AFIO时钟

    RCC_APB2PeriphClockCmd(CLK_CALU(RoyEcd_PORT) | CLK_CALU(RoyEcd_SW_PORT), ENABLE);

    GPIO_InitStruct.GPIO_Pin = RoyEcd_CLK_PIN | RoyEcd_DAT_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(RoyEcd_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = RoyEcd_SW_PIN;
    GPIO_Init(RoyEcd_SW_PORT, &GPIO_InitStruct);

    //GPIOB0 中断线以及中断初始化配置
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource13);
    EXTI_InitStructure.EXTI_Line = EXTI_Line13;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //上升沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);  //根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;          //使能按键所在的外部中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;  //抢占优先级2，
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;         //子优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);
}

unsigned char BSP_ReadRotaryEncoder(void)
{
    static char Enc0 = 0, Enc1 = 0;
    static unsigned char EncOld, EncX = 0;
    unsigned char EncNow;
    //	char EncType=0; //设置编码器类型: 0为一定位一脉冲,1为两定位一脉冲

    if (Enc0 == 0)
        EncOld = (Read_RoyEcd_CLK() ? 0x02 : 0x00) + (Read_RoyEcd_DAT() ? 0x01 : 0x00), Enc0 = 1;  //记住初次调用时编码器的状态
    EncNow = (Read_RoyEcd_CLK() ? 0x02 : 0x00) + (Read_RoyEcd_DAT() ? 0x01 : 0x00);                //根据两个IO当前状态组合成16进制的0x00|0x01|0x02|0x03
    if (EncNow == EncOld)
        return (0);  //如果新数据和原来的数据一样(没有转动)就直接返回0

    if (EncOld == 0x00 && EncNow == 0x02 || EncOld == 0x03 && EncNow == 0x01)
        EncX = EncNow;                                                                                         //00-10|11-01
    if (EncOld == 0x00 && EncX == 0x02 && EncNow == 0x03 || EncOld == 0x03 && EncX == 0x01 && EncNow == 0x00)  //00-10-11|11-01-00右转
    {
        EncOld = EncNow, EncX = 0;
        if (EncType == 1)
            return ('R');  //两定位一脉冲
        else               //一定位一脉冲
        {
            if (Enc1 == 0)
                Enc1 = 1;
            else
            {
                //Delayms(60);       //延时降低旋转灵敏度(不能用在定时器中)
                Enc1 = 0;
                return ('R');
            }
        }
    }

    if (EncOld == 0x00 && EncNow == 0x01 || EncOld == 0x03 && EncNow == 0x02)
        EncX = EncNow;                                                                                         //00-01|11-10
    if (EncOld == 0x00 && EncX == 0x01 && EncNow == 0x03 || EncOld == 0x03 && EncX == 0x02 && EncNow == 0x00)  //00-01-11|11-10-00左转
    {
        EncOld = EncNow, EncX = 0;
        if (EncType == 1)
            return ('L');  //两定位一脉冲
        else               //一定位一脉冲
        {
            if (Enc1 == 0)
                Enc1 = 1;
            else
            {
                //Delayms(60);       //延时降低旋转灵敏度(不能用在定时器中)
                Enc1 = 0;
                return ('L');
            }
        }
    }
    return (0);  //没有正确解码返回0
}

/**
* 旋转编码器按键检测函数
*/
uint8_t BSP_ReadRotaryEncoder_SW(void)
{
    uint8_t retVal;

    retVal = (GPIO_ReadInputDataBit(RoyEcd_SW_PORT, RoyEcd_SW_PIN) == Bit_SET) ? true : false;
    return retVal;
}
