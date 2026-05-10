#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate LIMIT( "LIMIT", SH_LIMIT::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_LIMIT,W_LIMIT,11)

void SH_LIMIT::InitParm()
{
#include "Blocks/LIMIT.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/LIMIT_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class LIMIT_IMPL : public W_LIMIT
{
public:
  void StepT( SStepCalcParams &dt );
};

void SH_LIMIT::StepT( SStepCalcParams &dt )
{
  InputConnectionsTransfer();
  LIMIT_IMPL *impl = reinterpret_cast<LIMIT_IMPL*>(W);
  impl->StepT( dt );
  OutputConnectionsTransfer();
}
//////////////////////////////////////////////////////////////////////////
void LIMIT_IMPL::StepT( SStepCalcParams &dt )
{
  if( IsNaN(IN) )
  {
    OUT = NaN;
    return;
  }
  OUT = std::min<double>( std::max<double>(IN,MIN), MAX );
}
