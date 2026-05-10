#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate ROR( "ROR", SH_ROR::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_ROR,W_ROR,10)

void SH_ROR::InitParm()
{
#include "Blocks/ROR.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/ROR_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class ROR_IMPL : public W_ROR
{
public:
  void StepT( SStepCalcParams &dt );
};

void SH_ROR::StepT( SStepCalcParams &dt )
{
  InputConnectionsTransfer();
  ROR_IMPL *impl = reinterpret_cast<ROR_IMPL*>(W);
  OutputConnectionsTransfer();
}
//////////////////////////////////////////////////////////////////////////
void ROR_IMPL::StepT( SStepCalcParams &dt )
{
  OUT = IN >> N;
  int mask = 1;
  for( int i=0; i<N; ++i, mask<<1 )
  {
    int right = IN & mask ? 1 : 0;
    int leftBit = 15 - N + i;
    if( right )
      OUT |= 1<<leftBit;
    else
      OUT &= ~(1<<leftBit);
  }
}
