#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate SELREAL( "SELREAL", SH_SELREAL::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_SELREAL,W_SELREAL,51)

void SH_SELREAL::InitParm()
{
#include "Blocks/SELREAL.h" 
s_defFlag = SVarInfo::efParam;
#include "Blocks/SELREAL_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class SELREAL_IMPL : public W_SELREAL
{
public:
  void StepT( SStepCalcParams &dt );
};

void SH_SELREAL::StepT( SStepCalcParams &dt )
{
  InputConnectionsTransfer();
  SELREAL_IMPL *impl = reinterpret_cast<SELREAL_IMPL*>(W);
  impl->StepT( dt );
  OutputConnectionsTransfer();
}
//////////////////////////////////////////////////////////////////////////
void SELREAL_IMPL::StepT( SStepCalcParams &dt )
{
  if( G )
    OUT = IN[2];
  else
    OUT = IN[1];
}
