#include "stdafx.h"
#include "Y_Data.h"
#include "StdDef.h"


void CY_PID::Deadband_action( )
	{
	double dHYS = W->HYS_DB * ( W->SH	- W->SL );
	double Emax, Emin;
  double DB = W->DB;
	if ( Deadband_state > 0 )
		{
		Emax = DB - dHYS;
		Emin = -DB;
		}
	if ( Deadband_state == 0 )
		{
		Emax = DB;
		Emin = -DB;
		}
	if ( Deadband_state < 0 )
		{
		Emax = DB;
		Emin = -DB + dHYS;
		}
	if ( En <= Emax && En >= Emin )
		{
		Deadband_state = 0;
		}
	else
		{
		if ( En > Emax )
			Deadband_state = 1;
		else
			Deadband_state = -1;
		}
//	if ( Deadband_state == 0 )
//		dMV = 0.;
	}
