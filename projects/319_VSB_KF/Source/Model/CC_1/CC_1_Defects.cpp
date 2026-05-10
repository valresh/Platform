#include "stdafx.h"
#include "CC_1.h"
#include "Err.h"


ON_DEFECT(CCC_1)
	{
  SET_BP BreakPoint;
  TDefVector::iterator it = defVector.begin();
  for(it = defVector.begin(); it!=defVector.end(); it++)
  {
	  if(pDefect == (*it))
	  {
		  IBaseModel *pModel = Find((*it)->ObjName);
		  if(!pModel)
			  continue;
		  pModel->OnDefect((*it)->pNextDefect);
	  }
  }
  IF_DEFECT(pRecoveryDef, RECOVERY_DEFECT_NAME)
    FlowQ = 0.;
    DefectStop = false;
//     if ( pOborot )
//       pOborot->DefectStop = false;
    RazGerm = false;
  END_IF
  IF_DEFECT(Разгерметизация,"Разгерметизация")
    //DEF_D( def_Omega_Razgerm, "Степень разгерметизации %", 50., 0., 100. )
    //PARM_D( _def_Omega_Razgerm, "def_Omega_Razgerm", 0. )
    RazGerm = true;
    OmegaRazGerm = def_Omega_Razgerm->Dbl * _def_Omega_Razgerm * 0.01;
    FlowQ = 10.;
  END_IF
    IF_DEFECT(Утечка,"Утечка")
    FlowQ = def_Утечки->Dbl * 0.01;
  END_IF
  //IF_DEFECT(Останов,"Останов")
  //  DefectStop = true;
  //if ( pOborot )
  //  pOborot->DefectStop = true;
  //END_IF
	return 0;
  }

