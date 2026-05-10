#include "stdafx.h"
#include "PC_1.h"
#include "Err.h"
#include "CommProc.h"
#include "SoftGDI.h"


_W::_W()
	{
  memset ( this, 0, sizeof ( _W ));
  State = 3;
  Oborot = 1.;
	}


int CPC_1::GetParams( char * )
	{
	#include "IO_Parms.h"
	#include "Points.h"
	//char Name[256];
	//TAB("Показатель адиабаты газов", 1)
	//for(USHORT i = 0; i < LAST_GAS; i++)
	//  {
	//	sprintf_s ( Name, sizeof(Name), "Gamma %s(%d)", pGas[i]->Name, i);
	//	PARM(ST_1.Thermo.aGamma[i], Name );;
	//  }
	//ETAB
  Defect_Params(this);
  ACS_Params( "АСУ", this );
//	TAB("Модули",5)
	return 0;
	}

int CPC_1::UpdateParam( CParams & Param )
  {
 
  return 0;
  }

//#include "ShowParams.h"

int CPC_1::ShowParams( DWORD DataType, struct CShowData * pSD )
  {
/*
	switch ( DataType )
		{
		case sp_Params:
		  GetParams( "" );
		return 1;
		}
*/
  return 0;
  }

void CPC_1::DrawObj ( struct CDrawObjData * pDraw )
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
