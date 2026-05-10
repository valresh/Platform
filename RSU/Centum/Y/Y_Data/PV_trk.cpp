#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"


void CY_PID::Process_variable_tracking( )
	{
	if ( W->mode.M == SMode::MAN && W->Trk_MAN )
		{
		W->SV = W->PV;
		return ;
		}
	if ( W->mode.M == SMode::AUT && W->Trk_AUT_CND && W->mode.IMAN )
		{
		W->SV = W->PV;
		return ;
		}
	if ( W->mode.M == SMode::CAS && W->Trk_CAS_CND && W->mode.IMAN )
		{
		W->SV = W->PV;
		return ;
		}
	}
