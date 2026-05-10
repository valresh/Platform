#include <rsuErr.h>
#include "H_Class.h"
#include <math.h>

static SBlockCreate RATIOCTL( "RATIOCTL", SH_RATIOCTL::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_RATIOCTL,W_RATIOCTL,300)

void SH_RATIOCTL::InitParm()
{
#include "Blocks/RATIOCTL.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/RATIOCTL_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class RATIOCTL_IMPL : public W_RATIOCTL
{
public:
  void StepT( SStepCalcParams &dt, bool bHaveSpConnection );
protected:
  void Modefl();
  void Обработка_SP( bool bHaveSpConnection );
  void Отслеживание_PV();
  void CalcCV();
  void Обработка_выхода();
};

void SH_RATIOCTL::StepT( SStepCalcParams &dt )
{
  bool bHaveSpConnection = false;
  for( size_t i=0; i<inConsC; ++i )
  {
    if( 'S'==pInConns[i].szInFld[0] && 'P'==pInConns[i].szInFld[1] )
    {
      bHaveSpConnection = true;
      pInConns[i].enabledTrasfer = W->MODE.V == W->MODE.CAS ? true : false;
    }
  }
  InputConnectionsTransfer();
  RATIOCTL_IMPL *impl = reinterpret_cast<RATIOCTL_IMPL*>(W);
  impl->StepT( dt, bHaveSpConnection );
  OutputConnectionsTransfer();
}

void SH_RATIOCTL::StepAfterRestoreState()
{
  W->HIALM.PR = __ALPRIOR::None;
  W->HIALM.TYPE = __DACALMTYPE::None;
  W->HIALM.SV = 0;
}
//////////////////////////////////////////////////////////////////////////
void RATIOCTL_IMPL::StepT( SStepCalcParams &dt, bool bHaveSpConnection )
{
  if( SIFL )
  {
    if( SIALM.OPT )
      SIALM.FL = true;
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
    SIALM.FL = false;
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

  if ( !finite( X1 ) )
  {
    X1STS.V = X1STS.BAD;
    return;
  }
  X1STS.V = X1STS.NORMAL;

  if ( !finite( X2 ) )
  {
    X2STS.V = X2STS.BAD;
    return;
  }
  X2STS.V = X2STS.NORMAL;

  X1KB = K1 * X1 + X1BIAS;
  X2KB = K2 * X2 + X2BIAS;
  switch( CTLEQN.V )
  {
  case _CTLEQN::EQA:
    if( finite(X2KB) )
    {
      PV = X1KB / X2KB;
      PVSTS = _PVSTS::NORMAL;
    }
    else
    {
      PV = NaN;
      PVSTS = _PVSTS::BAD;
    }
    break;
  case _CTLEQN::EQB:
    if( finite(X1KB) )
    {
      PV = X2KB / X1KB;
      PVSTS = _PVSTS::NORMAL;
    }
    else
    {
      PV = NaN;
      PVSTS = _PVSTS::BAD;
    }
    break;
  case _CTLEQN::EQC:
    if( finite(X2KB) && finite(X1KB) )
    {
      PV = X1KB / (X2KB+X1KB);
      PVSTS = _PVSTS::NORMAL;
    }
    else
    {
      PV = NaN;
      PVSTS = _PVSTS::BAD;
    }
    break;
  case _CTLEQN::EQD:
    if( finite(X2KB) && finite(X1KB) )
    {
      PV = X1KB / (X2KB+X1KB);
      PVSTS = _PVSTS::NORMAL;
    }
    else
    {
      PV = NaN;
      PVSTS = _PVSTS::BAD;
    }
    break;
  default:
    ASSD(0);
  }

  Отслеживание_PV();
  Обработка_SP( bHaveSpConnection );
  CalcCV();
  Обработка_выхода();
}

void RATIOCTL_IMPL::Обработка_выхода()
{
  switch( OUTIND.V )
  {
  case _OUTIND::Direct:
    break;
  default:
  case _OUTIND::Reverse:
    //OP = 100 - OP;
    ASSD(0);
    break;
  }

  if( CVTYPE.ENGRUNIT==CVTYPE.V )
  {
    if( MODE.MAN==MODE.V || INITMAN)
    {
      CV = OP * 0.01 * ( CVEUHI - CVEULO ) + CVEULO;
      OPEU = CV;
    }
    else if( !IsNaN(CV) )
    {
      OPEU = CV;
      OP = ( CV - CVEULO ) / ( CVEUHI - CVEULO ) * 100.;
    }
  }
  else
  {
    if( MODE.MAN==MODE.V || INITMAN)
    {
      CV = OP * 0.01 * ( CVEUHI - CVEULO ) + CVEULO;
      if( finite( CVEUHI ) )
      {
        double A = OP * 0.01;
        OPEU = ( 1. - A ) * CVEULO + A * CVEUHI;
      }
      else
        OPEU = OP;
    }
    else if( !IsNaN(CV) )
    {
      OP = CV;
      if( finite( CVEUHI ) )
      {
        double A = OP * 0.01;
        OPEU = ( 1. - A ) * CVEULO + A * CVEUHI;
      }
      else
        OPEU = OP;
    }
  }

  // Пределы 
  if ( OP > OPEXHILM )
  {
    OP = OPEXHILM;
    OPHIALM.FL = 1;
  }
  else
    OPHIALM.FL = 0;
  if ( OP < OPEXLOLM )
  {
    OP = OPEXLOLM;
    OPLOALM.FL = 1;
  }
  else
    OPLOALM.FL = 0;
}

void RATIOCTL_IMPL::CalcCV()
{
  if( INITMAN )
    return;
  if( !(MODE.V == MODE.AUTO ||	MODE.V == MODE.CAS) )
    return;

  switch( CTLEQN.V )
  {
  case _CTLEQN::EQA:
    CV = (X2KB*SP-X1BIAS) / K1 + OPBIAS.FIX + OPBIAS.FLOAT;
    break;
  case _CTLEQN::EQB:
    if( finite(SP) )
      CV = (X2KB*SP-X1BIAS) / (K1*SP) + OPBIAS.FIX + OPBIAS.FLOAT;
    else
      CV = NaN;
    break;
  case _CTLEQN::EQC:
    if( SP!=1. )
      CV = (X1BIAS*(1-SP)-SP*X2KB)/(K1*(SP-1)) + OPBIAS.FIX + OPBIAS.FLOAT;
    else
      CV = NaN;
    break;
  case _CTLEQN::EQD:
    if( finite(SP) )
      CV = (X2KB-SP*(X1BIAS+X2KB))/(K1*SP) + OPBIAS.FIX + OPBIAS.FLOAT;
    else
      CV = NaN;
    break;
  default:
    ASSD(0);
  }

  double limit = ( CVEUHI - CVEULO ) /100. * OPHILM + CVEULO;
  if ( CV > limit )
  {
    CV = limit;
    OPHIALM.FL = 1;
    /*if ( CTLACTN.V == CTLACTN.DIRECT )
      ARWNET[0].V = ARWNET[0].Lo;	 //???
    else
      ARWNET[0].V = ARWNET[0].Hi;	 *///???
  }
  else
    OPHIALM.FL = 0;

  limit = ( CVEUHI - CVEULO ) /100. * OPLOLM + CVEULO;
  if ( CV < limit )
  {
    CV = limit;
    OPLOALM.FL = 1;
    /*if ( CTLACTN.V == CTLACTN.DIRECT )
      ARWNET[0].V = ARWNET[0].Hi;
    else
      ARWNET[0].V = ARWNET[0].Lo;*/
  }
  else
    OPLOALM.FL = 0;
}


void RATIOCTL_IMPL::Modefl()
{
  MODEFL.MAN = MODE.V == MODE.MAN;
  MODEFL.AUTO = MODE.V == MODE.AUTO;
  MODEFL.CAS = MODE.V == MODE.CAS;
  MODEATTRFL.OPER = MODEATTR.V == MODEATTR.OPERATOR;
  MODEATTRFL.PROG = MODEATTR.V == MODEATTR.PROGRAM;
  MODEFL.NORM = MODE.V == NORMMODE.V;
  MODEATTRFL.NORM = MODEATTR.V == NORMMODEATTR.V;
}

void RATIOCTL_IMPL::Отслеживание_PV()
{
  if ( PVSTS.V == PVSTS.BAD )
    return;
  if ( (MODE.V == MODE.MAN || INITMAN) /*&& PVTRAKOPT.V==PVTRAKOPT.TRACK*/ )
  {
    SP = PV;
    if ( SP > SPHILM )
    {
      SP = SPHILM;
    }
    if ( SP < SPLOLM )
    {
      SP = SPLOLM;
    }
  }
}

void RATIOCTL_IMPL::Обработка_SP( bool bHaveSpConnection )
{
  if( MODE.V == MODE.CAS  )
  {
    if( bHaveSpConnection )
    {
      double t = ( SPHILM - SPLOLM ) / 100. * SP;
      SP = t;
    }
  }

  if ( SP > SPHILM )
  {
    if ( MODE.V == MODE.CAS )
    {
      SP = SPHILM;
    }
  }
  if ( SP < SPLOLM )
  {
    if ( MODE.V == MODE.CAS )
    {
      SP = SPLOLM;
    }
  }
  SPP = ( SP - SPLOLM ) / ( SPHILM - SPLOLM ) * 100.;
}
