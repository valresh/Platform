#include <rsuErr.h>
#include "H_Data.h"

static SModuleCreate AI_LLMUX( "AI-LLMUX", S_AI_LLMUX::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(S_AI_LLMUX,W_AI_LLMUX,50)

void S_AI_LLMUX::InitParm()
{
#include "Modules/AI-LLMUX.h" 
  s_defFlag = SVarInfo::efParam;
#include "Modules/AI-LLMUX_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}
