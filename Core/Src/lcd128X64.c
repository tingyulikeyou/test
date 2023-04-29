#include "main.h"
#include "lcdfont.h"

#ifdef LCD128X64_SUPPORT
__IO uint8_t m_switch = 0u;


uint8_t lcd_framebuf[LCD_HEIGHT/8][LCD_WIDHT];	





void delay_us(uint16_t us)
{
	 uint16_t j,k;
	 for(j=0;j<us;j++)
	 	for(k=0;k<10;k++);
}

void delay(uint16_t t)
{
	HAL_Delay(t);
}



/*****************************************************************************
 * @name       :void LCD_Clear(uint16_t Color)
 * @date       :2018-08-09 
 * @function   :Full screen filled LCD screen
 * @parameters :color:Filled color
 * @retvalue   :None
******************************************************************************/	
void LCD_Clear(uint16_t Color)
{
	unsigned char i,j;
 	for(i=0;i<9;i++)
	{
		LCD_Address(1+i,1);
		for(j=0;j<132;j++)
		{
			write_data(0x00);
		}
	}

} 

void LCD_Address(uint8_t page, uint8_t column)
{
	column=column-1;  						
	page=page-1;
	write_com(0xb0+page);   			//设置页地址。每页是8行。一个画面的64行被分成8个页。我们平常所说的第1页，在LCD驱动IC里是第0页，所以在这里减去1*/
	write_com(((column>>4)&0x0f)+0x10);	//设置列地址的高4位
	write_com(column&0x0f);				//设置列地址的低4位
}


/*****************************************************************************
 * @name       :void LCD_GPIOInit(void)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen GPIO
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_GPIOInit(void)
{
	LCD_RD_SET;
	LCD_CS_SET;
	LCD_RS_SET;
	LCD_WR_SET;
	LCD_RST_SET;
}

/*****************************************************************************
 * @name       :void LCD_RESET(void)
 * @date       :2018-08-09 
 * @function   :Reset LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCD_RESET(void)
{
	LCD_RST_CLR;
	HAL_Delay(100);	
	LCD_RST_SET;
	HAL_Delay(50);
}

void LCD_DisplayOff(void)
{
	write_com(0xac);/*静态图标关闭*/
	write_com(0x00);/*静态图标寄存器设置：关闭。此指令与上述指令一起完成静态图标关闭*/
	write_com(0xae);/*显示：关*/
	write_com(0xa5);/*全屏显示：开*/

}

void LCD_DisplayWake(void)
{
	write_com(0xa4);/*全屏显示：关。进入正常模式*/
	write_com(0xad);/*静态图标开启*/
	write_com(0x03);/*静态图标寄存器设置：开。此指令与上述指令一起完成静态图标开启*/
	write_com(0xaf);/*显示：开*/

}






/*****************************************************************************
 * @name       :void LCD_Init(void)
 * @date       :
 * @function   :Initialization LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 


void LCD_Init(void)
{  
	LCD_RST_CLR;        /*低电平复位*/
	delay(200);
	LCD_RST_SET;		    /*复位完毕*/
	delay(50);        
	write_com(0xe2);	 /*软复位*/
	delay(5);
	write_com(0x2c);  /*升压步聚1*/
	delay(5);	
	write_com(0x2e);  /*升压步聚2*/
	delay(5);
	write_com(0x2f);  /*升压步聚3*/
	delay(5);
	write_com(0x22);  /*粗调对比度，可设置范围0x20～0x27*/
	write_com(0x81);  /*微调对比度*/
	write_com(0x2a);  /*0x22,微调对比度的值，可设置范围0x00～0x3f*/
	write_com(0xa2);  /*1/9偏压比（bias）*/
	write_com(0xc8);  /*行扫描顺序：从上到下*/
	write_com(0xa0);  /*列扫描顺序：从左到右*/
	write_com(0x40);  /*起始行：第一行开始*/
	write_com(0xaf);  /*开显示*/

	LcdClear();

	LcdClearDisplay();

}


//========================================================
void write_com(uint8_t cmd)
{
	char i;
	LCD_CS_CLR;
	LCD_RS_CLR;
	for(i=0;i<8;i++)
	{
		LCD_SCK_CLR;
		if(cmd&0x80)
		{	LCD_SDA_SET;}
		else 
		{	LCD_SDA_CLR;	}	
		LCD_SCK_SET;
		delay_us(1);		
		cmd=cmd<<1;
	}
	LCD_CS_SET;

}
//========================================================
void write_data(uint8_t data)
{
	char i;
	LCD_CS_CLR;
	LCD_RS_SET;
	for(i=0;i<8;i++)
	{
		LCD_SCK_CLR;
		if(data&0x80)
		{		LCD_SDA_SET;}
		else 
		{	LCD_SDA_CLR;	}	
		LCD_SCK_SET;
		delay_us(1);		
		data=data<<1;
	}
	LCD_CS_SET;

}




void LcdUpdate(void)
{

	uint8_t i,j;
	uint8_t *dp=lcd_framebuf[0];
	
	for(j=0;j<8;j++)
	{
		LCD_Address(j+1,1);
		for(i=0;i<128;i++)
		{
			write_data(*dp);
			dp++;
		}
	}

}

void LcdClear(void)  
{  
	uint16_t i, n;  
	for(i=0;i<LCD_HEIGHT/8;i++)
	{	for(n=0;n<LCD_WIDHT;n++)
		{	lcd_framebuf[i][n]=0;
			// lcd_framebuf[i][n];	
			}
		}
}


void LcdClearDisplay(void)
{
	unsigned char i,j;
	for(j=0;j<8;j++)
	{
		LCD_Address(j+1,1);
		for(i=0;i<128;i++)
		{
			write_data(0x00);
		}
	}


}

/*
*********************************************************************************************************
*	�� �� ��: lcdDrawPoint
*	����˵��: ���㺯��
*	��    ��: uint8_t x��ˮƽλ��0~127
*			  uint8_t y����ֱλ��0~63
*			  uint8_t dot���Ƿ���䡣1-��䣬0-���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void lcdDrawPoint(uint8_t x, uint8_t y, uint8_t dot)
{
	uint8_t pos, bx, temp=0;
	
	if(x>LCD_WIDHT || y>LCD_HEIGHT)	
		return;	//������ʾ��Χ
	/*	
	pos =7-y/8;
	bx = y%8;
	temp = 1 << (7 - bx);*/

	pos=y/8;
	bx = y%8;
	temp = 1 << (bx);

	
	
	if(dot) 
		lcd_framebuf[pos][x] |= temp;
	else 
		lcd_framebuf[pos][x] &= ~temp;	    
}


void LcdDrawLine(uint8_t x, uint8_t y, uint8_t wide,uint8_t high)
{
	uint16_t xx, yy;
	
	if(x+wide>LCD_WIDHT || y+high>LCD_HEIGHT)	
		return;	//������ʾ��Χ
	for(xx=x;xx<x+wide;xx++)	
	{
		for(yy=y;yy<y+high;yy++)
     		 	lcdDrawPoint(xx,yy,1);
		}
}

void LcdDrawFrame(uint8_t x, uint8_t y, uint8_t wide,uint8_t high)
{
	uint16_t xx, yy;
	
	if(x+wide>LCD_WIDHT || y+high>LCD_HEIGHT)	
		return;	//������ʾ��Χ
	for(xx=x;xx<x+wide+1;xx++)	
	{
     	 lcdDrawPoint(xx,y,1);
		 lcdDrawPoint(xx,y+high,1);
		}

	for(yy=y;yy<y+high;yy++)	
	{
     	 lcdDrawPoint(x,yy,1);
		 lcdDrawPoint(x+wide,yy,1);
		}
}

void LcdDrawRegion(uint8_t x, uint8_t y, uint8_t wide,uint8_t high,uint8_t dot)
{
	uint8_t xx, yy;
	
	if(x+wide>LCD_WIDHT || y+high>LCD_HEIGHT)	
		return;	//������ʾ��Χ
	for(xx=x;xx<x+wide;xx++)	
	{
		for(yy=y;yy<y+high;yy++)
     		 	lcdDrawPoint(xx,yy,dot);
		}
}



/*
*********************************************************************************************************
*	�� �� ��: lcdShowChar
*	����˵��: ��ָ��λ����ʾһ���ַ�, ���������ַ�����������ʾ
*	��    ��: uint8_t x��ˮƽλ��0~192
*			  uint8_t y����ֱλ��0~63
*			  uint8_t chr����ʾ���ַ�
*			  uint8_t size���ֺŴ�С����λ�����ء��ֺţ�12,16,32
*			  uint8_t mode����ʾģʽ��0-������ʾ��1-������ʾ
*	�� �� ֵ: ��
*********************************************************************************************************
*/				 
void lcdShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size, uint8_t mode)
{      			    
	uint8_t temp, t, t1, width;
	uint8_t y0=y;
	
	if(size == 12) 
	{
		width = 1;
	}
	else if(size == 32)
	{				
		width = 2;
	}
	else 
	{
		width = 1;
	}	
	chr = chr-' ';	//�õ�ƫ�ƺ��ֵ
	
    for(t=0; t<size*width; t++)
    {
		if(size == 10) 
		{
			temp = asc2_1005[chr][t];	//����1206����
		}
		else if(size == 12) 
		{
			temp = oled_asc2_1206[chr][t];	//����1206����
		}
		else 
		{
			temp=oled_asc2_1608[chr][t];	//����1608����
		}

		for(t1=0; t1<8; t1++)
		{
			if(temp&0x80)
				lcdDrawPoint(x, y, mode);
			else
				lcdDrawPoint(x, y, !mode);
			temp<<= 1;
			y++;
			if((y-y0) == size)
			{
				y=y0;
				x++;
				break;
			}
		}

    }
}

void LcdShowAscStrs(uint8_t x, uint8_t y, uint8_t* str,uint8_t fontsize)
{
	uint8_t i, size=strlen(str);

	for(i=0;i<size;i++)
	{
		if(fontsize==10)
		 	lcdShowChar(x+i*5,  y, str[i], fontsize, 1);
		else if(fontsize==12)
		 	lcdShowChar(x+i*6,  y, str[i], fontsize, 1);
		else if(fontsize==32)
			lcdShowChar(x+i*16,  y, str[i], fontsize, 1);
		else
			lcdShowChar(x+i*8,  y, str[i], fontsize, 1);

		}
}

void LcdShowInvtAscStrs(uint8_t x, uint8_t y, uint8_t* str,uint8_t fontsize,uint8_t dot)
{
	uint8_t i, size=strlen(str);

	for(i=0;i<size;i++)
	{
		if(fontsize==12)
		 	lcdShowChar(x+i*6,  y, str[i], fontsize, dot);
		else if(fontsize==32)
			lcdShowChar(x+i*16,  y, str[i], fontsize, dot);
		else
			lcdShowChar(x+i*8,  y, str[i], fontsize, dot);

		}
}

void LcdDrawBmp(unsigned char x,unsigned char y,unsigned char width,unsigned char high,const unsigned char *buff)
{
	 unsigned int buf_index=0;
 	 uint8_t x_pos=0,y_pos=0,bit=0,temp=0;

	for(x_pos=0;x_pos<width;x_pos++)
	{
			
			for(y_pos=0;y_pos<high;y_pos++)
			{	 
				buf_index=((uint16_t)y_pos)/(uint16_t)8*(uint16_t)width+(uint16_t)x_pos;
				bit=(uint8_t)((y_pos)%(uint8_t)8);
				temp=(uint8_t)(1<<(bit));   
				temp=(uint8_t)(temp&buff[buf_index]);
				lcdDrawPoint(x+x_pos,y+y_pos,temp);

				}
		}
}



#endif


















