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

#undef IN_DLL
#ifdef DLL_LS
#undef IN_DLL
#define IN_DLL __declspec(dllexport)
#else
#define IN_DLL __declspec(dllimport)
#endif

struct IN_DLL _W_LS
  {
  double Value;
  double TrueValue;
  double ShowValue;
  _W_LS();
  };

class IN_DLL CLS : public IBaseModel, public _W_LS
	{
//
public:
	CLS( char * ObjName );
	virtual ~CLS();
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
//  int TestCtrlPoints(  int kNodes, struct CObjectPoint ** ppNodes  );
//  int Work( double dt );
	NEW
/////////////////////////////////////////////////
//#include "IO_Conn.h"
#include "IO_Create.h"
#include "Points.h"
#include "ACS_Create.h"
#include "ACS.h"
/////////////////////////////////////////////////
  double * pValue;
  static CLS * pFirstLS;
  static CLS * pLastLS;
  CLS * pNextLS;
  IBaseModel * pBaseSens;
//
  void Calc ( double dt );
	};
