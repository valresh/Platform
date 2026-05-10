#include <rsuErr.h>
#include "H_Class.h"
#include <math.h>

static SBlockCreate REGSUMMER( "REGSUMMER", SH_REGSUMMER::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_REGSUMMER,W_REGSUMMER,285)

void SH_REGSUMMER::InitParm()
{
#include "Blocks/REGSUMMER.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/REGSUMMER_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class REGSUMMER_IMPL : public W_REGSUMMER
{
public:
  void StepT( SStepCalcParams &dt, bool bInputs[5] );
protected:
  void Modefl();
};

void SH_REGSUMMER::StepT( SStepCalcParams &dt )
{
  bool bInputs[] = { false, false, false, false, false };
  InputConnectionsTransfer();

  for( size_t i=0; i<inConsC; ++i )
  {
    if( 'X'!=pInConns[i].szInFld[0] || '['!=pInConns[i].szInFld[1] )
      continue;
    int index = pInConns[i].szInFld[2] - '0';
    bInputs[index] = true;
    if( !('O'==pInConns[i].szOutFld[0] && 'P'==pInConns[i].szOutFld[1]) )
      continue;
    if( !('['==pInConns[i].szOutFld[2] || 0==pInConns[i].szOutFld[2]) )
      continue;
    W->X[index] = (W->XEUHI-W->XEULO) * 0.01 * W->X[index] + W->XEULO;
  }

  REGSUMMER_IMPL *impl = reinterpret_cast<REGSUMMER_IMPL*>(W);
  impl->StepT( dt, bInputs );
  OutputConnectionsTransfer();
}

void SH_REGSUMMER::StepAfterRestoreState()
{
  W->HIALM.PR = __ALPRIOR::None;
  W->HIALM.TYPE = __DACALMTYPE::None;
  W->HIALM.SV = 0;
}
//////////////////////////////////////////////////////////////////////////
void REGSUMMER_IMPL::StepT( SStepCalcParams &dt, bool bInputs[5] )
{
  if( SIFL )
  {
    switch( SIOPT.V )
    {
    case SIOPT.NO_SHED:
      break;
    case SIOPT.SHEDHOLD:
      MODE.V = MODE.MAN;
      MODEATTR = MODEATTR.OPERATOR;
      break;
    case SIOPT.SHEDLOW:
      MODE.V = MODE.MAN;
      MODEATTR = MODEATTR.OPERATOR;
      OP = OPEXLOLM;
      break;
    case SIOPT.SHEDHIGH:
      MODE.V = MODE.MAN;
      MODEATTR = MODEATTR.OPERATOR;
      OP = OPEXHILM;
      break;
    case SIOPT.SHEDSAFE:
      MODE.V = MODE.MAN;
      MODEATTR = MODEATTR.OPERATOR;
      if ( finite( SAFEOP ))
        OP = SAFEOP;
      break;
    }
  }
  else
  {
    if( ESWENB.ENABLE==ESWENB.V )
    {
      if( ESWFL.AUTO )
        MODE.V = MODE.AUTO;
      else if( ESWFL.BCAS )
        MODE.V = MODE.BCAS;
      else if( ESWFL.CAS )
        MODE.V = MODE.CAS;
      else if( ESWFL.MAN )
        MODE.V = MODE.MAN;
    }
  }

  Modefl();

  double x = 0;
  int nInput = 0;
  for( int i=0; i<_countof(X); ++i )
  {
    if( !bInputs[i] )
      continue;
    if( IsNaN(X[i]) )
      continue;
    nInput |= 1<<i;
    x += XK[i] * X[i];
  }
  if( nInput<2 )
  {
    CV = OP = NaN;
    return;
  }
  if( 2==nInput )
    x = X[1];

  if( MODE.MAN!=MODE.V )
  {
    CV = K * x + OPBIAS.FIX + OPBIAS.FLOAT;
    OP = ( CV - CVEULO ) / ( CVEUHI - CVEULO ) * 100.;
  }

  if( OP < OPLOLM )
    OP = OPLOLM;
  if( OP > OPHILM )
    OP = OPHILM;

  switch( OUTIND.V )
  {
  case _OUTIND::Direct:
    break;
  case _OUTIND::Reverse:
    OP = 100 - OP;
    break;
  }
  double A = OP * 0.01;
  OPEU = ( 1. - A ) * CVEULO + A * CVEUHI;
}

void REGSUMMER_IMPL::Modefl()
{
  MODEFL.MAN = MODE.V == MODE.MAN;
  MODEFL.AUTO = MODE.V == MODE.AUTO;
  MODEFL.CAS = MODE.V == MODE.CAS;
  MODEATTRFL.OPER = MODEATTR.V == MODEATTR.OPERATOR;
  MODEATTRFL.PROG = MODEATTR.V == MODEATTR.PROGRAM;
  MODEFL.NORM = MODE.V == NORMMODE.V;
  MODEATTRFL.NORM = MODEATTR.V == NORMMODEATTR.V;
}
