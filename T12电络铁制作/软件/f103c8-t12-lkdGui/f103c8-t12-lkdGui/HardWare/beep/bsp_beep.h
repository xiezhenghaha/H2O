/**
 ******************************************************************************
 * @file    bsp_beep.h
 * @author  Zxp
 * @version V1.1.0
 * @date    2021-06-02
 * @brief   蜂鸣器驱动头文件
 * @History Version     Date        Author      Note
 *          1.0.0       2021-06-02  ZXP         初始版本
 *          1.1.0       2021-06-07  ZXP         修改函数名,以符合命名规范
 *          1.2.0       2021-07-04  ZXP         添加音量功能,并构建蜂鸣器控制结构
 ******************************************************************************
*/

#ifndef __BSP_BEEP_H_
#define __BSP_BEEP_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>

#include "stm32f10x.h"

    typedef struct tagBeep
    {
        uint32_t volume : 7;     ///< 音量(0~100)
        uint8_t state : 1;       ///< 开关状态,true开启
        uint8_t mute : 1;        ///< 静音控制位,true时开启静音模式
        uint32_t openTime : 23;  ///< 打开的时间,超过此时间后即关闭
        uint32_t timeStamp;      ///< 开启时的时间戳
    } BeepTypeDef;

    void BSP_InitBeep(void);
    void BSP_BeepSetOn(BeepTypeDef *pBeep, uint32_t ts);
    void BSP_BeepSetOff(BeepTypeDef *pBeep, uint32_t ts);

#ifdef __cplusplus
}
#endif

#endif  // #ifndef __BSP_BEEP_H_
