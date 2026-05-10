#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate FTRIG( "FTRIG", SH_FTRIG::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_FTRIG,W_FTRIG,3)

void SH_FTRIG::InitParm()
{
#include "Blocks/FTRIG.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/FTRIG_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class FTRIG_IMPL : public W_FTRIG
{
public:
  void StepT( SStepCalcParams &dt );
};

void SH_FTRIG::StepT( SStepCalcParams &dt )
{
  InputConnectionsTransfer();
  FTRIG_IMPL *impl = reinterpret_cast<FTRIG_IMPL*>(W);
  impl->StepT( dt );
  OutputConnectionsTransfer();
}
//////////////////////////////////////////////////////////////////////////
void FTRIG_IMPL::StepT( SStepCalcParams &dt )
{
  if( IN )
    FEDGE = TRUE;
  else
  {
    if( FEDGE )
      OUT = TRUE, FEDGE = FALSE;
    else
      OUT = FALSE ,FEDGE = FALSE;
  }

}
