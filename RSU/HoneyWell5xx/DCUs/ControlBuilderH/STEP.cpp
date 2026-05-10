#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate STEP( "STEP", SH_STEP::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_STEP,W_STEP,1010)

void SH_STEP::InitParm()
{
#include "Blocks/STEP.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/STEP_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

void SH_STEP::OnReadProject( KBmBase *pRoot, KBmBase *pModule )
{
  ASS( _countof(W->OP)==_countof(m_CodeOP) )
  LPSTR pp[_countof(W->OP)] = {};
  for( int i=0; i<_countof(W->OP); ++i )
  {
    pp[i] = W->OP[i].SRCEXPR;
  }
  SAuxCalcCommon::CreateCodes( pRoot, pModule, m_CodeOP, pp, _countof(pp) );
  LPSTR stop[1] = {};
  stop[0] = W->SC.szEXPR;
  SAuxCalcCommon::CreateCodes( pRoot, pModule, &m_stopCon, stop, _countof(stop) );
}

void SH_STEP::SaveBlock(std::fstream& hFile )
{
  LPSTR pp[_countof(W->OP)] = {};
  for( int i=0; i<_countof(W->OP); ++i )
  {
    pp[i] = W->OP[i].SRCEXPR;
  }
  SAuxCalcCommon::SaveBlock( hFile, m_CodeOP, pp, _countof(pp) );
  LPSTR stop[1] = {};
  stop[0] = W->SC.szEXPR;
  SAuxCalcCommon::SaveBlock( hFile, &m_stopCon, stop, _countof(stop) );
}

bool SH_STEP::RestoreBlock(std::fstream& hFile )
{
  LPSTR pp[_countof(W->OP)] = {};
  for( int i=0; i<_countof(W->OP); ++i )
  {
    pp[i] = W->OP[i].SRCEXPR;
  }
  bool b = SAuxCalcCommon::RestoreBlock( hFile, m_CodeOP, pp, _countof(pp) );
  if( !b )
    return false;
  LPSTR stop[1] = {};
  stop[0] = W->SC.szEXPR;
  return SAuxCalcCommon::RestoreBlock( hFile, &m_stopCon, stop, _countof(stop) );
}

void SH_STEP::StepBeforeRestoreState( LPCSTR pSystemName, KBmBase *pRoot, KBmBase *pModule )
{
  bErrorInitialization = false;
  for( int n=0; n<_countof(m_CodeOP); ++n )
  {
    for( int i=0; i<m_CodeOP[n].cVars; i++ )
    {
      KCalcVar &cv = ((KCalcVar*)m_CodeOP[n].pVars)[i];
      bool r = cv.Init( pRoot );
      /*if( !r )
      {
        OutputDebugString( " поищу среди NonCeePoints, " );
        r = cv.Init( this );
        if( r )
          OutputDebugString( "всё таки связался\n" );
      }*/
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

  for( int i=0; i<m_stopCon.cVars; i++ )
  {
    KCalcVar &cv = ((KCalcVar*)m_stopCon.pVars)[i];
    bool r = cv.Init( pRoot );
    /*if( !r )
    {
      OutputDebugString( " поищу среди NonCeePoints, " );
      r = cv.Init( this );
      if( r )
        OutputDebugString( "всё таки связался\n" );
    }*/
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

void SH_STEP::StepAfterRestoreState()
{
  for( DWORD i=0; i<inConsC; ++i )
  {
    pInConns[i].enabledTrasfer = false;
  }
}

void SH_STEP::SelectedInSCM()
{
  W->ACTIVEFL = 1;
  W->COMPLETED = 0;
  W->SC.FL = 0;
  W->TIME = 0;
  for( int i=0; i<_countof(W->OP); ++i )
  {
    W->OP[i].DELAYTIMEREM = W->OP[i].DELAYTIME;
    W->OP[i].STATE = __STATE::ProcDelay;
  }
}
//////////////////////////////////////////////////////////////////////////
class STEP_IMPL : public W_STEP
{
public:
  void StepT( int dtMS );
};

void SH_STEP::StepT( SStepCalcParams &dt )
{
  if( bErrorInitialization )
    return;

  int dtMS = dt * 1000;

  W->TIME += dtMS;

  double r = NaN;
  if( Calc( m_stopCon, r ) )
  {
    W->SC.FL = (int)r;
    if( W->SC.FL )
      return;
  }

  for( int n=0; n<_countof(m_CodeOP); ++n )
  {
    if(__STATE::Complete == W->OP[n].STATE.V )
      continue;
    if( W->OP[n].DELAYTIMEREM > 0 )
    {
      W->OP[n].DELAYTIMEREM -= dt;
      if( W->OP[n].DELAYTIMEREM > 0 )
        break;
    }
    W->OP[n].DELAYTIMEREM = 0;
    W->OP[n].STATE = __STATE::Complete;
    double r = NaN;
    Calc( m_CodeOP[n], r );
  }

  STEP_IMPL *impl = reinterpret_cast<STEP_IMPL*>(W);
  impl->StepT( dt );
}
//////////////////////////////////////////////////////////////////////////
void STEP_IMPL::StepT( int dtMS )
{
  int minTimeMS = MINTIME * 1000;

  for( int i=0; i<_countof(OP); ++i )
  {
    if( OP[i].DELAYTIMEREM > 0 )
      return;
  }
  if( TIME <= minTimeMS )
    return;
  ACTIVEFL = 0;
  COMPLETED = 1;
}

