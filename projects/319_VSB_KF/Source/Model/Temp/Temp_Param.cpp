#include "stdafx.h"
#include "Temp.h"
#include "Err.h"
#include "CommProc.h"



_W_Temp::_W_Temp()
	{
  memset ( this, 0, sizeof ( _W_Temp ));
	}


int CTemp::GetParams( char * )
	{
	#include "IO_Parms.h"
	#include "Points.h"
	return 0;
	}

int CTemp::UpdateParam( CParams & Param )
  {
  return 0;
  }

//#include "ShowParams.h"

int CTemp::ShowParams( DWORD DataType, struct CShowData * pSD )
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

#ifdef LINUX
#else
HBRUSH hFon = CreateSolidBrush( RGB ( 255, 255, 255 ));
void CTemp::DrawObj ( struct CDrawObjData * pDraw )
  {
      if(g_bInstructor)
          return;
  //  FillRect ( pDraw->hDC, &pDraw->rc, hFon );
  static HPEN hPen = CreatePen( PS_SOLID, 2, RGB ( 255, 255, 0 ));
  static HBRUSH hFon = (HBRUSH)GetStockObject( NULL_BRUSH );
  HPEN hOldPen = (HPEN)SelectObject ( pDraw->hDC, hPen );
  HBRUSH hOldFon = (HBRUSH)SelectObject ( pDraw->hDC, hFon );
  Rectangle ( pDraw->hDC, pDraw->rc.left-4, pDraw->rc.top-4, pDraw->rc.right+4, pDraw->rc.bottom+4 );
  Rectangle ( pDraw->hDC, pDraw->rc.left-8, pDraw->rc.top-8, pDraw->rc.right+8, pDraw->rc.bottom+8 );
  SelectObject ( pDraw->hDC, hOldPen );
  SelectObject ( pDraw->hDC, hOldFon );
  }
#endif

