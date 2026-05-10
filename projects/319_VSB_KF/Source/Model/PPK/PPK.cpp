#include "stdafx.h"
#include "PPK.h"
#include "Err.h"
//#include "CommProc.h"
//#include "Data.h"
#include "SetData.h"

CPPK::CPPK( char * _ObjName ) :	IBaseModel ( _ObjName ) 
#include "ACS_Constr.h"
#include "ACS.h"
	{
  UNIMODEL
	ObjName = _ObjName;
	#include "IO_Clear.h"
	#include "Points.h"
    #include "PPK_Defects.h"
  OnlyIn = false;
  pFlow_in = NULL;
  DefOpen = false;
  Pin = Pout = 1.0;
	}

CPPK::~CPPK()
	{
	}

int CPPK::Init( int )
	{
	int Res = 0;
	//#include "IO_Init.h"
	//#include "Points.h"
	//#include "ACS_Init.h"  
	//#include "ACS.h"
	return 0;
	}

int CPPK::Step0( )
	{
	return 0;
	}

int CPPK::Step1( )
	{
  CalcK_PPK( );
	return 0;
	}

int CPPK::StepT ( double dt )
	{
  Calc ( dt );
	return 0;
	}

int CPPK::SetData( int TypeData, void * pData )
	{
#undef IF
#define IF(Parm) if ( strcmp ( pP->Name, Parm ) == 0  && pP->Value[0] ) {
#undef EIF
#define EIF return 0; }
  switch ( TypeData )
    {
    case sd_SetEquipParam:
      {
      CEquipParam * pP = (CEquipParam*)pData;
      IF("Pоткр")
        double V = atof( pP->Value );
        if ( V > 0.5 )
          P_PPK = V;
      EIF
      return 1;
      }
    default:
    return 2;
    }
	return 1;
	}

int CPPK::SaveState( )
{
	S_CLASS("W",_W);
	return 0;
}

int CPPK::RestoreState( char * StrName )
{
	COPY_FIRST;
	R_CLASS("W",_W);
	return 1;
}
