#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate RS( "RS", SH_RS::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_RS,W_RS,4)

void SH_RS::InitParm()
{
#include "Blocks/RS.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/RS_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class RS_IMPL : public W_RS
{
public:
  void StepT( SStepCalcParams &dt );
};

void SH_RS::StepT( SStepCalcParams &dt )
{
  InputConnectionsTransfer();
  RS_IMPL *impl = reinterpret_cast<RS_IMPL*>(W);
  impl->StepT( dt );
  OutputConnectionsTransfer();
}
//////////////////////////////////////////////////////////////////////////
void RS_IMPL::StepT( SStepCalcParams &dt )
{
  if( R )
  {
    Q = 0;
    return;
  }
  if( S )
  {
    Q = 1;
    return;
  }
}
