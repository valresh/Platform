#include <rsuErr.h>
#include <H_Data.h>

static SModuleCreate FFLINK( "FFLINK", S_FFLINK::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(S_FFLINK,W_FFLINK,5)

void S_FFLINK::InitParm()
{
#include "Modules/FFLINK.h" 
  s_defFlag = SVarInfo::efParam;
#include "Modules/FFLINK_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}
