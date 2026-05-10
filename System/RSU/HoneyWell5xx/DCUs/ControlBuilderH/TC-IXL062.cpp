#include <rsuErr.h>
#include <H_Data.h>

static SModuleCreate TC_IXL062( "TC-IXL062", S_TC_IXL062::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(S_TC_IXL062,W_TC_IXL062,235)

void S_TC_IXL062::InitParm()
{
#include "Modules/TC-IXL062.h" 
  s_defFlag = SVarInfo::efParam;
#include "Modules/TC-IXL062_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

void S_TC_IXL062::StepT( SStepCalcParams &dt )
{
	SH_Module::StepT( dt );
}
