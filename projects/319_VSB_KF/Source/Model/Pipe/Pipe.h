#pragma once

#include "BaseInt.h"
#include "Data.h"
#include "EpsVol.h"
#include "Defects.h"
#include "ACS_Def.h"
#include "IntReg.h"
#include "Control.h"
#ifdef LINUX
#include "../Drainage/Drainage.h"
#else
#include "..\Drainage\Drainage.h"
#endif

#include "Extensions.h"

//#define SPECIAL
#define N2

struct CNode
  {
  CStr ObjName;
  CStr Point;
  int Drainage;
  };

struct CShowPipeParams : public IShowParam
  {
  class CPipe * pPipe;
  void Init ( class CPipe * pPipe );
  void Show ( struct CParams * Param );
  double Show_Dbl ( struct CParams * Param ) { return *(double*) Param->Addr; };
  CShowPipeParams();
  };

struct _W
  {
  double P;     // Давление кг/см2
  double T;     // температура в градусах Цельсия
  double To_m3; // перевод в м3
  double To_kg; // перевод в кг
  double h_Flow;// кДж/кмоль
  double F_mol;
  bool IsComp_In[K_GAS];
  double Cmol_In[K_GAS];
  double Flow_Drain;
  double Cmol_Out[K_GAS];
//
  _W();
  };

class CCatPipe : public IBaseModel, public IUniModel, public IPoolCtrlModel, public _W
  {
  public:
//
  double P, T, Flow_kg;
  char * Type;
  double * Matr;
  bool IsMF;
  CFlow ** pFlow;
  int kF;
//
  CCatPipe( char * ObjName );
  virtual ~CCatPipe();
  int StepT(double dt);
  int GetParams(  char * StrName );
  virtual int TestNodes( int kNodes, struct CObjectPoint ** ppNodes );
  virtual int SetMatr( void * pExternals, int PntGroup,  double dt, 
    double A[/* kExkV */], double B[ /*kE*/ ] );   // A * X = B
  virtual int SetVar( void * pExternals, int PntGroup, double dt, 
    // Переменные на входах
    double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] );
  int InitPool ( struct CVarPool * pPool, struct CGroupObject * pPoolObject );
  };

class CPipe : public IBaseModel, public IUniModel, public IPoolCtrlModel, public _W
	{
//
public:
	CPipe( char * ObjName );
	virtual ~CPipe();
// Интерфейс IBaseModel
  int Init( int );
	int Step0( );
	int Step1( );
	int StepT(double dt);
	int GetParams(  char * StrName );
	int ShowParams( DWORD DataType, struct CShowData * pSD );
  int UpdateParam( struct CParams & Param );
  int SaveState ( );
  int RestoreState ( char * StrName );
  int SetData( int TypeData, void * pData );
  int OnDefect( CDef * pDefect );
///////////////////////////////////////////////////
///////////////////////////////////////////////////
  virtual int TestNodes( int kNodes, struct CObjectPoint ** ppNodes );
  virtual int SetMatr( void * pExternals, int PntGroup,  double dt, 
    double A[/* kExkV */], double B[ /*kE*/ ] );   // A * X = B
  virtual int SetVar( void * pExternals, int PntGroup, double dt, 
    // Переменные на входах
    double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] );
  virtual bool GetParamsList( struct CParams ** pParams, int * kParams );
  static CParams * pParams;
  static int kParams;
  static bool FormListParams;
  NEW
/////////////////////////////////////////////////
//#include "IO_Conn.h"
#include "IO_Create.h"
#include "Points.h"
#include "Pipe_Defects.h"
#include "ACS_Create.h"
#include "ACS.h"
  bool IsMF;
  int kF;
  double * Matr;
  CNode * pNodes;
  char * Type;
  CFlow ** pFlow;
  double * h_F;
  CComp ** pComp_in;
  double * Flow_mol;
  double * Flow_kg;
  double * Test_Flow_mol;
  bool Alarm_Flow_mol;
//  struct CFiltrFlow * Filtr_Flow;
//  double Max_Delta_P;
//  struct CFiltrFlow ** pFiltr;
  CComp * pComp_out;
  int ID_Comp_out;
  bool Out_Drain;
  CComp * pComp_out_drain; // С учетом отбора дренажами
  int ID_Comp_out_drain;
  double h_Out_drain;
  class CDrainage ** pDrainModel;
  int kDrain;
  double V_drain_g;
  double V_drain_l;
  double V_drain;
  double P_Calc_State;
  double Flow_C[K_GAS];
//  static char PipeMsg[128];
#ifdef N2
  static bool Show_N2;
  static double Limit_N2;
  bool IsN2;
#endif
/////////////////////////////////////////////////
  CSEps S_State;
  CSEps S_Cool;
  int kStep;
  bool TempCalcT;
  int nFlow_C;
  bool Razgerm;
  double Omega_Razgerm;
  double FlowQ;
  static double Q_max; 
  double Vol;
  bool SaveFlow;
  //IBaseModel * pBaseObj;
  //CAnalogControl * pControl_P;
  //CAnalogControl * pControl_F;
  //CStableControl * pControl_Stable;
  //CValue *pRazgerm;
  double F_Razgerm_max;
  CSEps S_Razgerm;
  CShowP ShowP;
  double _P;
  CShowF ShowFlow;
  CFlow FlowSumm;
  CShowF ** pShowFlow;
  char * FlowNames;
  CShowPipeParams Show;
  IEtalonComp * pEtalonComp;
//??  int G_Сульфолан;
//??  int G_Триэтиленгликоль;
  static bool Очищать;
  static bool UseAlt_Name;
/////////////////////////////////////////////////
//
  int InitPool ( struct CVarPool * pPool, struct CGroupObject * pPoolObject );
  void Calc ( double dt );
  void MixFlow( double dt );
//  void MixSolid( double dt );
  void OutFlow( );
  void CalcHeat ( double dt );
  void Compress( );
  void SetDrain(  );
  void SetState( );
  void PrepareParams ( struct CParams * Param );
};

#define MAX_FLOW 150000
#define MAX_PRESS 500
