#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate MAX( "MAX", SH_MAX::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_MAX,W_MAX,51)

void SH_MAX::InitParm()
{
#include "Blocks/MAX.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/MAX_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class MAX_IMPL : public W_MAX
{
public:
  void StepT( SStepCalcParams &dt );
};

void SH_MAX::StepT( SStepCalcParams &dt )
{
  InputConnectionsTransfer();
  MAX_IMPL *impl = reinterpret_cast<MAX_IMPL*>(W);
  impl->StepT( dt );
  OutputConnectionsTransfer();
}
//////////////////////////////////////////////////////////////////////////
void MAX_IMPL::StepT( SStepCalcParams &dt )
{
  double r = NaN;
  for( int i=1; i<(NUMOFINPUTS+1); ++i )
  {
    if( IsNaN(IN[i]) )
      continue;
    if( IsNaN(r) )
      r = IN[i];
    r = std::max<double>( r, IN[i] );
  }
  OUT = r;
}
