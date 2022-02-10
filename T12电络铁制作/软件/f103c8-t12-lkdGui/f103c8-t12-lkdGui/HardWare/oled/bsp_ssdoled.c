/**
 ******************************************************************************
 * @file    bsp_ssdoled.c
 * @author  Zxp
 * @version V1.0.2
 * @date    2021-06-16
 * @brief   使用DMA配合SPI刷新OLED屏幕的驱动
 * @history Version     Date        Author      Note
 *          V1.0.0      2021-06-16  Zxp         创建文件
 *          V1.0.1      2021-06-17  Zxp         添加BSP_OLEDFill函数;修改BSP_OLEDShowString参数列表;修改BSP_OLEDShowChar和BSP_OLEDDrawPoint的参数类型
 *          V1.0.2      2021-06-17  Zxp         添加BSP_OLEDGetPixel函数;修改BSP_OLEDDrawPoint函数名,改为BSP_OLEDSetPixel;修改几个函数中坐标x和y的类型为int32_t
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

static void SPIx_Init(void)  //硬件SPI2口初始化
{
    SPI_InitTypeDef SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable SPI and GPIO clocks */
    RCC_APB2PeriphClockCmd(OLED_SPIx_RCC_CLK, ENABLE);                            //使能SPI2的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);  //使能IO口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SPI1, ENABLE);

    /* Configure SPI pins: SCK, MISO and MOSI */
    GPIO_InitStructure.GPIO_Pin = OLED_SPI_SCLK_PIN | OLED_SPI_MISO_PIN | OLED_SPI_MOSI_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
    GPIO_Init(OLED_SPI_PORT, &GPIO_InitStructure);

    /* Configure PB.12 as Output push-pull, used as Flash Chip select */
    GPIO_InitStructure.GPIO_Pin = OLED_SPI_NSS_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //推挽输出
    GPIO_Init(OLED_SPI_NSS_PORT, &GPIO_InitStructure);

    /* SPI configuration */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  // SPI设置为双线双向全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                       //设置为主SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                   // SPI发送接收8位帧结构
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                         //时钟悬空高
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                        //数据捕获于第二个时钟沿
    SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;                           //内部NSS信号有SSI位控制
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;  //波特率预分频值为2
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                  //数据传输从MSB位开始
    SPI_InitStructure.SPI_CRCPolynomial = 7;                            // SPI_CRCPolynomial定义了用于CRC值计算的多项式
    SPI_Init(OLED_SPIx, &SPI_InitStructure);

    /* Enable SPI  */
    SPI_SSOutputCmd(OLED_SPIx, ENABLE);
    SPI_Cmd(OLED_SPIx, ENABLE);
}

static void SPI_DMA_Init(void)  //硬件SPI2口初始化
{
    DMA_InitTypeDef DMA_InitStruct;

    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    RCC_AHBPeriphClockCmd(OLED_SPI_DMA_CLK, ENABLE);

    DMA_DeInit(OLED_SPI_DMA_CHANNEL);

    // 配置 DMA 初始化结构体
    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)(&(OLED_SPIx->DR));  // 外设基址为：ADC 数据寄存器地址
    DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)DispBuff;                // 存储器地址
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;                        // 数据源来自内存
    DMA_InitStruct.DMA_BufferSize = 128 * 8;                               // 缓冲区大小，应该等于数据目的地的大小
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;          // 外设寄存器只有一个，地址不用递增
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;                   // 存储器地址递增
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;   // 外设数据大小为半字，即两个字节
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;           // 内存数据大小也为半字，跟外设数据大小相同
    DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;                           // 循环传输模式
    DMA_InitStruct.DMA_Priority = DMA_Priority_High;                       // DMA 传输通道优先级为高，当使用一个DMA通道时，优先级设置不影响
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;                              // 禁止存储器到存储器模式，因为是从外设到存储器
    DMA_Init(OLED_SPI_DMA_CHANNEL, &DMA_InitStruct);                       // 初始化DMA

    SPI_I2S_DMACmd(OLED_SPIx, OLED_SPI_DMA_REQ, ENABLE);
    DMA_Cmd(OLED_SPI_DMA_CHANNEL, DISABLE);  // 使能 DMA 通道
}

void BSP_Refresh(void)
{
}

static void OLED_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //使能IO口时钟

    GPIO_InitStructure.GPIO_Pin = OLED_DC_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //复用推挽输出
    GPIO_Init(OLED_DC_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = OLED_RST_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //复用推挽输出
    GPIO_Init(OLED_RST_PORT, &GPIO_InitStructure);
}

static void OLED_DMAStart(void)
{
    BSP_OLEDWriteCmd(0xb0);
    BSP_OLEDWriteCmd(0x00);
    BSP_OLEDWriteCmd(0x10);

    DMA_Cmd(OLED_SPI_DMA_CHANNEL, ENABLE);  // 使能 DMA 通道
}

/**
 * @brief   调用SPI发送并读取
 * @note    在发送前关闭DMA,退出函数前重新开启DMA
 * @param   data: 发送的数据
 * @return  读取的数据
 */
static uint8_t SPIx_ReadWriteByte(uint8_t data)
{
    uint8_t retry = 0, state = 0;

    state = OLED_SPI_DMA_CHANNEL->CCR & DMA_CCR1_EN;
    if (state)  // 若DMA处于开启状态,则先关闭DMA
    {
        OLED_SPI_DMA_CHANNEL->CCR &= (uint16_t)(~DMA_CCR1_EN);
    }

    while (SPI_I2S_GetFlagStatus(OLED_SPIx, SPI_I2S_FLAG_TXE) == RESET)  //检查指定的SPI标志位设置与否:发送缓存空标志位
    {
        retry++;
        if (retry > 200)
            return 0;
    }
    SPI_I2S_SendData(OLED_SPIx, data);  //通过外设SPIx发送一个数据
    retry = 0;
    while (SPI_I2S_GetFlagStatus(OLED_SPIx, SPI_I2S_FLAG_RXNE) == RESET)  //检查指定的SPI标志位设置与否:接受缓存非空标志位
    {
        retry++;
        if (retry > 200)
            return 0;
    }
    retry = SPI_I2S_ReceiveData(OLED_SPIx);  //返回通过SPIx最近接收的数据

    if (state)  // 若发送前DMA处于开启状态,则退出前重新开启DMA
    {
        DMA_SetCurrDataCounter(OLED_SPI_DMA_CHANNEL, 0);
        OLED_SPI_DMA_CHANNEL->CCR |= (uint16_t)(DMA_CCR1_EN);
    }
    return retry;  //返回通过SPIx最近接收的数据
}

/**
 * @brief   复位OLED
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
 * @brief   向OLED发送控制数据
 * @param   cmd: 控制数据
 * @return  None
 */
void BSP_OLEDWriteCmd(uint8_t cmd)
{
    OLED_DC_Clr();
    SPIx_ReadWriteByte(cmd);
    OLED_DC_Set();
}

/**
 * @brief   向OLED发送数据
 * @param   data: 数据
 * @return  None
 */
void BSP_OLEDWriteData(uint8_t data)
{
    OLED_DC_Set();
    SPIx_ReadWriteByte(data);
    OLED_DC_Clr();
}

/**
 * @brief   清屏OLED
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
 * @brief   填充一款区域为一个颜色
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

//开启OLED显示
void BSP_OLEDSetOn(void)
{
    BSP_OLEDWriteCmd(0X8D);  // SET DCDC命令
    BSP_OLEDWriteCmd(0X14);  // DCDC ON
    BSP_OLEDWriteCmd(0XAF);  // DISPLAY ON
    BSP_InitOLED();
}
//关闭OLED显示
void BSP_OLEDSetOff(void)
{
    BSP_OLEDWriteCmd(0X8D);  // SET DCDC命令
    BSP_OLEDWriteCmd(0X10);  // DCDC OFF
    BSP_OLEDWriteCmd(0XAE);  // DISPLAY OFF

    // SPI_I2S_DMACmd(OLED_SPIx, OLED_SPI_DMA_REQ, DISABLE);
    DMA_Cmd(OLED_SPI_DMA_CHANNEL, DISABLE);  // 使能 DMA 通道
}

/**
 * @brief   在OLED的显示缓冲区上画点
 * @param   x: 要画点的X轴坐标
 * @param   y: 要画点的Y轴坐标
 * @param   point: 是否要画点
 *  @arg    0: 擦除该点
 *  @arg    1: 绘制该点
 * @return  None
 */
void BSP_OLEDSetPixel(int32_t x, int32_t y, int32_t point)
{
    uint8_t temp = 0;

    if ((x >= OLED_DISPLAY_WIDTH) || (y >= OLED_DISPLAY_HEIGHT))
        return;  //超出范围了.
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
 * @brief   获取显示缓冲区上某点的颜色值
 * @param   x: 要画点的X轴坐标
 * @param   y: 要画点的Y轴坐标
 * @return  改点的颜色值
 */
int32_t BSP_OLEDGetPixel(int32_t x, int32_t y)
{
    uint8_t temp = 0;
    int32_t retVal;

    if ((x >= OLED_DISPLAY_WIDTH) || (y >= OLED_DISPLAY_HEIGHT))
        return 0;  //超出范围了.
    temp = 1 << (y & 7);
    y = y >> 3;

    retVal = (DispBuff[y][x] & temp) ? 1 : 0;

    return retVal;
}

/**
 * @brief   在指定位置显示字符
 * @param   x: 要显示字符的X轴坐标
 * @param   y: 要显示字符的Y轴坐标
 * @param   chr: 要显示的字符
 * @param   size: 要显示的字符的字号,可选12/16
 * @param   mode: 是否反色显示
 *  @arg    0: 正常显示
 *  @arg    1: 反色显示
 * @return  None
 */
void BSP_OLEDShowChar(int32_t x, int32_t y, uint8_t chr, uint8_t size, uint8_t inverseColor)
{
    uint8_t temp, t, t1;
    int32_t y0 = y;
    chr = chr - ' ';  //得到偏移后的值
    for (t = 0; t < size; t++)
    {
        if (size == 12)
            temp = oled_asc2_1206[chr][t];  //调用1206字体
        else
            temp = oled_asc2_1608[chr][t];  //调用1608字体
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
            return;  // 若y值超过屏幕最大高度,则退出
        }
        BSP_OLEDShowChar(x, y, *str, size, inverseColor);
        x += 8;
        str++;
    }
}

/**
 * @brief   初始化OLED,设置为SPI传输,DMA方式
 * @param   None
 * @return  None
 */
void BSP_InitOLED(void)
{
    SPIx_Init();
    SPI_DMA_Init();
    OLED_GPIO_Init();

    BSP_OLEDReset();

    BSP_OLEDWriteCmd(0xAE);  //OLED休眠
    BSP_OLEDWriteCmd(0x00);  //为页寻址模式设置下栏起始地址
    BSP_OLEDWriteCmd(0x10);  //为页寻址模式设置更高的列起始地址
    BSP_OLEDWriteCmd(0x40);  //设置显示起始线(0x00~0x3F)
    BSP_OLEDWriteCmd(0x81);  //设置对比度控制寄存器
    BSP_OLEDWriteCmd(0xCF);  //设置SEG输出电流亮度
    BSP_OLEDWriteCmd(0xA1);  //Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    BSP_OLEDWriteCmd(0xC8);  //Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    BSP_OLEDWriteCmd(0xA8);  //set multiplex ratio(1 to 64)
    BSP_OLEDWriteCmd(0x3f);  //1/64 duty
    BSP_OLEDWriteCmd(0x81);  //对比度设置
    BSP_OLEDWriteCmd(0x12);  //1~255;默认0X7F (亮度设置,越大越亮)
    BSP_OLEDWriteCmd(0xD3);  //设置显示偏移 (0x00~0x3F)
    BSP_OLEDWriteCmd(0x00);  //不偏移
    BSP_OLEDWriteCmd(0xd5);  //时钟频率
    BSP_OLEDWriteCmd(0x80);  //100 Frames/Sec
    BSP_OLEDWriteCmd(0xD9);  //预充电持续时间
    BSP_OLEDWriteCmd(0xF1);  //Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    BSP_OLEDWriteCmd(0xDA);  //设置com引脚硬件配置
    BSP_OLEDWriteCmd(0x12);  //
    BSP_OLEDWriteCmd(0xDB);  //set vcomh
    BSP_OLEDWriteCmd(0x40);  //Set VCOM Deselect Level
    BSP_OLEDWriteCmd(0x20);  //设置内存寻址模式
    BSP_OLEDWriteCmd(0x00);  //设置成自动巡回扫描模式，先行后纵
    BSP_OLEDWriteCmd(0x8D);  //设置电荷泵
    BSP_OLEDWriteCmd(0x14);  //开启电荷泵
    BSP_OLEDWriteCmd(0xA4);  //Disable Entire Display On (0xa4/0xa5)
    BSP_OLEDWriteCmd(0xA6);  //正常显示模式，1为亮0为灭
    BSP_OLEDWriteCmd(0xAF);  //OLED唤醒

    BSP_OLEDClear();

    OLED_DMAStart();
		
		BSP_OLEDFill(10,10,100,100,0);
}
