#pragma once

#include "BaseInt.h"
//#include "DataTypes.h"
//#include "SysDataTypes.h"
//#include "SR.h"
#include "EpsVol.h"
#include "Data.h"
#include "Chem.h"
//#include "Defects.h"
#include "ACS_Def.h"

struct _W
  {
  double P, T;
  double P_Gas, P_H2O, Pz_H2O; // В газе
  double M_H2O;
//
  double AA, BB, Ksi;
  double Flow_liq;
  double P_in, P_out;
  double dP_In, Omega;
  double L;
  double V_H2O;
  double k_Omega_Out;
  double Omega_In, Omega_Out;
  double B_In, B_Out;
  double Omega_Full;
  bool Full;
  _W();
  };

class CKO : public IBaseModel, public IUniModel, public _W
	{
//
public:
	CKO( char * ObjName );
	virtual ~CKO();
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
  virtual int TestNodes( int kNodes, struct CObjectPoint ** ppNodes ) = 0;
  virtual int SetMatr( void * pExternals, int PntGroup,  double dt, 
    double A[/* kExkV */], double B[ /*kE*/ ] ) = 0;
  virtual int SetVar( void * pExternals, int PntGroup, double dt, 
    double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] ) = 0;
	NEW
/////////////////////////////////////////////////
#include "IO_Create.h"
#include "Points.h"
#include "ACS_Create.h"
#include "ACS.h"
  CFlow * pFlow_In;
  CFlow * pFlow_Out;
  CComp * pComp_In;
  CComp * pComp_Out;
  int ID_pComp_In;
  int ID_pComp_Out;
/////////////////////////////////////////////////
  CSEps In;
  double dH;
  double P_Out;
  double Level;
  char * CurrErr;
  double Eta_Gas;
  double Z;
  double M_gas_old;
  double dM_Liq;
//
  virtual void Calc ( double dt ) = 0;
  void Calc_In ( double dt );
  void Calc_Out ( double dt );
  void Calc_Heat ( double dt );
	};

class CKO_O : public CKO
	{
public:
  CKO_O( char * _ObjName ) : CKO ( _ObjName ) { ; };
  void Calc ( double dt );
  virtual int TestNodes( int kNodes, struct CObjectPoint ** ppNodes );
  virtual int SetMatr( void * pExternals, int PntGroup,  double dt, 
    double A[/* kExkV */], double B[ /*kE*/ ] );
  virtual int SetVar( void * pExternals, int PntGroup, double dt, 
    double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] );
  };

class CKO_N : public CKO
	{
public:
  CKO_N( char * _ObjName ) : CKO ( _ObjName ) { ; };
  void Calc ( double dt );
  virtual int TestNodes( int kNodes, struct CObjectPoint ** ppNodes );
  virtual int SetMatr( void * pExternals, int PntGroup,  double dt, 
    double A[/* kExkV */], double B[ /*kE*/ ] );
  virtual int SetVar( void * pExternals, int PntGroup, double dt, 
    double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] );
  };
