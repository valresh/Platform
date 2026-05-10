#include "stdafx.h"
#include "CoolObj.h"
#include "Err.h"
#include "CommProc.h"


void CCoolObj::Calc( double dt )
	{
  SET_BP BreakPoint;
  _kSens = kSens;
  if( pFlow_in == NULL|| pFlow_in->ID_COMP == 0)
    return;
  if(pMotor)
  {
	  On = *pMotor->pOn;
	  Oborot = *pMotor->pRelOborot;
	  Connected = true;
  }
  else  
	  Connected = false;
  In.PH( pFlow_in );
  T_in = In.T;
  if ( New )
    {
    F_kg = pFlow_in->Flow_kg();
    double kT = 1.;
    if ( Set_Tz )
      {
      F_nom = F_kg;
      T_in_nom = T_in;
      }
    else
      {
      if ( F_nom <= 0. )
        F_nom = F_kg;
      if ( F_kg < 0.1 * F_nom )
        kT = 10.;
      else
        {
        if ( F_kg > 10. * F_nom )
          kT = 0.1;
        else
          kT = F_nom / F_kg;
        }
      }
    if ( Set_Oborot )
      {
      if ( Set_Oborot < 0 )
        kT = 0.;
      }
    else
      kT *= Oborot;
    double Ts = 0.;
    for ( int n = 0; n < kSens; n++ )
      {
      double dT = Tz[n] - T_in_nom;
      if ( Cp < 1. )
        Cp = 1.;
      double Ksi = 10. / Cp;
      double To = T_in + dT * kT;
      if ( To > Тm )
        To = Тm;
      if ( !_finite ( Tout[n] ))
        Tout[n] = Tz[n]; 
      Tout[n] = ( Tout[n] + Ksi * To ) / ( 1. + Ksi );
      Ts += Tout[n];
      }
    T = Ts / kSens;
    *pFlow_out = *pFlow_in;
    Out.PT( pFlow_out->P, T, pFlow_out->Get_Comp() );
    pFlow_out->h_Flow = Out.h;
    for ( int n = 0; n < kSens; n++ )
      {
      *pSens[n] = Tout[n];
      }
    return;
    }
  if ( Set_Norm )
    {
    T = 0.5 * ( T_min + T_max );
    if ( T < In.T || !_finite ( T ))
      T = In.T;
    Out.PT( pFlow_in->P, T, pFlow_in->Get_Comp() );
    *pFlow_out = *pFlow_in;
    pFlow_out->h_Flow = Out.h;
    double dH = fabs ( pFlow_in->Flow_mol ) * ( Out.h - In.h );
    W = dH;
    double dT_out = T - In.T;
    for ( int n = 0; n < kSens; n++ )
      {
      double dT = Tz[n] - In.T;
      if ( dT < 0. )
        dT = 0.;
      if ( dT_out > 1. )
        kTz[n] = dT / dT_out;
      Tout[n] = Tz[n];
      }
    }
  else
    {
    if ( !_finite ( T ))
      T = In.T;
    Out.PT( pFlow_in->P, T, pFlow_in->Get_Comp() );
    *pFlow_out = *pFlow_in;
    pFlow_out->h_Flow = Out.h;
    double dH = fabs ( pFlow_in->Flow_mol ) * ( Out.h - In.h ) - W * Oborot;
    double dT = LimdX ( -dH / Cp * dt, 3. );
    T += dT;
    if ( T < In.T )
      T = In.T;
    if ( T > Тm )
      T = Тm;
    double dT_out = T - In.T;
    for ( int n = 0; n < kSens; n++ )
      {
      if ( Defect && n == nSens )
        Tout[n] = (In.T + kTz[n] * dT_out) * ( 1. + Pow );
      else
        Tout[n] = In.T + kTz[n] * dT_out;
      }
    }
  for ( int n = 0; n < kSens; n++ )
    {
    *pSens[n] = Tout[n];
    }
	}
