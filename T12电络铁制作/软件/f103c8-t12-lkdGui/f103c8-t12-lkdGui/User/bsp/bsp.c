/**
 ******************************************************************************
 * @file    bsp.c
 * @author  Zxp
 * @version V1.0.1
 * @date    2021-06-07
 * @brief   这是硬件底层驱动程序模块的主文件。主要提供bsp_Init()函数供主程序调用。
 *          主程序的每个c文件可以在开头添加 #include "bsp.h"来包含所有的外设驱动模块。
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
 * @brief   初始化硬件设备。
 * @note    只需要调用一次。该函数配置CPU寄存器和外设的寄存器并初始化一些全局变量。
 * @param   None
 * @return  None
 */
void bsp_Init(void)
{
    /*
        由于ST固件库的启动文件已经执行了CPU系统时钟的初始化，所以不必再次重复配置系统时钟。
        启动文件配置了CPU主时钟频率、内部Flash访问速度和可选的外部SRAM FSMC初始化。

        系统时钟缺省配置为72MHz，如果需要更改，可以修改 system_stm32f10x.c 文件
    */

    /* 使能CRC校验, 用于开启STemWin的使用 */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);

    /* 优先级分组设置为4，可配置0-15级抢占式优先级，0级子优先级，即不存在子优先级。*/
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    //	bsp_InitDWT();       /* 初始DWT */
    //	bsp_InitUart(); 	 /* 初始化串口 */
    //	bsp_InitLed(); 		 /* 初始LED指示灯端口 */
    //	bsp_InitKey();		 /* 初始化按键 */
    //
    //	bsp_InitI2C();       /* 配置I2C总线 */
    //
    //	bsp_InitSPIBus();	 /* 配置SPI总线 */
    //
    //	bsp_InitExtSRAM();   /* 初始DWT */
    //	LCD_InitHard();	     /* 初始化显示器硬件(配置GPIO和FSMC,给LCD发送初始化指令) */
    //	TOUCH_InitHard();    /* 初始化触摸 */
    //
    //	/* 挂载文件系统 */
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
//  * @brief  解析版本号为字符串形式
//  * @param  v: 版本号
//  * @retval 版本号字符串
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
 * @brief   将float型数据限制在一定范围内,低于最小值则将其设为最小值,高于最大值则将其设为最大值
 * @param   val: 要处理的变量指针
 * @param   min: 最小值
 * @param   max: 最大值
 * @return  经过处理后的值
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
 * @brief   将int型数据限制在一定范围内,低于最小值则将其设为最小值,高于最大值则将其设为最大值
 * @param   val: 要处理的变量指针
 * @param   min: 最小值
 * @param   max: 最大值
 * @return  经过处理后的值
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
 * @brief   将数字从一个范围映射到另一个范围
 * @param   value: 要映射的值
 * @param   fromLow: 原范围的下限值
 * @param   fromHigh: 原范围的上限值
 * @param   toLow: 映射范围的下限值
 * @param   toHigh: 映射范围的上限值
 * @return  映射的值
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
