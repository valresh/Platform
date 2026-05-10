#pragma once

#include "BaseInt.h"

struct _W_ChemSpec
  {
  double O;
  _W_ChemSpec();
  };

class CChemSpec : public IBaseModel, public _W_ChemSpec
	{
//
public:
	CChemSpec( char * ObjName );
	virtual ~CChemSpec();
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
#include "IO_Create.h"
#include "Points.h"
/////////////////////////////////////////////////
	};


#include "Water.h"
