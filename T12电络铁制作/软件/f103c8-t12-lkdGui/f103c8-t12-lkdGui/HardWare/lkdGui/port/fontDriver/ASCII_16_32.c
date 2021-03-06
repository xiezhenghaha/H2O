#include <stdint.h>

#define ASCII_NUM 10
#define ASCII_EACH_SIZE 64
/* 
 * ASCII 6_12 字模库.去除了前32个控制字符。
 * 扫描方式：由左到右，由上到下。宽度按字节对齐。
 * 存储方式: 高位在前。
*/
const unsigned char ASCII_16_32[ASCII_NUM][ASCII_EACH_SIZE] = {
    // 0(0)1(1)2(2)3(3)4(4)5(5)6(6)7(7)8(8)9(9)

    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x07, 0xE0, 0x0F, 0xF0, 0x1C, 0x78, 0x38, 0x38, 0x38, 0x1C, 0x38, 0x1C, 0x70, 0x1C, 0x70, 0x0C, 0x70, 0x0E, 0x70, 0x0E, 0x70, 0x0E, 0x70, 0x0E, 0x70, 0x0E, 0x70, 0x0E, 0x70, 0x0E, 0x70, 0x0C, 0x70, 0x1C, 0x38, 0x1C, 0x38, 0x1C, 0x38, 0x38, 0x1C, 0x38, 0x0F, 0xF0, 0x0F, 0xE0, 0x07, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, /*"0",0*/
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0xC0, 0x01, 0xC0, 0x03, 0xC0, 0x1F, 0xC0, 0x1F, 0xC0, 0x01, 0xC0, 0x01, 0xC0, 0x01, 0xC0, 0x01, 0xC0, 0x01, 0xC0, 0x01, 0xC0, 0x01, 0xC0, 0x01, 0xC0, 0x01, 0xC0, 0x01, 0xC0, 0x01, 0xC0, 0x01, 0xC0, 0x01, 0xC0, 0x01, 0xC0, 0x01, 0xC0, 0x01, 0xC0, 0x01, 0xC0, 0x01, 0xC0, 0x01, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, /*"1",1*/
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x07, 0xF0, 0x0F, 0xF8, 0x1C, 0x38, 0x18, 0x1C, 0x38, 0x1C, 0x38, 0x1C, 0x38, 0x1C, 0x00, 0x1C, 0x00, 0x38, 0x00, 0x38, 0x00, 0x78, 0x00, 0x70, 0x00, 0xE0, 0x01, 0xC0, 0x03, 0xC0, 0x07, 0x80, 0x07, 0x00, 0x0E, 0x00, 0x1C, 0x00, 0x1C, 0x00, 0x38, 0x00, 0x3F, 0xFC, 0x7F, 0xFC, 0x7F, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, /*"2",2*/
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x0F, 0xE0, 0x1F, 0xF0, 0x1C, 0x38, 0x38, 0x38, 0x38, 0x1C, 0x38, 0x1C, 0x00, 0x1C, 0x00, 0x18, 0x00, 0x38, 0x00, 0x70, 0x01, 0xE0, 0x01, 0xE0, 0x00, 0x70, 0x00, 0x38, 0x00, 0x1C, 0x00, 0x1C, 0x70, 0x1C, 0x70, 0x1C, 0x30, 0x1C, 0x38, 0x1C, 0x38, 0x38, 0x1F, 0xF8, 0x0F, 0xF0, 0x07, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, /*"3",3*/
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x70, 0x00, 0xF0, 0x00, 0xF0, 0x01, 0xF0, 0x01, 0xF0, 0x03, 0xF0, 0x03, 0x70, 0x07, 0x70, 0x06, 0x70, 0x0E, 0x70, 0x0C, 0x70, 0x1C, 0x70, 0x18, 0x70, 0x38, 0x70, 0x30, 0x70, 0x70, 0x70, 0x7F, 0xFE, 0x7F, 0xFE, 0x7F, 0xFE, 0x00, 0x70, 0x00, 0x70, 0x00, 0x70, 0x00, 0x70, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, /*"4",4*/
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xFC, 0x3F, 0xFC, 0x3F, 0xFC, 0x38, 0x00, 0x38, 0x00, 0x38, 0x00, 0x38, 0x00, 0x38, 0x00, 0x38, 0x00, 0x39, 0xE0, 0x3F, 0xF0, 0x3E, 0x38, 0x38, 0x38, 0x38, 0x1C, 0x00, 0x1C, 0x00, 0x1C, 0x00, 0x1C, 0x00, 0x1C, 0x70, 0x1C, 0x30, 0x1C, 0x38, 0x38, 0x38, 0x38, 0x1F, 0xF0, 0x0F, 0xE0, 0x07, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, /*"5",5*/
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x03, 0xF0, 0x07, 0xF8, 0x0E, 0x38, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x00, 0x38, 0x00, 0x38, 0x00, 0x39, 0xE0, 0x3B, 0xF0, 0x3E, 0x78, 0x3C, 0x1C, 0x38, 0x1C, 0x38, 0x1C, 0x38, 0x1C, 0x38, 0x1C, 0x38, 0x1C, 0x38, 0x1C, 0x18, 0x1C, 0x1C, 0x1C, 0x1C, 0x38, 0x0F, 0xF8, 0x07, 0xF0, 0x03, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, /*"6",6*/
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xFC, 0x3F, 0xFC, 0x3F, 0xFC, 0x00, 0x1C, 0x00, 0x18, 0x00, 0x18, 0x00, 0x38, 0x00, 0x38, 0x00, 0x30, 0x00, 0x30, 0x00, 0x70, 0x00, 0x70, 0x00, 0x60, 0x00, 0xE0, 0x00, 0xE0, 0x00, 0xE0, 0x00, 0xC0, 0x01, 0xC0, 0x01, 0xC0, 0x01, 0xC0, 0x03, 0x80, 0x03, 0x80, 0x03, 0x80, 0x03, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, /*"7",7*/
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x0F, 0xE0, 0x1F, 0xF0, 0x1C, 0x38, 0x38, 0x18, 0x38, 0x1C, 0x38, 0x1C, 0x38, 0x1C, 0x38, 0x1C, 0x18, 0x18, 0x1C, 0x38, 0x0F, 0xF0, 0x07, 0xE0, 0x1E, 0x70, 0x18, 0x38, 0x38, 0x1C, 0x30, 0x1C, 0x70, 0x1C, 0x70, 0x1C, 0x30, 0x1C, 0x38, 0x1C, 0x3C, 0x3C, 0x1F, 0xF8, 0x0F, 0xF0, 0x07, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, /*"8",8*/
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0x0F, 0xE0, 0x1F, 0xF0, 0x1C, 0x70, 0x38, 0x38, 0x38, 0x18, 0x38, 0x1C, 0x38, 0x1C, 0x38, 0x1C, 0x38, 0x1C, 0x38, 0x1C, 0x38, 0x1C, 0x38, 0x3C, 0x1C, 0x7C, 0x1F, 0xFC, 0x0F, 0xDC, 0x03, 0x1C, 0x00, 0x1C, 0x00, 0x38, 0x38, 0x38, 0x38, 0x38, 0x3C, 0x70, 0x1F, 0xE0, 0x0F, 0xE0, 0x07, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, /*"9",9*/

};

uint8_t GetFontASCII_16_32(uint8_t code1, uint8_t *pBuff)
{
    uint16_t i, offset = code1 - '0';

    if (offset > ASCII_NUM)
    {
        for (i = 0; i < ASCII_EACH_SIZE; i++)
        {
            pBuff[i] = 0;
        }
    }
    else
    {
        for (i = 0; i < ASCII_EACH_SIZE; i++)
        {
            pBuff[i] = ASCII_16_32[offset][i];
        }
    }

    return 0;
}

/* END */
