#pragma once

#include "BaseInt.h"
#include "Connect.h"
#include "Data.h"
#include "CommProc.h"
#include "SysDataTypes.h"

#ifdef LINUX
#include <QtCore/qglobal.h>
#ifdef UTILS_EXPORTS
#define IN_DLL Q_DECL_EXPORT
#else
#define IN_DLL Q_DECL_IMPORT
#endif
#else
#undef IN_DLL
#ifdef DLL_IntReg
#undef IN_DLL
#define IN_DLL __declspec(dllexport)
#else
#define IN_DLL __declspec(dllimport)
#endif
#endif

struct CIntReg_W
  {
  bool Use;
  double Value;
  CIntReg_W();
  };

class IN_DLL CIntReg	: public IBaseModel, public CIntReg_W, public ISet
	{
	public:
#define NO_DEFECT
#include "IO_Create.h"
#undef NO_DEFECT
  bool Error;
  bool PID;
  double PID_P, PID_I;
  double En1, En;
  bool Work;
  bool NotRelease;
  static bool PrintUsed;
//
	CIntReg( IBaseModel * pMain, char * Name );
  int Step0();
  int Step1();
	int GetParams( char * );
  int UpdateParam( struct CParams & Param );
  int SaveState ( );
  int RestoreState ( char * StrName );
  operator double() { return Value; };
  double operator = ( double V ) { Value = V; return Value; };
  void Set ( double Parm ) { if ( Parm != 0. ) { Use = true; Value = Parm; } }; 
  bool IsOn() { if ( pSys ->bInit_0 ) return false;  return Use; };
  void Off() { Use = false; };
  void On() { Use = true; };
  void GetParam() { GetParams( NULL ); };
  void OutValue ( char Txt[64] );
  bool Reg( double dt, double PV, double * MV );
	};

struct CSetComp_W
  {
  bool Use;
  CSetComp_W();
  };

struct CWComp
  {
  CComp Comp;
  char CompName[32];
  CWComp();
  };

class IN_DLL CSetComp	: public IBaseModel, public CSetComp_W, public ISet
{
public:
#define NO_DEFECT
#include "IO_Create.h"
#undef NO_DEFECT
  CWComp * W;
  char TestCompName[32];
  CSetComp( IBaseModel * pMain, char * Name );
  int GetParams( char * );
  int UpdateParam( struct CParams & Param );
  int SaveState ( );
  int RestoreState ( char * StrName );
  int SetData( int TypeData, void * pData );
  void GetParam() { GetParams( NULL ); };
  bool Set( CComp * pComp );
  bool IsOn(){return Use;};
  void Off(){Use = false;};
  void On(){Use = true;};
  void OutValue ( char Txt[64] );
  void Set ( double Parm ){};
  void Create( );
  };

#define MAX_REG_NAME 64
#define MAX_REF_NAME 64


class IN_DLL CExtRegOut
  {
  public:
  CExtRegOut * pNextOut;
  char Ref[MAX_REG_NAME];
  double MV_0, MV_1;
//
  double * pMV;
  };

struct CExtReg_W
{
	double SP, PV, OP;
	double dV, dVm;
	double Delta;
	CExtReg_W(){memset(this, 0, sizeof(*this));}
};

class IN_DLL CExtReg : public CExtReg_W
  {
  public:
  char Name[MAX_REG_NAME];
  char Ref[MAX_REG_NAME];
  double K, T, TD;
  double Min, Max;
  bool On;
  double TrendScale;
  int TrendStep;
//
  double * pPV;
  int nTrend;
  CExtRegOut * pFirstOut;
  CExtReg * pNextExtReg; 
//
  void StepT ( double dt );
  };

void IN_DLL CTRL_Params( char * TabName, IBaseModel * pModel, const char * Descr = NULL );

