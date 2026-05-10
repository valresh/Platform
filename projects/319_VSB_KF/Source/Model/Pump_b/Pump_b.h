#pragma once

#include "BaseInt.h"
//#include "DataTypes.h"
//#include "SysDataTypes.h"
//#include "SR.h"
//#include "SelVar.h"
#include "SoftGDI.h"
#include "Data.h"
//#include "Chem.h"
//#include "Defects.h"
#include "ACS_Def.h"
#include "EpsVol.h"
#include "heatoil.h"
#ifdef LINUX
#include "../Motor/Motor.h"
#include "../CoolObj/CoolObj.h"
#else
#include "..\Motor\Motor.h"
#include "..\CoolObj\CoolObj.h"
#endif


#ifdef PUMP_EXPORTS
#define PUMP_API __declspec(dllexport)
#else
#define PUMP_API __declspec(dllimport)
#endif

#define DEFECT_ENGINE_BREAKWOWN		0x00000001
#define DEFECT_RESET				0x00000002

struct _W_Pump_b
{
	double Omega;
	double Head;
	double F_mol;
	double dh;
	double To_m3;
	double To_kg;
	bool bZapret;
	double power;
	_W_Pump_b();
};

class CPump_b : public IBaseModel, public IUniModel, public IElectroModel, public _W_Pump_b
{
	//
public:
	CPump_b( char * ObjName );
	virtual ~CPump_b();
	// Интерфейс IBaseModel
	int Init( int Regim );
	int Step0( );
	int Step1( );
	virtual int StepT(double dt) = 0;
	virtual int GetParams( char * ) = 0;
	int ShowParams( DWORD DataType, struct CShowData * pSD );
	int UpdateParam( struct CParams & Param );
	int SaveState ( );
	int RestoreState ( char * StrName );
	int SetData( int TypeData, void * pData );
	int OnDefect( CDef * pDefect );
	///////////////////////////////////////////////////
	int TestNodes( int kNodes, struct CObjectPoint ** ppNodes );
	virtual int SetMatr( void * pExternals, int PntGroup,  double dt, 
		double A[/* kExkV */], double B[ /*kE*/ ] ) = 0;   // A * X = B
	int SetVar( void * pExternals, int PntGroup, double dt, 
		// Переменные на входах
		double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] );
	virtual void ElectroOn();
	virtual void ElectroOff();
	void DrawObj ( struct CDrawObjData * pDraw );
	virtual int DstMM( char * DataName, struct IMM * pMM );// Вызов получателя сигнала
	virtual int SrcMM( char * DataName, struct IMM * pMM );

	NEW
	//////////////////////////////////////////////////////////////////////////
	double _P_In;
	double _P_Out;
	double _Flow_Work_m3;
	double _Flow_Work_kg;
	double K_F;
	double Номинальный_ток;
	bool Use_Motor;
    //////////////////////////////////////////////////////////////////////////
#include "ACS_Create.h"
#include "Pump_b_ACS.h"
	/////////////////////////////////////////////////
	static const double H2O_To_m3;
	static const double AtmToPa;
	static const double HourToSec;
	static const double kMolToMol;
	static const double g;
	static const double p_i_min;
	CFlow* pFlow_i;
	CFlow* pFlow_o;
	CFlow* pFlow_i_cool;
	CFlow* pFlow_o_cool;
	CSEps Eps;
	CCoolObj* pCoolObj;
	void Calc();
	double CalcRo(double E, double ro_l, double M_l, double p, double T, double M_g);
	double FlowQ;
	CMotorData* pMotor;
	CMotorData MotorData;
	bool bIgnoreBlk;
	int nDefect;
	bool LocalBlk;
	bool IsElectro;
	CMotor* _pMotor;
};

