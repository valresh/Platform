#include "stdafx.h"
#include "Fakel.h"
#include "Err.h"
#include "CommProc.h"


_W::_W()
	{
  memset ( this, 0, sizeof ( _W ));
	}

double CFurnace::Sigma = 0.;
double CFurnace::kkO2 = 0.15;


int CFakel::GetParams( char * )
	{
	#include "IO_Parms.h"
	#include "Points.h"
  ACS_Params( "АСУ", this );
	return 0;
	}

int CFurnace::GetParams( char * )
  {
#include "IO_Parms.h"
  PARM( IsFire, "#Горит" );
  PARM( T_fire, "#Температура пламени" );
  PARM( Sigma, "Излучение" );
  PARM( kkO2, "kO2" )
  ACS_Params( "АСУ", this );
//  TAB("Модули",5)
  return 0;
  }

int CFakel::UpdateParam( CParams & Param )
  {
  return 0;
  }

