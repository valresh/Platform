
#include "stdafx.h"
#include "CtrlReg.h"
#include "Err.h"
#include "CommProc.h"


_W_CtrlReg::_W_CtrlReg()
{
	memset ( this, 0, sizeof (_W_CtrlReg));
}


int CCtrlReg::GetParams( char * )
{	
	//#include "IO_Parms.h"
	//#include "Points.h"
	//ACS_Params( this );


	/*TABU("ÀÑÓ")
	#include "ACS_Param.h"
	#include "ACS.h"
	TABU("Ìîäóëè")
	*/
	return 0;
}

int CCtrlReg::UpdateParam( CParams & Param )
{
	return 0;
}

//#include "ShowParams.h"

int CCtrlReg::ShowParams( DWORD DataType, struct CShowData * pSD )
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
