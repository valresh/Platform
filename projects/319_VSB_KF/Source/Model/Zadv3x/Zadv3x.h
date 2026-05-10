#pragma once

#include "BaseInt.h"
#include "DataTypes.h"
//#include "SysDataTypes.h"
//#include "SR.h"
//#include "GydroData.h"
#include "Data.h"
//#include "Chem.h"
//#include "Defects.h"
#include "ACS_Def.h"

struct _W
  {
  double O;
  _W();
  };

class CZadv3x : public IBaseModel, public IUniModel, public _W
	{
//
public:
	CZadv3x( char * ObjName );
	virtual ~CZadv3x();
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
  int SetMatr( void * pExternals, int PntGroup,  double dt, 
    double A[/* kExkV */], double B[ /*kE*/ ] );
  int SetVar( void * pExternals, int PntGroup, double dt, 
    double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] );
	NEW
/////////////////////////////////////////////////
//#include "IO_Conn.h"
#include "IO_Create.h"
#include "Points.h"
#include "ACS_Create.h"
#include "ACS.h"
//  CZadv * pZadv;
  CFlow * pFlow[3];
  int Type;
  double O01, O02, Flow_0, Flow_1, Flow_2;
/////////////////////////////////////////////////
//
  void Calc ( double dt );
	};
