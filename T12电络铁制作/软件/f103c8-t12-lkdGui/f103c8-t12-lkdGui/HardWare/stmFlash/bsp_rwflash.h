/**
 ******************************************************************************
 * @file    stmflash.h
 * @author  Zxp
 * @version V1.0.1
 * @date    2021-01-25
 * @brief   本驱动可以将stm32的内部FLASH配置为EEPROM来存储数据
 ******************************************************************************
 */

#ifndef __BSP_RW_FLASH_H_
#define __BSP_RW_FLASH_H_

#include "stm32f10x.h"

/******************************* Start 用户修改区 *******************************/
#define FLASH_VERSION_ADDR 0X0800E000
#define FLASH_SAVE_ADDR 0X0800E010
#define STM32_FLASH_SIZE 64  //所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN 1   //使能FLASH写入(0，不是能;1，使能)
/******************************** End 用户修改区 ********************************/

void BSP_FlashUnlock(void);
void BSP_FlashLock(void);
uint8_t BSP_FlashGetStatus(void);
uint8_t BSP_FlashWaitDone(uint16_t time);
uint8_t BSP_FlashErasePage(uint32_t paddr);
uint8_t BSP_FlashWriteHalfWord(uint32_t faddr, uint16_t dat);
uint16_t BSP_FlashReadHalfWord(uint32_t faddr);

#if STM32_FLASH_WREN  //如果使能了写
void BSP_FlashWrite_NoCheck(uint32_t WriteAddr, uint16_t *pBuffer, uint16_t NumToWrite);
void BSP_FlashWrite(uint32_t WriteAddr, uint16_t *pBuffer, uint16_t NumToWrite);
#endif
void BSP_FlashRead(uint32_t ReadAddr, uint16_t *pBuffer, uint16_t NumToRead);

#endif  // __BSP_RW_FLASH_H_
