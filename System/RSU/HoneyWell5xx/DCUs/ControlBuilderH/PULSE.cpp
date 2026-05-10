#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate PULSE( "PULSE", SH_PULSE::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_PULSE,W_PULSE,50)

void SH_PULSE::InitParm()
{
#include "Blocks/PULSE.h" 
s_defFlag = SVarInfo::efParam;
#include "Blocks/PULSE_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class PULSE_IMPL : public W_PULSE
{
public:
  void StepT( SStepCalcParams &dt );
};

void SH_PULSE::StepT( SStepCalcParams &dt )
{
  InputConnectionsTransfer();
  PULSE_IMPL *impl = reinterpret_cast<PULSE_IMPL*>(W);
  impl->StepT( dt );
  OutputConnectionsTransfer();
}
//////////////////////////////////////////////////////////////////////////
void PULSE_IMPL::StepT( SStepCalcParams &dt )
{
  if( STATE == 2 )
  {
    OUT = 0;
    if( IN )	// Пока 1 новый импульс не начинается
    {
      return;
    }
    STATE = 0;
  }
  if( STATE == 0 )
  {
    if( IN == 0 )
    {
      OUT = 0;
      return;
    }
    OUT = 1;
    STATE = 1;
    TIMER = (double)PULSEWIDTH;
    return;
  }
  if( STATE == 1 )
  {
    TIMER -= dt;
    if( TIMER >= 0. )
    {
      OUT = 1;
      return;
    }
    TIMER = 0.;
    if( IN == 0 )
      STATE = 0;
    else
      STATE = 2;
    OUT = 0;
  }
}
