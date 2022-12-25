#include"main.h"

int tian = 0; 
uint8_t g_tokenState=TOKEN_OK;
uint8_t g_KeyboardDetectLock=FALSE;
uint8_t g_KeyboardDetectLockRst=FALSE;

PAYG_TypeDef payg;
__IO uint8_t g_PaygUpdate=0;
uint8_t g_sysInit=TRUE;
extern uint8_t Sys_Code ;
extern unsigned char keyUesed[7];//{0xC5,0x6A,0x29,0x07,0xFA,0x0B,0x1F};//密码已使用
extern unsigned char errkey[7]  ;//错误密码
extern uint8_t SaveFlash_Flag;

extern DS1302_TIME Read_T;
extern USER_SET_TypeDef g_UserSet;

extern __IO  uint8_t  g_AcState;
extern __IO uint8_t  g_DcState;
extern __IO uint8_t  g_UsbState;

extern uint8_t g_keyboardPwrHoldTmrLock;
extern  SYS_STATE_CODE_TypeDef  g_sysStateCode;


void PaygOvesTask(void)
{
	//  if(tian > 2192)//时钟芯片出错
	//  {
	//    payg.remaing_days = 0xffff;
	//  }
	//  else
	//  {
	
	if(payg.free != YES)
	{
		//    tian = GetDs1302Day();
		if((payg.free == NO) && (payg.remaing_days == 0))
		{	
			//no code ??
			}
		else//Payg状态 还有剩余天数
		{
			if((int32_t)((int32_t)tian + (int32_t)payg.day_cem) >(int32_t) payg.days_has_been_runing)
			{
				//payg.days_has_been_runing  = tian  + payg.day_cem;
				payg.days_has_been_runing  = tian + payg.day_cem;
				payg.run_days_backup ++;
			}

		}
		
		if(payg.recharge_days > payg.days_has_been_runing)
		{	
			payg.remaing_days = payg.recharge_days  - payg.days_has_been_runing;
			}
		else 
		{
			payg.remaing_days = 0;
			payg.days_has_been_runing = payg.recharge_days;
			payg.day_cem = payg.days_has_been_runing;
			//SaveFlash_Flag = 1;
			EEpUpdateEnable();
			}

	}
	
}


void sha1_starts( sha1_context *ctx )
{
  ctx->total[0] = 0;
  ctx->total[1] = 0;
  
  ctx->state[0] = 0x67452301;
  ctx->state[1] = 0xEFCDAB89;
  ctx->state[2] = 0x98BADCFE;
  ctx->state[3] = 0x10325476;
  ctx->state[4] = 0xC3D2E1F0;
}

void sha1_process( sha1_context *ctx, uint8_t data[64] )
{
  uint32_t temp, W[16], A, B, C, D, E;
  
  GET_UINT32( W[0],  data,  0 );
  GET_UINT32( W[1],  data,  4 );
  GET_UINT32( W[2],  data,  8 );
  GET_UINT32( W[3],  data, 12 );
  GET_UINT32( W[4],  data, 16 );
  GET_UINT32( W[5],  data, 20 );
  GET_UINT32( W[6],  data, 24 );
  GET_UINT32( W[7],  data, 28 );
  GET_UINT32( W[8],  data, 32 );
  GET_UINT32( W[9],  data, 36 );
  GET_UINT32( W[10], data, 40 );
  GET_UINT32( W[11], data, 44 );
  GET_UINT32( W[12], data, 48 );
  GET_UINT32( W[13], data, 52 );
  GET_UINT32( W[14], data, 56 );
  GET_UINT32( W[15], data, 60 );
  
#define S(x,n) ((x << n) | ((x & 0xFFFFFFFF) >> (32 - n)))
  
#define R(t)                                            \
  (                                                       \
    temp = W[(t -  3) & 0x0F] ^ W[(t - 8) & 0x0F] ^     \
      W[(t - 14) & 0x0F] ^ W[ t      & 0x0F],      \
        ( W[t & 0x0F] = S(temp,1) )                         \
          )
    
#define P(a,b,c,d,e,x)                                  \
  {                                                       \
    e += S(a,5) + F(b,c,d) + K + x; b = S(b,30);        \
  }
  
  A = ctx->state[0];
  B = ctx->state[1];
  C = ctx->state[2];
  D = ctx->state[3];
  E = ctx->state[4];
  
#define F(x,y,z) (z ^ (x & (y ^ z)))
#define K 0x5A827999
  
  P( A, B, C, D, E, W[0]  );
  P( E, A, B, C, D, W[1]  );
  P( D, E, A, B, C, W[2]  );
  P( C, D, E, A, B, W[3]  );
  P( B, C, D, E, A, W[4]  );
  P( A, B, C, D, E, W[5]  );
  P( E, A, B, C, D, W[6]  );
  P( D, E, A, B, C, W[7]  );
  P( C, D, E, A, B, W[8]  );
  P( B, C, D, E, A, W[9]  );
  P( A, B, C, D, E, W[10] );
  P( E, A, B, C, D, W[11] );
  P( D, E, A, B, C, W[12] );
  P( C, D, E, A, B, W[13] );
  P( B, C, D, E, A, W[14] );
  P( A, B, C, D, E, W[15] );
  P( E, A, B, C, D, R(16) );
  P( D, E, A, B, C, R(17) );
  P( C, D, E, A, B, R(18) );
  P( B, C, D, E, A, R(19) );
  
#undef K
#undef F
  
#define F(x,y,z) (x ^ y ^ z)
#define K 0x6ED9EBA1
  
  P( A, B, C, D, E, R(20) );
  P( E, A, B, C, D, R(21) );
  P( D, E, A, B, C, R(22) );
  P( C, D, E, A, B, R(23) );
  P( B, C, D, E, A, R(24) );
  P( A, B, C, D, E, R(25) );
  P( E, A, B, C, D, R(26) );
  P( D, E, A, B, C, R(27) );
  P( C, D, E, A, B, R(28) );
  P( B, C, D, E, A, R(29) );
  P( A, B, C, D, E, R(30) );
  P( E, A, B, C, D, R(31) );
  P( D, E, A, B, C, R(32) );
  P( C, D, E, A, B, R(33) );
  P( B, C, D, E, A, R(34) );
  P( A, B, C, D, E, R(35) );
  P( E, A, B, C, D, R(36) );
  P( D, E, A, B, C, R(37) );
  P( C, D, E, A, B, R(38) );
  P( B, C, D, E, A, R(39) );
  
#undef K
#undef F
  
#define F(x,y,z) ((x & y) | (z & (x | y)))
#define K 0x8F1BBCDC
  
  P( A, B, C, D, E, R(40) );
  P( E, A, B, C, D, R(41) );
  P( D, E, A, B, C, R(42) );
  P( C, D, E, A, B, R(43) );
  P( B, C, D, E, A, R(44) );
  P( A, B, C, D, E, R(45) );
  P( E, A, B, C, D, R(46) );
  P( D, E, A, B, C, R(47) );
  P( C, D, E, A, B, R(48) );
  P( B, C, D, E, A, R(49) );
  P( A, B, C, D, E, R(50) );
  P( E, A, B, C, D, R(51) );
  P( D, E, A, B, C, R(52) );
  P( C, D, E, A, B, R(53) );
  P( B, C, D, E, A, R(54) );
  P( A, B, C, D, E, R(55) );
  P( E, A, B, C, D, R(56) );
  P( D, E, A, B, C, R(57) );
  P( C, D, E, A, B, R(58) );
  P( B, C, D, E, A, R(59) );
  
#undef K
#undef F
  
#define F(x,y,z) (x ^ y ^ z)
#define K 0xCA62C1D6
  
  P( A, B, C, D, E, R(60) );
  P( E, A, B, C, D, R(61) );
  P( D, E, A, B, C, R(62) );
  P( C, D, E, A, B, R(63) );
  P( B, C, D, E, A, R(64) );
  P( A, B, C, D, E, R(65) );
  P( E, A, B, C, D, R(66) );
  P( D, E, A, B, C, R(67) );
  P( C, D, E, A, B, R(68) );
  P( B, C, D, E, A, R(69) );
  P( A, B, C, D, E, R(70) );
  P( E, A, B, C, D, R(71) );
  P( D, E, A, B, C, R(72) );
  P( C, D, E, A, B, R(73) );
  P( B, C, D, E, A, R(74) );
  P( A, B, C, D, E, R(75) );
  P( E, A, B, C, D, R(76) );
  P( D, E, A, B, C, R(77) );
  P( C, D, E, A, B, R(78) );
  P( B, C, D, E, A, R(79) );
  
#undef K
#undef F
  
  ctx->state[0] += A;
  ctx->state[1] += B;
  ctx->state[2] += C;
  ctx->state[3] += D;
  ctx->state[4] += E;
}

void sha1_update( sha1_context *ctx, uint8_t *input, uint32_t length )
{
  uint32_t left, fill;
  
  if( ! length ) return;
  
  left = ctx->total[0] & 0x3F;
  fill = 64 - left;
  
  ctx->total[0] += length;
  ctx->total[0] &= 0xFFFFFFFF;
  
  if( ctx->total[0] < length )
    ctx->total[1]++;
  
  if( left && length >= fill )
  {
    memcpy( (void *) (ctx->buffer + left), (void *) input, fill );
          
    sha1_process( ctx, ctx->buffer );
    length -= fill;
    input  += fill;
    left = 0;
  }
  
  while( length >= 64 )
  {
    sha1_process( ctx, input );
    length -= 64;
    input  += 64;
  }
  
  if( length )
  {
    memcpy( (void *) (ctx->buffer + left),(void *) input, length );
           
  }
}

static uint8_t sha1_padding[64] =
{
  0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void sha1_finish( sha1_context *ctx, uint8_t digest[20] )
{
  uint32_t last, padn;
  uint32_t high, low;
  uint8_t msglen[8];
  
  high = ( ctx->total[0] >> 29 )
    | ( ctx->total[1] <<  3 );
  low  = ( ctx->total[0] <<  3 );
  
  PUT_UINT32( high, msglen, 0 );
  PUT_UINT32( low,  msglen, 4 );
  
  last = ctx->total[0] & 0x3F;
  padn = ( last < 56 ) ? ( 56 - last ) : ( 120 - last );
  
  sha1_update( ctx, sha1_padding, padn );
  sha1_update( ctx, msglen, 8 );
  
  PUT_UINT32( ctx->state[0], digest,  0 );
  PUT_UINT32( ctx->state[1], digest,  4 );
  PUT_UINT32( ctx->state[2], digest,  8 );
  PUT_UINT32( ctx->state[3], digest, 12 );
  PUT_UINT32( ctx->state[4], digest, 16 );
}



/*
unsigned  long input_int[2];

void strtolong(void)
{
uint16_t i;
unsigned long temp1= 0x3b9aca00; 
unsigned long temp2= 0x3b9aca00; 
unsigned long temp3=0;
input_int[0] = 0;
input_int[1] = 0;
//ê?è??ainput_data.code
for(i=0;i<28;i++)
{
if((i==2)||(i==3)||(i==5)||(i==6)||(i==7)||(i==9)||(i==10)||(i==11)||(i==13)||(i==14))
{
temp3 = input_data.code[i];
input_int[0] +=( temp3- 0x30 )* temp1;
temp1 /= 10;
                   }
if((i==15)||(i==17)||(i==18)||(i==19)||(i==21)||(i==22)||(i==23)||(i==25)||(i==26)||(i==27))
{
temp3 = input_data.code[i];
input_int[1] += ( temp3- 0x30 ) * temp2;
temp2 /= 10;
                   }
          }

  }
*/

uint8_t MiddleStr(uint8_t hexd)
{
  uint8_t tmp=0;
  if (hexd <= 9)
  {
    tmp=hexd+'0';
  }
  else
  {
    tmp=hexd+55;
  }
  return tmp;
}
void HexToStr(uint32_t *hex,uint8_t *testbuff)
{
  uint8_t i,j=28;
  for(i=0;i<8;i++)
  {
    testbuff[i] = MiddleStr(hex[0]>>j&0x0f);
    j-=4;
  }
  j=28;
  for(i=0;i<8;i++)
  {
    testbuff[i+8] = MiddleStr(hex[1]>>j&0x0f);
    j-=4;
  }
  testbuff[16]='\0';
}


void SHA(uint8_t *Message_Digest,uint32_t *output)
{
  
  uint32_t digest[5];
  uint8_t i,j;
  
  for(i=0,j=4;i<5;i++,j--)
  {
    digest[j] = *((uint32_t*)Message_Digest + i);
  }
  
  digest[0] ^= digest[2];
  digest[1] ^= digest[3];
  digest[0] ^= digest[4];
  
  output[0] = digest[0];
  output[1] = digest[1];
}


int big_litle_endian(int x)  
{  
  int tmp;  
  tmp = (((x)&0xff)<<24) + (((x>>8)&0xff)<<16) + (((x>>16)&0xff)<<8) + (((x>>24)&0xff));  
  return tmp;  
} 

void sha1_c(void)
{
  sha1_context ctx;
  uint8_t  Message_Digest[20];
  uint8_t  testbuff[20];
  HexToStr(payg.hast_input,testbuff);
  sha1_starts( &ctx );
  sha1_update( &ctx, (uint8_t *) testbuff,16);
  sha1_finish( &ctx, Message_Digest);
  SHA(Message_Digest,payg.hast_input);
  payg.hast_input[0] = big_litle_endian(payg.hast_input[0]) ;
  payg.hast_input[1] = big_litle_endian(payg.hast_input[1]) ;
}


unsigned char keyocl[20]       =    {0xc5,0x6a,0x29,0x0e,0xfb,0xbe,0x5d,0x72,0xc3,0xb9,0xb8,0x81,0xfb,0xdd};
unsigned char key[20]          =    {0xc5,0x6a,0x29,0x0e,0xfa,0xce,0xb3,0x98,0x00,0x12,0xac,0xcd,0x5f,0xdd};

void fun2(void) 
{
  uint32_t tmp;
  
  tmp = keyocl[12];
  payg.hast_otp[0] = tmp << 24;
  tmp = keyocl[11];
  payg.hast_otp[0] |= tmp << 16;
  tmp = keyocl[10];
  payg.hast_otp[0] |= tmp << 8;
  payg.hast_otp[0] |= keyocl[9];
  
  tmp = keyocl[8];
  payg.hast_otp[1] = tmp << 24;
  tmp = keyocl[7];
  payg.hast_otp[1] |= tmp << 16;
  tmp = keyocl[6];
  payg.hast_otp[1] |= tmp << 8;
  payg.hast_otp[1] |= keyocl[5];
}
void fuc1(void)
{
  uint32_t tmp;
  
  tmp = key[12];
  payg.hast_input[0] = tmp << 24;
  tmp = key[11];
  payg.hast_input[0] |= tmp << 16;
  tmp = key[10];
  payg.hast_input[0] |= tmp << 8;
  payg.hast_input[0] |= key[9];
  
  tmp = key[8];
  payg.hast_input[1] = tmp << 24;
  tmp = key[7];
  payg.hast_input[1] |= tmp << 16;
  tmp = key[6];
  payg.hast_input[1] |= tmp << 8;
  payg.hast_input[1] |= key[5];
}

long Top_UpDays = 0;

extern  char Hashstr[28];
extern char hashshow[32];	
extern HASH_TOP_Def Hash_Hi,Hash_Lo;

void Hash_Opration(uint32_t *hastop)
{
		int j;
    memset(hashshow, 0x00, 32);
    memset(Hashstr, 0x00, 28);
 /*   Hash_Lo.Hash_char[0] = Receive2_Buffer[8];
    Hash_Lo.Hash_char[1] = Receive2_Buffer[7];
    Hash_Lo.Hash_char[2] = Receive2_Buffer[6];
    Hash_Lo.Hash_char[3] = Receive2_Buffer[5];
    Hash_Hi.Hash_char[0] = Receive2_Buffer[12];
    Hash_Hi.Hash_char[1] = Receive2_Buffer[11];
    Hash_Hi.Hash_char[2] = Receive2_Buffer[10];
    Hash_Hi.Hash_char[3] = Receive2_Buffer[9];*/

    Hash_Lo.Hash_long=hastop[1];
    Hash_Hi.Hash_long=hastop[0];

    CampInttoStrs(Hash_Hi.Hash_long, Hashstr);

    if(strlen(Hashstr) < 10)
     	 CampInsert(Hashstr, "0", 0);
    CampInttoStrs(Hash_Lo.Hash_long, Hashstr + strlen(Hashstr));
  //  if(strlen(Hashstr) < 20)
  //    insert(Hashstr, "0", 10);
    /* 补零 少几个补几个 */
    for( j = 20 - strlen(Hashstr); j  > 0; j--)
    {
      CampInsert(Hashstr, "0", 9 + j);
    }

    CampInsert(Hashstr, " ", 2);
    CampInsert(Hashstr, " ", 6);
    CampInsert(Hashstr, " ", 10);
    CampInsert(Hashstr, " ", 14);
    CampInsert(Hashstr, " ", 18);
    CampInsert(Hashstr, " ", 22);

    memcpy(hashshow, "*0", 2);
    memcpy(hashshow + 2, Hashstr, 26);
    memcpy(hashshow + 28, "#", 1);
}


PAYG_TypeDef* PaygGetPaygBuf(void)
{
	return &payg;
}

void PaygInit(void)
{
	  memcpy((uint8_t*)&payg,(uint8_t*)&g_UserSet.Payg,sizeof(PAYG_TypeDef));

	  payg.hast_root[0] = 0x00000000;
	  payg.hast_root[1] = 0x00000000;

	  if((payg.free != YES) && (payg.free != NO))
	  {
		    payg.free = YES;
		    payg.day_cem = 0;//备份天数等于0;
		    EEpUpdateEnable();
	  }
	  

	  PaygUpdateEnable();
	  PaygProcess();
}

void PaygOvesInput(void)
{
  long z;
  static uint32_t hast_top_tmp[2];
  //密码已使用
  if((payg.hast_input[0] == payg.hast_otp[0])&&(payg.hast_input[1] == payg.hast_otp[1]))
  {
	    Printf_Usart_num(keyUesed, 7);
	    g_tokenState=TOKEN_USEED;

	     if(EEpGetDemoCnt()<3&&payg.remaing_days==0&&payg.free == NO)
	     {	
		     	TimerDemoSet();
		     	DemoModeStart();
			EEpSetDemoCnt(EEpGetDemoCnt()+1);	
	     	}
	    else
	    {   
			g_sysStateCode.error.pswd_error=1;
	    	}
  }
  else//新密码
  {
    //备份输入密码
    hast_top_tmp[0] = payg.hast_input[0];
    hast_top_tmp[1] = payg.hast_input[1];
	
    for(z=1; z<HASH_MAX + 1; z++)
    {
      sha1_c(); //计算密码
      
      if((payg.hast_input[0] == payg.hast_otp[0])&&(payg.hast_input[1] == payg.hast_otp[1]))
        	break;
    }
	
    if(z <= HASH_MAX)
    	Top_UpDays = z;
    else
    	Top_UpDays = 0;
	
    if(Top_UpDays > 0)//充值密码正确
    {	
		//     Printf_Usart1("当前密码充值天数:%d  （天）\r\n",z);
		payg.hast_otp[0] = hast_top_tmp[0];
		payg.hast_otp[1] = hast_top_tmp[1];
		//SaveFlash_Flag = 1;//需要更新FLASH
		EEpUpdateEnable();

		if(Top_UpDays >= 2192) 
			Top_UpDays = 2192;
		else if(Top_UpDays >= 1096)
			Top_UpDays = 1096;

		if(YES == payg.free)
		{
			//初始化所有运行参数
			payg.time_cnt = 0;  //累计时间清0
			payg.recharge_days = 0; //充值天数清0
			payg.days_has_been_runing = 0;
			InitDs1302();//已运行天数清0
			payg.remaing_days = 1096; //剩余天数清0
			//payg.run_days_backup  //这个参数累计不清零
			payg.lock = NUL; //è¨?T?a3y

			//sys.eeprom_update = YES; //EEpeom óD2?êyDèòa?üD?
      		}
		
     	      if(Top_UpDays == 2192)
	      {
		        payg.free = NO;
		        payg.lock = NO;
		        payg.remaing_days = 0;
		        payg.days_has_been_runing = 0;
			InitDs1302();//已运行天数清0
		        payg.time_cnt = 0;
		        payg.recharge_days = 0;
		        //        //CBI(sys.system_status_Code,E05);
		        //        sys.system_status_Code = 0;
		        //        sys.eeprom_update = YES;
		        
		        //=========
		        Sys_Code = 0;//clean Err
		        g_sysStateCode.error.rtc_error=0;

		        g_tokenState=TOKEN_OK;
		        
		        payg.day_cem = 0;//运行天数备份清零
	      		}
     	       else if(Top_UpDays == 1096)
	       {
		        //初始化所有参数
		        payg.time_cnt = 0;  //累计时间清0
		        payg.recharge_days = 0; //天数0
		        payg.days_has_been_runing = 0;
			InitDs1302(); //已运行时间清0
		        payg.remaing_days = 1096; //剩余天数清0
		        //payg.run_days_backup  //这个参数累计不清零
		        payg.lock = NUL; //权限清楚
		        payg.free = YES;
		        //=========
		       // Sys_Code = 0;//clean Err
		         g_sysStateCode.error.rtc_error=0;

		        g_tokenState=TOKEN_OK;
		        
		        payg.day_cem = 0;//运行天数备份清零
	      		}
      	      else if((Top_UpDays > 0) && (Top_UpDays < 1096))
	      {
		        //如果之前已经停机欠费,将时间累计清0.防止快累计一天实际充值效果没有1天
		        if(payg.remaing_days == 0)
		        {
			          if(/*(Sys_Code != 0xE1) && (Sys_Code != 0xE3)*/  g_sysStateCode.error.rtc_error==FALSE)
			          {
				          payg.time_cnt = 0;  //如果剩余天数0
				          InitDs1302();//如果欠费状态下充值  时钟清零？ 
			          }
		        }
		        PaygOvesTask();
			g_tokenState=TOKEN_OK;
		        payg.recharge_days += Top_UpDays; //增加充值天数
		        payg.free = NO;
			#ifdef   SHOW_TIME        
		        //打印充值天数
		        memset(cmd_sendbuf, 0, 32);
		        itoa(Top_UpDays, cmd_sendbuf, 10);
		        Printf_Usart1("正确的充值命令 充值天数: ");
		        Printf_Usart1(cmd_sendbuf);
		        Printf_Usart1("\r\n");
			#endif        
	      }
    	  Send_RechargeOK();
	  g_sysStateCode.error.pswd_error=0;	  
	  g_sysStateCode.error.pwd_len_error=0;
      
    }
    else//充值密码错误
    {
      Printf_Usart_num(errkey, 7);

      g_tokenState=TOKEN_ERROR;	  
      g_sysStateCode.error.pswd_error=1;
	  
    }
  }
}

void PaygUpdateEnable(void)
{
	g_PaygUpdate=TRUE;
}

uint32_t  PaygGetPayState(void)
{
	if( payg.remaing_days>0||payg.free == YES)
	 	return TRUE;
	else
		 return FALSE;
}

uint32_t  PaygGetFreeState(void)
{
	if( YES == payg.free)
	 	return TRUE;
	else
		 return FALSE;
}

void PaygProcess(void)
{
	uint8_t syserr=GetSysErrorCode();
	uint8_t output_en=TRUE;

	
	if(g_PaygUpdate)
	{
		uint8_t temp[8];
		//extern  char hashshow[32];	

		g_PaygUpdate=FALSE;
		
		
		//ds1302_read_time (&Read_T);

		if(payg.free != YES)
		{
			//HAL_Delay(2);
			tian = GetDs1302Day();
		}

	#ifdef   SHOW_TIME
		Show_Time();
	#endif      
		PaygOvesTask();
	

		#if  defined( PUMP_PROJECT)||defined(UI1K_V2_PROJECT)||defined(UI1K_V13_PROJECT)||defined(E_MOB48V_PROJECT)||defined(P10KW_PROJECT)

		#ifndef BLE_MASTER_ENABLE
		GattSetPpid(payg.payg_id,20);
		GattSetOpid(payg.oem_id,20);
		#endif

		GattSetData(LIST_CMD, CMD_NAPN, (uint8_t*)g_UserSet.NetInfor.apn);
		
		#if defined(UI1K_V13_PROJECT)||defined(E_MOB48V_PROJECT)||defined(P10KW_PROJECT)
		#ifndef BLE_MASTER_ENABLE
		GattSetVersion(GATT_VERSON,5);
		GattSetSysStatusCode(g_sysStateCode.statecode);
		#endif
		#else
		GattSetVersion("6.5",3);
		#endif

		#ifndef BLE_MASTER_ENABLE
		GattSetRmPayDays(payg.remaing_days);//Remaining_PAYG_Days
		GattSetRunDays(payg.days_has_been_runing);//Days have been running
		GattSetPaygDays(payg.recharge_days);//PAYG_Dayss
		if(payg.remaing_days||payg.free == YES)
			GattSetPaygState(1);//PAYG_State
		else
			GattSetPaygState(0);//PAYG_State
		GattSetRunDayBackup( payg.run_days_backup);//Run_Days_Backup
		//GattSetSolarGeneration( payg.hast_otp[1]);//Solar Generation
		#endif

		GattSetHeartbeat(g_UserSet.heartbeat);
		GattSetReportMode(g_UserSet.reportt_auto?0:1);

		#ifndef BLE_MASTER_ENABLE

		#if defined( PUMP_PROJECT)||defined(UI1K_V2_PROJECT)||defined(UI1K_V13_PROJECT)||defined(E_MOB48V_PROJECT)||defined(P10KW_PROJECT)
		Hash_Opration(payg.hast_otp);
		#else
		temp[0]=payg.hast_otp[0];
		temp[1]=payg.hast_otp[0]>>8;
		temp[2]=payg.hast_otp[0]>>16;
		temp[3]=payg.hast_otp[0]>>24;

		temp[4]=payg.hast_otp[1];
		temp[5]=payg.hast_otp[1]>>8;
		temp[6]=payg.hast_otp[1]>>16;
		temp[7]=payg.hast_otp[1]>>24;
		
		CampHashOpration(temp);
		#endif
		GattSetHashTop((uint8_t*)hashshow,29);

		if(payg.remaing_days>0||GetDemoState()||payg.free == YES)
			GattSetOutputCtrlState(1);
		else
			GattSetOutputCtrlState(0);
		#endif
		#endif


	}
  
	#ifdef UI1K_V2_PROJECT
    #ifdef UI1K_V13_PROJECT
	if(/*syserr==0xE3*/ g_sysStateCode.error.rtc_error&&YES != payg.free)
		output_en==FALSE;

	if(HAL_GetTick()>1000*15)  //SYS INIT 30S 
		g_sysInit=FALSE;


	if(GetDemoState())
	{	
		if(GetTimerDemoEvent())
		{	DemoModeClose();
			TimerEventClear(TIMER_DEMO);
			}
		}

	if((payg.remaing_days==0&&payg.free== NO||g_sysInit)&&GetDemoState()==FALSE)
	{
		g_AcState=FALSE;
		g_UsbState=FALSE;
		g_DcState=FALSE;
		
		HAL_GPIO_WritePin(AC_INVERT_CTRL_GPIO_Port, AC_INVERT_CTRL_Pin, GPIO_PIN_RESET);  //ac 220v
		
		HAL_GPIO_WritePin(OUT_PG2_GPIO_Port, OUT_PG2_Pin, GPIO_PIN_RESET);  // 12 v output 
		//HAL_GPIO_WritePin(OUT_KEY_GPIO_Port, OUT_KEY_Pin, GPIO_PIN_RESET);  // usb pwr

		if(g_sysInit)
			debug_printf("payg","sys init 30s ,remain days=",payg.remaing_days);
		else
			debug_printf("payg","Arrearage mode ,remain days=",payg.remaing_days);
		
		if( KeyboardPlugIn()&&CoulomLowbat()<LOWBAT_LEVEL2&&g_sysInit==FALSE)
		{	//HAL_GPIO_WritePin(OUT_KEY_GPIO_Port, OUT_KEY_Pin, GPIO_PIN_SET);  //usb pwr
			HAL_GPIO_WritePin(CHRG_CTRL_GPIO_Port, CHRG_CTRL_Pin, GPIO_PIN_SET);
			}
		else	
		{	
			HAL_GPIO_WritePin(CHRG_CTRL_GPIO_Port, CHRG_CTRL_Pin, GPIO_PIN_RESET);
			}
		
		// Get uart 3 rx state
		 if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11)==GPIO_PIN_SET&&g_sysInit==FALSE)
		 {
		 	if(g_KeyboardDetectLock==FALSE&&g_keyboardPwrHoldTmrLock==FALSE)
		 	{
			 	TimerKbPwrHoldSet(5*1000);
			 	KeyboardUsbPwrSet(TRUE);
				g_KeyboardDetectLock=TRUE;
				g_KeyboardDetectLockRst=TRUE;

				debug_printf("payg","usb keyboard plug in ,remain days=",payg.remaing_days);
		 		}
		 	}
		 else
		 {
		 	if(g_KeyboardDetectLock&&g_KeyboardDetectLockRst==FALSE)
			{	
				if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11)==GPIO_PIN_RESET)
			 	{
			 		HAL_Delay(50);
					if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11)==GPIO_PIN_RESET)
			 			g_KeyboardDetectLock=FALSE;
			 		}
		 		}
				
		 	}

		
		HAL_GPIO_WritePin(OUT_KEY_GPIO_Port, OUT_KEY_Pin, GPIO_PIN_RESET);  //usb pwr

		}
	else
	{
		if(GetUserAcState()&&output_en&&CoulomLowbat()==LOWBAT_NONE)
			HAL_GPIO_WritePin(AC_INVERT_CTRL_GPIO_Port, AC_INVERT_CTRL_Pin, GPIO_PIN_SET);  //ac 220v off	
		else
		{	HAL_GPIO_WritePin(AC_INVERT_CTRL_GPIO_Port, AC_INVERT_CTRL_Pin, GPIO_PIN_RESET);	 //ac 220v on	
			g_AcState=FALSE;
			debug_printf("payg","ac off ,remain days=",payg.remaing_days);
			}
		
			
		if(AdcDianyanOverState()||CoulomLowbat()==LOWBAT_LEVEL2||output_en==FALSE
			#ifdef ROLL_SWITCH
			||GetUserDcState()==FALSE
			#endif
			)
		{	HAL_GPIO_WritePin(OUT_PG2_GPIO_Port, OUT_PG2_Pin, GPIO_PIN_RESET);  //12 v output  
			debug_printf("payg","dc off ,remain days=",payg.remaing_days);
			}
		else	
		{
			HAL_GPIO_WritePin(OUT_PG2_GPIO_Port, OUT_PG2_Pin, GPIO_PIN_SET);  //  12 v output
			}

		if(CoulomLowbat()==LOWBAT_LEVEL2
			#ifdef ROLL_SWITCH
			||GetUserUsbState()==FALSE
			#endif
			)
		{	HAL_GPIO_WritePin(OUT_KEY_GPIO_Port, OUT_KEY_Pin, GPIO_PIN_RESET);  //usb pwr
			debug_printf("payg","low battery ,usb off ,remain days=",payg.remaing_days);
			}
		else	
			HAL_GPIO_WritePin(OUT_KEY_GPIO_Port, OUT_KEY_Pin, GPIO_PIN_SET);  //usb pwr
			
		if(CoulomLowbat()<LOWBAT_LEVEL2
			#ifdef ROLL_SWITCH
			&&(GetUserUsbState()||KeyboardPlugIn())
			#endif
			)	
		{	HAL_GPIO_WritePin(CHRG_CTRL_GPIO_Port, CHRG_CTRL_Pin, GPIO_PIN_SET); //USB KEY BOARD
			
			}
		else	
		{	HAL_GPIO_WritePin(CHRG_CTRL_GPIO_Port, CHRG_CTRL_Pin, GPIO_PIN_RESET); //USB KEY BOARD
			debug_printf("payg","low battery ,usb keyboard off ,remain days=",payg.remaing_days);
			}
		}
        //--------debug--------
	//HAL_GPIO_WritePin(OUT_PG2_GPIO_Port, OUT_PG2_Pin, GPIO_PIN_SET);  //12 v output点烟器
	//HAL_GPIO_WritePin(OUT_KEY_GPIO_Port, OUT_KEY_Pin, GPIO_PIN_SET);  //usb pwr

	/*if(GetUserAcState())
		HAL_GPIO_WritePin(AC_INVERT_CTRL_GPIO_Port, AC_INVERT_CTRL_Pin, GPIO_PIN_SET);  //ac 220v off	
	else
		HAL_GPIO_WritePin(AC_INVERT_CTRL_GPIO_Port, AC_INVERT_CTRL_Pin, GPIO_PIN_RESET);	 //ac 220v on	*/
        //----------------------
  	//if(HAL_GPIO_ReadPin(AC_FAULT_GPIO_Port, AC_FAULT_Pin)==GPIO_PIN_SET&&g_AcState/**/)
	{	
		//HAL_GPIO_WritePin(AC_INVERT_CTRL_GPIO_Port, AC_INVERT_CTRL_Pin, GPIO_PIN_RESET);	 //ac 220v on	
		//g_AcState=FALSE;
  		}

	/*if(HAL_GPIO_ReadPin(BAT_C_CHRG_IN_GPIO_Port, BAT_C_CHRG_IN_Pin)==GPIO_PIN_RESET)
	{	
		HAL_GPIO_WritePin(CHRG_CTRL_GPIO_Port, CHRG_CTRL_Pin, GPIO_PIN_RESET);
  		}
	else*/
	{
		//HAL_GPIO_WritePin(CHRG_CTRL_GPIO_Port, CHRG_CTRL_Pin, GPIO_PIN_SET);
		}

	if(HAL_GPIO_ReadPin(PV_ON_CHRGE_IN_GPIO_Port, PV_ON_CHRGE_IN_Pin)==GPIO_PIN_RESET
		&&AdcPvChrgOverState()==FALSE
		&&CoulomVoltOver()==FALSE)
	{	
		HAL_GPIO_WritePin(PV_CHRG_CTRL_GPIO_Port, PV_CHRG_CTRL_Pin, GPIO_PIN_SET);
  		}
	else
	{
		HAL_GPIO_WritePin(PV_CHRG_CTRL_GPIO_Port, PV_CHRG_CTRL_Pin, GPIO_PIN_RESET);
		}
	#endif
	#endif

		  #ifdef PUMP_TEST
		/*GattSetPpid(payg.payg_id,20);
		GattSetOpid(payg.oem_id,20);

		
		GattSetVersion("6.5",3);
		GattSetRmPayDays(20);//Remaining_PAYG_Days
		GattSetRunDays(12);//Days have been running
		GattSetPaygDays(32);//PAYG_Dayss
		GattSetPaygState(1);//PAYG_State
		GattSetRunDayBackup(90);//Run_Days_Backup
		GattSetHashTop("006 140 021 633 927 101 619",27);*/
		#endif
		//GattSetSolarGeneration( payg.hast_otp[1]);//Solar Generation
}

