#pragma once
#include "./Y_export.h"
#include "./DataTypes_.h"
#include <_structmacros.h>
#include <rsuErr.h>
#include <YokogawaStruct.h>
#include <MemoryServiceY.h>
#include <Y_Signal.h>

#undef OUT
#undef IN


/////////////////////////////////////////////////////////////////////
struct Y_DATA_API CZ_OBJ
  {
  char Name[17];
	bool Used;
	int Numb;
	int Type;
	union
		{
		C_Z * pZ;
		C_AN * pAN;
		C_WB * pWB;
		C_GS * pGS;
		C_SW * pSW;
    C_Y * pY;
		};
	int PV_0, PV_1, PV_old;
  CSignal Out[3];
  CSignal * IN;
	struct CY_Base * pMain;
  char Comment[33];
////
	CZ_OBJ();
  CSignal * GetOUT ( char * Name );
	CSignal ** GetIN ( );
	int * pPV();
	int PV();
	char * Struct();
  };

/////////////////////////////////////////////////////////////////////
struct Y_DATA_API CY_IN
  {
  double * pSH, * pSL;
  bool * pPV_Overshoot;
  bool * pPV_Limit;
  double BIAS;
  double GAIN;
  CY_IN();
	void Work ( CSignal * IN, double & PV, double & PV_old, BYTE cali );
  void WorkScaled( CSignal * IN, double & PV, double & PV_old, double A, BYTE cali );
  void PV_Overshoot ( CSignal * IN, double & PV, bool * pBad, BYTE cali );
  };

struct Y_DATA_API CY_OUT_W
{
	bool IsMLe;
	bool IsMHe;
	double MLe;
	double MHe;
	int DataStatus;
	int OutputStatus;
};

class Y_DATA_API CY_OUT : public CY_OUT_W
{
	void ModeAC( bool Reverse );
	void ModeM( bool Reverse );
	void Clamp( );
	public:
// 
	bool OutputLimiter;
	double *pMH, *pML;
	double *pMSH, *pMSL;
	double *pMV, *pdMV;
//-----------------------------------
	struct CSignal * OUT;
  bool *pIMAN;
  bool *pOutTrk;
  SMode::tMode * pMode;
  bool *pMAN_bypass;
  double *pOut_VL;
  bool *pLimit_On_CLP;
  int *pPositional;
  double minusClampVal, plusClampVal;
	CY_OUT();
  void OutputProcessing( bool Reverse, bool TRK = false );
  int SaveState( );
  int RestoreState ( char * StrName );
  int StateSave( IYokoStateSer* pStateSer );
  int StateRestore( IYokoStateSer* pStateSer );
  void CorrectMVbyNullDMV();
};


typedef CSignal * (*tFindRef) ( char * ObjName, char * NodeName );
extern tFindRef Y_DATA_API pFindRef;

struct C_GeomBlk
	{
  BYTE Type;
  BYTE Symbol;
  char ObjName[32];
	int nObj;
	struct CZ_OBJ * pDiskrete;
	RECT rc;
  int x, y;
  };

struct IConnectionsImpl
{
  virtual void ConnectionExec( int firstC, int untilC, double dt, char dirIO ) = 0;
};

struct Y_DATA_API CY_DR
{
  enum
  {
    MAX_DR_PNT = 500,
  };
  char FCS[32];
  int domain;
  int NumbFCS;
  char Comment[80];
	POINT LinePnt[MAX_DR_PNT];
	int kLinePnt;
  HWND hWnd;
	int sx, sy;
  int nYObjects;
  int nPIOObjects;
  CY_Base **m_pBlks;
  CY_Base **m_pPios;
  static const DWORD s_dwFcsNumModulus;//модуль для выделения номера FCS
  CY_DR()
  {
    memset ( this, 0, sizeof ( CY_DR ));
  }
  void Prepare0();
  void AddYBlk( CY_Base *pY );
  void AddPIO( CY_Base *pPIO );
  void WorkYBlks( BYTE Level, double dt, IConnectionsImpl *pCI );
  NEWY
};

#include <Y_Class.h>

typedef CY_Base * (*tYCreateCP)( char * Name, char * Parm, int number, int domain ); 
typedef CY_Base * (*tYCreateIP)( char * Name, int Parm, int number, int domain ); 
typedef CY_Base * (*tYCreateNP)( char * Name, int number, int domain ); 

struct Y_DATA_API CY_Create
{
	static CY_Create * pFirst;
	static CY_Create * pLast;
	CY_Create * pNext;
//
	char * ModelName;
	char * Parm;
	int iParm;
	tYCreateCP pCreateCP;
	tYCreateIP pCreateIP;
	tYCreateNP pCreateNP;
//
	CY_Create( char * ModelName,	tYCreateNP pCreate );
	CY_Create( char * ModelName,	tYCreateCP pCreateCP, char * Parm );
	CY_Create( char * ModelName,	tYCreateIP pCreateIP, int Parm );
  static CY_Base * Create ( char * ModelName, char * ObjName, int number, int domain );
};

int GetMes ( char * cMes );
void Y_DATA_API SetTunningValues( CY_Base * pY, char * Params );

#define SENS_TO_FILE

void SetMODE( SMode::tMode & M );

struct Y_DATA_API STrace
  {
	static STrace * pFirst;
	static STrace * pLast;
	STrace * pNext;
//
	char * Model;
  char * Name;
  char * File;
  int Line;
	STrace( );
	static char * ConvModel ( char * pModel );
  };

#undef BP
#define BP(Pnt) \
	{\
	static STrace T;\
  if ( T.Name == NULL )\
	  {\
		T.Model = T.ConvModel ( Model );\
		T.Name = #Pnt;\
		T.File = __FILE__;\
		T.Line = __LINE__;\
		}\
	}\
if ( Stop )

struct Y_DATA_API CSensDefect
  {
	static CSensDefect * pFirst;
	static CSensDefect * pLast;
	CSensDefect * pNext;
//
  int Defect;
  char Name[64];
  double Min;
  double Max;
	CSensDefect( );
  };
