/**
 ******************************************************************************
 * @file    bsp_kalman.c
 * @author  Zxp
 * @version V1.0.0
 * @date    2021-06-08
 * @brief   ���ļ�ʵ���˿������˲��㷨
 ******************************************************************************
 */

#include "bsp_kalman.h"

/**
 * @brief   ��ʼ���������˲��Ĳ���
 * @param   pKalman: Ҫ���õĿ����������ṹ��ָ��
 * @param   covR: �۲�����Э����
 * @param   covQ: ��������Э����
 * @return  ״̬
 * @retval  0: ���óɹ�
 * @retval  -1: ָ��Ϊ��
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
 * @brief   �����˲�ֵ
 * @param   pKalman: Ҫ����Ŀ����������ṹ��ָ��
 * @param   measure: ��������ԭʼ����
 * @return  �˲�ֵ,��pKalmanΪ��,�򷵻�0
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
