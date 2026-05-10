#include "H_Class.h"
#include <rsuErr.h>

static SBlockCreate _9413_DIGIT_0102_AI( "9413-DIGIT_0102.AI", SH_9413_DIGIT_0102_AI::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_9413_DIGIT_0102_AI,W_9413_DIGIT_0102_AI,200)

void SH_9413_DIGIT_0102_AI::InitParm()
{
#include "Blocks/9413-DIGIT_0102.AI.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/9413-DIGIT_0102.AI_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class _9413_DIGIT_0102_AI_IMPL : public W_9413_DIGIT_0102_AI
{
public:
  void StepT( SStepCalcParams &dt );
};

void SH_9413_DIGIT_0102_AI::StepT( SStepCalcParams &dt )
{
  SH_Block::StepT( dt );
  _9413_DIGIT_0102_AI_IMPL *impl = reinterpret_cast<_9413_DIGIT_0102_AI_IMPL*>(W);
  impl->StepT( dt );
}
//////////////////////////////////////////////////////////////////////////
void _9413_DIGIT_0102_AI_IMPL::StepT( SStepCalcParams &dt )
{
  ASSD(0);
}
