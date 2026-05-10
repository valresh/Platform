#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"

static double aKnl[4] = { 1., 0.5, 0.25, 0.};

double CY_PID::NLG()
	{
	SET_HBP
		{
		KKK();
		}
	if ( W->Non_linear_Gain == 1 )
		{
		double Kp = 100. / W->P;
		double aEn = fabs ( En );
		if ( aEn > W->GW )
			return Kp;
    double Kpe = Kp;
    int nKnl = W->nKnl;
    if ( nKnl >= 1 && nKnl <= 3 )
      {
      double Knl = aKnl[nKnl];
		  Kpe *= ( 1. - ( 1. - Knl ) * W->GW / aEn );
      }
		return Kpe;
		}
// Squared Deviation Action
	double Kp = 100. / W->P;
	double aEn = fabs ( En );
	if ( aEn > W->GW )
		return Kp;
	double Kpe = Kp * aEn / W->GW;
	SET_HBP
		{
		KKK();
		}
	return Kpe; 
	}
