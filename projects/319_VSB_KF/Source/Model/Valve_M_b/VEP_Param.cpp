#include "stdafx.h"
#include "VEP.h"
#include "Err.h"
#include "CommProc.h"
#ifdef LINUX
#include "WinDraw.h"
#endif


_W_VEP::_W_VEP()
{
	memset ( this, 0, sizeof (_W_VEP));
	Omega_Plug = 1.0;
	Plug = false;
}


int CVEP::GetParams( char * )
{	
	#include "IO_Parms.h"
    #include "Vep_Pnt.h"
#include "Valve_M_b.h"
	return CValve_M_b::GetParams(NULL);
}

void CVEP::DrawObj( struct CDrawObjData * pDraw )
{
	if(strstr(pDraw->ObjName, ".Задв"))
		CValve_M_b::DrawObj(pDraw);
	else if(strstr(pDraw->ObjName, ".Загл"))
	{
		//Rectangle(pDraw->hDC, pDraw->rc.left, pDraw->rc.top, pDraw->rc.right, pDraw->rc.bottom);
		//TransparentStd( pDraw->hDC, pDraw->rc, Plug == true ? RGB(255, 0, 0) : RGB(0, 255, 0));
		TransparentRect( pDraw->hDC, pDraw->rc, Plug == true ? RGB(255, 0, 0) : RGB(0, 255, 0), 0x040302);
	}
}


int CVEP::ShowParams( DWORD DataType, struct CShowData * pSD )
{
	if ( GetAsyncKeyState( VK_CONTROL ) & 0x8000 )
	{
		if(Position != 0.0)
			Plug = true;
		else 
			Plug = false;
	}
	return CValve_M_b::ShowParams(DataType, pSD);
}
