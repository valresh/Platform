#include <rsuErr.h>
#include <H_Data.h>

static SModuleCreate DISOE( "DISOE", S_DISOE::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(S_DISOE,W_DISOE,205)

void S_DISOE::InitParm()
{
#include "Modules/DISOE.h" 
  s_defFlag = SVarInfo::efParam;
#include "Modules/DISOE_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}
