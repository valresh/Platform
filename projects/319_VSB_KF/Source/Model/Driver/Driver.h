#pragma once

#include "BaseInt.h"
//#include "HydroStruct.h"
//#include "SR.h"
//#include "Defects.h"
#include "ACS_Def.h"
#include "Data.h"

struct _W_Driver
  {
  bool On;
  double RelOborot; // 0 - 1
  double AbsOborot; // об/мин
  double Power;     // кВт
  _W_Driver();
  };

class CDriver : public IBaseModel, public IUniModel, public IPoolCtrlModel, public _W_Driver
	{
//
public:
	CDriver( char * ObjName );
	virtual ~CDriver();
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
  virtual int InitPool ( struct CVarPool * pPool, struct CGroupObject * pPoolObject );
	NEW
/////////////////////////////////////////////////
#include "IO_Create.h"
#include "Points.h"
#include "ACS_Create.h"
#include "ACS.h"
/////////////////////////////////////////////////
  int nMotor;
  int kDrive;
  CMotorData ** pMotorData;
//
	};
