#include "stdafx.h"
#define DLL_VolBase
#include "VolBase_b.h"
#include "math.h"
#include "Err.h"					 

void CVolBase::CalcEps_1F( int Conn, double H_io )
  {
  // Однофазная емкость
  Eps[2] = 0.;
  if ( NoFunnel && H_io == 0. )
    {
    if ( Lev_liq > 0. )
      {
      Eps[0] = 0.;
      Eps[1] = 1.;
      }
    else
      {
      Eps[0] = 1.;
      Eps[1] = 0.;
      }
    return;
    }
  if ( Conn == 2 )
    H_io = 1.;
  if ( Conn == 3 )
    H_io = 0.;
  // Eps на выходе емкости 
  double H_t, H_b; 
  if ( H_io <= 0.5 )
    {
    H_b	= H_io;
    H_t = H_b + D_trub;
    }
  else
    {
    H_t = H_io;
    H_b	= H_t - D_trub;
    }
  if ( Lev_liq < H_b )
    {
    Eps[0] = 1.;
    Eps[1] = 0.;
    return;
    }
  if ( Lev_liq > H_t )
    {
    Eps[0] = 0.;
    Eps[1] = 1.;
    return;
    }
  Eps[1] = ( Lev_liq - H_b ) / ( H_t - H_b );
  Eps[0] = 1. - Eps[1];
  }

void CVolBase::CalcEps_0(  int Conn, double H_io )
  {
  // Eps на выходе емкости 
  if ( NoFunnel && H_io == 0. )
    {
    if ( Lev_liq > 0. )
      {
      if ( Lev_liq_2in1 > 0. )
        {
        Eps[0] = 0.;
        Eps[1] = 0.;
        Eps[2] = 1.;
        return;
        }
      Eps[0] = 0.;
      Eps[1] = 1.;
      Eps[2] = 0.;
      return;
      }
    else
      {
      Eps[0] = 1.;
      Eps[1] = 0.;
      Eps[2] = 0.;
      }
    return;
    }
  if ( Conn == 2 )
    H_io = 1.;
  if ( Conn == 3 )
    H_io = 0.;
  double H_t, H_b; 
  if ( H_io <= 0.5 )
    {
    H_b	= H_io;
    H_t = H_b + D_trub;
    }
  else
    {
    H_t = H_io;
    H_b	= H_t - D_trub;
    }
  if ( OverFlow && H_io > 0.95 )
    {
    H_t = Lev_liq;
    H_b	= H_t - D_trub;
    }
  // Lev_liq - общий, Lev_liq_1 - слой 1, Lev_liq_2in1 - слой 2
  if ( Lev_liq < H_b )
    {
    Eps[0] = 1.;
    Eps[1] = 0.;
    Eps[2] = 0.;
    return;
    }
  if ( Lev_liq_2in1 > H_t )
    {
    Eps[0] = 0.;
    Eps[1] = 0.;
    Eps[2] = 1.;
    return;
    }
  if ( Lev_liq_2in1 < H_b )
    {
    // 0, 1
    Eps[2] = 0.;
    if ( Lev_liq > H_t )
      {
      Eps[0] = 0.;
      Eps[1] = 1.;
      return;
      }
    else
      {
      Eps[1] = ( Lev_liq - H_b ) / D_trub;
      Eps[0] = 1. - Eps[1];
      return;
      }
    }
  else
    {
    // 0, 1, 2
    Eps[2] = ( Lev_liq_2in1 - H_b ) / D_trub;
    if ( Lev_liq > H_t )
      {
      Eps[0] = 0.;
      Eps[1] = 1. - Eps[2];
      return;
      }
    Eps[1] = ( Lev_liq - Lev_liq_2in1 - H_b ) / D_trub;
    Eps[0] = 1. - Eps[1] - Eps[2];
    }
  }
void CVolBase::CalcEps_1(  int Conn, double H_io )
  {
  // Eps на выходе отстойника 
  if ( Conn == 2 )
    H_io = 1.;
  if ( Conn == 3 )
    H_io = 0.;
  double H_t, H_b; 
  if ( H_io <= 0.5 )
    {
    H_b	= H_io;
    H_t = H_b + D_trub;
    }
  else
    {
    H_t = H_io;
    H_b	= H_t - D_trub;
    }
  //  Lev_liq_2 - слой 1, Lev_liq_1in2 - слой 1
  double Lev_summ = Lev_liq_2 + Lev_liq_1in2;
  if ( Lev_summ < H_b )
    {
    Eps[0] = 1.;
    Eps[1] = 0.;
    Eps[2] = 0.;
    return;
    }
  if ( Lev_liq_2 > H_t )
    {
    Eps[0] = 0.;
    Eps[1] = 0.;
    Eps[2] = 1.;
    return;
    }
  if ( Lev_liq_2 < H_b )
    {
    // 0, 1
    Eps[2] = 0.;
    if ( Lev_summ > H_t )
      {
      Eps[0] = 0.;
      Eps[1] = 1.;
      return;
      }
    else
      {
      Eps[1] = ( Lev_summ - H_b ) / D_trub;
      Eps[0] = 1. - Eps[1];
      return;
      }
    }
  else
    {
    // 0, 1, 2
    Eps[2] = ( Lev_liq_2 - H_b ) / D_trub;
    if ( Lev_summ > H_t )
      {
      Eps[0] = 0.;
      Eps[1] = 1. - Eps[2];
      return;
      }
    Eps[1] = Lev_liq_1in2 / D_trub;
    Eps[0] = 1. - Eps[1] - Eps[2];
    }
  }
