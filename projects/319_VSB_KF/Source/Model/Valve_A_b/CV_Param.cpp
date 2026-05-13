#include "stdafx.h"
#include "CV.h"
#include "Err.h"
#include "CommProc.h"


_W_CV::_W_CV()
{
	memset ( this, 0, sizeof (_W_CV));
}


int CCV::GetParams( char * )
{	
	#include "IO_Parms.h"
	#define GET_PARAMS
	#include "CV_Pnt.h"
	#undef GET_PARAMS
	return CValve_A_b::GetParams(NULL);
}

int CCV::UpdateParam( CParams & )
{
  return 0;
}

int CCV::ShowParams( DWORD DataType, struct CShowData * pSD )
{
	int Res = CValve_A_b::ShowParams(DataType, pSD);
	if ( GetAsyncKeyState( VK_CONTROL ) & 0x8000 )
	{
		if(Position == 100.0)
			Task = Zadv_In = Zadv_Out = Zadv_Bypass = 100.0;
		else if(Position == 0.0)
			Task = Zadv_In = Zadv_Out = Zadv_Bypass = 0.0;
	}
	return Res;
}

void CCV::DrawObj ( struct CDrawObjData * pDraw )
{
	//TransparentStd( pDraw->hDC, pDraw->rc, ColZadv(Position));
	if((Strapping & STRAPPING_IN) || (Strapping & STRAPPING_OUT) || (Strapping & STRAPPING_BAYPASS))
	{
		if(strstr(pDraw->ObjName, ".Рег."))
			CValve_A_b::DrawObj(pDraw);	
		else if(strstr(pDraw->ObjName, ".Вх.") && (Strapping & STRAPPING_IN))
			TransparentStd( pDraw->hDC, pDraw->rc, ColZadv(Zadv_In));	
		else if(strstr(pDraw->ObjName, ".Вых.") && (Strapping & STRAPPING_OUT))
			TransparentStd( pDraw->hDC, pDraw->rc, ColZadv(Zadv_Out) );
		else if(strstr(pDraw->ObjName, ".Бп.") && (Strapping & STRAPPING_BAYPASS))
			TransparentStd( pDraw->hDC, pDraw->rc, ColZadv(Zadv_Bypass));
	}
	else
		CValve_A_b::DrawObj(pDraw);			
}
