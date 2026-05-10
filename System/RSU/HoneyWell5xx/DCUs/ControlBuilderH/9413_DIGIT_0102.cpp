#include <rsuErr.h>
#include <H_Data.h>

static SModuleCreate x9413_DIGIT_0102( "9413-DIGIT_0102", S_9413_DIGIT_0102::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(S_9413_DIGIT_0102,W_9413_DIGIT_0102,155)

void S_9413_DIGIT_0102::InitParm()
{
#include "Modules/9413-DIGIT_0102.h" 
  s_defFlag = SVarInfo::efParam;
#include "Modules/9413-DIGIT_0102_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}
