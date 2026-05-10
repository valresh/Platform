#include "stdafx.h"
#include "Driver.h"
#include "Err.h"
//#include "CommProc.h"
#include "Work.h"
#include "SetData.h"

CDriver::CDriver( char * _ObjName ) :	IBaseModel ( _ObjName ) 
#include "ACS_Constr.h"
#include "ACS.h"
	{
  UNIMODEL
  POOLCTRL
  ObjName = _ObjName;
  Model = "Driver";
	#include "IO_Clear.h"
	#include "Points.h"
  kDrive = 0;
  pMotorData = NULL;
  nMotor = -1;
	}

CDriver::~CDriver()
	{
	}

int CDriver::Init( int Regim )
	{
	int Res = 0;
	//#include "IO_Init.h"
	//#include "Points.h"
	return 0;
	}

int CDriver::Step0( )
	{
	return 0;
	}

int CDriver::Step1( )
	{
	return 0;
	}

int CDriver::StepT ( double dt )
	{
	return 0;
	}

int CDriver::SetData( int TypeData, void * pData )
	{
  switch ( TypeData )
    {
    case sd_SetInitState:
      {
//      P = 1.;
//      T = 15.;
      return 0;
      }
    case sd_ShowObjInfo:
      {
      Char<256>* pInfo = (Char<256>*)pData;
      if ( On )
        pInfo->Prt( $"%s: Обороты=%4.0lf, Мощность=%4.0lfкВт", ObjName.Str, AbsOborot, Power );
      else
        pInfo->Prt( $"%s: Двигатель стоит", ObjName.Str );
      }
    break;
    }
	return 1;
	}

int CDriver::SaveState( )
  {
	S_CLASS("W",_W_Driver)
  return 0;
  }

int CDriver::RestoreState( char * StrName )
  {
	R_CLASS("W",_W_Driver)
  return 1;
  }
