#include <rsuErr.h>
#include <H_Data.h>

static SModuleCreate TC_IDD321( "TC-IDD321", S_TC_IDD321::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(S_TC_IDD321,W_TC_IDD321,380)

void S_TC_IDD321::InitParm()
{
#include "Modules/TC-IDD321.h" 
  s_defFlag = SVarInfo::efParam;
#include "Modules/TC-IDD321_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}
