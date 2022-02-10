#include "bsp_adc.h"

__IO uint16_t ADC_ConvertedValue[ADC_BUFF_SIZE];

/**
 * @brief  ADC GPIO 初始化
 * @param  无
 * @retval 无
 */
static void ADCx_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 打开 ADC IO端口时钟
    RCC_APB2PeriphClockCmd(T12_TEMPER_ADC_GPIO_CLK, ENABLE);

    // 配置 ADC IO 引脚模式
    GPIO_InitStructure.GPIO_Pin = T12_TEMPER_ADC_PIN | NTC_ADC_PIN | VIN_ADC_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(T12_TEMPER_ADC_PORT, &GPIO_InitStructure);
}

/**
 * @brief  配置ADC工作模式
 * @param  无
 * @retval 无
 */
static void ADCx_Mode_Config(void)
{
    ADC_InitTypeDef ADC_InitStructure;

    RCC_APB2PeriphClockCmd(T12_TEMPER_ADC_CLK, ENABLE);

    // ADC 模式配置

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                   // 只使用一个ADC，属于独立模式
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;                        // 禁止扫描模式，多通道才要，单通道不需要
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                   // 连续转换模式
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  // 不用外部触发转换，软件开启即可
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;               // 转换结果右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 1;                              // 转换通道1个
    ADC_Init(T12_TEMPER_ADCx, &ADC_InitStructure);                       // 初始化ADC

    // 配置ADC时钟为PCLK2的8分频，即9MHz
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);

    // 配置 ADC 通道转换顺序和采样时间
    ADC_RegularChannelConfig(T12_TEMPER_ADCx, T12_TEMPER_ADC_CHANNEL, 1, ADC_SampleTime_55Cycles5);

    // ADC 转换结束产生中断，在中断服务程序中读取转换值
    // ADC_ITConfig(T12_TEMPER_ADCx, ADC_IT_EOC, ENABLE);

    // 开启ADC ，并开始转换
    ADC_Cmd(T12_TEMPER_ADCx, ENABLE);

    // 初始化ADC 校准寄存器
    ADC_ResetCalibration(T12_TEMPER_ADCx);
    // 等待校准寄存器初始化完成
    while (ADC_GetResetCalibrationStatus(T12_TEMPER_ADCx))
        ;

    // ADC开始校准
    ADC_StartCalibration(T12_TEMPER_ADCx);
    // 等待校准完成
    while (ADC_GetCalibrationStatus(T12_TEMPER_ADCx))
        ;

    // 由于没有采用外部触发，所以使用软件触发ADC转换
    ADC_SoftwareStartConvCmd(T12_TEMPER_ADCx, ENABLE);
}

/**
 * @brief  配置ADC工作模式
 * @param  无
 * @retval 无
 */
static void NtcVin_Mode_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    // 打开DMA时钟
    RCC_AHBPeriphClockCmd(NTC_VIN_ADC_DMA_CLK, ENABLE);
    // 打开ADC时钟
    RCC_APB2PeriphClockCmd(NTC_VIN_ADC_CLK, ENABLE);

    // 复位DMA控制器
    DMA_DeInit(NTC_VIN_ADC_DMA_CHANNEL);

    // 配置 DMA 初始化结构体
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(NTC_VIN_ADCx->DR));       // 外设基址为：ADC 数据寄存器地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_ConvertedValue;              // 存储器地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                           // 数据源来自外设
    DMA_InitStructure.DMA_BufferSize = ADC_BUFF_SIZE;                            // 缓冲区大小，应该等于数据目的地的大小
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;             // 外设寄存器只有一个，地址不用递增
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                      // 存储器地址递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  // 外设数据大小为半字，即两个字节
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;          // 内存数据大小也为半字，跟外设数据大小相同
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                              // 循环传输模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;                          // DMA 传输通道优先级为高，当使用一个DMA通道时，优先级设置不影响
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                 // 禁止存储器到存储器模式，因为是从外设到存储器
    DMA_Init(NTC_VIN_ADC_DMA_CHANNEL, &DMA_InitStructure);                       // 初始化DMA

    DMA_Cmd(NTC_VIN_ADC_DMA_CHANNEL, ENABLE);  // 使能 DMA 通道

    // ADC 模式配置
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                   // 只使用一个ADC，属于单模式
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;                         // 扫描模式
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                   // 连续转换模式
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  // 不用外部触发转换，软件开启即可
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;               // 转换结果右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 2;                              // 转换通道个数
    ADC_Init(NTC_VIN_ADCx, &ADC_InitStructure);                          // 初始化ADC

    // 配置ADC时钟为PCLK2的8分频，即9MHz
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);

    // 配置ADC 通道的转换顺序和采样时间
    // ADC_RegularChannelConfig(NTC_VIN_ADCx, T12_TEMPER_ADC_CHANNEL, 1, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(NTC_VIN_ADCx, NTC_ADC_CHANNEL, 1, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(NTC_VIN_ADCx, VIN_ADC_CHANNEL, 2, ADC_SampleTime_239Cycles5);

    ADC_DMACmd(NTC_VIN_ADCx, ENABLE);  // 使能ADC DMA 请求
    ADC_Cmd(NTC_VIN_ADCx, ENABLE);     // 开启ADC ，并开始转换

    ADC_ResetCalibration(NTC_VIN_ADCx);  // 初始化ADC 校准寄存器
    // 等待校准寄存器初始化完成
    while (ADC_GetResetCalibrationStatus(NTC_VIN_ADCx))
        ;

    ADC_StartCalibration(NTC_VIN_ADCx);  // ADC开始校准
    // 等待校准完成
    while (ADC_GetCalibrationStatus(NTC_VIN_ADCx))
        ;

    ADC_SoftwareStartConvCmd(NTC_VIN_ADCx, ENABLE);  // 由于没有采用外部触发，所以使用软件触发ADC转换
}

// //获得ADC值
// //ch:通道值 0~9
// uint16_t Get_Adc(uint8_t ch)
// {
//     //设置指定ADC的规则组通道，一个序列，采样时间
//     ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5);  //ADC1,ADC通道,采样时间为239.5个周期
//     ADC_SoftwareStartConvCmd(ADC1, ENABLE);                            //使能指定的ADC1的软件转换启动功能
//     while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
//         ;                                 //等待转换结束
//     return ADC_GetConversionValue(ADC1);  //返回最近一次ADC1规则组的转换结果
// }

/**
 * @brief  ADC初始化
 * @param  无
 * @retval 无
 */
void BSP_ADCx_Init(void)
{
    ADCx_GPIO_Config();
    ADCx_Mode_Config();
    NtcVin_Mode_Config();
}

uint16_t BSP_ADCx_Read(uint16_t channel)
{
    return ADC_GetConversionValue(T12_TEMPER_ADCx);
}
/*********************************************END OF FILE**********************/
