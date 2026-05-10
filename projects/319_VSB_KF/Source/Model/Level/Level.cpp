#include "stdafx.h"
#include "Level.h"
#include "Err.h"
//#include "CommProc.h"
#include "Work.h"
#include "SetData.h"

CLevel::CLevel( char * _ObjName ) :	IBaseModel ( _ObjName ) 
#include "ACS_Constr.h"
#include "ACS.h"
	{
  ObjName = _ObjName;
      Model = "Level";
    #include "IO_Clear.h"
	#include "Points.h"
  //pTop = NULL;
  //pBottom = NULL;
  //pValue = NULL;
	}

CLevel::~CLevel()
	{
	}

int CLevel::Init( int Regim )
	{
	//#include "IO_Init.h"
	//#include "Points.h"
  //Char <256> Name;
  //Name.Prt( "{Верх}%s", ObjName );
  //GiveCommData ( Name, CZadv::TypeID, (void**)&pTop ); 
  //Name.Prt( "{Низ}%s", ObjName );
  //GiveCommData ( Name, CZadv::TypeID, (void**)&pBottom ); 
  //Name.Prt( "{Колонка}%s", ObjName );
  //GiveCommData ( Name, CSensor::TypeID, (void**)&pValue ); 
  //char * SensName = ObjName;
  //GiveCommData ( ObjName, CSensor::TypeID, (void**)&pSens );
  //if(pSens)
  //  {
  //  lstrcpy ( pSens->SensorName, ObjName );
	 // pSens->StatusZadv |= ZADV_L_TOP;
	 // pSens->StatusZadv |= ZADV_L_BOTTOM;
  //  }
  return 0;
	}

int CLevel::Step0( )
{
	return 0;
}


int CLevel::Step1( )
	{
 // if ( pBottom )
 //   Vent_Bott = pBottom->Vent = pBottom->VentZ = 100.;
 // if ( pTop )
 //   Vent_Top = pTop->Vent = pTop->VentZ = 100.;
 // if( pSens)
	//{
	//	pSens->StatusZadv |= ZADV_L_TOP;
	//	pSens->StatusZadv |= ZADV_L_BOTTOM;
	//}
	return 0;
	}

int CLevel::StepT ( double dt )
	{
  Calc ( dt );
	return 0;
	}

int CLevel::SetData( int TypeData, void * pData )
	{
	return 1;
	}

int CLevel::SaveState( )
  {
	S_CLASS("W",_W_Level)
  return 0;
  }

int CLevel::RestoreState( char * StrName )
{
	COPY_FIRST
	R_CLASS("W",_W_Level)
	return 1;
}
