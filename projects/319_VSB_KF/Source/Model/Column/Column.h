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
#include "BaseCol.h"
#include "Control.h"

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

struct IN_DLL I_ColAdditionList : public I_AdditionList
  {
  I_Addition * pFirst;
  I_Addition * pLast;
  void Init( );
  void InitAdditions( void * pData );
  void Add( I_Addition * pAdd );
  void Gamma(  void * pData, double P, double T, bool IsComp[K_GAS], double Cmol[K_GAS], double P_sat_id[K_GAS], double Gamma[K_GAS], double & dH_liq );
  void GetParams( IBaseModel * pMain );
  void UpdateParams( );
  };

struct IN_DLL _W_Column
  {
  double Press[MAX_IO];
  double dP_hydro;
  double kGasOut;
  double Hydro_dP[MAX_IO];
  _W_Column();
  };

class IN_DLL CColumn : public IBaseModel, public IUniModel, public _W_Column
	{
//
public:
	CColumn( char * ObjName );
	virtual ~CColumn();
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
    return Column.GetPoint( PntName );
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
#include "Column_Defects.h"
#include "ACS_Create.h"
#include "ACS.h"
#undef PROF
#undef LONG
#undef EPROF
#undef ELONG
//
  CBaseCol Column;
//
  CNode * pNodes;
  int kIO;
  int kStage;
  int n1St;
  double * Omega;
  double * Hydro_dPmax;
  int * nST;
  double * Flow_kg;
	CExtIO ** Src;
/////////////////////////////////////////////////
  double * OmegaHeatStage;
  double * KSHeatStage;
  int kHeatStage;
/////////////////////////////////////////////////
  bool GasColumn;
  CFlow * pFlow_Gas_in;
  CFlow * pFlow_Gas_out;
  int nGasNode_x;
  int nGasNode_1;
  CFlow * pFlow_Heat_in;
  CFlow * pFlow_Heat_out;
  CFlow * pFlow_ExtHeat_in[MAX_HEAT];
  CFlow * pFlow_ExtHeat_out[MAX_HEAT];
  COmegaLevel * pLevCtrl;
  double FlowQ;
/////////////////////////////////////////////////
//
  virtual void Calc ( double dt );
	virtual void InitCol( bool WaterCol );
  void SaveCoef( );
  void SetAB( );
	};
