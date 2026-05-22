#pragma once

#include "BaseInt.h"
//#include "DataTypes.h"
//#include "SysDataTypes.h"
//#include "SR.h"
//#include "SelVar.h"
#include "Data.h"
#include "Chem.h"
#include "Defects.h"
#include "ACS_Def.h"
#include "BaseCol_NE.h"
#include "Control.h"
#include "Extensions.h"

#define MAX_IO 32
#define MAX_ACS 32

#ifdef LINUX
#include <QtCore/qglobal.h>
#ifdef UTILS_EXPORTS
#define IN_DLL Q_DECL_EXPORT
#else
#define IN_DLL Q_DECL_IMPORT
#endif
#else
#undef IN_DLL
#ifdef DLL_COLUMN
#undef IN_DLL
#define IN_DLL Q_DECL_EXPORT
#else
#define IN_DLL __declspec(dllimport)
#endif
#endif

struct IN_DLL _W_Column
  {
  double Press[MAX_IO];
  double L_уровнемер;
  double Flow_Cub_out;
  _W_Column();
  };

struct IN_DLL CXNode
  {
  CExtIO * pSrc;
  int nStage;
  };


struct ICreateColumnExtensions : IExtension
  {
  class CColumn_NE * pColumn;
  bool ReadData( IBaseModel * pModel );
  };

//struct CSaveData
//  {
//  double a, b, c, d, Ksi;
//  };

class IN_DLL CColumn_NE : public IBaseModel, public IUniModel, public _W_Column
	{
//
public:
	CColumn_NE( char * ObjName );
	virtual ~CColumn_NE();
// Интерфейс IBaseModel
	virtual int Init( int Regim );
	virtual int Step0( );
	virtual int Step1( );
	virtual int StepT(double dt);
	virtual int GetParams( char *  );
	virtual int ShowParams( DWORD DataType, struct CShowData * pSD );
  virtual int UpdateParam( struct CParams & Param );
  virtual int SaveState ( );
  virtual int RestoreState ( char * StrName );
  virtual int SetData( int TypeData, void * pData );
  virtual int OnDefect( CDef * pDefect );
  void DrawObj ( struct CDrawObjData * pDraw );
  virtual void AddResources (CGDIResourceMgr *pResMgr);
  virtual void DrawObject ( struct CDrawObjData * pDraw, CGDIResourceMgr *pResMgr);
  virtual BOOL NeedDrawBefore() { return TRUE; }
///////////////////////////////////////////////////
  int TestNodes( int kNodes, struct CObjectPoint ** ppNodes );
  int SetMatr( void * pExternals, int PntGroup,  double dt, 
    double A[/* kExkV */], double B[ /*kE*/ ] );
  int SetVar( void * pExternals, int PntGroup, double dt, 
    double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] );
  CExtern_Pnt * GetPoint( const char * PntName )
    {
		CExtern_Pnt* pPnt = Column.GetPoint( PntName );
		if( pPnt )
			return pPnt;
		else
			return IBaseModel::GetPoint(PntName);
    }
	NEW
/////////////////////////////////////////////////
//#include "IO_Conn.h"
#define PROF 
#define LONG 
#define EPROF
#define ELONG
#include "IO_Create.h"
#include "Points.h"
#include "ACS_Create.h"
#include "ACS.h"
//
  CBaseCol_NE Column;
  COmegaLevel * pLevCtrl;
  bool Use_LevCtrl;
  int nRegCtrl;
//
  CNode * pNodes;
  int kIO;
  int kStage;
  int n1St_out;
  int nCub_out;
  double FlowGasOut;
  double PressOut;
  int * nST;
  double * Flow_kg;
	CExtIO ** Src;
  double * EpsSrc;
  CKarman ** ppKapmans;
  int kKarman;
/////////////////////////////////////////////////
  double * OmegaHeatStage;
  double * KSHeatStage;
  int kHeatStage;
/////////////////////////////////////////////////
  CFlow * pFlow_Heat_in;
  CFlow * pFlow_Heat_out;
  CFlow * pFlow_ExtHeat_in[MAX_HEAT];
  CFlow * pFlow_ExtHeat_out[MAX_HEAT];
  double FlowQ;
  int kStep;
  IIntegral_Ctrl * pCtrlLev;
  __int64 ReplaceParams;
  __int64 ParamFrame;
  double Cmol_N1t, Cmol_N2t, Cmol_N1b, Cmol_N2b;
/////////////////////////////////////////////////
  ICreateColumnExtensions * pCreateColumnExtensions;
  double Calc_C[K_GAS];
  int nAir;
  bool Absorber;
  bool Desorber;
//  CParamExtension ColCommonGamma;
//  CParamExtension ColAbsGamma;
//  CParamExtension ColumnGamma;
//  CParamExtension ColReactions;
//
  virtual void Calc ( double dt );
  void SetPress( double dt, double Var[], struct CBaseStruct * pVar[] );
  void GetGasFlowSrc ( double * Vs );
  void GetStaticFlowSrc ( );
	void InitCol( );
//  void NewCommonGamma();
//  void NewAbsGamma();
  void NewColumnGamma();
  void NewReactions( char * Dll, char * Point );
  void InitCommonGamma();
  void InitAbsGamma();
  void InitColumnGamma();
  void InitReactions();
// void SetFlows( );
//////////////////////////////////////////////////////
  void CalcPress ( double dt );
  double TestCalcPress ( double TC );
  double TestCalcTemp ( double Pabs );
	};
