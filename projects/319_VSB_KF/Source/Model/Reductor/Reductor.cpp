#include "stdafx.h"
#include "Reductor.h"
#include "Err.h"
//#include "CommProc.h"
//#include "Data.h"
//#include "SetData.h"

CReductor::CReductor( char * _ObjName ) :	IBaseModel ( _ObjName ) 
#include "ACS_Constr.h"
#include "ACS.h"
{
	UNIMODEL
	lstrcpy ( ObjName, _ObjName );
#include "IO_Clear.h"
#include "Points.h"
	IsCtrlGas = false;
	dP_Gas_In = 1.;
	P_Gas = 0.;
}

CReductor::~CReductor()
{
}

int CReductor::Init( int Regim )
{
	int Res = 0;
	//#include "IO_Init.h"
	//#include "Points.h"
	//#include "ACS_Init.h"  
	//#include "ACS.h"
	return 0;
}

int CReductor::Step0( )
{
	return 0;
}

int CReductor::Step1( )
{
	return 0;
}

int CReductor::StepT ( double dt )
{
	Calc ( dt );
	return 0;
}

int CReductor::SetData( int TypeData, void * pData )
{
	return 1;
}

int CReductor::SaveState( )
{
	S_CLASS("W",_W)
		return 0;
}

int CReductor::RestoreState( char * StrName )
{
	COPY_FIRST
		R_CLASS("W",_W)
		return 1;
}
