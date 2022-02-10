/**
  * @file   lkdGuiColour.c
  * @author  guoweilkd
  * @version V0.0.0
  * @date    2018/04/18
  * @brief  ��ɫ���� Ŀǰֻ�кڰ�
  */

#include "lkdGui.h"

/* ����ǰ��ɫ/����ɫ */
static lkdColour forecolor = CBLACK,backcolor = CWHITLE;

/**
  *@brief ��ȡǰ��ɫ
  *@param  None
  *@retval ����ǰ��ɫ
  */
lkdColour GuiGetForecolor(void)
{
	return forecolor;
}

/**
  *@brief ����ǰ��ɫ
  *@param  colour ǰ��ɫ
  *@retval None
  */
void GuiSetForecolor(lkdColour colour)
{
	forecolor = colour;
}

/**
  *@brief ��ȡ����ɫ
  *@param  None
  *@retval ���ر���ɫ
  */
lkdColour GuiGetbackcolor(void)
{
	return backcolor;
}
/**
  *@brief ���ñ���ɫ
  *@param  colour ����ɫ
  *@retval None
  */
void GuiSetbackcolor(lkdColour colour)
{
	backcolor = colour;
}

/**
  *@brief ����ǰ��ɫ�ͱ���ɫ
  *@param  None
  *@retval None
  */
void GuiExchangeColor(void)
{
	lkdColour tColour;
	tColour = forecolor;
	forecolor = backcolor;
	backcolor = tColour;
}

/* END */
