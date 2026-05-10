#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate EQ( "EQ", SH_EQ::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_EQ,W_EQ,51)

void SH_EQ::InitParm()
{
#include "Blocks/EQ.h" 
s_defFlag = SVarInfo::efParam;
#include "Blocks/EQ_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class EQ_IMPL : public W_EQ
{
public:
  void StepT( SStepCalcParams &dt );
};

void SH_EQ::StepT( SStepCalcParams &dt )
{
  InputConnectionsTransfer();
  EQ_IMPL *impl = reinterpret_cast<EQ_IMPL*>(W);
  impl->StepT( dt );
  OutputConnectionsTransfer();
}
//////////////////////////////////////////////////////////////////////////
void EQ_IMPL::StepT( SStepCalcParams &dt )
{
  double V1 = IN[1];
  double V2 = (1==NUMOFINPUTS) ? TP : IN[2];

  if( IsNaN(V1) || IsNaN(V2) )
  {
    OUT = INBADOPT;
    return;
  }

  if( 1==NUMOFINPUTS )
  {
    if( DEADBAND1 > 0 )
    {
      if( abs(V1 - V2) <= DEADBAND1 )
        OUT = 1;
      else
        OUT = 0;
    }
    else
      OUT = V1 == V2 ? 1 : 0;
    return;
  }

  if( 0==DEADBAND1 && 0==DEADBAND2 )
  {
    OUT = V1 == V2 ? 1 : 0;
    return;
  }

  if( V1 > V2 )
  {
    if( (V1 - DEADBAND1) <= (V2 + DEADBAND2) )
      OUT = 1;
    else
      OUT = 0;
  }
  else
  {
    if( (V2 - DEADBAND2) <= (V1 + DEADBAND1) )
      OUT = 1;
    else
      OUT = 0;
  }
}
