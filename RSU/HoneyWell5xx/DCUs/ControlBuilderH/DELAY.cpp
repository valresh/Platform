#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate DELAY( "DELAY", SH_DELAY::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_DELAY,W_DELAY,50)

void SH_DELAY::InitParm()
{
#include "Blocks/DELAY.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/DELAY_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class DELAY_IMPL : public W_DELAY
{
public:
  void StepT( SStepCalcParams &dt );
};

void SH_DELAY::StepT( SStepCalcParams &dt )
{
  InputConnectionsTransfer();
  DELAY_IMPL *impl = reinterpret_cast<DELAY_IMPL*>(W);
  impl->StepT( dt );
  OutputConnectionsTransfer();
}
//////////////////////////////////////////////////////////////////////////
void DELAY_IMPL::StepT( SStepCalcParams &dt )
{
  OUT = STATE;
  STATE = IN;
}
