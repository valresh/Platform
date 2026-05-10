#pragma once

#include "BaseInt.h"
#include "DataTypes.h"
#include "SysDataTypes.h"
#include "SR.h"
#include "Data.h"
#include "Chem.h"
#include "Defects.h"
#include "EpsVol.h"
#include "ACS_Def.h"

struct _W
  {
  double T;
  _W();
  };

class CAirDry : public IBaseModel, public IUniModel, public _W
	{
public:
	CAirDry( char * ObjName );
	virtual ~CAirDry();
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
 
#include "IO_Create.h"
#include "Points.h"
#include "AirDry_Defects.h"
#include "ACS_Create.h"
#include "ACS.h"
/////////////////////////////////////////////////
 //
	CFlow * pFlow_in;
	CFlow * pFlow_out;
	CSEps In;
	int ID_Gas_out;
	CComp * cGas_out;
	double FlowQ;
/////////////////////////////////////////////////
//
  void Calc ( double dt );
	};
