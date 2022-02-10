/**
 ******************************************************************************
 * @file    bsp_ssdoled.h
 * @author  Zxp
 * @version V1.0.1
 * @date    2021-06-16
 * @brief   使用DMA驱动SPI刷新OLED屏幕的头文件
 * @history Version     Date        Author      Note
 *          V1.0.0      2021-06-16  Zxp         创建文件
 *          V1.0.1      2021-06-17  Zxp         添加BSP_OLEDFill函数;修改BSP_OLEDShowString参数列表;修改BSP_OLEDShowChar和BSP_OLEDDrawPoint的参数类型
 *          V1.0.2      2021-06-17  Zxp         添加BSP_OLEDGetPixel函数;修改BSP_OLEDDrawPoint函数名,改为BSP_OLEDSetPixel;修改几个函数中坐标x和y的类型为int32_t
 ******************************************************************************
 */

#include "stm32f10x.h"

#define OLED_DISPLAY_WIDTH (128)  // 屏幕宽度
#define OLED_DISPLAY_HEIGHT (64)  // 屏幕高度
#define OLED_BUFFER_X (OLED_DISPLAY_HEIGHT >> 3)
#define OLED_BUFFER_Y (OLED_DISPLAY_WIDTH)

extern uint8_t DispBuff[OLED_BUFFER_X][OLED_BUFFER_Y];

void BSP_OLEDReset(void);
void BSP_OLEDWriteCmd(uint8_t data);
void BSP_OLEDWriteData(uint8_t data);
void BSP_OLEDClear(void);
void BSP_OLEDFill(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color);
void BSP_InitOLED(void);
void BSP_OLEDSetOn(void);
void BSP_OLEDSetOff(void);
void BSP_Refresh(void);
void BSP_OLEDSetPixel(int32_t x, int32_t y, int32_t point);
int32_t BSP_OLEDGetPixel(int32_t x, int32_t y);
void BSP_OLEDShowChar(int32_t x, int32_t y, uint8_t chr, uint8_t size, uint8_t inverseColor);
void BSP_OLEDShowString(int32_t x, int32_t y, const uint8_t *str, uint8_t size, uint8_t inverseColor);
