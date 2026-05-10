#include <rsuErr.h>
#include <H_Data.h>

static SModuleCreate AO_HART( "AO-HART", S_xHART_HL::Create );
static SModuleCreate AI_HART( "AI-HART", S_xHART_HL::Create );
static SModuleCreate AI_HL( "AI-HL", S_xHART_HL::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(S_xHART_HL,W_xHART_HL,50)

void S_xHART_HL::InitParm()
{
#include "Modules/xHART_HL.h" 
s_defFlag = SVarInfo::efParam;
#include "Modules/xHART_HL_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}
