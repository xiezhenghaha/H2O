#ifndef __MAIN_H_
#define __MAIN_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "includes.h"
#include "stm32f10x.h"

#define ENCODE_VERSION(major, minor, patch, dev) \
    ((major << 24) | (minor << 16) | (patch << 8) | dev)

#define DEVICE_VERSION ENCODE_VERSION(0, 0, 0, 10)
    enum EnumInputEvent
    {
        eInputEvent_None = 0,        ///< 没有动作
        eInputEvent_SingleClick,     ///< 单击
        eInputEvent_DoubleClick,     ///< 双击
        eInputEvent_LongPressStart,  ///< 长按
        eInputEvent_LongPressHold,   ///< 长按
        eInputEvent_TurnLeft,        ///< 左转
        eInputEvent_TurnRight,       ///< 右转
        eInputEvent_Quake,           ///< 电烙铁手柄震动导致震动传感器变化
        eInputEvent_Release,         ///< 松开按键
    };

    enum EnumDeviceState
    {
        eDeviceState_Heat = 0,  ///< 升温状态
        eDeviceState_Cool,      ///< 降温状态
        eDeviceState_Error,     ///< 错误状态(烙铁头未插入)
        eDeviceState_Keep,      ///< 维持温度状态(温度在目标温度上下浮动)
    };

    typedef struct tagTiming
    {
        uint32_t lastTs;  ///< 上次触发时间的时间戳
        uint32_t time;    ///< 计时时间
    } TimingTypeDef;

    typedef struct tagStatus
    {
        uint32_t version;     ///< 设备版本信息
        uint32_t timeStamp;   ///< 当前时间戳
        uint32_t quakedTs;    ///< 上次震动开关变换时的时间戳
        uint32_t saveDataTs;  ///< 保存数据的时间戳
        struct
        {
            float lastTemperature;  ///< 转变模式前的温度
            uint32_t time : 31;     ///< 定时时间
            uint32_t mode : 1;      ///< 模式
        } shutdownMode;
        struct
        {
            float temperature;      ///< 设定温度
            float lastTemperature;  ///< 转变模式前的温度
            uint32_t time : 31;     ///< 定时时间
            uint32_t mode : 1;      ///< 模式
        } sleepMode;
        uint16_t temperAdcVal;         ///< 烙铁头温度的ADC采样值
        uint16_t heatPwmVal;           ///< 加热控制的PWM占空比
        uint16_t ntcAdcVal;            ///< NTC电阻的ADC采样值
        uint16_t vinAdcVal;            ///< 输入电源的ADC采样值
        uint16_t keyVal;               ///< 按键值
        uint16_t keyUpdate : 1;        ///< 按键更新状态位
        uint16_t dataUpdate : 1;       ///< 数据更新状态位
        uint16_t existFlag : 1;        ///< 烙铁头正确插入标志位,根据NTC测得的温度进行判断;正常插入为true
        BeepTypeDef beep;              ///< 蜂鸣器控制
        uint8_t currDeviceState;       ///< 当前设备状态
        int8_t targetTemperStepVal;    ///< 每次旋转编码器时目标温度改变的数值
        float defaultTargetTemper;     ///< 开机默认设定的温度
        float targetTemper;            ///< 设定的温度
        float currTemper;              ///< 当前温度
        float showTemper;              ///< 当前温度
        float temperVolt;              ///< 烙铁头温度的电压
        float ntcVolt;                 ///< NTC电阻上的分压
        float vinVolt;                 ///< 输入电源电压
        float vinVoltRate;             ///< 输入电源电压倍率
        PidParamTypeDef pid;           ///< PID算法的相关参数
        KalmanTypeDef kalman;          ///< Kalman滤波算法相关参数
        KalmanTypeDef kalmanShowTemp;  ///< Kalman滤波算法相关参数
    } StatusTypeDef;

    enum Temperature
    {
        eNtcExistAdcValue = 3950,         ///< NTC测得的ADC的值小于此值则说明烙铁头正常插入
        eTemperAdcValue_100 = 500,        ///< 100℃时的ADC值
        eTemperAdcValue_200 = 1000,       ///< 200℃时的ADC值
        eTemperAdcValue_300 = 1350,       ///< 300℃时的ADC值
        eTemperAdcValue_420 = 1825,       ///< 420℃时的ADC值
        eTemperAdcValue_MaxLimit = 2500,  ///< 最高限制温度的ADC值
    };

    extern StatusTypeDef g_Stat;
    extern QueueHandle_t xHandlerQueueInput;
    extern Button royEcdButton;

#ifdef __cplusplus
}
#endif

#endif  // #ifndef __MAIN_H_
