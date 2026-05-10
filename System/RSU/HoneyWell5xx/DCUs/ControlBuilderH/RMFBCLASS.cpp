#include <rsuErr.h>
#include <H_Data.h>

static SModuleCreate RMFBCLASS( "RMFBCLASS", S_RMFBCLASS::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(S_RMFBCLASS,W_RMFBCLASS,70)

void S_RMFBCLASS::InitParm()
{
#include "Modules/RMFBCLASS.h" 
  s_defFlag = SVarInfo::efParam;
#include "Modules/RMFBCLASS_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}
