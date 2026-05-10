#include "stdafx.h"
#include "Valve_M_b.h"
#include "SetDataTypes.h"

static LPCSTR g_szHelpFileName = "Базовый вентиль.pdf";

_W_Valve_M_b::_W_Valve_M_b()
{
	memset(this, 0, sizeof(*this));
}

CValve_M_b::CValve_M_b( char * _ObjName, char* _Type ) :	CValve_b ( _ObjName, _Type ) 
#include "ACS_Constr.h"
#include "Valve_M_b_ACS.h"
{
#include "IO_Clear.h"
#include "Valve_M_b_Pnt.h"
	Performance = Линейная_Характеристика;
	Valve_old = Valve;
}

CValve_M_b::~CValve_M_b(void)
{
}

int CValve_M_b::Step1( )
{
	if(Valve != Position)
		Valve = Position;
	return CValve_b::Step1();
}

int CValve_M_b::SetData( int TypeData, void * pData )
{
	switch ( TypeData )
	{
	case sd_SetInitState:
		Valve = 0.0;
		break;
	}
	return CValve_b::SetData(TypeData, pData);
}

int CValve_M_b::SaveState( )
{
	S_CLASS("_W_Valve_M_b", _W_Valve_M_b);
	return CValve_b::SaveState();
}

int CValve_M_b::RestoreState( char * StrName )
{
	//R_CLASS("_W_Valve_M_b", _W_Valve_M_b);
	if ( lstrcmp ( StrName, "_W_Valve_M_b" ) == 0 )
	{
		ASS(pSRFile->RStruct( sizeof ( _W_Valve_M_b ), static_cast<_W_Valve_M_b*>(this)));
		Valve_old = Valve;		
		return 0;
	}
	return CValve_b::RestoreState(StrName);
}
