#include "H_Class.h"
#include <rsuErr.h>

static SBlockCreate ADD( "ADD", SH_ADD::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_ADD,W_ADD,51)

void SH_ADD::InitParm()
{
#include "Blocks/ADD.h" 
s_defFlag = SVarInfo::efParam;
#include "Blocks/ADD_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class ADD_IMPL : public W_ADD
{
public:
  void StepT( SStepCalcParams &dt );
};

void SH_ADD::StepT( SStepCalcParams &dt )
{
  InputConnectionsTransfer();
  ADD_IMPL *impl = reinterpret_cast<ADD_IMPL*>(W);
  impl->StepT( dt );
  OutputConnectionsTransfer();
}
//////////////////////////////////////////////////////////////////////////
void ADD_IMPL::StepT( SStepCalcParams &dt )
{
  if( !NUMOFINPUTS )
    return;
  OUT = 0;
  for( int i=1; i<=NUMOFINPUTS; ++i )
    OUT += IN[i];
}
