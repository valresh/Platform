#pragma once

#include "BaseInt.h"
//#include "DataTypes.h"
//#include "SysDataTypes.h"
//#include "SR.h"
//#include "SelVar.h"
#include "Data.h"
//#include "Chem.h"
//#include "Defects.h"
#include "ACS_Def.h"
#include "EpsVol.h"

struct _W_Thermostatic_Valve_3
{
	double Position;
	double Task;	
	double PV, MV;
	double En1, En;
	_W_Thermostatic_Valve_3();
};

class CThermostatic_Valve_3 : public IBaseModel, public IUniModel, public _W_Thermostatic_Valve_3
{
	enum
	{	
		IO_I_1,
		IO_I_2,
		IO_O,
		IO_COUNT
	};
	//
public:
	CThermostatic_Valve_3( char * ObjName );
	virtual ~CThermostatic_Valve_3();
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
	NEW
	/////////////////////////////////////////////////
	//#include "IO_Conn.h"
	#include "IO_Create.h"
	#include "Points.h"
	#include "ACS_Create.h"
	#include "ACS.h"
	/////////////////////////////////////////////////
	//
	void Calc ( double dt );
	void Control(double dt);

	CFlow* pFlow[IO_COUNT];
	double Flow[IO_COUNT];

	CSEps state_o;
	CComp * pCompOut;
	int ID_CompOut;
};

