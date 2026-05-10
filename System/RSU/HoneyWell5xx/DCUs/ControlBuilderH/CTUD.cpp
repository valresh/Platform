#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate CTUD( "CTUD", SH_CTUD::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_CTUD,W_CTUD,50)

void SH_CTUD::InitParm()
{
#include "Blocks/CTUD.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/CTUD_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class CTUD_IMPL : public W_CTUD
{
public:
  void StepT( SStepCalcParams &dt, BYTE prevCNTDNFL, BYTE prevCNTUPFL );
};

void SH_CTUD::StepT( SStepCalcParams &dt )
{
  BYTE prevCNTDNFL = W->CNTDNFL, prevCNTUPFL = W->CNTUPFL;
  InputConnectionsTransfer();
  CTUD_IMPL *impl = reinterpret_cast<CTUD_IMPL*>(W);
  impl->StepT( dt, prevCNTDNFL, prevCNTUPFL );
  OutputConnectionsTransfer();
}
//////////////////////////////////////////////////////////////////////////
void CTUD_IMPL::StepT( SStepCalcParams &dt, BYTE prevCNTDNFL, BYTE prevCNTUPFL )
{
  if( PAUSEFL )
  {
    PAUSEFL = 0;
    return;
  }
  if( LOADFL )
  {
    LOADFL = 0;
    if( !SELINT32FL )
    {
      OUTFLOAT64 = INFLOAT64;
      OUTINT32 = INFLOAT64;
    }
    else
    {
      OUTFLOAT64 = ININT32;
      OUTINT32 = ININT32;
    }
  }
  if( RESETFL )
  {
    RESETFL = 0;
    OUTFLOAT64 = 0;
    OUTINT32 = 0;
  }

  if( !CNTLVLFL )
  {
    if( !prevCNTDNFL && CNTDNFL)
    {
      OUTINT32 -= 1;
      OUTFLOAT64 -= 1.;
    }
    if( !prevCNTUPFL && CNTUPFL)
    {
      OUTINT32 += 1;
      OUTFLOAT64 += 1.f;
    }
  }
  else
  {
    if( CNTDNFL)
    {
      OUTINT32 -= 1;
      OUTFLOAT64 -= 1.;
    }
    if( CNTUPFL)
    {
      OUTINT32 += 1;
      OUTFLOAT64 += 1.f;
    }
  }
}
