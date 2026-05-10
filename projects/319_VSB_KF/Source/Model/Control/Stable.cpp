#include "stdafx.h"
#define DLL_Control
#include "Control.h"
#include "Err.h"
#include "SetData.h"
#include "Connect.h"




bool CStableControl::Work = false;
char CStableControl::Object[256];
double CStableControl::ProcNonStable = 10.;
double CStableControl::AbsNonStable = 0.;
int CStableControl::LevelStable = 10;

CStableControl::CStableControl( char * _ControlName, struct IBaseModel * _pObject, double * _pVar ) : IBaseControl ( _ControlName, _pObject )
  {
  On = true; 
  Start = false;
  CLEAR(Pnt)
  pVar = _pVar;
  kPnt = 0;
  ProcNonStable = 20.;
  AbsNonStable = 0;
  kNonStable = 0;
  kStable = 0;
  State = 0;
  }

void CStableControl::Test( )
  {
  if ( !Work )
    return;
  double V = *pVar;
  kPnt++;
  int K = kPnt;
  if ( K > MAX_PNT )
    K = MAX_PNT;
  int kMove = K - 1;
//  if ( kMove > 0 )
//    memmove( &Pnt[kMove], &Pnt[kMove-1], 8 * kMove );
  for ( int n = kMove; n > 0; n-- )
    Pnt[n] = Pnt[n-1];
  Pnt[0] = V;
  if ( K % 2 )
    return;
  double PP = 0.;
  double PM = 0.;
  bool Overflow = false;
  for ( int n = 0; n < K; n += 2 )
    {
    double P = Pnt[n];
    if ( P > 1e6 )
      Overflow = true;
    if ( P < -1e6 )
      Overflow = true;
    PP += P;
    PM += P;
    P = Pnt[n+1];
    if ( P > 1e6 )
      Overflow = true;
    if ( P < -1e6 )
      Overflow = true;
    PP += P;
    PM -= P;
    }
  PM = fabs ( PM ) / K ;
  PP /= K;
  if( PP != 0.0 )
	  Value = PM / PP * 100.;
  bool Stable = true;
  if ( Overflow )
    {
    Stable = false;
    sprintf_s ( Object, 256, "%s", pObject->ObjName.Str );
    }
  else
    {
    if ( AbsNonStable > 0. )
      {
      if ( PM > AbsNonStable )
        {
        Stable = false;
        sprintf_s ( Object, 256, "%s", pObject->ObjName.Str );
        }
      }   
    else
      {
      if ( PM > PP * ProcNonStable * 0.01 )
        {
        Stable = false;
        sprintf_s ( Object, 256, "%s", pObject->ObjName.Str );
        }
      }
    }
  if ( Stable )
    {
    kStable++;
    kNonStable = 0;
    }
  else
    {
    kNonStable++;
    kStable = 0;
    }
  int OldState = State;
  if ( kStable >= LevelStable )
    {
    State = 1;
    }
  else
    {
    if ( kNonStable >= LevelStable )
      State = -1;
    else
      State = 0;
    }
  if ( State == OldState )
    return;
  if ( !Start )
    {
    Start = true;
    if ( State >= 0 )
      return;
    }
  if ( State == 0 )
    return;
  NewEvent = true;
  }

int CStableControl::GetMsg( char Msg[256] )
  {
  NewEvent = false;
  if ( State < 0 )
    {
    sprintf_s( Msg, 256, "Нестабильноcть %3.1lf%%", Value );
    return 1;
    }
  if ( State > 0 )
    {
    sprintf_s( Msg, 256, "#Колебания %3.1lf%%", Value );
    return 0;
    }
  return -1;
  }

void CStableControl::GetParams( IBaseModel * pMain )
{
#define this pMain
#include "IO_Parms.h"
	//  char Txt[32];
	//  sprintf_s ( Txt, "%s", Decs );
	if ( !CParams::LastIsTab())
	{
		TAB(ControlName,3)
			PARM(Work,"Работает")
			//  PARM(Start,"Стартовал")
			PARM(State,"#Состояние")
			PARM(Object,"#Нестабильность в ...")

			//
			PARM(ProcNonStable,"Уровень нестабильности %")
			PARM(AbsNonStable,"Порог нестабильности")
			PARM(LevelStable,"Число шагов диагностики")
		ETAB
	}
	PARM(Work,"Работает")
	//  PARM(Start,"Стартовал")
	PARM(State,"#Состояние")
	PARM(Object,"#Нестабильность в ...")

	//
	PARM(ProcNonStable,"Уровень нестабильности %")
	PARM(AbsNonStable,"Порог нестабильности")
	PARM(LevelStable,"Число шагов диагностики")

}
#undef this

