#include "stdafx.h"
#include "Gag.h"
#include "Err.h"
//#include "CommProc.h"
//#include "Data.h"
#include "SetData.h"

CGag::CGag( char * _ObjName ) :	IBaseModel ( _ObjName ) 
#include "ACS_Constr.h"
#include "ACS.h"
	{
  UNIMODEL
	ObjName = _ObjName;
	#include "IO_Clear.h"
	#include "Points.h"
    #include "Gag_Defects.h"
  NewParams = true;
  Defect = 0;
  pF = NULL;
  dP = 0.;
//  pGag = NULL;
  OnlyIn = false;
  OnlyOut = false;
  NoUsed = false;
	}

CGag::~CGag()
	{
	}

int CGag::Init( int )
	{
	int Res = 0;
	//#include "IO_Init.h"
	//#include "Points.h"
	//#include "ACS_Init.h"  
	//#include "ACS.h"
//  ASS(GiveCommData( ObjName, CHs::TypeID,	(void**)&pGag)==0);
	return 0;
	}

int CGag::Step0( )
	{
//  Исправна = 1;
	return 0;
	}

int CGag::Step1( )
	{
//  pGag->mOn = true;
	Заглушка = (Положение != 0);
	return 0;
	}

int CGag::StepT ( double dt )
	{
  Calc ( dt );
	return 0;
	}

int CGag::SetData( int TypeData, void * pData )
	{
  if ( TypeData == sd_SetInitState )
    {
	  Заглушка = false;
//    pGag->mOn = false;
    return 0;
    }
  if ( TypeData == sd_SaveIfNeverOpen )
    {
    if ( IsOpen || UsedInTuning )
      {
      UsedInTuning = true;
      return 0;
      }
//    PrtNoUsed( pData );
    return 0;
    }
  if ( TypeData == sd_ModelCmd )
    {
    CModelCmd * pCmd = (CModelCmd*)pData;
    if ( lstrcmp ( pCmd ->Cmd, "Снять заглушку" ) == 0 )
      {
      IsOpen = true;
	    Заглушка = false;
      //if ( pGag )
      //  pGag->mOn = true;
      return sizeof ( CModelCmd );
      }
    if ( lstrcmp ( pCmd ->Cmd, "Установить заглушку" ) == 0 )
      {
      IsOpen = false;
      Заглушка = true;
      //if ( pGag )
      //  pGag->mOn = false;
      return sizeof ( CModelCmd );
      }
    return 0;
    }
	return 1;
	}

int CGag::SaveState( )
  {
  IGNORE
  Положение = Заглушка ? 1 : 0;
	S_CLASS("W",_W)
  return 0;
  }

int CGag::RestoreState( char * StrName )
  {
  IGNORE
	R_CLASS("W",_W)
  return 1;
  }
