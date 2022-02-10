/**
 ******************************************************************************
 * @file    UICode.h
 * @author  Zxp
 * @version V1.0.0
 * @date    2021-06-17
 * @brief   界面布局头文件
 * @history Version     Date        Author      Note
 *          V1.0.0      2021-06-17  Zxp         创建文件
 ******************************************************************************
 */

#ifndef __UI_CODE_H_
#define __UI_CODE_H_

#include "includes.h"

enum keyValEnum
{
    eKeyVal_None = 0,
    eKeyVal_Up,
    eKeyVal_Down,
    eKeyVal_Sub,
    eKeyVal_Add,
    eKeyVal_Single,
    eKeyVal_Double,
    eKeyVal_Long,
};

void startUI(void);
void displayUI(void);
void sendKey2UI(int32_t key);

#endif  // #ifndef __UI_CODE_H_
