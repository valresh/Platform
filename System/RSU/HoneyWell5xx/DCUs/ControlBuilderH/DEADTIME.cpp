#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate DEADTIME( "DEADTIME", SH_DEADTIME::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_DEADTIME,W_DEADTIME,200)

void SH_DEADTIME::InitParm()
{
#include "Blocks/DEADTIME.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/DEADTIME_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class DEADTIME_IMPL : public W_DEADTIME
{
public:
  void StepT( SStepCalcParams &dt );
};

void SH_DEADTIME::StepT( SStepCalcParams &dt )
{
  InputConnectionsTransfer();
  DEADTIME_IMPL *impl = reinterpret_cast<DEADTIME_IMPL*>(W);
  impl->StepT( dt );
  OutputConnectionsTransfer();
}
//////////////////////////////////////////////////////////////////////////
void DEADTIME_IMPL::StepT( SStepCalcParams &dt )
{
  double wait = NaN;
  bool bPvUncertain = false;
  switch( DELAYTYPE.V )
  {
  case _DELAYTYPE::FIXED:
    wait = DELAYTIME;
    DPt = P1;
    if( IsNaN(P1) )
      bPvUncertain = true;
    break;
  case _DELAYTYPE::VARIABLE:
    wait = P2;
    if( IsNaN(P2) )
      bPvUncertain = true;
    else
    {
      if( !IsNaN(CUTOFF.LM) && P2<CUTOFF.LM )
        DPt = 0;
      else
      {
        DPt = P1;
        if( IsNaN(P1) )
          bPvUncertain = true;
      }
      wait = DELAYTIME = (C1/(C2*P2+D2)) + D1;
    }
    break;
  }

  if( IsNaN(wait) )
    PV = NaN;

  if( bPvUncertain )
  {
    PVSTS = _PVSTS::UNCERTN;
    return;
  }

  wait *= 60;
  if( t<=0 )
  {
    int MAX = std::min<int>( NUMLOC, _countof(DELAYTABLE) );
    MAX = std::max<int>( MAX, 1 );
    for( int i=MAX-1; i>0; --i )
    {
      DELAYTABLE[i] = DELAYTABLE[i-1];
    }
    DELAYTABLE[0] = CPV * DPt + DPV;
    t = wait;
  }
  else
    t -= dt;
  PV = DELAYTABLE[0];

  if( IsNaN(PV) )
    PVSTS = _PVSTS::BAD;
  else
    PVSTS = _PVSTS::NORMAL;
}
