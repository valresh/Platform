#pragma once

#include "BaseInt.h"
//#include "DataTypes.h"
//#include "SysDataTypes.h"
//#include "SR.h"
//#include "SelVar.h"
#include "Data.h"
//#include "Chem.h"
//#include "Defects.h"
//#include "ACS_Def.h"
#include "Pump_b.h"

struct _W_Dynamic
{
	double ro_l;
	double p_i;
	double p_o;
	double E;
	double T;
	double M_g;
	double M_l;
	bool bRest;
	double ro;
	_W_Dynamic();
};

class PUMP_API CDynamic : public CPump_b, public _W_Dynamic
{
	//
public:
	CDynamic( char * ObjName );
	virtual ~CDynamic();
	// Интерфейс IBaseModel
	virtual int Init( int Regim );
	virtual int Step0( );
	virtual int Step1( );
	virtual int StepT(double dt);
	virtual int GetParams( char * );
	virtual int ShowParams( DWORD DataType, struct CShowData * pSD );
	virtual int UpdateParam( struct CParams & Param );
	virtual int SaveState ( );
	virtual int RestoreState ( char * StrName );
	virtual int SetData( int TypeData, void * pData );
	virtual int OnDefect( CDef * pDefect );
	///////////////////////////////////////////////////
	virtual int TestNodes( int kNodes, struct CObjectPoint ** ppNodes );
	virtual int SetMatr( void * pExternals, int PntGroup,  double dt, 
		double A[/* kExkV */], double B[ /*kE*/ ] );   // A * X = B
	virtual int SetVar( void * pExternals, int PntGroup, double dt, 
		// Переменные на входах
		double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] );
	virtual void DrawObj ( struct CDrawObjData * pDraw );
#ifndef LINUX
    virtual void DrawObject ( struct CDrawObjData * pDraw, CGDIResourceMgr *pResMgr);
#endif

	virtual int DstMM( char * DataName, struct IMM * pMM );
	virtual int SrcMM( char * DataName, struct IMM * pMM );
	NEW
	/////////////////////////////////////////////////
#include "IO_Create.h"
#include "Dynamic_Pnt.h"
#include "Dynamic_Defects.h"
	CDef *Падение_мощности_насоса;
	CDefParam *Падение_мощности_насоса_Значение;
	/////////////////////////////////////////////////
	//
	virtual void Calc ( double dt );
	virtual void CalcCharacteristic();
	//virtual double H(double F_mol);
	double kDef;
	double a;
	double b;
	bool bH;
};

