#include <rsuErr.h>
#include <H_Data.h>

static SModuleCreate CEEFB( "CEEFB", S_CEEFB::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(S_CEEFB,W_CEEFB,50)

void S_CEEFB::InitParm()
{
#include "Modules/CEEFB.h" 
  s_defFlag = SVarInfo::efParam;
#include "Modules/CEEFB_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}
