#include "stdafx.h"
#include "Valve_M_b.h"
#ifdef LINUX
#include "WinDraw.h"
#endif

int CValve_M_b::GetParams( char * )
{	
#include "IO_Parms.h"
	Defect_Params(this);
	ACS_Params( this );
	TAB("Задания", 1 )
		CTRL_Params( NULL, this, $"Задания" );
		PARM( dReg, "Замедление регулировки" )
	ETAB	
	return CValve_b::GetParams(NULL);
}

int CValve_M_b::ShowParams( DWORD DataType, struct CShowData * pSD )
{
	if ( GetAsyncKeyState( VK_CONTROL ) & 0x8000 )
	{
		if(Position != 0.0)
			Valve = 0.0;
		else 
			Valve = 100.0;
	}
	return CValve_b::ShowParams(DataType, pSD);
}
