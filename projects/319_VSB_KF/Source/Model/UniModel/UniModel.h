#pragma once

#include "BaseInt.h"
//#include "DataTypes.h"
//#include "SysDataTypes.h"
//#include "SR.h"
#include "EpsVol.h"
#include "Data.h"
#include "Chem.h"
#include "Defects.h"
#include "ACS_Def.h"
#include "Oborot.h"

struct _W
  {
  double Omega, Mw, Flow;
  COborot ExtOborot;
  double Luk_Open;
  int Luk_Move;
  double C_N2;
  double С_O2;
  double k_Flow;
  bool IsHeat;
  double Qh;
  _W();
  };

class CUniModel : public IBaseModel, public IUniModel, public _W
	{
//
public:
	CUniModel( char * ObjName, LPCTSTR Type );
	virtual ~CUniModel();
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
  int SetProp ( struct CObjProps * pProp );
  int GetProp( );
///////////////////////////////////////////////////
  int TestNodes( int kNodes, struct CObjectPoint ** ppNodes );
  int SetMatr( void * pExternals, int PntGroup,  double dt, 
    double A[/* kExkV */], double B[ /*kE*/ ] );
  int SetVar( void * pExternals, int PntGroup, double dt, 
    double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] );
  ///////////////////////////////////////////////////
//  int Work( double dt );
//  int TestCtrlPoints( int kNodes, struct CObjectPoint ** _ppNodes );
//  int PutAddr( char * Point, void * pData, char * Name );
  int SrcMM( char * DataName, struct IMM * pMM );
	NEW
/////////////////////////////////////////////////
//#include "IO_Conn.h"
#include "IO_Create.h"
#include "Points.h"
#include "ACS_Create.h"
#include "ACS.h"
/////////////////////////////////////////////////
  class CCommonModel * pCurrentModel;
/*
  CFlow * pFlow_in;
  CFlow * pFlow_out;
  CSEps In;
  CSEps Out;
  char * Pipe_in;
  char * Pipe_out;
  COborot * pOborot;
  CComp * pCompOut;
  int ID_CompOut;
  CComp MEA;
  bool Mass;
  bool MustSetComp;
//
  CDef * Восстановление;
  CDef * Засорение;
  CDef * Опустошение;
  CDef * Переполнение;
  CDefParam * Засор;
  CDef * Загазованность;
  CDefParam * Уровень_загазованности;
  int Defect;
  double Omega_def;
  double k_Comp[K_GAS];
  double FlowQ;
//
  void Resist( double dt );
  void Heater( double dt );
  void ElectroHeat( double dt );
  void HeaterdT( double dt );
  void Script( double dt );
  void Cleaner( double dt );
  void Luk( double dt );
  void Produvka( double dt );
  void CalcMEA( double dt );
  void SetComp( );
  void SetOutComp( CFlow * pFlow_in,  CFlow * pFlow_out );
  void FiltrComp( CFlow * pFlow_i,  CFlow * pFlow_o );
*/
  void Calc ( double dt );
	};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////                    
class CCommonModel : public IBaseModel, public IUniModel
  {
  public:
    CCommonModel( char * _ObjName, IBaseModel * _pMain ) : IBaseModel ( _ObjName, _pMain )
      {
      };
    virtual ~CCommonModel() {;};
    // Интерфейс IBaseModel
    virtual int Init( int Regim ) { return 0; };
    virtual int Step0( ) { return 0; };
    virtual int Step1( ) { return 0; };
    virtual int StepT(double dt) { return 0; };
    virtual int GetParams( char * ) { return 0; };
    virtual int ShowParams( DWORD DataType, struct CShowData * pSD ) { return 0; };
    virtual int UpdateParam( struct CParams & Param ) { return 0; };
    virtual int SaveState ( ) { return 0; };
    virtual int RestoreState ( char * StrName ) { return 0; };
    virtual int SetData( int TypeData, void * pData ) { return 0; };
    virtual int OnDefect( CDef * pDefect ) { return 0; };
    ///////////////////////////////////////////////////
    virtual int TestNodes( int kNodes, struct CObjectPoint ** ppNodes ) { return 0; };
    virtual int SetMatr( void * pExternals, int PntGroup,  double dt, 
      double A[/* kExkV */], double B[ /*kE*/ ] ) { return 0; };
    virtual int SetVar( void * pExternals, int PntGroup, double dt, 
      double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] ){ return 0; };
    virtual void Calc( double dt ) { ; };
  };
//////////////////////////////////////////////////////////////////////////////////////////////////////////////                    


//...........................................................................................

