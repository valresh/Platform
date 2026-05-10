#include "stdafx.h"
#include "Valve_R.h"
#include "Err.h"
#include "CommProc.h"


void CValve_R::Calc( double dt )
	{
  SET_BP BreakPoint;
  if ( dP >= 0. )
    Forvard = true;
  else
    Forvard = false;
  if ( DefOpen )
    Forvard = true;
//  if ( Forvard )
//    Перепад.CalcCoef( pFlow_In );
	}
