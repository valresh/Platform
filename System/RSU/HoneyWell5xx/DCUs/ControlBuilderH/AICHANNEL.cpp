#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate AICHANNEL( "AICHANNEL", SH_AICHANNEL::Create );

SH_AICHANNEL::SH_AICHANNEL()
: pSlave( NULL )
{
}

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_AICHANNEL,W_AICHANNEL,100)

void SH_AICHANNEL::InitParm()
{
#include "Blocks/AICHANNEL.h" 
s_defFlag = SVarInfo::efParam;
#include "Blocks/AICHANNEL_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

static LPCSTR s_ConPoint = "PV";
LPCSTR SH_AICHANNEL::SetDestIO( KBmBase *dest, LPCSTR pszField )
{
  if( this==dest )
    return s_ConPoint;
  eVarType type = evtHZ;
  BYTE *pV = NULL;
  bool b = dest->GetVar( s_ConPoint, &pV, &type );
  ASS( b );
  ASS( type==evtDouble );
  pSlave = (double*)pV;
  return s_ConPoint;
}

void SH_AICHANNEL::StepT( SStepCalcParams &dt )
{
  if( pSlave )
    *pSlave = W->PV;
}
