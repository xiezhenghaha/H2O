/**
 ******************************************************************************
 * @file    bsp.c
 * @author  Zxp
 * @version V1.0.1
 * @date    2021-06-07
 * @brief   ����Ӳ���ײ���������ģ������ļ�����Ҫ�ṩbsp_Init()��������������á�
 *          �������ÿ��c�ļ������ڿ�ͷ��� #include "bsp.h"���������е���������ģ�顣
 ******************************************************************************
 */
#include "includes.h"
// #include "MainTask.h"

volatile uint32_t ulHighFrequencyTimerTicks;

static void RoyEcdButtonHandler(void* pHandler)
{
    Button* p = (Button*)pHandler;
    uint8_t event;

    switch (p->event)
    {
    case BTN_SINGLE_CLICK:
        event = eInputEvent_SingleClick;
        break;
    case BTN_DOUBLE_CLICK:
        event = eInputEvent_DoubleClick;
        break;
    case BTN_LONG_RRESS_START:
        event = eInputEvent_LongPressStart;
        break;
    case BTN_LONG_PRESS_HOLD:
        event = eInputEvent_LongPressHold;
        break;
    case BTN_PRESS_UP:
        event = eInputEvent_Release;
    default:
        break;
    }

    xQueueSend(xHandlerQueueInput, (void*)(&event), 10);
}

/**
 * @brief   ��ʼ��Ӳ���豸��
 * @note    ֻ��Ҫ����һ�Ρ��ú�������CPU�Ĵ���������ļĴ�������ʼ��һЩȫ�ֱ�����
 * @param   None
 * @return  None
 */
void bsp_Init(void)
{
    /*
        ����ST�̼���������ļ��Ѿ�ִ����CPUϵͳʱ�ӵĳ�ʼ�������Բ����ٴ��ظ�����ϵͳʱ�ӡ�
        �����ļ�������CPU��ʱ��Ƶ�ʡ��ڲ�Flash�����ٶȺͿ�ѡ���ⲿSRAM FSMC��ʼ����

        ϵͳʱ��ȱʡ����Ϊ72MHz�������Ҫ���ģ������޸� system_stm32f10x.c �ļ�
    */

    /* ʹ��CRCУ��, ���ڿ���STemWin��ʹ�� */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);

    /* ���ȼ���������Ϊ4��������0-15����ռʽ���ȼ���0�������ȼ����������������ȼ���*/
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    //	bsp_InitDWT();       /* ��ʼDWT */
    //	bsp_InitUart(); 	 /* ��ʼ������ */
    //	bsp_InitLed(); 		 /* ��ʼLEDָʾ�ƶ˿� */
    //	bsp_InitKey();		 /* ��ʼ������ */
    //
    //	bsp_InitI2C();       /* ����I2C���� */
    //
    //	bsp_InitSPIBus();	 /* ����SPI���� */
    //
    //	bsp_InitExtSRAM();   /* ��ʼDWT */
    //	LCD_InitHard();	     /* ��ʼ����ʾ��Ӳ��(����GPIO��FSMC,��LCD���ͳ�ʼ��ָ��) */
    //	TOUCH_InitHard();    /* ��ʼ������ */
    //
    //	/* �����ļ�ϵͳ */
    //	result = f_mount(&fs, "0:/", 0);

    BSP_InitLed();
    USART_Config();
    bsp_SetTIMforInt(TIM6, 20000, 1, 0);
    BSP_InitOLED();
    bsp_InitHeatPwm(0);
    BSP_InitBeep();
    BSP_ADCx_Init();
    BSP_InitPidParameters(&(g_Stat.pid), 15, 0.1, 2.7);
    BSP_InitKalmanParameter(&(g_Stat.kalman), 8.6, 0.9);
    BSP_InitKalmanParameter(&(g_Stat.kalmanShowTemp), 10, 0.2);
    BSP_InitRotaryEncoder();
    BSP_InitKey();
    button_init(&royEcdButton, BSP_ReadRotaryEncoder_SW, RESET);
    button_attach(&royEcdButton, BTN_SINGLE_CLICK, RoyEcdButtonHandler);
    button_attach(&royEcdButton, BTN_DOUBLE_CLICK, RoyEcdButtonHandler);
    button_attach(&royEcdButton, BTN_LONG_RRESS_START, RoyEcdButtonHandler);
    button_attach(&royEcdButton, BTN_PRESS_UP, RoyEcdButtonHandler);
    button_start(&royEcdButton);
    startUI();
}

// /**
//  * @brief  �����汾��Ϊ�ַ�����ʽ
//  * @param  v: �汾��
//  * @retval �汾���ַ���
//  */
// char* BSP_ParseVersion(uint32_t v)
// {
//     if ((v & 0xff) != 0)
//     {
//         sprintf(str, "%d.%d.%d.dev%d", (v >> 24) & 0xff, (v >> 16) & 0xff, (v >> 8) & 0xff,
//                 v & 0xff);
//     }
//     else
//     {
//         sprintf(str, "%d.%d.%d", (v >> 24) & 0xff, (v >> 16) & 0xff, (v >> 8) & 0xff);
//     }
//     return str;
// }

/**
 * @brief   ��float������������һ����Χ��,������Сֵ������Ϊ��Сֵ,�������ֵ������Ϊ���ֵ
 * @param   val: Ҫ����ı���ָ��
 * @param   min: ��Сֵ
 * @param   max: ���ֵ
 * @return  ����������ֵ
 */
float BSP_LimitFloat(float val, float min, float max)
{
    if (val > max)
    {
        val = max;
    }
    else if (val < min)
    {
        val = min;
    }

    return val;
}

/**
 * @brief   ��int������������һ����Χ��,������Сֵ������Ϊ��Сֵ,�������ֵ������Ϊ���ֵ
 * @param   val: Ҫ����ı���ָ��
 * @param   min: ��Сֵ
 * @param   max: ���ֵ
 * @return  ����������ֵ
 */
int32_t BSP_LimitInt(int32_t val, int32_t min, int32_t max)
{
    if (val > max)
    {
        val = max;
    }
    else if (val < min)
    {
        val = min;
    }

    return val;
}

/**
 * @brief   �����ִ�һ����Χӳ�䵽��һ����Χ
 * @param   value: Ҫӳ���ֵ
 * @param   fromLow: ԭ��Χ������ֵ
 * @param   fromHigh: ԭ��Χ������ֵ
 * @param   toLow: ӳ�䷶Χ������ֵ
 * @param   toHigh: ӳ�䷶Χ������ֵ
 * @return  ӳ���ֵ
 */
float map(float value, float fromLow, float fromHigh, float toLow, float toHigh)
{
    return ((value - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow);
}

void BSP_SaveParameters()
{
    g_Stat.saveDataTs = g_Stat.timeStamp;
}

void BSP_SaveDataLoop()
{
    if (g_Stat.saveDataTs)
    {
        if ((g_Stat.saveDataTs - g_Stat.timeStamp) > 500)
        {
            g_Stat.saveDataTs = 0;
            BSP_FlashWrite(FLASH_SAVE_ADDR, (uint16_t*)&g_Stat, sizeof(g_Stat) / 2);
        }
    }
}

void TIM6_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) == SET)
    {
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
        ulHighFrequencyTimerTicks++;
    }
}

/***************************** END OF FILE **********************************/
