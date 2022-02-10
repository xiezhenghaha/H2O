/**
 ******************************************************************************
 * @file    bsp_kalman.c
 * @author  Zxp
 * @version V1.0.0
 * @date    2021-06-08
 * @brief   此文件实现了卡尔曼滤波算法
 ******************************************************************************
 */

#include "bsp_kalman.h"

/**
 * @brief   初始化卡尔曼滤波的参数
 * @param   pKalman: 要设置的卡尔曼参数结构的指针
 * @param   covR: 观测噪声协方差
 * @param   covQ: 过程噪声协方差
 * @return  状态
 * @retval  0: 设置成功
 * @retval  -1: 指针为空
 */
int32_t BSP_InitKalmanParameter(KalmanTypeDef* pKalman, float covR, float covQ)
{
    if (pKalman == NULL)
    {
        return -1;
    }
    pKalman->covR = covR;
    pKalman->covQ = covQ;
    pKalman->x_last = 0;
    pKalman->p_last = pKalman->covQ;

    return 0;
}

/**
 * @brief   计算滤波值
 * @param   pKalman: 要计算的卡尔曼参数结构的指针
 * @param   measure: 测量到的原始数据
 * @return  滤波值,若pKalman为空,则返回0
 */
float BSP_CaluKalman(KalmanTypeDef* pKalman, float measure)
{
    float x_mid, kg, p_mid;
    float x_now, p_now;

    if (pKalman == NULL)
    {
        return 0;
    }
    x_mid = pKalman->x_last;
    p_mid = pKalman->p_last + pKalman->covQ;
    kg = p_mid / (p_mid + pKalman->covR);
    x_now = x_mid + kg * (measure - x_mid);
    p_now = (1 - kg) * p_mid;

    pKalman->p_last = p_now;
    pKalman->x_last = x_now;

    return x_now;
}
