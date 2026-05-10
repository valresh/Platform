#pragma once

#include "BaseInt.h"
#include "Data.h"
#include "EpsVol.h"
#include "Defects.h"
#include "ACS_Def.h"

#ifdef LINUX
#include <QtCore/qglobal.h>
#ifdef UTILS_EXPORTS
#define IN_DLL Q_DECL_EXPORT
#else
#define IN_DLL Q_DECL_IMPORT
#endif
#else
#undef IN_DLL
#ifdef DLL_HeatOil
#undef IN_DLL
#define IN_DLL __declspec(dllexport)
#else
#define IN_DLL __declspec(dllimport)
#endif
#endif

#define MAX_HEAT_OIL 20

struct CHeatOil_W
{
	double T;
	double F_Z_kg;
	CHeatOil_W();
};

class IN_DLL CHeatOil	: public IBaseModel, public CHeatOil_W
{
public:
	//
	double T_oil_in, T_oil_out;
	double* pOborot;
	double T_Fire;
	double Tau_Fire;
	//
	CAnalogD * pT_oil;
	double k_def;
	// 
	CFlow * pFlow_Oil_in;
	CFlow * pFlow_Oil_out;
	//
	CSEps In;
	CSEps Out;
	// Отказы
	int kdT_def;
	//
	CHeatOil( char * Name, IBaseModel * pMain, double* _pOborot );
	int GetParams( char * );
	int UpdateParam( CParams & Param );
	int SaveState ( );
	int RestoreState ( char * StrName );
	int OnDefect( struct CDef * pDefect );
	void NoDefect( );
	// Внешние вызовы	из основной программы 
  int SetMatr( void * pExternals, int PntGroup,  double dt, 
    double A[/* kExkV */], double B[ /*kE*/ ] );
  virtual int SetVar( void * pExternals, int PntGroup, double dt, 
    double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] );
	int Step1( );
	int StepT( double dt );
#include "IO_Create.h"
#include "HeatOil_Pnt.h"
	NEW
	bool SetTZ;
};

#define MAX_HEAT_AIR 16

struct CHeatAir_W
  {
	double T_corp[MAX_HEAT_AIR];
  double T_airZ[MAX_HEAT_AIR];
  CHeatAir_W();
  };

struct CHeatAir_P
  {
	bool Use_T_airZ, Set_T_airZ;
	double Tau_cool, T_room;
	double T_rand_max, T_rand_min;
  double T_max;  
  };

class IN_DLL CHeatAir	: public IBaseModel, public CHeatAir_W, public CHeatAir_P
	{
	public:
// Вход
	CAnalogD ** pT_corp;
  double * k_def;
  int kSens;
//
  double Oborot;
  int kdT_def;
	double T_Fire;
  // Отказы
  char * DefectNode;  // Название точки
  char * DefectName;  // Название отказа
	DEFECT(Перегрев,"");
	DEF_D(Нагрев, "Отн. нагрев", 2., 1., 10.)
//
  NEW
	CHeatAir( char * Name, IBaseModel * pMain );
	int Init( char * Fmt, int _kSens );
  int OnDefect( struct CDef * pDefect );
	int StepT( double dt );
	int GetParams( char * );
	int UpdateParam( CParams & Param );
  int SaveState ( );
  int RestoreState ( char * StrName );
	};

