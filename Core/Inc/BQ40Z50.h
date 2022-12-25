
#ifndef __BQ40Z50_H__
#define __BQ40Z50_H__


#define CURRENT_CAL     0.0657//0.0738
#define CAP_CAP        86000/(g_bq40z50_state.Full_charge_Capacity[0]| g_bq40z50_state.Full_charge_Capacity[1]<<8)// 86000/0x3988
#define IC_ADDRESS      0x16



typedef struct  
{
  unsigned char Temperature[2];
  unsigned char Voltage[2];
  unsigned char Current[2];
  unsigned char Relative_State_of_Charge[2];
  unsigned char Remaning_Capacity[2];
  unsigned char Full_charge_Capacity[2];
  unsigned char Cycle_Count[2];
  unsigned char State_of_Health[2];
  unsigned char CV1[2];
  unsigned char CV2[2];
  unsigned char CV3[2];
  unsigned char CV4[2];
  unsigned char Run_Time_To_Empty[2];
  unsigned char Run_Time_To_Full[2];
  unsigned char Average_Power[2];
   unsigned char Charge_Power[2];
  unsigned char Solar_Power[4];
  unsigned char Acc_Power[4];
  unsigned char OperationStatus[4];
  unsigned char Error;
  unsigned char OutputState;
} BQ40Z50_TypeDef;


uint8_t BQ40Z50_GetInfo(void);
void BmsComEnable(void);
void BmsProcess(void);

#endif

