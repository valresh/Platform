#include "stdafx.h"
#include "Klap3x.h"
#include "Err.h"
#include "CommProc.h"


_W::_W()
	{
  memset ( this, 0, sizeof ( _W ));
	}


int CKlap3x::GetParams( char * )
	{
	#include "IO_Parms.h"
	#include "Points.h"
  Defect_Params(this);
  ACS_Params( "АСУ", this );
//	TAB("Модули",5)
	return 0;
	}

int CKlap3x::UpdateParam( CParams & Param )
  {
  //if ( Param.Addr == &Vent )
  //  {
  //  pPos->Vent = pPos->VentZ = Vent;
  //  }
  return 0;
  }

//#include "ShowParams.h"

int CKlap3x::ShowParams( DWORD DataType, struct CShowData * pSD )
  {
	switch ( DataType )
		{
		case sp_Actions:
		return 1;
		}
  return 0;
  }

HPEN hPen4 = CreatePen(PS_SOLID,3,RGB ( 0, 0, 255 )); 
void CKlap3x::DrawObj ( struct CDrawObjData * pDraw )
  {
  if ( ErrInBlk )
    {
    // Блокировки изменят положение
    HPEN hOld = (HPEN)SelectObject ( pDraw->hDC, hPen4 );
    MoveToEx ( pDraw->hDC, pDraw->rc.left - 10, pDraw->rc.top - 10, NULL );
    LineTo ( pDraw->hDC, pDraw->rc.right + 10, pDraw->rc.bottom + 10 );
    MoveToEx ( pDraw->hDC, pDraw->rc.right + 10, pDraw->rc.top - 10, NULL );
    LineTo ( pDraw->hDC, pDraw->rc.left - 10, pDraw->rc.bottom + 10 );
    SelectObject ( pDraw->hDC, hOld );
//    ::DeleteObject (hOld);
    }
  }
