#include "stdafx.h"
#include "UniModel.h"
#include "Err.h"
#include "CommProc.h"
#include "HydroStruct.h"
#include "TestNodes.h"
//#include "CommProc.h"
//#include "Data.h"
//#include "SetData.h"


struct CReflux_W
  {
  double P, Mw_In, Mw_Out;
  };

class CReflux : public CCommonModel, public CReflux_W
  {
  //
  public:
    CReflux( char * _ObjName, IBaseModel * _pMain  );
    // Интерфейс IBaseModel
    int Init( int Regim );
    //    int Step0( );
    void Calc ( double dt );
    int Step1( );
//    int StepT(double dt);
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
    CSEps Cool;
    CSEps Out;
    double dP, Flow_kg;
    CFlow * pFlow_in;
    CFlow * pFlow_out;
    CComp * pComp;
    int ID_Comp;
    double _P, P_Z;
    bool SetPZ;
#include "IO_Create.h"
#include "ACS_Create.h"
    bool Off;
    PARM_D( Flow_In_kg, "#Поток на вход, кг", 0. );
    PARM_D( Flow_Out_kg, "#Поток на выход, кг", 0. );
    PARM_D( Flow_Out_H2O_kg, "#Слив воды, кг", 0. );
    PARM_D( Flow_Z_kg, "Рефлюкс кг, кг", 0. );
    PARM_D( Flow_Gas_nm3, "Газы нм3", 0. );
    PARM_D( k_Reg_P, "Регулировка давления", 0. );
    PARM_D( T_Out, "Температура на выходе", 1. );
//    PARM_D( T_In, "Температура на входе", 1. );
  };

CCommonModel * CreateReflux( char * _ObjName, IBaseModel * _pMain  )
  {
  return new CReflux( _ObjName, _pMain  );
  }

CReflux::CReflux( char * _ObjName, IBaseModel * _pMain  ) : CCommonModel ( _ObjName, _pMain )
, Cool("Охлаждение",this)
, In("Вход",this)
, Out("Выход",this)
  {
  ObjName = _ObjName;
  Model = "Reflux";
  dP = Flow_kg = 0.;
  pFlow_in = NULL;
  pFlow_out = NULL;
  Mw_Out = Mw_H2O;
  k_Reg_P = 0.001;
  T_Out = 50.;
  _P = 0.;
  P_Z = 0.;
  SetPZ = false;
  Off = false;
  }

void CReflux::Calc ( double dt )
  {
//  *pFlow_out = *pFlow_in;
  In.PT ( pFlow_in->P, T_Out, pFlow_in->Get_Comp() );
// Давление - исходя из заданного газа
  double Flow_In_gas = pFlow_in->Flow_mol * In.E;
  double Flow_In_liq = pFlow_in->Flow_mol - Flow_In_gas;
  double Flow_Gas_mol = Flow_Gas_nm3 * iM3;
  if ( SetPZ )
    {
    P = P_Z + 1.;
    Flow_Gas_nm3 = Flow_In_gas * M3;
    }
  else
    {
    if ( Flow_In_gas > Flow_Gas_mol )
      {
  // Давление растет
      double dP = LimdX ( k_Reg_P * ( Flow_In_gas - Flow_Gas_mol ), 0.2 );
      P += dP;
      }
    else
      {
  // Давление падает
      if ( In.E < 1. )
        {
        double dP = LimdX ( k_Reg_P * ( Flow_In_gas - Flow_Gas_mol ), 0.2 );
        P += dP;
        }
      }
    }
  _P = P - 1.;
  bool IsComp[K_GAS];
  double X[K_GAS];
  In.SetCompData ( IsComp, X, NULL, NULL, NULL );
  Flow_Out_H2O_kg = Flow_In_liq * X[G_H2O];
  X[G_H2O] = 0.;
  IsComp[G_H2O] = false;
  pComp->Set( IsComp, X );
  Norm ( IsComp, X );
  Out.PT ( pFlow_out->P, T_Out, IsComp, X );
  pFlow_out->h_Flow = Out.h;
  pFlow_out->ID_COMP = ID_Comp;
  Flow_Out_kg = Flow_kg - Flow_Out_H2O_kg - Flow_Z_kg;
  }

int CReflux::Init( int Regim )
  {
  pComp = NULL;
  NewComp ( &ID_Comp, &pComp, "Выход из %s", ObjName );
  return 0;
  }

int CReflux::Step1( )
  {
  return 0;
  }

#define this pMain
#include "IO_Parms.h"

int CReflux::GetParams( char * )
  {
  TAB("Режим",0)
    PARM( Off, "Выключен" );
    PARM( _P, "#Давление(и)" );
    PARM( In.E, "#Доля пара на входеДавление(а)" );
    PARM( Flow_kg, "#Поток на вход, кг" );
    PARM( In.T, "#Температура на входе" );
    PARM( Out.T, "#Температура на входе" );
  ETAB
  TAB("Конструктив",0)
    PARM ( P_Z, "Заданное давление(и)" );
    PARM ( SetPZ, "Задавать давление" );
    PARM_D( Flow_Z_kg, "Заданный поток кг", 0. );
    PARM_D( T_Out, "Заданная температура на выходе", 15. );
    PARM_D( Flow_Gas_nm3, "Заданный поток газов, нм3", 0. );
  ETAB
  TAB("Параметры",0)
  PARM_D( k_Reg_P, "Регулировка Здавления", 0. );
  ETAB
  return 0;
  }

#undef this

int CReflux::SaveState ( )
  {
  S_CLASS("W",CReflux_W)
    return 0;
  }
int CReflux::RestoreState ( char * StrName )
  {
  R_CLASS("W",CReflux_W)
    return 1;
  }

int CReflux::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
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

int CReflux::SetMatr( void * pExternals, int PntGroup,  double dt, 
                      double A[/* kExkV */], double B[ /*kE*/ ] )
  {
  double * Eq = A;
  Eq[P_0] = 1.;
  B[0] = P;
  Eq += 4;
  Eq[F_1] = 1.;
  if ( !Off )
    B[1] = Flow_Z_kg / Mw_Out;
  return 0;
  }
int CReflux::SetVar( void * pExternals, int PntGroup, double dt, 
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
  Mw_In = ::Mw ( pFlow_in->Get_Comp());
  Mw_Out = ::Mw ( pFlow_out->Get_Comp());
  Flow_kg = pFlow_in->Flow_mol * Mw_In;
  return 0;
  }
