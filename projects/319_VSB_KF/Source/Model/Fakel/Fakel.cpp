#include "stdafx.h"
#include "Fakel.h"
#include "Err.h"
#include "CommProc.h"
#include "SetData.h"
#include "Data.h"


CFakel::CFakel( LPCTSTR _ObjName ) : IBaseModel ( (char*)_ObjName )
, Furnace ( "Форсунка", this )
#include "ACS_Constr.h"
#include "ACS.h"
  {
	UNIMODEL
	#include "IO_Clear.h"
	#include "Points.h"

  // Падает
  //CLEAR(pGas_in)
  pGas_in = NULL;
  pFuel_in = NULL;
  pSteam_in = NULL;
	}

CFakel::~CFakel()
	{
	}

int CFakel::Init( int )
	{
	int Res = 0;
	#include "IO_Init.h"
	#include "Points.h"
	return 0;
	}

int CFakel::Step0( )
	{
	return 0;
	}

int CFakel::Step1( )
	{
//	Sigma = 10.;
	return 0;
	}

int CFakel::StepT ( double dt )
	{
  Calc ( dt );
	return 0;
	}

int CFakel::SetData( int TypeData, void * pData )
	{
	return 1;
	}

int CFakel::SaveState( )
  {
	S_CLASS("W",_W)
  return 0;
  }

int CFakel::RestoreState( char * StrName )
  {
	IGNORE
	R_CLASS("W",_W)
  return 1;
  }

int CFurnace::SaveState( )
  {
  S_CLASS("W",CFurnace_W)
  return 0;
  }

int CFurnace::RestoreState ( char * StrName )
  {
  IGNORE
  R_CLASS("W",CFurnace_W)
  return 1;
  }
