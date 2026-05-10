#include "stdafx.h"
#include "CtrlConn.h"
#include "Err.h"
#include "CommProc.h"


_W::_W()
	{
  memset ( this, 0, sizeof ( _W ));
	}


int CCtrlConn::GetParams( char * )
	{
	#include "IO_Parms.h"
	#include "Points.h"
  ACS_Params( "АСУ", this );
//	TAB("Модули",5)
	return 0;
	}

int CCtrlConn::UpdateParam( CParams & Param )
  {
  return 0;
  }

//#include "ShowParams.h"

int CCtrlConn::ShowParams( DWORD DataType, struct CShowData * pSD )
  {
	switch ( DataType )
		{
		case sp_Actions:
		return 1;
		}
  return 0;
  }
