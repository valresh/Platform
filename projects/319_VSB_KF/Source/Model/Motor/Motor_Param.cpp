#include "stdafx.h"
#include "Motor.h"
#include "Err.h"
#include "CommProc.h"
#ifndef LINUX
#include "SoftGDI.h"
#else
#include "WinDraw.h"
#endif



_W_Motor_b::_W_Motor_b()
	{
  memset ( this, 0, sizeof ( _W_Motor_b ));
	}


int CMotor::GetParams(  char * StrName )
  {
  KKK();
	#include "IO_Parms.h"
	#include "Points.h"
  CTRL_Params( "Задания", pMain ? pMain : this, $"Задания" );
  ACS_Params( "АСУ", this );
  Defect_Params(this);
	return 0;
	}

int CMotor::UpdateParam( CParams & Param )
  {
  return 0;
  }

//#include "ShowParams.h"
   

int CMotor::ShowParams( DWORD DataType, struct CShowData * pSD )
  {
  if ( GetAsyncKeyState( VK_CONTROL ) & 0x8000 )
    On = !On;
  return 0;
  }


HBRUSH hFon = CreateSolidBrush(RGB ( 255, 0, 0 ));
HPEN hPen = CreatePen(PS_SOLID,2,RGB ( 255, 255, 255 )); 

void CMotor::DrawObj ( struct CDrawObjData * pDraw )
  {

  if ( On )
    TransparentStd( pDraw->hDC, pDraw->rc, RGB ( 0, 255, 0 ));
  else
     TransparentStd( pDraw->hDC, pDraw->rc, RGB ( 255, 0, 0 ));
  if ( ErrInBlk )
    {
    static HPEN hPen_NoValid = CreatePen( PS_SOLID, 3, RGB ( 0, 255, 0 )); 
    HPEN OldPen = (HPEN)SelectObject ( pDraw->hDC, hPen_NoValid );
    MoveToEx ( pDraw->hDC, pDraw->rc.left, pDraw->rc.top, NULL );
    LineTo ( pDraw->hDC, pDraw->rc.right, pDraw->rc.bottom );
    MoveToEx ( pDraw->hDC, pDraw->rc.right, pDraw->rc.top, NULL );
    LineTo ( pDraw->hDC, pDraw->rc.left, pDraw->rc.bottom );
    SelectObject ( pDraw->hDC, OldPen );
    }
  if ( Oborot_AntiReg <= 0. )
    return;
  HPEN hPenOld = (HPEN)SelectObject ( pDraw->hDC, hPen );
  HBRUSH hFonOld = (HBRUSH)SelectObject ( pDraw->hDC, hFon );
  int x = ( pDraw->rc.left + pDraw->rc.right ) >> 1;
  int y = ( pDraw->rc.top + pDraw->rc.bottom ) >> 1;
  int r = 5;
  Ellipse ( pDraw->hDC, x - r, y - r, x + r, y + r );
  SelectObject ( pDraw->hDC, hPenOld );
  SelectObject ( pDraw->hDC, hFonOld );
  }
