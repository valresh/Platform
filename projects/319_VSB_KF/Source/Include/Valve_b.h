#pragma once

#include "BaseInt.h"
//#include "DataTypes.h"
//#include "SysDataTypes.h"
//#include "SR.h"
//#include "SelVar.h"
#include "SoftGDI.h"
#include "Data.h"
//#include "Chem.h"
#include "Defects.h"
#include "ACS_Def.h"
#include "EpsVol.h"
#include "IntReg.h"

#undef IN_DLL
#ifdef VALVE_B_EXPORTS
#undef IN_DLL
#define IN_DLL __declspec(dllexport)
#else
#define IN_DLL __declspec(dllimport)
#endif

#define DEFECT_FALLING_WEDGE		(1 << 0)
#define IS_DEFECT(Defect) ((nDefect & Defect) > 0)

struct _W_Valve_b
{
	double Omega;
	double Omega_max;
	double Position;
	double M;
	double Ro;
	double Pin, Pout;
	bool Fixed_Position;
	double Eps;
//
  double Reg_PV_, Reg_MV_, Reg_SP;
  bool Reg_On;
	_W_Valve_b();
};

class IN_DLL CValve_b : public IBaseModel, public IUniModel, public _W_Valve_b
{
	//
public:
	static const double MAX_K_OMEGA;
	CValve_b( char * ObjName, char * Type );
	virtual ~CValve_b();
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
	/////////////////////////////////////////////////
	//#include "IO_Conn.h"
	#include "IO_Create.h"
	#include "Valve_b_Pnt.h"
	#include "Valve_b_Defects_Pnt.h"
	#include "ACS_Create.h"
	#include "Valve_b_ACS.h"
	/////////////////////////////////////////////////
	//
	enum Characteristic
	{
		Равнопроцентная_Характеристика  = 0,
		Линейная_Характеристика,
		Параболическая_Характеристика,
		Характеристика_Обратного_Клапана,
	};
	virtual void Calc ( double dt );
	virtual double Calc_Omega(Characteristic eCharacteristic, double _Position);
	virtual bool Defect_Calc( double dt );
	virtual 
	bool IsHydroTask();
	void SetPosition(double _Position);
	COLORREF ColZadv(double _Position);	
	CFlow* pFlow_i;
	CFlow* pFlow_o;
	CSEps Eps_In;
	double dReg;
	bool Atmosphere;
	CComp* pAtmComp;
	int ID_COMP_ATM;
	enum HydroTask
	{
		HYDRO_TASK_NO,
		HYDRO_TASK_NORM,
		HYDRO_TASK_CONFLICT,		
	}eHydroTask;
	int kStep;
	double K_def;
	int nDefect;
	double Omega_Depressurization;
	double K_F;
	enum
	{
		CALC_NO = 0x00,
		CALC_SET_MATR = 0x01,
		CALC_SET_VAR = 0x02,
		CALC_OK = 0x03
	};
  static bool ShowConflict;
  static double MaxConflict;
  bool SbrosReg;
  short calcConflict;
	//static DWORD ClassRootNode;
};

