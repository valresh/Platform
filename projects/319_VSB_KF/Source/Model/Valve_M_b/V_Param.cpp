#include "stdafx.h"
#include "V.h"
#include "Err.h"
#include "CommProc.h"



int CV::GetParams( char * )
{	
	#include "IO_Parms.h"
	#define GET_PARAMS
	#include "V_Pnt.h"
	#undef GET_PARAMS
	return CValve_M_b::GetParams(NULL);
}
