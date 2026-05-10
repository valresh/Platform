#include "stdafx.h"
#include "Y_Data.h"
#include "StdDef.h"

void CY_PID::Setpoint_value_pushback( )
{
  switch( W->mode.M )
  {
  case SMode::AUT:
  case SMode::MAN:
    W->CSV = W->SV;
    break;
  case SMode::RCAS:
    W->SV = W->RSV;
    break;
  default:
    W->SV = W->CSV;
  }
	//if( W->mode.M == SMode::AUT || W->mode.M == SMode::MAN	)
	//{
	//	/*if( W->SV < W->SVL )
	//		W->SV = W->SVL;*/
	//	W->CSV = W->SV;
	//	return;
	//}
 // if( SMode::RCAS==W->mode.M )
 // {
 //   W->SV = W->RSV;
 //   return;
 // }
	//W->SV = W->CSV;
  if( W->mode.M == SMode::CAS )
  {
    if( W->SV < W->SVL )
		  W->SV = W->SVL;
    if( W->SV > W->SVH )
      W->SV = W->SVH;
  }
}

