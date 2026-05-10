#pragma once

#include "BaseInt.h"
//#include "DataTypes.h"
//#include "SysDataTypes.h"
#include "EpsVol.h"
//#include "GydroData.h"
#include "Data.h"
#include "Oborot.h"
#include "Defects.h"
#include "ACS_Def.h"
#include "Data.h"

#ifdef COOL_OBJ_EXPORTS
#define COOL_OBJ_API __declspec(dllexport)
#else
#define COOL_OBJ_API __declspec(dllimport)
#endif

struct _W_CoolObj
  {
  double T;
  _W_CoolObj();
  };

class COOL_OBJ_API CCoolObj : public IBaseModel, public IUniModel, public _W_CoolObj
	{
//
public:
	CCoolObj( char * _ObjName, IBaseModel* pMain = NULL);
	virtual ~CCoolObj();
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
  ///////////////////////////////////////////////////
//  CExtern_Pnt * FindPoint( const char * PntName );
  int DstMM( char * DataName, struct IMM * pMM );// Вызов получателя сигнала
	NEW
  int SetProp ( struct CObjProps * pProp );
  int GetProp( );
  int kSens;
  double Tout[32];
/////////////////////////////////////////////////
//#include "IO_Conn.h"
#include "IO_Create.h"
#include "Points.h"
#include "CoolObj_Defects.h"
#include "ACS_Create.h"
#include "ACS.h"
  CSEps In;
  CSEps Out;
  CFlow * pFlow_in; 
  CFlow * pFlow_out; 
  CAnalog ** pSens;
  double * kTz;
  double * Tz;
/////////////////////////////////
  int Defect;
  int nSens;
  double Pow;
  double W;
  bool On;
  double Oborot;
  CMotorData* pMotor;
/////////////////////////////////////////////////
//
  void Calc ( double dt );
	};
