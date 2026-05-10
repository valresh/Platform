#include "stdafx.h"
#include "Zadv3x.h"
#include "Err.h"
#include "CommProc.h"


void CZadv3x::Calc( double dt )
	{
  SET_BP BreakPoint;
  if ( pFlow[0] == NULL )
    return;
  if(ПоложениеЗадание.Use())
  {	  
	  //if(pZadv->Vent != pZadv->VentZ)
	  //{
		 // if(Inverse)
			//  ПоложениеЗадание = 100.0 - pZadv->VentZ;
		 // else 
			//  ПоложениеЗадание = pZadv->VentZ;
	  //}

	  //else
	  //{
		 // if(Inverse)
			//  pZadv->VentZ = 100.0 - ПоложениеЗадание;
		 // else
			//  pZadv->VentZ = ПоложениеЗадание;
	  //}
  }
  //Vent = pZadv->Vent = pZadv->VentZ;
  //
  double Vc;
  if ( Inverse )
	  Vc = 100. - Vent;
  else
	  Vc = Vent;
  double Угол_Закрытия_100 = 100. - Угол_Закрытия;
  if ( Vc > Угол_Закрытия )
	  O01 = Omega * ( Vc - Угол_Закрытия ) / Угол_Закрытия_100; 
  else
	  O01 = 0.; 
  if ( Vc < 100. - Угол_Закрытия )
	  O02 = Omega * ( Угол_Закрытия_100 - Vc ) / Угол_Закрытия_100; 
  else
	  O02 = 0.; 
  //
  Flow_0 = pFlow[0]->Flow_mol;
  Flow_1 = pFlow[1]->Flow_mol;
  Flow_2 = pFlow[2]->Flow_mol;
  double F_max = 0.;
  int nMax = -1;
  for ( int n = 0; n < 3; n++ )
    {
    double F = pFlow[n]->Flow_mol;
    if ( F >= F_max )
      {
      F_max = F;
      nMax = n;
      }
    }
  if ( nMax < 0 )
    return;
  CFlow * pFlowMax = pFlow[nMax];
  for ( int n = 0; n < 3; n++ )
    {
    if ( n != nMax )
      *pFlow[n] = *pFlowMax;
    }
	}
