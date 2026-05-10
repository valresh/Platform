#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate RTRIG( "RTRIG", SH_RTRIG::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_RTRIG,W_RTRIG,3)

void SH_RTRIG::InitParm()
{
#include "Blocks/RTRIG.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/RTRIG_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class RTRIG_IMPL : public W_RTRIG
{
public:
  void StepT( SStepCalcParams &dt );
};

void SH_RTRIG::StepT( SStepCalcParams &dt )
{
  InputConnectionsTransfer();
  RTRIG_IMPL *impl = reinterpret_cast<RTRIG_IMPL*>(W);
  impl->StepT( dt );
  OutputConnectionsTransfer();
}
//////////////////////////////////////////////////////////////////////////
void RTRIG_IMPL::StepT( SStepCalcParams &dt )
{
  if( IN )
  {
    if( !REDGE ) 
      OUT = TRUE, REDGE = TRUE;
    else 
      OUT = FALSE;
  }
  else
    OUT = FALSE, REDGE = IN;

}
