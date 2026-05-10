#include <rsuErr.h>
#include <H_Data.h>

static SModuleCreate TC_ODD321( "TC-ODD321", S_TC_ODD321::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(S_TC_ODD321,W_TC_ODD321,360)

void S_TC_ODD321::InitParm()
{
#include "Modules/TC-ODD321.h" 
  s_defFlag = SVarInfo::efParam;
#include "Modules/TC-ODD321_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}
