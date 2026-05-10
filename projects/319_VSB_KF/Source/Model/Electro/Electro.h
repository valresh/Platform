#pragma once

#include "BaseInt.h"
//#include "DataTypes.h"
//#include "SysDataTypes.h"
//#include "SR.h"
//#include "SelVar.h"
//#include "Data.h"
//#include "Chem.h"
#include "Defects.h"
//#include "ACS_Def.h"

struct _W_Electro
{
	_W_Electro();
};

class CElectro : public IBaseModel, public _W_Electro
{
	//
public:
	CElectro( char * ObjName );
	virtual ~CElectro();
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
	NEW
	/////////////////////////////////////////////////
	//#include "IO_Conn.h"
	#include "IO_Create.h"
	#include "Points.h"
	//#include "ACS_Create.h"
	//#include "ACS.h"
	/////////////////////////////////////////////////
	//
	void Calc ( double dt );
	IElectroModel** aModel;
	int nModel;
};

