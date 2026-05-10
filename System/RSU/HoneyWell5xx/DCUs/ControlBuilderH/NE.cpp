#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate NE( "NE", SH_NE::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_NE,W_NE,51)

void SH_NE::InitParm()
{
#include "Blocks/NE.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/NE_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class NE_IMPL : public W_NE
{
public:
  void StepT( SStepCalcParams &dt, bool bHaveIn2 );
};

void SH_NE::StepT( SStepCalcParams &dt )
{
  InputConnectionsTransfer();
  NE_IMPL *impl = reinterpret_cast<NE_IMPL*>(W);
  bool bHaveIn2 = false;
  if( impl->NUMOFINPUTS>1 )
  {
    for( size_t i=0; i<inConsC; ++i )
    {
      if( !strcmp( pInConns[i].szInFld, "IN[2]") )
      {
        bHaveIn2 = true;
        break;
      }
    }
  }
  impl->StepT( dt, bHaveIn2 );
  OutputConnectionsTransfer();
}
//////////////////////////////////////////////////////////////////////////
void NE_IMPL::StepT( SStepCalcParams &dt, bool bHaveIn2 )
{
  double V1 = IN[1];
  double V2 = TP;
  if( NUMOFINPUTS>1 )
  {
    if( bHaveIn2 )
      V2 = IN[2];
    else
      NUMOFINPUTS = 3;
  }

  if( IsNaN(V1) || IsNaN(V2) )
  {
    OUT = INBADOPT;
    return;
  }

  if( 0 <= DEADBAND1 && DEADBAND1 <= DEADBAND2  )
  {
    if( abs(V1 - V2) <= DEADBAND1 )
      OUT = false;
    else if( abs(V1 - V2) > DEADBAND2 )
      OUT = true;
  }
  else
    OUT = INBADOPT;
}
