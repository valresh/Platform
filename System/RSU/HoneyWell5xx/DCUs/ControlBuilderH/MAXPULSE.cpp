#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate MAXPULSE( "MAXPULSE", SH_MAXPULSE::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_MAXPULSE,W_MAXPULSE,10)

void SH_MAXPULSE::InitParm()
{
#include "Blocks/MAXPULSE.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/MAXPULSE_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class MAXPULSE_IMPL : public W_MAXPULSE
{
public:
  void StepT( SStepCalcParams &dt );
};

void SH_MAXPULSE::StepT( SStepCalcParams &dt )
{
    SH_Block::StepT( dt );
  MAXPULSE_IMPL *impl = reinterpret_cast<MAXPULSE_IMPL*>(W);
  impl->StepT( dt );
}
//////////////////////////////////////////////////////////////////////////
void MAXPULSE_IMPL::StepT( SStepCalcParams &dt )
{
  if( IN==0 )
  {
    OUT = 0;
    TIMER = 0;
    return;
  }
  if( TIMER < PULSEWIDTH )
    OUT = 1;
  else
    OUT = 0;
  TIMER += dt;
}
