
#ifndef __PAYG_H__
#define __PAYG_H__


#define GATT_VERSON  "3.3.3"
#define HASH_MAX        3287

#define YES 0xA5D2635A
#define NO  0x5A63D2A5
#define NUL  !YES


#define APN_LEN                 32
#define APN_USENAME_LEN         16
#define APN_PASSWORD_LEN        16

#define MQTT_BROKER_LEN         64
#define MQTT_PORT_LEN           6
#define MQTT_USENAME_LEN        8
#define MQTT_PASSWORD_LEN       10

#define SETAPN_LEN              80

#define MEARSURE_FRE_LEN        8
#define TRANSMISSION_FRE_LEN    8

enum
{
	TOKEN_OK,
	TOKEN_USEED,
	TOKEN_ERROR
};


#define GET_UINT32(n,b,i)                       \
{                                               \
  (n) = ( (uint32_t) (b)[(i)    ] << 24 )       \
    | ( (uint32_t) (b)[(i) + 1] << 16 )       \
      | ( (uint32_t) (b)[(i) + 2] <<  8 )       \
        | ( (uint32_t) (b)[(i) + 3]       );      \
}

#define PUT_UINT32(n,b,i)                       \
{                                               \
  (b)[(i)    ] = (uint8_t) ( (n) >> 24 );       \
    (b)[(i) + 1] = (uint8_t) ( (n) >> 16 );       \
      (b)[(i) + 2] = (uint8_t) ( (n) >>  8 );       \
        (b)[(i) + 3] = (uint8_t) ( (n)       );       \
}


typedef struct
{
  uint32_t total[2];
  uint32_t state[5];
  uint8_t buffer[64];
}sha1_context;

typedef struct
{
	uint8_t oem_id[20];
	uint8_t payg_id[20];
	uint32_t hast_otp[2];
        uint32_t hast_root[2]; 
	uint32_t recharge_days; 
	uint32_t days_has_been_runing;//辰???DD足足那y
	uint32_t remaing_days; //那㏒車角足足那y
	uint32_t run_days_backup; 
	uint32_t free;  //PAYG ℅∩足?
	uint32_t lock; //豕“?T
	//????2?那y             
	uint32_t hast_input[2]; //?邦??那?豕?o赤那?3?
	uint32_t dem_cnt; //?Y那??㏒那?∩?那y
	uint32_t dem_time; ////?Y那??㏒那?那㏒車角那㊣??
	//那㊣?????? 
	uint32_t time_cnt; //??足足那㊣??角??? ﹞??車????
        uint32_t day_cem;
	
   
} PAYG_TypeDef;

typedef struct Net_Info{
        char apn                [APN_LEN];
        char apn_usename        [APN_USENAME_LEN];
        char apn_password       [APN_PASSWORD_LEN];
        
        char mqtt_broker        [MQTT_BROKER_LEN];
        char mqtt_port          [MQTT_PORT_LEN];
        char mqtt_usename       [MQTT_USENAME_LEN];
        char mqtt_password      [MQTT_PASSWORD_LEN];
}NET_INFOR_TypeDef;
typedef struct CampFrequen{
        char M_frequen                [MEARSURE_FRE_LEN];
        char T_frequen                [TRANSMISSION_FRE_LEN];
}CAMP_FREQ_TypeDef;

void PaygInit(void);
PAYG_TypeDef* PaygGetPaygBuf(void);
void PaygOvesTask(void);
void PaygOvesInput(void);
void PaygProcess(void);
void PaygUpdateEnable(void);
uint32_t  PaygGetPayState(void);
uint32_t  PaygGetFreeState(void);

#endif










