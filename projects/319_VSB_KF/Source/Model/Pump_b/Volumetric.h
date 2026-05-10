#pragma once

#include "BaseInt.h"
#include "DataTypes.h"
#include "SysDataTypes.h"
//#include "SR.h"
//#include "SelVar.h"
#include "Data.h"
//#include "Chem.h"
//#include "Defects.h"
//#include "ACS_Def.h"
#include "Pump_b.h"

struct _W_Volumetric
{
	_W_Volumetric();
};

class CVolumetric : public CPump_b, public _W_Volumetric
{
	//
public:
	CVolumetric( char * ObjName );
	virtual ~CVolumetric();
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
	int TestNodes( int kNodes, struct CObjectPoint ** ppNodes );
	int SetMatr( void * pExternals, int PntGroup,  double dt, 
		double A[/* kExkV */], double B[ /*kE*/ ] );   // A * X = B
	int SetVar( void * pExternals, int PntGroup, double dt, 
		// Переменные на входах
		double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] );
	void DrawObj ( struct CDrawObjData * pDraw );
#ifndef LINUX
    void DrawObject ( struct CDrawObjData * pDraw, CGDIResourceMgr *pResMgr);
#endif

	NEW
	/////////////////////////////////////////////////
	//#include "IO_Conn.h"
	#include "IO_Create.h"
	#include "Volumetric_Pnt.h"
	//#include "ACS_Create.h"
	//#include "ACS.h"
	/////////////////////////////////////////////////
	//
	void Calc ( double dt );
	double Calc_Omega();
	double F_real;
	CDef *Останов_насоса;
	CDef *Выход_двигателя_из_строя;
};

