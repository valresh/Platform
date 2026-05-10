#include "stdafx.h"
#include "Pipe.h"
#include "Err.h"

DWORD CPipe::ClassRootNode = 0;
int CPipe::OnDefect( CDef * pDefect )
	{
  SET_BP
	  LPCSTR szPipe = "Труба";
  IF_DEFECT(pRecoveryDef, RECOVERY_DEFECT_NAME)
	  F_Razgerm_max = 0.;  
	  Razgerm = false;
	  Omega_Razgerm = 0.;
	  FlowQ = 0.;
      // char szMessage[150];
      // sprintf_s ( szMessage, sizeof(szMessage),  "%s,%s,%s", szPipe, pDefect->ObjName, pDefect->Name );
      // if ( pMessagingUser )
         //  (*pMessagingUser)( szMessage, RGB( 255, 0, 0 ),  RGB( 255, 255, 255 ) );
  END_IF
  IF_DEFECT(Разгерметизация,"Разгерметизация")
    if ( P < 1.1 )
      return 1;
    Razgerm = true;
    double F = def_Omega->Dbl * 0.1 * fabs ( F_mol );
		if ( def_Omega->Dbl == 0. || F < 1. )
			Omega_Razgerm = Omega_Sliv;
		else
			Omega_Razgerm = F / ( P - 1. );
    FlowQ = Q_max;
	F_Razgerm_max = 0.;
    // char szMessage[150];
    // sprintf_s ( szMessage, sizeof(szMessage),  "%s,%s,%s", szPipe, pDefect->ObjName, pDefect->Name );
    // if ( pMessagingUser )
    // 	(*pMessagingUser)( szMessage, RGB( 255, 0, 0 ),  RGB( 255, 255, 255 ) );
  END_IF
	return 0;
  }

