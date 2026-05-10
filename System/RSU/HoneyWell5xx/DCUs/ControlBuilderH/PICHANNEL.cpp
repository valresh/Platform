#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate PICHANNEL( "PICHANNEL", SH_PICHANNEL::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_PICHANNEL,W_PICHANNEL,50)

void SH_PICHANNEL::InitParm()
{
#include "Blocks/PICHANNEL.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/PICHANNEL_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class PICHANNEL_IMPL : public W_PICHANNEL
{
public:
  void StepT( SStepCalcParams &dt );
};

void SH_PICHANNEL::StepT( SStepCalcParams &dt )
{
  InputConnectionsTransfer();
  PICHANNEL_IMPL *impl = reinterpret_cast<PICHANNEL_IMPL*>(W);
  impl->StepT( dt );
  OutputConnectionsTransfer();
}
//////////////////////////////////////////////////////////////////////////
void PICHANNEL_IMPL::StepT( SStepCalcParams &dt )
{
}
