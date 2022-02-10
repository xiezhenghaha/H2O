/**
 ******************************************************************************
 * @file    stmflash.c
 * @author  Zxp
 * @version V1.0.0
 * @date    2021-01-25
 * @brief   ��stm32�ڲ�FLASH��ΪEEPROM�洢����
 ******************************************************************************
 */

#include "bsp_rwflash.h"

// FLASH��ʼ��ַ
#define STM32_FLASH_BASE 0x08000000  // STM32 FLASH����ʼ��ַ
// FLASH������ֵ
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
 * @brief   ����STM32��FLASH
 * @param   None
 * @retval  None
 */
void BSP_FlashUnlock(void)
{
    FLASH->KEYR = FLASH_KEY1;  //д���������.
    FLASH->KEYR = FLASH_KEY2;
}

/**
 * @brief   ����FLASH
 * @param   None
 * @retval  None
 */
void BSP_FlashLock(void)
{
    FLASH->CR |= 1 << 7;  //����
}

/**
 * @brief   ��ȡ��ǰFLASH��״̬
 * @param   None
 * @retval  ״̬: 0��ʾ���Բ���;1��ʾæ;2��ʾ����;3��ʾд����
 */
uint8_t BSP_FlashGetStatus(void)
{
    uint32_t res;
    res = FLASH->SR;
    if (res & (1 << 0))
        return 1;  //æ
    else if (res & (1 << 2))
        return 2;  //��̴���
    else if (res & (1 << 4))
        return 3;  //д��������
    return 0;      //�������
}

/**
 * @brief   �ȴ��������
 * @param   time: Ҫ��ʱ�ĳ���
 * @retval  ״̬: 0��ʾ���Բ���;1��ʾæ;2��ʾ����;3��ʾд����;0xff��ʾ�ȴ���ʱ
 */
uint8_t BSP_FlashWaitDone(uint16_t time)
{
    uint8_t res;
    do
    {
        res = BSP_FlashGetStatus();
        if (res != 1)
            break;  //��æ,����ȴ���,ֱ���˳�.
        Delay_us(1);
        time--;
    } while (time);
    if (time == 0)
        res = 0xff;  // TIMEOUT
    return res;
}

/**
 * @brief   ����ҳ
 * @param   paddr: ҳ��ַ
 * @retval  ִ�����
 */
uint8_t BSP_FlashErasePage(uint32_t paddr)
{
    uint8_t res = 0;
    res = BSP_FlashWaitDone(0X5FFF);  //�ȴ��ϴβ�������,>20ms
    if (res == 0)
    {
        FLASH->CR |= 1 << 1;              //ҳ����
        FLASH->AR = paddr;                //����ҳ��ַ
        FLASH->CR |= 1 << 6;              //��ʼ����
        res = BSP_FlashWaitDone(0X5FFF);  //�ȴ���������,>20ms
        if (res != 1)                     //��æ
        {
            FLASH->CR &= ~(1 << 1);  //���ҳ������־.
        }
    }
    return res;
}

/**
 * @brief   ��FLASHָ����ַд�����
 * @param   faddr:ָ����ַ(�˵�ַ����Ϊ2�ı���!!)
 * @param   dat: Ҫд�������
 * @retval  д������
 */
uint8_t BSP_FlashWriteHalfWord(uint32_t faddr, uint16_t dat)
{
    uint8_t res;
    res = BSP_FlashWaitDone(0XFF);
    if (res == 0)  // OK
    {
        FLASH->CR |= 1 << 0;            //���ʹ��
        *(vu16 *)faddr = dat;           //д������
        res = BSP_FlashWaitDone(0XFF);  //�ȴ��������
        if (res != 1)                   //�����ɹ�
        {
            FLASH->CR &= ~(1 << 0);  //���PGλ.
        }
    }
    return res;
}

/**
 * @brief   ��ȡָ����ַ�İ���(16λ����)
 * @param   faddr:����ַ
 * @retval  ����ֵ:��Ӧ����.
 */
uint16_t BSP_FlashReadHalfWord(uint32_t faddr)
{
    return *(vu16 *)faddr;
}
#if STM32_FLASH_WREN  //���ʹ����д

/**
 * @brief   ������д��
 * @param   WriteAddr: ��ʼ��ַ
 * @param   pBuffer: ����ָ��
 * @param   NumToWrite: ����(16λ)��
 * @retval  None
 */
void BSP_FlashWrite_NoCheck(uint32_t WriteAddr, uint16_t *pBuffer, uint16_t NumToWrite)
{
    uint16_t i;
    for (i = 0; i < NumToWrite; i++)
    {
        BSP_FlashWriteHalfWord(WriteAddr, pBuffer[i]);
        WriteAddr += 2;  //��ַ����2.
    }
}

/**
 * @brief   ��ָ����ַ��ʼд��ָ�����ȵ�����
 * @param   WriteAddr: ��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
 * @param   pBuffer: ����ָ��
 * @param   NumToWrite: ����(16λ)��(����Ҫд���16λ���ݵĸ���.)
 * @retval  None
 */
#if STM32_FLASH_SIZE < 256
#define STM_SECTOR_SIZE 1024  //�ֽ�
#else
#define STM_SECTOR_SIZE 2048
#endif
uint16_t STMFLASH_BUF[STM_SECTOR_SIZE / 2];  //�����2K�ֽ�
void BSP_FlashWrite(uint32_t WriteAddr, uint16_t *pBuffer, uint16_t NumToWrite)
{
    uint32_t secpos;     //������ַ
    uint16_t secoff;     //������ƫ�Ƶ�ַ(16λ�ּ���)
    uint16_t secremain;  //������ʣ���ַ(16λ�ּ���)
    uint16_t i;
    uint32_t offaddr;  //ȥ��0X08000000��ĵ�ַ
    if (WriteAddr < STM32_FLASH_BASE || (WriteAddr >= (STM32_FLASH_BASE + 1024 * STM32_FLASH_SIZE)))
        return;                                //�Ƿ���ַ
    BSP_FlashUnlock();                         //����
    offaddr = WriteAddr - STM32_FLASH_BASE;    //ʵ��ƫ�Ƶ�ַ.
    secpos = offaddr / STM_SECTOR_SIZE;        //������ַ  0~127 for STM32F103RBT6
    secoff = (offaddr % STM_SECTOR_SIZE) / 2;  //�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
    secremain = STM_SECTOR_SIZE / 2 - secoff;  //����ʣ��ռ��С
    if (NumToWrite <= secremain)
        secremain = NumToWrite;  //�����ڸ�������Χ
    while (1)
    {
        BSP_FlashRead(secpos * STM_SECTOR_SIZE + STM32_FLASH_BASE, STMFLASH_BUF,
                      STM_SECTOR_SIZE / 2);  //������������������
        for (i = 0; i < secremain; i++)      //У������
        {
            if (STMFLASH_BUF[secoff + i] != 0XFFFF)
                break;  //��Ҫ����
        }
        if (i < secremain)  //��Ҫ����
        {
            BSP_FlashErasePage(secpos * STM_SECTOR_SIZE + STM32_FLASH_BASE);  //�����������
            for (i = 0; i < secremain; i++)                                   //����
            {
                STMFLASH_BUF[i + secoff] = pBuffer[i];
            }
            BSP_FlashWrite_NoCheck(secpos * STM_SECTOR_SIZE + STM32_FLASH_BASE, STMFLASH_BUF,
                                   STM_SECTOR_SIZE / 2);  //д����������
        }
        else
            BSP_FlashWrite_NoCheck(WriteAddr, pBuffer, secremain);  //д�Ѿ������˵�,ֱ��д������ʣ������.
        if (NumToWrite == secremain)
            break;  //д�������
        else        //д��δ����
        {
            secpos++;                 //������ַ��1
            secoff = 0;               //ƫ��λ��Ϊ0
            pBuffer += secremain;     //ָ��ƫ��
            WriteAddr += secremain;   //д��ַƫ��
            NumToWrite -= secremain;  //�ֽ�(16λ)���ݼ�
            if (NumToWrite > (STM_SECTOR_SIZE / 2))
                secremain = STM_SECTOR_SIZE / 2;  //��һ����������д����
            else
                secremain = NumToWrite;  //��һ����������д����
        }
    };
    BSP_FlashLock();  //����
}
#endif

/**
 * @brief   ��ָ����ַ��ʼ����ָ�����ȵ�����
 * @param   ReadAddr: ��ʼ��ַ
 * @param   pBuffer: ����ָ��
 * @param   NumToWrite: ����(16λ)��
 * @retval  None
 */
void BSP_FlashRead(uint32_t ReadAddr, uint16_t *pBuffer, uint16_t NumToRead)
{
    uint16_t i;
    for (i = 0; i < NumToRead; i++)
    {
        pBuffer[i] = BSP_FlashReadHalfWord(ReadAddr);  //��ȡ2���ֽ�.
        ReadAddr += 2;                                 //ƫ��2���ֽ�.
    }
}
