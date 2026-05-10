#include <rsuErr.h>
#include <H_Data.h>

static SModuleCreate DI24V( "DI24V", S_DI24V::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(S_DI24V,W_DI24V,202)

void S_DI24V::InitParm()
{
#include "Modules/DI24V.h" 
  s_defFlag = SVarInfo::efParam;
#include "Modules/DI24V_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}
