#include "stdafx.h"
#include "CC_1.h"
#include "Err.h"
#include "CommProc.h"
#include "SoftGDI.h"


_W::_W()
	{
  memset ( this, 0, sizeof ( _W ));
  P_in = 1.0;
	}

int CCC_1::GetParams( char * )
	{
#define GET_PARAMS
	#include "IO_Parms.h"
	#include "Points.h"
  if ( k_Pnt_Q > 0 )
    {
/**/
    TAB("Рабочая характеристика",1)
      Char<128>Name;                                                   
      for ( int n = 0; n <= k_Pnt_Q; n++ )
        {
        Name.Prt("Отн F[%d]", n );
        PARM(F_Характеристики[n],Name )
        Name.Prt("Отн P[%d]", n );
        PARM(P_Характеристики[n],Name )
//        Name.Prt("Точка %d", n );
//        sprintf_s( S_Характеристики[n], sizeof(char16), "%4.3lf,   %4.3lf", F_Характеристики[n], P_Характеристики[n] );
//        PARM(S_Характеристики[n], Name )
        }
    ETAB
/**/
    }
  TAB("Блокировки",1)
	  PARM( CanPuskStop, "Локальные блокировки" )
	  PARM( ErrInBlk, "#Сработала блокировка" )
	  PARM( Err_BlkInObj, "#Срабатывание блокировки в ..." )
  ETAB
  Defect_Params(this);
  ACS_Params( "АСУ", this );
//	TAB("Модули",5)
	return 0;
	}

int CCC_1::UpdateParam( CParams & Param )
  {
  ST_1.Calc_K_P2_P1( );
  if ( ST_1.KPD > 100. )
    ST_1.KPD = 100.;
  if ( ST_1.KPD < 10. )
    ST_1.KPD = 10.;
  if ( ST_1.KPD_M > 100. )
    ST_1.KPD_M = 100.;
  if ( ST_1.KPD_M < 10. )
    ST_1.KPD_M = 10.;
  //if ( k_Pnt_Q > 0 )
  //  {
  //  for ( int n = 0; n <= k_Pnt_Q; n++ )
  //    {
  //    if ( Param.Addr == S_Характеристики[n] )
  //      {
  //      char S[256];
  //      lstrcpy ( S, S_Характеристики[n] );
  //      F_Характеристики[n] = atof ( S );
  //      char * P = strchr ( S, ',' );
  //      if ( P )
  //        P_Характеристики[n] = atof ( P + 1 );
  //      else
  //        {
  //        P = strchr ( S, ';' );
  //        if ( P )
  //          P_Характеристики[n] = atof ( P + 1 );
  //        }
  //      sprintf_s( S_Характеристики[n], sizeof(char16), "%4.3lf,%4.3lf", F_Характеристики[n], P_Характеристики[n] );
  //      }
  //    }
  //  }
  return 0;
  }

//#include "ShowParams.h"

int CCC_1::ShowParams( DWORD DataType, struct CShowData * pSD )
  {
	switch ( DataType )
		{
		case sp_Actions:
		return 1;
		}
  return 0;
  }

void CCC_1::DrawObj ( struct CDrawObjData * pDraw )
  {
  if ( On )
    TransparentStd( pDraw->hDC, pDraw->rc, RGB ( 0, 255, 0 ));
  else
    TransparentStd( pDraw->hDC, pDraw->rc, RGB ( 255, 0, 0 ));
  if ( ErrInBlk )
    {
    static HPEN hPen_NoValid = CreatePen( PS_SOLID, 3, RGB ( 0, 255, 0 )); 
    HGDIOBJ OldPen = SelectObject ( pDraw->hDC, hPen_NoValid );
    MoveToEx ( pDraw->hDC, pDraw->rc.left, pDraw->rc.top, NULL );
    LineTo ( pDraw->hDC, pDraw->rc.right, pDraw->rc.bottom );
    MoveToEx ( pDraw->hDC, pDraw->rc.right, pDraw->rc.top, NULL );
    LineTo ( pDraw->hDC, pDraw->rc.left, pDraw->rc.bottom );
    SelectObject ( pDraw->hDC, OldPen );
    }
  }
