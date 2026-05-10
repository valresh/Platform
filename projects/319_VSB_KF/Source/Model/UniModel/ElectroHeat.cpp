#include "stdafx.h"
#include "UniModel.h"
#include "Err.h"
#include "CommProc.h"
#include "HydroStruct.h"
#include "TestNodes.h"


struct CElectroHeat_W
  {
  double Qh;
  };

class CElectroHeat : public CCommonModel, public CElectroHeat_W
  {
  public:
    CElectroHeat( char * _ObjName, IBaseModel * _pMain  );
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
    class CUniModel * pUM;
//
#include "IO_Create.h"
//#include "ACS_Create.h"
//    ACS_DI(Пуск_нагрев,"Пуск_нагрев","Пуск нагревателя")
//    ACS_DI(Стоп_нагрев,"Стоп_нагрев","Стоп нагревателя")
//    ACS_DI(Пуск_Стоп_нагрев,"Пуск/Стоп нагрев","Пуск/Стоп нагревателя")
//    ACS_DI(Пуск_стоп_нагрев_мест,"Пуск_стоп_нагрев по месту","Пуск(1) стоп(0) нагревателя")
//    ACS_DI(Мест_дист_нагрев,"Мест_дист_нагрев","Местное управление(0) дистанционное(1) нагревателем")
//    ACS_DI(Стоп_по_блокировке,"Стоп_по_блокировке","Стоп нагревателя")
//    ACS_AI(Нагрев,"Нагрев","Мощность нагревателя")
//    ACS_AO(Ток,"Ток","Ток")
//    ACS_DO(Нагреватель_в_работе,"Нагреватель_в_работе","Нагреватель включен")
//    ACS_DO(Авария,"Авария","Авария нагревателя")
//
    PARM_B( On, "Работает", false );
    PARM_D( T_Out, "#Температура на выходе", 0. );
    PARM_D( Q, "Мощность, кВт", 0. );
    PARM_D( TauHeat,"Время нагрева, с", 20.)
    PARM_D( Omega,"Проходимость", 1000.)
    PARM_D( T_Max, "Макс. температура на выходе", 400. );
    PARM_D( T_Z, "Заданная температура на выходе", 0. );
//
    void ElectroHeat( double dt );
  };

CCommonModel * CreateElectroHeat( char * _ObjName, IBaseModel * _pMain  )
  {
  CElectroHeat * pObj = new CElectroHeat( _ObjName, _pMain  );
  pObj->pUM = (CUniModel*)_pMain;
  return pObj;
  }

CElectroHeat::CElectroHeat( char * _ObjName, IBaseModel * _pMain  ) : CCommonModel ( _ObjName, _pMain ) 
, In("Вход",this)
, Out("Выход",this)
  {
  ObjName = _ObjName;
  Model = "ElectroHeat";
  pFlow_in = NULL;
  pFlow_out = NULL;
#include "IO_Clear.h"
  PARM_B( On, "Работает", false );
  PARM_D( Q, "Мощность, кВт", 0. );
  PARM_D( TauHeat,"Время нагрева, с", 20.)
  PARM_D( T_Out, "#Температура на выходе", 0. );
  PARM_D( Omega,"Проходимость", 1000.)
  PARM_D( T_Max, "Макс. температура на выходе", 400. );
  PARM_D( T_Z, "Заданная температура на выходе", 0. );
  }

int CElectroHeat::Init( int Regim )
  {
  return 0;
  }

void CElectroHeat::Calc ( double dt )
  {
  ElectroHeat( dt );
  }

#define this pMain
#include "IO_Parms.h"
int CElectroHeat::GetParams( char * )
  {
  PARM_B( On, "Работает", false );
  PARM_D( T_Out, "#Температура на выходе", 0. );
  PARM_D( Q, "Мощность, кВт", 0. );
  PARM_D( TauHeat,"Время нагрева, с", 20.)
  PARM_D( Omega,"Проходимость", 1000.)
  PARM_D( T_Max, "Макс. температура на выходе", 400. );
  PARM_D( T_Z, "Заданная температура на выходе", 0. );
  return 0;
  }

void CElectroHeat::ElectroHeat( double dt )
  {
  double Ksi = dt * 3600. / TauHeat;
  if ( pUM->Пуск_Стоп_нагрев.Use() || pUM->Пуск_стоп_нагрев_мест.Use() || pUM->Пуск_нагрев.Use() || pUM->Стоп_нагрев.Use())
    {
    double Qz = 1.;
    if ( pUM->Нагрев.Use())
      Qz = pUM->Нагрев * 0.01;
    bool Mest = pUM->Мест_дист_нагрев.Off();
    if( Mest )
      {
      if (pUM->Пуск_стоп_нагрев_мест.On())
        {
        Qh = ( Qh + Ksi * Qz * Q * 3600. ) / ( 1. + Ksi );
        On = true;
        pUM->Нагреватель_в_работе = true;
        }
      else
        {
        Qh = Qh / ( 1. + Ksi );
        pUM->Нагреватель_в_работе = false;
        On = false;
        }
      }
    else
      {
      if (pUM->Пуск_Стоп_нагрев.On())
        {
        Qh = ( Qh + Ksi * Qz * Q * 3600. ) / ( 1. + Ksi );
        pUM->Нагреватель_в_работе = true;
        On = true;
        }
      else if(pUM->Пуск_Стоп_нагрев.Off())
        {
        Qh = Qh / ( 1. + Ksi );
        On = false;
        pUM->Нагреватель_в_работе = false;
        }
      if(pUM->Пуск_нагрев.On())
        {
        Qh = ( Qh + Ksi * Qz * Q * 3600. ) / ( 1. + Ksi );
        pUM->Нагреватель_в_работе = true;
        On = true;
        }
      if(pUM->Стоп_нагрев.On())
        {
        Qh = Qh / ( 1. + Ksi );
        On = false;
        pUM->Нагреватель_в_работе = false;
        }
      }		
    }
  else
    {
    if ( On )
      Qh = ( Qh + Ksi * Q * 3600. ) / ( 1. + Ksi );
    else
      Qh = Qh / ( 1. + Ksi );
    }
  double F = fabs ( pFlow_out->Flow_mol );
  if ( F < 0.1 )
    F = 0.1;
  *pFlow_out = *pFlow_in;
  if ( T_Z > 0. )
    {
    On = true;
    Out.PT( pFlow_out->P, T_Z, pFlow_out->Get_Comp() );
    Qh = F * ( Out.h - pFlow_in->h_Flow );
    Q = Qh / 3600.;
    pFlow_out->h_Flow = Out.h;
    pFlow_out->T = Out.T;
    T_Out = Out.T;
    return;
    }
  pFlow_out->h_Flow = pFlow_in->h_Flow + Qh / F;
  Out.PH( pFlow_out );
  if ( Out.T > T_Max )
    {
    Out.PT( pFlow_out->P, T_Max, pFlow_out->Get_Comp() );
    }
  pFlow_out->h_Flow = Out.h;
  pFlow_out->T = Out.T;
  T_Out = Out.T;
  }

int CElectroHeat::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
  {
  if ( kNodes != 2 )
    {
    ModMsg ( "У нагревателя '%s' неправильное число входов", ObjName );
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

int CElectroHeat::SetMatr( void * pExternals, int PntGroup,  double dt, 
                        double A[/* kExkV */], double B[ /*kE*/ ] )
  {
  MATR_OMEGA(Omega)
  return 0;
  }

int CElectroHeat::SetVar( void * pExternals, int PntGroup, double dt, 
                       double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
  {
  pFlow_in = (CFlow*)pVar[0];
  pFlow_out = (CFlow*)pVar[1];
  return 0;
  }

