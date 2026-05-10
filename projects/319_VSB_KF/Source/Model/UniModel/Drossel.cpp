#include "stdafx.h"
#include "UniModel.h"
#include "Err.h"
#include "CommProc.h"
#include "HydroStruct.h"
//#include "Data.h"
//#include "SetData.h"

struct CDrossel_W
  {
  double Omega, Mw, Flow;
  };

class CDrossel : public CCommonModel, public CDrossel_W
  {
  //
  public:
    CDrossel( char * _ObjName, IBaseModel * _pMain  );
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
    double dP, Flow_kg;
    int Defect;
    double Omega_def;

#include "IO_Create.h"
#include "ACS_Create.h"
    PARM_D( dP_nom, "Номинальный перепад", 0.01 );
    PARM_D( Flow_nom_m3, "Номинальный поток нм3", 100000. );
    PARM_D( Flow_nom_kg, "Номинальный поток кг", 0. );
    PARM_D( K_Rel, "Относительная проходимость", 1. );
    PARM_D( dP_Z, "Заданный перепад", 0. );
    PARM_D( Flow_Z_m3, "Заданный поток нм3", 0. );
    PARM_D( Flow_Z_kg, "Заданный поток кг", 0. );
    PARM_D( Omega_max, "Максимальная проходимость", 10000. );
    PARM_D( Pow_Defect, "Степень влияния засорения", 3.5 );
  };

CCommonModel * CreateDrossel( char * _ObjName, IBaseModel * _pMain  )
  {
  return new CDrossel( _ObjName, _pMain  );
  }

#undef PARM_D
#define PARM_D(Var,Name,Def) Var = Def;

CDrossel::CDrossel( char * _ObjName, IBaseModel * _pMain  ) : CCommonModel ( _ObjName, _pMain )
  {
  ObjName = _ObjName;
  Model = "Drossel";
  dP = Flow_kg = 0.;
  Defect = 0;
  Omega_def = 1.;
  Omega = 1.0;
  Mw = 18.;
  Flow = 0.;
  PARM_D( dP_nom, "Номинальный перепад", 0.01 );
  PARM_D( Flow_nom_m3, "Номинальный поток нм3", 100000. );
  PARM_D( Flow_nom_kg, "Номинальный поток кг", 0. );
  PARM_D( K_Rel, "Относительная проходимость", 1. );
  PARM_D( dP_Z, "Заданный перепад", 0. );
  PARM_D( Flow_Z_m3, "Заданный поток нм3", 0. );
  PARM_D( Flow_Z_kg, "Заданный поток кг", 0. );
  PARM_D( Omega_max, "Максимальная проходимость", 10000. );
  PARM_D( Pow_Defect, "Степень влияния засорения", 3.5 );
  }
#undef PARM_D

int CDrossel::StepT(double dt)
  {
  if ( Flow_nom_kg > 0. )
    {
    double F_mol = Flow_nom_kg / Mw;
    Flow_nom_m3 = F_mol * M3;
    Omega = F_mol / dP_nom * K_Rel;
    return 0;
    }
  if ( Flow_nom_m3 > 0. )
    {
    Omega = Flow_nom_m3 / ( M3 * dP_nom ) * K_Rel;
    return 0;
    }
  if ( dP_Z > 0. )
    {
    double absF = fabs ( Flow );
    Omega = 0.9 * Omega + 0.1 * absF / dP_Z;
    return 0;
    }
  if ( Flow_Z_kg > 0. )
    {
    double absF = fabs ( Flow );
    double F_kg = absF * Mw;
    if ( Omega * Flow_Z_kg < Omega_max * F_kg )
      Omega *= ( 0.9 + 0.1 * Flow_Z_kg / F_kg );
    else
      Omega = Omega_max;
    return 0;
    }
  if ( Flow_Z_m3 > 0. )
    {
    double absF = fabs ( Flow );
    double F_m3 = absF * M3;
    if ( Omega * Flow_Z_m3 < Omega_max * F_m3 )
      Omega *= ( 0.9 + 0.1 * Flow_Z_m3 / F_m3 );
    else
      Omega = Omega_max;
    return 0;
    }
  return 0;
  }

int CDrossel::Step1( )
  {
  StepT( 1e-4 );
  return 0;
  }

#define this pMain
#include "IO_Parms.h"

int CDrossel::GetParams( char * )
  {
  TAB("Режим",0)
  PARM( dP, "#Перепад" );
  PARM( Flow_kg, "#Поток, кг" );
  ETAB
  TAB("Конструктив",0)
  PARM_D( dP_nom, "Номинальный перепад", 0.01 );
  PARM_D( Flow_nom_m3, "Номинальный поток нм3", 100000. );
  PARM_D( Flow_nom_kg, "Номинальный поток кг", 0. );
  ETAB
  TAB("Параметры",0)
  PARM_D( Omega_max, "Максимальная проходимость", 10000. );
  PARM_D( Pow_Defect, "Степень влияния засорения", 3.5 );
  PARM_D( K_Rel, "Относительная проходимость", 1. );
  ETAB
  TAB("Задания",0)
  PARM_D( dP_Z, "Заданный перепад", 0. );
  PARM_D( Flow_Z_m3, "Заданный поток нм3", 0. );
  PARM_D( Flow_Z_kg, "Заданный поток кг", 0. );
  ETAB
  return 0;
  }

#undef this

int CDrossel::SaveState ( )
  {
  S_CLASS("W",CDrossel_W)
  return 0;
  }
int CDrossel::RestoreState ( char * StrName )
  {
  R_CLASS("W",CDrossel_W)
  return 1;
  }

int CDrossel::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
  {
  if ( kNodes != 2 )
    {
    ModMsg ( "У дросселя '%s' неправильное число входов", ObjName );
    return 2;
    }
  CObjectPoint * pOP_0 = ppNodes[0];
  CObjectPoint * pOP_1 = ppNodes[1];
  pOP_0->Type = eГидравлика;
  pOP_1->Type = eГидравлика;
  bool OK = false;
  if ( pOP_0->PntName[0] == 'i' && pOP_1->PntName[0] == 'o' )
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
  if ( pOP_0->PntName[0] == 'o' && pOP_1->PntName[0] == 'i' )
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
    ModMsg ( "У дросселя '%s' неправильные входы ('%s','%s')", ObjName, pOP_0->PntName.Str, pOP_1->PntName.Str );
    return 2;
    }
  pOP_0->PntGroup = 1;  // i,o
  pOP_1->PntGroup = 1;
  return 0;
  }

#include "SetMatr.h"

int CDrossel::SetMatr( void * pExternals, int PntGroup,  double dt, 
            double A[/* kExkV */], double B[ /*kE*/ ] )
  {
  if ( !_finite ( Omega ) )
    Omega = 1.;
  double O = Omega;
  if ( Defect == 1 )
    O *= Omega_def;
  MATR_OMEGA(O)
  return 0;
  }

#include "SetVar.h"

int CDrossel::SetVar( void * pExternals, int PntGroup, double dt, 
           double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
  {
  CFlow * pFlow_in = (CFlow*)pVar[0];
  CFlow * pFlow_out = (CFlow*)pVar[1];
  if ( Var[F_0] > 0. )
    *pFlow_out = *pFlow_in;
  else
    *pFlow_in = *pFlow_out;
  dP = Var[P_0] - Var[P_1];
  Mw = ::Mw ( pFlow_in->Get_Comp());
  Flow_kg = pFlow_in->Flow_mol * Mw;
  return 0;
  }
