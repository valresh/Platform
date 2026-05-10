#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate SP_AI( "SP_AI", SH_SP_AI::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_SP_AI,W_SP_AI,80)

void SH_SP_AI::InitParm()
{
#include "Blocks/SP_AI.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/SP_AI_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class SP_AI_IMPL : public W_SP_AI
{
public:
  void StepT( SStepCalcParams &dt );
};

void SH_SP_AI::StepT( SStepCalcParams &dt )
{
  InputConnectionsTransfer();
  SP_AI_IMPL *impl = reinterpret_cast<SP_AI_IMPL*>(W);
  impl->StepT( dt );
  OutputConnectionsTransfer();
}
//////////////////////////////////////////////////////////////////////////
void SP_AI_IMPL::StepT( SStepCalcParams &dt )
{
  PV = PVRAW;
}
