#include "stdafx.h"
#include "H_Class.h"

static SBlockCreate _9413_DIGIT_0102_MDO( "9413-DIGIT_0102.MDO", SH_9413_DIGIT_0102_MDO::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_9413_DIGIT_0102_MDO,W_9413_DIGIT_0102_MDO,200)

void SH_9413_DIGIT_0102_MDO::InitParm()
{
#include "Blocks/9413-DIGIT_0102.MDO.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/9413-DIGIT_0102.MDO_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class _9413_DIGIT_0102_MDO_IMPL : public W_9413_DIGIT_0102_MDO
{
public:
  void StepT( SStepCalcParams &dt );
};

void SH_9413_DIGIT_0102_MDO::StepT( SStepCalcParams &dt )
{
  __super::StepT( dt );
  _9413_DIGIT_0102_MDO_IMPL *impl = reinterpret_cast<_9413_DIGIT_0102_MDO_IMPL*>(W);
  impl->StepT( dt );
}
//////////////////////////////////////////////////////////////////////////
void _9413_DIGIT_0102_MDO_IMPL::StepT( SStepCalcParams &dt )
{
  ASSD(0);
}
