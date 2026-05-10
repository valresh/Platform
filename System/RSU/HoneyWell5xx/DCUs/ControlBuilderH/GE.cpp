#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate GE( "GE", SH_GE::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_GE,W_GE,11)

void SH_GE::InitParm()
{
#include "Blocks/GE.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/GE_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class GE_IMPL : public W_GE
{
public:
  void StepT( SStepCalcParams &dt );
};

void SH_GE::StepT( SStepCalcParams &dt )
{
  InputConnectionsTransfer();
  GE_IMPL *impl = reinterpret_cast<GE_IMPL*>(W);
  impl->StepT( dt );
  OutputConnectionsTransfer();
}
//////////////////////////////////////////////////////////////////////////
void GE_IMPL::StepT( SStepCalcParams &dt )
{
  double V1 = IN[1];
  double V2 = (1==NUMOFINPUTS) ? TP : IN[2];

  if( IsNaN(V1) )
  {
    OUT = INBADOPT;
    return;
  }

  if( 2==NUMOFINPUTS && IsNaN(V2) )
  {
    OUT = INBADOPT;
    return;
  }

  if( V1 >= V2)
    OUT = 1;
  else if( V1 < (V2 - DEADBAND) )
    OUT = 0;

}
