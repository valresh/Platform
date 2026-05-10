#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate MUL( "MUL", SH_MUL::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_MUL,W_MUL,51)

void SH_MUL::InitParm()
{
#include "Blocks/MUL.h" 
s_defFlag = SVarInfo::efParam;
#include "Blocks/MUL_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class MUL_IMPL : public W_MUL
{
public:
  void StepT( SStepCalcParams &dt );
};

void SH_MUL::StepT( SStepCalcParams &dt )
{
  InputConnectionsTransfer();
  MUL_IMPL *impl = reinterpret_cast<MUL_IMPL*>(W);
  impl->StepT( dt );
  OutputConnectionsTransfer();
}
//////////////////////////////////////////////////////////////////////////
void MUL_IMPL::StepT( SStepCalcParams &dt )
{
  OUT = IN[1] * IN[2];
}
