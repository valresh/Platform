#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate _9413_DIGIT_0102_MDI( "9413-DIGIT_0102.MDI", SH_9413_DIGIT_0102_MDI::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_9413_DIGIT_0102_MDI,W_9413_DIGIT_0102_MDI,200)

void SH_9413_DIGIT_0102_MDI::InitParm()
{
#include "Blocks/9413-DIGIT_0102.MDI.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/9413-DIGIT_0102.MDI_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class _9413_DIGIT_0102_MDI_IMPL : public W_9413_DIGIT_0102_MDI
{
public:
  void StepT( SStepCalcParams &dt );
};

void SH_9413_DIGIT_0102_MDI::StepT( SStepCalcParams &dt )
{
	SH_Block::StepT( dt );
  _9413_DIGIT_0102_MDI_IMPL *impl = reinterpret_cast<_9413_DIGIT_0102_MDI_IMPL*>(W);
  impl->StepT( dt );
}
//////////////////////////////////////////////////////////////////////////
void _9413_DIGIT_0102_MDI_IMPL::StepT( SStepCalcParams &dt )
{
}
