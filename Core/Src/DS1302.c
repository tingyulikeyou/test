#include "main.h"
#include "string.h"

DS1302_TIME Read_T;
DS1302_TIME Write_T;

uint8_t Sys_Code = 0;

uint8_t g_ds1302_ErrCnt = 0;

unsigned char TimePrint_buf[100];

extern int tian ; 
extern char Camp_ComOK ;
extern SYS_STATE_CODE_TypeDef  g_sysStateCode;


void Delay(__IO uint16_t nCount)
{
    nCount*=200;
    while (nCount != 0)
    {
        nCount--;
    }
}


void SetSysErrorCode(uint8_t error)
{
	Sys_Code=error;
}

uint8_t GetSysErrorCode(void)
{
	return Sys_Code;
}


/****************************************************************************
  Copyright (C), 2016-2018, Omnivoltaic Energy Solutions Tech. Co., Ltd.
	
  Function:       // bcd_to_hex

  Description:    // bcd_to_hex

  Calls:          // Oves_Canp_Command

  Input:          // intdata buffer len

  Output:         // Non
	
  Return:         // caldata

  Others:         // 
	
  Author:      	  // Liu Rongkun(Micheal)
	
  Data:		  // 2018/09
******************************************************************************/
unsigned char bcd_to_hex(unsigned char data)
{
    unsigned char temp;
 
    temp = ((data>>4)*10 + (data&0x0f));
    return temp;
}
/****************************************************************************
  Copyright (C), 2016-2018, Omnivoltaic Energy Solutions Tech. Co., Ltd.
	
  Function:       // itoa

  Description:    // int to str

  Calls:          // Oves_Canp_Command

  Input:          // intdata buffer len

  Output:         // Non
	
  Return:         // caldata

  Others:         // 
	
  Author:      	  // Liu Rongkun(Micheal)
	
  Data:		  // 2018/09
******************************************************************************/
char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }
    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} 
void DS_1302io()
{
	#if 0
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    
    //SCLK
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOB, GPIO_Pin_12);
 
    //RST
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA, GPIO_Pin_0);
 
    /*IO:PB2配置为开漏模式，此模式下能够实现真正的双向IO口*/  //必须上拉 4.7K  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOB, GPIO_Pin_2);
	#endif
}


//写一个字节的数据sck上升沿写数据
void write_1302byte(uint8_t dat)                             
{  
    uint8_t i = 0;  
    HAL_GPIO_WritePin(DS_SCLK_GPIO_Port,DS_SCLK_Pin,GPIO_PIN_RESET); //ds1302clk=0  
    Delay(2);//延时大约2us  
      
    for(i = 0;i < 8;i ++)  
        {  
            HAL_GPIO_WritePin(DS_SCLK_GPIO_Port,DS_SCLK_Pin,GPIO_PIN_RESET);   //ds1302clk=0;  
            if(dat&0x01)  
                HAL_GPIO_WritePin(DS_IO_GPIO_Port,DS_IO_Pin,GPIO_PIN_SET);  
            else                          
                HAL_GPIO_WritePin(DS_IO_GPIO_Port,DS_IO_Pin,GPIO_PIN_RESET);                  
            Delay(2);  
            HAL_GPIO_WritePin(DS_SCLK_GPIO_Port,DS_SCLK_Pin,GPIO_PIN_SET);    //发送一位数据，clk上升沿,//ds1302clk=1  
            dat >>= 1;  
            Delay(1);  
        }     
}




//向DS1302指定寄存器写入一个字节的数据
void write_1302(uint8_t add,uint8_t dat)                            
{  
    HAL_GPIO_WritePin(DS_CE_GPIO_Port,DS_CE_Pin,GPIO_PIN_RESET); //只有在rst为高电平的时候才能进行数据传输  
    HAL_GPIO_WritePin(DS_SCLK_GPIO_Port,DS_SCLK_Pin,GPIO_PIN_RESET); //只有clk为低电平的时候，rst才能被置为高电平  

    Delay(1);                     //略微延时  
    HAL_GPIO_WritePin(DS_CE_GPIO_Port,DS_CE_Pin,GPIO_PIN_SET);  //clk = 0之后，这里将rst拉高，准备传送数据  
 
    Delay(2);                     //时间大约2us  
    write_1302byte(add);             //先发地址  
    write_1302byte(dat);             //然后发数据  
    HAL_GPIO_WritePin(DS_CE_GPIO_Port,DS_CE_Pin,GPIO_PIN_RESET);  //这里释放总线  
    HAL_GPIO_WritePin(DS_SCLK_GPIO_Port,DS_SCLK_Pin,GPIO_PIN_RESET); //拉低clk，以备下一次数据发送  

    Delay(5);  
  
} 


//从DS1302指定寄存器读数据
uint8_t read_1302(uint8_t add)                     
{  
    uint8_t i=0;  
    uint8_t Return_dat=0x00;  
    HAL_GPIO_WritePin(DS_CE_GPIO_Port,DS_CE_Pin,GPIO_PIN_RESET);         //ds1302rst=0;  
    HAL_GPIO_WritePin(DS_SCLK_GPIO_Port,DS_SCLK_Pin,GPIO_PIN_RESET);         //ds1302clk=0;  
    Delay(3);                                 //略微延时2us  
    HAL_GPIO_WritePin(DS_CE_GPIO_Port,DS_CE_Pin,GPIO_PIN_SET);  ;             //ds1302rst=1;  
    Delay(3);                                //时间要大约3us  
    write_1302byte(add);                       //先写寄存器的地址  
    for(i=0;i<8;i++)  
    {  
         HAL_GPIO_WritePin(DS_SCLK_GPIO_Port,DS_SCLK_Pin,GPIO_PIN_SET);      //ds1302clk=1; 
        Delay(5);
        Return_dat >>= 1;  
        Delay(5);
        HAL_GPIO_WritePin(DS_SCLK_GPIO_Port,DS_SCLK_Pin,GPIO_PIN_RESET);      //ds1302clk=0;//拉低时钟线，以便于数据的读入  
        if(HAL_GPIO_ReadPin(DS_IO_GPIO_Port,DS_IO_Pin)==GPIO_PIN_SET)    //数据线此时为高电平  
        {Return_dat = Return_dat|0x80;}  
    }  
      Delay(1);  
      HAL_GPIO_WritePin(DS_CE_GPIO_Port,DS_CE_Pin,GPIO_PIN_RESET);        //ds1302rst=0;释放总线  
      return Return_dat;  
}

/****************************************************************************
  Copyright (C), 2016-2018, Omnivoltaic Energy Solutions Tech. Co., Ltd.
	
  Function:       // write time to ds1302

  Description:    // ds1302_write_time

  Calls:          // Oves_Canp_Command

  Input:          // time

  Output:         // Non
	
  Return:         // caldata

  Others:         // 
	
  Author:      	  // Liu Rongkun(Micheal)
	
  Data:		  // 2019/09
******************************************************************************/
void ds1302_write_time(DS1302_TIME* time) 
{ 
    write_1302(DS1302_CONTROL_REG,0x00); //关闭写保护 
    write_1302(DS1302_SEC_REG,0x80); //暂停 
    write_1302(DS1302_CHARGER_REG,0xa9); //涓流充电 
 
    write_1302(DS1302_SEC_REG, 0x00);           // 启动振荡器
    write_1302(DS1302_YEAR_REG,time->year); //年 
    write_1302(DS1302_MONTH_REG,time->month); //月 
    write_1302(DS1302_DATE_REG,time->day); //日 
    write_1302(DS1302_DAY_REG,time->week); //周 
    write_1302(DS1302_HR_REG,time->hour); //时 
    write_1302(DS1302_MIN_REG,time->minute); //分 
    write_1302(DS1302_SEC_REG,0x00); //秒 
    write_1302(DS1302_CONTROL_REG,0x80); //打开写保护 
}

/****************************************************************************
  Copyright (C), 2016-2018, Omnivoltaic Energy Solutions Tech. Co., Ltd.
	
  Function:       //ds1302_read_time

  Description:    // ds1302_read_time

  Calls:          // Oves_Canp_Command

  Input:          // time

  Output:         // Non
	
  Return:         // caldata

  Others:         // 
	
  Author:      	  // Liu Rongkun(Micheal)
	
  Data:		  // 2019/09
******************************************************************************/
void ds1302_read_time(DS1302_TIME* time) 
{ 
     static uint8_t rec = 0xff, sec_err_cnt = 0;
     uint8_t temp;
	 
    time->year=read_1302(DS1302_YEAR_REG_R); //年 
    time->month=read_1302(DS1302_MONTH_REG_R);//月 
    time->day=read_1302(DS1302_DATE_REG_R); //日 
    time->week=read_1302(DS1302_DAY_REG_R); //周 
    time->hour=read_1302(DS1302_HR_REG_R); //时 
    time->minute=read_1302(DS1302_MIN_REG_R); //分 
    time->second=read_1302(DS1302_SEC_REG_R); //秒  

    temp=time->second;

   if(rec == temp)//和上一秒调用的值一样 32.768K晶体故障
  {
  	if(sec_err_cnt<20)
		sec_err_cnt++;
	if(sec_err_cnt>=10)
   	 	g_sysStateCode.error.rtc_error=1;//Sys_Code = 0xE3;//晶体故障
  }
  else//晶体正常
  {
    	rec = temp;
	sec_err_cnt=0;	

	//if(Sys_Code==0xe3)
		g_sysStateCode.error.rtc_error=0;//Sys_Code=0x00;

  }
}

/****************************************************************************
  Copyright (C), 2016-2018, Omnivoltaic Energy Solutions Tech. Co., Ltd.
	
  Function:       // Show_Time

  Description:    // Show_Time

  Calls:          // Show_Time

  Input:          // intdata buffer len

  Output:         // Non
	
  Return:         // caldata

  Others:         // 
	
  Author:      	  // Liu Rongkun(Micheal)
	
  Data:		  // 2018/09
******************************************************************************/
void Show_Time()
{
  int n = 0,a = 0;
  char temp[8];
  memset(temp, 0 , 10);
  memset(TimePrint_buf, 0 ,100);
  ds1302_read_time (&Read_T);
  Check_Ds1302Runs();
  
  memcpy(TimePrint_buf, "Time: ",  6); n = 6;
  a = bcd_to_hex(Read_T.hour);
  itoa(a,temp,10);
  memcpy(TimePrint_buf + n, temp, strlen(temp));n += strlen(temp);
  memcpy(TimePrint_buf + n,  " : ", 3); n += 3;
  
  a = bcd_to_hex(Read_T.minute);
  itoa(a,temp,10);
  memcpy(TimePrint_buf + n, temp, strlen(temp));n += strlen(temp);
  memcpy(TimePrint_buf + n,  " : ", 3); n += 3;
  
  a = bcd_to_hex(Read_T.second);
  itoa(a,temp,10);
  memcpy(TimePrint_buf + n, temp, strlen(temp));n += strlen(temp);
  memcpy(TimePrint_buf + n,  " \r\n", 3); n += 3;
  
  
  
  
  memcpy(TimePrint_buf + n, "Date: 20",  8); n += 8;
  a = bcd_to_hex(Read_T.year);
  itoa(a,temp,10);
  memcpy(TimePrint_buf + n, temp, strlen(temp));n += strlen(temp);
  memcpy(TimePrint_buf + n,  "/", 1); n += 1;
  
  a = bcd_to_hex(Read_T.month);
  itoa(a,temp,10);
  memcpy(TimePrint_buf + n, temp, strlen(temp));n += strlen(temp);
  memcpy(TimePrint_buf + n,  "/", 1); n += 1;
  
  a = bcd_to_hex(Read_T.day);
  itoa(a,temp,10);
  memcpy(TimePrint_buf + n, temp, strlen(temp));n += strlen(temp);
//  memcpy(TimePrint_buf + n,  "", 1); n += 1;
  memcpy(TimePrint_buf + n,  " \r\n", 3); n += 3;
  
//  Printf_Usart1(TimePrint_buf);
}
void Rest_Ds1302()
{
  Write_T.year   = 0x17;
  Write_T.month  = 0x01;
  Write_T.day    = 0x01;
  Write_T.week   = 0;
  Write_T.hour   = 0x23;
  Write_T.minute = 0x59;
  Write_T.second = 0x30;
  
  ds1302_write_time(&Write_T);
}
/****************************************************************************
  Copyright (C), 2016-2018, Omnivoltaic Energy Solutions Tech. Co., Ltd.
	
  Function:       // GetDs1302Day

  Description:    // GetDs1302Day

  Calls:          // GetDs1302Day

  Input:          // intdata buffer len

  Output:         // Non
	
  Return:         // caldata

  Others:         // 
	
  Author:      	  // Liu Rongkun(Micheal)
	
  Data:		  // 2019/10
******************************************************************************/
#ifdef PAYG_TEST
uint32_t day_test_cnt=0;
#endif
unsigned int GetDs1302Day(void)
{
  unsigned int temp1,temp2,temp3;
  unsigned int temp4=0;
  unsigned int year,month;
  unsigned int old_year;
  unsigned char i;
  #ifdef PAYG_TEST
  static unsigned int test=0;
  #endif
  
  Check_Ds1302Runs();

  #ifdef PAYG_TEST
  if(test!= read_1302(/*0x87*/DS1302_MIN_REG_R))
  {		
  	test=read_1302(/*0x87*/DS1302_MIN_REG_R);
  	day_test_cnt++;
  	}
  #endif
  temp1 = read_1302(0x87); //天数
  temp2 = read_1302(0x89); //月数
  temp3 = read_1302(0x8d); //年数
  
  if( (temp3 == 0x00)||(temp3 == 0xff)) //ff通讯故障
  {
	if(g_ds1302_ErrCnt<DS1302_ERRCNT)
	{   	
		g_ds1302_ErrCnt++;	
		return tian;	
	}
	else
	{
		//Sys_Code   = 0xE3;
		g_sysStateCode.error.rtc_error=1;
		Camp_ComOK = 0xE1;//串口处理
		return 32768;
	}
  }

  g_ds1302_ErrCnt=0;
  
  temp4 = (temp3&0x0f);//算已经过去的整年的天数
  temp4 += (temp3>>4)*10;
  old_year = temp4;

  year = 0;
  for(i=17;i<temp4;i++)//2016.7.28增加过去整年闰年判断功能
  {
    if((i%4)==0)
    {
      year+=366;
    }
    else
    {
      year+=365;
    }
  }
  
  temp4 = (temp2&0x0f);
  temp4 += (temp2>>4)*10;
  month = 0;
  
  for(i=1; i< temp4; i++)
  {
	    if((i == 1)||(i==3)||(i == 5)||(i==7)||(i == 8)||(i==10)||(i == 12)) 
	    {
	     	 month += 31;
	    }
	    else if((i == 4)||(i==6)||(i == 9)||(i==11)) 
	    {
	      	month += 30;
	    }
	    else if(i == 2)
	    {
		      if((old_year%4) == 0) 
			  	month += 29;
		      else
			  	month += 28;
	    }
  }
  temp4 = temp1&0x0f;
  temp4 += (temp1>>4)*10;
  
  temp4 += month ;
  temp4 += year;
  temp4-=1;

  #ifdef PAYG_TEST 	
  temp4+=day_test_cnt;
  #endif

  return temp4;
}


unsigned int GetDs1302Min(void)
{
  unsigned int temp1,temp2,temp3,temp5,temp6;
  unsigned int temp4=0, temp = 0;
  unsigned int year,month,day,hour,min;
  unsigned int old_year;
  unsigned char i;
  
  Check_Ds1302Runs();
  
  temp1 = read_1302(0x87); //天数
  temp2 = read_1302(0x89); //月数
  temp3 = read_1302(0x8d); //年数
  temp5 = read_1302(0x85); //时数
  temp6 = read_1302(0x83); //分数
  
  if( (temp3 == 0x00)||(temp3 == 0xff)) //ff通讯故障
  {
	 
	   { 	
	   	//Sys_Code   = 0xE3;
	   	  g_sysStateCode.error.rtc_error=1;

	    	return 32768;
	   	}
  }
  
  temp4 = (temp3&0x0f);//算已经过去的整年的天数
  temp4 += (temp3>>4)*10;
  old_year = temp4;

  year = 0;
  for(i=17;i<temp4;i++)//2016.7.28增加过去整年闰年判断功能
  {
    if((i%4)==0)
    {
      year+=366;
    }
    else
    {
      year+=365;
    }
  }
  
  temp4 = (temp2&0x0f);
  temp4 += (temp2>>4)*10;
  month = 0;
  for(i=1; i< temp4; i++)
  {
    if((i == 1)||(i==3)||(i == 5)||(i==7)||(i == 8)||(i==10)||(i == 12)) 
    {
      month += 31;
    }
    else if((i == 4)||(i==6)||(i == 9)||(i==11)) 
    {
      month += 30;
    }
    else if(i == 2)
    {
      if((old_year%4) == 0) month += 29;
      else month += 28;
    }
  }
  temp4 = temp1&0x0f;
  temp4 += (temp1>>4)*10;
  
  //天
  temp4 += month ;
  temp4 += year;
  temp4-=1;
  day = temp4;
  
  //小时
  temp = temp5&0x0f;
  temp += (temp5>>4)*10;
  hour = temp;
  
  //分钟
  temp = temp6&0x0f;
  temp += (temp6>>4)*10;
  min = temp;
  
  temp4 = day * 1440 + hour * 60 + min;
  
  
  return temp4;
}

void InitDs1302(void)//用17年1月1日初始化DS1302
{

	write_1302(0x8e,0x00);
	write_1302(0x80,0x00);
	write_1302(0x82,0x00);
	write_1302(0x84,0x00);
	write_1302(0x86,0x01);
	write_1302(0x88,0x01);
	write_1302(0x8c,0x17);
	write_1302(0x8a,0x07);
	write_1302(0x90,0x00);
	write_1302(0x8e,0x80);
        
  	tian = 0;
	#ifdef PAYG_TEST
       day_test_cnt=0;
        #endif
//     Rest_Ds1302();
}

/*d最少大于1秒调用 否者报错*/
void Check_Ds1302Runs(void)
{
  static uint8_t rec = 0xff;
  uint8_t temp;
  
  temp = read_1302(DS1302_SEC_REG_R);
  if(rec == temp)//和上一秒调用的值一样 32.768K晶体故障
  {
	   if(g_ds1302_ErrCnt<DS1302_ERRCNT)
	   	 g_ds1302_ErrCnt++;	
	   else
    		g_sysStateCode.error.rtc_error=1;//Sys_Code = 0xE3;//晶体故障
  }
  else//晶体正常
  {
    rec = temp;
    //g_ds1302_ErrCnt=0;	
  }
}
