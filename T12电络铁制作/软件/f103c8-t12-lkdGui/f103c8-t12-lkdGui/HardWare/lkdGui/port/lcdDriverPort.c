/**
  * @file   lcdDriverPort.c
  * @author  guoweilkd
  * @version V0.0.0
  * @date    2018/04/18
  * @brief  lkdGuiҺ����ʾ�������ӿڣ�����ֲ����亯���ľ������ݡ�
  */

#include "bsp_ssdoled.h"
#include "lkdGui.h"

/**
  *@brief ��Gui������������ʾ��lcd��
  *@param  None
  *@retval None
  */
void GuiUpdateDisplayAll(void)
{
    /* ����û����� */
}

/**
  *@brief ��Guiָ��������������ʾ��lcd��
  *@param  beginx,beginy,endx,endy ����
  *@retval None
  */
void GuiRangeUpdateDisplay(lkdCoord beginx, lkdCoord beginy, lkdCoord endx, lkdCoord endy)
{
    /* ����û����� */
}

/**
  *@brief �򻺳�������
  *@param  x,y ����
  *@param  color ��ɫ <Ŀǰֻ�к�:CBLACK ��:CWHITLE>
  *@retval None
  */
void GuiDrawPoint(lkdCoord x, lkdCoord y, lkdColour color)
{
    /* ����û����� */
    BSP_OLEDSetPixel(x, y, color);
}

/**
  *@brief ��ȡ��ǰ��ʾ�ĵ�
  *@param  x,y ����
  *@param  color ��ɫ <Ŀǰֻ�к�:CBLACK ��:CWHITLE>
  *@retval None
  */
void GuiReadPoint(lkdCoord x, lkdCoord y, lkdColour *pColor)
{
    /* ����û����� */
    *pColor = BSP_OLEDGetPixel(x, y);
}

/**
  *@brief ����ʾ
  *@param  None
  *@retval None
  */
void CloseLcdDisplay(void)
{
    /* ����û����� */
    BSP_OLEDSetOff();
}

/**
  *@brief ����ʾ
  *@param  None
  *@retval None
  */
void OpenLcdDisplay(void)
{
    /* ����û����� */
    BSP_OLEDSetOn();
}
