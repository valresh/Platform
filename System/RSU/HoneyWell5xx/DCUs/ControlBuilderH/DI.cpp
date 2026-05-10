#include <rsuErr.h>
#include <H_Data.h>

static SModuleCreate DI( "DI", S_DI::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(S_DI,W_DI,202)

void S_DI::InitParm()
{
#include "Modules/DI.h" 
  s_defFlag = SVarInfo::efParam;
#include "Modules/DI_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}
