#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate SUB( "SUB", SH_SUB::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_SUB,W_SUB,51)

void SH_SUB::InitParm()
{
#include "Blocks/SUB.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/SUB_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class SUB_IMPL : public W_SUB
{
public:
  void StepT( SStepCalcParams &dt );
};

void SH_SUB::StepT( SStepCalcParams &dt )
{
	SH_Block::StepT( dt );
  SUB_IMPL *impl = reinterpret_cast<SUB_IMPL*>(W);
  impl->StepT( dt );
}
//////////////////////////////////////////////////////////////////////////
void SUB_IMPL::StepT( SStepCalcParams &dt )
{
  OUT = IN[1] - IN[2];
}
