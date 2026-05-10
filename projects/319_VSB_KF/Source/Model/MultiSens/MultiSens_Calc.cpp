#include "stdafx.h"
#include "MultiSens.h"
#include "Err.h"
#include "CommProc.h"


void CMultiSens::Calc( double dt )
	{
  SET_BP BreakPoint;
  if ( pMotor )
    {
    On = *pMotor->pOn;
    Oborot = *pMotor->pRelOborot;
    }
  if ( kVibroSens > 0 )
    Vibration( dt );
  if ( kShiftSens > 0 )
    Shift( dt );
  if ( On )
    {
    for ( int n = 0; n < kTSens; n++ )
      {
	  if(n == kDefectSens)
		  continue;
      double T = *pT[n];
      T = 0.95 * T + 0.05 * rndTSens[n];
      *pT[n] = T;
      }
    }
  else
    {
    for ( int n = 0; n < kTSens; n++ )
      {
	  if(n == kDefectSens)
		  continue;
      double T = *pT[n];
      T = 0.95 * T + 0.05 * T_air;
      *pT[n] = T;
      }
    }
	}

void CMultiSens::Vibration( double dt )
  {
  SET_BP BreakPoint;
  if(eDefect == DEFECT_VIBRO)
	  return;
  double Ksi = dt / Tau;
  double Old = 1. / ( 1. + Ksi );
  double New = Ksi * Old;
  double _O = Oborot / V_Krit;
  double _O_1 = _O - 1;
  double V;
  double Delta2 = 3.;
  if ( _O < 1. )
    V = _O * Delta2 / ( Delta2 + _O_1 * _O_1 );
  else
    V = Delta2 / ( Delta2 + _O_1 * _O_1 );
  for ( int n = 0; n < kVibroSens; n++ )
    {
    double Vib = V * rndVibrSens[n] * Max_Vibration;
    *(pVibro[n]) = Vib;
    //if ( r01() > 0.5 )
    //  rndVibrSens[n] = Old * rndVibrSens[n] + New * Max_Vibration;
    //else
    //  rndVibrSens[n] = Old * rndVibrSens[n] + 0.5 * New * Max_Vibration;
    //if ( rndVibrSens[n] > Max_Vibration )
    //  rndVibrSens[n] = Max_Vibration;
    //if ( rndVibrSens[n] < -Max_Vibration )
    //  rndVibrSens[n] = -Max_Vibration;
    }
  }

void CMultiSens::Shift( double dt )
  {
  if(eDefect == DEFECT_SHIFT)
	  return;
  double Ksi = dt / Tau;
  double Old = 1. / ( 1. + Ksi );
  double New = Ksi * Old;
  double O = Oborot;
  if ( O < 0.1 )
    O = 0.1;
  dt *= O;
  for ( int n = 0; n < kShiftSens; n++ )
    {
    *(pShift[n]) = rndShftSens_Curr[n];
    if ( rndShftSens_Curr[n] <= rndShftSens_Lim[n] )
      {
      if ( Oborot > 0. )
        rndShftSens_Curr[n] += rndShftSens_Vel[n] * dt;
      if ( rndShftSens_Curr[n] > rndShftSens_Lim[n] )
        {
        rndShftSens_Curr[n] = rndShftSens_Lim[n];
        rndShftSens_Lim[n] = ( 1. - 2. * r01()) * Max_Shift;
        double dT = ( 0.2 + 0.8 * r01()) * Tau; 
        double dV = fabs ( rndShftSens_Curr[n] - rndShftSens_Lim[n] );
        rndShftSens_Vel[n] = dV / dT;
        }
      }
    if ( rndShftSens_Curr[n] >= rndShftSens_Lim[n] )
      {
      if ( Oborot > 0. )
        rndShftSens_Curr[n] -= rndShftSens_Vel[n] * dt;
      if ( rndShftSens_Curr[n] < rndShftSens_Lim[n] )
        {
        rndShftSens_Curr[n] = rndShftSens_Lim[n];
        rndShftSens_Lim[n] = ( 1. - 2. * r01()) * Max_Shift;
        double dT = ( 0.2 + 0.8 * r01()) * Tau; 
        double dV = fabs ( rndShftSens_Curr[n] - rndShftSens_Lim[n] );
        rndShftSens_Vel[n] = dV / dT;
        }
      }
/*
    if ( r01() > 0.5 )
      rndShftSens[n] = Old * rndShftSens[n] + New * Max_Shift;
    else
      rndShftSens[n] = Old * rndShftSens[n] - New * Max_Shift;
    if ( rndShftSens[n] > Max_Shift )
      rndShftSens[n] = Max_Shift;
    if ( rndShftSens[n] < -Max_Shift )
      rndShftSens[n] = -Max_Shift;
*/
    }
  }
