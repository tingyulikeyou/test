
#include"main.h"


uint8_t g_pub_json[JSON_LEN];
GATT_PARA_Def g_gatt_value;
GATT_META_Def g_gatt_meta_ctrl;

/*uint8_t g_gatt_att[];
uint8_t g_gatt_cmd[];
uint8_t g_gatt_sts[];
uint8_t g_gatt_dta[];
uint8_t g_gatt_dia[];*/


uint8_t g_GattMem[MEM_GATT_SIZE];

const uint8_t g_GattlistMemberNum[LIST_COUNT]={ATT_COUNT,CMD_COUNT,STS_COUNT,DTA_COUNT,DIA_COUNT};




extern USER_SET_TypeDef g_UserSet;

const GATT1_Def  g_gatt1_list=
{
	{//att	
		//"_meta": "Attributes are immutable once set, typically at factory",
		{"opid",&g_GattMem[MEM_ADDR_OPID],"OEM Device ID. Factory set.",TYPE_STRS},
		{"ppid",&g_GattMem[MEM_ADDR_PPID],"PAYG Device ID. Factory set. Field override options.",TYPE_STRS},
		{"flid",&g_GattMem[MEM_ADDR_FLID],"Fleet ID. Factory set. Field override options.",TYPE_STRS},
		{"frmv",&g_GattMem[MEM_ADDR_FRMV]," Firmware Version. Factory set.",TYPE_STRS}
		},
		
	{//cmd	
		//"_meta": "Commands are values writed by external client.  Also readable",
		{"pubk",&g_GattMem[MEM_ADDR_PUBK], "Public Key / Last Code",TYPE_STRS},
		{"gstw",&g_GattMem[MEM_ADDR_GSTW],"GPRS Sleep Time Window in mins",TYPE_UINT16},
		{"gctw",&g_GattMem[MEM_ADDR_GCTW], "GPRS Connect Time Window in mins",TYPE_UINT16},
		{"napn",&g_GattMem[MEM_ADDR_NAPN], "GPRS Carrier APN Name",TYPE_STRS},
		{"swch",&g_GattMem[MEM_ADDR_SWCH],"Generic device switch swch = ON | OFF ",TYPE_UINT16},
		{"read",&g_GattMem[MEM_ADDR_READ], "Last read request of ANY GATT <data> = [opid|...|]",TYPE_UINT16},
		{"rptm",&g_GattMem[MEM_ADDR_RPTM], "Set notification request mode = [0|1|2|3|4], 0: Auto-Periodic | 1 = Request-Response | 2 = Change-Notification",TYPE_UINT16},
		{"hbfq",&g_GattMem[MEM_ADDR_HBFQ],"Heart Beat Interval Minutes",TYPE_UINT16}

		#ifdef P10KW_PROJECT
		,
		  {"aoct",&g_GattMem[MEM_ADDR_AOCT],"AC Output Control { [x00-- / x01-- | x--01 / x--01] = [Output Enable On/Off | Standby On/Off]}",TYPE_UINT16},
	      {"aoss",&g_GattMem[MEM_ADDR_AOSS],"AC Output Source Selectoion Priority {0:BAT | 1:PV | 2:UTL | 3:PV & UTL}",TYPE_UINT16},
	      {"bcss",&g_GattMem[MEM_ADDR_BCSS],"Battery Charge Source Selectoion Priority {0:PV First | 1:PV@UTL | 2:PV Only}",TYPE_UINT16},
	      {"uost",&g_GattMem[MEM_ADDR_UOST],"Utility Output Start Time",TYPE_UINT16},
	      {"uoet",&g_GattMem[MEM_ADDR_UOET],"Utility Output End Time",TYPE_UINT16},
	      {"ucst",&g_GattMem[MEM_ADDR_UCST],"Utility Charge Start Time",TYPE_UINT16},
	      {"ucet",&g_GattMem[MEM_ADDR_UCET],"Utility Charge End Time",TYPE_UINT16},
	      {"pimd",&g_GattMem[MEM_ADDR_PIMD],"PV Input Mode {0：Independent | 1：Parallel}",TYPE_UINT16},
	      {"aimd",&g_GattMem[MEM_ADDR_AIMD],"PV Input Mode {0：APL,90-280VAC | 1：UPS,170-280VAC | 2：GEN}",TYPE_UINT16},
	      {"lngs",&g_GattMem[MEM_ADDR_LNGS],"Language Selection {0：Chinese | 1：English}",TYPE_UINT16},
	      {"aovs",&g_GattMem[MEM_ADDR_AOVS],"AC Output Voltage Selection {0:208 | 1:230 | 2:240 | 3:220 4:100 | 5:110 | 6:120}",TYPE_UINT16}, 
	      {"aofs",&g_GattMem[MEM_ADDR_AOFS],"Output Frequency Selection {0:50 | 1:60}",TYPE_UINT16},  
	      {"olrs",&g_GattMem[MEM_ADDR_OLRS],"Overload Restart Mode {0:Yes | 1:No | 2:UTL}",TYPE_UINT16},  
	      {"otrs",&g_GattMem[MEM_ADDR_OTRS],"Over Temperature Restart Mode {0:Yes | 1:No }",TYPE_UINT16},  
	      {"bzon",&g_GattMem[MEM_ADDR_BZON],"Buzzer Enable On/Off",TYPE_UINT16}, 
	      {"cass",&g_GattMem[MEM_ADDR_CASS],"Communication Address SS?",TYPE_UINT16},
	      {"mxcc",&g_GattMem[MEM_ADDR_MXCC],"Max Charge Current Setting",TYPE_UINT16},
	      {"bbcv",&g_GattMem[MEM_ADDR_BBCV],"Bulk Charge Phase Voltage Setting",TYPE_UINT16},
	      {"bfcv",&g_GattMem[MEM_ADDR_BFCV],"Float Charge Phase Voltage Setting",TYPE_UINT16},
	      {"lbus",&g_GattMem[MEM_ADDR_LBUS],"Low Battery Switch To Utility Voltage Threshold",TYPE_UINT16},
	      {"accs",&g_GattMem[MEM_ADDR_ACCS],"AC Charge Current Setting",TYPE_UINT16},
	      {"agmd",&g_GattMem[MEM_ADDR_AGMD],"Aging Mode?",TYPE_UINT16},
	      {"stys",&g_GattMem[MEM_ADDR_STYS],"System Time Year Set",TYPE_UINT16},
	      {"stms",&g_GattMem[MEM_ADDR_STMS],"System Time Month Set",TYPE_UINT16},
	      {"stds",&g_GattMem[MEM_ADDR_STDS],"System Time Day Set",TYPE_UINT16},
	      {"sths",&g_GattMem[MEM_ADDR_STHS],"System Time Hour Set",TYPE_UINT16},
	      {"stmm",&g_GattMem[MEM_ADDR_STMM],"System Time Minute Set",TYPE_UINT16},
	      {"stss",&g_GattMem[MEM_ADDR_STSS],"System Time Second Set" ,TYPE_UINT16},    
		  #endif
		},	
	{//sts
		//{"_meta",&g_GattMem[MEM_ADDR_RPTM],"States are discrete state variables.  All Omnivoltaic devices share common state set. Read only",},
		{"sstc",&g_GattMem[MEM_ADDR_SSTC],"System Status Code",TYPE_UINT16},
		{"crtm",&g_GattMem[MEM_ADDR_CRTM],"created at time",TYPE_STRS},
		{"udtm",&g_GattMem[MEM_ADDR_UDTM],"update at time",TYPE_STRS},
		{"time",&g_GattMem[MEM_ADDR_TIME],"Device internal timer readout at last communication",TYPE_STRS },
		{"plat",&g_GattMem[MEM_ADDR_PLAT],"Device GPS Latitude",TYPE_FLOAT_STR},
		{"plon",&g_GattMem[MEM_ADDR_PLON],"Device GPS Longitude",TYPE_FLOAT_STR},
		{"gpss",&g_GattMem[MEM_ADDR_GPSS],"GPS Signal Strength in db",TYPE_UINT16},
		{"gpfc",&g_GattMem[MEM_ADDR_GPFC],"GPRS Connect Failure Counts",TYPE_UINT16},
		{"rcrd",&g_GattMem[MEM_ADDR_RCRD],"Remaining Credit Days",TYPE_UINT16},
		{"trhd",&g_GattMem[MEM_ADDR_TRHD],"Total Running History Days",TYPE_UINT16},
		{"tpgd",&g_GattMem[MEM_ADDR_TPGD],"PAYG Running Days",TYPE_UINT16},
		{"pgst",&g_GattMem[MEM_ADDR_PGST],"PAYG State = PAYG | FREE",TYPE_UINT16},
		{"tkre",&g_GattMem[MEM_ADDR_TKRE],"Last token entry response = NEW | REPEAT | ERROR",TYPE_UINT16},
		{"ocst",&g_GattMem[MEM_ADDR_OCST],"Device Output State = ENABLED | DISABLED",TYPE_UINT16}
	},
	#ifdef UI1K_V13_PROJECT
	{	//dta
		//{"_meta":"Camp specific data available for clients. Read only",
		{"batp",&g_GattMem[MEM_ADDR_BATP],"Battery Net Charging Power W",TYPE_UINT16},
		{"inpp",&g_GattMem[MEM_ADDR_INPP],"Total Input Power W",TYPE_UINT16},
		{"outp",&g_GattMem[MEM_ADDR_OUTP],"Output Power W,",TYPE_UINT16},
		{"aeng",&g_GattMem[MEM_ADDR_AENG],"Accumulated Energy Output in Whs",TYPE_UINT16},
		{"pckv",&g_GattMem[MEM_ADDR_PCKV],"Pack Voltage mV",TYPE_UINT32},
		{"pckc",&g_GattMem[MEM_ADDR_PCKC],"Pack Current in mA",TYPE_INT},
		{"rsoc",&g_GattMem[MEM_ADDR_RSOC],"Relative State of Charge",TYPE_UINT16},
		{"rcap",&g_GattMem[MEM_ADDR_RCAP],"Remaining Capacity in Whs",TYPE_UINT16},
		{"fccp",&g_GattMem[MEM_ADDR_FCCP],"Full Charge Capacity in Whs",TYPE_UINT16},
		{"pckt",&g_GattMem[MEM_ADDR_PCKT],"Pack Temperature in Celsius",TYPE_INT},
		{"acyc",&g_GattMem[MEM_ADDR_ACYC],"Accumulated Cycles",TYPE_UINT16}
		},
		
	{	//dia
		//{"_meta":"Oasis-1K specific diagnostics available service personnel. Read only",
		{"cv01",&g_GattMem[MEM_ADDR_CV01],"Cell 01 Voltage mV",TYPE_UINT16},
		{"cv02",&g_GattMem[MEM_ADDR_CV02],"Cell 02 Voltage mV",TYPE_UINT16},
		{"cv03",&g_GattMem[MEM_ADDR_CV03],"Cell 03 Voltage mV",TYPE_UINT16},
		{"cv04",&g_GattMem[MEM_ADDR_CV04],"Cell 04 Voltage mV",TYPE_UINT16},
		{"cv05",&g_GattMem[MEM_ADDR_CV05],"Cell 05 Voltage mV",TYPE_UINT16},
		{"cv06",&g_GattMem[MEM_ADDR_CV06],"Cell 06 Voltage mV",TYPE_UINT16},
		{"cv07",&g_GattMem[MEM_ADDR_CV07],"Cell 07 Voltage mV",TYPE_UINT16}

		}	
	#elif defined(P10KW_PROJECT)	
	{	//dta
		//{"_meta":"Camp specific data available for clients. Read only",
		{"batp",&g_GattMem[MEM_ADDR_BATP],"Battery Net Charging Power W",TYPE_UINT16},
		{"inpp",&g_GattMem[MEM_ADDR_INPP],"Total Input Power W",TYPE_UINT16},
		{"outp",&g_GattMem[MEM_ADDR_OUTP],"Output Power W,",TYPE_UINT16},
		{"aeng",&g_GattMem[MEM_ADDR_AENG],"Accumulated Energy Output in Whs",TYPE_UINT16},
		{"pckv",&g_GattMem[MEM_ADDR_PCKV],"Pack Voltage mV",TYPE_UINT32},
		{"pckc",&g_GattMem[MEM_ADDR_PCKC],"Pack Current in mA",TYPE_INT},
		{"rsoc",&g_GattMem[MEM_ADDR_RSOC],"Relative State of Charge",TYPE_UINT16},
		{"rcap",&g_GattMem[MEM_ADDR_RCAP],"Remaining Capacity in Whs",TYPE_UINT16},
		{"fccp",&g_GattMem[MEM_ADDR_FCCP],"Full Charge Capacity in Whs",TYPE_UINT16},
		{"pckt",&g_GattMem[MEM_ADDR_PCKT],"Pack Temperature in Celsius",TYPE_INT},
		{"acyc",&g_GattMem[MEM_ADDR_ACYC],"Accumulated Cycles",TYPE_UINT16},
        {"pv1v",&g_GattMem[MEM_ADDR_PV1V],"PV1 Voltage",TYPE_UINT16},
        {"pv2v",&g_GattMem[MEM_ADDR_PV2V],"PV2 Voltage",TYPE_UINT16},
        {"pv1p",&g_GattMem[MEM_ADDR_PV1P],"PV1 Charge Power",TYPE_UINT32},
        {"pv2p",&g_GattMem[MEM_ADDR_PV2V],"PV2 Charge Power",TYPE_UINT32},
        {"cg1c",&g_GattMem[MEM_ADDR_CG1C],"Charge Current 1",TYPE_UINT16}, 
        {"cg2c",&g_GattMem[MEM_ADDR_CG2C],"Charge Current 2",TYPE_UINT16}, 
        {"aoap",&g_GattMem[MEM_ADDR_AOAP],"AC Output Active Power (W)",TYPE_UINT32}, 
        {"aova",&g_GattMem[MEM_ADDR_AOVA],"AC Output Apparent Power (AV)",TYPE_UINT32},
        {"acap",&g_GattMem[MEM_ADDR_ACAP],"AC Charge Power",TYPE_UINT32},
        {"acva",&g_GattMem[MEM_ADDR_ACVA],"AC Charge Apparent Power",TYPE_UINT32},
        {"bpkv",&g_GattMem[MEM_ADDR_BPKV],"Battery Pack Voltage",TYPE_UINT16},
        {"acov",&g_GattMem[MEM_ADDR_ACOV],"AC Output Voltage",TYPE_UINT16},  
        {"acof",&g_GattMem[MEM_ADDR_ACOF],"AV Output Frequency",TYPE_UINT16},
        {"dcov",&g_GattMem[MEM_ADDR_DCOV],"DC Output Voltage",TYPE_UINT16}, 
        {"ldpp",&g_GattMem[MEM_ADDR_LDPP],"Load Power Percent",TYPE_UINT16}, 
        {"outc",&g_GattMem[MEM_ADDR_OUTC],"Output Current",TYPE_UINT16}, 
        {"aipp",&g_GattMem[MEM_ADDR_AIPP],"AC Input Power",TYPE_UINT16},
        {"fltb",&g_GattMem[MEM_ADDR_FLTB],"Fault Bit",TYPE_UINT16}, 
        {"wrnb",&g_GattMem[MEM_ADDR_WRNB],"Warning Bit",TYPE_UINT32},  
        {"acde",&g_GattMem[MEM_ADDR_ACDE],"AC Charge Energy Today (24hrs)",TYPE_UINT32},   
        {"acte",&g_GattMem[MEM_ADDR_ACTE],"AC Charge Energy Total",TYPE_UINT32},  
        {"bdde",&g_GattMem[MEM_ADDR_BDDE],"Battery Discharge Energy Today (24 Hrs)",TYPE_UINT32},  
        {"bdte",&g_GattMem[MEM_ADDR_BDTE],"Battery Discharge Energy Total",TYPE_UINT32},
        {"aode",&g_GattMem[MEM_ADDR_AODE],"AC Output Energy Today (24 Hrs)",TYPE_UINT32},
        {"aote",&g_GattMem[MEM_ADDR_AOTE],"AC Output Energy Total",TYPE_UINT32}, 
        {"btnp",&g_GattMem[MEM_ADDR_BTNP],"Battery Net Power (Signed)",TYPE_UINT32}, 
        {"bocf",&g_GattMem[MEM_ADDR_BOCF],"Battery Over-Charge Flag 0=NO",TYPE_UINT16}  

		},
		
	{	//dia
		//{"_meta":"Oasis-1K specific diagnostics available service personnel. Read only",
		{"busv",&g_GattMem[MEM_ADDR_BUSV],"DC Bus Voltage",TYPE_UINT16},
        {"aciv",&g_GattMem[MEM_ADDR_ACIV],"AC Input Voltage",TYPE_UINT16},
        {"acif",&g_GattMem[MEM_ADDR_ACIF],"AV Input Frequency",TYPE_UINT16},
        {"ivtt",&g_GattMem[MEM_ADDR_IVTT],"DC-AC Inverter Temperature",TYPE_UINT16},
        {"cvtt",&g_GattMem[MEM_ADDR_CVTT],"DC-DC Converter Tempature",TYPE_UINT16},
        {"bptv",&g_GattMem[MEM_ADDR_BPTV],"Battery Port Voltage",TYPE_UINT16},
        {"bbsv",&g_GattMem[MEM_ADDR_BBSV],"Battery Bus Voltage",TYPE_UINT16},
        {"wktm",&g_GattMem[MEM_ADDR_WKTM],"Work Time Total",TYPE_UINT32},
        {"bk1t",&g_GattMem[MEM_ADDR_BK1T],"Buck 1 Tempature",TYPE_UINT16}, 
        {"bk2t",&g_GattMem[MEM_ADDR_BK2T],"Buck 2 Tempature",TYPE_UINT16},
        {"intc",&g_GattMem[MEM_ADDR_INTC],"Inverter Current",TYPE_UINT16},
        {"aiap",&g_GattMem[MEM_ADDR_AIAP],"AC Input Power",TYPE_UINT32},
        {"aiva",&g_GattMem[MEM_ADDR_AIAP],"AC Input Apparent Power",TYPE_UINT32},
        {"wrnv",&g_GattMem[MEM_ADDR_WRNV],"Warning Value",TYPE_UINT32},
        {"dvtc",&g_GattMem[MEM_ADDR_DVTC],"Device Type Code",TYPE_UINT16},
        {"dvcs",&g_GattMem[MEM_ADDR_DVCS],"Device Self Check Step: 1:PV1 | 2:PV2 | 3: AC Power",TYPE_UINT16},
        {"prlm",&g_GattMem[MEM_ADDR_PRLM],"Production Line Mode 1 | 2 | 3",TYPE_UINT16},
        {"cpok",&g_GattMem[MEM_ADDR_CPOK],"Constant Power OK Flag",TYPE_UINT16},
        {"p1de",&g_GattMem[MEM_ADDR_P1DE],"PV1 Energy Today (24hrs)",TYPE_UINT32},
        {"p1te",&g_GattMem[MEM_ADDR_P1TE],"PV1 Energy Total",TYPE_UINT32},
        {"p2de",&g_GattMem[MEM_ADDR_P2DE],"PV2 Energy Today (24hrs)",TYPE_UINT32},
        {"p2te",&g_GattMem[MEM_ADDR_P2TE],"PV2 Energy Total",TYPE_UINT32},
        {"abcc",&g_GattMem[MEM_ADDR_ABCC],"AC Charge Battery Current",TYPE_UINT16},
        {"acdp",&g_GattMem[MEM_ADDR_ACDP],"AC Discharge Power",TYPE_UINT16},
        {"adva",&g_GattMem[MEM_ADDR_ADVA],"AC Discharge Apparent Power",TYPE_UINT32},
        {"bdap",&g_GattMem[MEM_ADDR_BDAP],"Battery Discharge Active Power",TYPE_UINT32},
        {"bdva",&g_GattMem[MEM_ADDR_BDVA],"Battery Discharge Apparent Power",TYPE_UINT32},
        {"mcfs",&g_GattMem[MEM_ADDR_MCFS],"MPPT Charger Fan Speed %",TYPE_UINT16},
        {"ivfs",&g_GattMem[MEM_ADDR_IVFS],"MPPT Charger Fan Speed %",TYPE_UINT16},
        {"ccrt",&g_GattMem[MEM_ADDR_CCRT],"Total Charge Current",TYPE_UINT16},
        {"dcct",&g_GattMem[MEM_ADDR_DCCT],"Total Discharge Current",TYPE_UINT16},
        {"dcde",&g_GattMem[MEM_ADDR_DCDE],"Discharged Energy Today (24hrs)",TYPE_UINT32},
        {"dcte",&g_GattMem[MEM_ADDR_DCTE],"Discharged Energy Total",TYPE_UINT32},
        {"pccr",&g_GattMem[MEM_ADDR_PCCR],"Parallel System Charge Current",TYPE_UINT16}

		}	
	#elif defined(E_MOB48V_PROJECT)		
	{	//dta
		//{"_meta":"Camp specific data available for clients. Read only",
		{"batp",&g_GattMem[MEM_ADDR_BATP],"Battery Net Charging Power W",TYPE_UINT16},
		{"inpp",&g_GattMem[MEM_ADDR_INPP],"Total Input Power W",TYPE_UINT16},
		{"outp",&g_GattMem[MEM_ADDR_OUTP],"Output Power W,",TYPE_UINT16},
		{"aeng",&g_GattMem[MEM_ADDR_AENG],"Accumulated Energy Output in Whs",TYPE_UINT16},
		{"pckv",&g_GattMem[MEM_ADDR_PCKV],"Pack Voltage mV",TYPE_UINT32},
		{"pckc",&g_GattMem[MEM_ADDR_PCKC],"Pack Current in mA",TYPE_INT},
		{"rsoc",&g_GattMem[MEM_ADDR_RSOC],"Relative State of Charge",TYPE_UINT16},
		{"rcap",&g_GattMem[MEM_ADDR_RCAP],"Remaining Capacity in Whs",TYPE_UINT16},
		{"fccp",&g_GattMem[MEM_ADDR_FCCP],"Full Charge Capacity in Whs",TYPE_UINT16},
		{"pckt",&g_GattMem[MEM_ADDR_PCKT],"Pack Temperature in Celsius",TYPE_INT},
		
		{"slon",&g_GattMem[MEM_ADDR_SLON],"Satellite Longitude in DD (decimal degrees)",TYPE_FLOAT_STR},
		{"slat",&g_GattMem[MEM_ADDR_SLAT],"Satellite Latitude in DD (decimal degrees)",TYPE_FLOAT_STR},
		{"salt",&g_GattMem[MEM_ADDR_SALT],"Satellite Altitude in Metres",TYPE_UINT16},
		{"sstm",&g_GattMem[MEM_ADDR_SSTM],"Satellite-Synchronized Network Clock time in ISO 8601",TYPE_STRS},
		{"sgps",&g_GattMem[MEM_ADDR_SGPS],"Satellite GPS Strength",TYPE_UINT16},
		{"sspe",&g_GattMem[MEM_ADDR_SSPE],"Satellite Speed in kM/h",TYPE_UINT16},
		
		},
		
	{	//dia
		//{"_meta":"Oasis-1K specific diagnostics available service personnel. Read only",
		{"cv01",&g_GattMem[MEM_ADDR_CV01],"Cell 01 Voltage mV",TYPE_UINT16},
		{"cv02",&g_GattMem[MEM_ADDR_CV02],"Cell 02 Voltage mV",TYPE_UINT16},
		{"cv03",&g_GattMem[MEM_ADDR_CV03],"Cell 03 Voltage mV",TYPE_UINT16},
		{"cv04",&g_GattMem[MEM_ADDR_CV04],"Cell 04 Voltage mV",TYPE_UINT16},
		{"cv05",&g_GattMem[MEM_ADDR_CV05],"Cell 05 Voltage mV",TYPE_UINT16},
		{"cv06",&g_GattMem[MEM_ADDR_CV06],"Cell 06 Voltage mV",TYPE_UINT16},
		{"cv07",&g_GattMem[MEM_ADDR_CV07],"Cell 07 Voltage mV",TYPE_UINT16},
		{"cv08",&g_GattMem[MEM_ADDR_CV08],"Cell 08 Voltage mV",TYPE_UINT16},
		{"cv09",&g_GattMem[MEM_ADDR_CV09],"Cell 09 Voltage mV",TYPE_UINT16},
		{"cv10",&g_GattMem[MEM_ADDR_CV10],"Cell 10 Voltage mV",TYPE_UINT16},
		{"cv11",&g_GattMem[MEM_ADDR_CV11],"Cell 11 Voltage mV",TYPE_UINT16},
		{"cv12",&g_GattMem[MEM_ADDR_CV12],"Cell 12 Voltage mV",TYPE_UINT16},
		{"cv13",&g_GattMem[MEM_ADDR_CV13],"Cell 13 Voltage mV",TYPE_UINT16},
		{"cv14",&g_GattMem[MEM_ADDR_CV14],"Cell 14 Voltage mV",TYPE_UINT16},
		{"cv15",&g_GattMem[MEM_ADDR_CV15],"Cell 15 Voltage mV",TYPE_UINT16},
		{"cv16",&g_GattMem[MEM_ADDR_CV16],"Cell 16 Voltage mV",TYPE_UINT16}
		#ifdef BMS_JBD_SUPPROT
		,{"cv17",&g_GattMem[MEM_ADDR_CV17],"Cell 17 Voltage mV",TYPE_UINT16}
		,{"cv18",&g_GattMem[MEM_ADDR_CV18],"Cell 18 Voltage mV",TYPE_UINT16}
		,{"cv19",&g_GattMem[MEM_ADDR_CV19],"Cell 19 Voltage mV",TYPE_UINT16}
		,{"cv20",&g_GattMem[MEM_ADDR_CV20],"Cell 20 Voltage mV",TYPE_UINT16}
		#endif

		}	
	#elif defined(DC_PUMP_SUPPORT)	
	{
		{"mfrq",&g_GattMem[MEM_ADDR_MFRQ],"motor frequency",TYPE_UINT16},
		//{"busv",&g_GattMem[MEM_ADDR_BUSV],"bus voltage in volts",TYPE_UINT16},
		{"outv",&g_GattMem[MEM_ADDR_OUTV],"output voltage in volts",TYPE_UINT16},
		{"outc",&g_GattMem[MEM_ADDR_OUTC],"output current in amps",TYPE_UINT16},
		{"outp",&g_GattMem[MEM_ADDR_OUTP],"output power in watt",TYPE_UINT16},
		//{"mtrq",&g_GattMem[MEM_ADDR_MTRQ],"motor torque in newton-meters",TYPE_UINT16},
		{"mspd",&g_GattMem[MEM_ADDR_MSPD],"motor speed in rpms",TYPE_UINT16},
		//{"awcm",&g_GattMem[MEM_ADDR_AWCM],"Accumulated water output in cubic meters",TYPE_UINT16},
		{"tnkf",&g_GattMem[MEM_ADDR_TNKF],"Tank Water Level Full. 0=no, 1=yes",TYPE_UINT16},
		{"wlle",&g_GattMem[MEM_ADDR_WLLE],"Well Water Level Exhausted. 0=no, 1=yes",TYPE_UINT16},
		{"spwr",&g_GattMem[MEM_ADDR_SPWR],"Solar Power Input Available. 0=no, 1=yes",TYPE_UINT16},
		//{"syst",&g_GattMem[MEM_ADDR_SYST],"System temperature in Celsius",TYPE_UINT16}
		}
	#endif

};


uint8_t GattGetListProp(uint8_t dt_list,uint8_t id,uint8_t *prop)
{
	GATTPROP_Def *p=NULL;
	
	switch(dt_list)
	{
		case LIST_ATT:
			if(id<ATT_COUNT)
			{	memcpy(prop,g_gatt1_list.att[id].prop,4);
				return TRUE;
				}
			break;
		case LIST_CMD:
			if(id<CMD_COUNT)
			{	memcpy(prop,g_gatt1_list.cmd[id].prop,4);
				return TRUE;
				}
			break;
		case LIST_STS:	
			if(id<STS_COUNT)
			{	memcpy(prop,g_gatt1_list.sts[id].prop,4);
				return TRUE;
				}
		case LIST_DTA:	
			if(id<DTA_COUNT)
			{	memcpy(prop,g_gatt1_list.dta[id].prop,4);
				return TRUE;
				}
			break;
		#ifndef DC_PUMP_SUPPORT
		case LIST_DIA:
			if(id<DIA_COUNT)
			{	memcpy(prop,g_gatt1_list.dia[id].prop,4);
				return TRUE;
				}
			break;
		#endif
		default:
			return FALSE;
	}

	return FALSE;

}


GATTPROP_Def *GattGetListInfor(uint8_t dt_list,uint8_t *count)
{
	GATTPROP_Def *p=NULL;
	
	switch(dt_list)
	{
		case LIST_ATT:
			*count=ATT_COUNT;
			p=(GATTPROP_Def*)g_gatt1_list.att;
			
			break;
		case LIST_CMD:
			*count=CMD_COUNT;
			p=(GATTPROP_Def*)g_gatt1_list.cmd;
			break;
		case LIST_STS:	
			*count=STS_COUNT;
			p=(GATTPROP_Def*)g_gatt1_list.sts;
			break;
		case LIST_DTA:	
			*count=DTA_COUNT;
			p=(GATTPROP_Def*)g_gatt1_list.dta;
			break;
		#ifndef DC_PUMP_SUPPORT
		case LIST_DIA:

			#ifdef BMS_JBD_14_SUPPORT
			*count=DIA_COUNT-6;
			#else
			*count=DIA_COUNT;
			#endif
		
			p=(GATTPROP_Def*)g_gatt1_list.dia;
			break;
		#endif
		default:
			return NULL;
	}

	return p;

}

uint8_t GattSetData(uint8_t list,uint8_t id,uint8_t*data)
{
	GATTPROP_Def *p=NULL;
	uint8_t count;

	p=GattGetListInfor(list,&count);
	
	if(p==NULL||id>=count)
		return FALSE;
	
	
	switch(p[id].data_type)
	{
		case TYPE_UINT16:
			memcpy(p[id].value,data,2);
			break;
		case TYPE_STRS:	
			memcpy(p[id].value,data,strlen((char*)data));
			break;
		case TYPE_INT:	
			memcpy(p[id].value,data,2);
			break;
	    case TYPE_FLOAT_STR:	
			memcpy(p[id].value,data,strlen((char*)data));
			break;	
		case TYPE_UINT32:	
			memcpy(p[id].value,data,4);
			break;
		 default:
			return FALSE;
	}
	return TRUE;
}


uint8_t GattGetData(uint8_t list,uint8_t id,uint8_t*data)
{
	GATTPROP_Def *p;
	uint8_t count;
	
	
	p=GattGetListInfor(list,&count);

	if(p==NULL||id>=count)
		return FALSE;
	
	switch(p[id].data_type)
	{
		case TYPE_UINT16:
			memcpy(data,p[id].value,2);
			break;
		case TYPE_STRS:	
			memcpy(data,p[id].value,strlen((char*)p[id].value));
			break;
		case TYPE_INT:	
			memcpy(data,p[id].value,2);
			break;
		case TYPE_FLOAT_STR:	
			memcpy(data,p[id].value,strlen((char*)p[id].value));
			break;
		case TYPE_UINT32:	
			memcpy(data,p[id].value,4);
			break;	
		default:
			return FALSE;
	}
	return TRUE;
}

GATT1_Def* GattGetList(void)
{
	return (GATT1_Def*)&g_gatt1_list;
}

const GATT_Def  g_gatt_list=
{
#if 0 //def PUMP_PROJECT
	/*3,
	{	
		{"paygProductID",g_gatt_value.opid,"OEM Product ID ",1,&g_gatt_meta_ctrl.opid},
		{"oemProductID",g_gatt_value.ppid,"PAYG Product ID ",1,&g_gatt_meta_ctrl.ppid},
		{"REV",g_gatt_value.rev," Firmware Version Number",1,&g_gatt_meta_ctrl.rev}
		},
	8,
	{	
		{"paygS",&g_gatt_value.rcrd,"  ",0,&g_gatt_meta_ctrl.rcrd},
		{"outputS",&g_gatt_value.trhd,"   ",0,&g_gatt_meta_ctrl.trhd},
		{"errCode",&g_gatt_value.tpgd,"   ",0,&g_gatt_meta_ctrl.tpgd},
		{"hashT",&g_gatt_value.pgst,"   ",0,&g_gatt_meta_ctrl.pgst},
		{"runDaysBP",&g_gatt_value.ocst,"   ",0,&g_gatt_meta_ctrl.ocst},
		{"remDays",&g_gatt_value.sstc,"   ",0,&g_gatt_meta_ctrl.sstc},
		{"runDays",&g_gatt_value.rsoc,"  ",0,&g_gatt_meta_ctrl.rsoc},
		{"payDays",&g_gatt_value.rcap,"   ",0,&g_gatt_meta_ctrl.rcap},
		},
	8,
	{	
		{"mFrq",&g_gatt_value.batp,"  ",0,&g_gatt_meta_ctrl.batp},
		{"busV",&g_gatt_value.inpp,"  ",0,&g_gatt_meta_ctrl.inpp},
		{"outV",&g_gatt_value.outp,"  ",0,&g_gatt_meta_ctrl.outp},
		{"outC",&g_gatt_value.aeng,"  ",0,&g_gatt_meta_ctrl.aeng},
		{"outP",&g_gatt_value.cv01,"  ",0,&g_gatt_meta_ctrl.cv01},
		{"mTrq",&g_gatt_value.cv02,"  ",0,&g_gatt_meta_ctrl.cv02},
		{"mSpd",&g_gatt_value.cv03,"  ",0,&g_gatt_meta_ctrl.cv03},
		{"runS",&g_gatt_value.cv04,"  ",0,&g_gatt_meta_ctrl.cv04},

		}*/
#else
	3,
	{	
		{"opid",(char*)g_gatt_value.opid,"OEM Product ID ",1,&g_gatt_meta_ctrl.opid},
		{"ppid",(char*)g_gatt_value.ppid,"PAYG Product ID ",1,&g_gatt_meta_ctrl.ppid},
		{"frmv",(char*)g_gatt_value.rev," Firmware Version Number",1,&g_gatt_meta_ctrl.rev}
		},
	#ifdef PUMP_PROJECT
	8,
	{	
		{"rcrd",&g_gatt_value.rcrd," Remaining Credit Days  This is a credit count in days.",0,&g_gatt_meta_ctrl.rcrd},
		{"trhd",&g_gatt_value.trhd," Total Run History Days",0,&g_gatt_meta_ctrl.trhd},
		{"tpgd",&g_gatt_value.tpgd," Total PAYG Days",0,&g_gatt_meta_ctrl.tpgd},
		{"pgst",&g_gatt_value.pgst," PAYG State",0,&g_gatt_meta_ctrl.pgst},
		{"ocst",&g_gatt_value.ocst," Output Control State",0,&g_gatt_meta_ctrl.ocst},
		{"sstc",&g_gatt_value.sstc," System Status Code",0,&g_gatt_meta_ctrl.sstc},
		{"rdbk",&g_gatt_value.rdbk," Run Days Backup",0,&g_gatt_meta_ctrl.rdbk},
		{"pubk",(uint16_t*)g_gatt_value.pubk," Public Key Hash Top",1,&g_gatt_meta_ctrl.pubk}
		},
	13,
	{	
		{"mfrq",&g_gatt_value.mfrq,"Inverter Motor Frequency Setting",0,&g_gatt_meta_ctrl.mfrq},
		{"outp",&g_gatt_value.outp,"Output Power to Loads in watts",0,&g_gatt_meta_ctrl.outp},
		{"outv",&g_gatt_value.outv,"Inverter Output Voltage in V",0,&g_gatt_meta_ctrl.outv},
		{"inpv",&g_gatt_value.inpv,"Inverter Inout Voltage in V",0,&g_gatt_meta_ctrl.inpv},
		{"outc",&g_gatt_value.outc,"Inverter Output Current in Amps",0,&g_gatt_meta_ctrl.outc},
		{"mtrq",&g_gatt_value.mtrq,"Motor Torque = Winding Current",0,&g_gatt_meta_ctrl.mtrq},
		{"mspd",&g_gatt_value.mspd,"Motor Speed Feedback in RPM",0,&g_gatt_meta_ctrl.mspd},
		{"mtmp",&g_gatt_value.mtmp,"Motor Temperature in Celsius",0,&g_gatt_meta_ctrl.mtmp},
		{"runs",&g_gatt_value.runs,"Running Status",0,&g_gatt_meta_ctrl.runs},
		{"pctk",&g_gatt_value.pckt,"Battery Pack Temperature in celcius",0,&g_gatt_meta_ctrl.pckt},
		{"plat",(uint16_t*)g_gatt_value.plat,"GPS Position Latitude",1,&g_gatt_meta_ctrl.plat},
		{"plon",(uint16_t*)g_gatt_value.plon,"GPS Position Longitude",1,&g_gatt_meta_ctrl.plon},
		//{"time",(uint16_t*)g_gatt_value.time,"Device On-Board Timer",1,&g_gatt_meta_ctrl.time},
		{"gpss",&g_gatt_value.gpss,"Device GPS Signal Strength in db",0,&g_gatt_meta_ctrl.gpss},
		{"gpfc",&g_gatt_value.gpfc,"GPRS Connect Failure Counts",0,&g_gatt_meta_ctrl.gpfc}

		}	
	#else
	16,
	{	
		{"rcrd",&g_gatt_value.rcrd," Remaining Credit Days  This is a credit count in days.",0,&g_gatt_meta_ctrl.rcrd},
		{"trhd",&g_gatt_value.trhd," Total Run History Days",0,&g_gatt_meta_ctrl.trhd},
		{"tpgd",&g_gatt_value.tpgd," Total PAYG Days",0,&g_gatt_meta_ctrl.tpgd},
		{"pgst",&g_gatt_value.pgst," PAYG State",0,&g_gatt_meta_ctrl.pgst},
		{"ocst",&g_gatt_value.ocst," Output Control State",0,&g_gatt_meta_ctrl.ocst},
		{"sstc",&g_gatt_value.sstc," System Status Code",0,&g_gatt_meta_ctrl.sstc},
		{"rsoc",&g_gatt_value.rsoc,"Relative State of Charge",0,&g_gatt_meta_ctrl.rsoc},
		{"rcap",&g_gatt_value.rcap," Remaining Cpacity in Whs",0,&g_gatt_meta_ctrl.rcap},
		{"fccp",&g_gatt_value.fccp," Full Charge Capacity in Whs",0,&g_gatt_meta_ctrl.fccp},
		{"rdbk",&g_gatt_value.rdbk," Run Days Backup",0,&g_gatt_meta_ctrl.rdbk},
		{"acyc",&g_gatt_value.acyc," Accumulated Cycles",0,&g_gatt_meta_ctrl.acyc},
		{"pubk",(uint16_t*)g_gatt_value.pubk," Public Key Hash Top",1,&g_gatt_meta_ctrl.pubk},
		{"gstw",&g_gatt_value.gstw," GPRS Sleep Time Window in mins",0,&g_gatt_meta_ctrl.gstw},
		{"gctw",&g_gatt_value.gctw," GPRS Connect Time Window in mins",0,&g_gatt_meta_ctrl.gctw},
		{"hbfq",&g_gatt_value.hbfq,"Heart Beat Interval in Minutes",0,&g_gatt_meta_ctrl.hbfq},
		{"rptm",&g_gatt_value.rptm,"Report mode Setting",0,&g_gatt_meta_ctrl.rptm}		
		},

	15,
	{	
		{"batp",&g_gatt_value.batp,"Battery Net Charging power in watts",0,&g_gatt_meta_ctrl.batp},
		{"inpp",&g_gatt_value.inpp,"Total Input Power in watts",0,&g_gatt_meta_ctrl.inpp},
		{"outp",&g_gatt_value.outp,"Output Power to Loads in watts",0,&g_gatt_meta_ctrl.outp},
		{"aeng",&g_gatt_value.aeng,"Total Accumulated Energy Output in Whs",0,&g_gatt_meta_ctrl.aeng},
		{"cv01",&g_gatt_value.cv01,"Voltage in mV of cell 01",0,&g_gatt_meta_ctrl.cv01},
		{"cv02",&g_gatt_value.cv02,"Voltage in mV of cell 02",0,&g_gatt_meta_ctrl.cv02},
		{"cv03",&g_gatt_value.cv03,"Voltage in mV of cell 03",0,&g_gatt_meta_ctrl.cv03},
		{"cv04",&g_gatt_value.cv04,"Voltage in mV of cell 04",0,&g_gatt_meta_ctrl.cv04},
		{"pckv",&g_gatt_value.pckv,"Battery Pack Voltage in mV",0,&g_gatt_meta_ctrl.pckv},
		{"pckc",&g_gatt_value.pckc,"Battery Pack Current in mA",2,&g_gatt_meta_ctrl.pckc},
		{"pckt",&g_gatt_value.pckt,"Battery Pack Temperature in celcius",0,&g_gatt_meta_ctrl.pckt},
		{"plat",(uint16_t*)g_gatt_value.plat,"GPS Position Latitude",1,&g_gatt_meta_ctrl.plat},
		{"plon",(uint16_t*)g_gatt_value.plon,"GPS Position Longitude",1,&g_gatt_meta_ctrl.plon},
		//{"time",(uint16_t*)g_gatt_value.time,"Device On-Board Timer",1,&g_gatt_meta_ctrl.time},
		{"gpss",&g_gatt_value.gpss,"Device GPS Signal Strength in db",0,&g_gatt_meta_ctrl.gpss},
		{"gpfc",&g_gatt_value.gpfc,"GPRS Connect Failure Counts",0,&g_gatt_meta_ctrl.gpfc}

		}
	#endif
	#endif
};

void GattInit(void)
{
   memset((uint8_t*)&g_gatt_value,0x00,sizeof(GATT_PARA_Def));
   memset((uint8_t*)&g_gatt_meta_ctrl,0x00,sizeof(GATT_META_Def));
   
   #ifdef NEW_GATTVERSION
   memset(g_GattMem,0x00,MEM_GATT_SIZE);
   #endif

   GattSetGpsCordLon("0.00");
   GattSetGpsCordLat("0.00");
   #ifdef E_MOB48V_PROJECT
   GattSetData( LIST_DTA, DTA_SLAT,"0.00");
   GattSetData( LIST_DTA, DTA_SLON,"0.00");
	GattSetData( LIST_DTA, DTA_SSTM," ");
   #endif
  
}


#ifndef NEW_GATTVERSION

uint8_t* GattAllFieldJsonMerge(void)
{
	uint8_t i,*json;

	memset(g_pub_json,0x00,JSON_LEN);

	json=g_pub_json;

	/*#ifdef PUMP_PROJECT
	memcpy(json,"{\"att\":{",10);
	#else*/
	memcpy(json,"{\"att\":{",8);
	//#endif

	GattSetGprsConnectFailCnt(g_UserSet.report_fail_cnt);
	GattSetGprsConnectTime(g_UserSet.onlinetime);
   	GattSetGprsSleepTime(g_UserSet.sleeptime);
	
	//json+=7;
	json+=strlen(json);
	//att
	
	for(i=0;i<g_gatt_list.num_att;i++)
	{
		if(*g_gatt_list.att[i].meta_ctrl)
			sprintf(json,"\"%s\":\"%s\",",g_gatt_list.att[i].prop,g_gatt_list.att[i].meta);
		else
		{
			if(g_gatt_list.att[i].val_type)
				sprintf(json,"\"%s\":\"%s\",",g_gatt_list.att[i].prop,g_gatt_list.att[i].value);
			else
				sprintf(json,"\"%s\":%d,",g_gatt_list.att[i].prop,*g_gatt_list.att[i].value);
			}

		json+=strlen(json);
		}

	json-=1;
	//sts
	/*#ifdef PUMP_PROJECT
	memcpy(json,"},\"sts\":{",12);
	#else*/
	memcpy(json,"},\"sts\":{",9);
	//#endif

	json+=strlen(json);

	for(i=0;i<g_gatt_list.num_sts;i++)
	{

		if(*g_gatt_list.sts[i].meta_ctrl)
			sprintf(json,"\"%s\":\"%s\",",g_gatt_list.sts[i].prop,g_gatt_list.sts[i].meta);
		else
		{
			if(g_gatt_list.sts[i].val_type)
				sprintf(json,"\"%s\":\"%s\",",g_gatt_list.sts[i].prop,(uint8_t*)g_gatt_list.sts[i].value);
			else
				sprintf(json,"\"%s\":%d,",g_gatt_list.sts[i].prop,*g_gatt_list.sts[i].value);
			}

		json+=strlen(json);
		}
        json-=1;

       //sta
      /* #ifdef PUMP_PROJECT
	   memcpy(json,"},\"sta\":{",12);
	#else */
	memcpy(json,"},\"sta\":{",9);
	//#endif
        json+=strlen(json);
	
	for(i=0;i<g_gatt_list.num_sta;i++)
	{

		if(*g_gatt_list.sta[i].meta_ctrl)
			sprintf(json,"\"%s\":\"%s\",",g_gatt_list.sta[i].prop,g_gatt_list.sta[i].meta);
		else
		{
			if(g_gatt_list.sta[i].val_type==2)
				sprintf(json,"\"%s\":%d,",g_gatt_list.sta[i].prop,(int16_t)(*g_gatt_list.sta[i].value));
			else if(g_gatt_list.sta[i].val_type)
				sprintf(json,"\"%s\":%s,",g_gatt_list.sta[i].prop,g_gatt_list.sta[i].value);
			else
				sprintf(json,"\"%s\":%d,",g_gatt_list.sta[i].prop,*g_gatt_list.sta[i].value);
			}
			
		 json+=strlen(json);
		}
	
	json-=1;

	memcpy(json,"}}",2);

	// json+=strlen(json);

	AtSetTopicId(NULL);

	return g_pub_json;
}


uint8_t* GattDtTypeFieldJsonMerge(uint8_t dt_type,uint8_t meta)
{
	uint8_t i,*json;

	uint8_t topic_ext[20];

	memset(g_pub_json,0x00,JSON_LEN);
	memset(topic_ext,0x00,20);

	json=g_pub_json;

	//memcpy(json,"{",1);

	GattSetGprsConnectFailCnt(g_UserSet.report_fail_cnt);
	GattSetGprsConnectTime(g_UserSet.onlinetime);
   	GattSetGprsSleepTime(g_UserSet.sleeptime);

	//json+=7;
	json+=strlen(json);

	switch(dt_type)
	{
		case DT_TYPE_ATT:

			memcpy(topic_ext,"/att",4);
			//AtSetTopicId(topic_ext);

			memcpy(json,"{\"att\":{",8);
			
			json+=strlen(json);
			
			for(i=0;i<g_gatt_list.num_att;i++)
			{
				if(meta)
				{	sprintf(json,"\"%s\":\"%s\",",g_gatt_list.att[i].prop,g_gatt_list.att[i].meta);
					memcpy(topic_ext,"/meta",5);
					AtSetTopicId(topic_ext);
					}
				else
				{
					if(g_gatt_list.att[i].val_type)
						sprintf(json,"\"%s\":\"%s\",",g_gatt_list.att[i].prop,g_gatt_list.att[i].value);
					else
						sprintf(json,"\"%s\":%d,",g_gatt_list.att[i].prop,*g_gatt_list.att[i].value);

					AtSetTopicId(NULL);
					}

				json+=strlen(json);
				
				}
			json-=1;
			break;
		case DT_TYPE_STS:
				//memcpy(topic_ext,"/sts",4);
				//AtSetTopicId(topic_ext);

				memcpy(json,"{\"sts\":{",8);
			
				json+=strlen(json);

				for(i=0;i<g_gatt_list.num_sts;i++)
				{

					if(meta)
					{	sprintf(json,"\"%s\":\"%s\",",g_gatt_list.sts[i].prop,g_gatt_list.sts[i].meta);
						memcpy(topic_ext,"/meta",5);
						AtSetTopicId(topic_ext);
						}
					else
					{
						if(g_gatt_list.sts[i].val_type)
							sprintf(json,"\"%s\":\"%s\",",g_gatt_list.sts[i].prop,(uint8_t*)g_gatt_list.sts[i].value);
						else
							sprintf(json,"\"%s\":%d,",g_gatt_list.sts[i].prop,*g_gatt_list.sts[i].value);
						AtSetTopicId(NULL);	
						}

					json+=strlen(json);
					}
			        json-=1;
			break;
		case DT_TYPE_STA:
		default:	
				//memcpy(topic_ext,"/sta",4);
				//AtSetTopicId(topic_ext);

				memcpy(json,"{\"sta\":{",8);
			
				json+=strlen(json);
				
				for(i=0;i<g_gatt_list.num_sta;i++)
				{

					if(meta)
					{	sprintf(json,"\"%s\":\"%s\",",g_gatt_list.sta[i].prop,g_gatt_list.sta[i].meta);
						memcpy(topic_ext,"/meta",5);
						AtSetTopicId(topic_ext);
						}
					else
					{
						if(g_gatt_list.sta[i].val_type==2)
							sprintf(json,"\"%s\":\"%d\",",g_gatt_list.sta[i].prop,(int16_t)(*g_gatt_list.sta[i].value));
						else if(g_gatt_list.sta[i].val_type)
							sprintf(json,"\"%s\":%s,",g_gatt_list.sta[i].prop,g_gatt_list.sta[i].value);
						else
							sprintf(json,"\"%s\":%d,",g_gatt_list.sta[i].prop,*g_gatt_list.sta[i].value);

						AtSetTopicId(NULL);
						}
						
					 json+=strlen(json);
					}
				
					json-=1;
			break;
		
		}

	memcpy(json,"}}",2);

	return g_pub_json;
}

uint8_t* GattSingleFieldMerge(uint8_t *tag_str)
{
	uint8_t i,*json;
	uint8_t topic_ext[20];

	memset(g_pub_json,0x00,JSON_LEN);
	memset(topic_ext,0x00,20);

	GattSetGprsConnectFailCnt(g_UserSet.report_fail_cnt);
	GattSetGprsConnectTime(g_UserSet.onlinetime);
   	GattSetGprsSleepTime(g_UserSet.sleeptime);

	json=g_pub_json;

	for(i=0;i<g_gatt_list.num_att;i++)
	{
		if(strstr(tag_str,g_gatt_list.att[i].prop)!=NULL)
		{
			memcpy(json,"{\"att\":{",8);

			json+=strlen(json);
			
			if(strstr(tag_str,"meta")!=NULL)
			{	
				sprintf(json,"\"%s\":\"%s\"",g_gatt_list.att[i].prop,g_gatt_list.att[i].meta);
				memcpy(topic_ext,"/meta",5);
				AtSetTopicId(topic_ext);
				}
			else
			{
				if(g_gatt_list.att[i].val_type)
					sprintf(json,"\"%s\":\"%s\"",g_gatt_list.att[i].prop,g_gatt_list.att[i].value);
				else
					sprintf(json,"\"%s\":%d",g_gatt_list.att[i].prop,*g_gatt_list.att[i].value);
				
				//sprintf(topic_ext,"/att/%s",g_gatt_list.att[i].prop);
				AtSetTopicId(NULL);
			}

			json+=strlen(json);

			memcpy(json,"}}",2);

			//AtSetTopicId(NULL);

			return g_pub_json;
		}

	}


	for(i=0;i<g_gatt_list.num_sts;i++)
	{
		if(strstr(tag_str,g_gatt_list.sts[i].prop)!=NULL)
		{
			memcpy(json,"{\"sts\":{",8);

			json+=strlen(json);
			
			if(strstr(tag_str,"meta")!=NULL)
			{	
				sprintf(json,"\"%s\":\"%s\"",g_gatt_list.sts[i].prop,g_gatt_list.sts[i].meta);
				
				memcpy(topic_ext,"/meta",5);
				AtSetTopicId(topic_ext);
				}
			else
			{
				if(g_gatt_list.sts[i].val_type)
					sprintf(json,"\"%s\":\"%s\"",g_gatt_list.sts[i].prop,g_gatt_list.sts[i].value);
				else
					sprintf(json,"\"%s\":%d",g_gatt_list.sts[i].prop,*g_gatt_list.sts[i].value);

				//sprintf(topic_ext,"/sts/%s",g_gatt_list.sts[i].prop);
				AtSetTopicId(NULL);
			}

			//AtSetTopicId(topic_ext);

			json+=strlen(json);

			memcpy(json,"}}",2);

			return g_pub_json;
		}

	}


	for(i=0;i<g_gatt_list.num_sta;i++)
	{

		if(strstr(tag_str,g_gatt_list.sta[i].prop)!=NULL)
		{
			memcpy(json,"{\"sta\":{",8);
			json+=strlen(json);
			
			if(strstr(tag_str,"meta")!=NULL)
			{	
				sprintf(json,"\"%s\":\"%s\"",g_gatt_list.sta[i].prop,g_gatt_list.sta[i].meta);
				memcpy(topic_ext,"/meta",5);
				AtSetTopicId(topic_ext);
				}
			else
			{
				if(g_gatt_list.sta[i].val_type==2)
					sprintf(json,"\"%s\":%d",g_gatt_list.sta[i].prop,(int16_t)(*g_gatt_list.sta[i].value));
				else if(g_gatt_list.sta[i].val_type)
					sprintf(json,"\"%s\":%s",g_gatt_list.sta[i].prop,g_gatt_list.sta[i].value);
				else
					sprintf(json,"\"%s\":%d",g_gatt_list.sta[i].prop,*g_gatt_list.sta[i].value);

				//sprintf(topic_ext,"/sta/%s",g_gatt_list.sta[i].prop);
				AtSetTopicId(NULL);
			}

			

			json+=strlen(json);

			memcpy(json,"}}",2);

			return g_pub_json;
		}

	}

	return g_pub_json;
}

#else

uint8_t* GattAllFieldJsonMerge(void)
{
	uint8_t i,j,*json,count;
	GATTPROP_Def *p;

	memset(g_pub_json,0x00,JSON_LEN);

	json=g_pub_json;

	GattSetGprsConnectFailCnt(g_UserSet.report_fail_cnt);
	GattSetGprsConnectTime(g_UserSet.onlinetime);
   	GattSetGprsSleepTime(g_UserSet.sleeptime);
	
	for(i=0;i<LIST_COUNT;i++)
	{
		p=GattGetListInfor(i,&count);

		if(p==NULL)
			continue;	
		
		switch(i)
		{
			case LIST_ATT:
				memcpy(json,"{\"att\":{",8);
				//json+=strlen(json);
				break;
			case LIST_CMD:	
				json-=1;
				//cmd
				memcpy(json,"},\"cmd\":{",9);
				//json+=strlen(json);
				break;
			case LIST_STS:	
			    json-=1;
				//cmd
				memcpy(json,"},\"sts\":{",9);
				//json+=strlen(json);
				break;
			case LIST_DTA:	
			    json-=1;
				//cmd
				memcpy(json,"},\"dta\":{",9);
				//json+=strlen(json);
				break;
			case LIST_DIA:
			    json-=1;
				//cmd
				memcpy(json,"},\"dia\":{",9);
				//json+=strlen(json);
				break;
			}
		
		json+=strlen((char*)json);
	
		for(j=0;j<count;j++)
		{
				/*if(*g_gatt_list.att[i].meta_ctrl)
					sprintf(json,"\"%s\":\"%s\",",g_gatt_list.att[i].prop,g_gatt_list.att[i].meta);
				else*/
				{
					if(p[j].data_type==TYPE_INT)
						sprintf((char*)json,"\"%s\":%d,",p[j].prop,*((int16_t*)p[j].value));
					else if(p[j].data_type==TYPE_STRS)
						sprintf((char*)json,"\"%s\":\"%s\",",p[j].prop,p[j].value);
					else if(p[j].data_type==TYPE_FLOAT_STR)
						sprintf((char*)json,"\"%s\":%s,",p[j].prop,p[j].value);
					else if(p[j].data_type==TYPE_UINT32)
						sprintf((char*)json,"\"%s\":%d,",p[j].prop,(*(uint32_t*)p[j].value));
					else
						sprintf((char*)json,"\"%s\":%d,",p[j].prop,(*(uint16_t*)p[j].value));
					}

				json+=strlen((char*)json);
			}

		}
	
	json-=1;
	memcpy(json,"}}",2);

	AtSetTopicId(NULL);

	return g_pub_json;
}





uint8_t* GattDtTypeFieldJsonMerge(uint8_t dt_type,uint8_t meta)
{
	uint8_t i,*json;

	uint8_t topic_ext[20];
	GATTPROP_Def *p=NULL;
	uint8_t count=0;

	memset(g_pub_json,0x00,JSON_LEN);
	memset(topic_ext,0x00,20);

	json=g_pub_json;

	//memcpy(json,"{",1);

	GattSetGprsConnectFailCnt(g_UserSet.report_fail_cnt);
	GattSetGprsConnectTime(g_UserSet.onlinetime);
   	GattSetGprsSleepTime(g_UserSet.sleeptime);

	//json+=7;
	json+=strlen((char*)json);

	p=GattGetListInfor(dt_type,&count);

	if(p==NULL)
		return g_pub_json;	
	
	switch(dt_type)
	{
		case LIST_ATT:
		   // memcpy(topic_ext,"/att",4);
		   if(meta)
		   		memcpy(json,"{\"_att\":{",9);
		   	else
				memcpy(json,"{\"att\":{",8);
			//json+=strlen(json);
			break;
		case LIST_CMD:	
			//memcpy(topic_ext,"/cmd",4);
			 if(meta)
			 	memcpy(json,"{\"_cmd\":{",9);
			 else	
			      memcpy(json,"{\"cmd\":{",8);
			//json+=strlen(json);
			break;
		case LIST_STS:	
			//memcpy(topic_ext,"/sts",4);
			if(meta)
				memcpy(json,"{\"sts\":{",9);
			else
				memcpy(json,"{\"sts\":{",8);
			//json+=strlen(json);
			break;
		case LIST_DTA:	
			///memcpy(topic_ext,"/dta",4);
			if(meta)
				memcpy(json,"{\"_dta\":{",9);
			else
				memcpy(json,"{\"dta\":{",8);
			//json+=strlen(json);
			break;
		case LIST_DIA:
			//memcpy(topic_ext,"/dia",4);
			if(meta)
				memcpy(json,"{\"_dia\":{",9);
			else	
				memcpy(json,"{\"dia\":{",8);
			//json+=strlen(json);
			break;
		}

		json+=strlen((char*)json);
		
		for(i=0;i<count;i++)
		{
			if(meta)
			{	sprintf((char*)json,"\"_%s\":\"%s\",",p[i].prop,p[i].meta);
				//memcpy(topic_ext,"/meta",5);
				AtSetTopicId(topic_ext);
				}
			else
			{
				if(p[i].data_type==TYPE_INT)
						sprintf((char*)json,"\"%s\":%d,",p[i].prop,*((int16_t*)p[i].value));
				else if(p[i].data_type==TYPE_STRS)
					sprintf((char*)json,"\"%s\":\"%s\",",p[i].prop,p[i].value);
				else if(p[i].data_type==TYPE_FLOAT_STR)
					sprintf((char*)json,"\"%s\":%s,",p[i].prop,p[i].value);
				else if(p[i].data_type==TYPE_UINT32)
					sprintf((char*)json,"\"%s\":%d,",p[i].prop,(*(uint32_t*)p[i].value));
				else
					sprintf((char*)json,"\"%s\":%d,",p[i].prop,*((uint16_t*)p[i].value));

				AtSetTopicId(NULL);
				}

			json+=strlen((char*)json);
			
			}

		json-=1;


	memcpy(json,"}}",2);

	return g_pub_json;
}




uint8_t* GattSingleFieldMerge(uint8_t *tag_str)
{
	uint8_t i,j,*json;
	uint8_t topic_ext[20];
	GATTPROP_Def *p=NULL;
	uint8_t count=0,meta=0;

	memset(g_pub_json,0x00,JSON_LEN);
	memset(topic_ext,0x00,20);

	GattSetGprsConnectFailCnt(g_UserSet.report_fail_cnt);
	GattSetGprsConnectTime(g_UserSet.onlinetime);
   	GattSetGprsSleepTime(g_UserSet.sleeptime);

	json=g_pub_json;

	

	for(j=0;j<LIST_COUNT;j++)
	{

		p=GattGetListInfor(j,&count);

		if(p==NULL)
			continue;
		

		for(i=0;i<count;i++)
		{
			if(strstr((char*)tag_str,p[i].prop)!=NULL)
			{
					if(strstr((char*)tag_str,"meta")!=NULL)
						meta=TRUE;
					
					switch(j)
					{
						case LIST_ATT:
							if(meta)
								memcpy(json,"{\"_att\":{",9);
							else
								memcpy(json,"{\"att\":{",8);
							break;
						case LIST_CMD:	
							if(meta)
								memcpy(json,"{\"_cmd\":{",9);
							else
								memcpy(json,"{\"cmd\":{",8);
							break;
						case LIST_STS:	
							if(meta)
								memcpy(json,"{\"_sts\":{",9);
							else
								memcpy(json,"{\"sts\":{",8);
							break;
						case LIST_DTA:	
							if(meta)
								memcpy(json,"{\"_dta\":{",9);
							else
								memcpy(json,"{\"dta\":{",8);
							break;
						case LIST_DIA:
							if(meta)
								memcpy(json,"{\"_dia\":{",9);
							else
								memcpy(json,"{\"dia\":{",8);
							break;
						}
					
				json+=strlen((char*)json);
				
				if(meta)
				{	
					sprintf((char*)json,"\"_%s\":\"%s\"",p[i].prop,p[i].meta);
					//memcpy(topic_ext,"/meta",5);
					AtSetTopicId(topic_ext);
					}
				else
				{
					if(p[i].data_type==TYPE_INT)
						sprintf((char*)json,"\"%s\":%d",p[i].prop,*((int16_t*)p[i].value));
					else if(p[i].data_type==TYPE_STRS)
						sprintf((char*)json,"\"%s\":\"%s\"",p[i].prop,p[i].value);
					else if(p[i].data_type==TYPE_FLOAT_STR)
						sprintf((char*)json,"\"%s\":%s",p[i].prop,p[i].value);
					else if(p[i].data_type==TYPE_UINT32)
						sprintf((char*)json,"\"%s\":%d",p[i].prop,(*(uint32_t*)p[i].value));
					else
						sprintf((char*)json,"\"%s\":%d",p[i].prop,*((uint16_t*)p[i].value));
					
					AtSetTopicId(NULL);
				}

				json+=strlen((char*)json);

				memcpy(json,"}}",2);

				//AtSetTopicId(NULL);

				return g_pub_json;
			}

		}

	}


	return g_pub_json;
}


uint8_t* GattMultiFieldMerge(void)
{
	uint8_t i,j,*json,m=0;
	uint8_t topic_ext[20];
	GATTPROP_Def *p=NULL;
	uint8_t count=0,meta=0,head=0;
	uint8_t tempjson[128];

	memset(g_pub_json,0x00,JSON_LEN);
	memset(topic_ext,0x00,20);

	GattSetGprsConnectFailCnt(g_UserSet.report_fail_cnt);
	GattSetGprsConnectTime(g_UserSet.onlinetime);
   	GattSetGprsSleepTime(g_UserSet.sleeptime);

	json=g_pub_json;

	if(g_UserSet.raml_num==0)
		return g_pub_json;

	
	memcpy(json,"{",1);
	json+=strlen((char*)json);


	for(j=0;j<LIST_COUNT;j++)
	{
		head=0;
		p=GattGetListInfor(j,&count);

		if(p==NULL)
			continue;

		if(m>=g_UserSet.raml_num)
			break;

		for(i=0;i<count;i++)
		{
			if(strstr((char*)g_UserSet.raml[m],(char*)p[i].prop)!=NULL)
			{
					switch(j)
					{
						case LIST_ATT:
							if(head==0)
								memcpy(json,"\"att\":{",7);
							head=TRUE;
							break;
						case LIST_CMD:	
							if(head==0)
								memcpy(json,"\"cmd\":{",7);
							head=TRUE;
							break;
						case LIST_STS:	
							if(head==0)
								memcpy(json,"\"sts\":{",7);
							head=TRUE;
							break;
						case LIST_DTA:	
							if(head==0)
								memcpy(json,"\"dta\":{",7);
							head=TRUE;
							break;
						case LIST_DIA:
							if(head==0)
								memcpy(json,"\"dia\":{",7);
							head=TRUE;
							break;
						}

				
					
				json+=strlen((char*)json);
					
				if(p[i].data_type==TYPE_INT)
					sprintf((char*)json,"\"%s\":%d,",p[i].prop,*((int16_t*)p[i].value));
				else if(p[i].data_type==TYPE_STRS)
					sprintf((char*)json,"\"%s\":\"%s\",",p[i].prop,p[i].value);
				else if(p[i].data_type==TYPE_FLOAT_STR)
					sprintf((char*)json,"\"%s\":%s,",p[i].prop,p[i].value);
				else if(p[i].data_type==TYPE_UINT32)
					sprintf((char*)json,"\"%s\":%d,",p[i].prop,(*(uint32_t*)p[i].value));
				else
					sprintf((char*)json,"\"%s\":%d,",p[i].prop,*((uint16_t*)p[i].value));


				m++;

				if(m>=g_UserSet.raml_num)
						break;	
					
				
			}


		}
		
		if(head)
		{	json+=strlen((char*)json)-1;
			memcpy(json,"},",2);
		    json+=strlen((char*)json);
			}

	}

	json+=strlen((char*)json)-1;
	memcpy(json,"}",1);

	AtSetTopicId(NULL);

	return g_pub_json;
}


#endif

void GattSetUplinkData(uint8_t *buf)
{
	memset(g_pub_json,0x00,JSON_LEN);

	if(strlen((char*)buf)<JSON_LEN)
		memcpy(g_pub_json,buf,strlen((char*)buf));
}
//new added  20211217
void GattSetCmdUplinkData(uint8_t *buf)
{
	uint8_t*json=g_pub_json;
	
	memset(g_pub_json,0x00,JSON_LEN);

	memcpy(json,"{\"cmd\":{",8);
	json+=strlen((char*)json);

	if(strlen((char*)buf)<JSON_LEN-10)
		memcpy(json,buf,strlen((char*)buf));

	json+=strlen((char*)json);

	memcpy(json,"}}",2);
}


uint8_t *GattGetJsonBuff(void)
{
	return g_pub_json;
}	
void GattGetPpid(uint8_t*ppid)
{
	//uint8_t *temp;

	GattGetData(LIST_ATT, ATT_PPID, ppid);
	
	//return g_gatt_value.ppid;
}	

void GattGetOpid(uint8_t*opid)
{
	GattGetData(LIST_ATT, ATT_OPID, opid);
	
	//return g_gatt_value.opid;
}	
	
	
void GattSetPpid(uint8_t*pid,uint8_t size)
{
	if(size<=32)
	{	memcpy(g_gatt_value.ppid,pid,size);
		GattSetData( LIST_ATT, ATT_PPID, g_gatt_value.ppid);
		}
}

void GattSetOpid(uint8_t*opid,uint8_t size)
{
	if(size<=20)
	{	memcpy(g_gatt_value.opid,opid,size);
		GattSetData( LIST_ATT, ATT_OPID, g_gatt_value.opid);
		}
}

void GattSetVersion(uint8_t*rev,uint8_t size)
{
	if(size<=8)
	{	memcpy(g_gatt_value.rev,rev,size);
	
		GattSetData( LIST_ATT, ATT_FRMV, g_gatt_value.rev);	
		}
}

void GattSetRmPayDays(uint16_t days)//Remaining_PAYG_Days
{
	g_gatt_value.rcrd=days;
	GattSetData( LIST_STS, STS_RCRD, (uint8_t*)&days);	
}

void GattSetRunDays(uint16_t days)//Days have been running
{
	g_gatt_value.trhd=days;
	GattSetData( LIST_STS, STS_TRHD, (uint8_t*)&days);
}

void GattSetPaygDays(uint16_t days)//PAYG_Days
{
	g_gatt_value.tpgd=days;

	GattSetData( LIST_STS, STS_TPGD, (uint8_t*)&days);
}

void GattSetPaygState(uint16_t state)//PAYG_State
{
	g_gatt_value.pgst=state;
	GattSetData( LIST_STS, STS_PGST, (uint8_t*)&state);
}

void GattSetOutputCtrlState(uint16_t state)//Output_Control_State
{
	g_gatt_value.ocst=state;
	GattSetData( LIST_STS, STS_OCST, (uint8_t*)&state);
}

void GattSetSysStatusCode(uint16_t state)//System_Status_Code
{
	g_gatt_value.sstc=state;
	GattSetData( LIST_STS, STS_SSTC, (uint8_t*)&state);
}

void GattSetRelativeSOC(uint16_t soc)//Relative_SOC
{
	g_gatt_value.rsoc=soc;
	#if defined(UI1K_V13_PROJECT)||defined(E_MOB48V_PROJECT)
	GattSetData( LIST_DTA, DTA_RSOC, (uint8_t*)&soc);
	#endif
}

uint16_t GattGetRelativeSOC(void)//Relative_SOC
{
	uint16_t temp;
	#if  defined(UI1K_V13_PROJECT)||defined(E_MOB48V_PROJECT)
	GattGetData( LIST_DTA, DTA_RSOC, (uint8_t*)&temp);
	#endif
	return temp;
	//return g_gatt_value.rsoc;
}

void GattSetRmCap(uint16_t cap)//Remaining_Capacity
{
	g_gatt_value.rcap=cap;
	#if  defined(UI1K_V13_PROJECT)||defined(E_MOB48V_PROJECT)
	GattSetData( LIST_DTA, DTA_RCAP, (uint8_t*)&cap);
	#endif
}

void GattSetFullChrgeCap(uint16_t cap)//Full_Charge_Capacity
{
	g_gatt_value.fccp=cap;
	#ifdef UI1K_V13_PROJECT
	GattSetData( LIST_DTA, DTA_FCCP, (uint8_t*)&cap);
	#endif
}

void GattSetRunDayBackup(uint16_t day)//Run_Days_Backup
{
	g_gatt_value.rdbk=day;
	//GattSetData( LIST_DTA, DTA_r, (uint16_t*)&day);
}

void GattSetAccuCyc(uint16_t cyc)//Accu_Cycles
{
	g_gatt_value.acyc=cyc;
	//GattSetData( LIST_DTA, DTA_FCCP, (uint8_t*)&cap);
}
	
void GattSetHashTop(uint8_t*hash,uint8_t size)//PAYG_Security_Hash_Top
{
	if(size<=31)
	{	memcpy(g_gatt_value.pubk,hash,size);
		GattSetData( LIST_CMD, CMD_PUBK, hash);
		}
}

void GattSetGprsSleepTime(uint16_t time)//GPRS Sleep Time Period
{
	g_gatt_value.gstw=time;
	GattSetData( LIST_CMD, CMD_GSTW, (uint8_t*)&time);
}

void GattSetGprsConnectTime(uint16_t time)//GPRS Connection Time Period
{
	g_gatt_value.gctw=time;
	GattSetData( LIST_CMD, CMD_GCTW, (uint8_t*)&time);
}	

void GattSetChargePower(uint16_t bat)//Charge_Power
{
	g_gatt_value.batp=bat;
	#if defined(UI1K_V13_PROJECT)||defined(E_MOB48V_PROJECT)
	GattSetData( LIST_DTA, DTA_BATP, (uint8_t*)&bat);
	#endif
}	
	
void GattSetSolarGeneration(uint16_t solar)//Solar Generation
{
	g_gatt_value.inpp=solar;
	#if  defined(UI1K_V13_PROJECT)||defined(E_MOB48V_PROJECT)
	GattSetData( LIST_DTA, DTA_INPP, (uint8_t*)&solar);
	#endif
}	

void GattSetLoadPower(uint16_t pwr)//Load Power
{
	g_gatt_value.outp=pwr;

	GattSetData( LIST_DTA, DTA_OUTP, (uint8_t*)&pwr);

}	

uint16_t GattGetLoadPower(void)//Load Power
{
	uint16_t temp;

	GattGetData( LIST_DTA, DTA_OUTP, (uint8_t*)&temp);
	return temp;
	
	//return g_gatt_value.outp;
}	
void GattSetAccuEnergy(uint16_t energy)//Accu_Energy_Output
{
	g_gatt_value.aeng=energy;
	#ifdef UI1K_V13_PROJECT
	GattSetData( LIST_DTA, DTA_AENG, (uint8_t*)&energy);
	#endif
}	

uint16_t GattGetAccuEnergy(void)//Accu_Energy_Output
{
	uint16_t temp;
	#ifdef UI1K_V13_PROJECT	
	GattGetData( LIST_DTA, DTA_AENG, (uint8_t*)&temp);
	#endif

	return temp;
	//return g_gatt_value.aeng;
}	

void GattSetCellVolt1(uint16_t cell_v)//CellVoltage1
{
	g_gatt_value.cv01=cell_v;
	#ifdef UI1K_V13_PROJECT
	GattSetData( LIST_DIA, DIA_CV01, (uint8_t*)&cell_v);
	#endif
}	

void GattSetCellVolt2(uint16_t cell_v)//CellVoltage2
{
	g_gatt_value.cv02=cell_v;
	#ifdef UI1K_V13_PROJECT
	GattSetData( LIST_DIA, DIA_CV02, (uint8_t*)&cell_v);
	#endif
}	

void GattSetCellVolt3(uint16_t cell_v)//CellVoltage3
{
	g_gatt_value.cv03=cell_v;
	#ifdef UI1K_V13_PROJECT
	GattSetData( LIST_DIA, DIA_CV03, (uint8_t*)&cell_v);
	#endif
}	

void GattSetCellVolt4(uint16_t cell_v)//CellVoltage4
{
	g_gatt_value.cv04=cell_v;
	#ifdef UI1K_V13_PROJECT
	GattSetData( LIST_DIA, DIA_CV04, (uint8_t*)&cell_v);
	#endif
}	

void GattSetCellVolt5(uint16_t cell_v)//CellVoltage4
{
//	g_gatt_value.cv04=cell_v;
	#ifdef UI1K_V13_PROJECT
	GattSetData( LIST_DIA, DIA_CV05, (uint8_t*)&cell_v);
	#endif
}	

void GattSetCellVolt6(uint16_t cell_v)//CellVoltage4
{
	//g_gatt_value.cv04=cell_v;
	#ifdef UI1K_V13_PROJECT
	GattSetData( LIST_DIA, DIA_CV06, (uint8_t*)&cell_v);
	#endif
}	

void GattSetCellVolt7(uint16_t cell_v)//CellVoltage4
{
	//g_gatt_value.cv04=cell_v;
	#ifdef UI1K_V13_PROJECT
	GattSetData( LIST_DIA, DIA_CV07, (uint8_t*)&cell_v);
	#endif
}	



	
void GattSetBattVolt(uint32_t volt)//BatteryVoltage
{
	g_gatt_value.pckv=volt;
	#if defined(UI1K_V13_PROJECT)||defined(E_MOB48V_PROJECT)
	GattSetData( LIST_DTA, DTA_PCKV, (uint8_t*)&volt);
	#endif
}	

uint16_t GattGetBattVolt(void)//BatteryVoltage
{
	uint16_t temp;
	#if defined(UI1K_V13_PROJECT)||defined(E_MOB48V_PROJECT)
	GattGetData( LIST_DTA, DTA_PCKV, (uint8_t*)&temp);
	#endif
	return temp;
	//return g_gatt_value.pckv;
}	

void GattSetBattCurrent(uint16_t current)//BatteryCurrent
{
	g_gatt_value.pckc=current;
	#if defined(UI1K_V13_PROJECT)||defined(E_MOB48V_PROJECT)
	GattSetData( LIST_DTA, DTA_PCKC, (uint8_t*)&current);
	#endif
}	

uint16_t GattGetBattCurrent(void)//BatteryVoltage
{
	uint16_t temp;
	#if defined(UI1K_V13_PROJECT)||defined(E_MOB48V_PROJECT)
	GattGetData( LIST_DTA, DTA_PCKC, (uint8_t*)&temp);
	#endif
	return temp;
	//return g_gatt_value.pckc;
}	


void GattSetBattTemp(uint16_t temp)//BatteryTemperature
{
	g_gatt_value.pckt=temp;
	#if  defined(UI1K_V13_PROJECT)||defined(E_MOB48V_PROJECT)
	GattSetData( LIST_DTA, DTA_PCKT, (uint8_t*)&temp);
	#endif
}	

uint16_t  GattGetBattTemp(void)//BatteryTemperature
{
	uint16_t temp;
	#if  defined(UI1K_V13_PROJECT)||defined(E_MOB48V_PROJECT)
	GattGetData( LIST_DTA, DTA_PCKT, (uint8_t*)&temp);
	#endif
	return temp;
	//return g_gatt_value.pckt;
}	

void GattSetGpsCordLat(uint8_t* lat)//GPSCordLat
{
	//g_gatt_value.plat=lat;
	if(strlen((char*)lat)<16)
	{	memcpy(g_gatt_value.plat,lat,strlen((char*)lat));
		GattSetData( LIST_STS, STS_PLAT, g_gatt_value.plat);
		}
}	

void GattSetGpsCordLon(uint8_t *lon)//GPSCordLon
{
	//g_gatt_value.plon=lon;
	if(strlen((char*)lon)<16)
	{	memcpy(g_gatt_value.plon,lon,strlen((char*)lon));
		GattSetData( LIST_STS, STS_PLON, g_gatt_value.plon);
		}
}	

void GattSetDeviceTime(uint8_t *time,uint8_t size)//DeviceTimer
{
	//if(size<=15)
	//	memcpy(g_gatt_value.time,time,size);
}	

void GattSetGprsRssi(uint16_t csq)//GPRSSignalStrength
{
	g_gatt_value.gpss=csq;
	GattSetData( LIST_STS, STS_GPSS, (uint8_t*)&csq);
}	

void GattSetGprsConnectFailCnt(uint16_t cnt)//GPRS Connct Failure Count
{
	g_gatt_value.gpfc=cnt;
	GattSetData( LIST_STS, STS_GPFC, (uint8_t*)&cnt);
}	

void GattSetReportMode(uint16_t mode)//GPRS Connct Failure Count
{
	g_gatt_value.rptm=mode;
	GattSetData( LIST_CMD, CMD_RPTM, (uint8_t*)&mode);
}	

void GattSetHeartbeat(uint16_t freq)//
{
	g_gatt_value.hbfq=freq;
	GattSetData( LIST_CMD, CMD_HBFQ, (uint8_t*)&freq);
}	


#ifdef PUMP_PROJECT

void GattSetPumpMfrqSet(uint16_t mfrq)
{
	g_gatt_value.mfrq=mfrq;
	GattSetData( LIST_DTA, DTA_MFRQ, (uint8_t*)&mfrq);
}	

void GattSetPumpOutputPwr(uint16_t outp)
{
	g_gatt_value.outp=outp;
	GattSetData( LIST_DTA, DTA_OUTP, (uint8_t*)&outp);
}	

void GattSetPumpOutputVolt(uint16_t outv)
{
	g_gatt_value.outv=outv;
	GattSetData( LIST_DTA, DTA_OUTV, (uint8_t*)&outv);
}	

void GattSetPumpInputVolt(uint16_t inpv)
{
	g_gatt_value.inpv=inpv;
	#ifdef UI1K_V13_PROJECT
	GattSetData( LIST_DTA, DTA_INPP, (uint8_t*)&inpv);
	#endif
}	
 
void GattSetPumpOutC(uint16_t outc) //
{
	g_gatt_value.outc=outc;
	GattSetData( LIST_DTA, DTA_OUTC, (uint8_t*)&outc);
}	

void GattSetPumpMTorque (uint16_t mtrq)
{
	g_gatt_value.mtrq=mtrq;
	
	//GattSetData( LIST_DTA, DTA_MTRQ, (uint8_t*)&mtrq);
}	

void GattSetPumpMSpeed(uint16_t mspd)
{
	g_gatt_value.mspd=mspd;
	GattSetData( LIST_DTA, DTA_MSPD, (uint8_t*)&mspd);
}	

void GattSetPumpMTemp(uint16_t mtmp)
{
	g_gatt_value.mtmp=mtmp;
	//GattSetData( LIST_DTA, DTA_Mt, (uint8_t*)&mtmp);
}	

void GattSetPumpRunState(uint16_t runs)
{
	g_gatt_value.runs=runs;
//	GattSetData( LIST_DTA, DTA_SYST, (uint8_t*)&runs);
}	


#endif	





