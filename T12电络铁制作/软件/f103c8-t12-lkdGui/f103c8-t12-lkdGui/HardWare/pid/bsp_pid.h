/**
 ******************************************************************************
 * @file    bsp_pid.h
 * @author  Zxp
 * @version V1.0.0
 * @date    2021-06-10
 * @brief   PID算法
 ******************************************************************************
 */

#ifndef __BSP_PID_H_
#define __BSP_PID_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "stddef.h"
#include "stdint.h"

    typedef struct tagPidParam
    {
        float kP;         ///< 比例
        float kI;         ///< 积分
        float kD;         ///< 微分
        float currError;  ///< 当前误差
        float lastError;  ///< 上次误差
        float preError;   ///< 上上次误差
    } PidParamTypeDef;

    int32_t BSP_InitPidParameters(PidParamTypeDef *pPid, float kP, float kI, float kD);
    float BSP_CaluPid(PidParamTypeDef *pPid, float targetVal, float currVal);

#ifdef __cplusplus
}
#endif

#endif  // #ifndef __BSP_PID_H_
