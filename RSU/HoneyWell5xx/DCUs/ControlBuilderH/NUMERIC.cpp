#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate NUMERIC( "NUMERIC", SH_NUMERIC::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_NUMERIC,W_NUMERIC,50)

void SH_NUMERIC::InitParm()
{
#include "Blocks/NUMERIC.h" 
s_defFlag = SVarInfo::efParam;
#include "Blocks/NUMERIC_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

void SH_NUMERIC::StepT( SStepCalcParams &dt )
{
  InputConnectionsTransfer();

  OutputConnectionsTransfer();
}
