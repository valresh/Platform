#include "stdafx.h"
#include "Temp.h"
#include "Err.h"
//#include "CommProc.h"
#include "Work.h"
#include "SetData.h"

CTemp::CTemp( char * _ObjName ) :	IBaseModel ( _ObjName ) 
	{
    UNIMODEL
    ObjName = _ObjName;
    Model = "Temp";
	#include "IO_Clear.h"
	#include "Points.h"
	}

CTemp::~CTemp()
	{
	}

int CTemp::Init( int Regim )
	{
	return 0;
	}

int CTemp::Step0( )
	{
	return 0;
	}

bool First = true;
int CTemp::Step1( )
  {
  // if ( First )
  //   {
  //   First = false;
  //   void * F = Fopen ( "Temp.csv", "wt" );
  //   Fclose( F );
  //   }
  // Fsend ( "Temp.csv", "%s;\n", ObjName.Str );
  return 0;
  }

int CTemp::StepT ( double dt )
	{
  Calc ( dt );
	return 0;
	}

int CTemp::SetData( int TypeData, void * pData )
	{
	return 1;
	}

int CTemp::SaveState( )
  {
	S_CLASS("W",_W_Temp)
  return 0;
  }

int CTemp::RestoreState( char * StrName )
  {
  IGNORE
  R_CLASS("W",_W_Temp)
  return 1;
  }
