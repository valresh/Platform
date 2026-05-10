#include "stdafx.h"
#include "Furnace.h"
#include "Err.h"
#include "CommProc.h"
#include "Work.h"


_W::_W()
	{
  memset ( this, 0, sizeof ( _W ));
  T_Wall = 15.;
  MinWorkGas = 0.1;
	}

CDepression::CDepression()
  {
  Mw_gas = Mw_air = Mw_smoke = Mw_podsos = 20.;
  Mw_mazut = 160.;
  Mw_steam = Mw_H2O;
  Flow_topl_gas = Flow_mazut = Flow_steam = Flow_air_fors = Flow_air_add = 0.;
  Omega_podsos = Omega_out_smoke = 1000000.;
  }

int CFurnace::SetProp ( struct CObjProps * pProp )
  {
  if ( pProp->PropName != "Число_форсунок" )
    return 1;
  if ( pProp->Value == NULL )
    return 2;
  kFors = atoi ( pProp->Value ); 
  if ( kFors < 1 || kFors > 100 )
    return 3;
  return 0;
  }

#include "IO_Props.h"
int CFurnace::GetProp( )
  {
  PARM( "Число форсунок", 'I', &kFors ); // Список
  return 0;
  }

int CFurnace::GetParams( char * )
	{
	#include "IO_Parms.h"
	#include "Points.h"
	char Name[128];
  TAB("Продукт",3)
  for ( int n = 0; n < kProduct; n++ )
    {
    sprintf_s ( Name, 128, "#%d - поток, моль", n + 1 );
    PARM( pTube[n]->pFlow_in->Flow_mol, Name );
    PARM( pTube[n]->T_in, "#  - T на входе" );
    PARM( pTube[n]->T_out, "#  - T на выходе" );
    PARM( pTube[n]->In.E, "#  - E на входе" );
    PARM( pTube[n]->Out.E, "#  - E на выходе" );
    sprintf_s ( Name, 128, "%d - Отн. нагрев", n + 1 );
    PARM( W_Tube[n], Name );
//    sprintf_s ( Name, 128, "%d - KPD", n + 1 );
//    PARM( pTube[n]->KPD, Name );
    }
  ETAB
  TAB("Форсунки",3)
  for ( int n = 0; n < kFors; n++ )
    {
    sprintf_s ( Name, 128, "#%d - ShibGas", n + 1 );
    PARM( pFire[n]->Logic.pFors->ShibGas, Name );
    }
  for ( int n = 0; n < kFors; n++ )
    {
    sprintf_s ( Name, 128, "#%d - VentGas", n + 1 );
    PARM( pFire[n]->Logic.pFors->VentGas, Name );
    }
  for ( int n = 0; n < kFors; n++ )
    {
    sprintf_s ( Name, 128, "#%d - VentPilot", n + 1 );
    PARM( pFire[n]->Logic.pFors->VentPilot, Name );
    }
  for ( int n = 0; n < kFors; n++ )
    {
    /////////////////
    sprintf_s ( Name, 128, "#%d - ShibMazut", n + 1 );
    PARM( pFire[n]->Logic.pFors->ShibMazut, Name );
    }
  for ( int n = 0; n < kFors; n++ )
    {
    sprintf_s ( Name, 128, "#%d - VentMazut", n + 1 );
    PARM( pFire[n]->Logic.pFors->VentMazut, Name );
    }
  for ( int n = 0; n < kFors; n++ )
    {
    sprintf_s ( Name, 128, "#%d - VentSteam", n + 1 );
    PARM( pFire[n]->Logic.pFors->VentSteam, Name );
    }
  for ( int n = 0; n < kFors; n++ )
    {
    sprintf_s ( Name, 128, "#%d - VentSteam", n + 1 );
    PARM( pFire[n]->Logic.pFors->VentSteam, Name );
    }
  ETAB
  TAB("Горение",3)
  for ( int n = 0; n < kFors; n++ )
    {
    sprintf_s ( Name, 128, "#%d - Зажигание пилота", n + 1 );
    PARM( pFire[n]->Logic.pFors->setFire, Name );
    }
  for ( int n = 0; n < kFors; n++ )
    {
    sprintf_s ( Name, 128, "#%d - Пилот горит", n + 1 );
    PARM( Горит_пилот[n]->Value, Name );
    }
  for ( int n = 0; n < kFors; n++ )
    {
    sprintf_s ( Name, 128, "#%d - Горелка горит", n + 1 );
    PARM( Горит[n]->Value, Name );
    }
  ETAB
  TAB("Змеевики",3)
    for ( int n = 0; n < kProduct; n++ )
      {
      sprintf_s ( Name, 128, "Нагрев змеевика %d", n + 1 );
      PARM( k_Zm[n], Name );
      }
  ETAB
  //for ( int n = 0; n < 4; n++ )
  //  {
  //  sprintf_s ( Name, 128, "#%d - Газ(внешний)", n + 1 );
  //  PARM( Gas[n], Name );
  //  }
  //for ( int n = 0; n < 4; n++ )
  //  {
  //  sprintf_s ( Name, 128, "#%d - Пилот(внешний)", n + 1 );
  //  PARM( Pilot[n], Name );
  //  }
 // ShowNode.GetParams();
//	TAB("Точки", 2)
  Defect_Params(this);
  ACS_Params( "АСУ", this );
//#include "ACS_Param.h"
//#include "ACS.h"
  CSpecPnt * pZ = pFirstPnt;
  while ( pZ )
    {
    pZ->pA->GetParams();
    pZ = pZ->pNext;
    }
//	TAB("Модули",5)
	return 0;
	}

int CFurnace::UpdateParam( CParams & Param )
  {
  if ( Param.Addr == &No_Friction && No_Friction )
    {
    Alfa_P_fors = P_fors_mm / P_out_smoke_mm;
    return 0;
    }
  if ( Param.Addr == &Calc_T4 && Calc_T4 )
    Calc_T_Wall = false;
  if ( Param.Addr == &Calc_T_Wall && Calc_T_Wall )
    Calc_T4 = false;
  double S = 0.;
  for ( int n = 0; n < kProduct; n++ )
    S += W_Tube[n];
  S = 1. / S;
  for ( int n = 0; n < kProduct; n++ )
    W_Tube[n] *= S;
  if ( CalcdP )
    {
    double dP = H_Calc * ( T_air + TK ) / ( kdP_Calc * ( T_Calc - T_air ) + T_air + TK );
    dP_Calc = dP - H_Calc;
    }
  if ( Calc_T4 )
    {
    double S = S_Fors / kFors;
    for ( int n = 0; n < kFors; n++ )
      {
      pFire[n]->S_Fors = S;
      pFire[n]->CreateRad( );
      }
    S = S_Trub / kProduct;
    for ( int n = 0; n < kProduct; n++ )
      {
      pTube[n]->S_Trub = S;
      }
    }
  CalcCoef( );
  return 0;
  }

//#include "ShowParams.h"

int CFurnace::ShowParams( DWORD DataType, struct CShowData * pSD )
  {
/*
	switch ( DataType )
		{
		case sp_Params:
		  GetParams( "" );
		return 1;
		}
*/
  return 0;
  }
