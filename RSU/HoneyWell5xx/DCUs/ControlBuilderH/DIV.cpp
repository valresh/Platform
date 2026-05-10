#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate DIV( "DIV", SH_DIV::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_DIV,W_DIV,51)

void SH_DIV::InitParm()
{
#include "Blocks/DIV.h" 
s_defFlag = SVarInfo::efParam;
#include "Blocks/DIV_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class DIV_IMPL : public W_DIV
{
public:
  void StepT( SStepCalcParams &dt );
};

void SH_DIV::StepT( SStepCalcParams &dt )
{
  InputConnectionsTransfer();
  DIV_IMPL *impl = reinterpret_cast<DIV_IMPL*>(W);
  impl->StepT( dt );
  OutputConnectionsTransfer();
}
//////////////////////////////////////////////////////////////////////////
void DIV_IMPL::StepT( SStepCalcParams &dt )
{
  OUT = IN[1] / IN[2];
}
