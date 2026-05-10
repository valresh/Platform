#include "stdafx.h"
#define DLL_PressDrop
//#include "PressDrop.h"
#include "Err.h"
//#include "CommProc.h"
#include "SetData.h"
#include "PressDrop.h"

CPressDrop::CPressDrop( char * _ObjName, IBaseModel * _pMain, CSEps * _pEps ) :	IBaseModel ( _ObjName, _pMain ) 
	{
  ObjName = _ObjName;
	#include "IO_Clear.h"
	#include "PressDrop_Pnt.h"
  Model = "PressDrop";
  pEps  = _pEps;
  D = 0.01;
  S = 1e-4;
  Re = 10000.;
  Omega = 1000.;
  Q = 0.;
  Xi_Flow = 1.;
	}

CPressDrop::~CPressDrop()
	{
	}

int CPressDrop::Init( int Regim )
	{
	int Res = 0;
	//#include "IO_Init.h"
	//#include "Points.h"
  if ( pEps == NULL )
    pEps = new CSEps ( "Состояние", this );
	return 0;
	}

int CPressDrop::Step0( )
	{
	return 0;
	}

int CPressDrop::Step1( )
	{
  D = Dmm * 1e-3;
  S = 0.78539816339744830961566084581988 * D * D; 
	return 0;
	}

int CPressDrop::StepT ( double dt )
	{
	return 0;
	}

int CPressDrop::SetData( int TypeData, void * pData )
	{
	return 1;
	}

int CPressDrop::SaveState( )
  {
	S_CLASS("W",_W_PressDrop)
  return 0;
  }

int CPressDrop::RestoreState( char * StrName )
  {
  IGNORE
	R_CLASS("W",_W_PressDrop)
  return 1;
  }
