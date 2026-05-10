#pragma once

#include "BaseInt.h"
#include "Data.h"
#include "Chem.h"
#include "Defects.h"
#include "ACS_Def.h"

struct _W_Drain
  {
  double Flow_mol;
  double K_Drain;
  double Ventil;
  _W_Drain();
  };

class CDrainage : public IBaseModel, public IUniModel, public _W_Drain
	{
//
public:
	CDrainage( char * ObjName );
	virtual ~CDrainage();
// Интерфейс IBaseModel
	int Init( int Regim );
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
  virtual int TestNodes( int kNodes, struct CObjectPoint ** ppNodes );
  virtual int SetMatr( void * pExternals, int PntGroup,  double dt, 
    double A[/* kExkV */], double B[ /*kE*/ ] );   // A * X = B
  virtual int SetVar( void * pExternals, int PntGroup, double dt, 
    // Переменные на входах
    double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] );
	NEW
/////////////////////////////////////////////////
/////////////////////////////////////////////////
  // Жидкость
  static double Ro_std_liq;// кг/м3
  static double Mu_std_liq;
  static double Dy_std_liq;// мм
  static double Q_std_liq; // м3/ч
  static double dP_std_liq;
  // Газ
  static double T_std_gas; // К
  static double Mu_std_gas;
  static double Dy_std_gas;// мм
  static double Q_std_gas; // нм3/ч
  static double Pin_std_gas;
  static double dP_std_gas;
/////////////////////////////////////////////////
//#include "IO_Conn.h"
#include "IO_Create.h"
#include "Points.h"
#include "Drainage_Defects.h"
#include "ACS_Create.h"
#include "ACS.h"
  enum eType { None, Газ, Жидкость };
  eType Type;
  int Defect;
/////////////////////////////////////////////////
//
  void Calc ( double dt );
  void CalcK( );
	};
