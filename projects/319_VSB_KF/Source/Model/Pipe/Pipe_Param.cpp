#include "stdafx.h"
#include "Pipe.h"
#include "Err.h"
#include "CommProc.h"
#ifdef LINUX
#else
#include "..\Drainage\Drainage.h"
#include "IO.h"
#include "Work.h"
#include "Connect.h"
#endif


_W::_W()
	{
  memset ( this, 0, sizeof ( _W ));
	}

double CPipe::Q_max = 10.;
int TypeFlow_C = 0;
CParams * CPipe::pParams = NULL;
int CPipe::kParams = -1;
bool CPipe::FormListParams = false;

#ifdef N2
//#pragma comment (lib,"Start.lib")
char ErrN2[256];
#endif 

#ifdef N2
double CPipe::Limit_N2 = 1.;
bool CPipe::Show_N2 = false;
#endif


char Alarm_Pipe[128];

bool CPipe::GetParamsList( struct CParams ** ppParams, int * pkParams )
  {
  if ( kParams <= 0 )
    return false;
  *ppParams = pParams;
  *pkParams = kParams;
  return true;
  }

int PressMes = 0;
int FlowMes = 0;
int CPipe::GetParams(  char * StrName )
	{
  //char * P1 = ObjName;
  //if ( pBaseObj == NULL && *P1 == '_' )
  //  pBaseObj = IBaseModel::Find( P1 + 1 );
  //if ( pBaseObj )
  //  {
  //  return pBaseObj->GetParams( StrName );
  //  }
  ShowP.pMes = &PressMes;
  ShowFlow.pMes = &FlowMes;
#define SHOW
	#include "IO_Parms.h"
	#include "Points.h"
#undef SHOW
  if ( FormListParams )
    {
    PARM_L( TypeFlow_C, "Выводить", "моль %;вес %;моль;кг;" );
    return 0;
    }
  char Name[256];
  CTRL_Params( "Задания", this );
  TAB("Потоки(>0 в трубу)",5)
  for ( int n = 0; n < kF; n++ )
    {
    sprintf_s( Name, 256, "#%s.%s", (char*)pNodes[n].ObjName, (char*)pNodes[n].Point );
    if ( pShowFlow[n] )
      pShowFlow[n]->GetParams( Name, false );
    }
  PARM( Cmol_In[0], "Состав" )
  ETAB
//
	TAB("Потоки компонент",5)
  if ( !CParams::SaveAllParams && !CParams::ReadProps )
    {
    if ( FlowNames == NULL )
      {
      int Len = lstrlen ( "Сумма;" );
      for ( int n = 0; n < kF; n++ )
        {
        Len += sprintf_s( Name, 256, "%s.%s;", (char*)pNodes[n].ObjName, (char*)pNodes[n].Point );
        }
      FlowNames = new char[Len+4];
      lstrcpy ( FlowNames, "Сумма;" );
      int P = 6;
      for ( int n = 0; n < kF; n++ )
        {
        Len = sprintf_s( Name, 256, "%s.%s;", (char*)pNodes[n].ObjName, (char*)pNodes[n].Point );
        memcpy ( FlowNames + P, Name, Len );
        P += Len;
        }
      FlowNames[P] = 0;
      }
    }
  PARM_L( nFlow_C, "Выводимый поток", FlowNames );
  CParams::GetLast().pShowParam = &Show;
  bool * IsComp = pComp_out->IsComp;
  PARM_L( TypeFlow_C, "Выводить", "моль %;вес %;моль;кг;" );
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      {
      if ( UseAlt_CompNames )
        sprintf_s ( Name, sizeof(Name), "#%s(%d)", pGas[n]->Alt_Name, n );
      else
        sprintf_s ( Name, sizeof(Name), "#%s(%d)", pGas[n]->Name, n );
      PARM ( Flow_C[n], Name )
      }
    }
  ETAB
  Defect_Params(this);
  ACS_Params( this );
  return 0;
  }

int CPipe::UpdateParam( CParams & Param )
  {
#ifdef SPECIAL
  if ( Param.Addr == &_Test )
    {
    if ( _Test == 0 )
      return 0;
    if ( Test_Flow_mol == NULL )
      Test_Flow_mol = NewArr(double,kF);
    for ( int n = 0; n < kF; n++ )
      {
      if ( pFlow[n] )
        Test_Flow_mol[n] = pFlow[n]->Flow_mol;
      }
    return 0;
    }
#endif
  //if ( Param.Addr == &Use_Filtr_Flow )
  //  {
  //  if ( Use_Filtr_Flow && Filtr_Flow == NULL )
  //    {
  //    Filtr_Flow = NewArr(CFiltrFlow,kF);
  //    for ( int n = 0; n < kF; n++ )
  //      {
  //      Filtr_Flow[n].x = -1;
  //      Filtr_Flow[n].Use = false;
  //      }
  //    return 0;
  //    }
  //  if ( !Use_Filtr_Flow )
  //    {
  //    for ( int n = 0; n < kF; n++ )
  //      {
  //      Filtr_Flow[n].x = -1;
  //      Filtr_Flow[n].Use = false;
  //      }
  //    return 0;
  //    }
  //  return 0;
  //  }
  //if ( Use_Filtr_Flow )
  //  {
  //  for ( int n = 0; n < kF; n++ )
  //    {
  //    if ( Param.Addr == &Filtr_Flow[n].K )
  //      {
  //      if ( Filtr_Flow[n].K <= 0. )
  //        Filtr_Flow[n].Use = false;
  //      else
  //        Filtr_Flow[n].Use = true;
  //      return 0;
  //      }
  //    }
  //  }
  //if ( Param.Addr == &Test_P && Test_P )
  //  {
  //  pControl_P = new CAnalogControl ( "Давление", this, &_P, &P_test_nom, &P_test_delta );
  //  P_test_nom = _P;
  //  P_test_delta = 0.05 * P_test_nom;
  //  return 0;
  //  }
  //if ( Param.Addr == &Test_F && Test_F )
  //  {
  //  pControl_F = new CAnalogControl ( "Поток", this, &_F_kg, &F_test_nom, &F_test_delta );
  //  F_test_nom = _F_kg;
  //  F_test_delta = 0.05 * F_test_nom;
  //  return 0;
  //  }
  //if ( Param.Addr == &Test_S && Test_S )
  //  {
  //  pControl_Stable = new CStableControl ( "Стабильность", this, &_F_mol );
  //  return 0;
  //  }
  if ( SaveFlow )
    {
    bool * IsComp = pComp_out->IsComp;
    double * Cmol = pComp_out->Cmol;
    if ( nFlow_C > 0 && nFlow_C <= kF )
      {
      if ( pComp_in[nFlow_C-1] == NULL )
        return 0;
      Cmol = pComp_in[nFlow_C-1]->Cmol;
      }
    SaveFlow = false;
    char Path[1024];
    sprintf_s ( Path, 1024, "%sDATA\\СОСТАВЫ\\Pipe_%s.csv", PROJECT_ROOT, ObjName.Str );
    if ( _access ( Path, 0 ) == 0 )
      {
      int n = 1;
      while ( 1 )
        {
        sprintf_s ( Path, 1024, "%sDATA\\СОСТАВЫ\\Pipe_%s(%d).csv", PROJECT_ROOT, ObjName.Str, n );
        if ( _access ( Path, 0 ) == 0 )
          {
          n++;
          continue;
          }
        break;
        }
      }
    TakeOffReadOnly( Path );
    FILE * F = fopen ( Path, "wt" );
    for ( int n = 0; n < LAST_GAS; n++ )
      {
      if ( IsComp[n] )
        fprintf( F, "%s;%7.4lf;\n", pGas[n]->Name, Cmol[n] * 100. );
      else
        fprintf( F, "%s;0;\n", pGas[n]->Name );
      }
    fclose ( F );
    return 0;
    }
  if ( pEtalonComp )
    {
    pEtalonComp->UpdateParams( Param );
    }
  return 0;
  }

//#include "ShowParams.h"
//PARM_D( T_out_Z, "Заданная т-ра", 0. )
//PARM_D( F_Fix, "Заданный поток", 0. )
//PARM_I( nF_Fix, "#N заданного потока", -1 )
//PARM_D( P_Fix, "Заданное давление(а)", 1. )
//PARM_B( Set_P_Fix, "#Задать давление", false )

int CPipe::ShowParams( DWORD DataType, struct CShowData * pSD )
  {
	switch ( DataType )
		{
    case sp_Actions:
      pSD->NewAction = pSD->ОкноСоставов;
		return 1;
		}
  return 0;
  }

void CPipe::PrepareParams ( struct CParams * Param )
  {
  SET_BP BreakPoint;
  extern int TypeFlow_C;
  if ( TypeFlow_C	>= 0 )
    {
    double F;
    CComp * pComp;
    if ( nFlow_C == 0 || nFlow_C > kF )
      {
      F = F_mol;
      pComp = pComp_out;
      }
    else
      {
      if ( pFlow[nFlow_C-1] == NULL )
        return;
      F = pFlow[nFlow_C-1]->Flow_mol;
      if ( Type[nFlow_C-1] == '+' )
        F = -F;
      pComp = pComp_in[nFlow_C-1];
      }
    if ( pComp )
      {
      bool * IsComp = pComp->IsComp;
      double * Cmol = pComp->Cmol;
      // "моль %;вес %;моль;кг;"
      if ( TypeFlow_C == 0 )
        {
        for ( int n = 0; n < LAST_GAS; n++ )
          {
          if ( IsComp[n] )
            Flow_C[n] = Cmol[n] * 100.;
          else
            Flow_C[n] = 0.;
          }
        }
      if ( TypeFlow_C == 1 )
        {
        double S = 0.;
        for ( int n = 0; n < LAST_GAS; n++ )
          {
          if ( IsComp[n] )
            S += Cmol[n] * pGas[n]->Mw;
          }
        if ( S > 1. )
          S = 100. / S;
        for ( int n = 0; n < LAST_GAS; n++ )
          {
          if ( IsComp[n] )
            Flow_C[n] = Cmol[n] * pGas[n]->Mw * S;
          else
            Flow_C[n] = 0.;
          }
        }
      if ( TypeFlow_C == 2 )
        {
        for ( int n = 0; n < LAST_GAS; n++ )
          {
          if ( IsComp[n] )
            Flow_C[n] = F * Cmol[n];
          else
            Flow_C[n] = 0.;
          }
        }
      if ( TypeFlow_C == 3 )
        {
        for ( int n = 0; n < LAST_GAS; n++ )
          {
          if ( IsComp[n] )
            Flow_C[n] = F * Cmol[n] * pGas[n]->Mw;
          else
            Flow_C[n] = 0.;
          }
        }
      }
    }
  }
