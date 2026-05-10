#include "stdafx.h"
#include "UniModel.h"
#include "Err.h"
#include "CommProc.h"
#include "HydroStruct.h"
#include "TestNodes.h"
//#include "CommProc.h"
//#include "Data.h"
//#include "SetData.h"

struct CRecycle_W
  {
  double Omega, Mw, Flow;
  };

class CRecycle : public CCommonModel, public CRecycle_W
  {
  //
  public:
    CRecycle( char * _ObjName, IBaseModel * _pMain  );
    // Интерфейс IBaseModel
    //    int Init( int Regim );
    //    int Step0( );
    int Step1( );
    int StepT(double dt);
    int GetParams( char * );
    //    int ShowParams( DWORD DataType, struct CShowData * pSD );
    //    int UpdateParam( struct CParams & Param );
    int SaveState ( );
    int RestoreState ( char * StrName );
    //    int SetData( int TypeData, void * pData );
    //    int OnDefect( CDef * pDefect );
    //    int SetProp ( struct CObjProps * pProp );
    //    int GetProp( );
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
    double dP, Flow_kg;
    CFlow * pFlow_in;
    CFlow * pFlow_out;
#include "IO_Create.h"
#include "ACS_Create.h"
    PARM_D( Flow_Z_kg, "Заданный поток кг", 0. );
    PARM_B( Use_Flow_Z_kg, "Использовать заданный поток", true );
    PARM_D( Omega, "Проходимость", 1000. );
    PARM_D( T_Out, "Температура на выходе", 1. );
    PARM_D( T_In, "Температура на выходе", 1. );
    PARM_B( Use_T_Out, "Использовать заданную температуру", true );
  };

CCommonModel * CreateRecycle( char * _ObjName, IBaseModel * _pMain  )
  {
  return new CRecycle( _ObjName, _pMain  );
  }

CRecycle::CRecycle( char * _ObjName, IBaseModel * _pMain  ) : CCommonModel ( _ObjName, _pMain )
, In("Вход",this)
, Out("Выход",this)
  {
  ObjName = _ObjName;
  Model = "Recycle";
  dP = Flow_kg = 0.;
  pFlow_in = NULL;
  pFlow_out = NULL;
  Mw = Mw_H2O;
  }

int CRecycle::StepT(double dt)
  {
  *pFlow_out = *pFlow_in;
  In.PH ( pFlow_in );
  if ( Use_T_Out )
    {
    Out.PT ( pFlow_out->P, T_Out, pFlow_in->Get_Comp());
    pFlow_out->h_Flow = Out.h;
    }
  return 0;
  }

int CRecycle::Step1( )
  {
  return 0;
  }

#define this pMain
#include "IO_Parms.h"

int CRecycle::GetParams( char * )
  {
  TAB("Режим",0)
    PARM( dP, "#Перепад" );
    PARM( Flow_kg, "#Поток, кг" );
    PARM( In.T, "#Температура на входе" );
    PARM( Out.T, "#Температура на входе" );
  ETAB
  TAB("Конструктив",0)
    PARM_D( Flow_Z_kg, "Заданный поток кг", 0. );
    PARM_B( Use_Flow_Z_kg, "Использовать заданный поток", true );
    PARM_D( Omega, "Проходимость", 1000. );
    PARM_D( T_Out, "Заданная температура на выходе", 15. );
    PARM_B( Use_T_Out, "Использовать заданную температуру", true );
  ETAB
  return 0;
  }

#undef this

int CRecycle::SaveState ( )
  {
  S_CLASS("W",CRecycle_W)
    return 0;
  }
int CRecycle::RestoreState ( char * StrName )
  {
  R_CLASS("W",CRecycle_W)
    return 1;
  }

int CRecycle::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
  {
  if ( kNodes != 2 )
    {
    ModMsg ( "У рециклера '%s' неправильное число входов", ObjName );
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
    //??    Pipe_in = pOP_0->pPool->Name;
    //??    Pipe_out = pOP_1->pPool->Name;
    //CStr Pipe_out;
    }
  if ( pOP_0->Point[0] == 'o' && pOP_1->Point[0] == 'i' )
    {
    pOP_1->NumbInGroup = 0;
    pOP_0->NumbInGroup = 1;
    pOP_1->IO = '+';
    pOP_0->IO = '-';
    OK = true;
    //??    Pipe_in = pOP_1->pPool->Name;
    //??    Pipe_out = pOP_0->pPool->Name;
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

int CRecycle::SetMatr( void * pExternals, int PntGroup,  double dt, 
                      double A[/* kExkV */], double B[ /*kE*/ ] )
  {
  double * Eq = A;
  if ( Use_Flow_Z_kg )
    {
    Eq[F_0] = 1.;
    B[0] = Flow_Z_kg / Mw;
    Eq += 4;
    Eq[F_1] = 1.;
    B[1] = Flow_Z_kg / Mw;
    return 0;
    }
  else
    {
    Eq[P_0] = Omega;
    Eq[P_1] = -Omega;
    Eq[F_0] = -1.;
    Eq += 4;
    Eq[F_0] = 1.;
    Eq[F_1] = -1.;
    return 0;
    }              
  return 0;
  }
int CRecycle::SetVar( void * pExternals, int PntGroup, double dt, 
                     double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
  {
  if ( Var[F_0] > 0. )
    {
    pFlow_in = (CFlow*)pVar[0];
    pFlow_out = (CFlow*)pVar[1];
    }
  else
    {
    pFlow_in = (CFlow*)pVar[1];
    pFlow_out = (CFlow*)pVar[0];
    }
  dP = Var[P_0] - Var[P_1];
  Mw = ::Mw ( pFlow_in->Get_Comp());
  Flow_kg = pFlow_in->Flow_mol * Mw;
  return 0;
  }
