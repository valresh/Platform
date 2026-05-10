#include "stdafx.h"
#include "Y_Data.h"
#include "StdDef.h"

void CY_PID::PID_control( double dt )
	{
	SET_HBP
		{
		KKK();
		}
	if ( W->P == 0. )
		{
// Control Action Bypass
		double Ks = ( W->MSH - W->MSL ) / ( W->SH - W->SL );
    if( 0!=W->I )
		  dMV = Ks * ( dt / W->I * En );
    else
      dMV = Ks * ( dt * En );
		return;
		}
	switch ( W->Algorithm )
		{
		case 0:
			PID_auto( dt );
		break;
		case 1:
			PID_I_PD( dt );
		break;
		case 2:
			PID_PI_D( dt );
		break;
		case 3:
			PID_basic( dt );
		break;
		case 4:
			PID_auto_2( dt );
		break;
		}
	}

void CY_PID::PID_basic( double dt )
	{
	SET_HBP
		{
		KKK();
		}
	double Ks = ( W->MSH - W->MSL ) / ( W->SH - W->SL );
	double Kp = 100. / W->P;
	double ddEn = dEn - dEn1;
  double DDD = ddEn / dt;
  if( DDD < 3. && DDD > -3. )
    dMV = Kp * Ks * ( dEn + dt / W->I * En + W->D * DDD );
  else
  {
    dMV = Kp * Ks * ( dPVn + dt / W->I * En );
  }
	SET_HBP
		{
		KKK();
		}
	}

void CY_PID::PID_I_PD( double dt )
	{
	SET_HBP
		{
		KKK();
		}
	double Kp = 100. / W->P;
	double Ks = ( W->MSH - W->MSL ) / ( W->SH - W->SL );
	double ddPVn = dPVn - dPVn1;
  if( CY_Base::s_nCalcSteps < 10 )
	  dMV = Kp * Ks * ( dPVn + dt / W->I * En );
  else
    dMV = Kp * Ks * ( dPVn + dt / W->I * En + W->D / dt * ddPVn );
	SET_HBP
		{
		KKK();
		}
	}

void CY_PID::PID_PI_D( double dt )
	{
	SET_HBP
		{
		KKK();
		}
	double Kp;
	if ( W->Non_linear_Gain )
		Kp = NLG();
	else
		Kp = 100. / W->P;
	double Ks = ( W->MSH - W->MSL ) / ( W->SH - W->SL );
	double ddPVn = dPVn - dPVn1;
	dMV = Kp * Ks * ( dEn + dt / W->I * En + W->D / dt * ddPVn );
  double velDMV = ( W->MSH - W->MSL ) * 0.01;
  if( dMV > 0 && dMV > velDMV )
    dMV = velDMV;
  else if( dMV < 0 && dMV < -velDMV )
    dMV = -velDMV;
	SET_HBP
		{
		KKK();
		}
	}

void CY_PID::PID_auto( double dt )
	{
	if ( W->mode.M == SMode::CAS )
		PID_PI_D( dt );
	else
		PID_I_PD( dt );
	}

void CY_PID::PID_auto_2( double dt )
	{
	if ( W->mode.M == SMode::CAS )
		PID_PI_D( dt );
	else
		PID_I_PD( dt );
	}

