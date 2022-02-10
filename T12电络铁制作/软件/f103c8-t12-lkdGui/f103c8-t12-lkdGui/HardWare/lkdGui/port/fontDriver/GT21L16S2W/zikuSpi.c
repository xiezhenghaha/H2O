
#include "zikuSpi.h"

/**
  *@brief ���ų�ʼ��
  *@param  None
  *@retval None
  */
static void ZikuGpioConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	//GPIO_PinRemapConfig(GPIO_Remap_SPI1,ENABLE);
	
	/* SCK */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* MOSI */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* MISO */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* CS pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	ZIKUSPI_CS_HIGH();/* ����Ƭѡ��λ */
}

/**
  *@brief spi��ʼ��
  *@param  None
  *@retval None
  */
static void ZikuSpiConfig(void)
{	 
	SPI_InitTypeDef  SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	 /* SPI1 configuration */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(ZIKUSPIIS, &SPI_InitStructure);
	
	SPI_DataSizeConfig(ZIKUSPIIS,SPI_DataSize_8b);
	/* Enable SPI1  */
	SPI_Cmd(ZIKUSPIIS, ENABLE);	
}   

/**
  *@brief  SPI��д
  *@param  txData Ҫд����ֽ�
  *@retval ���յ��ֽ�
  */
static uint8_t ZikuSpiReadWriteByte(uint8_t txData)
{
	uint16_t outTime = 0;
	uint8_t rxData = 0;
	outTime = 0;
	while (SPI_I2S_GetFlagStatus(ZIKUSPIIS, SPI_I2S_FLAG_TXE) == RESET && outTime < 20000){
		outTime ++;
	}
	SPI_I2S_SendData(ZIKUSPIIS, txData);
	outTime = 0;
	while (SPI_I2S_GetFlagStatus(ZIKUSPIIS, SPI_I2S_FLAG_RXNE) == RESET && outTime < 20000){
		outTime ++;
	}
	rxData =(uint8_t) SPI_I2S_ReceiveData(ZIKUSPIIS);
	return rxData;	
}

/**
  *@brief  ���ֿ��ж����ݡ�
  *@param  Address ��ַ
  *@param  byte_long �ֽ���
  *@param  p_arr  ���ݵ�������
  *@retval ��������
  */
unsigned char r_dat_bat(unsigned long address,unsigned long byte_long,unsigned char *p_arr)
{		 			 
	unsigned long j = 0;
		
	ZIKUSPI_CS_LOW();//����Ƭѡ��λ
	ZikuSpiReadWriteByte(((address >>24)|0x03) & 0xff);//���������ַ
	ZikuSpiReadWriteByte((address >>16) & 0xff);
	ZikuSpiReadWriteByte((address >>8) & 0xff);
	ZikuSpiReadWriteByte((address >>0) & 0xff);
	for(j = 0;j < byte_long; j++){
		p_arr[j] = ZikuSpiReadWriteByte(0xff);//������
	}

	ZIKUSPI_CS_HIGH();//����Ƭѡ��λ
	return p_arr[0];
}

/**
  *@brief �ֿ��ʼ��
  *@param  None
  *@retval None
  */
void ZiKuInit(void)
{
	ZikuGpioConfig();
	ZikuSpiConfig();
}
/* END */
