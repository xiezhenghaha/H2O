/**
 ******************************************************************************
 * @file    main.c
 * @author  Zxp
 * @version V1.0
 * @date    2021-02-07
 * @brief   T12电烙铁控制程序;
 *          使用FreeRTOS;
 ******************************************************************************
 */

// #include "main.h"

#include "includes.h"

static TaskHandle_t xHandleTaskLED;
static TaskHandle_t xHandleTaskTaskUserIF;
static TaskHandle_t xHandleTaskBaseTask;
static TaskHandle_t xHandleTaskReadAdc;
static TaskHandle_t xHandleTaskCaluPid;
static TaskHandle_t xHandleTaskDisplay;
static TaskHandle_t xHandleTaskAnalyseInput;

/* 控件句柄 */

static void AppTaskCreate(void);
static void AppObjCreate(void);
static void vTaskToggleLed(void *pvParameters);
static void vTaskTaskUserIF(void *pvParameters);
static void vTaskBaseTask(void *pvParameters);
static void vTaskReadAdc(void *pvParameters);
static void vTaskCaluPid(void *pvParameters);
static void vTaskDisplay(void *pvParameters);
static void vTaskHandleInput(void *pvParameters);

uint16_t ctrl = 0;
StatusTypeDef g_Stat;
QueueHandle_t xHandlerQueueInput;
Button royEcdButton;

void setup()
{
    uint32_t version = 0;

    BSP_FlashRead(FLASH_SAVE_ADDR, (uint16_t *)&version, 2);
    if (version == DEVICE_VERSION)
    {
        /* 非首次开机 */
        BSP_FlashRead(FLASH_SAVE_ADDR, (uint16_t *)&g_Stat, sizeof(g_Stat) / 2);
        // g_Stat.targetTemper = g_Stat.defaultTargetTemper;  // 将目标温度设置为开机默认温度
    }
    else
    {
        /* 首次开机 */
        g_Stat.version = DEVICE_VERSION;
        g_Stat.targetTemper = g_Stat.defaultTargetTemper = 300;  // TODO: 正常版本改为300摄氏度
        g_Stat.targetTemperStepVal = 1;
        g_Stat.vinVoltRate = 20.0;
        g_Stat.sleepMode.temperature = 37;
        g_Stat.sleepMode.time = pdMS_TO_TICKS(2 * 60 * 1000);
        g_Stat.shutdownMode.time = pdMS_TO_TICKS(10 * 60 * 1000);
        g_Stat.beep.volume = 100;
        g_Stat.beep.state = false;
        g_Stat.beep.mute = false;
    }
    g_Stat.dataUpdate = true;
    g_Stat.quakedTs = xTaskGetTickCount();
    g_Stat.sleepMode.mode = false;
    g_Stat.sleepMode.lastTemperature = g_Stat.targetTemper;
    g_Stat.shutdownMode.mode = false;
    g_Stat.shutdownMode.lastTemperature = g_Stat.targetTemper;
    g_Stat.beep.openTime = pdMS_TO_TICKS(10);
    g_Stat.saveDataTs = 0;
}

/**
 * @brief  主函数
 * @param  无
 * @retval 无
 */
int main(void)
{
    /* 关闭全局中断 */
    __set_PRIMASK(1);

    /* LED 端口初始化 */
    bsp_Init();
    setup();
    // g_Stat.version = DEVICE_VERSION;
    AppTaskCreate();
    AppObjCreate();
    printf("\r\nStart\r\n");
    vTaskStartScheduler();
    printf("\r\nNot run in FreeRTOS!\r\n");

    while (1)
    {
        if (ctrl == 1)
        {
            // ctrl = 0;
            BSP_LedSetOn();
        }
        else
        {
            // ctrl = 1;
            BSP_LedSetOff();
        }
    }
}

static void AppTaskCreate(void)
{
    xTaskCreate(vTaskTaskUserIF, "vTaskUserIF", 512, NULL, 1, &xHandleTaskTaskUserIF);
    xTaskCreate(vTaskToggleLed, "vTaskLed", 128, NULL, 2, &xHandleTaskLED);
    xTaskCreate(vTaskBaseTask, "vTaskBaseTask", 128, NULL, 3, &xHandleTaskBaseTask);
    xTaskCreate(vTaskDisplay, "vTaskDisplay", 512, NULL, 4, &xHandleTaskDisplay);
    xTaskCreate(vTaskCaluPid, "vTaskCaluPID", 128, NULL, 5, &xHandleTaskCaluPid);
    xTaskCreate(vTaskReadAdc, "vTaskReadAdc", 128, NULL, 6, &xHandleTaskReadAdc);
    xTaskCreate(vTaskHandleInput, "vTaskHandleInput", 128, NULL, 7, &xHandleTaskAnalyseInput);
}

static void AppObjCreate(void)
{
    xHandlerQueueInput = xQueueCreate(50, sizeof(uint8_t));
    if (xHandlerQueueInput == NULL)
    {
        DEBUG("消息队列创建失败!");
    }
}

static void vTaskTaskUserIF(void *pvParameters)
{
    // uint8_t str[300];
    // TickType_t lastTs;

    // (void *)pvParameters;

    // lastTs = xTaskGetTickCount();

    while (1)
    {
        // vTaskList((char *)str);
        // taskENTER_CRITICAL();
        // printf(
        //     "==========================================================\r\n"
        //     "任务名\t\t状态\t优先级\t剩余栈\t序号\r\n%s\r\n",
        //     str);
        // taskEXIT_CRITICAL();

        // vTaskGetRunTimeStats((char *)str);
        // taskENTER_CRITICAL();
        // printf("任务名\t\t运行计数\t使用率\r\n%s\r\n", str);
        // printf("剩余堆空间:%d\t时间戳: %d\r\n", xPortGetFreeHeapSize(),
        //        xTaskGetTickCount());
        // taskEXIT_CRITICAL();

        vTaskDelay(1000);
        // vTaskDelayUntil(&lastTs, 10);
    }
}

static void vTaskToggleLed(void *pvParameters)
{
    // (void *)pvParameters;

    while (1)
    {
        BSP_LedToggle();

        if (g_Stat.shutdownMode.mode == true)
        {
            vTaskDelay(2500);
        }
        else
        {
            vTaskDelay(500);
        }
    }
}

static void vTaskBaseTask(void *pvParameters)
{
    // (void *)pvParameters;
    TickType_t lastTs = xTaskGetTickCount();

    while (1)
    {
        button_ticks();
        g_Stat.timeStamp = lastTs;
        if ((g_Stat.shutdownMode.mode == false) && (g_Stat.timeStamp >= (g_Stat.shutdownMode.time + g_Stat.quakedTs)))
        {
            // TODO: 添加关机功能
            g_Stat.shutdownMode.mode = true;
            g_Stat.shutdownMode.lastTemperature = g_Stat.targetTemper;
            BSP_OLEDSetOff(); /* 关闭OLED显示 */
            /* 关闭加热PWM输出 */
        }
        else
        {
            if ((g_Stat.sleepMode.mode != true) && (g_Stat.timeStamp >= (g_Stat.sleepMode.time + g_Stat.quakedTs)))
            {
                // TODO: 添加睡眠功能
							  if(GPIO_ReadInputDataBit(HANDLE_SW_GPIO_PORT,HANDLE_SW_GPIO_PIN)==0)
								{
									g_Stat.sleepMode.mode = true;
									g_Stat.sleepMode.lastTemperature = g_Stat.targetTemper;
									g_Stat.targetTemper = g_Stat.sleepMode.temperature;
								}
								else
								{
								  g_Stat.targetTemper = g_Stat.sleepMode.lastTemperature;
								}
            }				
        }

        /* 判断当前设备状态 */
        if (g_Stat.existFlag == false)  // 烙铁头未插入状态
        {
            g_Stat.currDeviceState = eDeviceState_Error;
        }
        else
        {
            if (ABS(g_Stat.currTemper - g_Stat.targetTemper) < 2)
            {
                g_Stat.currDeviceState = eDeviceState_Keep;
            }
            else
            {
                if (g_Stat.heatPwmVal > 0)
                {
                    g_Stat.currDeviceState = eDeviceState_Heat;
                }
                else
                {
                    g_Stat.currDeviceState = eDeviceState_Cool;
                }
            }
        }

        /* 控制蜂鸣器 */
        if (g_Stat.beep.state == true)
        {
            if (g_Stat.timeStamp > (g_Stat.beep.timeStamp + g_Stat.beep.openTime))
            {
                g_Stat.beep.state = false;
                BSP_BeepSetOff(&g_Stat.beep, xTaskGetTickCount());
            }
        }
        BSP_SaveDataLoop();
        vTaskDelayUntil(&lastTs, pdMS_TO_TICKS(5));
    }
}

void delay100us(void)
{
    uint32_t i;
    for (i = 0; i < 7200; i++)
        ;
}

static void vTaskReadAdc(void *pvParameters)
{
    // (void *)pvParameters;
    uint16_t adcval, i, cnt = 1;
    uint32_t sumNtc = 0, sumVin = 0;
    uint32_t sum = 0;
    TickType_t lastTs;

    lastTs = xTaskGetTickCount();
    while (1)
    {
        /* 1.关闭加热输出 */
        BSP_SetHeatDuty(0);
        // BSP_SetHeatOff();
        /* 2.延时以等待运放电压稳定 */
        vTaskDelay(pdMS_TO_TICKS(5));
        /* 3.获取ADC值 */
        for (i = 0, sumNtc = 0, cnt = 0; i < ADC_BUFF_SIZE; i += 2, cnt++)
        {
            sumNtc += ADC_ConvertedValue[i];
        }
        g_Stat.ntcAdcVal = sumNtc / cnt;
        /* 根据NTC测得的ADC值判断烙铁头是否正常插入 */
        if (g_Stat.ntcAdcVal > eNtcExistAdcValue)
        {
            g_Stat.existFlag = false;
        }
        else
        {
            g_Stat.existFlag = true;
        }

        for (i = 1, sumVin = 0, cnt = 0; i < ADC_BUFF_SIZE; i += 2, cnt++)
        {
            sumVin += ADC_ConvertedValue[i];
        }
        g_Stat.vinAdcVal = sumVin / cnt;

        g_Stat.ntcVolt = (g_Stat.ntcAdcVal / 4095.0) * 3.3;
        g_Stat.vinVolt = ((g_Stat.vinAdcVal / 4095.0) * 3.3) * g_Stat.vinVoltRate;

//        for (i = 0, sum = 0, cnt = 0; i < 30; i++)
//        {
//            adcval = ADC_GetConversionValue(ADC2);
//            if (abs(adcval - g_Stat.temperAdcVal) < 100)
//            {
//                cnt++;
//                sum += adcval;
//            }
//            delay100us();
//        }
//        if (cnt < 15)
//        {
//            g_Stat.temperAdcVal = adcval;
//            for (i = 0, sum = 0, cnt = 0; i < 30; i++)
//            {
//                adcval = ADC_GetConversionValue(ADC2);
//                if (abs(adcval - g_Stat.temperAdcVal) < 100)
//                {
//                    cnt++;
//                    sum += adcval;
//                }
//                delay100us();
//            }
//        }
//        // sum = ADC_GetConversionValue(ADC2);
//        g_Stat.temperAdcVal = (uint16_t)BSP_CaluKalman(&g_Stat.kalman, ((float)sum) / cnt);
//        g_Stat.temperVolt = (g_Stat.temperAdcVal / 4095.0) * 3.3;
        g_Stat.temperAdcVal=g_Stat.ntcAdcVal;
        /* 4.计算ADC值对应的温度值 */
//        if (g_Stat.temperAdcVal <= eTemperAdcValue_100)
//        {
//            g_Stat.currTemper = map(g_Stat.temperAdcVal, 0, eTemperAdcValue_100, 0, 100);
//        }
//        else if (g_Stat.temperAdcVal <= eTemperAdcValue_200)
//        {
//            g_Stat.currTemper = map(g_Stat.temperAdcVal, eTemperAdcValue_100, eTemperAdcValue_200, 100, 200);
//        }
//        else if (g_Stat.temperAdcVal <= eTemperAdcValue_300)
//        {
//            g_Stat.currTemper = map(g_Stat.temperAdcVal, eTemperAdcValue_200, eTemperAdcValue_300, 200, 300);
//        }
//        else if (g_Stat.temperAdcVal <= eTemperAdcValue_420)
//        {
//            g_Stat.currTemper = map(g_Stat.temperAdcVal, eTemperAdcValue_300, eTemperAdcValue_420, 300, 420);
//        }
//        else if (g_Stat.temperAdcVal <= eTemperAdcValue_MaxLimit)
//        {
//            g_Stat.currTemper = map(g_Stat.temperAdcVal, eTemperAdcValue_420, eTemperAdcValue_MaxLimit, 420, 500);
//        }
//        else
//        {
//            g_Stat.currTemper = 0;
//        }
         
				 if(g_Stat.temperAdcVal<=3950)
				 {
				    g_Stat.currTemper=27.1+(g_Stat.temperAdcVal-856)*0.315;
				 }
         else
         {
				   g_Stat.currTemper=0;
				 }
        // 对显示在屏幕上的温度值进行滤波处理
        g_Stat.showTemper = BSP_CaluKalman(&g_Stat.kalmanShowTemp, g_Stat.currTemper + 0.3);
        taskENTER_CRITICAL();
        // printf("/*%d,%d,%d,%f,%f,%d*/", xTaskGetTickCount(), sum / cnt, g_Stat.temperAdcVal, g_Stat.targetTemper, g_Stat.currTemper, g_Stat.heatPwmVal);
        printf("data:%d,%d,%f,%f,%d,%f\r\n", sum / cnt, g_Stat.temperAdcVal, g_Stat.targetTemper, g_Stat.currTemper, g_Stat.heatPwmVal, g_Stat.showTemper);
        taskEXIT_CRITICAL();
        /* 5.开启加热输出 */
        BSP_SetHeatDuty(g_Stat.heatPwmVal);
        // BSP_SetHeatOn();
        /* 6.设置计算PID任务的信号量 */
        xTaskNotifyGive(xHandleTaskCaluPid);
        /* 7.延时以输出PWM */
        // vTaskDelay(pdMS_TO_TICKS(17));
        vTaskDelayUntil(&lastTs, pdMS_TO_TICKS(125));
    }
}

static void vTaskCaluPid(void *pvParameters)
{
    // (void *)pvParameters;
    PidParamTypeDef *pPid;
    float pidVal, pwmVal = 0;
    uint32_t notifyVal;

    while (1)
    {
        notifyVal = ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(3000));  // 等待信号量最大3000ms,并在接收到之后清除信号量
        // 只有在等待时间内接收到信号量,当前不处于关机模式,并且采集到的温度小于最高限制温度,才会计算PID并输出PWM,否则不输出
        if ((notifyVal > 0) && (g_Stat.shutdownMode.mode == false) && (g_Stat.temperAdcVal < eTemperAdcValue_MaxLimit))
        {
            pPid = &g_Stat.pid;
            pidVal = BSP_CaluPid(pPid, g_Stat.targetTemper, g_Stat.currTemper);
            pidVal = BSP_LimitFloat(pidVal, -10000, 10000);
            pwmVal += pidVal;
            pwmVal = BSP_LimitFloat(pwmVal, 0, 10000);
            g_Stat.heatPwmVal = (uint16_t)pwmVal;
        }
        else
        {
            g_Stat.heatPwmVal = 0;
        }
        BSP_SetHeatDuty(g_Stat.heatPwmVal); /* 设置占空比 */
    }
}
static void vTaskDisplay(void *pvParameters)
{
    // (void *)pvParameters;
    // char str[80];

    while (1)
    {
        /* 显示 */
        displayUI();
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

static void vTaskHandleInput(void *pvParameters)
{
    // (void *)pvParameters;
    uint8_t royKey, flag = 0;
    TickType_t lastTs = xTaskGetTickCount();

    while (1)
    {
        if (xQueueReceive(xHandlerQueueInput, (void *)(&royKey), portMAX_DELAY) == pdPASS)
        {
            switch (royKey)
            {
            case eInputEvent_TurnLeft:
                if (BSP_ReadRotaryEncoder_SW() == false)
                {
                    flag = true;
                    sendKey2UI(eKeyVal_Sub);
                }
                else
                {
                    sendKey2UI(eKeyVal_Up);
                }
                BSP_BeepSetOn(&g_Stat.beep, xTaskGetTickCount());
                break;
            case eInputEvent_TurnRight:
                if (BSP_ReadRotaryEncoder_SW() == false)
                {
                    flag = true;
                    sendKey2UI(eKeyVal_Add);
                }
                else
                {
                    sendKey2UI(eKeyVal_Down);
                }
                BSP_BeepSetOn(&g_Stat.beep, xTaskGetTickCount());
                break;
            case eInputEvent_SingleClick:
                sendKey2UI(eKeyVal_Single);
                break;
            case eInputEvent_DoubleClick:
                sendKey2UI(eKeyVal_Double);
                break;
            case eInputEvent_LongPressStart:
                if (flag == false)
                {
                    sendKey2UI(eKeyVal_Long);
                }
                break;
            case eInputEvent_Release:
                flag = false;
                break;

            // case eInputEvent_LongPressHold:
            //     sendKey2UI(eKeyVal_Long);
            //     break;
            case eInputEvent_Quake:
                break;
            default:
                break;
            }
            /* 只要有按键按下即退出睡眠或关机模式 */
            g_Stat.quakedTs = xTaskGetTickCount();
            if (g_Stat.sleepMode.mode == true)
            {
                g_Stat.sleepMode.mode = false;
                g_Stat.targetTemper = g_Stat.sleepMode.lastTemperature;
            }
            if (g_Stat.shutdownMode.mode == true)
            {
                g_Stat.shutdownMode.mode = false;
                g_Stat.targetTemper = g_Stat.shutdownMode.lastTemperature;
                BSP_OLEDSetOn();                  /* 开启OLED显示 */
                xTaskAbortDelay(xHandleTaskLED);  // 由于休眠时闪烁LED的任务会使用较长的延时,可能会导致唤醒后LED显示效果不理想,故使用此函数取消该任务的延时
            }
        }
    }
}

/*********************************************END OF FILE**********************/
