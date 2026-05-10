#include "stdafx.h"
#include "Klap3x.h"
#include "Err.h"
//#include "CommProc.h"
//#include "Data.h"
//#include "SetData.h"

CKlap3x::CKlap3x( char * _ObjName ) :	IBaseModel ( _ObjName ) 
#include "ACS_Constr.h"
#include "ACS.h"
	{
  UNIMODEL
	lstrcpy ( ObjName, _ObjName );
	#include "IO_Clear.h"
	#include "Points.h"
    #include "Klap3x_Defects.h"
  pOut = NULL;
  ID_Out = 0;
  CLEAR(pFlow)
  Defect = 0;
  Fixed_Pos = 0;
	}

CKlap3x::~CKlap3x()
	{
	}

int CKlap3x::Init( int Regim )
	{
	int Res = 0;
	//#include "IO_Init.h"
	//#include "Points.h"
	//#include "ACS_Init.h"  
	//#include "ACS.h"
  Char<64>Name;
  for ( int n = 0; n < 3; n++ )
    {
    Name.Prt( "Вход %d", n );
    pState[n] = new CSEps ( Name, this );
    }
  NewComp( &ID_Out, &pOut, "Состав из %s", ObjName );
  M = 1.;
  pOut->Set( pWater );
//  GiveCommData(ObjName,CZadv::TypeID,(void**)&pPos);
	return 0;
	}

int CKlap3x::Step0( )
	{
	return 0;
	}

int CKlap3x::Step1( )
	{
	return 0;
	}

int CKlap3x::StepT ( double dt )
	{
  Calc ( dt );
	return 0;
	}

int CKlap3x::SetData( int TypeData, void * pData )
	{
	return 1;
	}

int CKlap3x::SaveState( )
  {
	S_CLASS("W",_W)
  return 0;
  }

int CKlap3x::RestoreState( char * StrName )
  {
	R_CLASS("W",_W)
  return 1;
  }
