#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate NOR( "NOR", SH_NOR::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_NOR,W_NOR,52)

void SH_NOR::InitParm()
{
#include "Blocks/NOR.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/NOR_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class NOR_IMPL : public W_NOR
{
public:
  void StepT( SStepCalcParams &dt, int inConsC );
};

void SH_NOR::StepT( SStepCalcParams &dt )
{
  InputConnectionsTransfer();
  NOR_IMPL *impl = reinterpret_cast<NOR_IMPL*>(W);
  impl->StepT( dt, inConsC );
  OutputConnectionsTransfer();
}
//////////////////////////////////////////////////////////////////////////
void NOR_IMPL::StepT( SStepCalcParams &dt, int inConsC )
{
  if( inConsC <= 0 )
    return;
  for( int n = 1; n <= inConsC; n++ )
  {
    BYTE Actual_IN = IN[n];
    if( INPTINVSTS[n] )
      Actual_IN = !IN[n];
    if( Actual_IN )
    {
      OUT = 0;
      return;
    }
  }
  OUT = 1;
}
