#include <rsuErr.h>
#include "H_Class.h"
#include <math.h>

static SBlockCreate ABS( "ABS", SH_ABS::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_ABS,W_ABS,51)

void SH_ABS::InitParm()
{
#include "Blocks/ABS.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/ABS_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class ABS_IMPL : public W_ABS
{
public:
  void StepT( SStepCalcParams &dt );
};

void SH_ABS::StepT( SStepCalcParams &dt )
{
  InputConnectionsTransfer();
  ABS_IMPL *impl = reinterpret_cast<ABS_IMPL*>(W);
  impl->StepT( dt );
  OutputConnectionsTransfer();
}
//////////////////////////////////////////////////////////////////////////
void ABS_IMPL::StepT( SStepCalcParams &dt )
{
  OUT = fabs(IN[1]);
}
