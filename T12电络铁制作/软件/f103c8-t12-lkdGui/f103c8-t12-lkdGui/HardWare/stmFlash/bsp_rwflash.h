/**
 ******************************************************************************
 * @file    stmflash.h
 * @author  Zxp
 * @version V1.0.1
 * @date    2021-01-25
 * @brief   ���������Խ�stm32���ڲ�FLASH����ΪEEPROM���洢����
 ******************************************************************************
 */

#ifndef __BSP_RW_FLASH_H_
#define __BSP_RW_FLASH_H_

#include "stm32f10x.h"

/******************************* Start �û��޸��� *******************************/
#define FLASH_VERSION_ADDR 0X0800E000
#define FLASH_SAVE_ADDR 0X0800E010
#define STM32_FLASH_SIZE 64  //��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN 1   //ʹ��FLASHд��(0��������;1��ʹ��)
/******************************** End �û��޸��� ********************************/

void BSP_FlashUnlock(void);
void BSP_FlashLock(void);
uint8_t BSP_FlashGetStatus(void);
uint8_t BSP_FlashWaitDone(uint16_t time);
uint8_t BSP_FlashErasePage(uint32_t paddr);
uint8_t BSP_FlashWriteHalfWord(uint32_t faddr, uint16_t dat);
uint16_t BSP_FlashReadHalfWord(uint32_t faddr);

#if STM32_FLASH_WREN  //���ʹ����д
void BSP_FlashWrite_NoCheck(uint32_t WriteAddr, uint16_t *pBuffer, uint16_t NumToWrite);
void BSP_FlashWrite(uint32_t WriteAddr, uint16_t *pBuffer, uint16_t NumToWrite);
#endif
void BSP_FlashRead(uint32_t ReadAddr, uint16_t *pBuffer, uint16_t NumToRead);

#endif  // __BSP_RW_FLASH_H_
