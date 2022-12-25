
#ifndef __OFF_GRID_H__
#define __OFF_GRID_H__

enum
{
	OFFGRID_INPUT_REG,
	OFFGRID_HOLD_REG	
};
	
typedef struct
{
	uint8_t addr;
	uint8_t func;
	uint8_t regH;
	uint8_t regL;
	
	uint8_t lenH;
	uint8_t lenL;

	  
	uint8_t crcL;  
	uint8_t crcH;

}BMS_QUERY_TypeDef;


typedef struct
{
	uint8_t addr;
	uint8_t func;
	uint8_t len;
    
	uint8_t buff[33];  //data H,data L .....  crc H,crc L

}BMS_QUERY_RSP_TypeDef;

typedef struct
{
	uint8_t addr;
	uint8_t func;
	uint8_t regH;
	uint8_t regL;

	uint8_t dataH;
	uint8_t dataL;

	uint8_t crcL; 
	uint8_t crcH;  
	

}BMS_PSET_TypeDef;



typedef struct
{
	uint8_t addr;
	uint8_t func;
	uint8_t regH;
	uint8_t regL;
	
	uint8_t dataH;
	uint8_t dataL;
    
	 
	uint8_t crcL; 
	uint8_t crcH; 

}BMS_PSET_RSP_TypeDef;

void OffGridBmsInit(void);
void OffGrid_GetInfo(uint16_t reg,uint16_t len);
void OffGrid_SetReg(uint16_t reg,uint16_t state);
void OffGridBmsTask(void);
void OffGrid_GetHoldReg(uint16_t reg,uint16_t len);


#endif

