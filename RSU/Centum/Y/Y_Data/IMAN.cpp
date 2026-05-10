#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"


void CY_PID::Initialization_manual( )
	{
	if ( ( OUT.Status & CSignal::CND ))
		{
		W->mode.IMAN = true;
// Track output
		}
	else
		{
		W->mode.IMAN = false;
		}
	}

