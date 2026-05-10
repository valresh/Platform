#pragma once

#include "EpsVol.h"
#include "Data.h"

#undef IN_DLL

#ifdef DLL_Spider
#define IN_DLL __declspec(dllexport)
#else
#define IN_DLL __declspec(dllimport)
#endif

struct CPntGroup
  {
  int PntGroup;
  int kPoints;
  double * Var;
  CFlow ** pFlow;
  double * A;
  double * B;
  };

class IN_DLL CSpider : public IBaseModel 
	{
	public:
  IBaseModel * pAltModel;
  int kPoints;
  CObjectPoint ** ppObjPoints;
  CObjectPoint ** ppMainPoints;
  CMem<CPntGroup,8,256> Groups;
//////////////////////////////////////
  virtual int Init( int );
  virtual int Step0();
  virtual int Step1();
  virtual int PreStepT();
  virtual int StepT(double dt);
  virtual int GetParams( char * StrName );
  virtual int SelectParams( );
  virtual int SaveState( );
  virtual int RestoreState ( char * StrName );
///
  virtual int SetMatr( void * pExternals, int PntGroup,  double dt, 
    double A[/* kExkV */], double B[ /*kE*/ ] );   // A * X = B
  virtual int SetVar( void * pExternals, int PntGroup, double dt, 
    // Переменные на входах
    double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] );
  virtual int TestNodes( int kNodes, struct CObjectPoint ** ppNodes );
  CSpider( IBaseModel * _pMain, char * pAltObjName, char * pAltModelName, char * pAltSubType, char * pAltDIRName );
  NEW
  void AddGroup ( int PntGroup, int NumbInGroup );
	};

