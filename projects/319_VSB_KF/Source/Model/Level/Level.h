#pragma once

#include "BaseInt.h"
#include "DataTypes.h"
//#include "SysDataTypes.h"
//#include "SR.h"
//#include "GydroData.h"
//#include "Data.h"
//#include "Chem.h"
#include "Defects.h"
#include "ACS_Def.h"

struct _W_Level
  {
  double Lev;
  double dOldL;
  double Show;
  double ErrValue;
  double ShowValue;
  _W_Level();
  };

class CLevel : public IBaseModel, public _W_Level
	{
//
public:
	CLevel( char * ObjName );
	virtual ~CLevel();
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
  int Work( double dt );
  int TestCtrlPoints( int kNodes, struct CObjPoint ** _ppNodes );
	NEW
/////////////////////////////////////////////////
//#include "IO_Conn.h"
#include "IO_Create.h"
#include "Points.h"
#include "ACS_Create.h"
#include "ACS.h"
/////////////////////////////////////////////////
  //CZadv * pTop;
  //CZadv * pBottom;
  //CSensor * pValue;
  //CSensor *pSens;
  bool bZADV_L_TOP;
  bool bZADV_L_BOTTOM;
  //bool bZADV_L_AIR;
  bool bZADV_L_DRAINAGE;
//
  void Calc ( double dt );
	};
