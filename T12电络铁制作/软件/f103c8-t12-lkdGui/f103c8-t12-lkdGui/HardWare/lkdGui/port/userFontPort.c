/**
  * @file   userFontPort.c
  * @author  guoweilkd
  * @version V0.0.0
  * @date    2018/04/18
  * @brief  lkdGui���������ӿڣ�����ֲ����亯���ľ������ݡ�
  */

#include "lkdGui.h"

/* �������ʵ�� */
lkdFont defaultFont;
lkdFont selfFont;
lkdFont gb2312_16x16Font;
/* ��buff�Ĵ�С�������ģ��С���� */
uint8_t dataBuff[12 * 2];
uint8_t selfDataBuff[32 * 2];

extern uint8_t GetFontASCII_5_7(uint8_t code1, uint8_t *pBuff);
extern uint8_t GetFontASCII_6_12(uint8_t code1, uint8_t *pBuff);
extern uint8_t GetFontASCII_16_32(uint8_t code1, uint8_t *pBuff);
extern uint8_t GetFontGb2312_12_12(uint8_t codeH, uint8_t codeL, uint8_t *pBuff);
extern uint8_t GetFontGb2312_16_16(uint8_t codeH, uint8_t codeL, uint8_t *pBuff);

/**
  *@brief ��ȡ������ģ
  *@param  code1,code2 ���ֱ���[H,L] 
  *@param  pBuff ��ģbuff
  *@retval 0
  */
static uint8_t GetDfontData(uint8_t code1, uint8_t code2, uint8_t *pBuff)
{
    /* ����û����� */
    GetFontGb2312_12_12(code1, code2, pBuff);
    return 0;
}

/**
  *@brief ��ȡASCII��ģ
  *@param  code1, ���� 
  *@param  pBuff ��ģbuff
  *@retval 0
  */
static uint8_t GetSfontData(uint8_t code1, uint8_t *pBuff)
{
    /* ����û����� */

    return GetFontASCII_6_12(code1, pBuff);
}

/**
  *@brief �����ʼ��
  *@param  None
  *@retval None
  */
void defaultFontInit(void)
{
    /* ��������Ҫ������Ӧ���޸� */

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

    /* ����ΪϵͳĬ������ */
    GuiFontSet(&defaultFont);
    GuiSetbackcolor(CWHITLE);
    GuiSetForecolor(CBLACK);
}
