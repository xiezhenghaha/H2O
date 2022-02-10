/**
 ******************************************************************************
 * @file    bsp_ssdoled.c
 * @author  Zxp
 * @version V1.0.2
 * @date    2021-06-16
 * @brief   ʹ��DMA���SPIˢ��OLED��Ļ������
 * @history Version     Date        Author      Note
 *          V1.0.0      2021-06-16  Zxp         �����ļ�
 *          V1.0.1      2021-06-17  Zxp         ���BSP_OLEDFill����;�޸�BSP_OLEDShowString�����б�;�޸�BSP_OLEDShowChar��BSP_OLEDDrawPoint�Ĳ�������
 *          V1.0.2      2021-06-17  Zxp         ���BSP_OLEDGetPixel����;�޸�BSP_OLEDDrawPoint������,��ΪBSP_OLEDSetPixel;�޸ļ�������������x��y������Ϊint32_t
 ******************************************************************************
 */

#include "bsp_ssdoled.h"

#include "includes.h"
#include "oledfont.h"

uint8_t DispBuff[OLED_BUFFER_X][OLED_BUFFER_Y];

#define OLED_SPIx SPI1
#define OLED_SPIx_RCC_CLK RCC_APB2Periph_SPI1
#define OLED_SPI_DMA_CHANNEL DMA1_Channel3
#define OLED_SPI_DMA_CLK RCC_AHBPeriph_DMA1
#define OLED_SPI_DMA_REQ SPI_I2S_DMAReq_Tx

#define OLED_SPI_NSS_PORT GPIOA
#define OLED_SPI_NSS_PIN GPIO_Pin_15
#define OLED_SPI_PORT GPIOB
#define OLED_SPI_SCLK_PIN GPIO_Pin_3
#define OLED_SPI_MISO_PIN GPIO_Pin_4
#define OLED_SPI_MOSI_PIN GPIO_Pin_5

#define OLED_DC_PORT GPIOB
#define OLED_DC_PIN GPIO_Pin_8
#define OLED_RST_PORT GPIOB
#define OLED_RST_PIN GPIO_Pin_9

#define OLED_DC_Clr() GPIO_ResetBits(OLED_DC_PORT, OLED_DC_PIN)  // A0/DC
#define OLED_DC_Set() GPIO_SetBits(OLED_DC_PORT, OLED_DC_PIN)

#define OLED_RST_Clr() GPIO_ResetBits(OLED_RST_PORT, OLED_RST_PIN)  // RESET
#define OLED_RST_Set() GPIO_SetBits(OLED_RST_PORT, OLED_RST_PIN)

static void SPIx_Init(void)  //Ӳ��SPI2�ڳ�ʼ��
{
    SPI_InitTypeDef SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable SPI and GPIO clocks */
    RCC_APB2PeriphClockCmd(OLED_SPIx_RCC_CLK, ENABLE);                            //ʹ��SPI2��ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);  //ʹ��IO��ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SPI1, ENABLE);

    /* Configure SPI pins: SCK, MISO and MOSI */
    GPIO_InitStructure.GPIO_Pin = OLED_SPI_SCLK_PIN | OLED_SPI_MISO_PIN | OLED_SPI_MOSI_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
    GPIO_Init(OLED_SPI_PORT, &GPIO_InitStructure);

    /* Configure PB.12 as Output push-pull, used as Flash Chip select */
    GPIO_InitStructure.GPIO_Pin = OLED_SPI_NSS_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�������
    GPIO_Init(OLED_SPI_NSS_PORT, &GPIO_InitStructure);

    /* SPI configuration */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  // SPI����Ϊ˫��˫��ȫ˫��
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                       //����Ϊ��SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                   // SPI���ͽ���8λ֡�ṹ
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                         //ʱ�����ո�
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                        //���ݲ����ڵڶ���ʱ����
    SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;                           //�ڲ�NSS�ź���SSIλ����
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;  //������Ԥ��ƵֵΪ2
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                  //���ݴ����MSBλ��ʼ
    SPI_InitStructure.SPI_CRCPolynomial = 7;                            // SPI_CRCPolynomial����������CRCֵ����Ķ���ʽ
    SPI_Init(OLED_SPIx, &SPI_InitStructure);

    /* Enable SPI  */
    SPI_SSOutputCmd(OLED_SPIx, ENABLE);
    SPI_Cmd(OLED_SPIx, ENABLE);
}

static void SPI_DMA_Init(void)  //Ӳ��SPI2�ڳ�ʼ��
{
    DMA_InitTypeDef DMA_InitStruct;

    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    RCC_AHBPeriphClockCmd(OLED_SPI_DMA_CLK, ENABLE);

    DMA_DeInit(OLED_SPI_DMA_CHANNEL);

    // ���� DMA ��ʼ���ṹ��
    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)(&(OLED_SPIx->DR));  // �����ַΪ��ADC ���ݼĴ�����ַ
    DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)DispBuff;                // �洢����ַ
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;                        // ����Դ�����ڴ�
    DMA_InitStruct.DMA_BufferSize = 128 * 8;                               // ��������С��Ӧ�õ�������Ŀ�ĵصĴ�С
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;          // ����Ĵ���ֻ��һ������ַ���õ���
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;                   // �洢����ַ����
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;   // �������ݴ�СΪ���֣��������ֽ�
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;           // �ڴ����ݴ�СҲΪ���֣����������ݴ�С��ͬ
    DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;                           // ѭ������ģʽ
    DMA_InitStruct.DMA_Priority = DMA_Priority_High;                       // DMA ����ͨ�����ȼ�Ϊ�ߣ���ʹ��һ��DMAͨ��ʱ�����ȼ����ò�Ӱ��
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;                              // ��ֹ�洢�����洢��ģʽ����Ϊ�Ǵ����赽�洢��
    DMA_Init(OLED_SPI_DMA_CHANNEL, &DMA_InitStruct);                       // ��ʼ��DMA

    SPI_I2S_DMACmd(OLED_SPIx, OLED_SPI_DMA_REQ, ENABLE);
    DMA_Cmd(OLED_SPI_DMA_CHANNEL, DISABLE);  // ʹ�� DMA ͨ��
}

void BSP_Refresh(void)
{
}

static void OLED_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //ʹ��IO��ʱ��

    GPIO_InitStructure.GPIO_Pin = OLED_DC_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�����������
    GPIO_Init(OLED_DC_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = OLED_RST_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�����������
    GPIO_Init(OLED_RST_PORT, &GPIO_InitStructure);
}

static void OLED_DMAStart(void)
{
    BSP_OLEDWriteCmd(0xb0);
    BSP_OLEDWriteCmd(0x00);
    BSP_OLEDWriteCmd(0x10);

    DMA_Cmd(OLED_SPI_DMA_CHANNEL, ENABLE);  // ʹ�� DMA ͨ��
}

/**
 * @brief   ����SPI���Ͳ���ȡ
 * @note    �ڷ���ǰ�ر�DMA,�˳�����ǰ���¿���DMA
 * @param   data: ���͵�����
 * @return  ��ȡ������
 */
static uint8_t SPIx_ReadWriteByte(uint8_t data)
{
    uint8_t retry = 0, state = 0;

    state = OLED_SPI_DMA_CHANNEL->CCR & DMA_CCR1_EN;
    if (state)  // ��DMA���ڿ���״̬,���ȹر�DMA
    {
        OLED_SPI_DMA_CHANNEL->CCR &= (uint16_t)(~DMA_CCR1_EN);
    }

    while (SPI_I2S_GetFlagStatus(OLED_SPIx, SPI_I2S_FLAG_TXE) == RESET)  //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
    {
        retry++;
        if (retry > 200)
            return 0;
    }
    SPI_I2S_SendData(OLED_SPIx, data);  //ͨ������SPIx����һ������
    retry = 0;
    while (SPI_I2S_GetFlagStatus(OLED_SPIx, SPI_I2S_FLAG_RXNE) == RESET)  //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
    {
        retry++;
        if (retry > 200)
            return 0;
    }
    retry = SPI_I2S_ReceiveData(OLED_SPIx);  //����ͨ��SPIx������յ�����

    if (state)  // ������ǰDMA���ڿ���״̬,���˳�ǰ���¿���DMA
    {
        DMA_SetCurrDataCounter(OLED_SPI_DMA_CHANNEL, 0);
        OLED_SPI_DMA_CHANNEL->CCR |= (uint16_t)(DMA_CCR1_EN);
    }
    return retry;  //����ͨ��SPIx������յ�����
}

/**
 * @brief   ��λOLED
 * @param   None
 * @return  None
 */
void BSP_OLEDReset(void)
{
    uint32_t i;

    OLED_RST_Clr();
    for (i = 0; i < 888; i++)
        ;
    OLED_RST_Set();
}

/**
 * @brief   ��OLED���Ϳ�������
 * @param   cmd: ��������
 * @return  None
 */
void BSP_OLEDWriteCmd(uint8_t cmd)
{
    OLED_DC_Clr();
    SPIx_ReadWriteByte(cmd);
    OLED_DC_Set();
}

/**
 * @brief   ��OLED��������
 * @param   data: ����
 * @return  None
 */
void BSP_OLEDWriteData(uint8_t data)
{
    OLED_DC_Set();
    SPIx_ReadWriteByte(data);
    OLED_DC_Clr();
}

/**
 * @brief   ����OLED
 * @param   None
 * @return  None
 */
void BSP_OLEDClear(void)
{
    uint32_t r, c;

    BSP_OLEDWriteCmd(0xb0);
    BSP_OLEDWriteCmd(0x00);
    BSP_OLEDWriteCmd(0x10);

    for (r = 0; r < OLED_BUFFER_X; r++)
    {
        for (c = 0; c < OLED_BUFFER_Y; c++)
        {
            DispBuff[r][c] = 0;
            // BSP_OLEDWriteData(DispBuff[r][c]);
        }
    }
}

/**
 * @brief   ���һ������Ϊһ����ɫ
 * @param   None
 * @return  None
 */
void BSP_OLEDFill(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color)
{
    uint32_t xStart, yStart, xLimit, yLimit;

    xStart = x;
    yStart = y;
    xLimit = x + width;
    yLimit = y + height;
    if ((x >= OLED_DISPLAY_WIDTH) || (y >= OLED_DISPLAY_HEIGHT) || (xLimit >= OLED_DISPLAY_WIDTH) || (yLimit >= OLED_DISPLAY_HEIGHT))
    {
        return;
    }

    for (x = xStart; x < xLimit; x++)
    {
        for (y = yStart; y < yLimit; y++)
        {
            BSP_OLEDSetPixel(x, y, color);
        }
    }
}

//����OLED��ʾ
void BSP_OLEDSetOn(void)
{
    BSP_OLEDWriteCmd(0X8D);  // SET DCDC����
    BSP_OLEDWriteCmd(0X14);  // DCDC ON
    BSP_OLEDWriteCmd(0XAF);  // DISPLAY ON
    BSP_InitOLED();
}
//�ر�OLED��ʾ
void BSP_OLEDSetOff(void)
{
    BSP_OLEDWriteCmd(0X8D);  // SET DCDC����
    BSP_OLEDWriteCmd(0X10);  // DCDC OFF
    BSP_OLEDWriteCmd(0XAE);  // DISPLAY OFF

    // SPI_I2S_DMACmd(OLED_SPIx, OLED_SPI_DMA_REQ, DISABLE);
    DMA_Cmd(OLED_SPI_DMA_CHANNEL, DISABLE);  // ʹ�� DMA ͨ��
}

/**
 * @brief   ��OLED����ʾ�������ϻ���
 * @param   x: Ҫ�����X������
 * @param   y: Ҫ�����Y������
 * @param   point: �Ƿ�Ҫ����
 *  @arg    0: �����õ�
 *  @arg    1: ���Ƹõ�
 * @return  None
 */
void BSP_OLEDSetPixel(int32_t x, int32_t y, int32_t point)
{
    uint8_t temp = 0;

    if ((x >= OLED_DISPLAY_WIDTH) || (y >= OLED_DISPLAY_HEIGHT))
        return;  //������Χ��.
    temp = 1 << (y & 7);
    y = y >> 3;
    if (point)
    {
        DispBuff[y][x] |= temp;
    }
    else
    {
        DispBuff[y][x] &= ~temp;
    }
}

/**
 * @brief   ��ȡ��ʾ��������ĳ�����ɫֵ
 * @param   x: Ҫ�����X������
 * @param   y: Ҫ�����Y������
 * @return  �ĵ����ɫֵ
 */
int32_t BSP_OLEDGetPixel(int32_t x, int32_t y)
{
    uint8_t temp = 0;
    int32_t retVal;

    if ((x >= OLED_DISPLAY_WIDTH) || (y >= OLED_DISPLAY_HEIGHT))
        return 0;  //������Χ��.
    temp = 1 << (y & 7);
    y = y >> 3;

    retVal = (DispBuff[y][x] & temp) ? 1 : 0;

    return retVal;
}

/**
 * @brief   ��ָ��λ����ʾ�ַ�
 * @param   x: Ҫ��ʾ�ַ���X������
 * @param   y: Ҫ��ʾ�ַ���Y������
 * @param   chr: Ҫ��ʾ���ַ�
 * @param   size: Ҫ��ʾ���ַ����ֺ�,��ѡ12/16
 * @param   mode: �Ƿ�ɫ��ʾ
 *  @arg    0: ������ʾ
 *  @arg    1: ��ɫ��ʾ
 * @return  None
 */
void BSP_OLEDShowChar(int32_t x, int32_t y, uint8_t chr, uint8_t size, uint8_t inverseColor)
{
    uint8_t temp, t, t1;
    int32_t y0 = y;
    chr = chr - ' ';  //�õ�ƫ�ƺ��ֵ
    for (t = 0; t < size; t++)
    {
        if (size == 12)
            temp = oled_asc2_1206[chr][t];  //����1206����
        else
            temp = oled_asc2_1608[chr][t];  //����1608����
        for (t1 = 0; t1 < 8; t1++)
        {
            if (temp & 0x80)
                BSP_OLEDSetPixel(x, y, !inverseColor);
            else
                BSP_OLEDSetPixel(x, y, inverseColor);
            temp <<= 1;
            y++;
            if ((y - y0) == size)
            {
                y = y0;
                x++;
                break;
            }
        }
    }
}

void BSP_OLEDShowString(int32_t x, int32_t y, const uint8_t *str, uint8_t size, uint8_t inverseColor)
{
    while (*str != '\0')
    {
        if (x >= OLED_DISPLAY_WIDTH)
        {
            x = 0;
            y += 16;
        }
        if (y >= OLED_DISPLAY_HEIGHT)
        {
            // y = x = 0;
            return;  // ��yֵ������Ļ���߶�,���˳�
        }
        BSP_OLEDShowChar(x, y, *str, size, inverseColor);
        x += 8;
        str++;
    }
}

/**
 * @brief   ��ʼ��OLED,����ΪSPI����,DMA��ʽ
 * @param   None
 * @return  None
 */
void BSP_InitOLED(void)
{
    SPIx_Init();
    SPI_DMA_Init();
    OLED_GPIO_Init();

    BSP_OLEDReset();

    BSP_OLEDWriteCmd(0xAE);  //OLED����
    BSP_OLEDWriteCmd(0x00);  //ΪҳѰַģʽ����������ʼ��ַ
    BSP_OLEDWriteCmd(0x10);  //ΪҳѰַģʽ���ø��ߵ�����ʼ��ַ
    BSP_OLEDWriteCmd(0x40);  //������ʾ��ʼ��(0x00~0x3F)
    BSP_OLEDWriteCmd(0x81);  //���öԱȶȿ��ƼĴ���
    BSP_OLEDWriteCmd(0xCF);  //����SEG�����������
    BSP_OLEDWriteCmd(0xA1);  //Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
    BSP_OLEDWriteCmd(0xC8);  //Set COM/Row Scan Direction   0xc0���·��� 0xc8����
    BSP_OLEDWriteCmd(0xA8);  //set multiplex ratio(1 to 64)
    BSP_OLEDWriteCmd(0x3f);  //1/64 duty
    BSP_OLEDWriteCmd(0x81);  //�Աȶ�����
    BSP_OLEDWriteCmd(0x12);  //1~255;Ĭ��0X7F (��������,Խ��Խ��)
    BSP_OLEDWriteCmd(0xD3);  //������ʾƫ�� (0x00~0x3F)
    BSP_OLEDWriteCmd(0x00);  //��ƫ��
    BSP_OLEDWriteCmd(0xd5);  //ʱ��Ƶ��
    BSP_OLEDWriteCmd(0x80);  //100 Frames/Sec
    BSP_OLEDWriteCmd(0xD9);  //Ԥ������ʱ��
    BSP_OLEDWriteCmd(0xF1);  //Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    BSP_OLEDWriteCmd(0xDA);  //����com����Ӳ������
    BSP_OLEDWriteCmd(0x12);  //
    BSP_OLEDWriteCmd(0xDB);  //set vcomh
    BSP_OLEDWriteCmd(0x40);  //Set VCOM Deselect Level
    BSP_OLEDWriteCmd(0x20);  //�����ڴ�Ѱַģʽ
    BSP_OLEDWriteCmd(0x00);  //���ó��Զ�Ѳ��ɨ��ģʽ�����к���
    BSP_OLEDWriteCmd(0x8D);  //���õ�ɱ�
    BSP_OLEDWriteCmd(0x14);  //������ɱ�
    BSP_OLEDWriteCmd(0xA4);  //Disable Entire Display On (0xa4/0xa5)
    BSP_OLEDWriteCmd(0xA6);  //������ʾģʽ��1Ϊ��0Ϊ��
    BSP_OLEDWriteCmd(0xAF);  //OLED����

    BSP_OLEDClear();

    OLED_DMAStart();
		
		BSP_OLEDFill(10,10,100,100,0);
}
