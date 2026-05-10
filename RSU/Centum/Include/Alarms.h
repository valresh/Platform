#pragma once

#ifdef Y_DATA_EXPORTS
#define Y_DATA_API __declspec(dllexport)
#else
#define Y_DATA_API __declspec(dllimport)
#endif

#include "./AlarmEntry_.h"

//EnumMESS(NR  , 0,"NR"  )
//EnumMESS(IOPP, 1,"IOP" )// Сигнализация о верхнем предельном значении открытого входа

#undef EnumMESS
#define EnumMESS(ID,Numb,Name, CodeYoko ) \
	A_##ID = 0x0000000000000001LL << Numb,

enum AlarmFlags : __int64 // Флаги ALRM м.б. несколько одновременно
  {
  #include "./AlarmMess_.hpp"
  //A_IOP  = A_IOPP|A_IOPM, //Input Open Alarm
	A_HHLL = A_HH|A_LL,
  A_HL   = A_HI|A_LO,
  A_VEL   = A_VELP|A_VELM,
  A_DV   = A_DVP|A_DVM,
  A_MHL   = A_MHI|A_MLO,
  };

//номера сигнализаций
#undef EnumMESS
#define EnumMESS(ID,Numb,Name, CodeYoko ) ID##_ALRM = Numb,
enum AlarmNumbers
{
#include "./AlarmMess_.hpp"
};


struct Y_DATA_API CY_ALARM_BASE
{
  __int64 State;
protected:
  struct CAlarmBase * pBlk;
public:
  char * BlockName;
  int * pAlarmLevel;
  struct CY_Base * pObj;
  BYTE *pCALI;

  CY_ALARM_BASE();
  void SendAlarm( __int64 Alarm, bool On );
  void SEND_ON( __int64 Flag, double Value );
  void SEND_OFF( __int64 Flag );
  void SetBlk( struct CAlarmBase * _pBlk );
protected:
  void Set_ALRM();
  void CreateTextAlrm();
};

struct Y_DATA_API CY_ALARM : CY_ALARM_BASE
{    
  inline bool Input_Open_Alarm( int Status );
  inline void HHLL_ON( );
  inline void HHLL_OFF( );
  inline void HL_ON( );
  inline void HL_OFF( );
  inline void VEL( double dt );
  inline void DEV( double dt );
  __int64 Possible;
  int * pInput_Open_alarm;
  int * pHHLL;
  int * pHL;
//
  double * pSH;
  double * pSL;
  const double * pH_Limit;
  const double * pL_Limit;
  double * pPV;
  double * pSV;
  double * pHH;
  double * pLL;
  double * pH;
  double * pL;
  double * pHYS_HL;
//
  double * pVL;
  int * pSides_VL;
  double * pdT_V_scan;
  double * pHYS_VL;
//
  int * pDeviation_DL;
  double * pDL;
  double * pGAIN_DL;
  double * pTime_DL;
  double * pHYS_DL;
//
  double PVlast;
  double V_Time;
//
  double SVlast;
  double r;
//-------------------------------------------
  double * pMV;
  double * pMH;
  double * pML;
  double * pMSH;
  double * pMSL;
	int * pHL_OUT;		// 0 - NO 1 - H 2 - L 3 - HL 
	double * pHYS_OUT;
  int * AF;
  double GAINin;
  double BIASin;
  CY_ALARM();
  void TestIn( struct CSignal * IN, double dt, int & Status );
  void TestInScaled( struct CSignal * IN, double dt, int & Status, double A );
  void TestOut( );
  void AlarmPERR( bool Is );
};
