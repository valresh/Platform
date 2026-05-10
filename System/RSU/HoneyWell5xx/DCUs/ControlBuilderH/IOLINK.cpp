#include <rsuErr.h>
#include <H_Data.h>

static SModuleCreate IOLINK( "IOLINK", S_IOLINK::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(S_IOLINK,W_IOLINK,50)

void S_IOLINK::InitParm()
{
#include "Modules/IOLINK.h" 
s_defFlag = SVarInfo::efParam;
#include "Modules/IOLINK_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}
