#include "stdafx.h"
#include "SepLiq.h"
#include "Err.h"
//#include "CommProc.h"
//#include "Data.h"
#include "SetData.h"
//#include "CommData.h"

LPCSTR g_szHelpFileName = "Базовый сепаратор.pdf";

//CListModelMsg Messages;

CSepLiq::CSepLiq( char * _ObjName ) :	IBaseModel ( _ObjName ) 
#include "ACS_Constr.h"
#include "ACS.h"
, Heat ( "Нагреватель", this )
//, LevCtrl ( "уровень", this, &Level )   
	{
  UNIMODEL
	ObjName = _ObjName;
	#include "IO_Clear.h"
	#include "Points.h"
	#include "SepLiq_Defects.h"
  kStep = 0;
  FlowQ = 0.;
  Level_Z = 0.;
  pPW = NULL;
  PseudoWater = false;
  CLEAR(X_mass)
  CLEAR(Y_mass)
  CLEAR(X_mol)
  CLEAR(Y_mol)
  Use_mass = true;
  Use_mol = true;
  NoInLiq_N2 = false;
  Q_heat = 0.;
  VolumeZ = 0.;
  Разгерметизация = false;
  Cp_gas = 10.;
  Cp_liq = 10.;
	}

CSepLiq::~CSepLiq()
	{
	}

int CSepLiq::Init( int Regim )
	{
	int Res = 0;
	//#include "IO_Init.h"
	//#include "Points.h"
	//#include "ACS_Init.h"  
	//#include "ACS.h"
  NewComp( &ID_Comp_Out[0], &pComp_Out[0], "Выход 0  %s", ObjName.Str );
  NewComp( &ID_Comp_Out[1], &pComp_Out[1], "Выход 1  %s", ObjName.Str );
  NewComp( &ID_Comp_Out[2], &pComp_Out[2], "Выход 2  %s", ObjName.Str  );
  pComp_Out[0]->Set ( pN2 );
  pComp_Out[1]->Set ( pN2 );
  pComp_Out[2]->Set ( pWater );
  Char<128>Name;
  for ( int n = 0; n < 3; n++ )
    {
    Name.Prt ( "Вход %d %s", n + 1, ObjName.Str );
    State[n] = new CSEps ( Name, this );
    }
  P = 1.;
  T_gas = T_liq = T_air;
  M_Liq = Level = 0.;
  M_Gas = P * Volume / ( RP * ( T_gas + TK ));
  h_gas_mix = pN2Flow->h_Flow;
  h_Gas[G_N2] = h_gas_mix;
  h_liq_mix = 0.;
  memmove ( IsComp, pComp_Out[0]->IsComp, sizeof ( IsComp ));
  memmove ( Y, pComp_Out[0]->Cmol, sizeof ( Y ));
  memmove ( X, pComp_Out[0]->Cmol, sizeof ( X ));
//  ASS(Ctrl.AddLevel( &Level, &Level_Z ));
//  if ( PseudoWater )
//    pPW = new CPseudo_Water ( this );
  for ( int c = 0; c < LAST_GAS; c++ )
    Henry[c] = 1.;
  Heat.Init();
	return 0;
	}

int CSepLiq::Step0( )
	{
//  Fsend ( "Опасные объекты.csv", "%s;Да;\n", ObjName );
	return 0;
	}

int CSepLiq::Step1( )
	{
  if ( Level_Z > 0. )
    {
    Reg_L.Set( Level_Z );
    }
//  M_Liq = Level = 0.;
  VolumeZ = Volume;
  if ( !_finite ( T_gas ))
    T_gas = 15.;
  if ( !_finite ( T_liq ))
    T_liq = 15.;
  return 0;
	}

int CSepLiq::StepT ( double dt )
	{
  Calc ( dt );
//  LevCtrl.Test( );
  kStep++;
	return 0;
	}

/*
CModelMenu Menu[] = {
  "Уровень 50%",   false, // 0
  "Зафиксировать 50%",   false, // 0
  NULL, false,
  };
*/
#define LEV_50      0
#define LEV_50Z     1


int CSepLiq::SetData( int TypeData, void * pData )
	{
  if ( TypeData == sd_GetOverGasSource )
    {
    *(double**)pData = &FlowQ; 
    return sd_GetOverGasSource;
    }
#undef IF
#define IF(Parm) if ( strcmp ( pP->Name, Parm ) == 0 ) {
#undef EIF
#define EIF return 0; }
  switch ( TypeData )
    {
    case sd_SetEquipParam:
      {
      CEquipParam * pP = (CEquipParam*)pData;
      IF("#Заданный объем")
        if ( pP->Value[0] == 0 )
          return 0;
        Volume = atof( pP->Value );
        if ( Volume < 1. )
          Volume = 1.;
      EIF
      }
	case sd_GetHelpPath:
		{
			lstrcpy((char*)pData, HELP_PATH);
			lstrcat((char*)pData, g_szHelpFileName);
			return 1;
		}
    }
	return 1;
	}

int CSepLiq::SaveState( )
  {
	S_CLASS("W",_W)
//  LevCtrl.SaveState( );
  //if ( pPW )
  //  {
  //  pPW->SaveState( );
  //  }
  return 0;
  }

int CSepLiq::RestoreState( char * StrName )
  {
  COPY_FIRST
	R_CLASS("W",_W)
  //if ( LevCtrl.RestoreState( StrName ) == 0 )
  //  return 0;
  //if ( pPW )
  //  {
  //  if ( pPW->RestoreState( StrName ) == 0 )
  //    return 0;
  //  }
  return 1;
  }
