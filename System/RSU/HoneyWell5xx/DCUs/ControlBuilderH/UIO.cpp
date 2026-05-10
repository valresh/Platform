#include <rsuErr.h>
#include "H_Data.h"

static SModuleCreate UIO( "UIO", S_UIO::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(S_UIO,W_UIO,650)

void S_UIO::InitParm()
{
#include "Modules/UIO.h" 
  s_defFlag = SVarInfo::efParam;
#include "Modules/UIO_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}
