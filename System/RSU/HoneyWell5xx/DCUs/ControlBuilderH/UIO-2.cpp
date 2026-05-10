#include <rsuErr.h>
#include "H_Data.h"

static SModuleCreate UIO_2( "UIO-2", S_UIO_2::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(S_UIO_2,W_UIO_2,890)

void S_UIO_2::InitParm()
{
#include "Modules/UIO-2.h" 
  s_defFlag = SVarInfo::efParam;
#include "Modules/UIO-2_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}
