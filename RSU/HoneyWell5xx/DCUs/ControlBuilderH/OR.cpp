#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate OR( "OR", SH_OR::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_OR,W_OR,52)

void SH_OR::InitParm()
{
#include "Blocks/OR.h" 
s_defFlag = SVarInfo::efParam;
#include "Blocks/OR_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class OR_IMPL : public W_OR
{
public:
  void StepT( SStepCalcParams &dt );
};

void SH_OR::StepT( SStepCalcParams &dt )
{
  InputConnectionsTransfer();
  OR_IMPL *impl = reinterpret_cast<OR_IMPL*>(W);
  impl->StepT( dt );
  OutputConnectionsTransfer();
}
//////////////////////////////////////////////////////////////////////////
void OR_IMPL::StepT( SStepCalcParams &dt )
{
  for( int n = 1; n < _countof(IN); n++ )
  {
    bool b = !INPTINVSTS[n] ? IN[n] : !IN[n];
    if( b )
    {
      OUT = 1;
      return;
    }
  }
  OUT = 0;
}
