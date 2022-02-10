/**
  * @file   userAppPort.c
  * @author  guoweilkd
  * @version V0.0.0
  * @date    2019/01/17
  * @brief  lkdGui�û�Ӧ�ñ�д�ļ���
  */

#include "lkdGui.h"

static void UserMainWinFun(void *param);

/* �û������� */
lkdWin userMainWin = {
    .x = 10,
    .y = 10,
    .wide = 100,
    .hight = 100,
    .title = "������",
    .param = NULL,
    .WindowFunction = UserMainWinFun,
};

/**
  *@brief gui��ʼ���ο�����
  *@param  None
  *@retval None
  */
void userAppPortInit(void)
{
    /* 1. ��ʼ��lcd */

    /* 2. ��ʼ���û����� */
    //defaultFontInit();

    /* 3. ��ʼ������,��Ӵ��� */
    GuiWinInit();
    GuiWinAdd(&userMainWin);

    /* 4. ������Ļ */
    GuiClearScreen(GuiGetbackcolor());
    OpenLcdDisplay();
    GuiUpdateDisplayAll();
}

/**
  *@brief gui���к���
  *@param  None
  *@retval None
  */
void userAppPortRun(void)
{
    /* ���ڵ��ȹ��� */
    GuiWinDisplay();
}

static void UserMainWinFun(void *param)
{
    //�û��Զ�����롣
}
