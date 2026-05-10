#include "stdafx.h"
#define DLL_Control
#include "Control.h"
#include "Err.h"
#include "SetData.h"
#include "Connect.h"
#include "Resume.h"


CAnalogControl::CAnalogControl( char * _ControlName, struct IBaseModel * _pObject, double * _pVal, double * _pNominal, double * _pDelta ) : 
IBaseControl ( _ControlName, _pObject )
  {
  On = true;
  State = 2; 
  pVal = _pVal;
  pNominal = _pNominal;
  pDelta = _pDelta;
  }

void CAnalogControl::Test( )
  {
  if ( !On || !Use )
    return;
  double Min = *pNominal - *pDelta;
  double Max = *pNominal + *pDelta;
  double Eps = 0.1 * *pDelta;
  double V = *pVal;
  int New = State;
  if ( State == 0 )
    {
    if ( V < Min )
      New  = -1;
    if ( V > Max )
      New  = 1;
    }
  if ( State < 0 )
    {
    if ( V > Min + Eps )
      New  = 0;
    }
  if ( State > 0 )
    {
    if ( V < Max - Eps )
      New  = 0;
    }
  //
  int OldState = State;
  State = New; 
  if ( State == OldState )
    return;
  NewEvent = true;
  if ( State < 0 )
    Record ( "%s.%s = %5.1lf < %5.1lf", pObject->ObjName.Str, ControlName, V, Min );
  if ( State > 0 )
    Record( "%s.%s = %5.1lf > %5.1lf", pObject->ObjName.Str, ControlName, V, Max );
  if ( State == 0 )
    Record( "%s.%s = %5.1lf в заданных пределах", pObject->ObjName.Str, ControlName, V );
  }

int CAnalogControl::GetMsg( char Msg[256] )
  {
  // Сообщение в журнал
  NewEvent = false;
  double V = *pVal;
  if ( State < 0 )
    {
    sprintf_s( Msg, 256, "%s = %5.1lf < %5.1lf", ControlName, V, Min );
    return 1;
    }
  if ( State > 0 )
    {
    sprintf_s( Msg, 256, "%s = %5.1lf > %5.1lf", ControlName, V, Max );
    return 1;
    }
  if ( State == 0 )
    {
    sprintf_s( Msg, 256, "%s = %5.1lf в заданных пределах", ControlName, V );
    return 0;
    }
  return -1;
  }

void CAnalogControl::GetParams( IBaseModel * pMain )
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

