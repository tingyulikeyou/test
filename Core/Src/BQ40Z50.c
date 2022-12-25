#include "main.h"



BQ40Z50_TypeDef g_bq40z50_state;



//extern I2C_HandleTypeDef hi2c1;

__IO uint8_t g_BmsGetInfor_Enable=0;


uint8_t BQ40Z50_GetInfo(void)
{
    uint8_t  error_cnt=0;
	#if 0
	
    if(HAL_I2C_Mem_Read(&hi2c1,IC_ADDRESS,0x3d,I2C_MEMADD_SIZE_8BIT, g_bq40z50_state.CV1, 2,2000)!=HAL_OK)
		error_cnt++ ;
	 
    if(HAL_I2C_Mem_Read(&hi2c1,IC_ADDRESS,0x3d,I2C_MEMADD_SIZE_8BIT, g_bq40z50_state.CV2, 2,2000)!=HAL_OK)
		error_cnt++ ;
    
    if(HAL_I2C_Mem_Read(&hi2c1,IC_ADDRESS, 0x3e,I2C_MEMADD_SIZE_8BIT, g_bq40z50_state.CV3, 2,2000)!=HAL_OK)
		error_cnt++ ;
    
    if(HAL_I2C_Mem_Read(&hi2c1,IC_ADDRESS, 0x3f,I2C_MEMADD_SIZE_8BIT, g_bq40z50_state.CV4, 2,2000)!=HAL_OK)
		error_cnt++ ;
    
    if(HAL_I2C_Mem_Read(&hi2c1,IC_ADDRESS,0x08,I2C_MEMADD_SIZE_8BIT, g_bq40z50_state.Temperature,  2,2000)!=HAL_OK)
		error_cnt++ ;
    
    if(HAL_I2C_Mem_Read(&hi2c1,IC_ADDRESS, 0x4f, I2C_MEMADD_SIZE_8BIT, g_bq40z50_state.State_of_Health,2,2000)!=HAL_OK)
		error_cnt++ ;
    
    if(HAL_I2C_Mem_Read(&hi2c1,IC_ADDRESS, 0x17,I2C_MEMADD_SIZE_8BIT, g_bq40z50_state.Cycle_Count, 2,2000)!=HAL_OK)
		error_cnt++ ;
    
    if(HAL_I2C_Mem_Read(&hi2c1,IC_ADDRESS, 0x0d,I2C_MEMADD_SIZE_8BIT, g_bq40z50_state.Relative_State_of_Charge,2,2000)!=HAL_OK)
		error_cnt++ ;
    
    if(HAL_I2C_Mem_Read(&hi2c1,IC_ADDRESS,0x10, I2C_MEMADD_SIZE_8BIT, g_bq40z50_state.Full_charge_Capacity, 2,2000)!=HAL_OK)
		error_cnt++ ;
    
    if(HAL_I2C_Mem_Read(&hi2c1,IC_ADDRESS,0x0F,I2C_MEMADD_SIZE_8BIT, g_bq40z50_state.Remaning_Capacity,  2,2000)!=HAL_OK)
		error_cnt++ ;
    
    if(HAL_I2C_Mem_Read(&hi2c1,IC_ADDRESS,0x0a,I2C_MEMADD_SIZE_8BIT, g_bq40z50_state.Current,  2,2000)!=HAL_OK)
		error_cnt++ ;
    
    if(HAL_I2C_Mem_Read(&hi2c1,IC_ADDRESS,0x09, I2C_MEMADD_SIZE_8BIT, g_bq40z50_state.Voltage, 2,2000)!=HAL_OK)
		error_cnt++ ;
  
    if(HAL_I2C_Mem_Read(&hi2c1,IC_ADDRESS, 0x11,I2C_MEMADD_SIZE_8BIT, g_bq40z50_state.Run_Time_To_Empty, 2,2000)!=HAL_OK)
		error_cnt++ ;
  
    if(HAL_I2C_Mem_Read(&hi2c1,IC_ADDRESS,0x13,I2C_MEMADD_SIZE_8BIT, g_bq40z50_state.Run_Time_To_Full,  2,2000)!=HAL_OK)
		error_cnt++ ;
  
    if(HAL_I2C_Mem_Read(&hi2c1,IC_ADDRESS,0x71,I2C_MEMADD_SIZE_8BIT, g_bq40z50_state.Average_Power,  2,2000)!=HAL_OK)
		error_cnt++ ;
    
    //OperationStatus Reg Adress £º0x54
    if(HAL_I2C_Mem_Read(&hi2c1,IC_ADDRESS,0x54,I2C_MEMADD_SIZE_8BIT, g_bq40z50_state.OperationStatus,  4,2000)!=HAL_OK)
		error_cnt++ ;
    #endif
     return error_cnt;	
}


void BmsComEnable(void)
{
	g_BmsGetInfor_Enable=TRUE;
}

void BmsProcess(void)
{

	if(g_BmsGetInfor_Enable)
	{	
		BQ40Z50_GetInfo();

		GattSetSysStatusCode( 0);//System_Status_Code
		GattSetRelativeSOC( g_bq40z50_state.Relative_State_of_Charge[1] << 8 | g_bq40z50_state.Relative_State_of_Charge[0]);//Relative_SOC
		GattSetRmCap(g_bq40z50_state.Remaning_Capacity[1] << 8 | g_bq40z50_state.Remaning_Capacity[0] );//Remaining_Capacity
		GattSetFullChrgeCap( g_bq40z50_state.Full_charge_Capacity[1] << 8 | g_bq40z50_state.Full_charge_Capacity[0]);//Full_Charge_Capacity
		
		GattSetAccuCyc( g_bq40z50_state.Cycle_Count[1] << 8 | g_bq40z50_state.Cycle_Count[0] );//Accu_Cycles
		GattSetChargePower( 15000);//Charge_Power
		
		//GattSetAccuEnergy(g_bq40z50_state.a[1] << 8 | g_bq40z50_state.CV1[0] );//Accu_Energy_Output
		GattSetBattVolt(g_bq40z50_state.Voltage[1] << 8 | g_bq40z50_state.Voltage[1]);//BatteryVoltage
		GattSetBattCurrent( g_bq40z50_state.Current[1] << 8 | g_bq40z50_state.Current[0] );//BatteryCurrent


		//GattSetAccuEnergy(g_bq40z50_state.a[1] << 8 | g_bq40z50_state.CV1[0] );
		GattSetCellVolt1(g_bq40z50_state.CV1[1] << 8 | g_bq40z50_state.CV1[0] );
		GattSetCellVolt2(g_bq40z50_state.CV2[1] << 8 | g_bq40z50_state.CV2[0] );
		GattSetCellVolt3(g_bq40z50_state.CV3[1] << 8 | g_bq40z50_state.CV3[0] );
		GattSetCellVolt4(g_bq40z50_state.CV4[1] << 8 | g_bq40z50_state.CV4[0] );

		GattSetBattTemp(((g_bq40z50_state.Temperature[1] << 8 | g_bq40z50_state.Temperature[0] ) - 2731) / 10);

		
		}

	g_BmsGetInfor_Enable=FALSE;

}












