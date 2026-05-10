#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate DOREF( "DOREF", SH_DOREF::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_DOREF,W_DOREF,52)

SH_DOREF::SH_DOREF() : pIO( NULL )
{
}

void SH_DOREF::InitParm()
{
#include "Blocks/DOREF.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/DOREF_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class DOREF_IMPL : public W_DOREF
{
public:
  void StepT( SStepCalcParams &dt, BYTE *pIO, double &timerForPulse );
};

void SH_DOREF::StepT( SStepCalcParams &dt )
{
  SH_Block::StepT( dt );
  DOREF_IMPL *impl = reinterpret_cast<DOREF_IMPL*>(W);
  impl->StepT( dt, pIO, timerForPulse );
}

static LPCSTR s_ConPoint = "SO";
LPCSTR SH_DOREF::SetDestIO( KBmBase *dest, LPCSTR pszField )
{
  if( this==dest )
    return s_ConPoint;
  eVarType type = evtHZ;
  BYTE *pV = NULL;
  bool b = dest->GetVar( pszField ? pszField : s_ConPoint, &pV, &type );
  ASS( b );
  ASS( type==evtBool );
  pIO = pV;
  return s_ConPoint;
}
//////////////////////////////////////////////////////////////////////////
void DOREF_IMPL::StepT( SStepCalcParams &dt, BYTE *pIO, double &timerForPulse )
{
  if (ONPULSE == 0.)
    timerForPulse = 0.;
  if (ONPULSE > 0.)
  {
    if (timerForPulse > ONPULSE)
    {
      SO = false;
    }
    else
    {
      SO = true;
      timerForPulse += dt;
    }
  }
  if( pIO )
    *pIO = SO;
}
