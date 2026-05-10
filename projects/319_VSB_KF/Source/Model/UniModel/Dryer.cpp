#include "stdafx.h"
#include "UniModel.h"
#include "Err.h"
#include "CommProc.h"
#include "HydroStruct.h"
#include "TestNodes.h"

class CClear_H2O : public CCommonModel
  {
  public:
  CClear_H2O( char * _ObjName, IBaseModel * _pMain  );
  // Интерфейс IBaseModel
  int Init( int Regim );
  void Calc ( double dt );
  int GetParams( char * );
  ///////////////////////////////////////////////////
  int TestNodes( int kNodes, struct CObjectPoint ** ppNodes );
  int SetMatr( void * pExternals, int PntGroup,  double dt, 
    double A[/* kExkV */], double B[ /*kE*/ ] );
  int SetVar( void * pExternals, int PntGroup, double dt, 
    double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] );
  ///////////////////////////////////////////////////
  NEW
  CSEps In;
  CSEps Out;
  CFlow * pFlow_in;
  CFlow * pFlow_out;
  CComp * pComp;
  int ID_Comp;
#include "IO_Create.h"
#include "ACS_Create.h"
  PARM_D( Flow_In_kg, "#Поток воды на вход, кг", 0. );
  PARM_D( Flow_Out_kg, "#Поток воды на выход, кг", 0. );
  PARM_D( kFlow_H2O, "Скорость удаления воды", 1. );
  };

CCommonModel * CreateClear_H2O( char * _ObjName, IBaseModel * _pMain  )
  {
  return new CClear_H2O( _ObjName, _pMain  );
  }

CClear_H2O::CClear_H2O( char * _ObjName, IBaseModel * _pMain  ) : CCommonModel ( _ObjName, _pMain )
, In("Вход",this)
, Out("Выход",this)
  {
  ObjName = _ObjName;
  Model = "Dryer";
  pFlow_in = NULL;
  pFlow_out = NULL;
  Flow_In_kg = 0;
  Flow_Out_kg = 0;
  pComp = NULL;
  ID_Comp = 0;
  }

int CClear_H2O::Init( int Regim )
  {
  pComp = NULL;
  NewComp ( &ID_Comp, &pComp, "Выход из %s", ObjName );
  return 0;
  }

#define this pMain
#include "IO_Parms.h"

int CClear_H2O::GetParams( char * )
  {
  TAB("Режим",0)
    PARM_D( Flow_In_kg, "#Поток воды на вход, кг", 0. );
    PARM_D( Flow_Out_kg, "#Поток воды на выход, кг", 0. );
  ETAB
  TAB("Параметры",0)
    PARM_D( kFlow_H2O, "Скорость удаления воды", 0.1 );
  ETAB
  return 0;
  }

void CClear_H2O::Calc ( double dt )
  {
  In.PH ( pFlow_in );
  pComp->Set ( pFlow_in->Get_Comp());
  double Ksi = 1. / ( 1. + kFlow_H2O );
  pComp->Cmol[G_H2O] *= Ksi;
  pFlow_out->ID_COMP = ID_Comp;
  pFlow_out->pComp = pComp;
  pFlow_out->T = In.T;
  Out.PT ( pFlow_out->P, pFlow_out->T, pComp );
  pFlow_out->h_Flow = Out.h;
  }

int CClear_H2O::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
  {
  if ( kNodes != 2 )
    {
    ModMsg ( "У осушителя '%s' неправильное число входов", ObjName );
    return 2;
    }
  CObjectPoint * pOP_0 = ppNodes[0];
  CObjectPoint * pOP_1 = ppNodes[1];
  pOP_0->Type = eГидравлика;
  pOP_1->Type = eГидравлика;
  bool OK = false;
  if ( pOP_0->Point[0] == 'i' && pOP_1->Point[0] == 'o' )
    {
    pOP_0->NumbInGroup = 0;
    pOP_1->NumbInGroup = 1;
    pOP_0->IO = '+';
    pOP_1->IO = '-';
    OK = true;
    }
  if ( pOP_0->Point[0] == 'o' && pOP_1->Point[0] == 'i' )
    {
    pOP_1->NumbInGroup = 0;
    pOP_0->NumbInGroup = 1;
    pOP_1->IO = '+';
    pOP_0->IO = '-';
    OK = true;
    }
  if ( !OK )
    {
    ModMsg ( "У рециклера '%s' неправильные входы ('%s','%s')", ObjName, pOP_0->Point, pOP_1->Point );
    return 2;
    }
  pOP_0->PntGroup = 1;  // i,o
  pOP_1->PntGroup = 1;
  return 0;
  }

#include "SetMatr.h"

int CClear_H2O::SetMatr( void * pExternals, int PntGroup,  double dt, 
                      double A[/* kExkV */], double B[ /*kE*/ ] )
  {
  double * Eq = A;
  Eq[P_0] = 1.;
  Eq[P_1] = -1.;
  Eq += 4;
  Eq[F_0] = 1.;
  Eq[F_1] = -1.;
  return 0;
  }

int CClear_H2O::SetVar( void * pExternals, int PntGroup, double dt, 
                     double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
  {
  pFlow_in = (CFlow*)pVar[0];
  pFlow_out = (CFlow*)pVar[1];
  return 0;
  }

