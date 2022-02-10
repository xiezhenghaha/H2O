
#ifndef __ZIKUSPI_H
#define	__ZIKUSPI_H

#include "stm32f10x.h"

#define ZIKUSPIIS SPI1
#define ZIKUSPI_CS_LOW()  GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define ZIKUSPI_CS_HIGH() GPIO_SetBits(GPIOA, GPIO_Pin_4)

/* �ֿ��ʼ�� */
void ZiKuInit(void);
/* .lib�ļ���Ҫ�Ľӿ� */
unsigned char r_dat_bat(unsigned long address,\
	unsigned long byte_long,unsigned char *p_arr);

#endif /* END __ZIKUSPI_H */

