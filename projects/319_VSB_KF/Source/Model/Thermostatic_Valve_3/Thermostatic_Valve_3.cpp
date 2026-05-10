#include "stdafx.h"
#include "Thermostatic_Valve_3.h"
#include "Err.h"
//#include "CommProc.h"
#include "Work.h"
#include "SetData.h"

CThermostatic_Valve_3::CThermostatic_Valve_3( char * _ObjName ) :	IBaseModel ( _ObjName ),
state_o("Состояние o")
#include "ACS_Constr.h"
#include "ACS.h"
{
	UNIMODEL
	ObjName = _ObjName;
	memset(pFlow, NULL, sizeof(pFlow));
	pCompOut = NULL;
	ID_CompOut = 0;
#include "IO_Clear.h"
#include "Points.h"
}

CThermostatic_Valve_3::~CThermostatic_Valve_3()
{

}

int CThermostatic_Valve_3::Init( int Regim )
{
	int Res = 0;
	NewComp( &ID_CompOut, &pCompOut, "Состав из %s", ObjName );
	//#include "IO_Init.h"
	//#include "Points.h"
	return 0;
}

int CThermostatic_Valve_3::Step0( )
{
	return 0;
}

int CThermostatic_Valve_3::Step1( )
{
	return 0;
}

int CThermostatic_Valve_3::StepT ( double dt )
{
	Calc ( dt );
	return 0;
}

int CThermostatic_Valve_3::SetData( int TypeData, void * pData )
{
	return 1;
}

int CThermostatic_Valve_3::SaveState( )
{
	S_CLASS("W",_W_Thermostatic_Valve_3)
	return 0;
}

int CThermostatic_Valve_3::RestoreState( char * StrName )
{
	R_CLASS("W",_W_Thermostatic_Valve_3)
	return 1;
}












