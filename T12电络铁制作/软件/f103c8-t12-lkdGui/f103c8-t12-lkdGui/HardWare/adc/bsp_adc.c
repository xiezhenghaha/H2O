#include "bsp_adc.h"

__IO uint16_t ADC_ConvertedValue[ADC_BUFF_SIZE];

/**
 * @brief  ADC GPIO ��ʼ��
 * @param  ��
 * @retval ��
 */
static void ADCx_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // �� ADC IO�˿�ʱ��
    RCC_APB2PeriphClockCmd(T12_TEMPER_ADC_GPIO_CLK, ENABLE);

    // ���� ADC IO ����ģʽ
    GPIO_InitStructure.GPIO_Pin = T12_TEMPER_ADC_PIN | NTC_ADC_PIN | VIN_ADC_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(T12_TEMPER_ADC_PORT, &GPIO_InitStructure);
}

/**
 * @brief  ����ADC����ģʽ
 * @param  ��
 * @retval ��
 */
static void ADCx_Mode_Config(void)
{
    ADC_InitTypeDef ADC_InitStructure;

    RCC_APB2PeriphClockCmd(T12_TEMPER_ADC_CLK, ENABLE);

    // ADC ģʽ����

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                   // ֻʹ��һ��ADC�����ڶ���ģʽ
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;                        // ��ֹɨ��ģʽ����ͨ����Ҫ����ͨ������Ҫ
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                   // ����ת��ģʽ
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  // �����ⲿ����ת���������������
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;               // ת������Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel = 1;                              // ת��ͨ��1��
    ADC_Init(T12_TEMPER_ADCx, &ADC_InitStructure);                       // ��ʼ��ADC

    // ����ADCʱ��ΪPCLK2��8��Ƶ����9MHz
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);

    // ���� ADC ͨ��ת��˳��Ͳ���ʱ��
    ADC_RegularChannelConfig(T12_TEMPER_ADCx, T12_TEMPER_ADC_CHANNEL, 1, ADC_SampleTime_55Cycles5);

    // ADC ת�����������жϣ����жϷ�������ж�ȡת��ֵ
    // ADC_ITConfig(T12_TEMPER_ADCx, ADC_IT_EOC, ENABLE);

    // ����ADC ������ʼת��
    ADC_Cmd(T12_TEMPER_ADCx, ENABLE);

    // ��ʼ��ADC У׼�Ĵ���
    ADC_ResetCalibration(T12_TEMPER_ADCx);
    // �ȴ�У׼�Ĵ�����ʼ�����
    while (ADC_GetResetCalibrationStatus(T12_TEMPER_ADCx))
        ;

    // ADC��ʼУ׼
    ADC_StartCalibration(T12_TEMPER_ADCx);
    // �ȴ�У׼���
    while (ADC_GetCalibrationStatus(T12_TEMPER_ADCx))
        ;

    // ����û�в����ⲿ����������ʹ���������ADCת��
    ADC_SoftwareStartConvCmd(T12_TEMPER_ADCx, ENABLE);
}

/**
 * @brief  ����ADC����ģʽ
 * @param  ��
 * @retval ��
 */
static void NtcVin_Mode_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    // ��DMAʱ��
    RCC_AHBPeriphClockCmd(NTC_VIN_ADC_DMA_CLK, ENABLE);
    // ��ADCʱ��
    RCC_APB2PeriphClockCmd(NTC_VIN_ADC_CLK, ENABLE);

    // ��λDMA������
    DMA_DeInit(NTC_VIN_ADC_DMA_CHANNEL);

    // ���� DMA ��ʼ���ṹ��
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(NTC_VIN_ADCx->DR));       // �����ַΪ��ADC ���ݼĴ�����ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_ConvertedValue;              // �洢����ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                           // ����Դ��������
    DMA_InitStructure.DMA_BufferSize = ADC_BUFF_SIZE;                            // ��������С��Ӧ�õ�������Ŀ�ĵصĴ�С
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;             // ����Ĵ���ֻ��һ������ַ���õ���
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                      // �洢����ַ����
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  // �������ݴ�СΪ���֣��������ֽ�
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;          // �ڴ����ݴ�СҲΪ���֣����������ݴ�С��ͬ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                              // ѭ������ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;                          // DMA ����ͨ�����ȼ�Ϊ�ߣ���ʹ��һ��DMAͨ��ʱ�����ȼ����ò�Ӱ��
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                 // ��ֹ�洢�����洢��ģʽ����Ϊ�Ǵ����赽�洢��
    DMA_Init(NTC_VIN_ADC_DMA_CHANNEL, &DMA_InitStructure);                       // ��ʼ��DMA

    DMA_Cmd(NTC_VIN_ADC_DMA_CHANNEL, ENABLE);  // ʹ�� DMA ͨ��

    // ADC ģʽ����
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                   // ֻʹ��һ��ADC�����ڵ�ģʽ
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;                         // ɨ��ģʽ
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                   // ����ת��ģʽ
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  // �����ⲿ����ת���������������
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;               // ת������Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel = 2;                              // ת��ͨ������
    ADC_Init(NTC_VIN_ADCx, &ADC_InitStructure);                          // ��ʼ��ADC

    // ����ADCʱ��ΪPCLK2��8��Ƶ����9MHz
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);

    // ����ADC ͨ����ת��˳��Ͳ���ʱ��
    // ADC_RegularChannelConfig(NTC_VIN_ADCx, T12_TEMPER_ADC_CHANNEL, 1, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(NTC_VIN_ADCx, NTC_ADC_CHANNEL, 1, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(NTC_VIN_ADCx, VIN_ADC_CHANNEL, 2, ADC_SampleTime_239Cycles5);

    ADC_DMACmd(NTC_VIN_ADCx, ENABLE);  // ʹ��ADC DMA ����
    ADC_Cmd(NTC_VIN_ADCx, ENABLE);     // ����ADC ������ʼת��

    ADC_ResetCalibration(NTC_VIN_ADCx);  // ��ʼ��ADC У׼�Ĵ���
    // �ȴ�У׼�Ĵ�����ʼ�����
    while (ADC_GetResetCalibrationStatus(NTC_VIN_ADCx))
        ;

    ADC_StartCalibration(NTC_VIN_ADCx);  // ADC��ʼУ׼
    // �ȴ�У׼���
    while (ADC_GetCalibrationStatus(NTC_VIN_ADCx))
        ;

    ADC_SoftwareStartConvCmd(NTC_VIN_ADCx, ENABLE);  // ����û�в����ⲿ����������ʹ���������ADCת��
}

// //���ADCֵ
// //ch:ͨ��ֵ 0~9
// uint16_t Get_Adc(uint8_t ch)
// {
//     //����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
//     ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5);  //ADC1,ADCͨ��,����ʱ��Ϊ239.5������
//     ADC_SoftwareStartConvCmd(ADC1, ENABLE);                            //ʹ��ָ����ADC1�����ת����������
//     while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
//         ;                                 //�ȴ�ת������
//     return ADC_GetConversionValue(ADC1);  //�������һ��ADC1�������ת�����
// }

/**
 * @brief  ADC��ʼ��
 * @param  ��
 * @retval ��
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
