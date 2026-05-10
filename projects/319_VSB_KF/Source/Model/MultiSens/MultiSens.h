#pragma once

#include "BaseInt.h"
//#include "DataTypes.h"
//#include "SysDataTypes.h"
//#include "SR.h"
//#include "GydroData.h"
#include "Data.h"
//#include "Chem.h"
#include "Defects.h"
#include "ACS_Def.h"
#include "Data.h"

struct _W
  {
  bool On;
  double Oborot;
  _W();
  };

class CMultiSens : public IBaseModel, public IUniModel, public _W
	{
//
public:
	CMultiSens( char * ObjName );
	virtual ~CMultiSens();
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
  int SetProp ( struct CObjProps * pProp );
  int GetProp( );
  ///////////////////////////////////////////////////
  virtual int TestNodes( int kNodes, struct CObjectPoint ** ppNodes );
  virtual int SetMatr( void * pExternals, int PntGroup,  double dt, 
    double A[/* kExkV */], double B[ /*kE*/ ] );   // A * X = B
  virtual int SetVar( void * pExternals, int PntGroup, double dt, 
    // Переменные на входах
    double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] );
  ///////////////////////////////////////////////////
  //CExtern_Pnt * FindPoint( const char * PntName );
  int DstMM( char * DataName, struct IMM * pMM );// Вызов получателя сигнала
  int SrcMM( char * DataName, struct IMM * pMM );
	NEW
/////////////////////////////////////////////////
//#include "IO_Conn.h"
#include "IO_Create.h"
#include "Points.h"
#include "MultiSens_Defects.h"
#include "ACS_Create.h"
#include "ACS.h"
  CMotorData * pMotor;
/////////////////////////////////////////////////
  int kVibroSens;
  int kShiftSens;
  int kTSens;
  int nVibroSens;
  int nShiftSens;
  int nTSens;
  CAnalogD ** pVibro;
  CAnalogD ** pShift;
  CAnalogD ** pT;
  double * rndShftSens_Curr;
  double * rndShftSens_Lim;
  double * rndShftSens_Vel;
  double * rndVibrSens;
  double * rndTSens;
// 
//
  void Calc ( double dt );
  void Vibration( double dt );
  void Shift( double dt );
  int kDefectSens;
  enum
  {
	  DEFECT_NO = 0,
	  DEFECT_VIBRO,
	  DEFECT_SHIFT,
  } eDefect;
	};
