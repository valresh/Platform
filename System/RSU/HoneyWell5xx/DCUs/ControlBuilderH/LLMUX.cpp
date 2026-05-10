#include <rsuErr.h>
#include <H_Data.h>

static SModuleCreate LLMUX( "LLMUX", S_LLMUX::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(S_LLMUX,W_LLMUX,202)

void S_LLMUX::InitParm()
{
#include "Modules/LLMUX.h" 
  s_defFlag = SVarInfo::efParam;
#include "Modules/LLMUX_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}
