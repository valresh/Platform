#include "stdafx.h"
#include "Valve_A_b.h"

int CValve_A_b::GetParams( char * )
{	
#include "IO_Parms.h"
	Defect_Params(this);
	ACS_Params( this );
	TAB("Задания", 1 )
		CTRL_Params( NULL, this, $"Задания" );
		PARM( dReg, "Замедление регулировки" )
    PARM(ShowConflict,"Показывать конфликты")
    PARM(MaxConflict,"Макс. конфликта, %")
  ETAB
  TAB("Регулятор", 1)
    Reg.GetParams( this );
  ETAB
	return CValve_b::GetParams(NULL);
}
int CValve_A_b::UpdateParam( CParams & Param )
{
  Reg.UpdateParam( Param );
  return 0;
}

int CValve_A_b::ShowParams( DWORD DataType, struct CShowData * pSD )
{
	int Res = CValve_b::ShowParams(DataType, pSD);
	if ( GetAsyncKeyState( VK_CONTROL ) & 0x8000 )
	{
		if(Position == 100.0)
			Task = 100.0;
		else if(Position == 0.0)
			Task = 0.0;
	}
	return Res;
}

void CValve_A_b::DrawObj ( struct CDrawObjData * pDraw )
{
	//TransparentStd( pDraw->hDC, pDraw->rc, ColZadv(Position));
	CValve_b::DrawObj(pDraw);
  if ( Reg_On )
	  {
		static HPEN hPenCross = CreatePen(PS_SOLID,3,RGB ( 255, 0, 255 ));
		HPEN hOld = (HPEN)SelectObject ( pDraw->hDC, hPenCross );
		MoveToEx ( pDraw->hDC, pDraw->rc.left - 10, pDraw->rc.top - 10, NULL );
		LineTo ( pDraw->hDC, pDraw->rc.right + 10, pDraw->rc.top - 10 );
		LineTo ( pDraw->hDC, pDraw->rc.right + 10, pDraw->rc.bottom + 10 );
		LineTo ( pDraw->hDC, pDraw->rc.left - 10, pDraw->rc.bottom + 10 );
		LineTo ( pDraw->hDC, pDraw->rc.left - 10, pDraw->rc.top - 10 );
		SelectObject ( pDraw->hDC, hOld );
    return;
	 }
	if(bIgnoreBlk)
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
