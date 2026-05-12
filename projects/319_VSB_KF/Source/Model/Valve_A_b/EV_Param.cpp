#include "stdafx.h"
#include "EV.h"
#include "Err.h"
#include "CommProc.h"


_W_EV::_W_EV()
{
	memset ( this, 0, sizeof (_W_EV));
}


int CEV::GetParams( char * )
{	
	#include "IO_Parms.h"
	#define GET_PARAMS
	#include "EV_Pnt.h"
	#undef GET_PARAMS
	return CValve_A_b::GetParams(NULL);
}

//#include "ShowParams.h"

int CEV::ShowParams( DWORD DataType, struct CShowData * pSD )
{
	int Res = CValve_A_b::ShowParams(DataType, pSD);
	if ( GetAsyncKeyState( VK_CONTROL ) & 0x8000 )
	{
		if(Position == 100.0)
			Task = 100.0;
		else if(Position == 0.0)
			Task = 0.0;
	}
	return Res;
}