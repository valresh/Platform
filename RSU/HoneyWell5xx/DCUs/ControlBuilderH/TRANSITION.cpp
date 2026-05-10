#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate TRANSITION( "TRANSITION", SH_TRANSITION::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_TRANSITION,W_TRANSITION,255)

SH_TRANSITION::SH_TRANSITION()
{
  KKK();
}

void SH_TRANSITION::InitParm()
{
#include "Blocks/TRANSITION.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/TRANSITION_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

void SH_TRANSITION::OnReadProject( KBmBase *pRoot, KBmBase *pModule )
{
  ASS( _countof(W->C)==_countof(m_Code) )
  LPSTR pp[_countof(W->C)] = {};
  for( int i=0; i<_countof(W->C); ++i )
  {
    pp[i] = W->C[i].szEXPR;
  }
  SAuxCalcCommon::CreateCodes( pRoot, pModule, m_Code, pp, _countof(pp) );
}

void SH_TRANSITION::SaveBlock(std::fstream& hFile )
{
  LPSTR pp[_countof(W->C)] = {};
  for( int i=0; i<_countof(W->C); ++i )
  {
    pp[i] = W->C[i].szEXPR;
  }
  SAuxCalcCommon::SaveBlock( hFile, m_Code, pp, _countof(pp) );
}

bool SH_TRANSITION::RestoreBlock(std::fstream& hFile )
{
  LPSTR pp[_countof(W->C)] = {};
  for( int i=0; i<_countof(W->C); ++i )
  {
    pp[i] = W->C[i].szEXPR;
  }
  return SAuxCalcCommon::RestoreBlock( hFile, m_Code, pp, _countof(pp) );
}

void SH_TRANSITION::StepBeforeRestoreState( LPCSTR pSystemName, KBmBase *pRoot, KBmBase *pModule )
{
  bErrorInitialization = false;
  for( int n=0; n<_countof(m_Code); ++n )
  {
    for( int i=0; i<m_Code[n].cVars; i++ )
    {
      KCalcVar &cv = ((KCalcVar*)m_Code[n].pVars)[i];
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
}

void SH_TRANSITION::StepAfterRestoreState()
{
  for( int i=0; i<inConsC; ++i )
  {
    pInConns[i].enabledTrasfer = false;
  }
  int N = std::min<int>( W->NUMCONDS + 1, _countof(m_Code) );
  for( int n=1; n<N; ++n )
    W->C[n].FL = 0;
}

void SH_TRANSITION::SelectedInSCM()
{
  int N = std::min<int>( W->NUMCONDS + 1, _countof(m_Code) );
  for( int n=1; n<N; ++n )
    W->C[n].FL = 0;
}
//////////////////////////////////////////////////////////////////////////
class TRANSITION_IMPL : public W_TRANSITION
{
public:
  void StepT( SStepCalcParams &dt );
};

void SH_TRANSITION::StepT( SStepCalcParams &dt )
{
  if( bErrorInitialization )
    return;
  int N = std::min<int>( W->NUMCONDS + 1, _countof(m_Code) );
  for( int n=1; n<N; ++n )
  {
    switch( W->C[n].BYPREQ.V )
    {
    case W->C[n].BYPREQ.ToOff:
      W->C[n].FL = 0;
      continue;
    case W->C[n].BYPREQ.ToOn:
      W->C[n].FL = 1;
      continue;
    }
    double r = NaN;
    bool bR = Calc( m_Code[n], r );
    if( IsNaN(r) )
      continue;
    if( bR )
      W->C[n].FL = (int)r;
  }

  TRANSITION_IMPL *impl = reinterpret_cast<TRANSITION_IMPL*>(W);
  impl->StepT( dt );
}
//////////////////////////////////////////////////////////////////////////
void TRANSITION_IMPL::StepT( SStepCalcParams &dt )
{
  COMPLETED = 1;
  if( !NUMCONDS )
  {
    return;
  }
  BYTE SOs[_countof(G)] = {};
  bool wasGateAssign[_countof(G)] = {};
  for( int n=1; n<=NUMCONDS; ++n )
  {
    __GATEASGN ga = C[n].GATEASGN;
    int nGate = 0;
    if( __GATEASGN::GateP1 == ga.V )
      nGate = 2;
    else if( __GATEASGN::GateP2 == ga.V )
      nGate = 3;
    else if( __GATEASGN::GateP3 == ga.V )
      nGate = 4;
    else
      continue;
    _G &g = G[nGate];
    switch( g.ALGID.V )
    {
    case __ALGID::Connect:
      SOs[nGate] = C[n].FL;
      break;
    case __ALGID::None:
      break;
    case __ALGID::And:
      if( !wasGateAssign[nGate] )
        SOs[nGate] = C[n].FL;
      if( !SOs[nGate] )
        break;
      if( !C[n].FL )
        SOs[nGate] = C[n].FL;
      break;
    case __ALGID::Or:
      if( C[n].FL )
        SOs[nGate] = C[n].FL;
      break;
    default:
      ASSD(0);
      break;
    }
    wasGateAssign[nGate] = true;
  }
  for( int i=2; i<_countof(G); ++i )
  {
    G[i].SO = SOs[i];
  }
  switch( G[1].ALGID.V )
  {
  case __ALGID::Connect:
    for( int i=2; i<_countof(G); ++i )
    {
      if( __ALGID::None==G[i].ALGID.V )
        continue;
      if( SOs[i] )
        SOs[1] = SOs[i];
    }
    G[1].SO = SOs[1];
    break;
  case __ALGID::Or:
    for( int i=2; i<_countof(G); ++i )
    {
      if( __ALGID::None==G[i].ALGID.V )
        continue;
      if( SOs[i] )
        SOs[1] = SOs[i];
    }
    G[1].SO = SOs[1];
    break;
  case __ALGID::And:
    if( __ALGID::None!=G[2].ALGID.V )
      SOs[1] = SOs[2];
    else if( __ALGID::None!=G[3].ALGID.V )
      SOs[1] = SOs[3];
    else if( __ALGID::None!=G[4].ALGID.V )
      SOs[1] = SOs[4];
    else
      break;
    for( int i=3; i<_countof(G); ++i )
    {
      if( __ALGID::None==G[i].ALGID.V )
        continue;
      if( !SOs[1] )
        break;
      if( !SOs[i] )
        SOs[1] = SOs[i];
    }
    G[1].SO = SOs[1];
    break;
  case __ALGID::None:
    break;
  default:
    ASSD(0);
    break;
  }
  SO = G[1].SO;
}
