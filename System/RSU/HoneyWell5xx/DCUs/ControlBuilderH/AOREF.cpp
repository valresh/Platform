#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate AOREF( "AOREF", SH_AOREF::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_AOREF,W_AOREF,52)

SH_AOREF::SH_AOREF() : pIO( NULL )
{
}

void SH_AOREF::InitParm()
{
#include "Blocks/AOREF.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/AOREF_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class AOREF_IMPL : public W_AOREF
{
public:
  void StepT( SStepCalcParams &dt, double *pIO );
};

void SH_AOREF::StepT( SStepCalcParams &dt )
{
    SH_Block::StepT( dt );
  AOREF_IMPL *impl = reinterpret_cast<AOREF_IMPL*>(W);
  impl->StepT( dt, pIO );
}

static LPCSTR s_ConPoint = "OP";
LPCSTR SH_AOREF::SetDestIO( KBmBase *dest, LPCSTR pszField )
{
  if( this==dest )
    return s_ConPoint;
  eVarType type = evtHZ;
  BYTE *pV = NULL;
  bool b = dest->GetVar( pszField ? pszField : s_ConPoint, &pV, &type );
  ASS( b );
  ASS( type==evtDouble );
  pIO = (double*)pV;
  return s_ConPoint;
}
//////////////////////////////////////////////////////////////////////////
void AOREF_IMPL::StepT( SStepCalcParams &dt, double *pIO )
{
  if( pIO )
    *pIO = OP;
}
