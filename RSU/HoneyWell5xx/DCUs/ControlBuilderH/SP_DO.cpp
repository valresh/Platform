#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate SP_DO( "SP_DO", SH_SP_DO::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_SP_DO,W_SP_DO,80)

void SH_SP_DO::InitParm()
{
#include "Blocks/SP_DO.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/SP_DO_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class SP_DO_IMPL : public W_SP_DO
{
public:
  void StepT( SStepCalcParams &dt );
};

void SH_SP_DO::StepT( SStepCalcParams &dt )
{
  InputConnectionsTransfer();
  SP_DO_IMPL *impl = reinterpret_cast<SP_DO_IMPL*>(W);
  impl->StepT( dt );
  OutputConnectionsTransfer();
}
//////////////////////////////////////////////////////////////////////////
void SP_DO_IMPL::StepT( SStepCalcParams &dt )
{
}
