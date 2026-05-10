#pragma once

#include "BaseInt.h"
//#include "DataTypes.h"
//#include "SysDataTypes.h"
//#include "SR.h"
//#include "GydroData.h"
#include "Data.h"
#include "Defects.h"
#include "EpsVol.h"
#include "ACS_Def.h"
#include "ISO.h"

struct _W
  {
  double P_in, P_out, P_cam;
  double T_in, T_out;
  double F_in;
  double P_podsos;
  _W();
  };

struct CSS
  {
  // параметры
  double Mw_Mix, Cp_H2O, Cp_Add, Sp, Gamma;
  // рабочие переменные
  double PTO, Lam, Loose;
  // переменные состояния
  double Wp, G0, h_in, W3, Pp, RoP;
//
  double h_H2O( double T );
  double h_Add( double T );
  };

class CEjector : public IBaseModel, public IUniModel, public _W, public CSS
	{
//
public:
	CEjector( char * ObjName );
	virtual ~CEjector();
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
    double A[/* kExkV */], double B[ /*kE*/ ] );
  int SetVar( void * pExternals, int PntGroup, double dt, 
    double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] );
	NEW
/////////////////////////////////////////////////
//#include "IO_Conn.h"
#include "IO_Create.h"
#include "Points.h"
#include "Ejector_Defects.h"
#include "ACS_Create.h"
#include "ACS.h"
/////////////////////////////////////////////////
  CISO State_In;
  CISO State_Out;
  CSEps In;
  CSEps Out;
  CSEps Add;
//
  CFlow * p_In;
  CFlow * p_Podsos;
  CFlow * p_Out;
//
  CComp * pCompOut;
  int ID_CompOut;
  double FlowQ;
  double O_Razgerm;
  int Defect;
//
  void Calc ( double dt );
  double Calc_dh_0 ( double Tp );
  double Calc_h_mix ( double Tp );
  void SimplePhysModel ( double dt );
  void Podsos( double dt );
  double Напор( double Расход );
  void Mix( );
  void Heat( );
	};
