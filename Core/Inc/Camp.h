#ifndef __CAMP_H__
#define __CAMP_H__

#define UART_DI_RX_BUF_SIZE 512// 256


typedef enum
{
	CAMP_GET_IDLE,
	//version 
	CAMP_GET_VER,
	//paygProductID
	CAMP_GET_ID,
	//oemProductID
	CAMP_GET_OEMID,
	//REV
	CAMP_GET_REV,
	//paygS
	CAMP_GET_PAYGS,
	//outputS
	CAMP_GET_OUTPUTS,
	//hashT
	CAMP_GET_HASHT,
	//runDaysBP
	CAMP_GET_RUNDAYSBP,
	//remDays
	CAMP_GET_REMDAYS,
	//runDays
	CAMP_GET_RUNDAYS,
	//payDays
	CAMP_GET_PAYDAYS,
	//cCNT
	CAMP_GET_CCNT,
	//rCap
	CAMP_GET_RCAP,
	//socP
	CAMP_GET_SOCP,
	//batC
	CAMP_GET_BATC,
	//vIn
	CAMP_GET_VIN,
	//cIn
	CAMP_GET_CIN,
	//pIn
	CAMP_GET_PIN,
	//eIn
	CAMP_GET_EIN,
	//vOut
	CAMP_GET_VOUT,
	//cOut
	CAMP_GET_COUT,
	//pOut
	CAMP_GET_POUT,
	//eOut
	CAMP_GET_EOUT,
	//vC1
	CAMP_GET_VC1,
	//vC2
	CAMP_GET_VC2,
	//vC3
	CAMP_GET_VC3,
	//vC4
	CAMP_GET_VC4,
	//pckT
	CAMP_GET_PCKT,
}CAMP_GET_DEF;


typedef union {

    unsigned char Hash_char[4];
    unsigned long Hash_long;
}HASH_TOP_Def;

void CampStart(void);
void CampInit(void);
void CampGetEnable(void);
void CampSend(const uint8_t * buffer,uint16_t size);
void CampGetTxCmd(void);
void CampGetParse(uint8_t *buffer);
void CampGetTask(void);
unsigned char CRC8(unsigned char *p, char counter);
uint8_t CampTokenState(void);
char* CampInsert(char *s1, char *s2, int n);
void CampInttoStrs(unsigned long n,char *s);
#endif

