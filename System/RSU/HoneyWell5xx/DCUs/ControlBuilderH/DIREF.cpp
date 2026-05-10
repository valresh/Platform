#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate DIREF( "DIREF", SH_DIREF::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_DIREF,W_DIREF,52)

SH_DIREF::SH_DIREF() : pIO( NULL )
{
}

void SH_DIREF::InitParm()
{
#include "Blocks/DIREF.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/DIREF_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class DIREF_IMPL : public W_DIREF
{
public:
  void StepT( SStepCalcParams &dt, BYTE *pIO );
};

void SH_DIREF::StepT( SStepCalcParams &dt )
{
  SH_Block::StepT( dt );
  DIREF_IMPL *impl = reinterpret_cast<DIREF_IMPL*>(W);
  impl->StepT( dt, pIO );
}

static LPCSTR s_ConPoint = "PV";
LPCSTR SH_DIREF::SetDestIO( KBmBase *dest, LPCSTR pszField )
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
void DIREF_IMPL::StepT( SStepCalcParams &dt, BYTE *pIO )
{
  if( pIO )
  {
    PV = *pIO;
    PVSTS = PVSTS.Normal;
  }
}
