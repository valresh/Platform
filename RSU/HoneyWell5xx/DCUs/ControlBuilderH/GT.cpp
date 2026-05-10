#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate GT( "GT", SH_GT::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_GT,W_GT,11)

void SH_GT::InitParm()
{
#include "Blocks/GT.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/GT_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class GT_IMPL : public W_GT
{
public:
  void StepT( SStepCalcParams &dt );
};

void SH_GT::StepT( SStepCalcParams &dt )
{
  InputConnectionsTransfer();
  GT_IMPL *impl = reinterpret_cast<GT_IMPL*>(W);
  impl->StepT( dt );
  OutputConnectionsTransfer();
}
//////////////////////////////////////////////////////////////////////////
void GT_IMPL::StepT( SStepCalcParams &dt )
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

  if( V1 > V2)
    OUT = 1;
  else if( V1 <= (V2 - DEADBAND) )
    OUT = 0;

}
