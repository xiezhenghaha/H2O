/**
  * @file   lcdDriverPort.c
  * @author  guoweilkd
  * @version V0.0.0
  * @date    2018/04/18
  * @brief  lkdGui液晶显示屏驱动接口，由移植者填充函数的具体内容。
  */

#include "bsp_ssdoled.h"
#include "lkdGui.h"

/**
  *@brief 将Gui缓冲区数据显示到lcd上
  *@param  None
  *@retval None
  */
void GuiUpdateDisplayAll(void)
{
    /* 添加用户代码 */
}

/**
  *@brief 将Gui指定缓冲区数据显示到lcd上
  *@param  beginx,beginy,endx,endy 坐标
  *@retval None
  */
void GuiRangeUpdateDisplay(lkdCoord beginx, lkdCoord beginy, lkdCoord endx, lkdCoord endy)
{
    /* 添加用户代码 */
}

/**
  *@brief 向缓冲区画点
  *@param  x,y 坐标
  *@param  color 颜色 <目前只有黑:CBLACK 白:CWHITLE>
  *@retval None
  */
void GuiDrawPoint(lkdCoord x, lkdCoord y, lkdColour color)
{
    /* 添加用户代码 */
    BSP_OLEDSetPixel(x, y, color);
}

/**
  *@brief 读取当前显示的点
  *@param  x,y 坐标
  *@param  color 颜色 <目前只有黑:CBLACK 白:CWHITLE>
  *@retval None
  */
void GuiReadPoint(lkdCoord x, lkdCoord y, lkdColour *pColor)
{
    /* 添加用户代码 */
    *pColor = BSP_OLEDGetPixel(x, y);
}

/**
  *@brief 关显示
  *@param  None
  *@retval None
  */
void CloseLcdDisplay(void)
{
    /* 添加用户代码 */
    BSP_OLEDSetOff();
}

/**
  *@brief 开显示
  *@param  None
  *@retval None
  */
void OpenLcdDisplay(void)
{
    /* 添加用户代码 */
    BSP_OLEDSetOn();
}
