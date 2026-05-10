#include "stdafx.h"
#include "Volumetric.h"
#include "Err.h"
#include "CommProc.h"


_W_Volumetric::_W_Volumetric()
{
	memset ( this, 0, sizeof (_W_Volumetric));
}


int CVolumetric::GetParams( char * szParam)
{	
#define GET_PARAMS
#include "IO_Parms.h"
#include "Volumetric_Pnt.h"
#undef GET_PARAMS
	ACS_Params( this );
	return 0;
}

int CVolumetric::UpdateParam( CParams & Param )
{
	return 0;
}

//#include "ShowParams.h"

int CVolumetric::ShowParams( DWORD DataType, struct CShowData * pSD )
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


void CVolumetric::DrawObj( struct CDrawObjData * pDraw )
{
	CPump_b::DrawObj(pDraw);
}
#ifndef LINUX
void CVolumetric::DrawObject ( struct CDrawObjData * pDraw, CGDIResourceMgr *pResMgr)
{
  DrawObj(pDraw);
}
#endif

