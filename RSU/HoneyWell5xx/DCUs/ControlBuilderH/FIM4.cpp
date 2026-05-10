#include <rsuErr.h>
#include <H_Data.h>

static SModuleCreate FIM4( "FIM4", S_FIM4::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(S_FIM4,W_FIM4,10)

void S_FIM4::InitParm()
{
#include "Modules/FIM4.h" 
  s_defFlag = SVarInfo::efParam;
#include "Modules/FIM4_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}
