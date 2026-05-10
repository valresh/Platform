#pragma once

#include "BaseInt.h"
//#include "DataTypes.h"
//#include "SysDataTypes.h"
//#include "SR.h"
//#include "GydroData.h"
#include "Data.h"
//#include "Chem.h"
#include "Defects.h"
#include "ACS_Def.h"

struct _W
  {
  bool Open;
  double dP;
  bool OutIsGas;
  double P, T, Mw;
  double Flow_in;
  double Pin, Pout;
  _W();
  };

class CPPK : public IBaseModel, public IUniModel, public _W
	{
//
public:
	CPPK( char * ObjName );
	virtual ~CPPK();
// Интерфейс IBaseModel
  int Init( int );
	int Step0( );
	int Step1( );
	int StepT(double dt);
	int GetParams(  char * StrName );
	int ShowParams( DWORD DataType, struct CShowData * pSD );
  int UpdateParam( struct CParams & Param );
  int SaveState ( );
  int RestoreState ( char * StrName );
  int SetData( int TypeData, void * pData );
  int OnDefect( CDef * pDefect );
  void DrawObj ( struct CDrawObjData * pDraw );
///////////////////////////////////////////////////
  int TestNodes( int kNodes, struct CObjectPoint ** ppNodes );
  int SetMatr( void * pExternals, int PntGroup,  double dt, 
    double A[/* kExkV */], double B[ /*kE*/ ] );
  int SetVar( void * pExternals, int PntGroup, double dt, 
    double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] );
  NEW
/////////////////////////////////////////////////
/////////////////////////////////////////////////
//#include "IO_Conn.h"
#include "IO_Create.h"
#include "Points.h"
#include "PPK_Defects.h"
#include "ACS_Create.h"
#include "ACS.h"
  bool OnlyIn;
/////////////////////////////////////////////////
/////////////////////////////////////////////////
  static char ErrObj[128];
  static bool WorkPPK;
/////////////////////////////////////////////////
  // Жидкость
  static double Ro_std_liq;// кг/м3
  static double Mu_std_liq;
  static double Dy_std_liq;// мм
  static double Q_std_liq; // м3/ч
  static double dP_std_liq;
  // Газ
  static double Dy_std_gas;// мм
  static double Q_std_gas; // нм3/ч
  static double dP_std_gas;
  //
  static double T_std_gas; // К
  static double Mu_std_gas;
  static double Pin_std_gas;
  double K_PPK;
  bool DefOpen;
  bool NewParams;
  CFlow * pFlow_in;
/////////////////////////////////////////////////
//
  void Calc ( double dt );
  double Calc_Omega( );
  void CalcK_PPK( );
  void CalcParms();
	};
