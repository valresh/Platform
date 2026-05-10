#include <rsuErr.h>
#include "H_Class.h"
#include "../ScriptoH/Trans.h"
#include <SR_DCU.h>
#include <math.h>

using namespace std;
using namespace ns_translator;

static SBlockCreate ENHREGCALC( "ENHREGCALC", SH_ENHREGCALC::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_ENHREGCALC,W_ENHREGCALC,480)

void SH_ENHREGCALC::InitParm()
{
#include "Blocks/ENHREGCALC.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/ENHREGCALC_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class ENHREGCALC_IMPL : public W_ENHREGCALC
{
public:
  void StepT( SStepCalcParams &dt );
protected:
  void Modefl();
};

void SH_ENHREGCALC::StepT( SStepCalcParams &dt )
{
  SAuxCalcCommon::StepT( dt );
  for( int n=0; n<_countof(m_Code); ++n )
  {
    if( Calc( m_Code[n], W->C[n] ) )
    {
      W->EXECSTS[n] = W_ENHREGCALC::_EXECSTS::Ok;
      W->CSTS[n] = W_ENHREGCALC::_CSTS::NORMAL;
    }
    else
    {
      W->EXECSTS[n] = W_ENHREGCALC::_EXECSTS::Fail;
      W->CSTS[n] = W_ENHREGCALC::_CSTS::BAD;
    }
  }

  ENHREGCALC_IMPL *impl = reinterpret_cast<ENHREGCALC_IMPL*>(W);
  impl->StepT( dt );
}

void SH_ENHREGCALC::OnReadProject( KBmBase *pRoot, KBmBase *pModule )
{
  ASS( _countof(W->EXPR)==_countof(m_Code) );
  LPSTR pp[_countof(W->EXPR)];
  for( int i=0; i<_countof(W->EXPR); ++i )
  {
    pp[i] = W->EXPR[i];
  }
  SAuxCalcCommon::CreateCodes( pRoot, pModule, m_Code, pp, _countof(W->EXPR) );
}

void SH_ENHREGCALC::SaveBlock(std::fstream& hFile )
{
  LPSTR pp[_countof(W->EXPR)];
  for( int i=0; i<_countof(W->EXPR); ++i )
  {
    pp[i] = W->EXPR[i];
  }
  SAuxCalcCommon::SaveBlock( hFile, m_Code, pp, _countof(pp) );
}

bool SH_ENHREGCALC::RestoreBlock(std::fstream& hFile )
{
  LPSTR pp[_countof(W->EXPR)];
  for( int i=0; i<_countof(W->EXPR); ++i )
  {
    pp[i] = W->EXPR[i];
  }
  return SAuxCalcCommon::RestoreBlock( hFile, m_Code, pp, _countof(pp) );
}

void SH_ENHREGCALC::StepBeforeRestoreState( LPCSTR pSystemName, KBmBase *pRoot, KBmBase *pModule )
{
  for( int n=0; n<_countof(m_Code); ++n )
  {
    for( int i=0; i<m_Code[n].cVars; i++ )
    {
      KCalcVar &cv = ((KCalcVar*)m_Code[n].pVars)[i];
      bool r = cv.Init( pRoot );
      if( !r )
      {
        OutputDebugString( " (" );
        OutputDebugString( BlockName );
        OutputDebugString( ")\n" );
        bErrorInitialization = true;
#ifndef _DEBUG
        break;
#endif
      }
    }
  }
}

void SH_ENHREGCALC::StepAfterRestoreState()
{
  W->HIALM.PR = __ALPRIOR::None;
  W->HIALM.TYPE = __DACALMTYPE::None;
  W->HIALM.SV = 0;
}
//////////////////////////////////////////////////////////////////////////
void ENHREGCALC_IMPL::StepT( SStepCalcParams &dt )
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

  double lcv[_countof(X)] = { NaN };
  for( int i=0; i<_countof(X); ++i )
  {
    if ( !finite( X[i] ) )
    {
      XSTS[i].V = XSTS[i].BAD;
      continue;
    }
    XSTS[i].V = XSTS[i].NORMAL;
    //X1P = ( X[i] - XEULO ) / ( XEUHI - XEULO ) * 100.;
    lcv[i] = XK[i] * X[i] + OPBIAS.FIX + XB[i];
  }
  switch( CVSRC.V )
  {
  case _CVSRC::C1:
    CV = C[1];
    break;
  default:
    ASSD(0);
  }
  if ( !IsNaN( OPBIAS.FLOAT ))
    CV += OPBIAS.FLOAT;

  if ( MODE.V != MODE.MAN )
  {
    OP = ( CV - CVEULO ) / ( CVEUHI - CVEULO ) * 100.;
  }
  if ( OP < OPLOLM )
    OP = OPLOLM;
  if ( OP > OPHILM )
    OP = OPHILM;

  double A = OP * 0.01;
  OPEU = ( 1. - A ) * CVEULO + A * CVEUHI;
}


void ENHREGCALC_IMPL::Modefl()
{
  MODEFL.MAN = MODE.V == MODE.MAN;
  MODEFL.AUTO = MODE.V == MODE.AUTO;
  MODEFL.CAS = MODE.V == MODE.CAS;
  MODEATTRFL.OPER = MODEATTR.V == MODEATTR.OPERATOR;
  MODEATTRFL.PROG = MODEATTR.V == MODEATTR.PROGRAM;
  MODEFL.NORM = MODE.V == NORMMODE.V;
  MODEATTRFL.NORM = MODEATTR.V == NORMMODEATTR.V;
}
