#include <rsuErr.h>
#include <H_Data.h>

static SModuleCreate Root( "Root", S_ROOT::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(S_ROOT,W_ROOT,50)

void S_ROOT::InitParm()
{
  s_defFlag = SVarInfo::efParam;
#include "Modules/ROOT_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}
