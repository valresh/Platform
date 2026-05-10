#include <rsuErr.h>
#include <H_Data.h>

static SModuleCreate TC_IAH161( "TC-IAH161", S_TC_IAH161::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(S_TC_IAH161,W_TC_IAH161,90)

void S_TC_IAH161::InitParm()
{
#include "Modules/TC-IAH161.h" 
  s_defFlag = SVarInfo::efParam;
#include "Modules/TC-IAH161_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}
