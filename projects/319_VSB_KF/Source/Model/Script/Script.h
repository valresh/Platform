#pragma once

#include "BaseInt.h"

struct _W_MainScript
  {
  double O;
  _W_MainScript();
  };

class CMainScript : public IBaseModel, public _W_MainScript
	{
//
public:
	CMainScript( char * ObjName );
	virtual ~CMainScript();
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
	NEW
/////////////////////////////////////////////////
//
	};
