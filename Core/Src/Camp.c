#include "main.h"
CAMP_GET_DEF g_CampGetState=CAMP_GET_VER;
__IO uint8_t g_CampGetEnable=FALSE;



const uint8_t CampCmd_DEVID[]=                           {0xC5,0x6a,0x29,0x06,0xaf,0x25};
const uint8_t CampCmd_DEVIDOK[]=                         {0xC5,0x6a,0x29,0x0a,0xaf};

const uint8_t CampCmd_PPID[]=                            {0xC5,0x6a,0x29,0x07,0x08,0x14,0xcb};
const uint8_t CampCmd_PPIDRcvOK[]=                       {0xc5,0x6a,0x29,0x1b,0x08};

const uint8_t CampCmd_OPID[]=                            {0xC5,0x6a,0x29,0x07,0x01,0x0e,0x9a};
const uint8_t CampCmd_OPIDRcvOK[]=                       {0xC5,0x6a,0x29,0x15,0x01};

const uint8_t CampCmd_ReminingPAYGDays[]=                {0xC5,0x6a,0x29,0x06,0x05,0xf4};
const uint8_t CampCmd_ReminingPAYGDaysRcvOK[]=           {0xc5,0x6a,0x29,0x08,0x05};

const uint8_t CampCmd_DaysRun[]=                         {0xc5,0x6a,0x29,0x06,0x06,0x16};
const uint8_t CampCmd_DaysRunRcvOK[]=                    {0xc5,0x6a,0x29,0x08,0x06};

const uint8_t CampCmd_PAYGDays[]=                        {0xc5,0x6a,0x29,0x06,0x07,0x48};
const uint8_t CampCmd_PAYGDaysRcvOK[]=                   {0xc5,0x6a,0x29,0x08,0x07};

const uint8_t CampCmd_PAYGState[]=                       {0xC5,0x6a,0x29,0x06,0x09,0x57};
const uint8_t CampCmd_PAYGStateRcvOK[]=                  {0xc5,0x6a,0x29,0x08,0x09};

const uint8_t CampCmd_OCS[]=                             {0xC5,0x6a,0x29,0x06,0x0a,0xb5};
const uint8_t CampCmd_OCSRcvOK[]=                        {0xc5,0x6a,0x29};

const uint8_t CampCmd_SSC[]=                             {0xc5,0x6a,0x29,0x06,0x0b,0xeb};
const uint8_t CampCmd_SSCRcvOK[]=                        {0xc5,0x6a,0x29,0x08,0x0b};

const uint8_t CampCmd_RelativeSOC[]=                     {0xc5,0x6a,0x29,0x06,0x0c,0x68};
const uint8_t CampCmd_RelativeSOCRcvOK[]=                {0xc5,0x6a,0x29,0x08,0x0c};

const uint8_t CampCmd_RemainingCap[]=                                    {0xc5,0x6a,0x29,0x06,0x0d,0x36};
const uint8_t CampCmd_RemainingCapRcvOK[]=                               {0xc5,0x6a,0x29,0x08,0x0d};

const uint8_t CampCmd_FullCharCap[]=                                     {0xc5,0x6a,0x29,0x06,0x0e,0xd4};
const uint8_t CampCmd_FullCharCapRcvOK[]=                                {0xc5,0x6a,0x29,0x08,0x0e};

const uint8_t CampCmd_AccuEnergyOut[]=                   {0xc5,0x6a,0x29,0x06,0x0f,0x8a};
const uint8_t CampCmd_AccuEnergyOutRcvOK[]=              {0xc5,0x6a,0x29,0x08,0x0f};

const uint8_t CampCmd_Accu_Engery_Output_LoadPower []=                   {0xc5,0x6a,0x29,0x06,0x17,0xd5};
const uint8_t CampCmd_Accu_Engery_Output_LoadPowerRcvOK[]=               {0xc5,0x6a,0x29,0x0C,0x17};

const uint8_t CampCmd_Solar_Generation_Charge_Power[]=                   {0xc5,0x6a,0x29,0x06,0x18,0x94};
const uint8_t CampCmd_Solar_Generation_Charge_PowerRcvOK[]=              {0xc5,0x6a,0x29,0x0C,0x18};

const uint8_t CampCmd_AccuCycles[]=                      {0xc5,0x6a,0x29,0x06,0x10,0x56};
const uint8_t CampCmd_AccuCyclesRcvOK[]=                 {0xc5,0x6a,0x29,0x08,0x10};

const uint8_t CampCmd_HashTop[]=                         {0xc5,0x6a,0x29,0x06,0x11,0x08};
const uint8_t CampCmd_HashTopRcvOK[]=                    {0xc5,0x6a,0x29,0x0e,0x11};

const uint8_t CampCmd_RBD[]=                             {0xC5,0x6a,0x29,0x06,0x13,0xb4};
const uint8_t CampCmd_RBDRcvOK[]=                        {0xc5,0x6a,0x29,0x08,0x13};

const uint8_t CampCmd_CV1[]=                             {0xc5,0x6a,0x29,0x08,0x00,0x3f,0x02,0x0b};
const uint8_t CampCmd_CV1RcvOK[]=                        {0xc5,0x6a,0x29,0x0a,0x00,0x3f};

const uint8_t CampCmd_CV2[]=                             {0xc5,0x6a,0x29,0x08,0x00,0x3e,0x02,0xcf};
const uint8_t CampCmd_CV2RcvOK[]=                        {0xc5,0x6a,0x29,0x0a,0x00,0x3e};

const uint8_t CampCmd_CV3[]=                             {0xc5,0x6a,0x29,0x08,0x00,0x3d,0x02,0x9a};
const uint8_t CampCmd_CV3RcvOK[]=                        {0xc5,0x6a,0x29,0x0a,0x00,0x3d};

const uint8_t CampCmd_CV4[]=                             {0xc5,0x6a,0x29,0x08,0x00,0x3c,0x02,0x5e};
const uint8_t CampCmd_CV4RcvOK[]=                        {0xc5,0x6a,0x29,0x0a,0x00,0x3c};

const uint8_t CampCmd_BatteryVolatge[]=                  {0xc5,0x6a,0x29,0x08,0x00,0x09,0x02,0x8c};
const uint8_t CampCmd_BatteryVolatgeRcvOK[]=             {0xc5,0x6a,0x29,0x0a,0x00,0x09};

const uint8_t CampCmd_BatteryCurrent[]=                  {0xc5,0x6a,0x29,0x08,0x00,0x0a,0x02,0xd9};
const uint8_t CampCmd_BatteryCurrentRcvOK[]=             {0xc5,0x6a,0x29,0x0a,0x00,0x0a};

const uint8_t CampCmd_Temperature[]=                     {0xc5,0x6a,0x29,0x08,0x00,0x08,0x02,0x48};
const uint8_t CampCmd_TemperatureRcvOK[]=                {0xc5,0x6a,0x29,0x0a,0x00,0x08};

const uint8_t CampCmd_SoftRev[]=                         {0xc5,0x6a,0x29,0x06,0x14,0x37};
const uint8_t CampCmd_SoftRevRcvOK[]=                    {0xc5,0x6a,0x29,0x08,0x14};


__IO uint8_t g_Uart485Buf[UART3_RX_BUF_SIZE];

uint16_t  RxUart3Counter=0;
uint16_t  RxUart3ParsePos=0;
int16_t g_current=0;


HASH_TOP_Def Hash_Hi,Hash_Lo;
 char Hashstr[28];
 char hashshow[32];	

unsigned char crc_array[256] =
{
   0x00, 0x5e, 0xbc, 0xe2, 0x61, 0x3f, 0xdd, 0x83,
   0xc2, 0x9c, 0x7e, 0x20, 0xa3, 0xfd, 0x1f, 0x41, 
   0x9d, 0xc3, 0x21, 0x7f, 0xfc, 0xa2, 0x40, 0x1e,
   0x5f, 0x01, 0xe3, 0xbd, 0x3e, 0x60, 0x82, 0xdc, 
   0x23, 0x7d, 0x9f, 0xc1, 0x42, 0x1c, 0xfe, 0xa0,
   0xe1, 0xbf, 0x5d, 0x03, 0x80, 0xde, 0x3c, 0x62, 
   0xbe, 0xe0, 0x02, 0x5c, 0xdf, 0x81, 0x63, 0x3d,
   0x7c, 0x22, 0xc0, 0x9e, 0x1d, 0x43, 0xa1, 0xff,
   0x46, 0x18, 0xfa, 0xa4, 0x27, 0x79, 0x9b, 0xc5, 
   0x84, 0xda, 0x38, 0x66, 0xe5, 0xbb, 0x59, 0x07,
   0xdb, 0x85, 0x67, 0x39, 0xba, 0xe4, 0x06, 0x58,
   0x19, 0x47, 0xa5, 0xfb, 0x78, 0x26, 0xc4, 0x9a,
   0x65, 0x3b, 0xd9, 0x87, 0x04, 0x5a, 0xb8, 0xe6, 
   0xa7, 0xf9, 0x1b, 0x45, 0xc6, 0x98, 0x7a, 0x24, 
   0xf8, 0xa6, 0x44, 0x1a, 0x99, 0xc7, 0x25, 0x7b,
   0x3a, 0x64, 0x86, 0xd8, 0x5b, 0x05, 0xe7, 0xb9, 
   0x8c, 0xd2, 0x30, 0x6e, 0xed, 0xb3, 0x51, 0x0f, 
   0x4e, 0x10, 0xf2, 0xac, 0x2f, 0x71, 0x93, 0xcd,
   0x11, 0x4f, 0xad, 0xf3, 0x70, 0x2e, 0xcc, 0x92,
   0xd3, 0x8d, 0x6f, 0x31, 0xb2, 0xec, 0x0e, 0x50, 
   0xaf, 0xf1, 0x13, 0x4d, 0xce, 0x90, 0x72, 0x2c, 
   0x6d, 0x33, 0xd1, 0x8f, 0x0c, 0x52, 0xb0, 0xee, 
   0x32, 0x6c, 0x8e, 0xd0, 0x53, 0x0d, 0xef, 0xb1, 
   0xf0, 0xae, 0x4c, 0x12, 0x91, 0xcf, 0x2d, 0x73, 
   0xca, 0x94, 0x76, 0x28, 0xab, 0xf5, 0x17, 0x49,
   0x08, 0x56, 0xb4, 0xea, 0x69, 0x37, 0xd5, 0x8b,
   0x57, 0x09, 0xeb, 0xb5, 0x36, 0x68, 0x8a, 0xd4,
   0x95, 0xcb, 0x29, 0x77, 0xf4, 0xaa, 0x48, 0x16, 
   0xe9, 0xb7, 0x55, 0x0b, 0x88, 0xd6, 0x34, 0x6a, 
   0x2b, 0x75, 0x97, 0xc9, 0x4a, 0x14, 0xf6, 0xa8, 
   0x74, 0x2a, 0xc8, 0x96, 0x15, 0x4b, 0xa9, 0xf7,
   0xb6, 0xe8, 0x0a, 0x54, 0xd7, 0x89, 0x6b, 0x35,
};


extern UART_HandleTypeDef huart3;


unsigned char CRC8(unsigned char *p, char counter)
{
    unsigned char crc8 = 0;

    for( ; counter > 0; counter--)
    {
          crc8 = crc_array[crc8^*p]; //????CRC?
           p++;
    }
     return crc8;

 }

void CampInit(void)
{
	   memset((uint8_t*)g_Uart485Buf,0x00,UART3_RX_BUF_SIZE);
	   //HAL_UART_Receive_IT(&huart1,(uint8_t*)g_Uart1Buf,UART1_RX_BUF_SIZE);
	   huart3.RxXferSize=UART3_RX_BUF_SIZE;
	   huart3.pRxBuffPtr=(uint8_t*)g_Uart485Buf;
}
 

void CampSend(const uint8_t * buffer,uint16_t size)
{
    // HAL_UART_Transmit(&huart1,(uint8_t *)buffer,size,size*10);
    Uart3Send((uint8_t *)buffer,size);
}

void CampStart(void)
{
	g_CampGetState=CAMP_GET_VER;
}

void CampGetEnable(void)
{
	g_CampGetEnable=TRUE;
}

void CampInttoStrs(unsigned long n,char *s)//超大数据转换用的函数
{
  int i=0,j;
  char temp;
  while(n)//先将整数逆序保存进数组
  {
    s[i]=n%10+'0';//将十进制数字转换成字符保存
    n=n/10;
    i++;
  }
  for(j=i-1;j>=i/2;j--)//再将逆序的字符结果再逆转一次，得到正确结果
  {
    temp=s[j];
    s[j]=s[i-j-1];
    s[i-j-1]=temp;
  }
}

char* CampInsert(char *s1, char *s2, int n)
{
  int len1 = 0, len2 = 0, j = 0, k = 0;
  char s4[64];
	int i;

  len1 = strlen(s1);
  len2 = strlen(s2);
  
  if (s1 == NULL||s2 == NULL||n>len1)
   	 return NULL;

  for ( i = 0; i<n; i++)
  {
    j++;
  }
  for ( i= 0; i<len1; i++)
  {
    s4[k++] = s1[i];
  }

  for ( i = 0; i<len2; i++)
    s1[j++] = s2[i];

  for( i=n;i<len1;i++)
    s1[j++]=s4[i];

  s1[j] = '\0';


  return s1;
}

void CampHashOpration(uint8_t *buffer)
{
		int j;
    memset(hashshow, 0x00, 32);
    memset(Hashstr, 0x00, 28);//lsb ----msb
    Hash_Lo.Hash_char[3] = buffer[3];
    Hash_Lo.Hash_char[2] = buffer[2];
    Hash_Lo.Hash_char[1] = buffer[1];
    Hash_Lo.Hash_char[0] = buffer[0];
    Hash_Hi.Hash_char[3] = buffer[7];
    Hash_Hi.Hash_char[2] = buffer[6];
    Hash_Hi.Hash_char[1] = buffer[5];
    Hash_Hi.Hash_char[0] = buffer[4];

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

void CampGetTxCmd(void)
{
	switch(g_CampGetState)
	{
		//idle 
		case CAMP_GET_IDLE:
			break;
		//version 
		case CAMP_GET_VER:
			CampSend(CampCmd_SoftRev,6);
			break;
		//paygProductID
		case CAMP_GET_ID:
			CampSend(CampCmd_PPID,7);
			break;
		//oemProductID
		case CAMP_GET_OEMID:
			CampSend(CampCmd_OPID,7);
			break;
		//REV
		case CAMP_GET_REV:
			CampSend(CampCmd_SoftRev,8);
			break;
		//paygS
		case CAMP_GET_PAYGS:
			CampSend(CampCmd_PAYGState,6);
			break;
		//outputS
		case CAMP_GET_OUTPUTS:
			CampSend(CampCmd_OCS,6);
			CampSend(CampCmd_SSC, 6);
			break;
		//hashT
		case CAMP_GET_HASHT:
			CampSend(CampCmd_HashTop,6);
			break;
		//runDaysBP
		case CAMP_GET_RUNDAYSBP:
			CampSend(CampCmd_RBD, 6);
			break;
		//remDays
		case CAMP_GET_REMDAYS:
			CampSend(CampCmd_ReminingPAYGDays, 6);
			break;
		//runDays
		case CAMP_GET_RUNDAYS:
			CampSend(CampCmd_DaysRun, 6);
			break;
		//payDays
		case CAMP_GET_PAYDAYS:
			CampSend(CampCmd_PAYGDays, 6);
			break;
		//cCNT
		case CAMP_GET_CCNT:
			CampSend(CampCmd_AccuCycles, 6);
			break;
		//rCap
		case CAMP_GET_RCAP:
			CampSend(CampCmd_FullCharCap, 6);
			break;
		//socP
		case CAMP_GET_SOCP:
			CampSend(CampCmd_RelativeSOC, 6);
			break;
		//batC
		case CAMP_GET_BATC:
			 CampSend(CampCmd_RemainingCap, 6);
			break;
		//vIn
		case CAMP_GET_VIN:
			g_CampGetState++;
			break;
		//cIn
		case CAMP_GET_CIN:
			CampSend(CampCmd_BatteryCurrent, 8);
			break;
		//pIn
		case CAMP_GET_PIN:
			CampSend(CampCmd_Solar_Generation_Charge_Power, 6);
			break;
		//eIn
		case CAMP_GET_EIN:
			g_CampGetState++;
			break;
		//vOut
		case CAMP_GET_VOUT:
			CampSend(CampCmd_BatteryVolatge, 8);
			break;
		//cOut
		case CAMP_GET_COUT:
			CampSend(CampCmd_BatteryCurrent, 8);
			break;
		//pOut
		case CAMP_GET_POUT:
			CampSend(CampCmd_Accu_Engery_Output_LoadPower, 6);
			break;
		//eOut
		case CAMP_GET_EOUT:
			//g_CampGetState++;
			CampSend(CampCmd_AccuEnergyOut, 6);
			break;
		//vC1
		case CAMP_GET_VC1:
			CampSend(CampCmd_CV1, 8);
			break;
		//vC2
		case CAMP_GET_VC2:
			CampSend(CampCmd_CV2, 8);
			break;
		//vC3
		case CAMP_GET_VC3:
			CampSend(CampCmd_CV3, 8);
			break;
		//vC4
		case CAMP_GET_VC4:
			CampSend(CampCmd_CV3, 8);
			break;
		//pckT
		case CAMP_GET_PCKT:
			CampSend(CampCmd_Temperature, 8);
			break;
	}
}

void CampGetParse(uint8_t *buffer)
{
	uint8_t temp[10]={0};
	
	switch(g_CampGetState)
	{
		//idle 
		case CAMP_GET_IDLE:
			break;
		//version 
		case CAMP_GET_VER:
			break;
		//paygProductID
		case CAMP_GET_ID:
			GattSetPpid(buffer+6,20);
			break;
		//oemProductID
		case CAMP_GET_OEMID:
			GattSetOpid(buffer+6,14);
			break;
		//REV
		case CAMP_GET_REV:
			
			temp[2] =((buffer[6] << 8 | buffer[5])%10)+'0'	;
    			temp[1] = '.';
			temp[0]=(((buffer[6] << 8 | buffer[5])/10)%10)+'0';	
			GattSetVersion(temp,3);
			break;
		//paygS
		case CAMP_GET_PAYGS:
			GattSetPaygState(buffer[5] );
			break;
		//outputS
		case CAMP_GET_OUTPUTS:
			GattSetOutputCtrlState(buffer[5] );
			break;
		//hashT
		case CAMP_GET_HASHT:
			CampHashOpration(&buffer[5]);
			GattSetHashTop((uint8_t*)hashshow,29);
			break;
		//runDaysBP
		case CAMP_GET_RUNDAYSBP:
			GattSetRunDayBackup(buffer[6] << 8 | buffer[5]);
			break;
		//remDays
		case CAMP_GET_REMDAYS:
			GattSetRmPayDays(buffer[6] << 8 | buffer[5]);
			break;
		//runDays
		case CAMP_GET_RUNDAYS:
			GattSetRunDays(buffer[6] << 8 | buffer[5]);
			break;
		//payDays
		case CAMP_GET_PAYDAYS:
			GattSetPaygDays(buffer[6] << 8 | buffer[5]);
			break;
		//cCNT
		case CAMP_GET_CCNT:
			GattSetPaygDays(buffer[6] << 8 | buffer[5]);
			break;
		//rCap
		case CAMP_GET_RCAP:
			//CampSend(CampCmd_FullCharCap, 6);
			GattSetFullChrgeCap(buffer[6] << 8 | buffer[5]);
			break;
		//socP
		case CAMP_GET_SOCP:
			//CampSend(CampCmd_RelativeSOC, 6);
			GattSetRelativeSOC(buffer[6] << 8 | buffer[5]);
			break;
		//batC
		case CAMP_GET_BATC:
			 //CampSend(CampCmd_RemainingCap, 6);
			 GattSetRmCap(buffer[6] << 8 | buffer[5]);
			break;
		//vIn
		case CAMP_GET_VIN:
			GattSetBattVolt(15000);
			break;
		//cIn
		case CAMP_GET_CIN:
			//CampSend(CampCmd_BatteryCurrent, 8);
			//GattSetBattCurrent(buffer[8] << 8 | buffer[7]);
			g_current=(int16_t)(buffer[8] << 8 | buffer[7]);
			break;
		//pIn
		case CAMP_GET_PIN:
			GattSetChargePower(buffer[10] << 8 | buffer[9]);
			/*GattSetSolarGeneration(buffer[10] << 8 | buffer[9]);*/

			  memset(Hash_Hi.Hash_char, 0x00, 4);
			  Hash_Hi.Hash_char[0] = buffer[8];
			  Hash_Hi.Hash_char[1] = buffer[7];
			  Hash_Hi.Hash_char[2] = buffer[6];
			  Hash_Hi.Hash_char[3] = buffer[5];

			 // GattSetLoadPower(Hash_Hi.Hash_long);
			GattSetSolarGeneration(buffer[6] << 8 | buffer[5]/*Hash_Hi.Hash_long*/);
			break;
		//eIn
		case CAMP_GET_EIN:
			//GattSetSolarGeneration(buffer[10] << 8 | buffer[9]);
			break;
		//vOut
		case CAMP_GET_VOUT:
			GattSetBattVolt(buffer[8] << 8 | buffer[7]);
			break;
		//cOut
		case CAMP_GET_COUT:
			//g_current+=(int16_t)(buffer[8] << 8 | buffer[7]);
			//GattSetBattCurrent((uint16_t)g_current);
			GattSetBattCurrent(buffer[8] << 8 | buffer[7]);
			break;
		//pOut
		case CAMP_GET_POUT:
			

			 GattSetLoadPower(buffer[10] << 8 | buffer[9]);

			/*  memset(Hash_Hi.Hash_char, 0x00, 4);
			  Hash_Hi.Hash_char[0] = buffer[8];
			  Hash_Hi.Hash_char[1] = buffer[7];
			  Hash_Hi.Hash_char[2] = buffer[6];
			  Hash_Hi.Hash_char[3] = buffer[5];*/

			  //GattSetAccuEnergy(Hash_Hi.Hash_long);
			break;
		//eOut
		case CAMP_GET_EOUT:
			GattSetAccuEnergy(buffer[6] << 8 | buffer[7]);
			break;
		//vC1
		case CAMP_GET_VC1:
			GattSetCellVolt1(buffer[8] << 8 | buffer[7]);
			break;
		//vC2
		case CAMP_GET_VC2:
			GattSetCellVolt2(buffer[8] << 8 | buffer[7]);
			break;
		//vC3
		case CAMP_GET_VC3:
			GattSetCellVolt3(buffer[8] << 8 | buffer[7]);
			break;
		//vC4
		case CAMP_GET_VC4:
			GattSetCellVolt4(buffer[8] << 8 | buffer[7]);
			break;
		//pckT
		case CAMP_GET_PCKT:
			GattSetBattTemp(((buffer[8] << 8 | buffer[7] ) - 2731) / 10);
			break;
	}
}

uint8_t CampTokenState(void)
{
	uint16_t i,len=0;
	uint8_t ret=0xff;
	
	RxUart3Counter=huart3.RxXferCount;

	for(i=0;i<RxUart3Counter;i++)
	{
		if((g_Uart485Buf[i]==0xc5&&g_Uart485Buf[i+1]==0x6a)&&g_Uart485Buf[i+2]==0x29)
		{

			len=g_Uart485Buf[i+3];


			if(RxUart3Counter-i>=len&&RxUart3Counter-i>=4)
			{

				  if(g_Uart485Buf[i+len - 1] == CRC8((unsigned char *)&g_Uart485Buf[i], len - 1))
				  {
					if(g_Uart485Buf[i+5]==0x0a)
						ret=0;
					else if(g_Uart485Buf[i+5]==0x0b)
						ret=1;
					else if(g_Uart485Buf[i+5]==0xa0)
						ret=2;
				      }

			 memset((uint8_t*)g_Uart485Buf,0x00,UART485_RX_BUF_SIZE);
		   	 huart3.RxXferSize=UART_DI_RX_BUF_SIZE;
			 huart3.RxXferCount=0;
			 huart3.pRxBuffPtr=(uint8_t*)g_Uart485Buf; 

			}

			 
			}
		}

	return ret;
}

void CampGetTask(void)
{

	uint16_t i,len=0;
	
	RxUart3Counter=huart3.RxXferCount;

	for(i=0;i<RxUart3Counter;i++)
	{
		if((g_Uart485Buf[i]==0xc5&&g_Uart485Buf[i+1]==0x6a)&&g_Uart485Buf[i+2]==0x29)
		{

			len=g_Uart485Buf[i+3];


			if(RxUart3Counter-i>=len&&RxUart3Counter-i>=4)
			{

			  if(g_Uart485Buf[i+len - 1] == CRC8((unsigned char *)&g_Uart485Buf[i], len - 1))
			  {
			//          Camp_ComOK = 1;
			/*            memcpy(Camp_cmd, &g_Uart3Buf[i], len);
			           memcpy(Uart_Buffer, &g_Uart3Buf[i], len);
			            Serial_Cmd(g_Uart3Buf[i+4]);

				    Process_cmd_all(Camp_ComOK);*/

			CampGetParse((uint8_t*)&g_Uart485Buf[i]);

				
			if(g_CampGetState<CAMP_GET_PCKT)
				g_CampGetState++;
			else
				g_CampGetState=CAMP_GET_IDLE;
			      }

			 memset((uint8_t*)g_Uart485Buf,0x00,UART3_RX_BUF_SIZE);
		   	 huart3.RxXferSize=UART3_RX_BUF_SIZE;
			 huart3.RxXferCount=0;
			 huart3.pRxBuffPtr=(uint8_t*)g_Uart485Buf; 

			}

			 
			}
		}

	
       if(g_CampGetEnable)
       	{
			CampGetTxCmd();
			g_CampGetEnable=FALSE;
       	}
}


