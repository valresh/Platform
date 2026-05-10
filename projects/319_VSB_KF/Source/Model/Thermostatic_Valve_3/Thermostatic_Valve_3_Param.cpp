#include "stdafx.h"
#include "Thermostatic_Valve_3.h"
#include "Err.h"
#include "CommProc.h"


_W_Thermostatic_Valve_3::_W_Thermostatic_Valve_3()
{
	memset ( this, 0, sizeof (_W_Thermostatic_Valve_3));
}


int CThermostatic_Valve_3::GetParams( char * )
{	
	#include "IO_Parms.h"
	#include "Points.h"
	ACS_Params( this );


	/*TABU("ÀÑÓ")
	#include "ACS_Param.h"
	#include "ACS.h"
	TABU("Ìîäóëè")
	*/
	return 0;
}

int CThermostatic_Valve_3::UpdateParam( CParams & Param )
{
	return 0;
}

//#include "ShowParams.h"

int CThermostatic_Valve_3::ShowParams( DWORD DataType, struct CShowData * pSD )
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