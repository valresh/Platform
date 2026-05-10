#include "stdafx.h"
#define DLL_Control
#include "Control.h"
#include "Err.h"
#include "SetData.h"
#include "Connect.h"




CLevelControl::CLevelControl( char * _ControlName, struct IBaseModel * _pObject, double * _pLev ) : IBaseControl ( _ControlName, _pObject )
  {
  On = true;
  pLev = _pLev;
  Min = 10.;
  Max = 90.;
  OldState = State = 2; 
  }

void CLevelControl::Test( )
  {
  if ( !On || !Use )
    {
    OldState = State = 2;
    return;
    }
  double L = *pLev * 100.;
  int New = 0;
  if ( L < Min )
    New  = -1;
  else
    {
    if ( L > Max )
      New  = 1;
    }
//
  if ( State == 2 && New < 0 )
    {
    OldState = State;
    return; // Еще не заливали
    }
  if ( State == 2 && New == 0 )
    OldState = State = New; 
  if ( State == 2 && New > 0 )
    State = New; 
  if ( OldState == 2 && New == 0 )
    OldState = State = New; 
  if ( State == OldState )
    return;
  OldState = State;
  NewEvent = true;
  }

int CLevelControl::GetMsg( char Msg[256] )
  {
  // Сообщение в журнал
  NewEvent = false;
  double L = *pLev * 100.;
  if ( State < 0 )
    {
    sprintf_s( Msg, 256, "Низкий уровень %5.1lf", L );
    return 1;
    }
  if ( State > 0 )
    {
    sprintf_s( Msg, 256, "Высокий уровень %5.1lf", L );
    return 1;
    }
  if ( State == 0 )
    {
    sprintf_s( Msg, 256, "#Уровень %5.1lf", L );
    return 0;
    }
  return -1;
  }

void CLevelControl::GetParams( IBaseModel * pMain )
  {
#define this pMain
#include "IO_Parms.h"
//  char Txt[32];
//  sprintf_s ( Txt, "%s", ControlName );
  if ( !CParams::LastIsTab())
    {
    TAB(ControlName,3)
		PARM(On,"Работает")
		PARM(State,"#Состояние")
		//
		PARM(Min,"Минимальный уровень")
		PARM(Max,"Максимальный уровень")
	ETAB
    }
  PARM(On,"Работает")
  PARM(State,"#Состояние")
//
  PARM(Min,"Минимальный уровень")
  PARM(Max,"Максимальный уровень")
  }
#undef this

void CLevelControl::SaveState( )
  {
  S_CLASS(ControlName,CLevelControl_W)
  }

int CLevelControl::RestoreState( char * StrName )
  {
	  R_CLASS(ControlName,CLevelControl_W)
	  return 1;
  }
