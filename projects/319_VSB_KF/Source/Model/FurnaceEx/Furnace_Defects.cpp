#include "stdafx.h"
#include "Furnace.h"
#include "Err.h"

ON_DEFECT(CFurnace)
	{
  SET_BP BreakPoint;
  IF_DEFECT(Восстановление,"Восстановление")		//Возможно нужно добавть другие дефекты в Восстановление
	  FlowQ = 0.;
  for ( int n = 0; n < kFors; n++ )
  {
	  pFire[n]->IsCondensat = false;
	  pFire[n]->kCondensat = 1.0;
  }
  END_IF
  IF_DEFECT(Прогар_змеевика,"Прогар змеевика")
    for ( int n = 0; n < kProduct; n++ )
      {
      pTube[n]->O_Прогар = 0.; 
      }
    nProgar = (int)(kProduct * r01());
	  double A = def_Omega->Dbl * 0.001;
    double P = pTube[nProgar]->pFlow_in->P - 1.;
    if ( P < 0.1 )
      return 1;
    double F = pTube[nProgar]->pFlow_in->Flow_mol;
    if ( F < 1. )
      return 1;
    pTube[nProgar]->O_Прогар = fabs ( F * A / P );
    IsProgar = true;
  END_IF
  IF_DEFECT(Погасание_горелок,"Погасание горелок")
  for ( int n = 0; n < kFors; n++ )
    {
	  CForsunka * pFors = pFire[n]->Logic.pFors;
    pFors->isFire = false;
    pFors -> Off_State ( CForsunka::IS_ZAPAL_FIRE );
    pFors -> Off_State ( CForsunka::IS_WORK_FIRE );
    }
  END_IF
  IF_DEFECT(Попадание_конденсата, "Попадание конденсата")
  for ( int n = 0; n < kFors; n++ )
    {
    pFire[n]->IsCondensat = true;
    pFire[n]->kCondensat = kCondensat;
    }
  END_IF
  IF_DEFECT(Загазован,"Загазованность")
	  FlowQ = def_Загазован_3->Dbl * 0.01;
  END_IF
	return 0;
  }

