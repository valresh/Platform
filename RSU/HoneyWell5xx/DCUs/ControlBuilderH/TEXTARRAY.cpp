#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate TEXTARRAY( "TEXTARRAY", SH_TEXTARRAY::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_TEXTARRAY,W_TEXTARRAY,127)

void SH_TEXTARRAY::InitParm()
{
#include "Blocks/TEXTARRAY.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/TEXTARRAY_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

void SH_TEXTARRAY::StepT( SStepCalcParams &dt )
{
}
