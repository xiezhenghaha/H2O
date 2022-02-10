/**
 ******************************************************************************
 * @file    bsp_pid.c
 * @author  Zxp
 * @version V1.0.0
 * @date    2021-06-10
 * @brief   PID�㷨
 ******************************************************************************
 */

#include "bsp_pid.h"

/**
 * @brief   ��ʼ��PID����
 * @param   pPid: Ҫ�ı��PID�����ṹ��ָ��
 * @param   kP: ��������(���Ƶ�������)
 * @param   kI: ���ֲ���(������̬���)
 * @param   kD: ���ֲ���(Ԥ�����ı仯,�Ա���ǰ��������)
 * @return  ���óɹ����;
 * @retval  0: ���óɹ�
 * @retval  -1: pPidָ�벻��ȷ
 */
int32_t BSP_InitPidParameters(PidParamTypeDef *pPid, float kP, float kI, float kD)
{
    if (pPid == NULL)
    {
        return -1;
    }
    pPid->kP = kP;
    pPid->kI = kI;
    pPid->kD = kD;

    return 0;
    // g_Stat.pid.aggKp = 11;   //�������������õ�������
    // g_Stat.pid.aggKi = 0.5;  //���ֲ���T/Ti������������̬���
    // g_Stat.pid.aggKd = 1;    //΢�ֲ���Td/T������Ԥ�����ı仯��������ǰ����
    // g_Stat.pid.consKp = 11;  //���ص�PID����
    // g_Stat.pid.consKi = 3;
    // g_Stat.pid.consKd = 5;
}

/**
 * @brief   ����PID�����ֵ,��Ϊ����ʽPID,��������
 * @param   pPid: Ҫ�ı��PID�����ṹ��ָ��
 * @param   targetVal: Ŀ��ֵ
 * @param   currVal: ��ǰֵ
 * @return  ����,��pPidָ����ִ���,�򷵻�0
 */
float BSP_CaluPid(PidParamTypeDef *pPid, float targetVal, float currVal)
{
    float val;

    if (pPid == NULL)
    {
        return 0;
    }

    pPid->currError = targetVal - currVal;

    // val = pPid->kP * pPid->currError + pPid->kI * pPid->lastError + pPid->kD * pPid->preError;
    val = pPid->kP * pPid->currError + pPid->kI * (pPid->currError - pPid->lastError) + pPid->kD * (pPid->currError - (2 * pPid->lastError) + pPid->preError);

    pPid->preError = pPid->lastError;
    pPid->lastError = pPid->currError;

    return val;
}
