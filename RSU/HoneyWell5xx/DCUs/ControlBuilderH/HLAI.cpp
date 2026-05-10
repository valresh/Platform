#include <rsuErr.h>
#include <H_Data.h>

static SModuleCreate HLAI( "HLAI", S_HLAI::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(S_HLAI,W_HLAI,200)

void S_HLAI::InitParm()
{
#include "Modules/HLAI.h" 
  s_defFlag = SVarInfo::efParam;
#include "Modules/HLAI_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}
