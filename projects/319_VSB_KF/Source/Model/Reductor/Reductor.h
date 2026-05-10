#pragma once

#include "BaseInt.h"
//#include "DataTypes.h"
//#include "SysDataTypes.h"
//#include "SR.h"
//#include "GydroData.h"
#include "Data.h"
//#include "Chem.h"
//#include "Defects.h"
#include "ACS_Def.h"

struct _W
  {
  double Flow_out;
  double oP_Z, P_in, P_out, F_in;
  _W();
  };

class CReductor : public IBaseModel, public IUniModel, public _W
	{
//
public:
	CReductor( char * ObjName );
	virtual ~CReductor();
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
  virtual int TestNodes( int kNodes, struct CObjectPoint ** ppNodes );
  virtual int SetMatr( void * pExternals, int PntGroup,  double dt, 
	  double A[/* kExkV */], double B[ /*kE*/ ] );   // A * X = B
  virtual int SetVar( void * pExternals, int PntGroup, double dt, 
	  // Переменные на входах
	  double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] );
	NEW
/////////////////////////////////////////////////
//#include "IO_Conn.h"
#include "IO_Create.h"
#include "Points.h"
#include "ACS_Create.h"
#include "ACS.h"
/////////////////////////////////////////////////
  CFlow * pF_in;
  CFlow * pF_out;
  CFlow * pGas;
  bool IsCtrlGas;
  double dP_Gas_In, P_Gas;
//
  void Calc ( double dt );
	};
