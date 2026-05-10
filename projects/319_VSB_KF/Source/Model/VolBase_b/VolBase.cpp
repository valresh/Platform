#include "stdafx.h"
#define DLL_VolBase
#include "VolBase_b.h"
#include "math.h"
#include "Err.h"					 
#include "Connect.h"					 
#include "Control.h"					 
#include "math.h"					 




//static FILE * Tr = NULL;
void CVolBase::VolStep( double dt )
	{
//  if ( strstr ( ObjName, "Е-101" ))
//    KKK();
  if ( Static_Calc_Mix )
    Static( dt );
  else
    Dynamic( dt );
	}



int CVolBase::SaveState ( )
	{
	S_CLASS("W",CVolBase_W)
	return 0;
	}

int CVolBase::RestoreState ( char * StrName )
	{
  COPY_FIRST
  if ( Restore )
    {
	  R_CLASS("W",CVolBase_W)
    }
	return 1;
	}

