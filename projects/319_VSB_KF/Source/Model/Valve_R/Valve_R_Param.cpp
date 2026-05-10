#include "stdafx.h"
#include "Valve_R.h"
#include "Err.h"
#include "CommProc.h"


_W_Valve_R::_W_Valve_R()
	{
  memset ( this, 0, sizeof ( _W_Valve_R ));
	}


int CValve_R::GetParams( char * )
	{
#define SHOW
	#include "IO_Parms.h"
	#include "Points.h"
#undef SHOW
  Defect_Params(this);
  ACS_Params( this );
	return 0;
	}

int CValve_R::UpdateParam( CParams & Param )
  {
  if ( Param.Addr == &Dy )
    {
    Перепад.SetD( Dy );
//    return 0;
    }
  if ( GasKlap )
    {
    Перепад.SetGasEtalon( pN2, F_gas * iM3, P_gas + 1., dP_gas, T_gas );
    }
  else
    {
    Перепад.SetLiqEtalon( pWater, F_liq * 1000. / Mw_H2O, dP_liq, T_liq );
    }
 return 0;
 }

//#include "ShowParams.h"

int CValve_R::ShowParams( DWORD DataType, struct CShowData * pSD )
  {
/*
	switch ( DataType )
		{
		case sp_Params:
		  GetParams( pSD->N, pSD->Params );
		return 1;
		}
*/
  return 0;
  }
