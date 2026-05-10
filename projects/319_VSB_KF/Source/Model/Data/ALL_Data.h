#pragma once

#include "BaseInt.h"



class CALL_Data : public IBaseModel
	{
//
public:
	CALL_Data( char * ObjName );
	virtual ~CALL_Data();
// Интерфейс IBaseModel
  int Init( int );
	int Step0( );
	int Step1( );
	int StepT(double dt);
	int GetParams( char * StrName );
  int UpdateParam( struct CParams & Param );
  int SaveState( );
  int RestoreState( char * StrName );
  int SetData( int TypeData, void * pData );
	int OnDefect( struct CDef * pDefect ) { ASS(FALSE); return 1; };
	int GetConnections ( int & Cnt, struct CConnection Connections[/*100*/]) NOT_USED;
//??	NEW
//
  void Calc ( double dt );
	};
