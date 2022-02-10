/**
 ******************************************************************************
 * @file    bsp_rotaryEncoder.h
 * @author  Zxp
 * @version V1.0.0
 * @date    2021-06-10
 * @brief   旋转编码器驱动
 ******************************************************************************
*/
#ifndef _BSP_ROTARYENCODER_H_
#define _BSP_ROTARYENCODER_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "includes.h"

#define EncType 1  //设置编码器类型: 0为一定位一脉冲,1为两定位一脉冲

#define RoyEcd_PORT GPIOB
#define RoyEcd_CLK_PIN GPIO_Pin_13
#define RoyEcd_DAT_PIN GPIO_Pin_14
#define RoyEcd_SW_PORT GPIOB
#define RoyEcd_SW_PIN GPIO_Pin_12

#define Read_RoyEcd_CLK() (GPIO_ReadInputDataBit(RoyEcd_PORT, RoyEcd_CLK_PIN) == Bit_RESET)
#define Read_RoyEcd_DAT() (GPIO_ReadInputDataBit(RoyEcd_PORT, RoyEcd_DAT_PIN) == Bit_RESET)
    //#define Read_RoyEcd_SW()				(GPIO_ReadInputDataBit(RoyEcd_SW_PORT,RoyEcd_SW_PIN) == Bit_RESET)

#define RoyEcd_SW_True Bit_RESET
#define RoyEcd_SW_False (!RoyEcd_SW_True)

#define ROTARY_LEFT 'L'
#define ROTARY_RIGHT 'R'

    void BSP_InitRotaryEncoder(void);
    unsigned char BSP_ReadRotaryEncoder(void);
    uint8_t BSP_ReadRotaryEncoder_SW(void);

#ifdef __cplusplus
}
#endif

#endif  //_BSP_ROTARYENCODER_H_
