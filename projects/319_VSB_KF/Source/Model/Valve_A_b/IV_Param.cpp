#include "stdafx.h"
#include "IV.h"
#include "Err.h"
#include "CommProc.h"


_W_IV::_W_IV()
{
	memset ( this, 0, sizeof (_W_IV));
}


int CIV::GetParams( char * )
{	
	#include "IO_Parms.h"
	#define GET_PARAMS
	#include "IV_Pnt.h"
	#undef GET_PARAMS
	return CValve_A_b::GetParams(NULL);
}

//#include "ShowParams.h"

int CIV::ShowParams( DWORD DataType, struct CShowData * pSD )
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

#define IS_UPDATE(_Param) Param.Addr == &_Param

int CIV::UpdateParam( CParams & Param )
{
  if ( Param.Addr == Reg_Ref )
    pRef = NULL;

	if(IS_UPDATE(KIP))
	{
		switch(KIP)
		{
		case Открывается:
			lstrcpy(Type, "IVFO");
			break;
		case Закрывается:
			lstrcpy(Type, "IVFC");
			break;
		case Не_меняется:
			lstrcpy(Type, "IVFN");
			break;
		case Не_влияет:
			lstrcpy(Type, "IV");
			break;
		}
	}
	return CValve_A_b::UpdateParam(Param);
}

#undef IS_UPDATE
