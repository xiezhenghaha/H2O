/**
 ******************************************************************************
 * @file    bsp_pid.c
 * @author  Zxp
 * @version V1.0.0
 * @date    2021-06-10
 * @brief   PID算法
 ******************************************************************************
 */

#include "bsp_pid.h"

/**
 * @brief   初始化PID参数
 * @param   pPid: 要改变的PID参数结构的指针
 * @param   kP: 比例参数(控制调节力度)
 * @param   kI: 积分参数(消除稳态误差)
 * @param   kD: 积分参数(预测误差的变化,以便提前做出决策)
 * @return  设置成功与否;
 * @retval  0: 设置成功
 * @retval  -1: pPid指针不正确
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
    // g_Stat.pid.aggKp = 11;   //比例参数，设置调节力度
    // g_Stat.pid.aggKi = 0.5;  //积分参数T/Ti，可以消除稳态误差
    // g_Stat.pid.aggKd = 1;    //微分参数Td/T，可以预测误差的变化，做到提前决策
    // g_Stat.pid.consKp = 11;  //保守的PID参数
    // g_Stat.pid.consKi = 3;
    // g_Stat.pid.consKd = 5;
}

/**
 * @brief   计算PID的输出值,此为增量式PID,返回增量
 * @param   pPid: 要改变的PID参数结构的指针
 * @param   targetVal: 目标值
 * @param   currVal: 当前值
 * @return  增量,若pPid指针出现错误,则返回0
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
