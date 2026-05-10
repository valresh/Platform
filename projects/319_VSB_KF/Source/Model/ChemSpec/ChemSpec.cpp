#include "stdafx.h"
#include "ChemSpec.h"
#include "Err.h"
//#include "CommProc.h"
#include "SetData.h"

CChemSpec::CChemSpec( char * _ObjName ) :	IBaseModel ( _ObjName ) 
	{
  ObjName = _ObjName;
  Model = "ChemSpec";
	#include "IO_Clear.h"
	#include "Points.h"
	}

CChemSpec::~CChemSpec()
	{
	}

int CChemSpec::Init( int Regim )
	{
	int Res = 0;
	return 0;
	}

int CChemSpec::Step0( )
	{
  //CComp * pComp = new CComp;
	return 0;
	}

int CChemSpec::Step1( )
	{
	return 0;
	}

int CChemSpec::StepT ( double dt )
	{
	return 0;
	}

int CChemSpec::SetData( int TypeData, void * pData )
	{
	return 1;
	}

int CChemSpec::SaveState( )
  {
//	S_CLASS("W",_W_ChemSpec)
  return 0;
  }

int CChemSpec::RestoreState( char * StrName )
  {
//	R_CLASS("W",_W_ChemSpec)
  return 1;
  }
