#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate NOT( "NOT", SH_NOT::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_NOT,W_NOT,5)

void SH_NOT::InitParm()
{
#include "Blocks/NOT.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/NOT_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class NOT_IMPL : public W_NOT
{
public:
  void StepT( SStepCalcParams &dt, int inConsC );
};

void SH_NOT::StepT( SStepCalcParams &dt )
{
  InputConnectionsTransfer();
  NOT_IMPL *impl = reinterpret_cast<NOT_IMPL*>(W);
  impl->StepT( dt, inConsC );
  OutputConnectionsTransfer();
}
//////////////////////////////////////////////////////////////////////////
void NOT_IMPL::StepT( SStepCalcParams &dt, int inConsC )
{
  OUT = !IN;
}
