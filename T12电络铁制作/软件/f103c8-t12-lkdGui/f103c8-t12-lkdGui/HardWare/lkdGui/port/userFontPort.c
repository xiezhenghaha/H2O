/**
  * @file   userFontPort.c
  * @author  guoweilkd
  * @version V0.0.0
  * @date    2018/04/18
  * @brief  lkdGui字体驱动接口，由移植者填充函数的具体内容。
  */

#include "lkdGui.h"

/* 字体控制实体 */
lkdFont defaultFont;
lkdFont selfFont;
lkdFont gb2312_16x16Font;
/* 此buff的大小由最大字模大小决定 */
uint8_t dataBuff[12 * 2];
uint8_t selfDataBuff[32 * 2];

extern uint8_t GetFontASCII_5_7(uint8_t code1, uint8_t *pBuff);
extern uint8_t GetFontASCII_6_12(uint8_t code1, uint8_t *pBuff);
extern uint8_t GetFontASCII_16_32(uint8_t code1, uint8_t *pBuff);
extern uint8_t GetFontGb2312_12_12(uint8_t codeH, uint8_t codeL, uint8_t *pBuff);
extern uint8_t GetFontGb2312_16_16(uint8_t codeH, uint8_t codeL, uint8_t *pBuff);

/**
  *@brief 获取汉字字模
  *@param  code1,code2 汉字编码[H,L] 
  *@param  pBuff 字模buff
  *@retval 0
  */
static uint8_t GetDfontData(uint8_t code1, uint8_t code2, uint8_t *pBuff)
{
    /* 添加用户代码 */
    GetFontGb2312_12_12(code1, code2, pBuff);
    return 0;
}

/**
  *@brief 获取ASCII字模
  *@param  code1, 编码 
  *@param  pBuff 字模buff
  *@retval 0
  */
static uint8_t GetSfontData(uint8_t code1, uint8_t *pBuff)
{
    /* 添加用户代码 */

    return GetFontASCII_6_12(code1, pBuff);
}

/**
  *@brief 字体初始化
  *@param  None
  *@retval None
  */
void defaultFontInit(void)
{
    /* 根据字体要求做相应的修改 */

    selfFont.name = "ASCII-Num[16*32]";
    selfFont.dhigh = 0;
    selfFont.dwide = 0;
    selfFont.shigh = 32;
    selfFont.swide = 16;
    selfFont.pZmBuff = selfDataBuff;
    selfFont.getDfont = GetDfontData;
    selfFont.getSfont = GetFontASCII_16_32;

    gb2312_16x16Font.name = "GB2312-Chinese[16*16]";
    gb2312_16x16Font.dhigh = 16;
    gb2312_16x16Font.dwide = 16;
    gb2312_16x16Font.shigh = 0;
    gb2312_16x16Font.swide = 0;
    gb2312_16x16Font.pZmBuff = selfDataBuff;
    gb2312_16x16Font.getDfont = GetFontGb2312_16_16;
    gb2312_16x16Font.getSfont = GetSfontData;

    defaultFont.name = "GB2312[12*12];ASCII[5*7]";
    defaultFont.dhigh = 12;
    defaultFont.dwide = 12;
    defaultFont.shigh = 12;
    defaultFont.swide = 6;
    defaultFont.pZmBuff = dataBuff;
    defaultFont.getDfont = GetDfontData;
    defaultFont.getSfont = GetSfontData;

    /* 设置为系统默认字体 */
    GuiFontSet(&defaultFont);
    GuiSetbackcolor(CWHITLE);
    GuiSetForecolor(CBLACK);
}
