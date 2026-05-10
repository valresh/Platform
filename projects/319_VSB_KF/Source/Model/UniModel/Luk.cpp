#include "stdafx.h"
#include "UniModel.h"
#include "Err.h"
#include "SetData.h"


#if 0
void CUniModel::Luk( double dt )
  {
  if ( Откр.On())
    Luk_Move = 1;
  if ( Закр.On())
    Luk_Move = -1;
  if ( Luk_Move > 0 )
    {
    Luk_Open += dt * 3600. / Tau_Open;
    if ( Luk_Open > 100. )
      {
      Luk_Move = 0;
      Luk_Open = 100.;
      }
    }
  if ( Luk_Move < 0 )
    {
    Luk_Open -= dt * 3600. / Tau_Open;
    if ( Luk_Open < 0. )
      {
      Luk_Move = 0;
      Luk_Open = 0.;
      }
    }
  if ( Luk_Open < 1. )
    Конц_закр = 1;
  else
    Конц_закр = 0;
  if ( Luk_Open > 99. )
    Конц_откр = 1;
  else
    Конц_откр = 0;
  }
#endif