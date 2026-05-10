#include "stdafx.h"
#include "Furnace.h"
#include "Err.h"
#include "CommProc.h"
#include "Work.h"
#include "HydroStruct.h"

_W::_W()
	{
  memset ( this, 0, sizeof ( _W ));
  T_Wall = 15.;
  Mu_Smoke_in = 28.;
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
  char TabName[128];
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
  if ( pCreatePyrolysis )
    {
    TAB("Пиролиз",3)
    for ( int n = 0; n < kProduct; n++ )
      {
      TAB(pTube[n]->pBasePyrolysis->ObjName,3)
      pTube[n]->pBasePyrolysis->GetParams( NULL );
      ETAB
      }
    ETAB
    }
//  TAB("Воздух",3)
//    sprintf_s ( Name, 128, "#Разряжение на входе воздуха %d ф. мм в ст", n + 1 );
  for ( int n = 0; n < kFors; n++ )
    {
    sprintf_s ( TabName, 128, "Форсунка %d", n + 1 );
    TAB(TabName,3)
		sprintf_s ( Name, 128, "#Разряжение на выходе дыма" );
		PARM( P_Smoke[n], Name );
		sprintf_s ( Name, 128, "#Разряжение в камере" );
		PARM( Pz_mm[n], Name );
		sprintf_s ( Name, 128, "#Давление газа(и)" );
		PARM( P_Gas[n], Name );
		sprintf_s ( Name, 128, "#Поток газа, м3" );
		PARM( F_Gas_m3[n], Name );
		sprintf_s ( Name, 128, "#Поток мазута, кг" );
		PARM( F_Mazut_kg[n], Name );
		sprintf_s ( Name, 128, "#Поток пара, кг" );
		PARM( F_Steam_kg[n], Name );
		sprintf_s ( Name, 128, "#Давление пилота(и)" );
		PARM( P_Pilot[n], Name );
		sprintf_s ( Name, 128, "#Поток пилота, м3" );
		PARM( F_Pilot_m3[n], Name );
		sprintf_s ( Name, 128, "#Давление воздуха(и)" );
		PARM( P_Air[n], Name );
		sprintf_s ( Name, 128, "#Поток воздуха, м3" );
		PARM( F_Air_m3[n], Name );
		sprintf_s ( Name, 128, "#Подсос воздуха, м3" );
		PARM( F_Air_podsos_m3[n], Name );
	ETAB
    }
  //for ( int n = 0; n < kFors; n++ )
  //  {
  //  sprintf_s ( Name, 128, "#%d - ShibGas", n + 1 );
  //  PARM( pFire[n]->Logic.pFors->ShibGas, Name );
  //  }
  //for ( int n = 0; n < kFors; n++ )
  //  {
  //  sprintf_s ( Name, 128, "#%d - VentGas", n + 1 );
  //  PARM( pFire[n]->Logic.pFors->VentGas, Name );
  //  }
  //for ( int n = 0; n < kFors; n++ )
  //  {
  //  sprintf_s ( Name, 128, "#%d - VentPilot", n + 1 );
  //  PARM( pFire[n]->Logic.pFors->VentPilot, Name );
  //  }
  //for ( int n = 0; n < kFors; n++ )
  //  {
  //  /////////////////
  //  sprintf_s ( Name, 128, "#%d - ShibMazut", n + 1 );
  //  PARM( pFire[n]->Logic.pFors->ShibMazut, Name );
  //  }
  //for ( int n = 0; n < kFors; n++ )
  //  {
  //  sprintf_s ( Name, 128, "#%d - VentMazut", n + 1 );
  //  PARM( pFire[n]->Logic.pFors->VentMazut, Name );
  //  }
  //for ( int n = 0; n < kFors; n++ )
  //  {
  //  sprintf_s ( Name, 128, "#%d - VentSteam", n + 1 );
  //  PARM( pFire[n]->Logic.pFors->VentSteam, Name );
  //  }
  //for ( int n = 0; n < kFors; n++ )
  //  {
  //  sprintf_s ( Name, 128, "#%d - VentSteam", n + 1 );
  //  PARM( pFire[n]->Logic.pFors->VentSteam, Name );
  //  }
  TAB("Горение",3)
  for ( int n = 0; n < kFors; n++ )
    {
    sprintf_s ( Name, 128, "#%d - Зажигание пилота", n + 1 );
    PARM( pFire[n]->Logic.SetFirePilot, Name );
    }
  for ( int n = 0; n < kFors; n++ )
    {
    sprintf_s ( Name, 128, "#%d - Зажигание основных", n + 1 );
    PARM( pFire[n]->Logic.SetFireMain, Name );
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
//	TAB("Точки", 2)
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
  Omega[N_SMOKE] = Omega_Smoke;
  Omega[N_AIR] = Omega_Air;
  Omega[N_GAS] = Omega_Topl_Gas;
  Omega[N_PILOT] = Omega_Pilot;
  Omega[N_MAZUT] = Omega_Mazut;
  Omega[N_STEAM] = Omega_Mazut_Steam;
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
