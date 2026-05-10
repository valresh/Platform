#include "stdafx.h"
#define DLL_PressDrop
#include "PressDrop.h"
#include "Err.h"
#include "CommProc.h"


_W_PressDrop::_W_PressDrop()
	{
  memset ( this, 0, sizeof ( _W_PressDrop ));
	}


int CPressDrop::GetParams( char * )
	{
#define SHOW
	#include "IO_Parms.h"
	#include "PressDrop_Pnt.h"
#undef SHOW
	return 0;
	}

int CPressDrop::UpdateParam( CParams & Param )
  {
  if ( Param.Addr == &Dmm )
    {
    D = Dmm * 1e-3;
    S = 0.78539816339744830961566084581988 * D * D; 
    return 0;
    }
  return 0;
  }

void CPressDrop::SetD( double Dmm, double Sz )
  {
  D = Dmm * 1e-3;
  if ( Sz > 0. )
    S = Sz;
  else
    S = 0.78539816339744830961566084581988 * D * D; 
  }

