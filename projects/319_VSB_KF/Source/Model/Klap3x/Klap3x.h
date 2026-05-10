#pragma once

#include "BaseInt.h"
//#include "DataTypes.h"
//#include "SysDataTypes.h"
#include "EpsVol.h"
//#include "GydroData.h"
#include "Data.h"
//#include "Chem.h"
#include "Defects.h"
#include "ACS_Def.h"

struct _W
  {
  double O;
  _W();
  };

class CKlap3x : public IBaseModel, public IUniModel, public _W
	{
//
public:
	CKlap3x( char * ObjName );
	virtual ~CKlap3x();
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
  void DrawObj ( struct CDrawObjData * pDraw );
	NEW
/////////////////////////////////////////////////
//#include "IO_Conn.h"
#include "IO_Create.h"
#include "Points.h"
#include "Klap3x_Defects.h"
#include "ACS_Create.h"
#include "ACS.h"
/////////////////////////////////////////////////
//  CZadv * pPos;
  CComp * pOut;
  int ID_Out;
  CFlow * pFlow[3];
  CSEps * pState[3];
  double M;
  int Defect;
  double Fixed_Pos;
//
  void Calc ( double dt );
  void Add ( double dt, bool * IsComp, double * Cmol,  double & H, double & F, CFlow * pFlow );
	};
