#pragma once

#include "BaseInt.h"
//#include "DataTypes.h"
//#include "SysDataTypes.h"
//#include "SR.h"
//#include "SelVar.h"
#include "Data.h"
//#include "Chem.h"
#include "Defects.h"
#include "ACS_Def.h"
#include "EpsVol.h"
#include "IntReg.h"

#define DEFECT_OVERHEATING		(1 << 0)

enum Switch
{
	SWITCH_DIST,
	SWITCH_MEST,	
};

struct _W_ElectricHeater_b
{
	double Q;
	double T_i;
	double T_o;
	double Task;
	Switch eSwitch;
	bool State;
	_W_ElectricHeater_b();
};

class CElectricHeater_b : public IBaseModel, public IUniModel, public IElectroModel, public _W_ElectricHeater_b
{
	//
public:
	CElectricHeater_b( char * ObjName );
	virtual ~CElectricHeater_b();
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
	//  int GydroInit( IBaseModel * pGydro, int PntGroup, struct CNodeData * pInfo );
	int SetMatr( void * pExternals, int PntGroup,  double dt, 
		double A[/* kExkV */], double B[ /*kE*/ ] );
	int SetVar( void * pExternals, int PntGroup, double dt, 
		double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] );
	virtual void ElectroOn();
	virtual void ElectroOff();
	NEW
	void DrawObj( CDrawObjData * pDraw );
	/////////////////////////////////////////////////
	//#include "IO_Conn.h"
	#include "IO_Create.h"
	#include "Points.h"
	#include "ElectricHeater_b_Defects.h"
	#include "ACS_Create.h"
	#include "ACS.h"
	/////////////////////////////////////////////////
	//
	void Calc ( double dt );
	bool Heat(double dt);
	bool HydroTask();
	void VerifyBlk(int Command);
	void Control(double dt);
	bool DefectCalc();
	CFlow* pFlow_i;
	CFlow* pFlow_o;
	CSEps In;
	CSEps Out;
	bool bRegTask;
	bool bIgnoreBlk;
	double dReg;
	int nDefect;
	double IsElectro;
};

