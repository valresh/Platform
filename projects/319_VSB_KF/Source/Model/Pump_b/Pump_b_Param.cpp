#include "stdafx.h"
#include "Pump_b.h"
#include "Err.h"
#include "CommProc.h"


_W_Pump_b::_W_Pump_b()
{
	memset ( this, 0, sizeof (_W_Pump_b));
	To_m3 = To_kg = 1.0;
	//F_real = 1.0;
}


int CPump_b::UpdateParam( CParams & Param )
{
	return 0;
}

//#include "ShowParams.h"

int CPump_b::ShowParams( DWORD DataType, struct CShowData * pSD )
{
	/*
	switch ( DataType )
	{
	case sp_Params:
	GetParams( pSD->N, pSD->Params );
	return 1;
	}
	*/
	return 0;
}


void CPump_b::DrawObj ( struct CDrawObjData * pDraw )
{
	COLORREF Color = *pMotor->pOn ? RGB(0,255,0) : RGB(255,0,0);
	TransparentStd( pDraw->hDC, pDraw->rc, Color);
	if(bIgnoreBlk || (_pMotor && _pMotor->ErrInBlk))
	{
		static HPEN hPenCross = CreatePen(PS_SOLID,3,RGB ( 0, 0, 255 ));
		HPEN hOld = (HPEN)SelectObject ( pDraw->hDC, hPenCross );
		MoveToEx ( pDraw->hDC, pDraw->rc.left - 10, pDraw->rc.top - 10, NULL );
		LineTo ( pDraw->hDC, pDraw->rc.right + 10, pDraw->rc.bottom + 10 );
		MoveToEx ( pDraw->hDC, pDraw->rc.right + 10, pDraw->rc.top - 10, NULL );
		LineTo ( pDraw->hDC, pDraw->rc.left - 10, pDraw->rc.bottom + 10 );
		SelectObject ( pDraw->hDC, hOld );
	}
}
