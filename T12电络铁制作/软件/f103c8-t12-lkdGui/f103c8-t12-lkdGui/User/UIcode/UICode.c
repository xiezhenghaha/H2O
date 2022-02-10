/**
 ******************************************************************************
 * @file    UICode.c
 * @author  Zxp
 * @version V1.0.0
 * @date    2021-06-17
 * @brief   界面布局文件
 * @history Version     Date        Author      Note
 *          V1.0.0      2021-06-17  Zxp         创建文件
 ******************************************************************************
 */

#include "UICode.h"

#include "lkdGui.h"

char str[80];
extern lkdFont selfFont, defaultFont, gb2312_16x16Font;

enum MenuKeyIs
{
    MKEY_NO,
    MKEY_UP,
    MKEY_DOWN,
    MKEY_LEFT,
    MKEY_RIGHT,
    MKEY_OK,
    MKEY_HOME,
    MKEY_CANCEL
};

/* 菜单按键 */
enum MenuKeyIs keyStatus;
/* 菜单句柄 */
lkdMenu HmiMenu;
/* 菜单栈 */
#define MENUSTACK_NUM 8
MenuStack userMenuStack[MENUSTACK_NUM];

/* 函数申明 */
static void HomeFun(void *param);
static void MenuFun(void *param);
static void InfoFun(void *param);
static void settingFun(void *param);
static void Win1Fun(void *param);
static void stepAdjustFun(void *param);
static void defaultTemperFun(void *param);
static void sleepTemperFun(void *param);
static void sleepTimeFun(void *param);
static void sleepTimeFun(void *param);
static void powerOffTimeFun(void *param);
static void volumeAdjustFun(void *param);
static void vinVoltRateFun(void *param);

/* 窗口定义 */
lkdWin homeWin = {0, 0, 128, 64, NULL, HomeFun, NULL};          /* 桌面窗口 */
lkdWin menuWin = {0, 0, 128, 64, "Setting", MenuFun, NULL};     /* 菜单窗口 */
lkdWin infoWin = {0, 0, 128, 64, "Information", InfoFun, NULL}; /* 信息窗口 */
lkdWin settingWin = {0, 0, 128, 64, "Parameters", settingFun, NULL};
lkdWin win1 = {0, 0, 128, 64, "Test Win", Win1Fun, NULL};
lkdWin stepAdjustWin = {10, 18, 108, 40, NULL, stepAdjustFun, NULL};
lkdWin defaultTemperWin = {10, 18, 108, 40, NULL, defaultTemperFun, NULL};
lkdWin sleepTemperWin = {10, 18, 108, 40, NULL, sleepTemperFun, NULL};
lkdWin sleepTimeWin = {10, 18, 108, 40, NULL, sleepTimeFun, NULL};
lkdWin powerOffTimeWin = {10, 18, 108, 40, NULL, powerOffTimeFun, NULL};
lkdWin volumeAdjustWin = {10, 18, 108, 40, NULL, volumeAdjustFun, NULL};
lkdWin vinVoltRateWin = {10, 18, 108, 40, NULL, vinVoltRateFun, NULL};

/* 菜单节点定义 */
lkdMenuNode Node33 = {33, "Menu2->4", NULL, NULL, NULL};
lkdMenuNode Node32 = {32, "Menu2->3", &Node33, NULL, NULL};
lkdMenuNode Node31 = {31, "Menu2->2", &Node32, NULL, NULL};
lkdMenuNode Node30 = {30, "Menu2->1", &Node31, NULL, NULL};

lkdMenuNode Node2_9 = {29, "Menu2->17", NULL, NULL, NULL};
lkdMenuNode Node2_8 = {28, "Menu2->16", &Node2_9, NULL, NULL};
lkdMenuNode Node2_7 = {27, "Menu2->15", &Node2_8, NULL, NULL};
lkdMenuNode Node2_6 = {26, "Menu2->14", &Node2_7, NULL, NULL};
lkdMenuNode Node2_5 = {25, "Menu2->13", &Node2_6, NULL, NULL};
lkdMenuNode Node2_4 = {24, "Menu2->12", &Node2_5, NULL, NULL};
lkdMenuNode Node2_3 = {23, "Menu2->11", &Node2_4, NULL, NULL};
lkdMenuNode Node2_2 = {22, "Menu2->10", &Node2_3, NULL, NULL};
lkdMenuNode Node2_1 = {21, "Menu2->9", &Node2_2, NULL, NULL};
lkdMenuNode Node2_0 = {20, "Menu2->8", &Node2_1, NULL, NULL};

// 参数设置
lkdMenuNode Node1_6 = {14, "Supply voltage ratio", NULL, NULL, &vinVoltRateWin};       // 输入电压倍率
lkdMenuNode Node1_5 = {13, "Adjust volume", &Node1_6, NULL, &volumeAdjustWin};         // 调节音量
lkdMenuNode Node1_4 = {13, "Power off time", &Node1_5, NULL, &powerOffTimeWin};        // 关机时间
lkdMenuNode Node1_3 = {13, "Sleep time", &Node1_4, NULL, &sleepTimeWin};               // 进入待机的时间
lkdMenuNode Node1_2 = {13, "Sleep temperature", &Node1_3, NULL, &sleepTemperWin};      // 待机温度
lkdMenuNode Node1_1 = {14, "Default temperature", &Node1_2, NULL, &defaultTemperWin};  // 开机默认温度
lkdMenuNode Node1_0 = {13, "Step", &Node1_1, NULL, &stepAdjustWin};                    // 旋钮步进值

// 主设置
lkdMenuNode Node2 = {2, "Information", NULL, NULL, &infoWin};     // 信息
lkdMenuNode Node1 = {1, "Param", &Node2, &Node1_0, &settingWin};  // 参数设置
lkdMenuNode NodeRoot0 = {0, "root", NULL, &Node1, NULL};

/**
  *@brief  菜单项处理
  *@param  step 步骤 pNode 菜单节点
  *@retval None
  */
static void MenuItemDealWith(lkdMenuNode *pNode)
{
    if (pNode->pSon != NULL)
    {  //展开选中节点的菜单
        GuiMenuCurrentNodeSonUnfold(&HmiMenu);
    }
    else if (pNode->user != NULL)
    {  //打开菜单对应的窗口
        GuiWinAdd(pNode->user);
    }
}

/**
  *@brief  菜单控制函数
  *@param  None
  *@retval None
  */
void MenuControlFun(void)
{
    switch (keyStatus)
    {
    case MKEY_UP:
        GuiMenuItemUpMove(&HmiMenu);
        break;
    case MKEY_DOWN:
        GuiMenuItemDownMove(&HmiMenu);
        break;
    case MKEY_LEFT:
    case MKEY_CANCEL:
        GuiMenuCurrentNodeHide(&HmiMenu);
        if (HmiMenu.count == 0)
        {  //检测到菜单退出信号
            GuiWinDeleteTop();
        }
        break;
    case MKEY_RIGHT:
    case MKEY_OK:
        MenuItemDealWith(GuiMenuGetCurrentpNode(&HmiMenu));
        break;
    }
}

/**
  *@brief  菜单初始化
  *@param  None
  *@retval None
  */
void UserMenuInit(void)
{
    HmiMenu.x = 0;
    HmiMenu.y = 16;
    HmiMenu.wide = 127;
    HmiMenu.hight = 48;
    HmiMenu.Itemshigh = 15;
    HmiMenu.ItemsWide = 127;
    HmiMenu.stack = userMenuStack;
    HmiMenu.stackNum = 8;
    HmiMenu.Root = &NodeRoot0;                /* 菜单句柄跟菜单节点绑定到一起 */
    HmiMenu.MenuDealWithFun = MenuControlFun; /* 菜单控制回掉函数 */
    GuiMenuInit(&HmiMenu);
}

/**
  *@brief  桌面窗口实体
  *@param  None
  *@retval None
  */
static void HomeFun(void *param)
{
    //用户应用代码:画图等
    static uint8_t step;
    static int32_t lastTarget = -12345;
    int32_t len, bmpNum;
    lkdColour forecolor = GuiGetForecolor();

    if (step == 0)
    {
        GuiClearScreen(CWHITLE);
        step = 1;
        lastTarget = -12345;  // 改变此值以刷新界面
    }
    if (g_Stat.currDeviceState == eDeviceState_Error)
    {
        sprintf(str, "%03d", (int32_t)0);
    }
    else
    {
        sprintf(str, "%3d", (int32_t)g_Stat.showTemper);
    }
    GuiFontSet(&selfFont);
    GuiRowText(0, 16, 50, FONT_MID, (uint8_t *)str);
    GuiFontSet(&defaultFont);

    /* 显示占空比 */
    sprintf(str, "%3d%%", (int32_t)g_Stat.heatPwmVal / 100);
    GuiRowText(103, 40, 25, FONT_MID, (uint8_t *)str);

    /* 显示电源电压 */
    // GuiRect(0, 0, 32, 15, CBLACK);
    sprintf(str, "%4.1fv", g_Stat.vinVolt);
    // sprintf(str, "%x", ((((uint32_t) "温") >> 8)));
    GuiRowText(2, 3, 30, FONT_MID, (uint8_t *)str);

    /* 显示设备当前状态 */
    extern uint8_t bmpArr[][32];
    switch (g_Stat.currDeviceState)
    {
    case eDeviceState_Heat:
        bmpNum = 0;
        strcpy(str, "升温");
        break;
    case eDeviceState_Cool:
        bmpNum = 1;
        strcpy(str, "降温");
        break;
    case eDeviceState_Error:
        bmpNum = 2;
        strcpy(str, "错误");
        break;
    case eDeviceState_Keep:
        bmpNum = 3;
        strcpy(str, "维持");
        break;
    default:
        break;
    }
    GuiFontSet(&gb2312_16x16Font);
    GuiRowText(76, 0, 38, FONT_MID, (uint8_t *)str);
    if (g_Stat.sleepMode.mode == true)
    {
        GuiRowText(38, 0, 38, FONT_MID, "睡眠");
    }
    else
    {
        GuiRowText(38, 0, 38, FONT_MID, "运行");
    }
    GuiFontSet(&defaultFont);
    GuiBitMap(bmpArr[bmpNum], 111, 0, 16, 16, 0);

    /* 按键状态处理 */
    switch (keyStatus)
    {
    case MKEY_UP:
        BSP_SaveParameters();
        g_Stat.targetTemper -= g_Stat.targetTemperStepVal;
        break;
    case MKEY_DOWN:
        BSP_SaveParameters();
        g_Stat.targetTemper += g_Stat.targetTemperStepVal;
        break;
    case MKEY_LEFT:
        BSP_SaveParameters();
        g_Stat.targetTemper -= g_Stat.targetTemperStepVal * 10;
        break;
    case MKEY_RIGHT:
        BSP_SaveParameters();
        g_Stat.targetTemper += g_Stat.targetTemperStepVal * 10;
        break;
    case MKEY_CANCEL:
        // step = 0;
        break;
    case MKEY_OK:
        step = 0;
        GuiWinAdd(&menuWin);
        return;
    default:
        break;
    }
    g_Stat.targetTemper = BSP_LimitFloat(g_Stat.targetTemper, 0, 500);

    /* 绘制目标温度 */
    len = (int32_t)g_Stat.targetTemper;
    if (len != lastTarget)
    {
        lastTarget = len;

        len = (int32_t)map(g_Stat.targetTemper, 0, 500, 1, 126);
        len = BSP_LimitInt(len, 0, 126);
        GuiFillRect(0, 50, 127, 63, CWHITLE);
        sprintf(str, "%d", (int32_t)g_Stat.targetTemper);
        if (len >= (127 - 18))
        {
            GuiRowText(len - 18, 50, 18, FONT_MID, (uint8_t *)str);
        }
        else
        {
            GuiRowText(len, 50, 18, FONT_MID, (uint8_t *)str);
        }
        GuiRect(0, 61, 127, 63, CBLACK);  // 绘制进度条边框
        // GuiHLine(1, 62, 126, CWHITLE);    // 清空进度条
        GuiHLine(0, 62, len, CBLACK);  // 填充进度条
    }
}

/**
  *@brief  菜单窗口实体
  *@param  None
  *@retval None
  */
static void MenuFun(void *param)
{
    if (HmiMenu.count == 0)
    {
        GuiMenuCurrentNodeSonUnfold(&HmiMenu);
    }
    HmiMenu.MenuDealWithFun();
}

/**
  *@brief  信息窗口实体
  *@param  None
  *@retval None
  */
static void InfoFun(void *param)
{
    //用户应用代码:画图等
    static uint8_t step;
    lkdColour forecolor = GuiGetForecolor();

    if (step == 0)
    {
        // GuiClearScreen(CWHITLE);
        GuiRowText(0, 16, 128, FONT_MID, "Version");
        // GuiRowText(0, 30, 128, FONT_MID, g_Stat.version);
        step = 1;
    }

    /* 按键状态处理 */
    switch (keyStatus)
    {
    case MKEY_UP:
        break;
    case MKEY_DOWN:
        break;
    case MKEY_LEFT:
        break;
    case MKEY_RIGHT:
        break;
    case MKEY_CANCEL:
        step = 0;
        GuiWinDeleteTop();
        GuiMenuRedrawMenu(&HmiMenu);
        break;
    case MKEY_OK:
        step = 0;
        break;
    }
}

/**
  *@brief  窗口0实体函数
  *@param  None
  *@retval None
  */
static void settingFun(void *param)
{
    //用户应用代码:画图等
    // GuiFillRect(20, 20, 88, 60, CWHITLE);
    /* 按键状态处理 */
    switch (keyStatus)
    {
    case MKEY_UP:
        break;
    case MKEY_DOWN:
        break;
    case MKEY_LEFT:
        break;
    case MKEY_RIGHT:
        break;
    case MKEY_CANCEL:
        GuiWinDeleteTop();
        GuiMenuRedrawMenu(&HmiMenu);
        break;
    case MKEY_OK:
        break;
    }
}
/**
  *@brief  窗口1实体函数
  *@param  None
  *@retval None
  */
static void Win1Fun(void *param)
{
    //用户应用代码:画图等
    /* 按键状态处理 */
    switch (keyStatus)
    {
    case MKEY_UP:
        break;
    case MKEY_DOWN:
        break;
    case MKEY_LEFT:
        break;
    case MKEY_RIGHT:
        break;
    case MKEY_CANCEL:
        GuiWinDeleteTop();
        GuiMenuRedrawMenu(&HmiMenu);
        break;
    case MKEY_OK:
        break;
    }
}
/**
  *@brief  窗口2实体函数
  *@param  None
  *@retval None
  */
static void stepAdjustFun(void *param)
{
    //用户应用代码:画图等
    GuiRowText(10, 24, 108, FONT_MID, "Step Adjust");
    sprintf(str, "   %d   ", (int32_t)g_Stat.targetTemperStepVal);
    GuiRowText(10, 40, 108, FONT_MID, (uint8_t *)str);

    /* 按键状态处理 */
    switch (keyStatus)
    {
    case MKEY_UP:
        g_Stat.targetTemperStepVal--;
        break;
    case MKEY_DOWN:
        g_Stat.targetTemperStepVal++;
        break;
    case MKEY_LEFT:
        g_Stat.targetTemperStepVal -= 10;
        break;
    case MKEY_RIGHT:
        g_Stat.targetTemperStepVal += 10;
        break;
    case MKEY_CANCEL:
        // TODO 此处添加保存参数功能
        BSP_FlashWrite(FLASH_SAVE_ADDR, (uint16_t *)&g_Stat, sizeof(g_Stat) / 2);
        GuiWinDeleteTop();
        GuiMenuRedrawMenu(&HmiMenu);
        break;
    case MKEY_OK:
        break;
    }
    g_Stat.targetTemperStepVal = (int8_t)BSP_LimitInt(g_Stat.targetTemperStepVal, 0, 100);
}

/**
  *@brief  窗口2实体函数
  *@param  None
  *@retval None
  */
static void defaultTemperFun(void *param)
{
    //用户应用代码:画图等
    GuiRowText(10, 24, 108, FONT_MID, "Default Temper");
    sprintf(str, "   %d   ", (int32_t)g_Stat.defaultTargetTemper);
    GuiRowText(10, 40, 108, FONT_MID, (uint8_t *)str);

    /* 按键状态处理 */
    switch (keyStatus)
    {
    case MKEY_UP:
        g_Stat.defaultTargetTemper--;
        break;
    case MKEY_DOWN:
        g_Stat.defaultTargetTemper++;
        break;
    case MKEY_LEFT:
        g_Stat.defaultTargetTemper -= 10;
        break;
    case MKEY_RIGHT:
        g_Stat.defaultTargetTemper += 10;
        break;
    case MKEY_CANCEL:
        // TODO 此处添加保存参数功能
        BSP_FlashWrite(FLASH_SAVE_ADDR, (uint16_t *)&g_Stat, sizeof(g_Stat) / 2);
        GuiWinDeleteTop();
        GuiMenuRedrawMenu(&HmiMenu);
        break;
    case MKEY_OK:
        break;
    }
    g_Stat.defaultTargetTemper = BSP_LimitFloat(g_Stat.defaultTargetTemper, 0, 500);
}

/**
  *@brief  窗口2实体函数
  *@param  None
  *@retval None
  */
static void sleepTemperFun(void *param)
{
    //用户应用代码:画图等
    GuiRowText(10, 24, 108, FONT_MID, "Sleep Temperature");
    sprintf(str, "   %d   ", (int32_t)g_Stat.sleepMode.temperature);
    GuiRowText(10, 40, 108, FONT_MID, (uint8_t *)str);

    /* 按键状态处理 */
    switch (keyStatus)
    {
    case MKEY_UP:
        g_Stat.sleepMode.temperature--;
        break;
    case MKEY_DOWN:
        g_Stat.sleepMode.temperature++;
        break;
    case MKEY_LEFT:
        g_Stat.sleepMode.temperature -= 10;
        break;
    case MKEY_RIGHT:
        g_Stat.sleepMode.temperature += 10;
        break;
    case MKEY_CANCEL:
        // TODO 此处添加保存参数功能
        BSP_FlashWrite(FLASH_SAVE_ADDR, (uint16_t *)&g_Stat, sizeof(g_Stat) / 2);
        GuiWinDeleteTop();
        GuiMenuRedrawMenu(&HmiMenu);
        break;
    case MKEY_OK:
        break;
    }
    g_Stat.sleepMode.temperature = BSP_LimitFloat(g_Stat.sleepMode.temperature, 0, 500);
}

/**
  *@brief  窗口2实体函数
  *@param  None
  *@retval None
  */
static void sleepTimeFun(void *param)
{
    uint32_t temp = 0;
    //用户应用代码:画图等
    GuiRowText(10, 24, 108, FONT_MID, "Sleep Time");
    temp = g_Stat.sleepMode.time;
    if (temp >= pdMS_TO_TICKS(60 * 60 * 1000))  // 大于1小时
    {
        sprintf(str, " %dh%dm%ds ", temp / pdMS_TO_TICKS(60 * 60 * 1000), (temp % pdMS_TO_TICKS(60 * 60 * 1000)) / pdMS_TO_TICKS(60 * 1000), (temp % pdMS_TO_TICKS(60 * 1000)) / pdMS_TO_TICKS(1000));
    }
    else if (temp >= pdMS_TO_TICKS(60 * 1000))  // 大于1分钟
    {
        sprintf(str, "    %dm%ds    ", (temp % pdMS_TO_TICKS(60 * 60 * 1000)) / pdMS_TO_TICKS(60 * 1000), (temp % pdMS_TO_TICKS(60 * 1000)) / pdMS_TO_TICKS(1000));
    }
    else  // 只显示秒
    {
        sprintf(str, "    %ds    ", (temp) / pdMS_TO_TICKS(1000));
    }
    GuiRowText(10, 40, 108, FONT_MID, (uint8_t *)str);

    /* 按键状态处理 */
    switch (keyStatus)
    {
    case MKEY_UP:
        g_Stat.sleepMode.time -= pdMS_TO_TICKS(1000);  // 1000ms
        break;
    case MKEY_DOWN:
        g_Stat.sleepMode.time += pdMS_TO_TICKS(1000);
        break;
    case MKEY_LEFT:
        g_Stat.sleepMode.time -= pdMS_TO_TICKS(10 * 1000);  // 10s
        break;
    case MKEY_RIGHT:
        g_Stat.sleepMode.time += pdMS_TO_TICKS(10 * 1000);  // 10s
        break;
    case MKEY_CANCEL:
        // TODO 此处添加保存参数功能
        BSP_FlashWrite(FLASH_SAVE_ADDR, (uint16_t *)&g_Stat, sizeof(g_Stat) / 2);
        GuiWinDeleteTop();
        GuiMenuRedrawMenu(&HmiMenu);
        break;
    case MKEY_OK:
        break;
    }
    // 2s~3hour
    if (g_Stat.sleepMode.time < pdMS_TO_TICKS(20 * 1000))
    {
        g_Stat.sleepMode.time = pdMS_TO_TICKS(20 * 1000);
    }
}

/**
  *@brief  窗口2实体函数
  *@param  None
  *@retval None
  */
static void powerOffTimeFun(void *param)
{
    uint32_t temp = 0;
    //用户应用代码:画图等
    GuiRowText(10, 24, 108, FONT_MID, "Power Off Time");
    temp = g_Stat.shutdownMode.time;
    if (temp >= pdMS_TO_TICKS(60 * 60 * 1000))  // 大于1小时
    {
        sprintf(str, " %dh%dm%ds ", temp / pdMS_TO_TICKS(60 * 60 * 1000), (temp % pdMS_TO_TICKS(60 * 60 * 1000)) / pdMS_TO_TICKS(60 * 1000), (temp % pdMS_TO_TICKS(60 * 1000)) / pdMS_TO_TICKS(1000));
    }
    else if (temp >= pdMS_TO_TICKS(60 * 1000))  // 大于1分钟
    {
        sprintf(str, "    %dm%ds    ", (temp % pdMS_TO_TICKS(60 * 60 * 1000)) / pdMS_TO_TICKS(60 * 1000), (temp % pdMS_TO_TICKS(60 * 1000)) / pdMS_TO_TICKS(1000));
    }
    else  // 只显示秒
    {
        sprintf(str, "    %ds    ", (temp) / pdMS_TO_TICKS(1000));
    }
    GuiRowText(10, 40, 108, FONT_MID, (uint8_t *)str);

    /* 按键状态处理 */
    switch (keyStatus)
    {
    case MKEY_UP:
        g_Stat.shutdownMode.time -= pdMS_TO_TICKS(1000);  // 1000ms
        break;
    case MKEY_DOWN:
        g_Stat.shutdownMode.time += pdMS_TO_TICKS(1000);
        break;
    case MKEY_LEFT:
        g_Stat.shutdownMode.time -= pdMS_TO_TICKS(10 * 1000);  // 10s
        break;
    case MKEY_RIGHT:
        g_Stat.shutdownMode.time += pdMS_TO_TICKS(10 * 1000);  // 10s
        break;
    case MKEY_CANCEL:
        // TODO 此处添加保存参数功能
        BSP_FlashWrite(FLASH_SAVE_ADDR, (uint16_t *)&g_Stat, sizeof(g_Stat) / 2);
        GuiWinDeleteTop();
        GuiMenuRedrawMenu(&HmiMenu);
        break;
    case MKEY_OK:
        break;
    }
    // 2s~3hour
    if (g_Stat.shutdownMode.time < pdMS_TO_TICKS(20 * 1000))
    {
        g_Stat.shutdownMode.time = pdMS_TO_TICKS(20 * 1000);
    }
}

/**
  *@brief  窗口2实体函数
  *@param  None
  *@retval None
  */
static void volumeAdjustFun(void *param)
{
    int32_t temp = 0;
    //用户应用代码:画图等
    GuiRowText(10, 24, 108, FONT_MID, "Volume");
    temp = g_Stat.beep.volume;
    sprintf(str, "   %d   ", temp);
    GuiRowText(10, 40, 108, FONT_MID, (uint8_t *)str);

    /* 按键状态处理 */
    switch (keyStatus)
    {
    case MKEY_UP:
        temp -= 1;  // 1000ms
        break;
    case MKEY_DOWN:
        temp += 1;
        break;
    case MKEY_LEFT:
        temp -= 10;  // 10s
        break;
    case MKEY_RIGHT:
        temp += 10;  // 10s
        break;
    case MKEY_CANCEL:
        // TODO 此处添加保存参数功能
        BSP_FlashWrite(FLASH_SAVE_ADDR, (uint16_t *)&g_Stat, sizeof(g_Stat) / 2);
        GuiWinDeleteTop();
        GuiMenuRedrawMenu(&HmiMenu);
        break;
    case MKEY_OK:
        break;
    }
    g_Stat.beep.volume = (uint8_t)BSP_LimitInt(temp, 0, 100);
}

/**
  *@brief  窗口2实体函数
  *@param  None
  *@retval None
  */
static void vinVoltRateFun(void *param)
{
    //用户应用代码:画图等
    GuiRowText(10, 24, 108, FONT_MID, "Vin Volt Rate");
    sprintf(str, "    %.2f    ", g_Stat.vinVoltRate);
    GuiRowText(10, 40, 108, FONT_MID, (uint8_t *)str);
    sprintf(str, "  Vin:%.2f  ", g_Stat.vinVolt);
    GuiRowText(10, 54, 108, FONT_MID, (uint8_t *)str);

    /* 按键状态处理 */
    switch (keyStatus)
    {
    case MKEY_UP:
        g_Stat.vinVoltRate -= 0.01;
        break;
    case MKEY_DOWN:
        g_Stat.vinVoltRate += 0.01;
        break;
    case MKEY_LEFT:
        g_Stat.vinVoltRate -= 0.1;
        break;
    case MKEY_RIGHT:
        g_Stat.vinVoltRate += 0.1;
        break;
    case MKEY_CANCEL:
        // TODO 此处添加保存参数功能
        BSP_FlashWrite(FLASH_SAVE_ADDR, (uint16_t *)&g_Stat, sizeof(g_Stat) / 2);
        GuiWinDeleteTop();
        GuiMenuRedrawMenu(&HmiMenu);
        break;
    case MKEY_OK:
        break;
    }
    // 2s~3hour
    if (g_Stat.shutdownMode.time < pdMS_TO_TICKS(20 * 1000))
    {
        g_Stat.shutdownMode.time = pdMS_TO_TICKS(20 * 1000);
    }
}

/**
  *@brief  用户菜单主函数
  *@param  None
  *@retval None
  */
void userAppMain(void)
{
    if (g_Stat.keyUpdate == true)
    {
        switch (g_Stat.keyVal)
        {
        case eKeyVal_Up:
            keyStatus = MKEY_UP;
            break;
        case eKeyVal_Down:
            keyStatus = MKEY_DOWN;
            break;
        case eKeyVal_Sub:
            keyStatus = MKEY_LEFT;
            break;
        case eKeyVal_Add:
            keyStatus = MKEY_RIGHT;
            break;
        case eKeyVal_Long:
            keyStatus = MKEY_HOME;
            GuiWinHome();
            GuiMenuReset(&HmiMenu);
            // TODO: 此处实现长按返回主界面功能
            break;
        case eKeyVal_Single:
            keyStatus = MKEY_OK;
            break;
        case eKeyVal_Double:
            keyStatus = MKEY_CANCEL;
            break;
        default:
            break;
        }
        g_Stat.keyUpdate = false;
    }
    else
    {
        keyStatus = MKEY_NO;
    }
    GuiWinDisplay();
}

/**
  *@brief  用户应用初始化
  *@param  None
  *@retval None
  */
void userAppInit(void)
{
    UserMenuInit();
    GuiSetForecolor(1);
    GuiSetbackcolor(0);
    GuiWinAdd(&homeWin);
}

/* END */

void startUI(void)
{
    extern void defaultFontInit(void);
    defaultFontInit(); /* 字库初始化 */
    userAppInit();
}

void displayUI(void)
{
    userAppMain();
    g_Stat.dataUpdate = false;  // 一定要放在最后
}

void sendKey2UI(int32_t key)
{
    g_Stat.keyVal = key;
    g_Stat.keyUpdate = true;
    g_Stat.dataUpdate = true;
}
