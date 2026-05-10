#include "stdafx.h"
#include "Ejector.h"
#include "Err.h"
#include "CommProc.h"


_W::_W()
	{
  memset ( this, 0, sizeof ( _W ));
  P_podsos = 100.;
	}


int CEjector::GetParams( char * )
	{
	#include "IO_Parms.h"
	#include "Points.h"
  Defect_Params(this);
  ACS_Params( "АСУ", this );
//	TAB("Модули",5)
	return 0;
	}

int CEjector::UpdateParam( CParams & Param )
  {
  if ( Param.Addr == &P_out_nom || Param.Addr == &P_in_nom || Param.Addr == &P_cam_min )
    h_max = ( ( P_out_nom + 1. ) - P_cam_min ) / ( ( P_in_nom + 1. ) - P_cam_min );
  return 0;
  }

//#include "ShowParams.h"

int CEjector::ShowParams( DWORD DataType, struct CShowData * pSD )
  {
	switch ( DataType )
		{
		case sp_Actions:
		return 1;
		}
  return 0;
  }
