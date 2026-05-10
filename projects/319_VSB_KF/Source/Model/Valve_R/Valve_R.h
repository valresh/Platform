#pragma once

#include "BaseInt.h"
#include "Data.h"
//#include "SR.h"
#include "Defects.h"
#include "ACS_Def.h"
#include "PressDrop.h"

struct _W_Valve_R
  {
  double dP;
  double Pin, Pout;
  bool Forvard;
  _W_Valve_R();
  };

class CValve_R : public IBaseModel, public IUniModel, public _W_Valve_R
	{
//
public:
	CValve_R( char * ObjName );
	virtual ~CValve_R();
// Интерфейс IBaseModel
	int Init( int Regim );
	int Step0( );
	int Step1( );
	int StepT(double dt);
	int GetParams( char * );
	int ShowParams( DWORD DataType, struct CShowData * pSD );
  int UpdateParam( struct CParams & Param );
  int SaveState ( );
  int RestoreState ( char * StrName );
  int SetData( int TypeData, void * pData );
  int OnDefect( CDef * pDefect );
///////////////////////////////////////////////////
  int TestNodes( int kNodes, struct CObjectPoint ** ppNodes );
  //  int GydroInit( IBaseModel * pGydro, int PntGroup, struct CNodeData * pInfo );
  int SetMatr( void * pExternals, int PntGroup,  double dt, 
    double A[/* kExkV */], double B[ /*kE*/ ] );
  int SetVar( void * pExternals, int PntGroup, double dt, 
    double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] );
	NEW
/////////////////////////////////////////////////
#include "IO_Create.h"
#include "Points.h"
#include "Valve_R_Defects.h"
#include "ACS_Create.h"
#include "ACS.h"
  bool DefOpen;
  CPressDrop Перепад;
  CFlow * pFlow_In;
/////////////////////////////////////////////////
//
  void Calc ( double dt );
	};
