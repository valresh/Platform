#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate TEXTCOMMENT( "TEXTCOMMENT", SH_TEXTCOMMENT::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_TEXTCOMMENT,W_TEXTCOMMENT,50)

void SH_TEXTCOMMENT::InitParm()
{
#include "Blocks/TEXTCOMMENT.h" 
s_defFlag = SVarInfo::efParam;
#include "Blocks/TEXTCOMMENT_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class TEXTCOMMENT_IMPL : public W_TEXTCOMMENT
{
public:
  void StepT( SStepCalcParams &dt );
};

void SH_TEXTCOMMENT::StepT( SStepCalcParams &dt )
{
  InputConnectionsTransfer();
  TEXTCOMMENT_IMPL *impl = reinterpret_cast<TEXTCOMMENT_IMPL*>(W);
  impl->StepT( dt );
  OutputConnectionsTransfer();
}
//////////////////////////////////////////////////////////////////////////
void TEXTCOMMENT_IMPL::StepT( SStepCalcParams &dt )
{
}
