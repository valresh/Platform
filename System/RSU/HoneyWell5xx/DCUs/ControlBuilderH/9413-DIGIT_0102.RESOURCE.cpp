#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate _9413_DIGIT_0102_RESOURCE( "9413-DIGIT_0102.RESOURCE", SH_9413_DIGIT_0102_RESOURCE::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_9413_DIGIT_0102_RESOURCE,W_9413_DIGIT_0102_RESOURCE,200)

void SH_9413_DIGIT_0102_RESOURCE::InitParm()
{
#include "Blocks/9413-DIGIT_0102.RESOURCE.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/9413-DIGIT_0102.RESOURCE_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class _9413_DIGIT_0102_RESOURCE_IMPL : public W_9413_DIGIT_0102_RESOURCE
{
public:
  void StepT( SStepCalcParams &dt );
};

void SH_9413_DIGIT_0102_RESOURCE::StepT( SStepCalcParams &dt )
{
	SH_Block::StepT( dt );
  _9413_DIGIT_0102_RESOURCE_IMPL *impl = reinterpret_cast<_9413_DIGIT_0102_RESOURCE_IMPL*>(W);
  impl->StepT( dt );
}
//////////////////////////////////////////////////////////////////////////
void _9413_DIGIT_0102_RESOURCE_IMPL::StepT( SStepCalcParams &dt )
{
  ASSD(0);
}
