#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate AOCHANNEL( "AOCHANNEL", SH_AOCHANNEL::Create );

SH_AOCHANNEL::SH_AOCHANNEL()
: pSlave( NULL )
{
}

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_AOCHANNEL,W_AOCHANNEL,60)

void SH_AOCHANNEL::InitParm()
{
#include "Blocks/AOCHANNEL.h" 
s_defFlag = SVarInfo::efParam;
#include "Blocks/AOCHANNEL_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

static LPCSTR s_ConPoint = "OP";
LPCSTR SH_AOCHANNEL::SetDestIO( KBmBase *dest, LPCSTR pszField )
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

void SH_AOCHANNEL::StepT( SStepCalcParams &dt )
{
  InputConnectionsTransfer();
  if( pSlave )
    W->OP = *pSlave;
}
