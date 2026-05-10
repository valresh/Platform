#pragma once

#include "BaseInt.h"
#include "DataTypes.h"
#include "SysDataTypes.h"
#include "SR.h"
#include "Chem.h"
#include "Data.h"
#include "UniHeat.h"
#include "ACS_Def.h"

#undef _W
#define _W CFakel_W

struct CFurnace_W 
  {
  double T_fire;
  bool IsFire;
  };

struct CFurnace : public IBaseModel, public CFurnace_W
  {
  bool IsComp[K_GAS];
  double F_in[K_GAS];
  double F_smoke[K_GAS];
  double FSumm, HSumm;
  static double Sigma;
  static double kkO2;
  bool SetFire;
//
  CFurnace( char * _Name, IBaseModel * _pMain ) : IBaseModel ( _Name, _pMain )
    {
    T_fire = 15.;
    IsFire = false;
    SetFire = false;
    Init( );
    }

  void Init( )
    {
    CLEAR(IsComp)
    CLEAR( F_in )
    FSumm = 0.;
    HSumm = 0.;
    }
  int SaveState( );
  int RestoreState ( char * StrName );
  int GetParams( char * );
  void AddFlow( CFlow * pFlow );
  void Calc( double dt );
  double NeedO2( );
  void Smoke( );
  };


struct _W
  {
//
  double F_Smoke;
  _W();
  };


class CFakel : public IBaseModel, public IUniModel, public _W
	{
//
public:
	CFakel( LPCTSTR ObjName );
	virtual ~CFakel();
// Интерфейс IBaseModel
  int Init( int Regim );
  int Step0( );
  int Step1( );
  int StepT(double dt);
  int GetParams( char * );
  int UpdateParam( struct CParams & Param );
  int SaveState ( );
  int RestoreState ( char * StrName );
  int SetData( int TypeData, void * pData );
  int OnDefect( CDef * pDefect );
  ///////////////////////////////////////////////////
  int TestNodes( int kNodes, struct CObjectPoint ** ppNodes );
  int SetMatr( void * pExternals, int PntGroup,  double dt, 
    double A[/* kExkV */], double B[ /*kE*/ ] );
  int SetVar( void * pExternals, int PntGroup, double dt, 
    double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] );
  NEW
/////////////////////////////////////////////////
#include "IO_Create.h"
#include "Points.h"
#include "ACS_Create.h"
#include "ACS.h"
/////////////////////////////////////////////////
  CFurnace Furnace;
//
  CFlow * pGas_in;
  CFlow * pFuel_in;
  CFlow * pSteam_in;
  bool IsComp[K_GAS];
  double F_smoke[K_GAS];
//
//
  void Calc ( double dt );
  void Fire( double dt );
  double NeedO2( );
  void Smoke( );
	};
