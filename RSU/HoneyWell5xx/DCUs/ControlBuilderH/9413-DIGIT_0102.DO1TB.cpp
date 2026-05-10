#include "H_Class.h"
#include <rsuErr.h>

static SBlockCreate _9413_DIGIT_0102_DO1TB( "9413-DIGIT_0102.DO1TB", SH_9413_DIGIT_0102_DO1TB::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_9413_DIGIT_0102_DO1TB,W_9413_DIGIT_0102_DO1TB,200)

void SH_9413_DIGIT_0102_DO1TB::InitParm()
{
#include "Blocks/9413-DIGIT_0102.DO1TB.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/9413-DIGIT_0102.DO1TB_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class _9413_DIGIT_0102_DO1TB_IMPL : public W_9413_DIGIT_0102_DO1TB
{
public:
  void StepT( SStepCalcParams &dt );
};

void SH_9413_DIGIT_0102_DO1TB::StepT( SStepCalcParams &dt )
{
	SH_Block::StepT( dt );
  _9413_DIGIT_0102_DO1TB_IMPL *impl = reinterpret_cast<_9413_DIGIT_0102_DO1TB_IMPL*>(W);
  impl->StepT( dt );
}
//////////////////////////////////////////////////////////////////////////
void _9413_DIGIT_0102_DO1TB_IMPL::StepT( SStepCalcParams &dt )
{
  ASSD(0);
}
