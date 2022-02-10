/**
 ******************************************************************************
 * @file    bsp_kalman.h
 * @author  Zxp
 * @version V1.0.0
 * @date    2021-06-08
 * @brief   卡尔曼滤波算法头文件
 ******************************************************************************
*/

#ifndef __BSP_KALMAN_H_
#define __BSP_KALMAN_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "stddef.h"
#include "stdint.h"

    typedef struct tagKalmanParam
    {
        float covQ;  ///< 过程噪声协方差,Q增大,动态响应变快,收敛稳定性变坏
        float covR;  ///< 观测噪声协方差,R增大,动态响应变慢,收敛稳定性变好
        float x_last;
        float p_last;
    } KalmanTypeDef;

    int32_t BSP_InitKalmanParameter(KalmanTypeDef* pKalman, float covR, float covQ);
    float BSP_CaluKalman(KalmanTypeDef* pKalman, float measure);

#ifdef __cplusplus
}
#endif

#endif  // #ifndef __BSP_KALMAN_H_
