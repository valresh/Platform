#include "stdafx.h"
#define DLL_Control
#include "Control.h"
#include "Err.h"
#include "SetData.h"
#include "Connect.h"


CDiscreteControl::CDiscreteControl( char * _ControlName, struct IBaseModel * _pObject, bool * _pTest, bool _Norma ) : IBaseControl ( _ControlName, _pObject )
  {
  On = true;
  State = 2; 
  Norma = _Norma;
  pTest = _pTest;
  }

void CDiscreteControl::Test( )
  {
  if ( !On || !Use )
    return;
  if ( *pTest == Norma )
    {
    if ( State == 1 )
      NewEvent = true;
    State = 0;
    return;
    }
  //
  if ( State == 1 )
    return; // Уже известно
  State = 1;
  NewEvent = true;
  }

int CDiscreteControl::GetMsg( char Msg[256] )
  {
  // Сообщение в журнал
  NewEvent = false;
  if ( State )
    {
    sprintf_s( Msg, 256, "%s : ДА", ControlName );
    return 1;
    }
  sprintf_s( Msg, 256, "%s : нет", ControlName );
  return 0;
  }

void CDiscreteControl::GetParams( IBaseModel * pMain )
  {
#define this pMain
#include "IO_Parms.h"
  if ( !CParams::LastIsTab())
    {
    TAB(ControlName,3)
		PARM(On,"Работает")
		PARM(State,"#Состояние")
	ETAB
    }
  PARM(On,"Работает")
  PARM(State,"#Состояние")
  }
#undef this

