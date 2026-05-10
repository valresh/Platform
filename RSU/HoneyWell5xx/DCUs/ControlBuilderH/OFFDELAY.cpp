#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate OFFDELAY( "OFFDELAY", SH_OFFDELAY::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_OFFDELAY,W_OFFDELAY,50)

void SH_OFFDELAY::InitParm()
{
#include "Blocks/OFFDELAY.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/OFFDELAY_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class OFFDELAY_IMPL : public W_OFFDELAY
{
public:
  void StepT( SStepCalcParams &dt );
};

void SH_OFFDELAY::StepT( SStepCalcParams &dt )
{
  InputConnectionsTransfer();
  OFFDELAY_IMPL *impl = reinterpret_cast<OFFDELAY_IMPL*>(W);
  impl->StepT( dt );
  OutputConnectionsTransfer();
}
//////////////////////////////////////////////////////////////////////////
void OFFDELAY_IMPL::StepT( SStepCalcParams &dt )
{
  if( IN )
  {
    OUT = 1;
    STATE = 0;
    TIMER = 0.;
    return;
  }
  // IN = 0
  if( STATE == 0 )
  {
    OUT = 1;
    STATE = 1;
    TIMER = (double)DELAYTIME;
    return;
  }
  if( STATE == 1 )
  {
    TIMER -= dt;
    if ( TIMER >= 0. )
      return;
    TIMER = 0.;
    STATE = 2;
  }
  OUT = 0;
}
