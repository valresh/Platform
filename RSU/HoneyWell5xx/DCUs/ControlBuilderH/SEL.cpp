#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate SEL( "SEL", SH_SEL::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_SEL,W_SEL,12)

void SH_SEL::InitParm()
{
#include "Blocks/SEL.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/SEL_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class SEL_IMPL : public W_SEL
{
public:
  void StepT( SStepCalcParams &dt );
};

void SH_SEL::StepT( SStepCalcParams &dt )
{
  InputConnectionsTransfer();
  SEL_IMPL *impl = reinterpret_cast<SEL_IMPL*>(W);
  impl->StepT( dt );
  OutputConnectionsTransfer();
}
//////////////////////////////////////////////////////////////////////////
void SEL_IMPL::StepT( SStepCalcParams &dt )
{
  if( G )
  {
    if( INPTINVSTS[2] )
      OUT = !IN[2];
    else
      OUT = IN[2];
  }
  else
  {
    if( INPTINVSTS[1] )
      OUT = !IN[1];
    else
      OUT = IN[1];
  }
}
