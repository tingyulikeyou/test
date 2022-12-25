#ifndef __GATT_H__
#define __GATT_H__

#define PROP_LENGTH 16
#define META_LENGTH 63
#define TEXT_LENGTH 64


#ifdef P10KW_PROJECT
#define JSON_LEN  1024*4
#else
#define JSON_LEN  1024*2
#endif
#define NEW_GATTVERSION

#define MEM_POS(name) MEM_ADDR_##name+MEM_SIZE_##name
//#define MEM_SIZE(name) MEM_SIZE_##name



#define MEM_SIZE_OPID    20
#define MEM_SIZE_PPID    32
#define MEM_SIZE_FLID    32
#define MEM_SIZE_FRMV    8
//CMD
#define MEM_SIZE_PUBK    32
#define MEM_SIZE_GSTW    2	
#define MEM_SIZE_GCTW    2
#define MEM_SIZE_NAPN    32	
#define MEM_SIZE_SWCH    2
#define MEM_SIZE_READ    32
#define MEM_SIZE_RPTM    2
#define MEM_SIZE_HBFQ    2

#ifdef P10KW_PROJECT
#define MEM_SIZE_AOCT        2
#define MEM_SIZE_AOSS        2
#define MEM_SIZE_BCSS        2
#define MEM_SIZE_UOST        2
#define MEM_SIZE_UOET        2
#define MEM_SIZE_UCST        2
#define MEM_SIZE_UCET        2
#define MEM_SIZE_PIMD        2
#define MEM_SIZE_AIMD        2
#define MEM_SIZE_LNGS        2
#define MEM_SIZE_AOVS        2
#define MEM_SIZE_AOFS        2
#define MEM_SIZE_OLRS        2
#define MEM_SIZE_OTRS        2
#define MEM_SIZE_BZON        2
#define MEM_SIZE_CASS        2
#define MEM_SIZE_MXCC        2
#define MEM_SIZE_BBCV        2
#define MEM_SIZE_BFCV        2
#define MEM_SIZE_LBUS        2
#define MEM_SIZE_ACCS        2
#define MEM_SIZE_AGMD        2
#define MEM_SIZE_STYS        2
#define MEM_SIZE_STMS        2
#define MEM_SIZE_STDS        2
#define MEM_SIZE_STHS        2
#define MEM_SIZE_STMM        2
#define MEM_SIZE_STSS        2
#endif
//STS
#define MEM_SIZE_SSTC    2
#define MEM_SIZE_CRTM    16
#define MEM_SIZE_UDTM    16
#define MEM_SIZE_TIME    16
#define MEM_SIZE_PLAT    16
#define MEM_SIZE_PLON    16
#define MEM_SIZE_GPSS    2
#define MEM_SIZE_GPFC    2
#define MEM_SIZE_RCRD    2	
#define MEM_SIZE_TRHD    2	
#define MEM_SIZE_TPGD    2	
#define MEM_SIZE_PGST    2		
#define MEM_SIZE_TKRE    2
#define MEM_SIZE_OCST    2


#if defined(UI1K_V13_PROJECT)||defined(UI1K_V2_PROJECT)

////DTA
#define MEM_SIZE_BATP    2
#define MEM_SIZE_INPP    2
#define MEM_SIZE_OUTP    2
#define MEM_SIZE_AENG    2
#define MEM_SIZE_PCKV    4
#define MEM_SIZE_PCKC    2
#define MEM_SIZE_RSOC    2
#define MEM_SIZE_RCAP    2
#define MEM_SIZE_FCCP    2
#define MEM_SIZE_PCKT    2
#define MEM_SIZE_ACYC    2

//DIA
#define MEM_SIZE_CV01    2
#define MEM_SIZE_CV02    2
#define MEM_SIZE_CV03    2
#define MEM_SIZE_CV04    2
#define MEM_SIZE_CV05    2
#define MEM_SIZE_CV06    2
#define MEM_SIZE_CV07    2
#elif defined(E_MOB48V_PROJECT)
////DTA
#define MEM_SIZE_BATP    2
#define MEM_SIZE_INPP    2
#define MEM_SIZE_OUTP    2
#define MEM_SIZE_AENG    2
#define MEM_SIZE_PCKV    4
#define MEM_SIZE_PCKC    2
#define MEM_SIZE_RSOC    2
#define MEM_SIZE_RCAP    2
#define MEM_SIZE_FCCP    2
#define MEM_SIZE_PCKT    2

#define MEM_SIZE_SLON    16
#define MEM_SIZE_SLAT    16
#define MEM_SIZE_SALT    2
#define MEM_SIZE_SSTM    24
#define MEM_SIZE_SGPS    2
#define MEM_SIZE_SSPE    2


//DIA
#define MEM_SIZE_CV01    2
#define MEM_SIZE_CV02    2
#define MEM_SIZE_CV03    2
#define MEM_SIZE_CV04    2
#define MEM_SIZE_CV05    2
#define MEM_SIZE_CV06    2
#define MEM_SIZE_CV07    2
#define MEM_SIZE_CV08    2
#define MEM_SIZE_CV09    2
#define MEM_SIZE_CV10    2
#define MEM_SIZE_CV11    2
#define MEM_SIZE_CV12    2
#define MEM_SIZE_CV13    2
#define MEM_SIZE_CV14    2
#define MEM_SIZE_CV15    2
#define MEM_SIZE_CV16    2

#ifdef BMS_JBD_SUPPROT
#define MEM_SIZE_CV17    2
#define MEM_SIZE_CV18    2
#define MEM_SIZE_CV19    2
#define MEM_SIZE_CV20    2
#endif

#elif defined(P10KW_PROJECT)

//DTA                      
#define MEM_SIZE_BATP    2 
#define MEM_SIZE_INPP    2 
#define MEM_SIZE_OUTP    2 
#define MEM_SIZE_AENG    2 
#define MEM_SIZE_PCKV    4 
#define MEM_SIZE_PCKC    2 
#define MEM_SIZE_RSOC    2 
#define MEM_SIZE_RCAP    2 
#define MEM_SIZE_FCCP    2 
#define MEM_SIZE_PCKT    2 
#define MEM_SIZE_ACYC    2 
#define MEM_SIZE_PV1V    2 
#define MEM_SIZE_PV2V    2 
#define MEM_SIZE_PV1P    4 
#define MEM_SIZE_PV2P    4 
#define MEM_SIZE_CG1C    2 
#define MEM_SIZE_CG2C    2 
#define MEM_SIZE_AOAP    4 
#define MEM_SIZE_AOVA    4 
#define MEM_SIZE_ACAP    4 
#define MEM_SIZE_ACVA    4 
#define MEM_SIZE_BPKV    2 
#define MEM_SIZE_ACOV    2 
#define MEM_SIZE_ACOF    2 
#define MEM_SIZE_DCOV    2 
#define MEM_SIZE_LDPP    2 
#define MEM_SIZE_OUTC    2 
#define MEM_SIZE_AIPP    4 
#define MEM_SIZE_FLTB    2 
#define MEM_SIZE_WRNB    4 
#define MEM_SIZE_ACDE    4 
#define MEM_SIZE_ACTE    4 
#define MEM_SIZE_BDDE    4 
#define MEM_SIZE_BDTE    4 
#define MEM_SIZE_AODE    4 
#define MEM_SIZE_AOTE    4
#define MEM_SIZE_BTNP    4 
#define MEM_SIZE_BOCF    2 

//DIA

#define MEM_SIZE_BUSV  2 
#define MEM_SIZE_ACIV  2 
#define MEM_SIZE_ACIF  2 
#define MEM_SIZE_IVTT  2 
#define MEM_SIZE_CVTT  2 
#define MEM_SIZE_BPTV  2 
#define MEM_SIZE_BBSV  2 
#define MEM_SIZE_WKTM  4 
#define MEM_SIZE_BK1T  2 
#define MEM_SIZE_BK2T  2 
#define MEM_SIZE_INTC  2 
#define MEM_SIZE_AIAP  4 
#define MEM_SIZE_AIVA  4 
#define MEM_SIZE_WRNV  4
#define MEM_SIZE_DVTC  2 
#define MEM_SIZE_DVCS  2 
#define MEM_SIZE_PRLM  2 
#define MEM_SIZE_CPOK  2 
#define MEM_SIZE_P1DE  4 
#define MEM_SIZE_P1TE  4
#define MEM_SIZE_P2DE  4 
#define MEM_SIZE_P2TE  4 
#define MEM_SIZE_ABCC  2 
#define MEM_SIZE_ACDP  2 
#define MEM_SIZE_ADVA  4 
#define MEM_SIZE_BDAP  4 
#define MEM_SIZE_BDVA  4 
#define MEM_SIZE_MCFS  2 
#define MEM_SIZE_IVFS  2 
#define MEM_SIZE_CCRT  2 
#define MEM_SIZE_DCCT  2 
#define MEM_SIZE_DCDE  4 
#define MEM_SIZE_DCTE  4 
#define MEM_SIZE_PCCR  2 


#elif defined(PUMP_PROJECT)

#ifdef DC_PUMP_SUPPORT
////DTA
#define MEM_SIZE_MFRQ    2
#define MEM_SIZE_OUTV    2
#define MEM_SIZE_OUTC    2
#define MEM_SIZE_OUTP    2
//#define MEM_SIZE_MTRQ    2
#define MEM_SIZE_MSPD    2
//#define MEM_SIZE_AWCM    2
#define MEM_SIZE_TNKF    2
#define MEM_SIZE_WLLE    2
#define MEM_SIZE_SPWR    2
//#define MEM_SIZE_SYST    2

#else

#endif

#elif defined(CAMP_PROJECT)

#endif

enum
{	//ATT
	MEM_ADDR_OPID=0,
	MEM_ADDR_PPID=MEM_ADDR_OPID+MEM_SIZE_OPID,
	MEM_ADDR_FLID=MEM_ADDR_PPID+MEM_SIZE_PPID,
	MEM_ADDR_FRMV=MEM_ADDR_FLID+MEM_SIZE_FLID,
	//CMD
	MEM_ADDR_PUBK=MEM_ADDR_FRMV+MEM_SIZE_FRMV, 
	MEM_ADDR_GSTW=MEM_ADDR_PUBK+MEM_SIZE_PUBK,	
	MEM_ADDR_GCTW=MEM_ADDR_GSTW+MEM_SIZE_GSTW,
	MEM_ADDR_NAPN=MEM_ADDR_GCTW+MEM_SIZE_GCTW,	
	MEM_ADDR_SWCH=MEM_ADDR_NAPN+MEM_SIZE_NAPN,
	MEM_ADDR_READ=MEM_ADDR_SWCH+MEM_SIZE_SWCH,
	MEM_ADDR_RPTM=MEM_ADDR_READ+MEM_SIZE_READ,
	MEM_ADDR_HBFQ=MEM_ADDR_RPTM+MEM_SIZE_RPTM,
	#ifdef P10KW_PROJECT
	MEM_ADDR_AOCT=MEM_ADDR_HBFQ  +MEM_SIZE_HBFQ   , 
	MEM_ADDR_AOSS=MEM_ADDR_AOCT  +MEM_SIZE_AOCT   , 
	MEM_ADDR_BCSS=MEM_ADDR_AOSS  +MEM_SIZE_AOSS   , 
	MEM_ADDR_UOST=MEM_ADDR_BCSS  +MEM_SIZE_BCSS   , 
	MEM_ADDR_UOET=MEM_ADDR_UOST  +MEM_SIZE_UOST   , 
	MEM_ADDR_UCST=MEM_ADDR_UOET  +MEM_SIZE_UOET   , 
	MEM_ADDR_UCET=MEM_ADDR_UCST  +MEM_SIZE_UCST   , 
	MEM_ADDR_PIMD=MEM_ADDR_UCET  +MEM_SIZE_UCET   , 
	MEM_ADDR_AIMD=MEM_ADDR_PIMD  +MEM_SIZE_PIMD   , 
	MEM_ADDR_LNGS=MEM_ADDR_AIMD  +MEM_SIZE_AIMD   , 
	MEM_ADDR_AOVS=MEM_ADDR_LNGS  +MEM_SIZE_LNGS   , 
	MEM_ADDR_AOFS=MEM_ADDR_AOVS  +MEM_SIZE_AOVS   , 
	MEM_ADDR_OLRS=MEM_ADDR_AOFS  +MEM_SIZE_AOFS   , 
	MEM_ADDR_OTRS=MEM_ADDR_OLRS  +MEM_SIZE_OLRS   , 
	MEM_ADDR_BZON=MEM_ADDR_OTRS  +MEM_SIZE_OTRS   , 
	MEM_ADDR_CASS=MEM_ADDR_BZON  +MEM_SIZE_BZON   , 
	MEM_ADDR_MXCC=MEM_ADDR_CASS  +MEM_SIZE_CASS   , 
	MEM_ADDR_BBCV=MEM_ADDR_MXCC  +MEM_SIZE_MXCC   , 
	MEM_ADDR_BFCV=MEM_ADDR_BBCV  +MEM_SIZE_BBCV   , 
	MEM_ADDR_LBUS=MEM_ADDR_BFCV  +MEM_SIZE_BFCV   , 
	MEM_ADDR_ACCS=MEM_ADDR_LBUS  +MEM_SIZE_LBUS   , 
	MEM_ADDR_AGMD=MEM_ADDR_ACCS  +MEM_SIZE_ACCS   , 
	MEM_ADDR_STYS=MEM_ADDR_AGMD  +MEM_SIZE_AGMD   , 
	MEM_ADDR_STMS=MEM_ADDR_STYS  +MEM_SIZE_STYS   , 
	MEM_ADDR_STDS=MEM_ADDR_STMS  +MEM_SIZE_STMS   , 
	MEM_ADDR_STHS=MEM_ADDR_STDS  +MEM_SIZE_STDS   , 
	MEM_ADDR_STMM=MEM_ADDR_STHS  +MEM_SIZE_STHS   , 
	MEM_ADDR_STSS=MEM_ADDR_STMM  +MEM_SIZE_STMM   , 
	

	MEM_ADDR_SSTC=MEM_ADDR_STSS+MEM_SIZE_STSS,
	#else
	//STS
	MEM_ADDR_SSTC=MEM_ADDR_HBFQ+MEM_SIZE_HBFQ,
	#endif
	MEM_ADDR_CRTM=MEM_ADDR_SSTC+MEM_SIZE_SSTC,
	MEM_ADDR_UDTM=MEM_ADDR_CRTM+MEM_SIZE_CRTM,
	MEM_ADDR_TIME=MEM_ADDR_UDTM+MEM_SIZE_UDTM,
	MEM_ADDR_PLAT=MEM_ADDR_TIME+MEM_SIZE_TIME,
	MEM_ADDR_PLON=MEM_ADDR_PLAT+MEM_SIZE_PLAT,
	MEM_ADDR_GPSS=MEM_ADDR_PLON+MEM_SIZE_PLON,
	MEM_ADDR_GPFC=MEM_ADDR_GPSS+MEM_SIZE_GPSS,
	MEM_ADDR_RCRD=MEM_ADDR_GPFC+MEM_SIZE_GPFC,	
	MEM_ADDR_TRHD=MEM_ADDR_RCRD+MEM_SIZE_RCRD,	
	MEM_ADDR_TPGD=MEM_ADDR_TRHD+MEM_SIZE_TRHD,	
	MEM_ADDR_PGST=MEM_ADDR_TPGD+MEM_SIZE_TPGD,	
	MEM_ADDR_TKRE=MEM_ADDR_PGST+MEM_SIZE_PGST,	
	MEM_ADDR_OCST=MEM_ADDR_TKRE+MEM_SIZE_TKRE,

#if defined(UI1K_V13_PROJECT)||defined(UI1K_V2_PROJECT)
	////DTA
	MEM_ADDR_BATP=MEM_ADDR_OCST+MEM_SIZE_OCST,
	MEM_ADDR_INPP=MEM_ADDR_BATP+MEM_SIZE_BATP,
	MEM_ADDR_OUTP=MEM_ADDR_INPP+MEM_SIZE_INPP,
	MEM_ADDR_AENG=MEM_ADDR_OUTP+MEM_SIZE_OUTP,
	MEM_ADDR_PCKV=MEM_ADDR_AENG+MEM_SIZE_AENG,
	MEM_ADDR_PCKC=MEM_ADDR_PCKV+MEM_SIZE_PCKV,
	MEM_ADDR_RSOC=MEM_ADDR_PCKC+MEM_SIZE_PCKC,
	MEM_ADDR_RCAP=MEM_ADDR_RSOC+MEM_SIZE_RSOC,
	MEM_ADDR_FCCP=MEM_ADDR_RCAP+MEM_SIZE_RCAP,
	MEM_ADDR_PCKT=MEM_ADDR_FCCP+MEM_SIZE_FCCP,
	MEM_ADDR_ACYC=MEM_ADDR_PCKT+MEM_SIZE_PCKT,

	//DIA
	MEM_ADDR_CV01=MEM_ADDR_ACYC+MEM_SIZE_ACYC,
	MEM_ADDR_CV02=MEM_ADDR_CV01+MEM_SIZE_CV01,
	MEM_ADDR_CV03=MEM_ADDR_CV02+MEM_SIZE_CV02,
	MEM_ADDR_CV04=MEM_ADDR_CV03+MEM_SIZE_CV03,
	MEM_ADDR_CV05=MEM_ADDR_CV04+MEM_SIZE_CV04,
	MEM_ADDR_CV06=MEM_ADDR_CV05+MEM_SIZE_CV05,
	MEM_ADDR_CV07=MEM_ADDR_CV06+MEM_SIZE_CV06,
	MEM_GATT_SIZE=MEM_ADDR_CV07+MEM_SIZE_CV07
	
#elif defined(E_MOB48V_PROJECT)	
	////DTA
	MEM_ADDR_BATP=MEM_ADDR_OCST+MEM_SIZE_OCST,
	MEM_ADDR_INPP=MEM_ADDR_BATP+MEM_SIZE_BATP,
	MEM_ADDR_OUTP=MEM_ADDR_INPP+MEM_SIZE_INPP,
	MEM_ADDR_AENG=MEM_ADDR_OUTP+MEM_SIZE_OUTP,
	MEM_ADDR_PCKV=MEM_ADDR_AENG+MEM_SIZE_AENG,
	MEM_ADDR_PCKC=MEM_ADDR_PCKV+MEM_SIZE_PCKV,
	MEM_ADDR_RSOC=MEM_ADDR_PCKC+MEM_SIZE_PCKC,
	MEM_ADDR_RCAP=MEM_ADDR_RSOC+MEM_SIZE_RSOC,
	MEM_ADDR_FCCP=MEM_ADDR_RCAP+MEM_SIZE_RCAP,
	MEM_ADDR_PCKT=MEM_ADDR_FCCP+MEM_SIZE_FCCP,

	MEM_ADDR_SLON=MEM_ADDR_PCKT+MEM_SIZE_PCKT,
	MEM_ADDR_SLAT=MEM_ADDR_SLON+MEM_SIZE_SLON,
	MEM_ADDR_SALT=MEM_ADDR_SLAT+MEM_SIZE_SLAT,
	MEM_ADDR_SSTM=MEM_ADDR_SALT+MEM_SIZE_SALT,
	MEM_ADDR_SGPS=MEM_ADDR_SSTM+MEM_SIZE_SSTM,
	MEM_ADDR_SSPE=MEM_ADDR_SGPS+MEM_SIZE_SGPS,

	
	//DIA
	///MEM_ADDR_CV01=MEM_ADDR_PCKT+MEM_SIZE_PCKT,
	MEM_ADDR_CV01=MEM_ADDR_SSPE+MEM_SIZE_SSPE,
	MEM_ADDR_CV02=MEM_ADDR_CV01+MEM_SIZE_CV01,
	MEM_ADDR_CV03=MEM_ADDR_CV02+MEM_SIZE_CV02,
	MEM_ADDR_CV04=MEM_ADDR_CV03+MEM_SIZE_CV03,
	MEM_ADDR_CV05=MEM_ADDR_CV04+MEM_SIZE_CV04,
	MEM_ADDR_CV06=MEM_ADDR_CV05+MEM_SIZE_CV05,
	MEM_ADDR_CV07=MEM_ADDR_CV06+MEM_SIZE_CV06,
	MEM_ADDR_CV08=MEM_ADDR_CV07+MEM_SIZE_CV07,
	MEM_ADDR_CV09=MEM_ADDR_CV08+MEM_SIZE_CV08,
	MEM_ADDR_CV10=MEM_ADDR_CV09+MEM_SIZE_CV09,
	MEM_ADDR_CV11=MEM_ADDR_CV10+MEM_SIZE_CV10,
	MEM_ADDR_CV12=MEM_ADDR_CV11+MEM_SIZE_CV11,
	MEM_ADDR_CV13=MEM_ADDR_CV12+MEM_SIZE_CV12,
	MEM_ADDR_CV14=MEM_ADDR_CV13+MEM_SIZE_CV13,
	MEM_ADDR_CV15=MEM_ADDR_CV14+MEM_SIZE_CV14,
	MEM_ADDR_CV16=MEM_ADDR_CV15+MEM_SIZE_CV15,
	#ifdef BMS_JBD_SUPPROT
	MEM_ADDR_CV17=MEM_ADDR_CV16+MEM_SIZE_CV16,
	MEM_ADDR_CV18=MEM_ADDR_CV17+MEM_SIZE_CV17,
	MEM_ADDR_CV19=MEM_ADDR_CV18+MEM_SIZE_CV18,
	MEM_ADDR_CV20=MEM_ADDR_CV19+MEM_SIZE_CV19,
	MEM_GATT_SIZE=MEM_ADDR_CV20+MEM_SIZE_CV20
	#else
	MEM_GATT_SIZE=MEM_ADDR_CV16+MEM_SIZE_CV16
	#endif
#elif defined(P10KW_PROJECT)	
	//DTA									 
	 MEM_ADDR_BATP=MEM_ADDR_OCST +MEM_SIZE_OCST  ,	 
	 MEM_ADDR_INPP=MEM_ADDR_BATP +MEM_SIZE_BATP  ,	 
	 MEM_ADDR_OUTP=MEM_ADDR_INPP +MEM_SIZE_INPP  ,	 
	 MEM_ADDR_AENG=MEM_ADDR_OUTP +MEM_SIZE_OUTP  ,	 
	 MEM_ADDR_PCKV=MEM_ADDR_AENG +MEM_SIZE_AENG  ,	 
	 MEM_ADDR_PCKC=MEM_ADDR_PCKV +MEM_SIZE_PCKV  ,	 
	 MEM_ADDR_RSOC=MEM_ADDR_PCKC +MEM_SIZE_PCKC  ,	 
	 MEM_ADDR_RCAP=MEM_ADDR_RSOC +MEM_SIZE_RSOC  ,	 
	 MEM_ADDR_FCCP=MEM_ADDR_RCAP +MEM_SIZE_RCAP  ,	 
	 MEM_ADDR_PCKT=MEM_ADDR_FCCP +MEM_SIZE_FCCP  ,	 
	 MEM_ADDR_ACYC=MEM_ADDR_PCKT +MEM_SIZE_PCKT  ,	 
	 MEM_ADDR_PV1V=MEM_ADDR_ACYC +MEM_SIZE_ACYC  ,	 
	 MEM_ADDR_PV2V=MEM_ADDR_PV1V +MEM_SIZE_PV1V  ,	 
	 MEM_ADDR_PV1P=MEM_ADDR_PV2V +MEM_SIZE_PV2V  ,	 
	 MEM_ADDR_PV2P=MEM_ADDR_PV1P +MEM_SIZE_PV1P  ,	 
	 MEM_ADDR_CG1C=MEM_ADDR_PV2P +MEM_SIZE_PV2P  ,	 
	 MEM_ADDR_CG2C=MEM_ADDR_CG1C +MEM_SIZE_CG1C  ,	 
	 MEM_ADDR_AOAP=MEM_ADDR_CG2C +MEM_SIZE_CG2C  ,	 
	 MEM_ADDR_AOVA=MEM_ADDR_AOAP +MEM_SIZE_AOAP  ,	 
	 MEM_ADDR_ACAP=MEM_ADDR_AOVA +MEM_SIZE_AOVA  ,	 
	 MEM_ADDR_ACVA=MEM_ADDR_ACAP +MEM_SIZE_ACAP  ,	 
	 MEM_ADDR_BPKV=MEM_ADDR_ACVA +MEM_SIZE_ACVA  ,	 
	 MEM_ADDR_ACOV=MEM_ADDR_BPKV +MEM_SIZE_BPKV  ,	 
	 MEM_ADDR_ACOF=MEM_ADDR_ACOV +MEM_SIZE_ACOV  ,	 
	 MEM_ADDR_DCOV=MEM_ADDR_ACOF +MEM_SIZE_ACOF  ,	 
	 MEM_ADDR_LDPP=MEM_ADDR_DCOV +MEM_SIZE_DCOV  ,	 
	 MEM_ADDR_OUTC=MEM_ADDR_LDPP +MEM_SIZE_LDPP  ,	 
	 MEM_ADDR_AIPP=MEM_ADDR_OUTC +MEM_SIZE_OUTC  ,	 
	 MEM_ADDR_FLTB=MEM_ADDR_AIPP +MEM_SIZE_AIPP  ,	 
	 MEM_ADDR_WRNB=MEM_ADDR_FLTB +MEM_SIZE_FLTB  ,	 
	 MEM_ADDR_ACDE=MEM_ADDR_WRNB +MEM_SIZE_WRNB  ,	 
	 MEM_ADDR_ACTE=MEM_ADDR_ACDE +MEM_SIZE_ACDE  ,	 
	 MEM_ADDR_BDDE=MEM_ADDR_ACTE +MEM_SIZE_ACTE  ,	 
	 MEM_ADDR_BDTE=MEM_ADDR_BDDE +MEM_SIZE_BDDE  ,	 
	 MEM_ADDR_AODE=MEM_ADDR_BDTE +MEM_SIZE_BDTE  ,	 
	 MEM_ADDR_AOTE=MEM_ADDR_AODE +MEM_SIZE_AODE  ,	 
	 MEM_ADDR_BTNP=MEM_ADDR_AOTE +MEM_SIZE_AOTE  ,	 
	 MEM_ADDR_BOCF=MEM_ADDR_BTNP +MEM_SIZE_BTNP  ,	 
											 
	//DIA									 
											 
	 MEM_ADDR_BUSV=MEM_ADDR_BOCF +MEM_SIZE_BOCF ,	 
	 MEM_ADDR_ACIV=MEM_ADDR_BUSV +MEM_SIZE_BUSV ,	 
	 MEM_ADDR_ACIF=MEM_ADDR_ACIV +MEM_SIZE_ACIV ,	 
	 MEM_ADDR_IVTT=MEM_ADDR_ACIF +MEM_SIZE_ACIF ,	 
	 MEM_ADDR_CVTT=MEM_ADDR_IVTT +MEM_SIZE_IVTT ,	 
	 MEM_ADDR_BPTV=MEM_ADDR_CVTT +MEM_SIZE_CVTT ,	 
	 MEM_ADDR_BBSV=MEM_ADDR_BPTV +MEM_SIZE_BPTV ,	 
	 MEM_ADDR_WKTM=MEM_ADDR_BBSV +MEM_SIZE_BBSV ,	 
	 MEM_ADDR_BK1T=MEM_ADDR_WKTM +MEM_SIZE_WKTM ,	 
	 MEM_ADDR_BK2T=MEM_ADDR_BK1T +MEM_SIZE_BK1T ,	 
	 MEM_ADDR_INTC=MEM_ADDR_BK2T +MEM_SIZE_BK2T ,	 
	 MEM_ADDR_AIAP=MEM_ADDR_INTC +MEM_SIZE_INTC ,	 
	 MEM_ADDR_AIVA=MEM_ADDR_AIAP +MEM_SIZE_AIAP ,	 
	 MEM_ADDR_WRNV=MEM_ADDR_AIVA +MEM_SIZE_AIVA ,	 
	 MEM_ADDR_DVTC=MEM_ADDR_WRNV +MEM_SIZE_WRNV ,	 
	 MEM_ADDR_DVCS=MEM_ADDR_DVTC +MEM_SIZE_DVTC ,	 
	 MEM_ADDR_PRLM=MEM_ADDR_DVCS +MEM_SIZE_DVCS ,	 
	 MEM_ADDR_CPOK=MEM_ADDR_PRLM +MEM_SIZE_PRLM ,	 
	 MEM_ADDR_P1DE=MEM_ADDR_CPOK +MEM_SIZE_CPOK ,	 
	 MEM_ADDR_P1TE=MEM_ADDR_P1DE +MEM_SIZE_P1DE ,	 
	 MEM_ADDR_P2DE=MEM_ADDR_P1TE +MEM_SIZE_P1TE ,	 
	 MEM_ADDR_P2TE=MEM_ADDR_P2DE +MEM_SIZE_P2DE ,	 
	 MEM_ADDR_ABCC=MEM_ADDR_P2TE +MEM_SIZE_P2TE ,	 
	 MEM_ADDR_ACDP=MEM_ADDR_ABCC +MEM_SIZE_ABCC ,	 
	 MEM_ADDR_ADVA=MEM_ADDR_ACDP +MEM_SIZE_ACDP ,	 
	 MEM_ADDR_BDAP=MEM_ADDR_ADVA +MEM_SIZE_ADVA ,	 
	 MEM_ADDR_BDVA=MEM_ADDR_BDAP +MEM_SIZE_BDAP ,	 
	 MEM_ADDR_MCFS=MEM_ADDR_BDVA +MEM_SIZE_BDVA ,	 
	 MEM_ADDR_IVFS=MEM_ADDR_MCFS +MEM_SIZE_MCFS ,	 
	 MEM_ADDR_CCRT=MEM_ADDR_IVFS +MEM_SIZE_IVFS ,	 
	 MEM_ADDR_DCCT=MEM_ADDR_CCRT +MEM_SIZE_CCRT ,	 
	 MEM_ADDR_DCDE=MEM_ADDR_DCCT +MEM_SIZE_DCCT ,	 
	 MEM_ADDR_DCTE=MEM_ADDR_DCDE +MEM_SIZE_DCDE ,	 
	 MEM_ADDR_PCCR=MEM_ADDR_DCTE +MEM_SIZE_DCTE ,

											 
	MEM_GATT_SIZE=MEM_ADDR_PCCR+MEM_SIZE_PCCR
	
#elif defined(PUMP_PROJECT)	

#ifdef DC_PUMP_SUPPORT
	////DTA
   MEM_ADDR_MFRQ=MEM_ADDR_OCST+MEM_SIZE_OCST,
   MEM_ADDR_OUTV=MEM_ADDR_MFRQ+MEM_SIZE_MFRQ,   
   MEM_ADDR_OUTC=MEM_ADDR_OUTV+MEM_SIZE_OUTV,   
   MEM_ADDR_OUTP=MEM_ADDR_OUTC+MEM_SIZE_OUTC,   
   MEM_ADDR_MSPD=MEM_ADDR_OUTP+MEM_SIZE_OUTP,   
   MEM_ADDR_TNKF=MEM_ADDR_MSPD+MEM_SIZE_MSPD,   
   MEM_ADDR_WLLE=MEM_ADDR_TNKF+MEM_SIZE_TNKF,   
   MEM_ADDR_SPWR=MEM_ADDR_WLLE+MEM_SIZE_WLLE,   
   MEM_GATT_SIZE=MEM_ADDR_SPWR+MEM_SIZE_SPWR
#else
	
#endif


#endif

};


enum
{
	TYPE_UINT16,
	TYPE_STRS,
	TYPE_INT,
	TYPE_FLOAT,
	TYPE_FLOAT_STR,
	TYPE_UINT32,
	TYPE_INT32
};




enum
{
	ATT_OPID,
	ATT_PPID,
	ATT_FLID,
	ATT_FRMV,
	ATT_COUNT
};

enum
{
	CMD_PUBK, 
	CMD_GSTW,	
	CMD_GCTW,
	CMD_NAPN,	
	CMD_SWCH,
	CMD_READ,
	CMD_RPTM,
	CMD_HBFQ,
	#ifdef P10KW_PROJECT
	CMD_AOCT,
    CMD_AOSS,
    CMD_BCSS,
    CMD_UOST,
    CMD_UOET,
    CMD_UCST,
    CMD_UCET,
    CMD_PIMD,
    CMD_AIMD,
    CMD_LNGS,
    CMD_AOVS,
    CMD_AOFS,
    CMD_OLRS,
    CMD_OTRS,
    CMD_BZON,
    CMD_CASS,
    CMD_MXCC,
    CMD_BBCV,
    CMD_BFCV,
    CMD_LBUS,
    CMD_ACCS,
    CMD_AGMD,
    CMD_STYS,
    CMD_STMS,
    CMD_STDS,
    CMD_STHS,
    CMD_STMM,
    CMD_STSS,
	#endif
	CMD_COUNT
};	

enum
{
	STS_SSTC,
	STS_CRTM,
	STS_UDTM,
	STS_TIME,
	STS_PLAT,
	STS_PLON,
	STS_GPSS,
	STS_GPFC,
	STS_RCRD,	
	STS_TRHD,	
	STS_TPGD,	
	STS_PGST,	
	STS_TKRE,
	STS_OCST,		
	STS_COUNT
};
#if defined(UI1K_V13_PROJECT)||defined(UI1K_V2_PROJECT)
enum
{
	DTA_BATP,
	DTA_INPP,
	DTA_OUTP,
	DTA_AENG,
	DTA_PCKV,
	DTA_PCKC,
	DTA_RSOC,
	DTA_RCAP,
	DTA_FCCP,
	DTA_PCKT,
	DTA_ACYC,
	DTA_COUNT
};

enum
{
	DIA_CV01,
	DIA_CV02,
	DIA_CV03,
	DIA_CV04,
	DIA_CV05,
	DIA_CV06,
	DIA_CV07,
	DIA_COUNT
};
#elif defined(E_MOB48V_PROJECT)
enum
{
	DTA_BATP,
	DTA_INPP,
	DTA_OUTP,
	DTA_AENG,
	DTA_PCKV,
	DTA_PCKC,
	DTA_RSOC,
	DTA_RCAP,
	DTA_FCCP,
	DTA_PCKT,
	DTA_SLON,
	DTA_SLAT,
	DTA_SALT,
	DTA_SSTM,
	DTA_SGPS,
	DTA_SSPE,
	DTA_COUNT
};

enum
{
	DIA_CV01,
	DIA_CV02,
	DIA_CV03,
	DIA_CV04,
	DIA_CV05,
	DIA_CV06,
	DIA_CV07,
	DIA_CV08,
	DIA_CV09,
	DIA_CV10,
	DIA_CV11,
	DIA_CV12,
	DIA_CV13,
	DIA_CV14,
	DIA_CV15,
	DIA_CV16,
	#ifdef BMS_JBD_SUPPROT
	DIA_CV17,
	DIA_CV18,
	DIA_CV19,
	DIA_CV20,
	#endif
	DIA_COUNT
};

#elif defined(P10KW_PROJECT)
enum
{
    DTA_BATP,//BATTERY NET CHARGING POWER W",
    DTA_INPP,//TOTAL INPUT ENERGY KWH",
    DTA_OUTP,//OUTPUT POWER W,",
    DTA_AENG,//ACCUMULATED ENERGY OUTPUT IN WHS",
    DTA_PCKV,//PACK VOLTAGE MV",
    DTA_PCKC,//PACK CURRENT IN MA",
    DTA_RSOC,//RELATIVE STATE OF CHARGE",
    DTA_RCAP,//REMAINING CAPACITY IN WHS",
    DTA_FCCP,//FULL CHARGE CAPACITY IN WHS",
    DTA_PCKT,//PACK TEMPERATURE IN CELSIUS",
    DTA_ACYC,//ACCUMULATED CYCLES",
    DTA_PV1V,//PV1 VOLTAGE",
    DTA_PV2V,//PV2 VOLTAGE",
    DTA_PV1P,//PV1 CHARGE POWER",
    DTA_PV2P,//PV2 CHARGE POWER",
    DTA_CG1C,//CHARGE CURRENT 1", 
    DTA_CG2C,//CHARGE CURRENT 2", 
    DTA_AOAP,//AC OUTPUT ACTIVE POWER (W)", 
    DTA_AOVA,//AC OUTPUT APPARENT POWER (AV)",
    DTA_ACAP,//AC CHARGE POWER",
    DTA_ACVA,//AC CHARGE APPARENT POWER",
    DTA_BPKV,//BATTERY PACK VOLTAGE",
    DTA_ACOV,//AC OUTPUT VOLTAGE",  
    DTA_ACOF,//AV OUTPUT FREQUENCY",
    DTA_DCOV,//DC OUTPUT VOLTAGE", 
    DTA_LDPP,//LOAD POWER PERCENT", 
    DTA_OUTC,//OUTPUT CURRENT", 
    DTA_AIPP,//AC INPUT POWER",
    DTA_FLTB,//FAULT BIT", 
    DTA_WRNB,//WARNING BIT",  
    DTA_ACDE,//AC CHARGE ENERGY TODAY (24HRS)",   
    DTA_ACTE,//AC CHARGE ENERGY TOTAL",  
    DTA_BDDE,//BATTERY DISCHARGE ENERGY TODAY (24 HRS)",  
    DTA_BDTE,//BATTERY DISCHARGE ENERGY TOTAL",
    DTA_AODE,//AC OUTPUT ENERGY TODAY (24 HRS)",
    DTA_AOTE,//AC OUTPUT ENERGY TOTAL", 
    DTA_BTNP,//BATTERY NET POWER (SIGNED)", 
    DTA_BOCF,//BATTERY OVER-CHARGE FLAG 0=NO"    
	DTA_COUNT
};

enum
{
    DIA_BUSV,// "DC BUS VOLTAGE",
    DIA_ACIV,// "AC INPUT VOLTAGE",TYPE_UINT16},
    DIA_ACIF,// "AV INPUT FREQUENCY",
    DIA_IVTT,// "DC-AC INVERTER TEMPERATURE",
    DIA_CVTT,// "DC-DC CONVERTER TEMPATURE",TYPE_UINT16},
    DIA_BPTV,// "BATTERY PORT VOLTAGE",
    DIA_BBSV,// "BATTERY BUS VOLTAGE",TYPE_UINT16},
    DIA_WKTM,// "WORK TIME TOTAL",
    DIA_BK1T,// "BUCK 1 TEMPATURE",TYPE_UINT16}, 
    DIA_BK2T,// "BUCK 2 TEMPATURE",TYPE_UINT16},
    DIA_INTC,// "INVERTER CURRENT",
    DIA_AIAP,// "AC INPUT POWER",
    DIA_AIVA,// "AC INPUT APPARENT POWER",
    DIA_WRNV,// "WARNING VALUE",
    DIA_DVTC,// "DEVICE TYPE CODE",
    DIA_DVCS,// "DEVICE SELF CHECK STEP: 1:PV1 | 2:PV2 | 3: AC POWER",
    DIA_PRLM,// "PRODUCTION LINE MODE 1 | 2 | 3",
    DIA_CPOK,// "CONSTANT POWER OK FLAG",
    DIA_P1DE,// "PV1 ENERGY TODAY (24HRS)",
    DIA_P1TE,// "PV1 ENERGY TOTAL",
    DIA_P2DE,// "PV2 ENERGY TODAY (24HRS)",
    DIA_P2TE,// "PV2 ENERGY TOTAL",TYPE_UINT16},
    DIA_ABCC,// "AC CHARGE BATTERY CURRENT",
    DIA_ACDP,// "AC DISCHARGE POWER",
    DIA_ADVA,// "AC DISCHARGE APPARENT POWER",
    DIA_BDAP,// "BATTERY DISCHARGE ACTIVE POWER",
    DIA_BDVA,// "BATTERY DISCHARGE APPARENT POWER",
    DIA_MCFS,// "MPPT CHARGER FAN SPEED %",
    DIA_IVFS,// "MPPT CHARGER FAN SPEED %",TYPE_UINT16},
    DIA_CCRT,// "TOTAL CHARGE CURRENT",
    DIA_DCCT,// "TOTAL DISCHARGE CURRENT",
    DIA_DCDE,// "DISCHARGED ENERGY TODAY (24HRS)",
    DIA_DCTE,// "DISCHARGED ENERGY TOTAL",
    DIA_PCCR,// "PARALLEL SYSTEM CHARGE CURRENT"

	DIA_COUNT
};


#elif defined(PUMP_PROJECT)||defined(CAMP_PROJECT)

#ifdef DC_PUMP_SUPPORT
enum
{
	DTA_MFRQ,
	//DTA_BUSV,
	DTA_OUTV,
	DTA_OUTC,
	DTA_OUTP,
	//DTA_MTRQ,
	DTA_MSPD,
	//DTA_AWCM,
	DTA_TNKF,
	DTA_WLLE,
	DTA_SPWR,
	//DTA_SYST,
	DTA_COUNT
};
enum
{
	DIA_CV01,
	DIA_CV02,
	DIA_CV03,
	DIA_CV04,
	DIA_COUNT
};

#else
enum
{
	DTA_BATP,
	DTA_INPP,
	DTA_OUTP,
	DTA_AENG,
	DTA_PCKV,
	DTA_PCKC,
	DTA_RSOC,
	DTA_RCAP,
	DTA_FCCP,
	DTA_PCKT,
	DTA_COUNT
};

enum
{
	DIA_CV01,
	DIA_CV02,
	DIA_CV03,
	DIA_CV04,
	DIA_COUNT
};

#endif
#endif
	
typedef struct Prop
{
	char *prop;//[PROP_LENGTH];
	uint8_t* value;
	char *meta;//[META_LENGTH];
	uint32_t data_type;//  0 :uint   1:str  2:int
}GATTPROP_Def;

typedef struct GATT1
{
	GATTPROP_Def att[ATT_COUNT]; 
	GATTPROP_Def cmd[CMD_COUNT]; 
	GATTPROP_Def sts[STS_COUNT]; 
	GATTPROP_Def dta[DTA_COUNT]; 
	#ifndef DC_PUMP_SUPPORT
	GATTPROP_Def dia[DIA_COUNT]; 
	#endif
}GATT1_Def;


typedef struct numProp
{
    char prop[PROP_LENGTH];
    uint16_t* value;
    char meta[META_LENGTH];
    char val_type;//  0 :uint   1:str  2:int
    uint8_t *meta_ctrl;	
}NUMPROP_Def;



/* Êï∞ÊçÆÁªìÊûÑ
    {
        "v_c4",
        1000,
        "This is the voltage of cell 4 in mV"
    }
*/

typedef struct textProp
{
    char prop[PROP_LENGTH];
    char *value;
    char meta[META_LENGTH];
    char val_type;	
    uint8_t *meta_ctrl;		
}TXTPROP_Def;

/* Êï∞ÊçÆÁªìÊûÑ
    {
        "ppid",
        "12AH00001111"",
        "This is the PAYG_TypeDef ID of device"
    }
*/

#define NUMBER_OF_ATT 3
#define NUMBER_OF_STS 16
#define NUMBER_OF_STA 15


typedef struct GATT
{
	uint8_t num_att ;
	TXTPROP_Def  att[NUMBER_OF_ATT]; // array of attributes of length num_att

	uint8_t num_sts ;
	NUMPROP_Def sts[NUMBER_OF_STS]; // array of state variables

	uint16_t num_sta ;
	NUMPROP_Def sta[NUMBER_OF_STA]; // array of status numeric values

}GATT_Def;

enum
{
	LIST_ATT,
	LIST_CMD,
	LIST_STS,
	LIST_DTA,
	LIST_DIA,
	LIST_COUNT
};


#ifdef NEW_GATTVERSION

enum
{
	DT_TYPE_ATT=0,
	DT_TYPE_STS=2,
	DT_TYPE_STA
};

#else
enum
{
	DT_TYPE_ATT,
	DT_TYPE_STS,
	DT_TYPE_STA
};
#endif

/*enum
{
//ATT
	ATT_OPID,
	ATT_PPID,
	ATT_REV,
	ATT_ITEM_COUNT
};
enum
{	
	STA_BATP,//CHARGE_POWER
	STA_INPP,//SOLAR GENERATION
	STA_OUTP,//LOAD POWER
	STA_AENG,//ACCU_ENERGY_OUTPUT
	
	STA_CV01,//CELLVOLTAGE1
	STA_CV02,//CELLVOLTAGE2
	STA_CV03,//CELLVOLTAGE3
	STA_CV04,//CELLVOLTAGE4
	
	STA_PCKV,//PACK BATTERYVOLTAGE 
	STA_PCKC,//PACK BATTERYCURRENT 
	STA_PCKT,//PACK BATTERYTEMPERATURE 
	STA_RESERVED1,
	
	STA_PLAT,//[16],//GPSCORDLAT
	
	STA_PLON,//[16],//GPSCORDLON
	STA_TIME,//[20],//DEVICETIMER
	STA_GPSS,//GPRSSIGNALSTRENGTH
	STA_GPFC,//GPRS CONNCT FAILURE COUNT


	#ifdef PUMP_PROJECT
	STA_MFRQ,
	//STA_OUTP,
	STA_OUTV,
	STA_INPV,
	
	STA_OUTC,
	STA_MTRQ,
	STA_MSPD,
	STA_MTMP,
	
	STA_RUNS,
	//STA_RESERVED,
	#endif
};*/

typedef struct GATT_PARA
{
	//att
	uint8_t opid[20];
	uint8_t ppid[32];
	uint8_t rev[8];
	
	//sts
	uint16_t rcrd;//Remaining_PAYG_Days
	uint16_t trhd;//Days have been running
	uint16_t tpgd;//PAYG_Days 
	uint16_t pgst;//PAYG_State 
	
	uint16_t ocst;//Output_Control_State 
	uint16_t sstc;//System_Status_Code 
	uint16_t rsoc;//Relative_SOC 
	uint16_t rcap;//Remaining_Capacity 
	
	uint16_t fccp;//Full_Charge_Capacity 
	uint16_t rdbk;//Run_Days_Backup 
	uint16_t acyc;//Accu_Cycles 
	uint16_t reserved0;// 
	
	uint8_t pubk[32];//PAYG_Security_Hash_Top
	uint16_t gstw;//GPRS Sleep Time Period
	uint16_t gctw;//GPRS Connection Time Period
	
	uint16_t hbfq;//GPRS Sleep Time Period
	uint16_t rptm;//GPRS Connection Time Period
	
	//sta
	uint16_t batp;//Charge_Power
	uint16_t inpp;//Solar Generation
	uint16_t outp;//Load Power
	uint16_t aeng;//Accu_Energy_Output
	
	uint16_t cv01;//CellVoltage1
	uint16_t cv02;//CellVoltage2
	uint16_t cv03;//CellVoltage3
	uint16_t cv04;//CellVoltage4
	
	uint16_t pckv;//Pack BatteryVoltage 
	uint16_t pckc;//pack BatteryCurrent 
	uint16_t pckt;//pack BatteryTemperature 
	uint16_t reserved1;
	
	uint8_t plat[16];//GPSCordLat
	
	uint8_t plon[16];//GPSCordLon
	//uint8_t time[20];//DeviceTimer
	uint16_t gpss;//GPRSSignalStrength
	uint16_t gpfc;//GPRS Connct Failure Count


	#ifdef PUMP_PROJECT
	uint16_t mfrq;
	//uint16_t outp;
	uint16_t outv;
	uint16_t inpv;
	
	uint16_t outc;
	uint16_t mtrq;
	uint16_t mspd;
	uint16_t mtmp;
	
	uint16_t runs;
	//uint16_t reserved;
	#endif	
	
}GATT_PARA_Def;


typedef struct GATT_META
{
	//att
	uint8_t opid;
	uint8_t ppid;
	uint8_t rev;
	//sts
	uint8_t rcrd;//Remaining_PAYG_Days
	uint8_t trhd;//Days have been running
	uint8_t tpgd;//PAYG_Days 
	uint8_t pgst;//PAYG_State 
	uint8_t ocst;//Output_Control_State 
	
	uint8_t sstc;//System_Status_Code 
	uint8_t rsoc;//Relative_SOC 
	uint8_t rcap;//Remaining_Capacity 
	
	uint8_t fccp;//Full_Charge_Capacity 
	uint8_t rdbk;//Run_Days_Backup 
	uint8_t acyc;//Accu_Cycles 
	uint8_t pubk;//PAYG_Security_Hash_Top
	uint8_t gstw;//GPRS Sleep Time Period

	uint8_t gctw;//GPRS Connection Time Period
	uint8_t hbfq;//GPRS Connection Time Period
	uint8_t rptm;//GPRS Connection Time Period

	//sta
	uint8_t batp;//Charge_Power
	uint8_t inpp;//Solar Generation
	uint8_t outp;//Load Power
	uint8_t aeng;//Accu_Energy_Output
	uint8_t cv01;//CellVoltage1
	uint8_t cv02;//CellVoltage2
	uint8_t cv03;//CellVoltage3
	
	uint8_t cv04;//CellVoltage4
	uint8_t pckv;//BatteryVoltage 
	uint8_t pckc;//BatteryCurrent 
	uint8_t pckt;//BatteryTemperature 
	uint8_t plat;//GPSCordLat
	uint8_t plon;//GPSCordLon
	//uint8_t time;//DeviceTimer
	uint8_t gpss;//GPRSSignalStrength
	
	uint8_t gpfc;//GPRS Connct Failure Count

	//uint8_t reserved[1];

	#ifdef PUMP_PROJECT
	uint8_t mfrq;
	//uint8_t outp;
	uint8_t outv;
	uint8_t inpv;
	
	uint8_t outc;
	uint8_t mtrq;
	uint8_t mspd;
	uint8_t mtmp;
	
	uint8_t runs;
	//uint8_t reserved1;
	#endif	
}GATT_META_Def;


void GattInit(void);
uint8_t* GattAllFieldJsonMerge(void);
uint8_t* GattDtTypeFieldJsonMerge(uint8_t dt_type,uint8_t meta);
uint8_t* GattSingleFieldMerge(uint8_t *tag_str);
uint8_t *GattGetJsonBuff(void);
void GattSetUplinkData(uint8_t *buf);
void GattGetOpid(uint8_t*opid);
GATTPROP_Def *GattGetListInfor(uint8_t dt_list,uint8_t *count);
uint8_t GattGetListProp(uint8_t dt_list,uint8_t id,uint8_t *prop);


void GattSetPpid(uint8_t*pid,uint8_t size);
void GattSetOpid(uint8_t*opid,uint8_t size);
void GattSetVersion(uint8_t*rev,uint8_t size);
void GattSetRmPayDays(uint16_t days);//Remaining_PAYG_Days
void GattSetRunDays(uint16_t days);//Days have been running
void GattSetPaygDays(uint16_t days);//PAYG_Days
void GattSetPaygState(uint16_t state);//PAYG_State
void GattSetOutputCtrlState(uint16_t state);//Output_Control_State
void GattSetSysStatusCode(uint16_t state);//System_Status_Code
void GattSetRelativeSOC(uint16_t soc);//Relative_SOC
void GattSetRmCap(uint16_t cap);//Remaining_Capacity
void GattSetFullChrgeCap(uint16_t cap);//Full_Charge_Capacity
void GattSetRunDayBackup(uint16_t day);//Run_Days_Backup
void GattSetAccuCyc(uint16_t cyc);//Accu_Cycles
void GattSetHashTop(uint8_t*hash,uint8_t size);//PAYG_Security_Hash_Top
void GattSetGprsSleepTime(uint16_t time);//GPRS Sleep Time Period
void GattSetGprsConnectTime(uint16_t time);//GPRS Connection Time Period
void GattSetChargePower(uint16_t bat);//Charge_Power
void GattSetSolarGeneration(uint16_t solar);//Solar Generation
void GattSetLoadPower(uint16_t pwr);//Load Power
void GattSetAccuEnergy(uint16_t energy);//Accu_Energy_Output
void GattSetCellVolt1(uint16_t cell_v);//CellVoltage1
void GattSetCellVolt2(uint16_t cell_v);//CellVoltage2
void GattSetCellVolt3(uint16_t cell_v);//CellVoltage3
void GattSetCellVolt4(uint16_t cell_v);//CellVoltage4
void GattSetCellVolt5(uint16_t cell_v);//CellVoltage5
void GattSetCellVolt6(uint16_t cell_v);//CellVoltage6
void GattSetCellVolt7(uint16_t cell_v);//CellVoltage7



void GattSetBattVolt(uint32_t volt);//BatteryVoltage
void GattSetBattCurrent(uint16_t current);//BatteryCurrent
void GattSetBattTemp(uint16_t temp);//BatteryTemperature
void GattSetGpsCordLat(uint8_t *lat);//GPSCordLat  Œ≥∂»
void GattSetGpsCordLon(uint8_t* lon);//GPSCordLon  æ≠∂»
void GattSetDeviceTime(uint8_t *time,uint8_t size);//DeviceTimer
void GattSetGprsSignal(uint16_t csq);//GPRSSignalStrength
void GattSetHeartbeat(uint16_t freq );//heartbeat
void GattSetReportMode(uint16_t mode);//report mode
void GattSetGprsConnectFailCnt(uint16_t csq);//GPRS Connct Failure Count
void GattGetPpid(uint8_t*ppid);

void GattSetGprsConnectFailCnt(uint16_t cnt);
void GattSetGprsRssi(uint16_t csq);//GPRSSignalStrength
uint16_t GattGetRelativeSOC(void);//Relative_SOC

uint16_t GattGetAccuEnergy(void);//Accu_Energy_Output
uint16_t GattGetBattCurrent(void);//BatteryVoltage
uint16_t GattGetBattVolt(void);//BatteryVoltage
uint16_t GattGetLoadPower(void);//Load Power
uint16_t  GattGetBattTemp(void);//BatteryTemperature

void GattSetCmdUplinkData(uint8_t *buf);

#ifdef PUMP_PROJECT
void GattSetPumpMfrqSet(uint16_t mfrq);
void GattSetPumpOutputPwr(uint16_t outp);
void GattSetPumpOutputVolt(uint16_t outv);
void GattSetPumpInputVolt(uint16_t inpv);
void GattSetPumpOutC(uint16_t outc);
void GattSetPumpMTorque (uint16_t mtrq);
void GattSetPumpMSpeed(uint16_t mspd);
void GattSetPumpMTemp(uint16_t mtmp);
void GattSetPumpRunState(uint16_t runs);
#endif	

uint8_t* GattMultiFieldMerge(void);

uint8_t GattGetData(uint8_t list,uint8_t id,uint8_t*data);
uint8_t GattSetData(uint8_t list,uint8_t id,uint8_t*data);
GATT1_Def* GattGetList(void);



#endif


