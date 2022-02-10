/**
 ******************************************************************************
 * @file    stmflash.c
 * @author  Zxp
 * @version V1.0.0
 * @date    2021-01-25
 * @brief   将stm32内部FLASH作为EEPROM存储数据
 ******************************************************************************
 */

#include "bsp_rwflash.h"

// FLASH起始地址
#define STM32_FLASH_BASE 0x08000000  // STM32 FLASH的起始地址
// FLASH解锁键值
#define FLASH_KEY1 0X45670123
#define FLASH_KEY2 0XCDEF89AB

void Delay_us(uint32_t delay)
{
    uint32_t i;
    for (i = 0; i < 72; i++)
    {
    }
}

/**
 * @brief   解锁STM32的FLASH
 * @param   None
 * @retval  None
 */
void BSP_FlashUnlock(void)
{
    FLASH->KEYR = FLASH_KEY1;  //写入解锁序列.
    FLASH->KEYR = FLASH_KEY2;
}

/**
 * @brief   锁定FLASH
 * @param   None
 * @retval  None
 */
void BSP_FlashLock(void)
{
    FLASH->CR |= 1 << 7;  //上锁
}

/**
 * @brief   获取当前FLASH的状态
 * @param   None
 * @retval  状态: 0表示可以操作;1表示忙;2表示错误;3表示写保护
 */
uint8_t BSP_FlashGetStatus(void)
{
    uint32_t res;
    res = FLASH->SR;
    if (res & (1 << 0))
        return 1;  //忙
    else if (res & (1 << 2))
        return 2;  //编程错误
    else if (res & (1 << 4))
        return 3;  //写保护错误
    return 0;      //操作完成
}

/**
 * @brief   等待操作完成
 * @param   time: 要延时的长短
 * @retval  状态: 0表示可以操作;1表示忙;2表示错误;3表示写保护;0xff表示等待超时
 */
uint8_t BSP_FlashWaitDone(uint16_t time)
{
    uint8_t res;
    do
    {
        res = BSP_FlashGetStatus();
        if (res != 1)
            break;  //非忙,无需等待了,直接退出.
        Delay_us(1);
        time--;
    } while (time);
    if (time == 0)
        res = 0xff;  // TIMEOUT
    return res;
}

/**
 * @brief   擦除页
 * @param   paddr: 页地址
 * @retval  执行情况
 */
uint8_t BSP_FlashErasePage(uint32_t paddr)
{
    uint8_t res = 0;
    res = BSP_FlashWaitDone(0X5FFF);  //等待上次操作结束,>20ms
    if (res == 0)
    {
        FLASH->CR |= 1 << 1;              //页擦除
        FLASH->AR = paddr;                //设置页地址
        FLASH->CR |= 1 << 6;              //开始擦除
        res = BSP_FlashWaitDone(0X5FFF);  //等待操作结束,>20ms
        if (res != 1)                     //非忙
        {
            FLASH->CR &= ~(1 << 1);  //清除页擦除标志.
        }
    }
    return res;
}

/**
 * @brief   在FLASH指定地址写入半字
 * @param   faddr:指定地址(此地址必须为2的倍数!!)
 * @param   dat: 要写入的数据
 * @retval  写入的情况
 */
uint8_t BSP_FlashWriteHalfWord(uint32_t faddr, uint16_t dat)
{
    uint8_t res;
    res = BSP_FlashWaitDone(0XFF);
    if (res == 0)  // OK
    {
        FLASH->CR |= 1 << 0;            //编程使能
        *(vu16 *)faddr = dat;           //写入数据
        res = BSP_FlashWaitDone(0XFF);  //等待操作完成
        if (res != 1)                   //操作成功
        {
            FLASH->CR &= ~(1 << 0);  //清除PG位.
        }
    }
    return res;
}

/**
 * @brief   读取指定地址的半字(16位数据)
 * @param   faddr:读地址
 * @retval  返回值:对应数据.
 */
uint16_t BSP_FlashReadHalfWord(uint32_t faddr)
{
    return *(vu16 *)faddr;
}
#if STM32_FLASH_WREN  //如果使能了写

/**
 * @brief   不检查的写入
 * @param   WriteAddr: 起始地址
 * @param   pBuffer: 数据指针
 * @param   NumToWrite: 半字(16位)数
 * @retval  None
 */
void BSP_FlashWrite_NoCheck(uint32_t WriteAddr, uint16_t *pBuffer, uint16_t NumToWrite)
{
    uint16_t i;
    for (i = 0; i < NumToWrite; i++)
    {
        BSP_FlashWriteHalfWord(WriteAddr, pBuffer[i]);
        WriteAddr += 2;  //地址增加2.
    }
}

/**
 * @brief   从指定地址开始写入指定长度的数据
 * @param   WriteAddr: 起始地址(此地址必须为2的倍数!!)
 * @param   pBuffer: 数据指针
 * @param   NumToWrite: 半字(16位)数(就是要写入的16位数据的个数.)
 * @retval  None
 */
#if STM32_FLASH_SIZE < 256
#define STM_SECTOR_SIZE 1024  //字节
#else
#define STM_SECTOR_SIZE 2048
#endif
uint16_t STMFLASH_BUF[STM_SECTOR_SIZE / 2];  //最多是2K字节
void BSP_FlashWrite(uint32_t WriteAddr, uint16_t *pBuffer, uint16_t NumToWrite)
{
    uint32_t secpos;     //扇区地址
    uint16_t secoff;     //扇区内偏移地址(16位字计算)
    uint16_t secremain;  //扇区内剩余地址(16位字计算)
    uint16_t i;
    uint32_t offaddr;  //去掉0X08000000后的地址
    if (WriteAddr < STM32_FLASH_BASE || (WriteAddr >= (STM32_FLASH_BASE + 1024 * STM32_FLASH_SIZE)))
        return;                                //非法地址
    BSP_FlashUnlock();                         //解锁
    offaddr = WriteAddr - STM32_FLASH_BASE;    //实际偏移地址.
    secpos = offaddr / STM_SECTOR_SIZE;        //扇区地址  0~127 for STM32F103RBT6
    secoff = (offaddr % STM_SECTOR_SIZE) / 2;  //在扇区内的偏移(2个字节为基本单位.)
    secremain = STM_SECTOR_SIZE / 2 - secoff;  //扇区剩余空间大小
    if (NumToWrite <= secremain)
        secremain = NumToWrite;  //不大于该扇区范围
    while (1)
    {
        BSP_FlashRead(secpos * STM_SECTOR_SIZE + STM32_FLASH_BASE, STMFLASH_BUF,
                      STM_SECTOR_SIZE / 2);  //读出整个扇区的内容
        for (i = 0; i < secremain; i++)      //校验数据
        {
            if (STMFLASH_BUF[secoff + i] != 0XFFFF)
                break;  //需要擦除
        }
        if (i < secremain)  //需要擦除
        {
            BSP_FlashErasePage(secpos * STM_SECTOR_SIZE + STM32_FLASH_BASE);  //擦除这个扇区
            for (i = 0; i < secremain; i++)                                   //复制
            {
                STMFLASH_BUF[i + secoff] = pBuffer[i];
            }
            BSP_FlashWrite_NoCheck(secpos * STM_SECTOR_SIZE + STM32_FLASH_BASE, STMFLASH_BUF,
                                   STM_SECTOR_SIZE / 2);  //写入整个扇区
        }
        else
            BSP_FlashWrite_NoCheck(WriteAddr, pBuffer, secremain);  //写已经擦除了的,直接写入扇区剩余区间.
        if (NumToWrite == secremain)
            break;  //写入结束了
        else        //写入未结束
        {
            secpos++;                 //扇区地址增1
            secoff = 0;               //偏移位置为0
            pBuffer += secremain;     //指针偏移
            WriteAddr += secremain;   //写地址偏移
            NumToWrite -= secremain;  //字节(16位)数递减
            if (NumToWrite > (STM_SECTOR_SIZE / 2))
                secremain = STM_SECTOR_SIZE / 2;  //下一个扇区还是写不完
            else
                secremain = NumToWrite;  //下一个扇区可以写完了
        }
    };
    BSP_FlashLock();  //上锁
}
#endif

/**
 * @brief   从指定地址开始读出指定长度的数据
 * @param   ReadAddr: 起始地址
 * @param   pBuffer: 数据指针
 * @param   NumToWrite: 半字(16位)数
 * @retval  None
 */
void BSP_FlashRead(uint32_t ReadAddr, uint16_t *pBuffer, uint16_t NumToRead)
{
    uint16_t i;
    for (i = 0; i < NumToRead; i++)
    {
        pBuffer[i] = BSP_FlashReadHalfWord(ReadAddr);  //读取2个字节.
        ReadAddr += 2;                                 //偏移2个字节.
    }
}
