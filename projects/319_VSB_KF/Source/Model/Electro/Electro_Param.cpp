#include "stdafx.h"
#include "Electro.h"
#include "Err.h"
#include "CommProc.h"


_W_Electro::_W_Electro()
{
	memset ( this, 0, sizeof (_W_Electro));
}


int CElectro::GetParams( char * )
{	
	#include "IO_Parms.h"
	#include "Points.h"
	//ACS_Params( this );
	return 0;
}

int CElectro::UpdateParam( CParams & Param )
{
	return 0;
}

//#include "ShowParams.h"

int CElectro::ShowParams( DWORD DataType, struct CShowData * pSD )
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