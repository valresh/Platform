#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate FLAGARRAY( "FLAGARRAY", SH_FLAGARRAY::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_FLAGARRAY,W_FLAGARRAY,1010)

void SH_FLAGARRAY::InitParm()
{
#include "Blocks/FLAGARRAY.h" 
s_defFlag = SVarInfo::efParam;
#include "Blocks/FLAGARRAY_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class FLAGARRAY_IMPL : public W_FLAGARRAY
{
public:
  void StepT( SStepCalcParams &dt );
};

void SH_FLAGARRAY::StepT( SStepCalcParams &dt )
{
  InputConnectionsTransfer();
  FLAGARRAY_IMPL *impl = reinterpret_cast<FLAGARRAY_IMPL*>(W);
  impl->StepT( dt );
}

void FLAGARRAY_IMPL::StepT( SStepCalcParams &dt )
{
}
