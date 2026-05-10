#include "stdafx.h"
#define DLL_LS
#include "LS.h"
#include "Err.h"
#include "CommProc.h"


_W_LS::_W_LS()
	{
  memset ( this, 0, sizeof ( _W_LS ));
	}


int CLS::GetParams( char * )
	{
	#include "IO_Parms.h"
	#include "Points.h"
  ACS_Params( "АСУ", this );
//	TAB("Модули",5)
	return 0;
	}

double * Connect( char * Pnt );

int CLS::UpdateParam( CParams & Param )
  {
  if ( Ref[0] )
    {
    Char<256>Name;
    Name = Ref;
    pValue = ::Connect( Name );
    }
  else
    pValue = NULL;
  return 0;
  }

//#include "ShowParams.h"

int CLS::ShowParams( DWORD DataType, struct CShowData * pSD )
  {
	switch ( DataType )
		{
		case sp_Actions:
		return 1;
		}
  return 0;
  }
