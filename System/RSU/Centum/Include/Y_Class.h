#pragma once
#include <Y_export.h>
#include <Y_Signal.h>

#include "./Alarms.h"
//#include <Y_Graf.h>
#include <yokoheaders.h>
#include <MemoryServiceY.h>
#include <YBridge2SysParam.h>

#undef OUT
#undef IN


template<typename Class, typename Struct>
CY_Base * CreateObjectsImpl( char * Name, int number, int domain )
{
  Struct * pParms = (Struct*)KMemoryServiceY::Instance().CreateObjectsStruct ( Name, Struct::TypeID, number, domain );
  pParms->mode.M = SMode::MAN;
  Class * pClass = new Class ( pParms );
  pClass->Y_Type = Struct::TypeID;
  return pClass;
}

template<typename Class, typename Struct>
CY_Base * CreateIOsImpl( char * Name, int number, int domain )
{
  Struct * pParms = (Struct*)KMemoryServiceY::Instance().CreateIOsStruct ( Name, Struct::TypeID, number, domain );
  pParms->mode.M = SMode::MAN;
  Class * pClass = new Class ( pParms );
  pClass->Y_Type = Struct::TypeID;
  return pClass;
}

template<typename Class, typename Struct>
CY_Base * CreateBOpcIOsImpl( char * Name, int number, int domain )
{
  Struct * pParms = (Struct*)KMemoryServiceY::Instance().CreateIOsStruct ( Name, Struct::TypeID, number, domain );
  Class * pClass = new Class ( pParms );
  pClass->Y_Type = Struct::TypeID;
  return pClass;
}

#define CREATE(Class,Struct) \
  Class( Struct * W );\
  static CY_Base * Create( char * Name, int number, int domain )\
		{\
    return CreateObjectsImpl<Class, Struct>( Name, number, domain );\
		};\
	char * GetClass(){ return #Class; };

#define CREATE_TYPE(Class,Struct) \
  Class( Struct * W );\
  static CY_Base * Create_##Struct( char * Name, int number, int domain )\
		{\
    return CreateObjectsImpl<Class, Struct>( Name, number, domain );\
		};

#define GET_CLASS(Class) \
	char * GetClass(){ return #Class; };

#define CREATE_FF(Class,Struct) \
  Class( Struct * W );\
  static CY_Base * Create( char * Name, int number, int domain )\
		{\
    return CreateIOsImpl<Class, Struct>( Name, number, domain );\
		};\
	char * GetClass(){ return #Class; };

#define CREATE_BOPC(Class,Struct) \
  Class( Struct * W );\
  static CY_Base * Create( char * Name, int number, int domain )\
{\
  return CreateBOpcIOsImpl<Class, Struct>( Name, number, domain );\
};\
  char * GetClass(){ return #Class; };
/////////////////////////////////////////////////////////////

enum EYKeys
{
  eYKeyNULL = 0,
  eYKeyLC64begin = 0x5963241A,
  eYKeyLC64end = 0x742F5439,
  eYKeyCALCUbegin = 0x6595b641,
  eYKeyCALCUend = 0x44ccf1df,
  eYKeyST16begin = 0x37482ABF,
  eYKeyST16end = 0xF5729715,
};

struct Y_DATA_API CY_Base_W
	{
	int xInfo, yInfo;
	bool ShowInfo;
	};

struct CDrawData
  {
  HWND hWnd;
  HDC hDC;
  HFONT hFont;
  HFONT hFontInfo;
  HPEN hPenInfo;
  };

struct Y_DATA_API CY_Base : public CY_Base_W
{
  bool BreakPoint;
  enum
  {
    reserveNameLength = 20,
  };
  enum EESCA_ScanPeriod
  {
    ESCA_ScanPeriod_basic = 'S',
    ESCA_ScanPeriod_medium = 'M',
    ESCA_ScanPeriod_high = 'H',
  };
  static bool s_ReadingTunningData;
  static bool s_flagReadAllTunningValues;
  static const double s_ControlPeriod;
  static bool s_flagBasicScanControl;
  static DWORD s_nCalcSteps;
  static bool s_IsBlk;
  static __time64_t s_ModelFullTimeT;// Модельное полное время 
private:
  CAlarmBase::tFlagUse_ *m_pFlagUseInW;
  void Clear();
public:
  struct CY_DI* GetNextDI( CY_Base *pObj );
public:
	char Name[64*4];
  char szAkaObjName[128*4];
	char Model[16*4];
  bool IsWork;
  bool IsLimits;
  struct SMode * pMode;
  bool * pFound;
  char * pComm;
  CY_DR * pDR;
  BYTE GeomType;
  BYTE GeomSymbol;
  int x, y;
	RECT rc;
  int sequenceNumberInDR;//номер последовательности на DR, нумерация с 1
                         //0 - типа PIO, AREAIN, AREAOUT
                         //-100 - PIO с подключенным IN и вызовом Work сразу за блоком, формирующим ему вход
                         //-200 - PIO с подключением из MC-NE, с вызовом из него же, обычно OUT2 и OUT3
  BOOL bConnectedByACS;//использовалась при подключении при чтении Y.CSV
  char szNumber[24*4];//%BLxxxx
  int firstConnectionK, lastConnectionK;
  bool bExecutedAtStep;
  EESCA_ScanPeriod ESCA;
  int m_nBasicControlPeriodCounter;
  char szUPPL[24*4];
//
  BYTE * pClassBase;
  struct CYParmInfo * ClassParmInfo;
  int kClassParmInfo;
  struct CYVarInfo * ClassVarInfo;
  int kClassVarInfo;
  BYTE * pClass_P;
  int L_Class_P;
//
	SYParam *Info[10];
	int kInfo;
	DWORD Y_Type;
  BYTE Used;
  BYTE Level;
  BYTE XAF;//Y - IOP or IOP- detection is disabled.
          //N - IOP or IOP- detection is enabled
  int PV_BAD;// 0 - хорош
  enum eControllerType : BYTE
  {
    ectRSU = 0,
    ectPAZ = 1,
  };
  eControllerType controllerType;
  enum eVarType : BYTE
  {
    evtP = 0,
    evtWork = 1,
    evtSignal = 2
  };
///////////////////////////////////////////
private:
  BYTE * pInitialClass_W;//для отслеживания изменения параметров
  int L_InitialClass_W;
public:
	CY_Base();
	CY_Base( bool NoAdd );
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
  virtual void OnReadTunningData(){}
  virtual void OnFailedTunningAssign( LPCSTR pszParm, LPCSTR pszValue ){}
	virtual void Work ( double dt );
  virtual void Init( );
  virtual void LinkToDI();
  virtual int Prepare0();
  virtual int Prepare1();
  virtual void ResetAlarms();
  virtual bool SetLimits( );
  // сохранение и восстановление параметров, состояния
  virtual int SaveState();
  virtual int RestoreState( char * StrName );
  virtual int SetData( int TypeData, void * pData );
  virtual double* GetValueAddr();
  virtual char* GetModelName();
  //новое для параметров
  virtual int GetParams( KYBridge2SysParam &params );
  //
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
  virtual void OnParamsInitialized();//восстановлена проектные параметры из CY_MainLogic::RestAll()
  virtual int ChangedParamsSave( IYokoStateSer* pStateSer );
  virtual int ChangedParamsRestore( IYokoStateSer* pStateSer );
  //
	virtual bool CreatePSMGraf( struct CGrafParm ** GP );
	virtual char * GetClass();
	void ShowCENTUM();
	virtual void SetObjName( int NumbFCS = 0, char chStationType = 'F' );
  virtual void SetObjName( const char *pTextedName );
  virtual bool Draw ( CDrawData * pDD, int x, int y );
  virtual bool ReadParm( char * ParmName, char * ParmValue, BYTE * pData );
  virtual void NoRead ( int * pVar, char * Name, char * Fmt );
  virtual bool GetVar( char * VarName, void ** pVar, void ** pTuneVar, char * Type, eVarType * Work, CAlarmBase::tFlagUse_ **ppFlagUse = NULL, USHORT *pVarSize = NULL );
  virtual void SetParmList( );
  virtual bool SaveParms(std::fstream& hFile );
  virtual bool RestParms(std::fstream& hFile );
  virtual bool CallSA(LPCSTR pszParam){return false;}
  void InitFlagUse( CAlarmBase::tFlagUse_ &FlagUse );
  CAlarmBase::tFlagUse_ *GetFlagUse();
  virtual char TestCNCT( LPSTR pConn );
  virtual void OnAssigned( void *pAddr ){}
  virtual void OnNoAssignedByFalseState( void *pAddr, LPCVOID pCanByValue ){}
  virtual bool TransferConnection( LPCSTR pszFieldInName );
  virtual bool SetResetAlrmFromExtern( int alrmNumber, bool bSet )//изменение сигнализации из вне (для PBS5C PBS10C)
  {
    return false;
  }
  //
  virtual CBase* GetStructure();
  virtual bool EnumUsedObj( DWORD &pos, LPCSTR *ppName, CY_Base **ppObj );
  enum ELActLC64
  {
    elnoL = -3,
    elHZ = 0,
    elyesL = 4,
  };
  virtual ELActLC64 IsLActionInLC64( LPCSTR pszAct ){ return elHZ;}
  NEWY
};
////////////////////////////////////////////////////////////////////////
// Начало //////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

enum eAuxOutDestType //SUB
{
  eaodt_DataItem = 0,
  eaodt_ProcessIO = 1,
};

enum eOutputSignalConvertion//PID FFSUM MLD-SW PI-HLD
{
  LINEAR = 0,
  PW,
  SUBSYS,
  SUBWFT
};

struct Y_DATA_API CY_AREAIN_W
	{
	double PV;
	int iPV;
	};

struct Y_DATA_API CY_AREAIN : public CY_Base, public CY_AREAIN_W
{
	CSignal OUT;
  enum eType { AREAIN, AREAOUT };
  eType Type;
  CY_AREAIN( char * Name, int Type, int number, int domain );
	CSignal * IN;
  double GAIN, BIAS;
  char Ref[32*4];
  void * pRefVar;
  char TypeRefVar;
	bool Is;
  CY_Base *m_pObjX;
	static CY_Base * Create( char * Name, int Type, int number, int domain )
	{
    CY_AREAIN * pClass = new CY_AREAIN ( Name, Type, number, domain );
		return pClass;
	};
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  static void InitParm();
  virtual void SetParmList( );
  virtual bool GetVar( char * VarName, void ** pVar, void ** pTuneVar, char * Type, eVarType *Work, CAlarmBase::tFlagUse_ **ppFlagUse /*= NULL*/, USHORT *pVarSize /*= NULL*/ );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
	NEWY
};
/////////////////////////////////////////////////////////////////

struct Y_DATA_API CY_ARITH_W
{
  double RV;
  double RV1, RV2;
  int iRV1, iRV2;
  int OutStatus;
  double dCPV;
};

struct Y_DATA_API CY_ARITH : public CY_Base, public CY_ARITH_W
{
  CARITH * W;
	enum eType { Null, eADD, eDIV, eNOT, eOR, eAND, eGT, eGE, eSQRT, eMUL, eEQ };
	eType TypeOP;
	CSignal * IN;
	CSignal * Q01;
	CSignal * Q02;
	CSignal OUT, SUB;
  bool CPV_RV;
  CY_ARITH( char * Name, eType TypeOP, int number, int domain );
  static CY_Base * Create( char * Name, int TypeOP, int number, int domain )
	{
    CY_ARITH * pClass = new CY_ARITH ( Name, (eType)TypeOP, number, domain );
		return pClass;
	};
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  void ADD();
  void DIV();
  void NOT();
  void OR();
  void AND();
  void GT();
  void GE();
  void SQRT();
  void MUL();
  void EQ();
  void GetIN();
  void iGetIN();
  void dGetIN();
  static void InitParm();
  virtual void SetParmList( );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
	NEWY
};
//////////////////////////////////////////////////////////////////////////

struct Y_DATA_API CY_LD : public CY_Base
{
  double prevRV;
  CSignal * IN;
  CSignal OUT;
  CSignal SUB;
  CREATE(CY_LD,CLD)
  CLD * W;
  virtual CSignal * GetOUT ( char * Name );
  virtual CSignal ** GetIN ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  NEWY
};
///////////////////////////////////////////////////////////////

struct Y_DATA_API CY_AS_HML_W
{
  enum ENCCS
  {
    NC,
    C,
  };
  double dMV;
	int SEL;
	double PSW;// PSW Preset MV switch 0, 1, 2, 3 0
	double MVn;
};

struct Y_DATA_API CY_AS_HML : public CY_Base, public CY_AS_HML_W
{
	CAS_COMM * W;
	enum eType { L, M, H };
	eType Type;
	bool Bad1, Bad2, Bad3;
	CSignal * IN1;
	CSignal * IN2;
	CSignal * IN3;
	CSignal * INT;
//
	CSignal OUT;
  CY_IN ObrIn;
  CY_OUT ObrOut;
  CY_AS_HML( char * Name, int Type, int number, int domain );
	static CY_Base * Create( char * Name, int Type, int number, int domain )
	{
		CY_AS_HML * pCY_AS_HML = new CY_AS_HML( Name, Type, number, domain );
		return pCY_AS_HML;
	};
	char * GetClass(){ return "CY_AS_HML"; };
	bool Limits;
	bool UseSW_IN;
	int SW_TO;
	CY_ALARM Alarm;
	virtual CSignal ** GetIN ( char * Name );
	virtual CSignal * GetOUT ( char * Name );
//
	void Signal_selection( );
	void Automatic_control_output_computation( );
	void Control_output_action();
	void Output_pushback();
	void Initialization_manual();
	void SS_H( );
	void SS_M( );
	void SS_L( );
	virtual void Work ( double dt );
	void Clamp( int DataStatus );
  virtual void Init( );
  virtual void ResetAlarms();
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  virtual bool ReadParm( char * ParmName, char * ParmValue, BYTE * pData );
  virtual bool GetVar( char * VarName, void ** pVar, void ** pTuneVar, char * Type, eVarType * Work, CAlarmBase::tFlagUse_ **ppFlagUse = NULL, USHORT *pVarSize = NULL );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
	NEWY
};
/////////////////////////////////////////////////////////////////

struct Y_DATA_API CY_AVE_W
{
	double dCPVn;
};

struct Y_DATA_API CY_AVE : public CY_Base, public CY_AVE_W
	{
	CSignal * Q01;
	CSignal * Q02;
	CSignal * Q03;
	CSignal * Q04;
	CSignal * Q05;
	CSignal * Q06;
	CSignal * Q07;
	CSignal * Q08;
	CSignal OUT;
	CSignal SUB;
	CREATE(CY_AVE,CAVE)
  CAVE * W;
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
	NEWY
	};

/////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_BAND_W
{
  int iRV1, iRV2;
  int OutStatus;
};

struct Y_DATA_API CY_BAND : public CY_Base, public CY_BAND_W
{
  CBAND * W;
	CSignal * Q01;
	CSignal * Q02;
	CSignal OUT;
	CREATE(CY_BAND,CBAND)
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  void iGetIN();
  static void InitParm();
  virtual void SetParmList( );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
	NEWY
};

/////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_BNOT_W
{
  int iRV;
};

struct Y_DATA_API CY_BNOT : public CY_Base, public CY_BNOT_W
{
  CBNOT * W;
	CSignal * IN;
	CSignal OUT;
	CREATE(CY_BNOT,CBNOT)
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
	NEWY
};

//////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_BDSET_1C_W
{
  double O;
  double dT_V_scan;
};
struct Y_DATA_API CY_BDSET_ACT
{
  int ACT[16];
};
struct Y_DATA_API CY_BDSET_1C : public CY_Base, public CY_BDSET_1C_W, public CY_BDSET_ACT
	{
  CBDSET_1C * W;
//
	CSignal Out[16];
	int kOut;
//
	CREATE(CY_BDSET_1C,CBDSET_1C)
	virtual CSignal ** GetIN ( char * Name );
	virtual CSignal * GetOUT ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  static void InitParm();
  virtual void SetParmList( );
  virtual bool GetVar( char * VarName, void ** pVar, void ** pTuneVar, char * Type, eVarType * Work, CAlarmBase::tFlagUse_ **ppFlagUse = NULL, USHORT *pVarSize = NULL );
  virtual void OnAssigned( void *pAddr );
  NEWY
	};
//////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_BDSET_2C : public CY_Base, public CY_BDSET_ACT
{
  CBDSET_2C * W;
  //
  CSignal Out[16];
  int kOut;
  //
  CREATE(CY_BDSET_2C,CBDSET_2C)
  virtual CSignal ** GetIN ( char * Name );
  virtual CSignal * GetOUT ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  static void InitParm();
  virtual void SetParmList( );
  virtual bool GetVar( char * VarName, void ** pVar, void ** pTuneVar, char * Type, eVarType * Work, CAlarmBase::tFlagUse_ **ppFlagUse = NULL, USHORT *pVarSize = NULL );
  virtual void OnAssigned( void *pAddr );
  NEWY
};
/////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_BDSET_1L_W
{
  double _O;
  double dT_V_scan;
};
struct Y_DATA_API CY_BDSET_1L : public CY_Base, public CY_BDSET_1L_W, public CY_BDSET_ACT
{
  CBDSET_1L * W;
//
	CSignal Out[16];
	int kOut;
//
	CREATE(CY_BDSET_1L,CBDSET_1L)
	virtual CSignal ** GetIN ( char * Name );
	virtual CSignal * GetOUT ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  static void InitParm();
  virtual void SetParmList( );
  virtual bool GetVar( char * VarName, void ** pVar, void ** pTuneVar, char * Type, eVarType * Work, CAlarmBase::tFlagUse_ **ppFlagUse = NULL, USHORT *pVarSize = NULL );
  virtual void OnAssigned( void *pAddr );
  NEWY
};
/////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_BDSET_2L : public CY_Base, public CY_BDSET_ACT
{
  CBDSET_2L * W;
  //
  CSignal Out[16];
  int kOut;
  //
  CREATE(CY_BDSET_2L,CBDSET_2L)
  virtual CSignal ** GetIN ( char * Name );
  virtual CSignal * GetOUT ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  static void InitParm();
  virtual void SetParmList( );
  virtual bool GetVar( char * VarName, void ** pVar, void ** pTuneVar, char * Type, eVarType * Work, CAlarmBase::tFlagUse_ **ppFlagUse = NULL, USHORT *pVarSize = NULL );
  virtual void OnAssigned( void *pAddr );
  NEWY
};
//////////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_BDA_C : public CY_Base
{
  CBDA_C * W;
  //
  CSignal Out[16];
  int kOut;
  //
  CREATE(CY_BDA_C,CBDA_C)
  virtual CSignal ** GetIN ( char * Name );
  virtual CSignal * GetOUT ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  static void InitParm();
  virtual void SetParmList( );
  NEWY
};
//////////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_BDA_L : public CY_Base
{
  CBDA_L * W;
  //
  CSignal Out[16];
  int kOut;
  //
  CREATE(CY_BDA_L,CBDA_L)
  virtual CSignal ** GetIN ( char * Name );
  virtual CSignal * GetOUT ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  static void InitParm();
  virtual void SetParmList( );
  NEWY
};
/////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_CI_W
	{
  int iRV;
	};

struct Y_DATA_API CY_CI : public CY_Base, public CY_CI_W
	{
  CCI * W;
	CSignal * IN;
	CREATE(CY_CI,CCI)
	CY_DI * pDI[16];
  int * pPV[16];
  CY_DI * pWW;
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  virtual bool EnumUsedObj( DWORD &pos, LPCSTR *ppName, CY_Base **ppObj );
	NEWY
	};

/////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_CO_W
	{
  int iRV;
	int OUT1;
	};

struct Y_DATA_API CY_CO : public CY_Base, public CY_CO_W
	{
  CCO * W;
	CSignal OUT;
	CREATE(CY_CO,CCO)
	struct CY_DI * pDI[16];
  int *pPV[16];
  CY_DI * pWW;
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  virtual bool EnumUsedObj( DWORD &pos, LPCSTR *ppName, CY_Base **ppObj );
	NEWY
	};

/////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_DI : public CY_Base
{
  CY_DI( bool NoAdd );//использовать только как временный объект для поиска
  CY_DI( char * Name, int ID, int NumbFCS, int domain, int numb );
	static CY_DI * Create( char * Name, int ID, int NumbFCS, int domain, int numb )
  {
    CY_DI * pCY_DI = new CY_DI( Name, ID, NumbFCS, domain, numb );
    strcpy_s( pCY_DI->Name, Name );
		return pCY_DI;
	};
	union
	{
		C_Z * pZ;
		C_AN * pAN;
		C_WB * pWB;
		C_GS * pGS;
		C_SW * pSW;
		C_PR * pPR;
		C_OG * pOG;
    C_WW * pWW;
    C_RQ * pRQ;
    C_Y * pY;
    C_XW * pXW;
	};
  enum ePointType : char
  {
    eptUnknown = ' ',
    eptDigital = 'D',
    eptAnalog = 'A',
  };
  CY_DI * pParenWW4WB;
  int nBitWW4WB;
  int Numb;
  int NumbFCS;
  int domain;
  DWORD No;//последовательный номер в edf-ке
  bool bWrote2Csv;//был ли выведен в csv
  ePointType pointTypeZ;
  ePointType modelTypeZ;
  char IOZ;
  int Mes;
  char Comment[67*4];
  CSignal OUT;
  CSignal OUTD;
  CSignal * IN;
  int * pPV;
  double * pDPV;
  BYTE FlagUse;
  int AlarmLevel;
  CY_ALARM_BASE Alarm;
  char nameWithFcs[64];
  virtual void SetObjName( int NumbFCS, char chStationType = 'F' );
  virtual void SetObjName( const char *pTextedName );
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual void ResetAlarms();
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  virtual bool GetVar( char * VarName, void ** pVar, void ** pTuneVar, char * Type, eVarType *Work, CAlarmBase::tFlagUse_ **ppFlagUse /*= NULL*/, USHORT *pVarSize /*= NULL*/ );
  virtual CBase* GetStructure();
	NEWY
};

/////////////////////////////////////////////////////////
struct Y_DATA_API CY_CALCU_W
	{
	double EERL;// Error statement number ----- 0
	double ERRC;// (* 2) Error code ----- 0
  double dCPV;
	};

struct Y_DATA_API CY_CALCU_BASE : public CY_Base
{
  enum Processing_StartTiming
  {
    eOne_shotProcessing = 0,
    PeriodicExecution = 1,
  };
  int cVars, cOPs;
  void *pOPs, *pVars;
  void *pLocalVars;
  int cLocalVars;
  int nStepDuration;
  int m_LastWorkStep;
  enum eType 
  { 
    Simple,
    Character,
  }Type;
  char szAlgorithmFileName[64*4];
  CY_ALARM Alarm;
  CY_CALCU_BASE();
  virtual bool ReadParm( char * ParmName, char * ParmValue, BYTE * pData );
  virtual bool SaveParms( std::fstream &hFile );
  virtual bool RestParms(std::fstream &hFile );
  virtual bool GetVar( char * VarName, void ** pVar, void ** pTuneVar, char * Type, eVarType *Work, CAlarmBase::tFlagUse_ **ppFlagUse /*= NULL*/, USHORT *pVarSize /*= NULL*/ );
  virtual int GetParams( KYBridge2SysParam &params );
  virtual bool CreatePSMGraf( struct CGrafParm ** GP );
  void InitTrans();
  void WorkTranslation( double dt, SMode &mode );
  virtual int SaveState();
  virtual int RestoreState( char * StrName );
  virtual bool EnumUsedObj( DWORD &pos, LPCSTR *ppName, CY_Base **ppObj );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
public:
  bool SingleOP_exec( int &nOP, SMode *pMode, bool *pMathErr );
protected:
  void SetSkipAllOutTerminals();
};

struct Y_DATA_API CY_CALCU : public CY_CALCU_BASE, public CY_CALCU_W
{
  CCALCU * W;
	CSignal * IN;
	CSignal * Q01;
	CSignal * Q02;
	CSignal * Q03;
	CSignal * Q04;
	CSignal * Q05;
	CSignal * Q06;
	CSignal * Q07;
	CSignal Out[5];
	int kOut;
//	int Alg;
	char PID[64*4];
	char SEL[64*4];
  bool Start;
  struct CAlg * pAlg;
  int UseAlg;
  bool bSaveP0StoState;
  int m_stepActOn;
  int UseIn;//1 - Data_GAINin, 2 - Data_BIASin
  static bool s_globalUseInGainBias;//глобальное использование входных Gain Bias
//=============================================
  CREATE(CY_CALCU,CCALCU)
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
//////////////////////////////
  static void InitParm();
  virtual void SetParmList( );
  virtual void OnAssigned( void *pAddr );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
	NEWY
};

struct Y_DATA_API CY_CALCU_C : public CY_CALCU_BASE, public CY_CALCU_W
{
  CCALCU_C * W;
	CSignal * IN;
	CSignal * Q01;
	CSignal * Q02;
	CSignal * Q03;
	CSignal * Q04;
	CSignal * Q05;
	CSignal * Q06;
	CSignal * Q07;
	CSignal Out[5];
	int kOut;
  bool Start;
  int UseAlg;
  bool bSaveP0StoState;
//=============================================
  CREATE(CY_CALCU_C,CCALCU_C)
  double BIAS;
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
	NEWY
};

/////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_DLAY_W
{
	double Delay[61];
	double CPV;
	CY_DLAY_W();
	NEWY
};

struct Y_DATA_API CY_DLAY : public CY_Base, public CY_DLAY_W
{
  CDLAY * W;
	CREATE(CY_DLAY,CDLAY)
	CSignal * IN;
	CSignal OUT;
	CSignal SUB;
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
	NEWY
};

/////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_DLAY_C_W
{
  double Delay[61];
  double CPV;
  double waitSmpl;
  CY_DLAY_C_W();
};

struct Y_DATA_API CY_DLAY_C : public CY_Base, public CY_DLAY_C_W
{
  CDLAY_C * W;
  CREATE(CY_DLAY_C,CDLAY_C)
  CSignal * IN;
  CSignal OUT;
  CSignal SUB;
  virtual CSignal * GetOUT ( char * Name );
  virtual CSignal ** GetIN ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
  NEWY
};

/////////////////////////////////////////////////////////////////////

struct Y_DATA_API CY_DSET_W
{
	double O;
};

struct Y_DATA_API CY_DSET : public CY_Base, public CY_DSET_W
{
  CDSET * W;
	CSignal OUT;
	CREATE(CY_DSET,CDSET)
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
	NEWY
};
/////////////////////////////////////////////////////////////////////

struct Y_DATA_API CY_DSET_PVI_W
{
  double O;
};
//////////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_DSET_PVI : public CY_Base, public CY_DSET_PVI_W
{
  CDSET_PVI * W;
  CSignal * IN;
  CSignal OUT, SUB;
  CY_ALARM Alarm;
  CY_IN ObrIn;
  CREATE(CY_DSET_PVI,CDSET_PVI)
  virtual CSignal * GetOUT ( char * Name );
  virtual CSignal ** GetIN ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual void ResetAlarms();
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  NEWY
};
/////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_DSW_16_W
{
  int _State;
	CY_DSW_16_W();
	NEWY
};

struct Y_DATA_API CY_DSW_16 : public CY_Base, public CY_DSW_16_W
{
  CDSW_16 * W;
	CREATE(CY_DSW_16,CDSW_16)
	CSignal OUT;
  double * pSD[16];
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
	NEWY
};
//////////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_DSW_16C_W
{
  int _prevCPV;
  CY_DSW_16C_W();
};

struct Y_DATA_API CY_DSW_16C : public CY_Base, public CY_DSW_16C_W
{
  CDSW_16C * W;
  CREATE(CY_DSW_16C,CDSW_16C)
  char * pSD[16*4];
  virtual CSignal * GetOUT ( char * Name );
  virtual CSignal ** GetIN ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  NEWY
};
/////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_FF_AI : public CY_Base
{
  CFF_AI * W;
  CREATE_FF(CY_FF_AI,CFF_AI)
  CSignal OUT;
  CY_ALARM Alarm;
  //
  virtual CSignal * GetOUT ( char * Name );
  virtual CSignal ** GetIN ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual void ResetAlarms();
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  NEWY
};
/////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_FF_MAI : public CY_Base
{
  CFF_MAI * W;
  CREATE_FF(CY_FF_MAI,CFF_MAI)
  CSignal Out[8];
  int kOut;
  //
  virtual CSignal * GetOUT ( char * Name );
  virtual CSignal ** GetIN ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  static void InitParm();
  virtual void SetParmList( );
  NEWY
};
/////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_FF_AO : public CY_Base
{
  CFF_AO * W;
  CREATE_FF(CY_FF_AO,CFF_AO)
  CSignal * SET;
  CSignal OUT;
  //
  virtual CSignal * GetOUT ( char * Name );
  virtual CSignal ** GetIN ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  NEWY
};
/////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_FF_DI : public CY_Base
{
  CFF_DI * W;
  CREATE_FF(CY_FF_DI,CFF_DI)
  CSignal OUT;
  CY_ALARM Alarm;
  //
  virtual CSignal * GetOUT ( char * Name );
  virtual CSignal ** GetIN ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual void ResetAlarms();
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  NEWY
};
/////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_FF_DO : public CY_Base
{
  CFF_DO * W;
  CREATE_FF(CY_FF_DO,CFF_DO)
  CSignal * SET;
  CSignal OUT;
  //
  virtual CSignal * GetOUT ( char * Name );
  virtual CSignal ** GetIN ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  NEWY
};
/////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_FF_IS : public CY_Base
{
  CFF_IS * W;
  CREATE_FF(CY_FF_IS,CFF_IS)
  CSignal *IN1, *IN2, *IN3, *IN4, *DIS1, *DIS2, *DIS3, *DIS4, *OSEL;
  CSignal OUT, SEL;
  //
  virtual CSignal * GetOUT ( char * Name );
  virtual CSignal ** GetIN ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  NEWY
};
/////////////////////////////////////////////////////////////////////

struct Y_DATA_API CY_FOUT_W
{
	double MSH1;
	double MSH2;
	double MSH3;
	double MSH4;
	double MSH5;
	double MSH6;
	double MSH7;
	double MSH8;
	double MSL1;
	double MSL2;
	double MSL3;
	double MSL4;
	double MSL5;
	double MSL6;
	double MSL7;
	double MSL8;
};

struct Y_DATA_API CY_FOUT : public CY_Base, public CY_FOUT_W
{
  CFOUT * W;
	CSignal Out[8];
	int kOut;
  CREATE(CY_FOUT,CFOUT)
	CSignal * SET;
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
	NEWY
};
//////////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_FFSUM_W
{
  double dPV, dPVn;
  double dMV;

  eAuxOutDestType tSUB;
  double SUB_Value;
  double mvForOutBySUBSYS;
};

struct Y_DATA_API CY_FFSUM : public CY_Base, public CY_FFSUM_W
{
  CFFSUM * W;
  CSignal * IN;
  CSignal * SET;
  CSignal * BIN;
  CSignal * TIN;
  CSignal * TSI;
  CSignal * INT;
  
  CSignal OUT;
  CSignal SUB;

  CY_OUT ObrOut;
  CY_IN ObrIn;
  CY_ALARM Alarm;

  CREATE(CY_FFSUM,CFFSUM)
  virtual CSignal * GetOUT ( char * Name );
  virtual CSignal ** GetIN ( char * Name );
  virtual void Work ( double dt );
  virtual void Init();
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  virtual void ResetAlarms();
  NEWY
private:
  void InitData();
  void CalcSUB();
  void CalcOUT();
};
//////////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_XCPL : public CY_Base
{
  CXCPL * W;
  CSignal * IN;
  CSignal * BIN;
  CSignal * TIN;
  CSignal * TSI;
  CSignal * INT;
  
  CSignal OUT;
  CSignal SUB;

  CY_OUT ObrOut;
  CY_IN ObrIn;
  CY_ALARM Alarm;

  double dMV;

  CREATE(CY_XCPL,CXCPL)
  virtual CSignal * GetOUT ( char * Name );
  virtual CSignal ** GetIN ( char * Name );
  virtual void Work ( double dt );
  virtual void Init();
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  virtual void ResetAlarms();
  NEWY
};
/////////////////////////////////////////////////////////////////////

struct Y_DATA_API CY_HAS3C_W
{
  double O;
  double testMV, testSV, testPV;
};

struct Y_DATA_API CY_HAS3C : public CY_Base, public CY_HAS3C_W
{
  enum eButtonActionType { ALTER=0,RADIO,MOMENT };
  CHAS3C * W;
	CSignal * JPV, *Q01, *Q02, *Q03, *Q04, *Q05, *JMV, *JSV, *JMOD, *JALM, *JBST;
	CSignal Out[9];
	int kOut;
	CREATE(CY_HAS3C,CHAS3C)
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
	NEWY
};
//////////////////////////////////////////////////////////////////////////

struct Y_DATA_API CY_INDST2 : public CY_Base
{
  CINDST2 * W;
  CSignal * JMOD;
  CSignal * INT;
  CSignal * JBST;
  CSignal * JALM;
  CSignal * JPV;
  CSignal * JSV;
  CSignal * J01;
  CSignal * J02;
  CREATE(CY_INDST2,CINDST2)
  virtual CSignal * GetOUT ( char * Name );
  virtual CSignal ** GetIN ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  NEWY
};
/////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_INDST3 : public CY_Base
	{
  CINDST3 * W;
	CSignal * JMOD;
	CSignal * INT;
	CSignal * JBST;
	CSignal * JALM;
	CSignal * JPV;
	CSignal * JMV;
	CSignal * JSV;
	CSignal * J01;
	CSignal * J02;
	CSignal * J03;
	CSignal * J04;	
  CSignal   JSVout, JMVout, J01out, J02out, J03out, J04out;
  double tSV, tMV, tSVH, tSVL, tMH, tML;
	CREATE(CY_INDST3,CINDST3)
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
	NEWY
	};

//////////////////////////////////////////////////////
struct Y_DATA_API CY_IO_W
{
	double _dPV;
	int compareResultOfI;
  int _iPV_old;

  CY_IO_W();
};

//////////////////////////////////////////////////////
struct Y_DATA_API CY_IO : public CY_Base, public CY_IO_W
{
	int Type;
	enum eTypePV 
  { 
    None, 
    Int, 
    Double, 
    IntCompareWith,
    FlagByte,
    FlagByteCompareWith,
    DblCompareWith,
  };
	eTypePV	TypePV;
	CSignal * IN, *ins[5];
	CSignal OUT;
  CSignal **ppRefSignal;
  void * pVarRef;
	bool NoPrepare;
	CY_IO();
	double ConstVal_D;
	int ConstVal_I;
  static CY_Base * Create( char * Name, int number, int domain )
	{
		CY_IO * pClass = new CY_IO;
		return pClass;
	};
	bool Is;
	bool WorkModel;
	int Mes;
	double Gain;
	double Bias;
	double MashtabL;
	CY_DI * pDI;
	struct C_AN * pAN;
  bool Start;
	bool Get_Mes;
	bool LocalActions;
	int iPV_old;
  bool SetDefect;
	double PV_Z;
  BYTE FlagUse;
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void LinkToDI();
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  virtual bool Draw ( CDrawData * pDD, int x, int y );
  virtual int SetData( int TypeData, void * pData );
  bool GetObj( int Type, void ** pData, char * Name );
  bool GetObj( bool Out );
  void GetMes();
	void ShowOG();
	void InitData();
  virtual bool GetVar( char * VarName, void ** pVar, void ** pTuneVar, char * Type, eVarType *Work, CAlarmBase::tFlagUse_ **ppFlagUse /*= NULL*/, USHORT *pVarSize /*= NULL*/ );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
  NEWY
};

/////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_LAG_W
{
  int State;
  double dCPV;
	CY_LAG_W();
	NEWY
};

struct Y_DATA_API CY_LAG : public CY_Base, public CY_LAG_W
{
  CLAG * W;
	CREATE(CY_LAG,CLAG)
	CSignal * IN;
	CSignal OUT;
	CSignal SUB;
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
	NEWY
};
/////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_INTEG : public CY_Base
{
  CINTEG * W;
  CREATE(CY_INTEG,CINTEG)
  CSignal * IN;
  CSignal OUT;
  CSignal SUB;
  virtual CSignal * GetOUT ( char * Name );
  virtual CSignal ** GetIN ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  NEWY
};
/////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_RAMP_W
{
  CY_RAMP_W();
  double dCPVn;

};

struct Y_DATA_API CY_RAMP : public CY_Base, public CY_RAMP_W
{
  CRAMP * W;
  CREATE(CY_RAMP,CRAMP)
  CSignal * IN;
  CSignal OUT;
  CSignal SUB;
  virtual CSignal * GetOUT ( char * Name );
  virtual CSignal ** GetIN ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
  NEWY
};
/////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_LDLAG_W
{
  CY_LDLAG_W();
  double dCPV;
};

struct Y_DATA_API CY_LDLAG : public CY_Base, public CY_LDLAG_W
{
  CLDLAG * W;
  CREATE(CY_LDLAG,CLDLAG)
  CSignal * IN;
  CSignal OUT;
  CSignal SUB;
  virtual CSignal * GetOUT ( char * Name );
  virtual CSignal ** GetIN ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
  NEWY
};
/////////////////////////////////////////////////////////////////
struct C_LC64_Node
{
  enum eNodeTypeLogicOperation
  {
    ent_undefined = -1,
    ent_empty = 0,
    ent_OR = 1,
    ent_TON,
    ent_TOFF,
    ent_NOT,
    ent_AND,
    ent_OFFD,
    ent_OND,
    ent_SRS1_S,
    ent_SRS2_S,
    ent_SRS1_R,
    ent_SRS2_R,
    ent_GT,
    ent_GE,
    ent_EQ,
    ent_WOUT,
    ent_WOUT_Up,
    ent_Last,
    ent_Input1 = ent_Last + 100,
    ent_Input2,
    ent_Input3,
    ent_Input4,
    ent_Input5,
    ent_Input6,
    ent_Output1,
    ent_Output2,
    ent_Output3,
    ent_Output4,
    ent_Output5,
    ent_Output6,
  };
  enum
  {
    MAX_IN = 32,
  };
  struct SIn
  {
    SHORT index : 12;
    SHORT stateNum : 4;
  };
  BYTE x, y;
  BYTE State : 3;
  BYTE State2 : 1;
  BYTE State3 : 1;
  BYTE State4 : 1;
  BYTE State5 : 1;
  BYTE State6 : 1;
  BYTE old_State : 3;
  BYTE old_State2 : 1;
  BYTE old_State3 : 1;
  BYTE old_State4 : 1;
  BYTE old_State5 : 1;
  BYTE old_State6 : 1;
  SIn In[MAX_IN];
  BYTE kIn;
  eNodeTypeLogicOperation Type;
  BYTE Time;
  BYTE GetState( BYTE sn );
  NEWY
};

struct C_LC64_FT
{
  SHORT o_f, n_f;
  SHORT o_t, n_t;
  NEWY
};

struct C_LC64_Pnt
{
  short x, y;
  NEWY
};


struct C_LC64_OP : public C_LC64_Node
	{
	static C_LC64_Node * Nodes;
	void OR(); 
	void TON();
	void TOFF();
	void NOT();
	void AND();
	void OFFD( double dt, double & ST, double & PT );
	void OND( double dt, double & ST, double & PT );
  void SRS1_S();
  void SRS2_S();
  void SRS1_R();
  void SRS2_R();
  void GT();
  void GE();
  void EQ();
  void AddIn ( SHORT N, SHORT Np, SHORT stateN );
  void WOUT();
  void WOUT_Up();
  void Set( );
	};

struct Y_DATA_API CY_LC64_W
	{
  int MODE_AUT;
	CY_LC64_W();
	NEWY
	};

#include "LC64_AC.h"
#pragma warning( push )
#pragma warning( disable : 4275 )
struct Y_DATA_API SBlkLC64 : SGBLK_LC64_HDR
{
  int dWidth;
  int dHeight;
  NEWY
};
#pragma warning( pop )
struct Y_DATA_API CY_LC64_E_Base : public CY_Base, public CY_LC64_W
{
  enum
  {
    maxInputOutput = 6,
  };
  typedef char charMx32[maxInputOutput][32*4];
  enum eTMNL_StartTiming
  {
    eTMNL_StartupAtInitialColdStart_Restart = 'I',
    eTMNL_PeriodicExecution = 'T',
    eTMNL_OneshotProcessing = 'O',
    eTMNL_RestrictedInitialExecution = 'B',
  };
  enum eBLK_Types
  {
    eBLK_Action1Output1   = 70010,
    eBLK_Action1Output2   = 70620,
    eBLK_Action1Output3   = 70930,
    eBLK_Action1Output4   = 71040,
    eBLK_Action1Output5   = 71150,
    eBLK_Action1Output6   = 71260,
    eBLK_Action2          = 70110,
    eBLK_Action2Output1   = 70210,
    eBLK_Action3Output1   = 70310,//+descr
    eBLK_Action3Output2   = 70720,//+descr
    eBLK_Action4Output1   = 80410,//POut2+comment(R)
    eBLK_Action5Output1   = 80510,//POut2+comment(U)
    eBLK_Action6Output1   = 80310,//POut2
    eBLK_Condition1Input1 = 10001,
    eBLK_Condition1Input2 = 10702,
    eBLK_Condition1Input3 = 10803,
    eBLK_Condition1Input4 = 11004,
    eBLK_Condition1Input5 = 11105,
    eBLK_Condition1Input6 = 11206,
    eBLK_Condition2 =       10201,//+descr
    eBLK_Condition2Input1 = 10301,
    eBLK_Condition3Input1 = 10401,//+descr
    eBLK_Condition3Input2 = 10902,//+descr
  };
  bool execStartupAtInitialColdStart_Restart;
  HWND hWnd;
  int DX_Cond_S;
  int DY_Cond_S;
  int DX_Cond_L;
  int DY_Cond_L;
  int * OrdNode;
  int * OrdISS;
  int kISS;
  int * OrdCOS;
  int kCOS;
  bool StopOnChange;
  double m_timeHolder;
  ////////////////////////////////////////////////////////
  DWORD kObj;
  ////////////////////////////////////////////////////////
  typedef int tIO_[maxInputOutput];
  tIO_ * IO;
  //
  //=======================================================
  typedef CCond_ST16_LC64 tCond_[maxInputOutput];
  typedef CAct_LC64 tAction_[maxInputOutput];
  tCond_ * VarCond;
  tAction_ * VarAct;
  //=======================================================
  //
  DWORD kDATA;
  DWORD kMTRX;
  DWORD kMTRX_CONN;
  DWORD kMTRX_PNT;
  //
  ///////////////////////////////////////////////////////
  // чтение
  BYTE * Timer;
  SBlkLC64 * Blk;
  C_LC64_Node * Node;	//x,y
  C_LC64_FT * FT;
  C_LC64_Pnt * Pnt;
  charMx32 * RefName;
  //////////////////////////////////////////////////////////////////////////
  double *pSTs64;
  double *pPTs64;
  ////////////////////////////////////////////////////////
  int m_LastWorkStep;
  //////////////////////////////////////////////////////////////////////////
  CY_LC64_E_Base();
  //char * Alias ( char * RefName );
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  virtual bool Draw ( CDrawData * pDD, int x, int y );
  virtual bool CreatePSMGraf( struct CGrafParm ** GP );
  void Prepare();
  int FindNode ( int x, int y );
  void Logic( double dt );
  void OutOG( int N );
  virtual bool ReadParm( char * ParmName, char * ParmValue, BYTE * pData );
  void ReadData( BYTE * pData );
  virtual bool SaveParms( HANDLE hFile );
  virtual bool RestParms( HANDLE hFile );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
  virtual bool EnumUsedObj( DWORD &pos, LPCSTR *ppName, CY_Base **ppObj );
};

struct Y_DATA_API CY_LC64 : public CY_LC64_E_Base
{
  CLC64 * W;
	CSignal Out[16];
	int kOut;
	CREATE(CY_LC64,CLC64)
////////////////////////////////////////////////////////
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
  virtual void Init( );
  virtual int Prepare0();
  virtual void Work( double dt );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool ReadParm( char * ParmName, char * ParmValue, BYTE * pData );
  static void InitParm();
  virtual void SetParmList( );
  virtual int GetParams( KYBridge2SysParam &params );
  NEWY
};
//
struct Y_DATA_API CY_LC64_E : public CY_LC64_E_Base
{
  CLC64_E * W;
	CSignal *In[32], Out[32];
	int kOut;
	CREATE(CY_LC64_E,CLC64_E)
  ////////////////////////////////////////////////////////
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
  virtual void Init( );
  virtual int Prepare0();
  virtual void Work ( double dt );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool ReadParm( char * ParmName, char * ParmValue, BYTE * pData );
  static void InitParm();
  virtual void SetParmList( );
  virtual int GetParams( KYBridge2SysParam &params );
  NEWY
};

/////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_MC_NE_W
{
  int dummyTSW;
  int dummyTSW1;
  int OUT_dummy, OUT1, OUT2, OUT3;
  int dummyPV, prevMV;
  double Time_1;
  double Time_2;
  double Time_3;
  int Timer_1;
  int Timer_2;
  int Timer_3;
  double dummyFV, dummyANSP;
//
	double mtm;//answerback check suppression timer
	int MV_old;
	bool AllOK;
  int dummyMV_0;
  //
  double dblONTM;
};

struct Y_DATA_API CY_MC_NE : public CY_Base, public CY_MC_NE_W
{
  class Y_DATA_API SPulseOutMoment
  {
    int nStepOn;
    int nStepOff;
  public:
    char szText[128*4];
  public:
    SPulseOutMoment();
    void Start( int nStep );
    void Stop( int nStep );
  }_PLwork[3];
  CMC_NE_COMM * W;
  enum eOutputConversion{ _2ST=0, _2PL=1, _3ST=2, _3PL=3 };
  enum eType { Null, _2, _2E, _3, _3E };
  eType Type;
	CSignal * IN;
	CSignal * IN1;
	CSignal * IN2;
	CSignal * TT;
	CSignal * FB;
	CSignal * SWI;
	CSignal * INT;
	CSignal * IL;
	CSignal * TSI;
	CSignal Out[3];
  CY_Base *pPIOouts[3];
	int kOut;
  bool TT_mode;
  CY_MC_NE( char * Name, int Type, int number, int domain );
	static CY_Base * Create( char * Name, int Type, int number, int domain )
	{
		CY_MC_NE * pCY_MC_NE = new CY_MC_NE( Name, Type, number, domain );
		return pCY_MC_NE;
	};
	char * GetClass(){ return "CY_MC_NE"; };
  int GetOutputConversion();
  int GetOutputOnIL();
  int GetOutCount();
  int K_in;
  int K_out;
  const double PulseW;
  char * TypeOut;
  //bool PV_MV;
  bool WorkModel;
  bool InvIN12;
  bool InvOUT12;
	bool Start;
	bool IL_mode;
  int MV_On_IL;
	CY_ALARM Alarm;
	CSignal IN_add;
  char Name_IN_add[16*4];
  char From_IN_add[16*4];
  char To_IN_add[16*4];
  char Name_OUT2_add[16*4];
  char From_OUT2_add[16*4];
  char To_OUT2_add[16*4];
  char Name_OUT3_add[16*4];
  char From_OUT3_add[16*4];
  char To_OUT3_add[16*4];
  int * pPV1;
  int * pPV2;
  int * pPV3;
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual void ResetAlarms();
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  void Work_1out( double dt );
  void Work_2out( double dt );
  void Work_3out( double dt );
//  void Work_3( double dt );
  void SetPV( );
  void WorkMV( );
  void AnswerbackCheck( double dt );
  void Set_Klap( );
  void Set_Pump( );
  void Set_AVO( );
  static void InitParm();
  virtual void SetParmList( );
  bool Double_IN( );
  bool Double_OUT( int kOut );
  void CheckONs( int mvOld, int mvNew, double dt );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
  virtual bool EnumUsedObj( DWORD &pos, LPCSTR *ppName, CY_Base **ppObj );
  virtual void OnAssigned( void *pAddr );
	NEWY
protected:
  void WorkImpl( double dt, bool bSafeCSV = false );
};

/////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_MLD_W
{
	double dummyTSW;
	double dummRSW;
	double dMV;
  double subsysMV;//если выбран коммуникационный выход
	CY_MLD_W();
	NEWY
};

struct Y_DATA_API CY_MLD : public CY_Base, public CY_MLD_W
{
  CMLD * W;
	CSignal * TIN;
	CSignal * TSI;
	CSignal OUT;
	CSignal SUB;
  CY_OUT ObrOut;
	CREATE(CY_MLD,CMLD)
  bool GetOut;
	CY_ALARM Alarm;
	bool Start;	
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual void ResetAlarms();
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
	NEWY
};

/////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_MLD_PVI_W
{
	double dummyTSW;// Tracking switch 0, 1 0
	double dPVn;
	double dMV;
  double subsysMV;//если выбран коммуникационный выход
  CY_MLD_PVI_W();
  NEWY
};

struct Y_DATA_API CY_MLD_PVI : public CY_Base, public CY_MLD_PVI_W
{
  CMLD_PVI * W;
	CSignal * IN;
	CSignal * TIN;
	CSignal * TSI;
	CSignal OUT;
	CSignal SUB;
  CY_IN ObrIn;
  CY_OUT ObrOut;
  int UseAlg;//1 - Data_GAINin, 2 - Data_BIASin
	CREATE(CY_MLD_PVI,CMLD_PVI)
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
	NEWY
};

/////////////////////////////////////////////////////////////////////

struct Y_DATA_API CY_MLD_SW_W
{
  CY_MLD_SW_W();
  double O;
  double dMV;
  int pwmUP;
  int pwmDOWN;
};

struct Y_DATA_API CY_MLD_SW : public CY_Base, public CY_MLD_SW_W
{
  CMLD_SW * W;
	CSignal * SET;
	CSignal * TIN;
	CSignal * TSI;
	CSignal * INT;
	CSignal OUT, OUT_DOWN;
	CSignal SUB;
	CREATE(CY_MLD_SW,CMLD_SW)
	CY_ALARM Alarm;
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual void ResetAlarms();
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
	NEWY
	};

/////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_OFFD_W
	{
  int State;
	CY_OFFD_W();
	NEWY
	};

struct Y_DATA_API CY_OFFD : public CY_Base, public CY_OFFD_W
	{
  COFFD * W;
	CREATE(CY_OFFD,COFFD)
	CSignal * IN;
	CSignal OUT;
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
	NEWY
	};
/////////////////////////////////////////////////////////////////
/*struct Y_DATA_API CY_WOUT_W
{
  int State;
  CY_WOUT_W();
  NEWY
};
*/
struct Y_DATA_API CY_WOUT : public CY_Base//, public CY_WOUT_W
{
  CWOUT * W;
  CREATE(CY_WOUT,CWOUT)
  CSignal * Q01;
  CSignal * Q02;
  CSignal OUT;
  virtual CSignal * GetOUT ( char * Name );
  virtual CSignal ** GetIN ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  NEWY
};

/////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_OND_W
	{
  int State;
	CY_OND_W();
	NEWY
	};

struct Y_DATA_API CY_OND : public CY_Base, public CY_OND_W
	{
  COND * W;
	CREATE(CY_OND,COND)
	CSignal * IN;
	CSignal OUT;
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
	NEWY
	};


/////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_ONOFF_W
	{
  double dPV;
  double prevEn;
	CY_ONOFF_W();
	};

struct Y_DATA_API CY_ONOFF : public CY_Base, public CY_ONOFF_W
	{
  CONOFF * W;
	CSignal * IN;
	CSignal * SET;
	CSignal * BIN;
	CSignal * INT;
	CSignal OUT;
	CSignal SUB;
	CREATE(CY_ONOFF,CONOFF)
  bool GetOut;
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
	NEWY
	};
/////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_ONOFF_G_W
{
  double dPV;
  double prevEn;
  int pinOut0, pinOut1;
  CY_ONOFF_G_W();
  NEWY
};

struct Y_DATA_API CY_ONOFF_G : public CY_Base, public CY_ONOFF_G_W
{
  CONOFF_G * W;
  CSignal * IN;
  CSignal * SET;
  CSignal * BIN;
  CSignal * INT;
  CSignal Out[2];
  CSignal SUB;
  int kOut;
  CY_ALARM Alarm;
  CREATE(CY_ONOFF_G,CONOFF_G)
  virtual CSignal * GetOUT ( char * Name );
  virtual CSignal ** GetIN ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual void ResetAlarms();
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  bool Double_OUT();
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
  NEWY
};
//////////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_ONOFF_GE_W
{
  double dPV;
  double prevEn;
  int pinOut0, pinOut1;
  CY_ONOFF_GE_W();
  NEWY
};

struct Y_DATA_API CY_ONOFF_GE : public CY_Base, public CY_ONOFF_GE_W
{
  CONOFF_GE * W;
  CSignal * IN;
  CSignal * SET;
  CSignal * BIN;
  CSignal * INT;
  CSignal Out[2];
  CSignal SUB;
  int kOut;
  CY_ALARM Alarm;
  CREATE(CY_ONOFF_GE,CONOFF_GE)
  virtual CSignal * GetOUT ( char * Name );
  virtual CSignal ** GetIN ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual void ResetAlarms();
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  bool Double_OUT();
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
  NEWY
};
//////////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_PTC : public CY_Base
{
  double prevP;
  double m_timeHolder;
  CPTC * W;
  CSignal * IN;
  CSignal OUT;
  CY_ALARM Alarm;
  CREATE(CY_PTC,CPTC)
  virtual CSignal * GetOUT( char * Name );
  virtual CSignal ** GetIN( char * Name );
  virtual void Work( double dt );
  virtual void Init();
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  static void InitParm();
  virtual void SetParmList();
  NEWY
};
/////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_PBS5C_W
{
	int oMV01, oMV02, oMV03, oMV04, oMV05;
	double Timer;
};

struct Y_DATA_API CY_PBS5C : public CY_Base, public CY_PBS5C_W
{
  enum eType
  {
    NO,
    LAMPBUTN,
    LAMP,
    BUTTON,
  };
  struct SFPSW
  {
    eType btnType;
    char szOff[12*4];
    char szOn[12*4];
    int clrOff;
    int clrOn;
  }fpsw[5];
  enum
  {
    nElements = 5,
  };
  CPBS5C * W;
	CSignal * JMOD;
	CSignal * JBST;
	CSignal * JALM;
//
	CSignal * Q01;
	CSignal * Q02;
	CSignal * Q03;
	CSignal * Q04;
	CSignal * Q05;
//
	CSignal * Q[nElements];
	int * MV[nElements];
	int * oMV[nElements];
	int * PV[nElements];
	CSignal * INT;
  bool SetoMV;
  double m_timeHolder;
//
	CSignal Out[nElements];
	int kOut;
  int PosZ;
  CY_ALARM Alarm;
	CREATE(CY_PBS5C,CPBS5C)
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual void ResetAlarms();
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  virtual bool ReadParm( char * ParmName, char * ParmValue, BYTE * pData );
  virtual bool SaveParms( HANDLE hFile );
  virtual bool RestParms( HANDLE hFile );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
  virtual bool SetResetAlrmFromExtern( int alrmNumber, bool bSet );
	NEWY
};
/////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_PBS10C_W
{
  int oMV01, oMV02, oMV03, oMV04, oMV05, oMV06, oMV07, oMV08, oMV09, oMV10;
  double Timer;
};

struct Y_DATA_API CY_PBS10C : public CY_Base, public CY_PBS10C_W
{
  enum
  {
    nElements = 10,
  };
  CPBS10C * W;
  CSignal * JMOD;
  CSignal * JBST;
  CSignal * JALM;
  //
  CSignal * Q01;
  CSignal * Q02;
  CSignal * Q03;
  CSignal * Q04;
  CSignal * Q05;
  CSignal * Q06;
  CSignal * Q07;
  CSignal * Q08;
  CSignal * Q09;
  CSignal * Q10;
  //
  CSignal * Q[nElements];
  int * MV[nElements];
  int * oMV[nElements];
  int * PV[nElements];
  CSignal * INT;
  bool SetoMV;
  //
  CSignal Out[nElements];
  int kOut;
  int PosZ;
  CY_ALARM Alarm;
  CREATE(CY_PBS10C,CPBS10C)
  virtual CSignal * GetOUT ( char * Name );
  virtual CSignal ** GetIN ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual void ResetAlarms();
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  static void InitParm();
  virtual void SetParmList( );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
  virtual bool SetResetAlrmFromExtern( int alrmNumber, bool bSet );
  NEWY
};

////////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_PID_W	// p328
	{
	double _CSV; //Cascade setpoint value  Value in the same engineering unit as PV SL
	double VN; //I/O compensation value  ----- 0
	double RLV1; //Reset limit value 1 Value in the same engineering unit as MV MSL
	double RLV2; //Reset limit value 2 Value in the same engineering unit as MV MSL
	double oldTSW; //Tracking switch  0, 1 0
	double oldCSW; //Control switch  0, 1 0
	double oldPSW; //Preset MV switch  0, 1, 2, 3 0
	double oldRSW; //Pulse width reset switch  0, 1 0
	double oldBSW;
	double En, En1;
	double dEn, dEn1;
	double PVn1;
	double dPVn, dPVn1;
  double dMV;
//
	int Deadband_state;
  int pwmUP;
  int pwmDOWN;
  BYTE BSW;//computer failure (BSW=ON)
	CY_PID_W();
// PVn1 = PV в начале!!!
	};


struct Y_DATA_API CY_PID : public CY_Base, public CY_PID_W
{
  CPID * W;
	CSignal * IN;
	CSignal * SET;
	CSignal * BIN;
	CSignal * RL1;
	CSignal * RL2;
	CSignal * TIN;
	CSignal * TSI;
	CSignal * INT;
//
	CSignal OUT, OUT_DOWN;
	CSignal SUB;

  eAuxOutDestType m_tSUB;
  double m_SUB_Value;
//
  CY_OUT ObrOut;
  CY_IN ObrIn;
  CY_ALARM Alarm;
  CGrafParm GP;
//
  bool Limits;
  bool Out_MV;
  bool PV_SV;       
  bool StdLevOut;
  bool StdLevIn;
  bool StdFlowUp;
  bool StdFlowDown;
  bool Get_SV;
  bool NoBack;
	//double dOpt, dMed, dMin, dMax;
  //double MVOpt; 
  enum eType { PID, PID_STC, FF_PID };
  eType Type;
  bool bFirstCall;
  int nCtrlPeriodWork;//срабывание на flagBasicScanControl
  double dblInDt;
  DWORD nStepWork;
  double mv4Out;
  static bool s_globalApplyOutGainBias;//глобальное применение Gain Bias при выходе SUBSYS
  virtual CSignal ** GetIN ( char * Name );
	virtual CSignal * GetOUT ( char * Name );
//
  CY_PID( char * Name, int Type, int number, int domain );
  static CY_Base * Create ( char * Name, int Type, int number, int domain )
	{
    CY_PID * pCY_PID = new CY_PID( Name, Type, number, domain );
		return pCY_PID;
	};
	GET_CLASS(CY_PID)
	void PID_control( double dt );
	void PID_basic( double dt );
	void PID_level( double dt, double pv, double sp, double scale );
	void PID_I_PD( double dt );
	void PID_PI_D( double dt );
	void PID_auto( double dt );
	void PID_auto_2( double dt );
  void CalcSUB();
  void CalcOUT();
//
	double NLG();
	void Reset_limit_function( double dt );
	void Deadband_action( );
	void Process_variable_tracking( );
	void Setpoint_value_limiter( );// При получении SET
	void Setpoint_value_pushback( );
	void Initialization_manual( );
  void InitData( );
  static void InitParm();
  virtual void SetParmList( );
  virtual bool ReadParm( char * ParmName, char * ParmValue, BYTE * pData );
//
  virtual void OnReadTunningData();
	virtual void Work ( double dt );
	virtual int GetParams( KYBridge2SysParam &params );
	virtual bool CreatePSMGraf( struct CGrafParm ** GP );
  virtual void Init( );
  virtual void ResetAlarms();
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  virtual int SetData( int TypeData, void * pData );
  virtual double * GetValueAddr();
  virtual bool GetVar( char * VarName, void ** pVar, void ** pTuneVar, char * Type, eVarType * Work, CAlarmBase::tFlagUse_ **ppFlagUse = NULL, USHORT *pVarSize = NULL );
  virtual bool TransferConnection( LPCSTR pszFieldInName );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
	NEWY
	};
//////////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_PI_HLD_W
{
  double En, En1;
  double dEn, dEn1;
  double PVn1;
  double dPVn, dPVn1;
  double dMV;
  //
  int Deadband_state;
  int pwmUP;
  int pwmDOWN;
  double tsCounter;
  CY_PI_HLD_W();
};

struct Y_DATA_API CY_PI_HLD : public CY_Base, public CY_PI_HLD_W
{
  CPI_HLD * W;
  CSignal * IN;
  CSignal * SET;
  CSignal * BIN;
  CSignal * RL1;
  CSignal * RL2;
  CSignal * TIN;
  CSignal * TSI;
  CSignal * INT;
  //
  CSignal OUT, OUT_DOWN;
  CSignal SUB;

  eAuxOutDestType m_tSUB;
  double m_SUB_Value;

  CY_OUT ObrOut;
  CY_IN ObrIn;
  CY_ALARM Alarm;
  CGrafParm GP;
  bool bFirstCall;
  static bool s_globalApplyOutGainBias;//глобальное применение Gain Bias при выходе SUBSYS

  CREATE(CY_PI_HLD,CPI_HLD)
  void InitData();
  void CalcSUB();
  void CalcOUT();
  void Reset_limit_function( double dt );
  void Deadband_action();
  void Initialization_manual();
  void Setpoint_value_pushback();
  void Setpoint_value_limiter();// При получении SET
  void PID_control( double dt );
  //
  virtual CSignal * GetOUT ( char * Name );
  virtual CSignal ** GetIN ( char * Name );
  virtual void OnReadTunningData();
  virtual bool CreatePSMGraf( struct CGrafParm ** GP );
  virtual void Work( double dt );
  virtual void Init( );
  virtual void ResetAlarms();
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  virtual int SetData( int TypeData, void * pData );
  virtual bool TransferConnection( LPCSTR pszFieldInName );
  NEWY
};
//////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_PVI : public CY_Base 
{
  CPVI * W;
//
	CSignal * IN;
	CSignal OUT;
	CSignal SUB;
  CY_IN ObrIn;
//===================================
	double dOpt, dMed, dMin, dMax;
//===================================
  int UseAlg;//1 - Data_GAINin, 2 - Data_BIASin
  static bool s_globalUseInGainBias;//глобальное использование входных Gain Bias
//
  eAuxOutDestType m_tSUB;
  double m_SUB_Value;

	CREATE(CY_PVI,CPVI)
  CY_ALARM Alarm;
  virtual bool ReadParm( char * ParmName, char * ParmValue, BYTE * pData );
	virtual CSignal ** GetIN ( char * Name );
	virtual CSignal * GetOUT ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual void ResetAlarms();
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  virtual bool Draw ( CDrawData * pDD, int x, int y );
  virtual int SetData( int TypeData, void * pData );
  static void InitParm();
  virtual void SetParmList( );
  virtual bool GetVar( char * VarName, void ** pVar, void ** pTuneVar, char * Type, eVarType *Work, CAlarmBase::tFlagUse_ **ppFlagUse /*= NULL*/, USHORT *pVarSize /*= NULL*/ );
  void InitData( );
  virtual double * GetValueAddr();
  NEWY
  void CalcSUB();
};
//////////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_PVI_DV : public CY_Base 
{
  CPVI_DV * W;
  //
  CSignal * IN;
  CSignal OUT;
  CSignal SUB;
  CY_IN ObrIn;
  //===================================
  double dOpt, dMed, dMin, dMax;
  //===================================
  int UseAlg;//1 - Data_GAINin, 2 - Data_BIASin
  static bool s_globalUseInGainBias;//глобальное использование входных Gain Bias
  //
  CREATE(CY_PVI_DV,CPVI_DV)
  CY_ALARM Alarm;
  virtual CSignal ** GetIN ( char * Name );
  virtual CSignal * GetOUT ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual void ResetAlarms();
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  virtual bool Draw ( CDrawData * pDD, int x, int y );
  virtual int SetData( int TypeData, void * pData );
  static void InitParm();
  virtual void SetParmList( );
  virtual bool GetVar( char * VarName, void ** pVar, void ** pTuneVar, char * Type, eVarType *Work, CAlarmBase::tFlagUse_ **ppFlagUse /*= NULL*/, USHORT *pVarSize /*= NULL*/ );
  void InitData( );
  virtual double * GetValueAddr();
  NEWY
};
//////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_FUNC_VAR_W
{
  double dCPV;
};

struct Y_DATA_API CY_FUNC_VAR : public CY_Base, public CY_FUNC_VAR_W
{
  CFUNC_VAR * W;
//
	CSignal * IN;
	CSignal OUT;
	CSignal SUB;
  double * X[16];
  double * Y[16];
  bool CPV_RV;
//
	CREATE(CY_FUNC_VAR,CFUNC_VAR)
	virtual CSignal ** GetIN ( char * Name );
	virtual CSignal * GetOUT ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
  NEWY
};

/////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_PG_L13_W
	{
	int vSUB;
  double svInSecs;
	};

struct Y_DATA_API CY_PG_L13 : public CY_Base, public CY_PG_L13_W
	{
  int minSV_index, maxSV_index;
  CPG_L13 * W;
	CSignal * INT;
	CSignal OUT;
	CSignal SUB;
	CREATE(CY_PG_L13,CPG_L13)
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  static void InitParm();
  virtual void SetParmList( );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
	NEWY
	};

/////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_RATIO_W
	{
	double _CSV; //Cascade ratio setpoint value Value in the same engineering unit as SV SSL
	double _RSV; //Remote ratio setpoint value (*4) Value in the same engineering unit as SV SSL
	double _RMV; //Remote manipulated output value (*5) Value in the same engineering unit as MV MSL
	double _CALC; //Calculated output value Value in the same engineering unit as MV MSL
	double dPVn, dMV;
//
	double SVe;
	};

struct Y_DATA_API CY_RATIO : public CY_Base, public CY_RATIO_W
	{
  CRATIO * W;
	CSignal * IN;
	CSignal * SET;
	CSignal * TIN;
	CSignal * TSI;
	CSignal * INT;
	CSignal OUT;
	CSignal SUB;
  CY_IN ObrIn;
  CY_OUT ObrOut;
	CREATE(CY_RATIO,CRATIO)
  bool BIAS0;
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
	NEWY
	};

/////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_RL_W
{
  double O;
	CY_RL_W();
};

struct Y_DATA_API CY_RL : public CY_Base, public CY_RL_W
{
  enum EOper
  {
    eHZ = -1,
    eCMP = 0,
    eAND = 1,
  };
  CRL * W;
	CREATE(CY_RL,CRL)
	void * L[16*4];
  char TypeL[16*4];
	void * R[16*4];
  char TypeR[16*4];
//
  CSignal * Q[32];
//
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  virtual bool ReadParm( char * ParmName, char * ParmValue, BYTE * pData );
  virtual bool CallSA(LPCSTR pszParam);
	NEWY
};
//////////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_RS : public CY_Base
{
  CRS * W;
  CREATE(CY_RS,CRS)
  //
  virtual CSignal * GetOUT ( char * Name );
  virtual CSignal ** GetIN ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  NEWY
};
////////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_SIO_X_W
{
	int OUT1, OUT2;	 // Сигналы на выходах
	int K_AP;//Number of answerback points
	int K_OUT;//Number of output points
	double mtmHolder;//накопитель тиков
	int MV_old;
	bool AllOK;
  int MV_0;
	int PV_0;
	int PV_2;
};

struct Y_DATA_API CY_SIO_X : public CY_Base, public CY_SIO_X_W
{
  CSIO_COMM * W;
	enum eType { SI_1, SI_1E, SI_2, SI_2E,
		           SO_1, SO_1E, SO_2, SO_2E,
							 SIO_11, SIO_11E, SIO_12, SIO_12E,
							 SIO_21, SIO_21E, SIO_22, SIO_22E,
							 SIO_12P, SIO_12PE, SIO_22P, SIO_22PE };
	eType	Type;
	CSignal * IN;
	CSignal * IN1;
	CSignal * IN2;
	CSignal * SWI;
	CSignal * INT;
	CSignal * TSI;
//
	CSignal Out[3];
	int kOut;
  CY_SIO_X( char * Name, int Type, int number, int domain );
	static CY_Base * Create( char * Name, int Type, int number, int domain )
	{
    CY_SIO_X * pCY_SIO_X = new CY_SIO_X( Name, Type, number, domain );
		return pCY_SIO_X;
	};
	char * GetClass(){ return "CY_SIO_X"; };
	CY_ALARM Alarm;
  bool PV_MV;
	bool PAZ;
  bool Start;
  int * pPV_PAZ_IN;
  bool bNextOut, bNextIn;
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual void ResetAlarms();
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
	void Answerback_Input();
	void AnswerbackCheck( double dt );
	void OutPut( );
  static void InitParm();
  virtual void SetParmList( );
	bool Double_IN( );
	bool Double_OUT( );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
	NEWY
};

/////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
struct Y_DATA_API C_SFC_BASE_Impl: public CY_Base
{
  bool ReadUserDefinition( SUserDef *pUserDefinitions, int udCount, int memSize, char * ParmName, char * ParmValue, BYTE * pData );
  bool ReadSfc( char * ParmName, char * ParmValue );
  void GetParamsFromUserDefinition( char *pUSERmem, SUserDef *pUserDefinitions, int udCount, KYBridge2SysParam &params );
  bool GetVarFromUserDefinition( char *pUSERmem, SUserDef *pUserDefinitions, int udCount, char * VarName, void ** pVar, void ** pTuneVar, char * Type, eVarType *Work, BYTE **ppFlagUse = NULL, USHORT *pVarSize = NULL  );
protected:
  int UserStateSave( IYokoStateSer* pStateSer, char *pUSERmem, SUserDef *pUserDefinitions, int udCount );
  int UserStateRestore( IYokoStateSer* pStateSer, char *pUSERmem, SUserDef *pUserDefinitions, int udCount );
};

struct Y_DATA_API CY_SFCSW : C_SFC_BASE_Impl
	{
  struct CAlg * pAlg;
	CSignal Out[16];
  CREATE(CY_SFCSW,C_SFCSW)
	int kOut;
	C_SFCSW * W;
  CSignal * IN;
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
  virtual bool SetLimits( );
  virtual bool GetVar( char * VarName, void ** pVar, void ** pTuneVar, char * Type, eVarType *Work, CAlarmBase::tFlagUse_ **ppFlagUse /*= NULL*/, USHORT *pVarSize /*= NULL*/ );
  void SetParmList( );
  virtual bool ReadParm( char * ParmName, char * ParmValue, BYTE * pData );
  virtual void OnFailedTunningAssign( LPCSTR pszParm, LPCSTR pszValue );
  static void InitParm();
	NEWY
	};
/////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_SFCPB : C_SFC_BASE_Impl
{
  struct CAlg * pAlg;
  CSignal Out[16];
  CREATE(CY_SFCPB,C_SFCPB)
  int kOut;
  C_SFCPB * W;
  virtual CSignal * GetOUT ( char * Name );
  virtual CSignal ** GetIN ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  virtual bool GetVar( char * VarName, void ** pVar, void ** pTuneVar, char * Type, eVarType *Work, CAlarmBase::tFlagUse_ **ppFlagUse /*= NULL*/, USHORT *pVarSize /*= NULL*/ );
  void SetParmList( );
  virtual bool ReadParm( char * ParmName, char * ParmValue, BYTE * pData );
  virtual void OnFailedTunningAssign( LPCSTR pszParm, LPCSTR pszValue );
  static void InitParm();
  NEWY
};
/////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_UTAS : C_SFC_BASE_Impl
{
  struct CAlg * pAlg;
  CSignal Out[16];
  CREATE(CY_UTAS,C_UTAS)
  int kOut;
  C_UTAS * W;
  virtual CSignal * GetOUT ( char * Name );
  virtual CSignal ** GetIN ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  virtual bool GetVar( char * VarName, void ** pVar, void ** pTuneVar, char * Type, eVarType *Work, CAlarmBase::tFlagUse_ **ppFlagUse /*= NULL*/, USHORT *pVarSize /*= NULL*/ );
  void SetParmList( );
  virtual bool ReadParm( char * ParmName, char * ParmValue, BYTE * pData );
  virtual void OnFailedTunningAssign( LPCSTR pszParm, LPCSTR pszValue );
  static void InitParm();
  NEWY
};
/////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_UTSW : C_SFC_BASE_Impl
{
  struct CAlg * pAlg;
  CSignal Out[16];
  CREATE(CY_UTSW,C_UTSW)
  int kOut;
  C_UTSW * W;
  virtual CSignal * GetOUT ( char * Name );
  virtual CSignal ** GetIN ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual bool SetLimits( );
  virtual bool GetVar( char * VarName, void ** pVar, void ** pTuneVar, char * Type, eVarType *Work, CAlarmBase::tFlagUse_ **ppFlagUse /*= NULL*/, USHORT *pVarSize /*= NULL*/ );
  void SetParmList( );
  virtual bool ReadParm( char * ParmName, char * ParmValue, BYTE * pData );
  virtual void OnFailedTunningAssign( LPCSTR pszParm, LPCSTR pszValue );
  static void InitParm();
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
  NEWY
};
/////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_SFCAS : C_SFC_BASE_Impl
{
  struct CAlg * pAlg;
  CSignal Out[16];
  CREATE(CY_SFCAS,C_SFCAS)
  int kOut;
  C_SFCAS * W;
  virtual CSignal * GetOUT ( char * Name );
  virtual CSignal ** GetIN ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  virtual bool GetVar( char * VarName, void ** pVar, void ** pTuneVar, char * Type, eVarType *Work, CAlarmBase::tFlagUse_ **ppFlagUse /*= NULL*/, USHORT *pVarSize /*= NULL*/ );
  void SetParmList( );
  virtual bool ReadParm( char * ParmName, char * ParmValue, BYTE * pData );
  virtual void OnFailedTunningAssign( LPCSTR pszParm, LPCSTR pszValue );
  static void InitParm();
  NEWY
};
/////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_UTPB : C_SFC_BASE_Impl
{
  struct CAlg * pAlg;

  enum eSwType
  {
    NO,
    LAMPBUTN,
  };
  enum
  {
    nElements = 5,
  };
  struct SSwitchAttr
  {
    eSwType swType;
    char szLabel[12*4];
    int clrDisplay;
    char szDirective[12*4];
  }switchAttrs[nElements];
  CREATE(CY_UTPB,C_UTPB)
  C_UTPB * W;
  virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual bool SetLimits( );
  virtual bool GetVar( char * VarName, void ** pVar, void ** pTuneVar, char * Type, eVarType *Work, CAlarmBase::tFlagUse_ **ppFlagUse /*= NULL*/, USHORT *pVarSize /*= NULL*/ );
  void SetParmList( );
  virtual bool ReadParm( char * ParmName, char * ParmValue, BYTE * pData );
  virtual bool SaveParms( HANDLE hFile );
  virtual bool RestParms( HANDLE hFile );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
  virtual void OnFailedTunningAssign( LPCSTR pszParm, LPCSTR pszValue );
  static void InitParm();
  NEWY
};
/////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_UTPB_N : C_SFC_BASE_Impl
{
  enum eSwType
  {
    NO,
    LAMPBUTN,
  };
  enum
  {
    nElements = 5,
  };
  struct SSwitchAttr
  {
    eSwType swType;
    char szLabel[12*4];
    int clrDisplay;
    char szDirective[12*4];
  }switchAttrs[nElements];
  CREATE(CY_UTPB_N,C_UTPB_N)
  C_UTPB_N * W;
  virtual void Work ( double dt );
  virtual void Init( );
  virtual int GetParams( KYBridge2SysParam &params );
  virtual bool SetLimits( );
  virtual bool GetVar( char * VarName, void ** pVar, void ** pTuneVar, char * Type, eVarType *Work, CAlarmBase::tFlagUse_ **ppFlagUse /*= NULL*/, USHORT *pVarSize /*= NULL*/ );
  void SetParmList( );
  virtual bool ReadParm( char * ParmName, char * ParmValue, BYTE * pData );
  virtual bool SaveParms( HANDLE hFile );
  virtual bool RestParms( HANDLE hFile );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
  virtual void OnFailedTunningAssign( LPCSTR pszParm, LPCSTR pszValue );
  static void InitParm();
  NEWY
};
/////////////////////////////////////////////////////////////////
struct Y_DATA_API CYOPSFC : C_SFC_BASE_Impl
{
  struct CAlg * pAlg;
  CREATE(CYOPSFC,COPSFC)
  COPSFC * W;
  virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  void SetParmList( );
  virtual bool ReadParm( char * ParmName, char * ParmValue, BYTE * pData );
  virtual void OnFailedTunningAssign( LPCSTR pszParm, LPCSTR pszValue );
  static void InitParm();
  NEWY
};
/////////////////////////////////////////////////////////////////
struct Y_DATA_API CYOPSBL : C_SFC_BASE_Impl
{
  struct CAlg * pAlg;
  CREATE(CYOPSBL,COPSBL)
  COPSBL * W;
  virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  void SetParmList( );
  virtual bool ReadParm( char * ParmName, char * ParmValue, BYTE * pData );
  virtual void OnFailedTunningAssign( LPCSTR pszParm, LPCSTR pszValue );
  static void InitParm();
  NEWY
};
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_ALM_R : public CY_Base
{
  struct SRepresentativeAlarm
  {
    char RangeOfAlarmProcessing;//1-4
    char SV[16*4];//0-2
    char ConnctionDestination[24*4];
  };
  //
  CALM_R * W;
  SRepresentativeAlarm representAlarms[16];
  CREATE(CY_ALM_R,CALM_R)
  virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  static void InitParm();
  virtual void SetParmList( );
  virtual bool SaveParms( HANDLE hFile );
  virtual bool RestParms( HANDLE hFile );
  virtual bool ReadParm( char * ParmName, char * ParmValue, BYTE * pData );
  NEWY
};

/////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_SPLIT_W
	{
  double BL1, BL2;
  char oldMODE;
	};

struct Y_DATA_API CY_SPLIT : public CY_Base, public CY_SPLIT_W
	{
  CSPLIT * W;
	CSignal * SET;
	CSignal Out[2];
	int kOut;
	CREATE(CY_SPLIT,CSPLIT)
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  int Clamp();
  static void InitParm();
  virtual void SetParmList( );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
	NEWY
	};

//////////////////////////////////////////////////////////////
struct Y_DATA_API CY_SRSX_X_W
{
	double O;
};

struct Y_DATA_API CY_SRSX_X : public CY_Base, public CY_SRSX_X_W
{
  CSRSN_X * W;
	CSignal * Q01;
	CSignal * Q02;
//
	CSignal Out[2];
	int kOut;
  int Type;

  CY_SRSX_X( char * Name, int Type, int number, int domain );
  static CY_Base * Create( char * Name, int Type, int number, int domain )
  {
    CY_SRSX_X * pCY_SRSX = new CY_SRSX_X( Name, Type, number, domain );
    return pCY_SRSX;
  };
  char * GetClass(){ return "CY_SRSX_X"; };
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  static void InitParm();
  virtual void SetParmList( );
	NEWY
};

//////////////////////////////////////////////////////////////
struct Y_DATA_API CY_SS_W
	{
	int SEL; //Selected number 0, 1, 2, 3 0
  int dummyIOP;
	};

struct Y_DATA_API CY_SS : public CY_Base, public CY_SS_W
{
  CSS_COMM * W;
	enum eType { L, M, H };
	eType m_Type;
	bool Bad1, Bad2, Bad3;
	CSignal * IN1;
	CSignal * IN2;
	CSignal * IN3;
//
	CSignal OUT;
  CY_IN ObrIn;
  CY_SS( char * Name, int Type, int number, int domain );
  static CY_Base * Create ( char * Name, int Type, int number, int domain )
  {
    CY_SS * pCY = new CY_SS( Name, Type, number, domain );
    return pCY;
  };
	double M_LIM;
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  void Init( char * Name, int number, int domain );
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
	void Signal_selection( );
	void SS_L( );
	void SS_H( );
	void SS_M( );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
	NEWY
};

/////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_SS_DUAL_W
	{
  int SEL;
	};

struct Y_DATA_API CY_SS_DUAL : public CY_Base, public CY_SS_DUAL_W
	{
  CSS_DUAL * W;
	bool Bad1, Bad2;
	CSignal * IN1;
	CSignal * IN2;
//
	CSignal OUT;
  CY_IN ObrIn;
	CREATE(CY_SS_DUAL,CSS_DUAL)
	CY_ALARM Alarm;
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual void ResetAlarms();
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
	void Signal_selection( );
  static void InitParm();
  virtual void SetParmList( );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
	NEWY
	};

/////////////////////////////////////////////////////////////////////

struct Y_DATA_API CY_ST16_W
{
  enum
  {
    eST16_Count = 64,
  };
  BYTE OldStateCond[eST16_Count];
  char curSTEP[4];
  CY_ST16_W();
};

#include <ST16_AC.h>

struct Y_DATA_API CY_ST16 : public CY_Base, public CY_ST16_W
{
  enum eTMNG_StartTiming : char
  {
    eTMNG_StartupAtInitialColdStart_Restart = 'I',
    eTMNG_PeriodicExecution = 'T',
    eTMNG_OneshotProcessing = 'O',
    eTMNG_RestrictedInitialExecution = 'B',
    eTMNG_OutputOnlyWhenConditionsChange = 'C',
    eTMNG_OutputEachTimeConditionsSatisfied = 'E',
  };

  struct Y_DATA_API S_TMNG
  {
    char StartTiming;
    char OutputTiming;
    BYTE ControlPeriod;
    BYTE ControlPhase;
    S_TMNG() : StartTiming(eTMNG_PeriodicExecution), ControlPeriod(1), ControlPhase(0)
    {}
  };

  enum eType { ST16, ST16E };
  eType Type;
  CST16 * W;
/////////////////////////////////////////////////////////////////////
	S_TMNG TimeType;
  int m_nWorkIf0;//во избежание зацикливания работать только по 0
////////////////////////////////////////////////////////////
	char Cond[eST16_Count][eST16_Count];
  char CondComm[eST16_Count][eST16_Count+8]; 
	char Act[eST16_Count][eST16_Count]; 
  char ActComm[eST16_Count][eST16_Count+8]; 
  char MatrCond[eST16_Count][eST16_Count];
  char MatrAct[eST16_Count][eST16_Count];
  char STEP[eST16_Count][4];
  char THEN[eST16_Count][4];
  char STEL[eST16_Count][4];
  DWORD nStepSaExec[eST16_Count];
  char NEXT[reserveNameLength], PREV[reserveNameLength], ORGN[reserveNameLength];
  bool bHasCondOnColumn[eST16_Count];//если нет условий в столбце, то выполнить действие с Y
  bool UseMatr;
  bool ActOnlyOnChange;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  CCond_ST16_LC64 VarCond[eST16_Count];
  CAct_ST16 VarAct[eST16_Count];
/////////////////////////////////////////////////////////////////////
  CSignal Out[16];
	int kOut;
  CY_Base* pNextEx, *pPrevEx, *pOrgn;
  bool bRestoredState;//для выявления первоначального запуска или продолжения работы из состояния - для отработки 'I'==TimeType
  int m_LastWorkStep;
	//CREATE(CY_ST16,CST16)
  CY_ST16( char * Name, int Type, int number, int domain );
  static CY_Base * Create( char * Name, int Type, int number, int domain )
  {
    CY_ST16 * pCY = new CY_ST16( Name, Type, number, domain );
    return pCY;
  };
  GET_CLASS(CY_ST16)
  int MaxState;
  int MaxCond;
  int MaxAct;
  BYTE StateCond[eST16_Count];
  BYTE YesAction[eST16_Count];//для выдачи через SA
	HWND hWnd;
//
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  virtual bool Draw ( CDrawData * pDD, int x, int y );
	virtual bool CreatePSMGraf( struct CGrafParm ** GP );
  static void InitParm();
  virtual void SetParmList( );
  virtual bool ReadParm( char * ParmName, char * ParmValue, BYTE * pData );
  virtual bool SaveParms( HANDLE hFile );
  virtual bool RestParms( HANDLE hFile );
  virtual bool GetVar( char * VarName, void ** pVar, void ** pTuneVar, char * Type, eVarType *Work, CAlarmBase::tFlagUse_ **ppFlagUse /*= NULL*/, USHORT *pVarSize /*= NULL*/ );
  virtual bool CallSA(LPCSTR pszParam);
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
  virtual bool EnumUsedObj( DWORD &pos, LPCSTR *ppName, CY_Base **ppObj );
	NEWY
protected:
  void GetAllRef();
  void ExecuteColumns( int from, int to );
  void RefreshConds();
  void FindNearestSTs();
};

////////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_SW_33_W
	{
	double OUT_1;
	double OUT_2;
	double OUT_3;
	};

struct Y_DATA_API CY_SW_33 : public CY_Base, public CY_SW_33_W
{
  enum eSignalRoute
  {
    esrUnknown = 0,
    esr0_to_123 = -1,
    esr0_from_123 = 1,
  };
  CSW_33 * W;
  eSignalRoute Dir10;
  eSignalRoute Dir20;
  eSignalRoute Dir30;
// SEL(S11,S12,S13)->S10
	CSignal ** nS10;
	CSignal * nS11;
	CSignal * nS12;
	CSignal * nS13;
// SEL(S21,S22,S23)->S20
	CSignal ** nS20;
	CSignal * nS21;
	CSignal * nS22;
	CSignal * nS23;
// SEL(S31,S32,S33)->S30
	CSignal ** nS30;
	CSignal * nS31;
	CSignal * nS32;
	CSignal * nS33;
//
// S10->S11,S12,S13
	CSignal * iS10;
	CSignal ** iS11;
	CSignal ** iS12;
	CSignal ** iS13;
// S20->S21,S22,S23
	CSignal * iS20;
	CSignal ** iS21;
	CSignal ** iS22;
	CSignal ** iS23;
// S30->S31,S32,S33
	CSignal * iS30;
	CSignal ** iS31;
	CSignal ** iS32;
	CSignal ** iS33;
//
	CREATE(CY_SW_33,CSW_33)
	bool SetOUT ( char * Name, CSignal ** pExtIN );
	bool SetIN ( char * Name, CSignal * pExtOUT );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
	void Answerback_Input();
	void AnswerbackCheck( double dt );
  static void InitParm();
  virtual void SetParmList( );
  void Transfer1to3( CSignal * i0, CSignal ** o1, CSignal ** o2, CSignal ** o3 );
  virtual char TestCNCT( LPSTR pConn );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
	NEWY
};

////////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_SW_91_W
	{
  CY_SW_91_W();
	double OUT;
  double OUT9[9];
	};

struct Y_DATA_API CY_SW_91 : public CY_Base, public CY_SW_91_W
	{
  CSW_91 * W;
	CSignal *iS11, *iS12, *iS13, *iS14, *iS15, *iS16, *iS17, *iS18, *iS19;
	CSignal oS10;

  CSignal oS1to19[9];
  CSignal *iS10;

  enum
  {
    eHZ,
    e19to0,
    e0to19,
  }b19to0;

	CREATE(CY_SW_91,CSW_91)
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
	void Answerback_Input();
	void AnswerbackCheck( double dt );
  static void InitParm();
  virtual void SetParmList( );
  void Work0to19 ( double dt );
  void Work19to0 ( double dt );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
	NEWY
	};

/////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_TM : public CY_Base
	{
  BYTE prevOP;
  BYTE prevPausBSTS;
  void checkOPstart_stop();
	CREATE(CY_TM,CTM)
  CTM * W;
	CSignal OUT;
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  virtual void OnAssigned( void *pAddr );
  virtual void OnNoAssignedByFalseState( void *pAddr, LPCVOID pCanByValue );
  virtual ELActLC64 IsLActionInLC64( LPCSTR pszAct );
	NEWY
	};

/////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_CTS : public CY_Base
	{
	CREATE(CY_CTS,CCTS)
  CCTS * W;
	CSignal OUT;
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  virtual bool GetVar( char * VarName, void ** pVar, void ** pTuneVar, char * Type, eVarType * Work, CAlarmBase::tFlagUse_ **ppFlagUse = NULL, USHORT *pVarSize = NULL );
	NEWY
	};
/////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_TON : public CY_Base
{
  int nBetween1IN;
  CTON * W;
	CSignal OUT;
  BYTE State;
	CSignal * IN;
	CREATE(CY_TON,CTON)
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
	NEWY
};

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_TOFF : public CY_Base
	{
  CTOFF * W;
	CSignal * IN;
	CSignal OUT;
  BYTE State;
	CREATE(CY_TOFF,CTOFF)
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
	NEWY
	};

/////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_TPCFL_W
{
	double dCPVn;
};

struct Y_DATA_API CY_TPCFL : public CY_Base, public CY_TPCFL_W
{
  CTPCFL * W;
	CSignal * IN;
	CSignal * Q01;
	CSignal * Q02;
	CSignal OUT, SUB;
	CREATE(CY_TPCFL,CTPCFL)
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
	NEWY
};

/////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_VELLIM_W
	{
	double 	dummyCSV;
	double dMVn;
	};

struct Y_DATA_API CY_VELLIM : public CY_Base, public CY_VELLIM_W
	{
  CVELLIM * W;
	CSignal * SET;
	CSignal * INT;
	CSignal OUT;
	CSignal SUB;
	CREATE(CY_VELLIM,CVELLIM)
	CY_ALARM Alarm;
	virtual CSignal * GetOUT ( char * Name );
	virtual CSignal ** GetIN ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual void ResetAlarms();
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
	NEWY
	};
//////////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_ANLGI : public CY_Base 
	{
  CS_ANLGI * W;
//
  CREATE(CY_ANLGI,CS_ANLGI)
  CY_ALARM Alarm;
	virtual CSignal ** GetIN ( char * Name );
	virtual CSignal * GetOUT ( char * Name );
	virtual void Work ( double dt );
  virtual void Init( );
  virtual void ResetAlarms();
  virtual int Prepare0();
  virtual int Prepare1();
	virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  virtual bool Draw ( CDrawData * pDD, int x, int y );
  virtual int SetData( int TypeData, void * pData );
  void InitData( );
  static void InitParm();
  virtual void SetParmList();
  NEWY
	};

struct Y_DATA_API CY_MOB_21 : public CY_Base 
{
  CS_MOB_21 * W;
  //
  CREATE(CY_MOB_21,CS_MOB_21)
  virtual CSignal ** GetIN ( char * Name );
  virtual CSignal * GetOUT ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  virtual bool Draw ( CDrawData * pDD, int x, int y );
  virtual int SetData( int TypeData, void * pData );
  void InitData( );
  static void InitParm();
  virtual void SetParmList();
  NEWY
};

struct Y_DATA_API CY_MOB_RS : public CY_Base 
{
  CS_MOB_RS * W;
  //
  CREATE(CY_MOB_RS,CS_MOB_RS)
  //CY_ALARM Alarm;
  virtual CSignal ** GetIN ( char * Name );
  virtual CSignal * GetOUT ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  virtual bool Draw ( CDrawData * pDD, int x, int y );
  virtual int SetData( int TypeData, void * pData );
  void InitData( );
  static void InitParm();
  virtual void SetParmList( );
  NEWY
};

struct Y_DATA_API CY_VEL : public CY_Base 
{
  CS_VEL * W;
  //
  CREATE(CY_VEL,CS_VEL)
  CY_ALARM Alarm;
  virtual CSignal ** GetIN ( char * Name );
  virtual CSignal * GetOUT ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  virtual bool Draw ( CDrawData * pDD, int x, int y );
  virtual int SetData( int TypeData, void * pData );
  virtual void ResetAlarms();
  void InitData( );
  static void InitParm();
  virtual void SetParmList();
  NEWY
};

struct Y_DATA_API CY_MOB_11 : public CY_Base 
{
  CS_MOB_11 * W;
  //
  CREATE(CY_MOB_11,CS_MOB_11)
  //CY_ALARM Alarm;
  virtual CSignal ** GetIN ( char * Name );
  virtual CSignal * GetOUT ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  virtual bool Draw ( CDrawData * pDD, int x, int y );
  virtual int SetData( int TypeData, void * pData );
  void InitData( );
  static void InitParm();
  virtual void SetParmList();
  NEWY
};

struct Y_DATA_API CY_OVR_B : public CY_Base 
{
  CS_OVR_B * W;
  //
  CREATE(CY_OVR_B,CS_OVR_B)
  CY_ALARM Alarm;
  virtual CSignal ** GetIN ( char * Name );
  virtual CSignal * GetOUT ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual void ResetAlarms();
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  virtual bool Draw ( CDrawData * pDD, int x, int y );
  virtual int SetData( int TypeData, void * pData );
  void InitData( );
  static void InitParm();
  virtual void SetParmList( );
  NEWY
};

struct Y_DATA_API CYS_PASSWD : public CY_Base 
{
  CS_PASSWD * W;
  //
  CREATE(CYS_PASSWD,CS_PASSWD)
  //CY_ALARM Alarm;
  virtual CSignal ** GetIN ( char * Name );
  virtual CSignal * GetOUT ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  virtual bool Draw ( CDrawData * pDD, int x, int y );
  virtual int SetData( int TypeData, void * pData );
  void InitData( );
  NEWY
};

struct Y_DATA_API CYS_ANLG_S : public CY_Base 
{
  CS_ANLG_S * W;
  //
  CREATE(CYS_ANLG_S,CS_ANLG_S)
  CY_ALARM Alarm;
  virtual CSignal ** GetIN ( char * Name );
  virtual CSignal * GetOUT ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual void ResetAlarms();
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  virtual bool Draw ( CDrawData * pDD, int x, int y );
  virtual int SetData( int TypeData, void * pData );
  void InitData( );
  static void InitParm();
  virtual void SetParmList( );
  NEWY
  };

struct Y_DATA_API CY_ANN_FUP : public CY_Base 
{
  CANN_FUP * W;
  //
  CREATE(CY_ANN_FUP,CANN_FUP)
  virtual CSignal ** GetIN ( char * Name );
  virtual CSignal * GetOUT ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  virtual bool Draw ( CDrawData * pDD, int x, int y );
  virtual int SetData( int TypeData, void * pData );
  void InitData( );
  static void InitParm();
  virtual void SetParmList( );
  NEWY
};

struct Y_DATA_API CY_S_MOA : public CY_Base 
{
  CS_MOA * W;
  //
  CREATE(CY_S_MOA,CS_MOA)
  CY_ALARM Alarm;
  virtual CSignal ** GetIN ( char * Name );
  virtual CSignal * GetOUT ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  virtual bool Draw ( CDrawData * pDD, int x, int y );
  virtual int SetData( int TypeData, void * pData );
  void InitData( );
  static void InitParm();
  virtual void SetParmList( );
  NEWY
};

struct Y_DATA_API CY_AVE_M_W
{
  double dCPVn;
  double samplesIn[60];
  double dtWait;
  int nSamplesReady;
  //
};

struct Y_DATA_API CY_AVE_M : public CY_Base, public CY_AVE_M_W
{
  CSignal *IN;
  CSignal OUT;
  CSignal SUB;
  CREATE(CY_AVE_M,CAVE_M)
  CAVE_M * W;
  virtual CSignal * GetOUT ( char * Name );
  virtual CSignal ** GetIN ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
  NEWY
};

struct Y_DATA_API CY_AVE_C_W
{
  double dCPVn;
};

struct Y_DATA_API CY_AVE_C : public CY_Base, public CY_AVE_C_W
{
  CSignal * IN;
  CSignal OUT;
  CSignal SUB;
  CREATE(CY_AVE_C,CAVE_C)
  CAVE_C * W;
  virtual CSignal * GetOUT ( char * Name );
  virtual CSignal ** GetIN ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  static void InitParm();
  virtual void SetParmList( );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
  NEWY
};

struct Y_DATA_API CY_GOV_B : public CY_Base 
{
  CS_GOV_B * W;
  //
  CREATE(CY_GOV_B,CS_GOV_B)
  CY_ALARM Alarm;
  virtual CSignal ** GetIN ( char * Name );
  virtual CSignal * GetOUT ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual void ResetAlarms();
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  virtual bool SetLimits( );
  virtual bool Draw ( CDrawData * pDD, int x, int y );
  virtual int SetData( int TypeData, void * pData );
  void InitData( );
  static void InitParm();
  virtual void SetParmList( );
  NEWY
};
//////////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_BSETU_2 : public CY_Base
{
  CBSETU_2 * W;
  //
  CSignal *IN, *INT;
  CSignal OUT, OUT2;
  //
  CY_ALARM Alarm;
  CREATE(CY_BSETU_2,CBSETU_2)
  virtual CSignal ** GetIN ( char * Name );
  virtual CSignal * GetOUT ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual void ResetAlarms();
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  void InitData( );
  static void InitParm();
  virtual void SetParmList( );
  virtual void OnFailedTunningAssign( LPCSTR pszParm, LPCSTR pszValue );
  NEWY
};
//////////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_ASTM1 : public CY_Base
{
  CASTM1 * W;
  //
  CSignal *IN, *Q01;
  CSignal OUT, SUB;
  //
  CREATE(CY_ASTM1,CASTM1)
  virtual CSignal ** GetIN ( char * Name );
  virtual CSignal * GetOUT ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  void InitData( );
  static void InitParm();
  virtual void SetParmList( );
  double Alpha(double T, double C);
  double Beta(double T, double C);
  NEWY
};
//////////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_ASTM2 : public CY_Base
{
  CASTM2 * W;
  //
  CSignal *IN, *Q01;
  CSignal OUT, SUB;
  //
  CREATE(CY_ASTM2,CASTM2)
  virtual CSignal ** GetIN ( char * Name );
  virtual CSignal * GetOUT ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  void InitData( );
  static void InitParm();
  virtual void SetParmList( );
  NEWY
};
//SIOS OPC>
//////////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_BOPCSYS : public CY_Base
{
  CBOPCSYS * W;
  //
  CREATE_BOPC(CY_BOPCSYS,CBOPCSYS)
  virtual CSignal ** GetIN ( char * Name );
  virtual CSignal * GetOUT ( char * Name );
  virtual void Work ( double dt );
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  void InitData( );
  static void InitParm();
  virtual void SetParmList( );
  NEWY
};
struct Y_DATA_API CY_BOPCCHR : public CY_Base
{
  CBOPCCHR * W;
  //
  CREATE_BOPC(CY_BOPCCHR,CBOPCCHR)
  virtual CSignal ** GetIN ( char * Name );
  virtual CSignal * GetOUT ( char * Name );
  virtual void Work ( double dt );
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  void InitData( );
  static void InitParm();
  virtual void SetParmList( );
  NEWY
};
struct Y_DATA_API CY_BOPCBOL : public CY_Base
{
  CBOPCBOL * W;
  //
  CREATE_BOPC(CY_BOPCBOL,CBOPCBOL)
  virtual CSignal ** GetIN ( char * Name );
  virtual CSignal * GetOUT ( char * Name );
  virtual void Work ( double dt );
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  void InitData( );
  static void InitParm();
  virtual void SetParmList( );
  NEWY
};
struct Y_DATA_API CY_BOPCI16 : public CY_Base
{
  CBOPCI16 * W;
  //
  CREATE_BOPC(CY_BOPCI16,CBOPCI16)
  virtual CSignal ** GetIN ( char * Name );
  virtual CSignal * GetOUT ( char * Name );
  virtual void Work ( double dt );
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  void InitData( );
  static void InitParm();
  virtual void SetParmList( );
  NEWY
};
struct Y_DATA_API CY_BOPCU16 : public CY_Base
{
  CBOPCU16 * W;
  //
  CREATE_BOPC(CY_BOPCU16,CBOPCU16)
  virtual CSignal ** GetIN ( char * Name );
  virtual CSignal * GetOUT ( char * Name );
  virtual void Work ( double dt );
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  void InitData( );
  static void InitParm();
  virtual void SetParmList( );
  NEWY
};
struct Y_DATA_API CY_BOPCI32 : public CY_Base
{
  CBOPCI32 * W;
  //
  CREATE_BOPC(CY_BOPCI32,CBOPCI32)
  virtual CSignal ** GetIN ( char * Name );
  virtual CSignal * GetOUT ( char * Name );
  virtual void Work ( double dt );
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  void InitData( );
  static void InitParm();
  virtual void SetParmList( );
  NEWY
};
struct Y_DATA_API CY_BOPCU32 : public CY_Base
{
  CBOPCU32 * W;
  //
  CREATE_BOPC(CY_BOPCU32,CBOPCU32)
  virtual CSignal ** GetIN ( char * Name );
  virtual CSignal * GetOUT ( char * Name );
  virtual void Work ( double dt );
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  void InitData( );
  static void InitParm();
  virtual void SetParmList( );
  NEWY
};
struct Y_DATA_API CY_BOPCF32 : public CY_Base
{
  CBOPCF32 * W;
  //
  CREATE_BOPC(CY_BOPCF32,CBOPCF32)
  virtual CSignal ** GetIN ( char * Name );
  virtual CSignal * GetOUT ( char * Name );
  virtual void Work ( double dt );
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  void InitData( );
  static void InitParm();
  virtual void SetParmList( );
  NEWY
};
struct Y_DATA_API CY_BOPCF64 : public CY_Base
{
  CBOPCF64 * W;
  //
  CREATE_BOPC(CY_BOPCF64,CBOPCF64)
  virtual CSignal ** GetIN ( char * Name );
  virtual CSignal * GetOUT ( char * Name );
  virtual void Work ( double dt );
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  void InitData( );
  static void InitParm();
  virtual void SetParmList( );
  NEWY
};
struct Y_DATA_API CY_SR_FNCVR : public CY_Base
{
  CSR_FNCVR * W;
  //
  CREATE_BOPC(CY_SR_FNCVR,CSR_FNCVR)
  virtual CSignal ** GetIN ( char * Name );
  virtual CSignal * GetOUT ( char * Name );
  virtual void Work ( double dt );
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  void InitData( );
  static void InitParm();
  virtual void SetParmList( );
  NEWY
};
struct Y_DATA_API CY_SR_PID : public CY_Base
{
  CSR_PID * W;
  //
  CREATE_BOPC(CY_SR_PID,CSR_PID)
  virtual CSignal ** GetIN ( char * Name );
  virtual CSignal * GetOUT ( char * Name );
  virtual void Work ( double dt );
  virtual int GetParams( KYBridge2SysParam &params );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
  void InitData( );
  static void InitParm();
  virtual void SetParmList( );
  NEWY
};
//<SIOS OPC
/////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_BSI : CY_Base
{
  enum eType
  {
    NO,
    LAMPBUTN,
    LAMP,
    BUTTON,
  };
  enum
  {
    nElements = 3,
  };
  struct SFPSW
  {
    eType btnType;
    char szOff[12*4];
    char szOn[12*4];
    int clrOff;
    int clrOn;
  }fpsw[nElements];

  CSignal * JMOD;
  CSignal * JBST;
  CSignal * JALM;

  CSignal * SET;

  CSignal * Q01;
  CSignal * Q02;
  CSignal * Q03;
  CSignal * INT;

  CSignal * Q[nElements];
  int * PV[nElements];

  CY_ALARM Alarm;
  CBSI * W;
  CREATE(CY_BSI,CBSI)
  virtual CSignal * GetOUT ( char * Name );
  virtual CSignal ** GetIN ( char * Name );
  virtual void Work ( double dt );
  virtual void Init( );
  virtual void ResetAlarms();
  virtual int Prepare0();
  virtual int Prepare1();
  virtual int GetParams( KYBridge2SysParam &params );
  virtual bool SetLimits( );
  void SetParmList( );
  virtual bool ReadParm( char * ParmName, char * ParmValue, BYTE * pData );
  virtual bool SaveParms( HANDLE hFile );
  virtual bool RestParms( HANDLE hFile );
  virtual int StateSave( IYokoStateSer* pStateSer );
  virtual int StateRestore( IYokoStateSer* pStateSer );
  static void InitParm();
  virtual bool SetResetAlrmFromExtern( int alrmNumber, bool bSet );
  NEWY
};