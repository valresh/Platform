#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate ONDELAY( "ONDELAY", SH_ONDELAY::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_ONDELAY,W_ONDELAY,50)

void SH_ONDELAY::InitParm()
{
#include "Blocks/ONDELAY.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/ONDELAY_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class ONDELAY_IMPL : public W_ONDELAY
{
public:
  void StepT( SStepCalcParams &dt );
};

void SH_ONDELAY::StepT( SStepCalcParams &dt )
{
  InputConnectionsTransfer();
  ONDELAY_IMPL *impl = reinterpret_cast<ONDELAY_IMPL*>(W);
  impl->StepT( dt );
  OutputConnectionsTransfer();
}
//////////////////////////////////////////////////////////////////////////
void ONDELAY_IMPL::StepT( SStepCalcParams &dt )
{
  if ( IN == 0 )
  {
    OUT = 0;
    STATE = 0;
    TIMER = 0.;
    return;
  }
  // IN = 1
  if ( STATE == 0 )
  {
    OUT = 0;
    STATE = 1;
    TIMER = (double)DELAYTIME;
    return;
  }
  if ( STATE == 1 )
  {
    TIMER -= dt;
    if ( TIMER >= 0. )
      return;
    TIMER = 0.;
    STATE = 2;
  }
  OUT = 1;
}
