#include "stdafx.h"
#include "Zadv3x.h"
#include "Err.h"
//#include "CommProc.h"
//#include "Data.h"
//#include "SetData.h"

CZadv3x::CZadv3x( char * _ObjName ) :	IBaseModel ( _ObjName ) 
#include "ACS_Constr.h"
#include "ACS.h"
	{
  UNIMODEL
	lstrcpy ( ObjName, _ObjName );
	#include "IO_Clear.h"
	#include "Points.h"
  Type = 0;
	}

CZadv3x::~CZadv3x()
	{
	}

int CZadv3x::Init( int Regim )
	{
	int Res = 0;
	#include "IO_Init.h"
	#include "Points.h"
	//#include "ACS_Init.h"  
	//#include "ACS.h"
  //GiveCommData ( ObjName, CZadv::TypeID, (void**)&pZadv );
	return 0;
	}

int CZadv3x::Step0( )
	{
	return 0;
	}

int CZadv3x::Step1( )
	{
	return 0;
	}

int CZadv3x::StepT ( double dt )
	{
  Calc ( dt );
	return 0;
	}

int CZadv3x::SetData( int TypeData, void * pData )
	{
	return 1;
	}

int CZadv3x::SaveState( )
  {
//	S_CLASS("W",_W)
  return 0;
  }

int CZadv3x::RestoreState( char * StrName )
  {
//	R_CLASS("W",_W)
  return 1;
  }
