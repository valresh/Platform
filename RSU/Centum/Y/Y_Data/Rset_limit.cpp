#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"

void CY_PID::Reset_limit_function( double dt )
	{
  return;
  if ( OUT.BackVar == 0. )
    return ;
	if ( RL1 )
		*RL1 >> RLV1;
	else
    {
	  RLV1 = OUT.BackVar;
    }
	if ( RL2 )
		*RL2 >> RLV2;
	else
		RLV2 = 0.;
	dMV = dMV + dt / W->I * ( RLV1 - RLV2 - W->MV );
	}

