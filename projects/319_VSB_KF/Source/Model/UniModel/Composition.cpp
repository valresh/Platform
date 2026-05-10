#include "stdafx.h"
#include "UniModel.h"
#include "Err.h"
#include "HydroStruct.h"
#include "TestNodes.h"
//#include "CommProc.h"
//#include "Data.h"
//#include "SetData.h"

struct CComposition_W
  {
  bool SetNewComp;
  double Omega;
  double Mw_in;
  double Mw_out;
  };

#define K_R_GAS 6

class CComposition : public CCommonModel, public CComposition_W
  {
  //
  public:
    CComposition( char * _ObjName, IBaseModel * _pMain  );
    // Интерфейс IBaseModel
    int Init( int Regim );
    //    int Step0( );
    int Step1( );
    int StepT(double dt);
    int GetParams( char * );
    //    int ShowParams( DWORD DataType, struct CShowData * pSD );
    int UpdateParam( struct CParams & Param );
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
    CFlow * pFlow_in;
    CFlow * pFlow_out;
    CComp * pComp_out;
    int ID_Comp_out;
    bool DirectOutComp;
    double dP, Flow_kg;
    int ID_Gas[K_R_GAS];
    double Gas_in[K_R_GAS];
    double Gas_out[K_R_GAS];
    double Real_Gas_out[K_R_GAS];
    double Gas_et[K_R_GAS];
    double Diff[K_R_GAS];
    double Coef[K_R_GAS];
#include "IO_Create.h"
#include "ACS_Create.h"
    PARM_D( dP_nom, "Номинальный перепад", 0.01 );
    PARM_D( Flow_nom_m3, "Номинальный поток нм3", 100000. );
    PARM_D( Flow_nom_kg, "Номинальный поток кг", 0. );
    PARM_B( NormComp, "Нормировать выходной состав", false );
    };

CCommonModel * CreateComposition( char * _ObjName, IBaseModel * _pMain  )
  {
  return new CComposition( _ObjName, _pMain  );
  }

#undef PARM_D
#define PARM_D(Var,Name,Def) Var = Def;
#undef PARM_B
#define PARM_B(Var,Name,Def) Var = Def;

CComposition::CComposition( char * _ObjName, IBaseModel * _pMain  ) : CCommonModel ( _ObjName, _pMain )
, In("Вход",this)
  {
  dP = Flow_kg = 0.;
  pFlow_in = NULL;
  pFlow_out = NULL;
  Mw_out = Mw_in = 42;
  DirectOutComp = true;
  PARM_B( SetNewComp, "Задать новый состав", false );
  PARM_D( dP_nom, "Номинальный перепад", 0.01 );
  PARM_D( Flow_nom_m3, "Номинальный поток нм3", 100000. );
  PARM_D( Flow_nom_kg, "Номинальный поток кг", 0. );
  PARM_B( NormComp, "Нормировать выходной состав", false );
  CLEAR(ID_Gas)
  CLEAR(Gas_in)
  CLEAR(Gas_out)
  CLEAR(Gas_et)
  CLEAR(Diff)
  }
#undef PARM_D
#undef PARM_B

int CComposition::Init( int Regim )
  {
  NewComp( &ID_Comp_out, &pComp_out, "Состав рецикла из %s", ObjName.Str );
  ID_Gas[0] = GetGasID( "PROPANE");
  Gas_out[0] = Gas_et[0] = 12.32;
  ID_Gas[1] = GetGasID( "PROPYLENE");
  Gas_out[1] = Gas_et[1] = 86.81;
  ID_Gas[2] = GetGasID( "ETHYLENE");
  Gas_out[2] = Gas_et[2] = 0.026;
  ID_Gas[3] = GetGasID( "ETHANE");
  Gas_out[3] = Gas_et[3] = 0.492;
  ID_Gas[4] = GetGasID( "NITROGEN");
  Gas_out[4] = Gas_et[4] = 0.0690;
  ID_Gas[5] = GetGasID( "HYDROGEN");
  Gas_out[5] = Gas_et[5] = 0.215;
  return 0;
  }

int CComposition::UpdateParam( struct CParams & Param )
  {
  return 0;
  }


int CComposition::StepT(double dt)
  {
  if ( Flow_nom_kg > 0. )
    {
    double F_mol = Flow_nom_kg / Mw_in;
    Flow_nom_m3 = F_mol * M3;
    Omega = F_mol / dP_nom;
    }
  if ( Flow_nom_m3 > 0. )
    {
    Omega = Flow_nom_m3 / ( M3 * dP_nom );
    }
///////////////////////////////////////////////////////
  In.PH ( pFlow_in );
  CComp * pComp_in = pFlow_in->Get_Comp();
  for ( int n = 0; n < K_R_GAS; n++ )
    {
    int ID = ID_Gas[n];
    if ( pComp_in->IsComp[ID] )
      {
      Gas_in[n] = pComp_in->Cmol[ID] * 100.;
      if ( Gas_out[n] > 0. )
        Diff[n] = Gas_in[n] / Gas_et[n];
      else
        Diff[n] = -Gas_in[n] / Gas_et[n];
      if ( Gas_in[n] > 0. && DirectOutComp )
        Coef[n] = Gas_out[n] / Gas_in[n];
      }
    else
      {
      Gas_in[n] = 0.;
      Diff[n] = 0.;
      }
    }
  if ( !SetNewComp )
    {
    *pFlow_out = *pFlow_in;
    return 0;
    }
  double S = 0.;
  double kIO = 1.;
  if ( pFlow_in->Flow_mol > 1 )
    kIO = pFlow_in->Flow_mol / pFlow_out->Flow_mol;
//
  double Sp = 0.;
  for ( int n = 0; n < K_R_GAS; n++ )
    {
    Sp += Gas_out[n];
    }
  if ( fabs ( Sp - 1. ) > 1e-5 )
    {
    Sp = 100. / Sp;
    for ( int n = 0; n < K_R_GAS; n++ )
      {
      Gas_out[n] *= Sp;
      }
    }
  for ( int n = 0; n < K_R_GAS; n++ )
    {
    int ID = ID_Gas[n];
    pComp_out->IsComp[ID] = true;
    if (  DirectOutComp )
      {
      pComp_out->Cmol[ID] = Gas_out[n] * 0.01;
      }
    else
      {
      pComp_out->Cmol[ID] = Coef[n] * Gas_in[n] * 0.01;
      }
    Real_Gas_out[n] = pComp_out->Cmol[ID] * 100.;
    S += pComp_out->Cmol[ID];
    }
  Norm ( pComp_out );
  Mw_out = Mw ( pComp_out );
  if ( NormComp )
    {
    S = 100. / S;
    for ( int n = 0; n < K_R_GAS; n++ )
      {
      Gas_out[n] *= S;
      }
    }
  pFlow_out->ID_COMP = ID_Comp_out;
  pFlow_out->pComp = pComp_out;
  pFlow_out->T = In.T;
  pFlow_out->h_Flow = H_IG ( In.T, pComp_out );
  return 0;
  }

int CComposition::Step1( )
  {
  if ( Flow_nom_kg > 0. )
    {
    double F_mol = Flow_nom_kg / Mw_in;
    Flow_nom_m3 = F_mol * M3;
    Omega = F_mol / dP_nom;
    }
  if ( Flow_nom_m3 > 0. )
    {
    Omega = Flow_nom_m3 / ( M3 * dP_nom );
    }
  return 0;
  }

#define this pMain
#include "IO_Parms.h"

int CComposition::GetParams( char * )
  {
  TAB("Режим",0)
  PARM( dP, "#Перепад" );
  PARM( Flow_kg, "#Поток, кг" );
  PARM( In.T, "#Температура на входе" );
  ETAB
  TAB("Конструктив",0)
  PARM_D( dP_nom, "Номинальный перепад", 0.01 );
  PARM_D( Flow_nom_m3, "Номинальный поток нм3", 100000. );
  PARM_D( Flow_nom_kg, "Номинальный поток кг", 0. );
  ETAB
  TAB("Составы",0)
  PARM( SetNewComp, "Задавать выходной состав" );
  PARM( DirectOutComp, "Напрямую задавать выходной состав" );

  TAB("Вход",0)
    Char<128> Name;
    for ( int n = 0; n < K_R_GAS; n++ )
      {
      int ID = ID_Gas[n];
      Name.Prt ( "# %s", pGas[ID]->Name );
      PARM( Gas_in[n], Name );
      }
  ETAB
  TAB("Отличия",0)
  Char<128> Name;
  for ( int n = 0; n < K_R_GAS; n++ )
    {
    int ID = ID_Gas[n];
    Name.Prt ( "# %s", pGas[ID]->Name );
    PARM( Diff[n], Name );
    }
  ETAB
  TAB("Выход/Вход",0)
    Char<128> Name;
    for ( int n = 0; n < K_R_GAS; n++ )
      {
      int ID = ID_Gas[n];
      Name.Prt ( "вых/вх %s", pGas[ID]->Name );
      PARM( Coef[n], Name );
      }
  ETAB
  TAB("Задаваемый выход",0)
  Char<128> Name;
  for ( int n = 0; n < K_R_GAS; n++ )
    {
    int ID = ID_Gas[n];
    Name.Prt ( "Выход %s", pGas[ID]->Name );
    PARM( Gas_out[n], Name );
    }
  ETAB
    TAB("Реальный выход",0)
    Char<128> Name;
  for ( int n = 0; n < K_R_GAS; n++ )
    {
    int ID = ID_Gas[n];
    Name.Prt ( "# %s", pGas[ID]->Name );
    PARM( Real_Gas_out[n], Name );
    }
  ETAB
  ETAB
  return 0;
  }

#undef this

int CComposition::SaveState ( )
  {
  S_CLASS("W",CComposition_W)
  return 0;
  }
int CComposition::RestoreState ( char * StrName )
  {
  COPY_FIRST
  R_CLASS("W",CComposition_W)
  return 1;
  }

int CComposition::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
  {
  if ( kNodes != 2 )
    {
    ModMsg ( "У объъекта '%s' неправильное число входов", ObjName );
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
    ModMsg ( "У объъекта '%s' неправильные входы ('%s','%s')", ObjName, pOP_0->Point, pOP_1->Point );
    return 2;
    }
  pOP_0->PntGroup = 1;  // i,o
  pOP_1->PntGroup = 1;
  return 0;
  }

#include "SetMatr.h"

int CComposition::SetMatr( void * pExternals, int PntGroup,  double dt, 
                      double A[/* kExkV */], double B[ /*kE*/ ] )
  {
  double * Eq = A;
  Eq[P_0] = Omega;
  Eq[P_1] = -Omega;
  Eq[F_0] = -1.;
  Eq += 4;
  Eq[F_0] =  Mw_in;//1.;
  Eq[F_1] = -Mw_out;//;
  return 0;
  }
int CComposition::SetVar( void * pExternals, int PntGroup, double dt, 
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
  Mw_in = ::Mw ( pFlow_in->Get_Comp());
  Flow_kg = pFlow_in->Flow_mol * Mw_in;
  return 0;
  }
