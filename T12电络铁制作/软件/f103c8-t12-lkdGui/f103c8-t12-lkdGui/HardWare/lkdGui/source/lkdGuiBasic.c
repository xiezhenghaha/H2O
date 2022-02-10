/**
  * @file   lkdGuiBasic.c
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/03/19
  * @brief  lkdGui����������
  */
  
#include "lkdGui.h"

uint8_t const GetBit[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
/**
  *@brief ����
  *@param  x,y ����
  *@param  color ��ɫ
  *@retval None
  */
void GuiPoint(lkdCoord x, lkdCoord y, lkdColour color)
{
	/* ������ */
	if(x >= GUIXMAX || y >= GUIYMAX || x < 0 || y < 0)
		return;
	GuiDrawPoint(x, y, color);
}

/**
  *@brief ��б��
  *@param  x,y ����
  *@param  color ��ɫ
  *@retval None
  */
void GuiBiasLine(lkdCoord x0, lkdCoord y0,\
	lkdCoord x1, lkdCoord y1, lkdColour color)
{
	float offset,tf;	
	lkdCoord tempCoord;
	
	if(x0 > x1){
		tempCoord = x1;
		x1 = x0;
		x0 = tempCoord;
		tempCoord = y1;
		y1 = y0;
		y0 = tempCoord;
		
	}
	
	/* ������ */
	if(y0 >= GUIYMAX || x0 >= GUIXMAX || y1 < 0 || x1 < 0){
		return;
	}
	if(x1 >= GUIXMAX){
		x1 = GUIXMAX - 1;
	}
	if(y1 >= GUIYMAX){
		y1 = GUIYMAX - 1;
	}
	if(x0 < 0){
		x0 = 0;
	}
	if(y0 < 0){
		y0 = 0;
	}
	if(x0 == x1){
		GuiRPointLine(x0,y0,y1,1,color);
	}
	else if(y0 == y1){
		GuiHPointLine(x0,y0,x1,1,color);
	}
	else if(y1 - y0 > 0){
		if((y1 - y0) < (x1 - x0)){
			tf = y0;
			offset = ((float)(y1 - y0))/(x1 - x0);
			while(1){
				GuiDrawPoint(x0, (lkdCoord)tf, color);
				x0 ++;
				tf += offset;
				if(tf > y1){
					tf = y1;
				}
				if(x0 > x1){
					break;
				}
			}
		}
		else{
			tf = x0;
			offset = ((float)(x1 - x0))/(y1 - y0);
			while(1){
				GuiDrawPoint((lkdCoord)tf, y0, color);
				y0 ++;
				tf += offset;
				if(x0 > x1){
					x0 = x1;
				}
				if(y0 > y1){
					break;
				}
			}
		}
	}
	else{
		if((y0 - y1) < (x1 - x0)){
			tf = y0;
			offset = ((float)(y0 - y1))/(x1 - x0);
			while(1){
				GuiDrawPoint(x0, (lkdCoord)tf, color);
				x0 ++;
				tf -= offset;
				if(tf <= y1){
					tf = y1;
				}
				if(x0 > x1){
					break;
				}
			}
		}
		else{
			tf = x0;
			offset = ((float)(x1 - x0))/(y0 - y1);
			while(1){
				GuiDrawPoint((lkdCoord)tf, y0, color);
				y0 --;
				tf += offset;
				if(tf > x1){
					tf = x1;
				}
				if(y0 <= y1){
					break;
				}
			}
		}
	}
}
/**
  *@brief ��ˮƽ����
  *@param  x0,y0 ��ʼ����
  *@param  x1 ��������
  *@param  interval ÿ��N�㻭һ����
  *@param  color ��ɫ
  *@retval None
  */
void GuiHPointLine(lkdCoord x0, lkdCoord y0,\
	lkdCoord x1, uint8_t interval, lkdColour color)
{
	lkdCoord tx;
	
	if(x0 > x1){//���,����-> x0 < x1
		tx = x1;
		x1 = x0;
		x0 = tx;
	}
	
	/* ������ */
	if(y0 < 0 || y0 >= GUIYMAX){
		return;
	}
	if(x1 < 0){
		return;
	}
	if(x0 >= GUIXMAX){
		return;
	}
	if(x1 >= GUIXMAX){
		x1 = GUIXMAX - 1;
	}
	if(x0 < 0){
		x0 = 0;
	}
	
	/* ���� */
	if(x0 == x1){
		GuiDrawPoint(x0, y0, color);
		return;
	}
	while(1){
		GuiDrawPoint(x0, y0, color);
		x0 += interval;
		if(x0 > x1){
			break;
		}
	}
}

/**
  *@brief ����ֱ����
  *@param  x0,y0 ��ʼ����
  *@param  y1 ��������
  *@param  interval ÿ��N�㻭һ����
  *@param  color ��ɫ
  *@retval None
  */
void  GuiRPointLine(lkdCoord x0, lkdCoord y0,\
	lkdCoord y1,uint8_t interval, lkdColour color)
{
	lkdCoord ty;
	if(y0 > y1){//���,����-> y0 < y1
		ty = y1;
		y1 = y0;
		y0 = ty;
	}
	
	/* ������ */
	if(x0 < 0 || x0 >= GUIXMAX){
		return;
	}
	if(y1 < 0){
		return;
	}
	if(y0 >= GUIYMAX){
		return;
	}
	if(y1 >= GUIYMAX){
		y1 = GUIYMAX - 1;
	}
	if(y0 < 0){
		y0 = 0;
	}
	
	/* ���� */
	if(y0 == y1){
		GuiDrawPoint(x0, y0, color);
		return;
	}
	while(1){
		GuiDrawPoint(x0, y0, color);
		y0 += interval;
		if(y0 > y1){
			break;
		}
	}
}
/**
  *@brief ��ˮƽ��
  *@param  x0,y0 ��ʼ����
  *@param  x1��������
  *@param  color ��ɫ
  *@retval None
  */
void GuiHLine(lkdCoord x0, lkdCoord y0, lkdCoord x1, lkdColour color)
{
	GuiHPointLine(x0, y0, x1, 1, color);
}

/**
  *@brief ����ֱ��
  *@param  x0,y0 ��ʼ����
  *@param  y1 ��������
  *@param  color ��ɫ
  *@retval None
  */
void  GuiRLine(lkdCoord x0, lkdCoord y0, lkdCoord y1, lkdColour color)
{
	GuiRPointLine(x0, y0, y1, 1, color);
}

/**
  *@brief ������
  *@param  x0,y0 ��ʼ����
  *@param  x1,y1 ��������
  *@param  color ��ɫ
  *@retval None
  */
void GuiRect(lkdCoord x0, lkdCoord y0, lkdCoord x1,lkdCoord y1, lkdColour color)
{
	GuiHPointLine(x0, y0, x1, 1, color);
	GuiHPointLine(x0, y1, x1, 1, color);
	GuiRPointLine(x0, y0, y1, 1, color);
	GuiRPointLine(x1, y0, y1, 1, color);
}

/**
  *@brief ��������
  *@param  x0,y0 ��ʼ����
  *@param  x1,y1 ��������
  *@param  color ��ɫ
  *@retval None
  */
void GuiFillRect(lkdCoord x0, lkdCoord y0, lkdCoord x1,lkdCoord y1, lkdColour color)
{
	lkdCoord i;	
	lkdCoord tempCoord;
	
	if(y0 > y1){//����
		tempCoord = y1;
		y1 = y0;
		y0 = tempCoord;
	}
	if(x0 > x1){
		tempCoord = x1;
		x1 = x0;
		x0 = tempCoord;
	}
	for(i = y0; i<= y1; i++){
		GuiHPointLine(x0, i, x1, 1, color);
	}
}

/**
  *@brief ��λͼ
  *@param  bitMap λͼ����
  *@param  x0,y0 ��ʼ����
  *@param  xlen,ylen ��Ⱥ͸߶�
  *@param  flag 0 ������ʾ 1����
  *@retval None
  */
void GuiBitMap(uint8_t *bitMap,lkdCoord x0,\
	lkdCoord y0,uint32_t xlen,uint32_t ylen, uint8_t flag)
{
	uint8_t z,zMax;
	lkdColour colour;
	uint32_t i,j,tj,tx,eachRow;
	
	if(x0 < 0 || y0 < 0 || x0 >= GUIXMAX || y0 >= GUIYMAX){
		return;
	}
	/* �����ص�λΪ�ֽ� */
	if(xlen%8 != 0){
		eachRow = xlen/8 + 1;
	}
	else{
		eachRow = xlen/8;
	}
	/* ���������󣬳�������ʾ */
	if(x0 + xlen >= GUIXMAX){
		xlen = GUIXMAX - x0 - 1;
	}
	if(y0 + ylen >= GUIYMAX){
		ylen = GUIYMAX - y0 - 1;
	}
	/* ��ͼ */
	for(j = 0; j < ylen; j++){
		tj = eachRow * j;
		zMax = 8;
		for(i = 0; i < xlen; i += 8){
			if(i + 8 >= xlen){
				zMax = xlen - i;
			}
			tx = i/8;
			for(z = 0; z < zMax; z++){//λ����
				colour = (bitMap[tj + tx]&GetBit[z])?CBLACK:CWHITLE;
				if(flag){//���Դ���
					colour = colour?CWHITLE:CBLACK;
				}
				GuiDrawPoint(x0 + i + z,y0 + j,colour);
			}
		}
	}
}

/**
  *@brief ��ָ����Χ��λͼ
  *@param  bitMap λͼ����
  *@param  x0,y0 ��ʼ����
  *@param  endx,endy �յ�����
  *@param  flag 0 ������ʾ 1����
  *@retval None
  */
void GuiAreaBitMap(lkdBitmap *bitMap,\
	lkdCoord x0, lkdCoord y0, lkdCoord endx, lkdCoord endy, uint8_t flag)
{
	lkdCoord tcoord;
	lkdColour colour;
	uint8_t str,z,zMax,tflag = 0;
	uint16_t rowWide,i,j,yNum,xNum,tx;
	uint32_t tj;
	if(x0 >= GUIXMAX || y0 >= GUIYMAX || endx >= GUIXMAX || endy >= GUIYMAX ||\
		 x0 < 0 || y0 < 0 || endx < 0 || endy < 0 || endx == x0 || endy == y0 ||\
		 bitMap->beginx >= bitMap->wide || bitMap->beginy >= bitMap->high)
	{
		return;
	}
	if(x0 > endx){
		tcoord = x0;
		x0 = endx; 
		endx = tcoord;
	}
	if(y0 > endy){
		tcoord = y0;
		y0 = endy; 
		endy = tcoord;
	}
	if(bitMap->wide - bitMap->beginx < endx - x0){
		endx = bitMap->wide - bitMap->beginx + x0;
	}
	if(bitMap->high - bitMap->beginy < endy - y0){
		endy = bitMap->high - bitMap->beginy + y0;
	}

	/* �����ص�λΪ�ֽ� */
	rowWide = bitMap->wide / 8;
	if(bitMap->wide % 8 != 0){
		rowWide = rowWide + 1;
	}

	/* ����ʼ�����벿��  */
	xNum = endx - x0;
	yNum = endy - y0;
	z = bitMap->beginx % 8;
	if(z != 0){
		if(xNum + z >= 8){
			zMax = 8;
		}
		else{
			zMax = xNum + z;
		}
		tj = rowWide * bitMap->beginy;
		tx = bitMap->beginx / 8;
		for(j = 0; j < yNum; j ++){
			str = bitMap->bitmap[tj + tx];
			z = bitMap->beginx % 8;
			for( ; z < zMax; z ++){
				colour = (str & GetBit[z]) ? CBLACK : CWHITLE;
				if(flag){//���Դ���
					colour = colour ? CWHITLE : CBLACK;
				}
				GuiDrawPoint(x0 + z,y0 + j,colour);
			}
			tj += rowWide;
		}
		tflag = 1;
	}
	/* ������������  */
	tx = tflag + bitMap->beginx / 8;
	tj = rowWide * bitMap->beginy;
	if(bitMap->beginx % 8){
		x0 += (8 - bitMap->beginx % 8);
	}
	xNum = endx - x0;
	for(j = 0; j < yNum; j ++){
		for(i = 0; i < xNum; i += 8){
			zMax = 8;
			str = bitMap->bitmap[tj + tx + i / 8];
			if(i + 8 >= xNum){
				zMax = xNum - i;
			}
			for(z = 0 ; z < zMax; z ++){
				colour = (str & GetBit[z]) ? CBLACK : CWHITLE;
				if(flag){//���Դ���
					colour = colour ? CWHITLE : CBLACK;
				}
				GuiDrawPoint(x0 + i + z,y0 + j,colour);
			}
		}
		tj += rowWide;
	}
}

void GuiAreaBitMapNumber(lkdBitmap *bitMap,uint8_t number,\
	lkdCoord x0, lkdCoord y0, lkdCoord endx, lkdCoord endy, uint8_t flag)
{
	GuiAreaBitMap(bitMap,x0, y0, endx, endy, flag);
}

/* END */
