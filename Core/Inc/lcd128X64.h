#ifndef __LCD_H
#define __LCD_H		

#ifdef LCD128X64_SUPPORT

////////////////////////////////////////////////////////////////////
//-----------------LCD�˿ڶ���---------------- 
//#define GPIO_TYPE  GPIOB  //GPIO������
//#define LED      10        //�����������      PC10
#define LCD_CS   1       //Ƭѡ����            PB1
#define LCD_RS   10      //�Ĵ���/����ѡ������ PB10 
#define LCD_RST  2       //��λ����            PB2
#define LCD_WR   11      //д����              PB11
#define LCD_RD   0       //������              PB0

//���ʹ�ùٷ��⺯���������еײ㣬�ٶȽ����½���14֡ÿ�룬���������˾�Ƽ�����
//����IO����ֱ�Ӳ����Ĵ���������IO������ˢ�����ʿ��Դﵽ28֡ÿ�룡 
//GPIO��λ�����ߣ�
#define	LCD_CS_SET   HAL_GPIO_WritePin(LCD_CS_GPIO_Port,LCD_CS_Pin,GPIO_PIN_SET) //Ƭѡ�˿�  	
#define	LCD_RS_SET	 HAL_GPIO_WritePin(LCD_RS_GPIO_Port,LCD_RS_Pin,GPIO_PIN_SET)   //����/����    
#define	LCD_RST_SET	 HAL_GPIO_WritePin(LCD_RST_GPIO_Port,LCD_RST_Pin,GPIO_PIN_SET)   //��λ			  
#define	LCD_WR_SET	// HAL_GPIO_WritePin(lcd_wr_,GPIO_PIN_SET)  //д 	
#define LCD_RD_SET   //HAL_GPIO_WritePin(LCD_rdPIO_Port,GPIO_PIN_SET)    //��	
#define LCD_SCK_SET  HAL_GPIO_WritePin(LCD_CLK_GPIO_Port,LCD_CLK_Pin,GPIO_PIN_SET)  
#define LCD_SDA_SET  HAL_GPIO_WritePin(LCD_DAT_GPIO_Port,LCD_DAT_Pin,GPIO_PIN_SET)  

//GPIO��λ�����ͣ�							    
#define	LCD_CS_CLR  	HAL_GPIO_WritePin(LCD_CS_GPIO_Port,LCD_CS_Pin,GPIO_PIN_RESET)     //Ƭѡ�˿�  	
#define	LCD_RS_CLR		HAL_GPIO_WritePin(LCD_RS_GPIO_Port,LCD_RS_Pin,GPIO_PIN_RESET)    //����/����  	 
#define	LCD_RST_CLR		HAL_GPIO_WritePin(LCD_RST_GPIO_Port,LCD_RST_Pin,GPIO_PIN_RESET)  //��λ			  
#define	LCD_WR_CLR		//HAL_GPIO_WritePin(LCD_CS_GPIO_Port,LCD_CS_Pin,GPIO_PIN_RESET) //д
#define	LCD_RD_CLR  	//HAL_GPIO_WritePin(LCD_CS_GPIO_Port,LCD_CS_Pin,GPIO_PIN_RESET)  //��	
#define LCD_SCK_CLR    HAL_GPIO_WritePin(LCD_CLK_GPIO_Port,LCD_CLK_Pin,GPIO_PIN_RESET)  
#define LCD_SDA_CLR    HAL_GPIO_WritePin(LCD_DAT_GPIO_Port,LCD_DAT_Pin,GPIO_PIN_RESET)  






#define LCD_WIDHT    128
#define LCD_HEIGHT   64

typedef struct
{
    const uint8_t *strdata;
     uint8_t width;
     uint8_t high;	 
}BMP_ICON;

void delay_us(uint16_t us);
	    															  
void LCD_Init(void);
void LCD_DisplayOff(void);
void LCD_DisplayWake(void);

void LCD_Address(uint8_t page, uint8_t column);



void write_com(uint8_t cmd);
void write_data(uint8_t data);
void delay(uint16_t t);

void clealddram(void);
void LcdUpdate(void);
void LcdClear(void)  ;

void lcdDrawPoint(uint8_t x, uint8_t y, uint8_t dot);
void lcdShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size, uint8_t mode);
void Lcd_ShowZH(uint8_t x, uint8_t y, const char *p);
void LcdDrawLine(uint8_t x, uint8_t y, uint8_t wide,uint8_t high);
void LcdShowAscStrs(uint8_t x, uint8_t y, uint8_t* str,uint8_t fontsize);
void LcdDrawFrame(uint8_t x, uint8_t y, uint8_t wide,uint8_t high);
void LcdDrawRegion(uint8_t x, uint8_t y, uint8_t wide,uint8_t high,uint8_t dot);
void LcdClearDisplay(void);
void LcdShowInvtAscStrs(uint8_t x, uint8_t y, uint8_t* str,uint8_t fontsize,uint8_t dot);
void LcdDrawBmp(unsigned char x,unsigned char y,unsigned char width,unsigned char high,const unsigned char *buff);
#endif
#endif  





 

