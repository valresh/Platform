#include "stdafx.h"
#include "Dynamic.h"
#include "Err.h"
#include "CommProc.h"

static double paise = 50;
_W_Dynamic::_W_Dynamic()
{
	memset ( this, 0, sizeof (_W_Dynamic));
}


int CDynamic::GetParams( char * )
{	
#define GET_PARAMS
#include "IO_Parms.h"
#include "Dynamic_Pnt.h"
#undef GET_PARAMS
	CTRL_Params( "Задания", pMain ? pMain : this, $"Задания" );
	ACS_Params(this);
	Defect_Params(this);
	return 0;
}

int CDynamic::UpdateParam( CParams & Param )
{
	if(Param.Addr == &p_nom || Param.Addr == &Q_nom || Param.Addr == &p_max || Param.Addr == &ro_nom)
	{				  
		CalcCharacteristic();					  
	}
	return 0;
}

//#include "ShowParams.h"

int CDynamic::ShowParams( DWORD DataType, struct CShowData * pSD )
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


void CDynamic::DrawObj( struct CDrawObjData * pDraw )
{
	if(bRest)
		TransparentStd( pDraw->hDC, pDraw->rc, RGB(255,255,0));
	else if(*pMotor->pOn && ro < ro_nom * 0.01 && F_mol > 0.0)
		TransparentStd( pDraw->hDC, pDraw->rc, RGB(255,165,0));
	else
		CPump_b::DrawObj(pDraw);
}

#ifndef LINUX
void CDynamic::DrawObject ( struct CDrawObjData * pDraw, CGDIResourceMgr *pResMgr)
{
  DrawObj(pDraw);
}
#endif

