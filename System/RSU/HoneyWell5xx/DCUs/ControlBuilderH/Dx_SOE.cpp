#include <rsuErr.h>
#include <H_Data.h>

static SModuleCreate DI_SOE( "DI-SOE", S_Dx_SOE::Create );
static SModuleCreate DO_SOE( "DO-SOE", S_Dx_SOE::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(S_Dx_SOE,W_Dx_SOE,190)

void S_Dx_SOE::InitParm()
{
#include "Modules/Dx_SOE.h" 
  s_defFlag = SVarInfo::efParam;
#include "Modules/Dx_SOE_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}
