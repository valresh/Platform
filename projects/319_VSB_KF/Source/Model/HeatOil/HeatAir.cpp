#include "stdafx.h"
#define DLL_HeatOil
#include "heatoil.h"
#include "math.h"
#include "Err.h"
#include "CommProc.h"
#include "DataTypes.h"


CHeatAir::CHeatAir( char * Name, IBaseModel * pMain ) : IBaseModel ( Name, pMain )
{
#include "IO_Clear.h"
	Model = "Воздушное охлаждение";
	pT_corp = NULL;
	kSens = 0;
	Tau_cool = 10.;
	T_room = 25.;
	T_rand_min = 50.;
	T_rand_max = 60.;
	T_max = 150.;
	DefectNode = "Охлаждение";
	DefectName = "Перегрев";
	T_Fire = 0.;
	Set_T_airZ = false;
	kdT_def = 0;
}

CHeatAir_W::CHeatAir_W()
	{
	memset ( this, 0, sizeof ( CHeatAir_W ));
	}

int CHeatAir::GetParams( char * )
	{
	char Name[65];
	#include "IO_Parms.h"
	int K = 0;
	if ( kSens > 0 )
		K = kSens;
  TAB("Параметры",1)		
	PARM( Oborot, "#Обороты" )
	PARM( T_rand_min, "Случ. мин" )
	PARM( T_rand_max, "Случ. макс" )
	PARM( Tau_cool, "Время охлаждения, мин" )
	PARM( T_room, "T в корпусе" )
  PARM( T_max, "Максимальная T" )
  ETAB
	
	TAB("Тz",6)		
	for ( int n = 0; n < K; n++ )
		{
		sprintf_s ( Name, 64, "#%d) заданные температуры", n + 1 );
		PARM( T_airZ[n], Name );
		}
	PARM_B( Use_T_airZ, "Использовать Tz", false );
	PARM_B( Set_T_airZ, "Задать Tz", false );
	ETAB
	return 0;
	}

int CHeatAir::SaveState( )
  {
	if ( kSens == 0 )
		return 0;
	S_CLASS("W",CHeatAir_W)
  return 0;
  }

int CHeatAir::RestoreState( char * StrName )
  {
  IGNORE
	R_CLASS("W",CHeatAir_W)
  return 1;
  }

int CHeatAir::UpdateParam( CParams & Param )
	{
	return 0;
	}

int CHeatAir::Init( char * Fmt, int _kSens )
	{
  kSens = _kSens;
  if ( kSens == 0 )
    return 0;
  pT_corp = NewArr(CAnalogD*, kSens );
  k_def = NewArr(double, kSens );
  Char<32> Name;
  for ( int n = 0; n < kSens; n++ )
	  {
    Name.Prt( Fmt, n + 1 );
    pT_corp[n] = new CAnalogD( pMain, Name, "AO" );
	  k_def[n] = 1.;
	  }
//////////////
  if ( DefectNode == NULL )
    return 0;
	DWORD Root = pMain->RootNode;
	if ( DefectNode )
		{
		Root = NewDefect( );
		CDef * pNode = GetDefect( Root );
		pNode->Parent = pMain->RootNode;
		lstrcpy ( pNode->Name, DefectNode );
		lstrcpy ( pNode->ObjName, pMain->ObjName );
		pNode->Flags = CDef::Defects|CDef::Node|CDef::Active;
		pNode->pModel = pMain;
		}
	DWORD LastDefect = NewDefect( );
	Перегрев = GetDefect( LastDefect );
	Перегрев->Parent = Root;
	lstrcpy ( Перегрев->Name, DefectName );
	lstrcpy ( Перегрев->ObjName, pMain->ObjName );
	Перегрев->Flags = CDef::Defects|CDef::Defect|CDef::Active;
  Перегрев->pModel = pMain;
//////////////
	return 0;
	}

int CHeatAir::StepT( double dt )
	{
  SET_BP BreakPoint;
  Use_T_airZ = false;
  double Ta;
	if ( T_room )
		Ta = T_room;
	else
		Ta = T_air;
	for ( int n = 0; n < kSens; n++ )
		{
		if ( T_airZ[n] == 0. || Set_T_airZ )
			{
			T_airZ[n] = T_rand_min + r01() * ( T_rand_max - T_rand_min );
			}
		}
  Set_T_airZ = false;
///////////////////////////////////////////////////////////////
  double Ksi = dt * 60. / Tau_cool;
  double Old = 1. / ( 1. + Ksi );
  double New = Ksi * Old;
	for ( int n = 0; n < kSens; n++ )
		{
		double T; 
		double W = Oborot * k_def[n]; 
    double T_lim = W * ( T_airZ[n] - T_room ) + T_room; 
		if ( Use_T_airZ )
			{
			T = T_airZ[n];
			}
		else
			{
			T = Old * T_corp[n] + New * T_lim;
			}
  	if ( T > T_max )
		  T = T_max;
		T_corp[n] = T;
    if ( pT_corp[n] )
      *pT_corp[n] = T + T_Fire;
		}
	return 0;
	}

int CHeatAir::OnDefect( struct CDef * pDefect )
	{
	Use_T_airZ = false;
  for ( int n = 0; n < kSens; n++ )
    {
    if ( k_def[n] != 1. )
      continue;
    if ( !pT_corp[n]->Use())
      continue;
    k_def[n] = Нагрев->Dbl * 2.;
    Tau_cool *= 0.25;
    kdT_def++;
    break;
    }
	return 1;
  }
