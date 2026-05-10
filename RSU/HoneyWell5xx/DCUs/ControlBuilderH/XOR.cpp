#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate XOR( "XOR", SH_XOR::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_XOR,W_XOR,52)

void SH_XOR::InitParm()
{
#include "Blocks/XOR.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/XOR_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class XOR_IMPL : public W_XOR
{
public:
  void StepT( SStepCalcParams &dt, int inConsC );
};

void SH_XOR::StepT( SStepCalcParams &dt )
{
  InputConnectionsTransfer();
  XOR_IMPL *impl = reinterpret_cast<XOR_IMPL*>(W);
  impl->StepT( dt, inConsC );
  OutputConnectionsTransfer();
}
//////////////////////////////////////////////////////////////////////////
void XOR_IMPL::StepT( SStepCalcParams &dt, int inConsC )
{
  if( inConsC <= 0 )
    return;
  int nTrue = 0;
  for( int n = 1; n <= inConsC; n++ )
  {
    BYTE Actual_IN = IN[n];
    if( INPTINVSTS[n]>0 )
      Actual_IN = !IN[n];
    if( Actual_IN )
      ++nTrue;
  }
  if( nTrue % 1 )
    OUT = 1;
  else
    OUT = 0;
}
