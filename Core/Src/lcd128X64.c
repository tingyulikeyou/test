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
	write_com(0xb0+page);   			//è®¾ç½®é¡µåœ°å€ã€‚æ¯é¡µæ˜¯8è¡Œã€‚ä¸€ä¸ªç”»é¢çš„64è¡Œè¢«åˆ†æˆ8ä¸ªé¡µã€‚æˆ‘ä»¬å¹³å¸¸æ‰€è¯´çš„ç¬¬1é¡µï¼Œåœ¨LCDé©±åŠ¨ICé‡Œæ˜¯ç¬¬0é¡µï¼Œæ‰€ä»¥åœ¨è¿™é‡Œå‡å»1*/
	write_com(((column>>4)&0x0f)+0x10);	//è®¾ç½®åˆ—åœ°å€çš„é«˜4ä½
	write_com(column&0x0f);				//è®¾ç½®åˆ—åœ°å€çš„ä½4ä½
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
	write_com(0xac);/*é™æ€å›¾æ ‡å…³é—­*/
	write_com(0x00);/*é™æ€å›¾æ ‡å¯„å­˜å™¨è®¾ç½®ï¼šå…³é—­ã€‚æ­¤æŒ‡ä»¤ä¸ä¸Šè¿°æŒ‡ä»¤ä¸€èµ·å®Œæˆé™æ€å›¾æ ‡å…³é—­*/
	write_com(0xae);/*æ˜¾ç¤ºï¼šå…³*/
	write_com(0xa5);/*å…¨å±æ˜¾ç¤ºï¼šå¼€*/

}

void LCD_DisplayWake(void)
{
	write_com(0xa4);/*å…¨å±æ˜¾ç¤ºï¼šå…³ã€‚è¿›å…¥æ­£å¸¸æ¨¡å¼*/
	write_com(0xad);/*é™æ€å›¾æ ‡å¼€å¯*/
	write_com(0x03);/*é™æ€å›¾æ ‡å¯„å­˜å™¨è®¾ç½®ï¼šå¼€ã€‚æ­¤æŒ‡ä»¤ä¸ä¸Šè¿°æŒ‡ä»¤ä¸€èµ·å®Œæˆé™æ€å›¾æ ‡å¼€å¯*/
	write_com(0xaf);/*æ˜¾ç¤ºï¼šå¼€*/

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
	LCD_RST_CLR;        /*ä½ç”µå¹³å¤ä½*/
	delay(200);
	LCD_RST_SET;		    /*å¤ä½å®Œæ¯•*/
	delay(50);        
	write_com(0xe2);	 /*è½¯å¤ä½*/
	delay(5);
	write_com(0x2c);  /*å‡å‹æ­¥èš1*/
	delay(5);	
	write_com(0x2e);  /*å‡å‹æ­¥èš2*/
	delay(5);
	write_com(0x2f);  /*å‡å‹æ­¥èš3*/
	delay(5);
	write_com(0x22);  /*ç²—è°ƒå¯¹æ¯”åº¦ï¼Œå¯è®¾ç½®èŒƒå›´0x20ï½0x27*/
	write_com(0x81);  /*å¾®è°ƒå¯¹æ¯”åº¦*/
	write_com(0x2a);  /*0x22,å¾®è°ƒå¯¹æ¯”åº¦çš„å€¼ï¼Œå¯è®¾ç½®èŒƒå›´0x00ï½0x3f*/
	write_com(0xa2);  /*1/9åå‹æ¯”ï¼ˆbiasï¼‰*/
	write_com(0xc8);  /*è¡Œæ‰«æé¡ºåºï¼šä»ä¸Šåˆ°ä¸‹*/
	write_com(0xa0);  /*åˆ—æ‰«æé¡ºåºï¼šä»å·¦åˆ°å³*/
	write_com(0x40);  /*èµ·å§‹è¡Œï¼šç¬¬ä¸€è¡Œå¼€å§‹*/
	write_com(0xaf);  /*å¼€æ˜¾ç¤º*/

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
*	º¯ Êı Ãû: lcdDrawPoint
*	¹¦ÄÜËµÃ÷: »­µãº¯Êı
*	ĞÎ    ²Î: uint8_t x£ºË®Æ½Î»ÖÃ0~127
*			  uint8_t y£º´¹Ö±Î»ÖÃ0~63
*			  uint8_t dot£ºÊÇ·ñÌî³ä¡£1-Ìî³ä£¬0-Çå¿Õ
*	·µ »Ø Öµ: ÎŞ
*********************************************************************************************************
*/
void lcdDrawPoint(uint8_t x, uint8_t y, uint8_t dot)
{
	uint8_t pos, bx, temp=0;
	
	if(x>LCD_WIDHT || y>LCD_HEIGHT)	
		return;	//³¬³öÏÔÊ¾·¶Î§
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
		return;	//³¬³öÏÔÊ¾·¶Î§
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
		return;	//³¬³öÏÔÊ¾·¶Î§
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
		return;	//³¬³öÏÔÊ¾·¶Î§
	for(xx=x;xx<x+wide;xx++)	
	{
		for(yy=y;yy<y+high;yy++)
     		 	lcdDrawPoint(xx,yy,dot);
		}
}



/*
*********************************************************************************************************
*	º¯ Êı Ãû: lcdShowChar
*	¹¦ÄÜËµÃ÷: ÔÚÖ¸¶¨Î»ÖÃÏÔÊ¾Ò»¸ö×Ö·û, °üÀ¨²¿·Ö×Ö·û¡£²»¸üĞÂÏÔÊ¾
*	ĞÎ    ²Î: uint8_t x£ºË®Æ½Î»ÖÃ0~192
*			  uint8_t y£º´¹Ö±Î»ÖÃ0~63
*			  uint8_t chr£ºÏÔÊ¾µÄ×Ö·û
*			  uint8_t size£º×ÖºÅ´óĞ¡£¬µ¥Î»£ºÏñËØ¡£×ÖºÅ£º12,16,32
*			  uint8_t mode£ºÏÔÊ¾Ä£Ê½¡£0-·´°×ÏÔÊ¾£¬1-Õı³£ÏÔÊ¾
*	·µ »Ø Öµ: ÎŞ
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
	chr = chr-' ';	//µÃµ½Æ«ÒÆºóµÄÖµ
	
    for(t=0; t<size*width; t++)
    {
		if(size == 10) 
		{
			temp = asc2_1005[chr][t];	//µ÷ÓÃ1206×ÖÌå
		}
		else if(size == 12) 
		{
			temp = oled_asc2_1206[chr][t];	//µ÷ÓÃ1206×ÖÌå
		}
		else 
		{
			temp=oled_asc2_1608[chr][t];	//µ÷ÓÃ1608×ÖÌå
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


















