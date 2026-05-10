#include "stdafx.h"
#include "Furnace.h"
#include "Err.h"
//#include "EventLogTypes.h"
#include "SysDataTypes.h"

//const double MIN_GAS_AIR = 1.;
//const double MIN_MAZUT_AIR = 10.;
//const double MIN_WORK_GAS = 0.1;
//const double MIN_WORK_MAZUT = 0.01;
//const double MIN_ZAPAL = 0.02;
//const double VentMin = 0.001;

const double kShib_0 = 0.5;
const double Shib_1 = 80.;
const double Norm_Shib = ( 1. - kShib_0 ) / Shib_1;
#define TIME_OPEN  60. //сек

CForsLogic::CForsLogic( )
  {
//	pFors = NULL;
	CLEAR(Name)
	IsFire = true;
	IsZapal = true;
  MIN_GAS_AIR = 1.;
  MIN_MAZUT_AIR = 10.;
  MIN_WORK_GAS = 0.1;
  MIN_WORK_MAZUT = 0.01;
  MIN_ZAPAL = 0.02;
  SetFirePilot = false;
  SetFireMain = false;
  }

void CForsLogic::Init ( char * _Name )
  {
  ASS(lstrlen ( _Name ) < 32 );
  lstrcpy ( Name, _Name );
  TimerWork = 0;
  pFors -> isFire = true;
  pFors -> canFire = true;
  if ( pSys ->bInit_0 )
    {
    pFors -> VentGas = VentMin;
    pFors -> VentPilot = 0.;
    pFors -> VentMazut = VentMin;
    pFors -> VentSteam = VentMin;
    pFors -> ShibGas = 0.;
    pFors -> State_Flags = 0; 
    }
  else
    {
    pFors -> VentGas = 100.;
    pFors -> VentPilot = 100.;
    pFors -> VentMazut = VentMin;
    pFors -> VentSteam = VentMin;
    pFors -> ShibGas = 80.;
    pFors->On_State ( CForsunka::IS_ZAPAL_GAS|CForsunka::IS_ZAPAL_FIRE|CForsunka::IS_WORK_GAS|CForsunka::IS_WORK_FIRE );
    }
  }

void CForsLogic::Work (double Flow_Fors_Air, 
                       double Flow_Fors_Gas, 
                       double Flow_Fors_Zapal,
                       double Flow_Fors_Mazut, 
                       double Flow_Fors_Steam,
                       bool NoZapal )
  {
  // IsZapal, IS_ZAPAL_FIRE - горит ли запал
  // IS_ZAPAL_GAS - есть газ на пилот
  // pFors -> isFire  = true;	если есть огонь
  if ( NoZapal )
    {
    IsZapal = false;
    pFors-> Off_State ( CForsunka::IS_ZAPAL_GAS | CForsunka::IS_ZAPAL_FIRE );
    }
  if ( Flow_Fors_Zapal > MIN_ZAPAL || NoZapal )
    {
    pFors->On_State ( CForsunka::IS_ZAPAL_GAS );
    pFors -> canFire = true;
#ifdef AUTO_FIRE
    pFors->VentPilot = 100.;
    pFors ->setFire = true;
#endif
    
//    if ( pFors ->setFire ) // Поджиг запальника
    if ( SetFirePilot ) // Поджиг запальника
      {
      SetFirePilot = false;
      pFors -> setFire = false;
      //			pFors -> isFire  = true;
      IsZapal = true;
      pFors -> On_State ( CForsunka::IS_ZAPAL_FIRE );
      }
    if ( pFors->Is_State ( CForsunka::IS_WORK_FIRE ))
      {
      IsZapal = true;
      pFors -> On_State ( CForsunka::IS_ZAPAL_FIRE );
      }
    }
  else
    {
    IsZapal = false;
    pFors -> canFire = false;
    pFors-> Off_State ( CForsunka::IS_ZAPAL_GAS | CForsunka::IS_ZAPAL_FIRE );
    }
  //Подача///////////////////////////////////////////////////////
  // Форсунка
  bool GasFire;
  if ( Flow_Fors_Gas < MIN_WORK_GAS )   //pFors -> VentGas == 0. || 
    {
    GasFire = false;
    TimerWork = 0;
    pFors->Off_State ( CForsunka::IS_WORK_GAS );
    }
  else
    {
    if ( Flow_Fors_Air > MIN_GAS_AIR * Flow_Fors_Gas )
      {
      GasFire = true;
      pFors->On_State ( CForsunka::IS_WORK_GAS );
      }
    else
      {
      GasFire = false;
      TimerWork = 0;
      pFors->Off_State ( CForsunka::IS_WORK_GAS );
      }
    }
//
  bool MazutFire;
  if ( Flow_Fors_Mazut < MIN_WORK_MAZUT ) //pFors -> VentMazut == 0. || 
    {
    MazutFire = false;
    pFors->Off_State ( CForsunka::IS_WORK_MAZUT );
    }
  else
    {
    if ( Flow_Fors_Air > MIN_MAZUT_AIR * Flow_Fors_Mazut )
      {
      MazutFire = true;
      pFors->On_State ( CForsunka::IS_WORK_MAZUT );
      }
    else
      {
      MazutFire = false;
      pFors->Off_State ( CForsunka::IS_WORK_MAZUT );
      }
    }
//
  // IsFire есть ли горение
  if ( !GasFire && !MazutFire )
    {
    // Не подан ни газ ни мазут
    pFors->isFire = false;
    pFors->Off_State ( CForsunka::IS_WORK_FIRE );
    }
//
  if ( !IsZapal && !GasFire && !MazutFire )
    {
    // Не подан ни газ ни мазут ни запал
    pFors -> dKPD = 0.0;
    IsFire = false;
    pFors -> isFire = IsFire;
    pFors->Off_State ( CForsunka::IS_WORK_FIRE );
    return;
    }
  //Горение///////////////////////////////////////////////////////
#ifdef AUTO_FIRE
  pFors->On_State ( IS_WORK_FIRE );
#endif
  if ( ( GasFire || MazutFire ) && ( pFors -> Is_State ( CForsunka::IS_ZAPAL_FIRE ) || SetFireMain ))
    {
    SetFireMain = false;
    pFors->On_State ( CForsunka::IS_WORK_FIRE );
    }
  if ( pFors->Is_State ( CForsunka::IS_WORK_FIRE ))
    {
    IsFire = true;
    TimerWork = 0;
    pFors -> isFire = true;
    return;
    }
  if ( pFors -> Is_State ( CForsunka::IS_ZAPAL_FIRE ))
    {
    IsFire = true;
    return;
    }
  IsFire = false;
  pFors -> isFire = false;
  }

