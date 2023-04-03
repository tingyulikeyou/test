#include "main.h"



const uint8_t  _pbLcdMode[] = {
    #ifdef DC_PUMP_SUPPORT
	0x00,0x80,// point 4
	0x02,0x80,// point 3
	0x04,0x80,// point 2
	0x06,0x80,// point 1
	0x08,0x01,// TCTH  TKFULL
	0x08,0x02,// SOLAR
	0x08,0x04,// PUMPRUN
	0x08,0x08,// FAULT 
	0x08,0x10,// WCWH
	#else
	0x09,0x02,// 0%
	0x09,0x01,// 20%
	0x09,0x04,// 40%
	0x09,0x08,// 60%
	0x011,0x08,//80%
	0x0A,0x01,// CLOCK
	0x0A,0x02,// BLE
	0x0A,0x04,// WIFI
	0x0A,0x08,// UNLOCK
	0x0B,0x01,// C
	0x0B,0x02,// H our
	0x0B,0x04,// h our
	0x0B,0x08,// POWER
	0x0C,0x01,// d
	0x0C,0x02,// A
	0x0C,0x04,// W
	0x0C,0x08,// LOCK
	0x0D,0x01,// %
	0x0D,0x02,// V
	0x0D,0x04,// k
	0x0D,0x08,// LINE
	0x0F,0x08,// point
	0x13,0x08,// charge
	0x15,0x08,// eng
	#endif
};


const uint8_t  _pbLcdSeg[4][14] = {
	#ifdef DC_PUMP_SUPPORT
	{0x06,0x01,0x06,0x02,0x06,0x04,0x06,0x08,0x06,0x10,0x06,0x20,0x06,0x40,}, // 4 
	{0x04,0x01,0x04,0x02,0x04,0x04,0x04,0x08,0x04,0x10,0x04,0x20,0x04,0x40,}, // 3  
	{0x02,0x01,0x02,0x02,0x02,0x04,0x02,0x08,0x02,0x10,0x02,0x20,0x02,0x40,}, // 2 
	{0x00,0x01,0x00,0x02,0x00,0x04,0x00,0x08,0x00,0x10,0x00,0x20,0x00,0x40,}, // 1 
	#else
	{0x14,0x08,0x14,0x04,0x14,0x02,0x14,0x01,0x15,0x01,0x15,0x04,0x15,0x02,}, // 1  
	{0x12,0x08,0x12,0x04,0x12,0x02,0x12,0x01,0x13,0x01,0x13,0x04,0x13,0x02,},// 2  
	{0x10,0x08,0x10,0x04,0x10,0x02,0x10,0x01,0x11,0x01,0x11,0x04,0x11,0x02,},// 3    
	{0x0E,0x08,0x0E,0x04,0x0E,0x02,0x0E,0x01,0x0F,0x01,0x0F,0x04,0x0F,0x02,}// 4
	#endif
};


 const uint8_t _pbSegUnit[] = {
(1<<DOT_G),//|(1<<DOT_C),                                               // -
0,                                                                   // clear all  space
(1<<DOT_A)|(1<<DOT_B)|(1<<DOT_C)|(1<<DOT_D)|(1<<DOT_E)|(1<<DOT_F),    // 0
(1<<DOT_B)|(1<<DOT_C),                                               // 1
(1<<DOT_A)|(1<<DOT_B)|(1<<DOT_D)|(1<<DOT_E)|(1<<DOT_G),              // 2
(1<<DOT_A)|(1<<DOT_B)|(1<<DOT_C)|(1<<DOT_D)|(1<<DOT_G),              // 3
(1<<DOT_B)|(1<<DOT_C)|(1<<DOT_F)|(1<<DOT_G),                         // 4
(1<<DOT_A)|(1<<DOT_C)|(1<<DOT_D)|(1<<DOT_F)|(1<<DOT_G),              // 5
(1<<DOT_A)|(1<<DOT_C)|(1<<DOT_D)|(1<<DOT_E)|(1<<DOT_F)|(1<<DOT_G),   // 6
(1<<DOT_A)|(1<<DOT_B)|(1<<DOT_C),                                    // 7
(1<<DOT_A)|(1<<DOT_B)|(1<<DOT_C)|(1<<DOT_D)|(1<<DOT_E)|(1<<DOT_F)|(1<<DOT_G), // 8
(1<<DOT_A)|(1<<DOT_B)|(1<<DOT_C)|(1<<DOT_F)|(1<<DOT_G)|(1<<DOT_D),              // 9
(1<<DOT_A)|(1<<DOT_B)|(1<<DOT_C)|(1<<DOT_E)|(1<<DOT_F)|(1<<DOT_G),     // A
/*(1<<DOT_A)|(1<<DOT_B)|*/(1<<DOT_C)|(1<<DOT_D)|(1<<DOT_E)|(1<<DOT_F)|(1<<DOT_G), // B
(1<<DOT_A)|(1<<DOT_D)|(1<<DOT_E)|(1<<DOT_F),                   // C
/*(1<<DOT_A)|*/(1<<DOT_B)|(1<<DOT_C)|(1<<DOT_D)|(1<<DOT_E)/*|(1<<DOT_F)*/|(1<<DOT_G),    // D
(1<<DOT_A)|(1<<DOT_D)|(1<<DOT_E)|(1<<DOT_F)|(1<<DOT_G),              // E
(1<<DOT_A)|(1<<DOT_E)|(1<<DOT_F)|(1<<DOT_G),                         // F
(1<<DOT_A)|(1<<DOT_C)|(1<<DOT_D)|(1<<DOT_E)|(1<<DOT_F),              // G
(1<<DOT_B)|(1<<DOT_C)|(1<<DOT_E)|(1<<DOT_F)|(1<<DOT_G),              // H
(1<<DOT_B)|(1<<DOT_C),                                               // I
(1<<DOT_B)|(1<<DOT_C)|(1<<DOT_D)|(1<<DOT_E),                         // J
(1<<DOT_B)|(1<<DOT_C)|(1<<DOT_D)|(1<<DOT_E),                         // K   --no
(1<<DOT_D)|(1<<DOT_E)|(1<<DOT_F),                                    // L
/*(1<<DOT_B)|*/(1<<DOT_C)|(1<<DOT_G)|(1<<DOT_E),                         // M   --no
/*(1<<DOT_B)|*/(1<<DOT_C)|(1<<DOT_G)|(1<<DOT_E),                         // N   --no
(1<<DOT_A)|(1<<DOT_B)|(1<<DOT_C)|(1<<DOT_D)|(1<<DOT_E)|(1<<DOT_F),   // 0
(1<<DOT_A)|(1<<DOT_B)|(1<<DOT_E)|(1<<DOT_F)|(1<<DOT_G),              // P
(1<<DOT_B)|(1<<DOT_C)|(1<<DOT_A)|(1<<DOT_F)|(1<<DOT_G),                         // Q   --no
/*(1<<DOT_B)|(1<<DOT_D)|*/(1<<DOT_G)|(1<<DOT_E),                         // R   --no
(1<<DOT_A)|(1<<DOT_C)|(1<<DOT_D)|(1<<DOT_F)|(1<<DOT_G),              // S
(1<<DOT_D)|(1<<DOT_E)|(1<<DOT_F)|(1<<DOT_G),                         // T  -t
(1<<DOT_B)|(1<<DOT_C)|(1<<DOT_D)|(1<<DOT_E)|(1<<DOT_F),              // U
/*(1<<DOT_B)|*/(1<<DOT_C)|(1<<DOT_D)|(1<<DOT_E)/*|(1<<DOT_F)*/,              // V
(1<<DOT_B)|(1<<DOT_C)|(1<<DOT_D)|(1<<DOT_E),                         // W  --no
(1<<DOT_B)|(1<<DOT_C)|(1<<DOT_E)|(1<<DOT_F)|(1<<DOT_G),              // X
/*(1<<DOT_B)|*/(1<<DOT_D)|(1<<DOT_E)|(1<<DOT_G),                         // Y  --no
(1<<DOT_A)|(1<<DOT_B)|(1<<DOT_F)|(1<<DOT_G),                         // Z  --no
 	};


uint8_t  Lcd_Buffer[LCD_BUFFER_SIZE] ;
// xdata u8  Lcd_Reg[LCD_BUFFER_SIZE] //_at_ 0x1E0;


void Delayus(void)// 2us
{
        uint8_t i;
        for(i=0;i<20;i++);
}


void LcdInit(void) 
{
  #ifndef DC_PUMP_SUPPORT 
 //for SYS_EN
  P_Strobe(FALSE); 
  LCDSendCom(LCD_CMD, 3);
  LCDSendCom(LCD_SYS_EN, 8);
  LCDSendCom(0, 1);
  P_Strobe(TRUE); 

  //for LCD_ON
  P_Strobe(FALSE); 
  LCDSendCom(LCD_CMD, 3);
  LCDSendCom(LCD_ON, 8);
  LCDSendCom(0, 1);
  P_Strobe(TRUE); 
  
  //for TIMER_EN
  P_Strobe(FALSE); 
  LCDSendCom(LCD_CMD, 3);
  LCDSendCom(LCD_TIMER_EN, 8);
  LCDSendCom(0, 1);
  P_Strobe(TRUE); 
  
   //for  rc 256k
  P_Strobe(FALSE); 
  LCDSendCom(LCD_CMD, 3);
  LCDSendCom(LCD_RC_256K, 8);
  LCDSendCom(0, 1);
  P_Strobe(TRUE);

  //for bias 1/3, 4com
  P_Strobe(FALSE); 
  LCDSendCom(LCD_CMD, 3);
  LCDSendCom(LCD_BIAS13_COM4, 8);
  LCDSendCom(0, 1);
  P_Strobe(TRUE);

  //for TNORMAL 
  P_Strobe(FALSE); 
  LCDSendCom(LCD_CMD, 3);
  LCDSendCom(LCD_TNORMAL, 8);
  LCDSendCom(0, 1);
  P_Strobe(TRUE);



  P_Strobe(FALSE); 
  LCDSendCom(LCD_CMD, 3);
  LCDSendCom(LCD_TONE_2K, 8);
  LCDSendCom(0, 1);
  P_Strobe(TRUE); 

  #else
  P_Strobe(FALSE);        
  LedWrite(MODESET);
  P_Strobe(TRUE);          
  
  P_Strobe(FALSE);        
  LedWrite(LED_CONTROL);

  P_Strobe(TRUE);          // strobe pull high to release the mode set

  #endif


  LcdClearAll();
  LcdUpdateAll();

}




void LCDSendCom(uint8_t bVal, uint8_t bCnt)
{
  uint8_t  i;
  bVal <<=(8-bCnt);

  for (i = 0; i < bCnt; i++)
  {
    P_Clk(FALSE);

    P_Data(bVal & 0x80);

    bVal <<= 1; 
    P_Clk(TRUE);

  }
}

 void LCDSendAddr(uint8_t Addr) 
{
  uint8_t  i; 
  
  Addr <<= 2;
  for (i = 0; i < 6; i++)       //debug !!!
  {
    P_Clk(FALSE);

   if(Addr & 0x80)
    {
      P_Data(1);
    }
    else
    {
      P_Data(0);
    }

    Addr <<= 1;
    
    P_Clk(TRUE);
  }
}

 void LCDSendData(uint8_t bData,uint8_t bCnt) 
{
  uint8_t  i;
  
  for (i = 0; i < bCnt; i++)
  {
     P_Clk(FALSE);


     //DATA_PIN =(bData & 0x01);
     P_Data(bData & 0x01);
     bData >>= 1;

     P_Clk(TRUE);

  }

  P_Data(TRUE);
}


/*void LcdInit(void)
{
    u8 i;

    for(i=0;i<LCD_BUFFER_SIZE;i++)
    	{
    	    Lcd_Buffer[i]=0x00;
    	}

	LcdUpdateAll();

}*/

void LcdFillAll(void)
{
    uint8_t i;

    for(i=0;i<LCD_BUFFER_SIZE;i++)
    	{
    	
    	    Lcd_Buffer[i]=0xff;
    	}
}

void LcdClearAll(void)
{
    uint8_t i;

    for(i=0;i<LCD_BUFFER_SIZE;i++)
    	{
    	    Lcd_Buffer[i]=0x00;
    	}

}


/*void LcdWaterFlow(BYTE rotate)
{
     u8   i,addr;
    for(i=0;i<2;i++)
     {  
         addr=_pbWater[i*2]-0x80; 
         if(i!=rotate)
	     { 
			  Lcd_Buffer[addr]|= _pbWater[i*2 + 1];	 
         	}
		 else
		 {
		        Lcd_Buffer[addr]&= ~_pbWater[i*2 + 1];	 
		 	}
    	}
}*/


void LcdSetIcon(uint8_t point,uint8_t enble)
{
      uint8_t addr;
	if(point==ICON_NONE)
		return ;
	
  	addr=_pbLcdMode[point*2];
	
  	if(_pbLcdMode[point]!=0xff)
        {
	         if(enble)
	             Lcd_Buffer[addr]|= _pbLcdMode[point*2 + 1];
	         else
	             Lcd_Buffer[addr]&=~( _pbLcdMode[point*2 + 1]);
        	}

}

void ShowStr(uint8_t *str)
{
	uint8_t i;
	uint8_t lenght=strlen((char*)str);

	if(lenght>4)
		lenght=4;
	
	for(i=0;i<lenght;i++)
		LcdShowChar(3-i,str[lenght-i-1]);

}

 void LcdShowChar(uint8_t pos,uint8_t str)
{
   uint8_t /*i,*/Seg_i;

  uint8_t SegData,temp;

  uint8_t DecodePos=0;


   if(str==' ')
           DecodePos=Space_Pos;
   else if((str>='0')&&(str<='9'))
           DecodePos=str-'0'+Number_Pos;
   else if((str>='A')&&(str<='Z'))
           DecodePos=str-'A'+Letter_Pos;
    else if((str>='a')&&(str<='z'))
           DecodePos=str-'a'+Letter_Pos;
   else if(str=='-')
           DecodePos=Line_Pos;
   else
   	    DecodePos=Space_Pos;

	SegData=_pbSegUnit[DecodePos];
	for(Seg_i=0;Seg_i<SEG_SIZE;Seg_i++)
	{
		
		temp=_pbLcdSeg[pos][Seg_i*2];

		if(temp>=LCD_BUFFER_SIZE)
			temp=0;

		if(SegData&0x01)
		{       
 			Lcd_Buffer[temp]|=_pbLcdSeg[pos][Seg_i*2+1];	
		}
		else
			Lcd_Buffer[temp]&=~_pbLcdSeg[pos][Seg_i*2+1];	
		
		SegData=SegData>>1;
		}

}


  void LcdShowNumber(uint8_t pos,uint8_t number)
{
   uint8_t /*i,*/Seg_i;

  uint8_t SegData,temp;

	uint8_t DecodePos=0;

	//number=number&0x0f;

	if(/*(number>=0x00)&&*/(number<=0x09))
		DecodePos=number+Number_Pos;
	else if((number>=0x0a)&&(number<=0x0f))
		DecodePos=number-0x0a+Letter_Pos;

	SegData=_pbSegUnit[DecodePos];
	for(Seg_i=0;Seg_i<SEG_SIZE;Seg_i++)
	{
		
		temp=_pbLcdSeg[pos][Seg_i*2];

		if(temp>=LCD_BUFFER_SIZE)
			temp=0;

		if(SegData&0x01)
		{       
 			Lcd_Buffer[temp]|=_pbLcdSeg[pos][Seg_i*2+1];	
		}
		else
			Lcd_Buffer[temp]&=~_pbLcdSeg[pos][Seg_i*2+1];	
		SegData=SegData>>1;
	}


}

  void LcdTestPoint(uint8_t addr,uint8_t test)
  {
  	 Lcd_Buffer[addr]|=(0x01<<test);
  //	u8 i=0;
//	for(i=0;i<3;i++)
  //	  Lcd_Buffer[i] =0x0f;

   //LcdInit();

/*  while(1)
  	{
   
  for(y=0;y<10;y++) 
{  for(x=0;x<12;x++)
  	{
  		LcdShowNumber(x,y);
  	}
 for(water_stop=0;water_stop<0x05ff;water_stop++);  	
 LcdUpdateAll();
  	}
  	}
   while(1);*/
   /*{
   	for(x=0;x<19;x++)
   	{
   		for(y=0;y<8;y++)
		{	LcdTestPoint(x,y);
			for(water_stop=0;water_stop<0x05ff;water_stop++);
   			}
   		}
	
   	}*/ 
  	}





  void LcdUpdateAll(void)
  {
	uint8_t  i;
	P_Strobe(FALSE);
	LCDSendCom(LCD_WRITE,3);
	LCDSendAddr(0x00);

	for(i=0;i<LCD_BUFFER_SIZE;i++)
	{
		LCDSendData(Lcd_Buffer[i],4); 
	}
	P_Strobe(TRUE);

  }

  void LedWrite(uint8_t bTmp)
{
  uint8_t i;
  
  for (i = 0; i < 8; i++)
  {
    P_Clk(FALSE);

    P_Data(bTmp&0x01);
    bTmp >>= 1;

    P_Clk(TRUE);
  }

  P_Data(TRUE);
}	

   void LedWriteRam(uint8_t     bAddr, uint8_t bData) 
  {
	if(bAddr == 0xff)
	{
	  return;
	}

	P_Strobe(FALSE);
	LedWrite(DATA_SET_INC) ;  
	P_Strobe(TRUE);
  
	
	P_Strobe(FALSE);
	LedWrite(ADDR_SET | bAddr); 
	P_Strobe(FALSE);
  
	LedWrite(bData);	
	P_Strobe(TRUE);
  }


   
void LedUpdateAll(void)
{
   uint8_t i;	
 	for(i=0;i<LED_BUFFER_SIZE;i++)
	{	
	       //Lcd_Buffer[i]=0x00;
	       LedWriteRam(i,Lcd_Buffer[i]);  
    	}	
}



