#ifndef __ADC_H
#define __ADC_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32f10x.h"

#define NTC_VIN_ADCx ADC1
#define NTC_VIN_ADC_CLK RCC_APB2Periph_ADC1
#define NTC_VIN_ADC_DMA_CHANNEL DMA1_Channel1
#define NTC_VIN_ADC_DMA_CLK RCC_AHBPeriph_DMA1
#define T12_TEMPER_ADCx ADC2
#define T12_TEMPER_ADC_CLK RCC_APB2Periph_ADC2

// ADC GPIO宏定义
// 注意：用作ADC采集的IO必须没有复用，否则采集电压会有影响
#define T12_TEMPER_ADC_GPIO_CLK RCC_APB2Periph_GPIOA
#define T12_TEMPER_ADC_PORT GPIOA
#define T12_TEMPER_ADC_PIN GPIO_Pin_1
#define NTC_ADC_PIN GPIO_Pin_2
#define VIN_ADC_PIN GPIO_Pin_3
// ADC 通道宏定义
#define T12_TEMPER_ADC_CHANNEL ADC_Channel_1
#define NTC_ADC_CHANNEL ADC_Channel_2
#define VIN_ADC_CHANNEL ADC_Channel_3

// ADC 中断相关宏定义
#define T12_TEMPER_ADC_IRQ ADC1_2_IRQn
#define T12_TEMPER_ADC_IRQHandler ADC1_2_IRQHandler

#define ADC_BUFF_SIZE 30
    extern __IO uint16_t ADC_ConvertedValue[];

    void BSP_ADCx_Init(void);
    uint16_t BSP_ADCx_Read(uint16_t channel);

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H */
