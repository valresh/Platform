#include "stdafx.h"
#include "Collector.h"
#include "Err.h"
#include "CommProc.h"
#include "ACS_Def.h"
#include "HydroStruct.h"


_W::_W()
	{
  memset ( this, 0, sizeof ( _W ));
	}


int CCollector::GetParams(  char * StrName )
	{
	Char<256> Name;
#define GET_PARAMS
	#include "IO_Parms.h"
	#include "Points.h"
  Defect_Params(this);
  ACS_Params( "АСУ", this );
  TAB("Стабильность",1)
  Stable.GetParams( this );
  ETAB
 //	TAB("Модули",5)
	return 0;
	}

int CCollector::UpdateParam( CParams & Param )
  {
  if ( Param.Addr == Состав )
    {
    if ( Состав[0] )
      pCompZ->Read( Состав );
    }
  if ( Use_Comp )
    {
    pComp->Set( pCompZ );
    Use_Tz = true;
    memmove ( IsComp, pComp->IsComp, sizeof ( IsComp ));
    memmove ( Cmol, pComp->Cmol, sizeof ( Cmol ));
    To_kg = ::Mw( pComp );
    T = Tz;
    State.PT( P, T, pComp );
    h_Flow = State.h;
    if ( State.E > 0.2 )
      To_m3 = M3;
    else
      To_m3 = To_m3_liq ( T, pComp );
    }
  if ( Param.Addr == &Summ_In )
    {
    int n = Summ_In - 1;
    if ( n >= 0 )
      {
      CObjectPoint * pOP = pNodes[n].pOP;
      strcpy_s ( NameIn, 64, pOP->pPool->Name );
      }
    else
      strcpy_s ( NameIn, 64, "" );
    return 0;
    }
  if ( Param.Addr == &Summ_Out )
    {
    int n = Summ_Out - 1;
    if ( n >= 0 )
      {
      CObjectPoint * pOP = pNodes[n].pOP;
      strcpy_s ( NameOut, 64, pOP->pPool->Name );
      }
    else
      strcpy_s ( NameOut, 64, "" );
    return 0;
    }
  return 0;
  }

//#include "ShowParams.h"

int CCollector::ShowParams( DWORD DataType, struct CShowData * pSD )
  {
	switch ( DataType )
		{
		case sp_Actions:
		return 1;
		}
  return 0;
  }

int iabs ( int v )
  {
  if ( v >= 0 )
    return v;
  return -v;
  }

HBRUSH hFon = CreateSolidBrush( RGB ( 142, 142, 142 )); 
void CCollector::DrawObj (struct CDrawObjData *pDraw )
  {
	  if(g_bInstructor)
		  return;
  if ( iabs ( pDraw->rc.top - pDraw->rc.bottom ) > 5 && iabs ( pDraw->rc.left - pDraw->rc.right ) > 5 )
    return;
  pDraw->rc.top -= 2;
  pDraw->rc.left -= 2;
  pDraw->rc.right += 2;
  pDraw->rc.bottom += 2;
  HBRUSH hOldFon = (HBRUSH)SelectObject ( pDraw->hDC, hFon );
  Rectangle ( pDraw->hDC, pDraw->rc.left, pDraw->rc.top, pDraw->rc.right, pDraw->rc.bottom ); 
  SelectObject ( pDraw->hDC, hOldFon );
  }
