#include <rsuErr.h>
#include <H_Data.h>
#include <macros/StrHelps.h>
#include <SR_DCU.h>
#include <macros/AutoRestore.h>

static SModuleCreate SCM( "SCM", S_SCM::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(S_SCM,W_SCM,471)

void S_SCM::InitParm()
{
#include "Modules/SCM.h" 
  s_defFlag = SVarInfo::efParam;
#include "Modules/SCM_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

bool S_SCM::SetValue( LPCSTR pField, LPCSTR pszVal )
{
  if( !strcmp(pField,"ORDER") )
  {
    size_t l = strlen( pszVal );
    pszRawOrder = (LPSTR)KMemoryServiceH5xx::Instance().NewMem( l+1 );
    strcpy_s((char*)pszRawOrder, l+1, (char*)pszVal );
  }
  return SH_Module::SetValue( pField, pszVal );
}

void S_SCM::OnReadProject( KBmBase *pRoot, KBmBase *pModule )
{
  W->STATE = W_SCM::_STATE::Idle;
  SH_Module::OnReadProject( pRoot, pModule );
}

void S_SCM::SaveBlock(std::fstream& hFile )
{
  DWORD Lw;
  DWORD size = (DWORD)strlen( pszRawOrder );
  WRT_VAR( size );
  WRT_DATA( size, pszRawOrder );
}

bool S_SCM::RestoreBlock(std::fstream& hFile )
{
  DWORD Lr;
  DWORD size = 0;
  READ_VAR( size );
  pszRawOrder = (LPSTR)KMemoryServiceH5xx::Instance().NewMem( size+1 );
  READ_DATA( size, pszRawOrder );

  return true;
}

void S_SCM::StepBeforeRestoreState( LPCSTR pSystemName, KBmBase *pRoot, KBmBase *pModule )
{
    SH_Module::StepBeforeRestoreState( pSystemName, pRoot, pModule );
  char splits[200][48*4] = {};
  const int C = SplitString( pszRawOrder, splits, ';', false );
  ASS( C < _countof(splits) );
  int iC = 0;
  {
    char szP[4][sizeofBlockName] = {};
    SplitString( splits[iC++], szP, ',', false );
    ASSD( 'V'==szP[0][0] );
    ASSD( '1'==szP[1][0] );
    SplitString( splits[iC++], szP, ',', false );
    ASSD( 'G'==szP[0][0] );
    ASSD( nSubModules==atoi(szP[1]) );
    KKK();
  }
  for( ; iC<C; ++iC )
  {
    if( (C-iC) < 2 )
      break;
    S_HANDLER* pHandler = NULL;
    int subs = 0;
    for( int i=0; i<2; ++i, ++iC )
    {
      char szP[4][sizeofBlockName] = {};
      const int cP = SplitString( splits[iC], szP, ',', false );
      switch( i )
      {
      case 0:
        ASSD( 'H'==szP[0][0] );
        ASSD( '0'==szP[1][0] );
        {
          for( int m=0; m<nSubModules; ++m )
          {
            LPCSTR pszEnd = strchr( ppSubModules[m]->BlockName, '.' ) + 1;
            if( !strcmp( pszEnd, szP[2] ) )
            {
              pHandler = (S_HANDLER*)ppSubModules[m];
              switch( pHandler->W->TYPE.V )
              {
              case W_HANDLER::_TYPE::Main:
              case W_HANDLER::_TYPE::Check:
              case W_HANDLER::_TYPE::Stop:
              case W_HANDLER::_TYPE::Abort:
              case W_HANDLER::_TYPE::Interrupt:
              case W_HANDLER::_TYPE::Restart:
              case W_HANDLER::_TYPE::Hold:
                ASSD( pHandler->W->TYPE.V < _countof(pHandlers) );
                pHandlers[pHandler->W->TYPE.V] = pHandler;
                break;
              default:
                KKK();
                break;
              }
              break;
            }
          }
          ASSD( pHandler );
        }
        break;
      case 1:
        ASSD( 'G'==szP[0][0] );
        subs = atoi( szP[1] );
        break;
      }
    }
    if( !pHandler )
      break;
    const int tSeqCount = subs;
    pHandler->pSeqs = new S_HANDLER::SSeqRec[tSeqCount];

    if( W_HANDLER::_TYPE::Main == pHandler->W->TYPE.V )
    {
      W->NUMSTEPS = 0;
      W->NUMTRANS = 0;
    }
    int deltaSeqNum = INT_MIN;
    for( int i=0; i<subs; ++i, ++iC )
    {
      char szP[3+_countof(pHandler->pSeqs->nextRecs)][sizeofBlockName] = {};
      const int cP = SplitString( splits[iC], szP, ',', false );

      if( INT_MIN==deltaSeqNum )
        deltaSeqNum = 0 - (atoi( szP[3] ) - 1);

      S_HANDLER::SSeqRec &rec = pHandler->pSeqs[pHandler->nSeqCount++];
      ASSD( pHandler->nSeqCount <= tSeqCount );
      rec.type = (S_HANDLER::SSeqRec::eType)szP[0][0];
      rec.number = pHandler->nSeqCount - 1;

      switch( rec.type )
      {
      case S_HANDLER::SSeqRec::eTrans:
        if( W_HANDLER::_TYPE::Main == pHandler->W->TYPE.V )
          ++W->NUMTRANS;
        break;
      case S_HANDLER::SSeqRec::eStep:
        if( W_HANDLER::_TYPE::Main == pHandler->W->TYPE.V )
          ++W->NUMSTEPS;
        break;
      }
      strcpy_s( rec.szName, szP[2] );
      KBmBase *p = FindObj( rec.szName );
      ASSD( p );
      rec.pBlock = (SH_Block*)p;
      int j = 0;
      for( int r=3; r<cP; ++r, ++j )
      {
        rec.nextRecs[j] = atoi( szP[r] ) + deltaSeqNum;
        ASSD( j < _countof(rec.nextRecs) );
      }
    }
    --iC;
  }
}

void S_SCM::StepAfterRestoreState()
{
    SH_Module::StepAfterRestoreState();
  switch( W->PERIOD.V )
  {
  case W_SCM::_PERIOD::_50mS:
    periodSec = 0.05;
    break;
  case W_SCM::_PERIOD::_100mS:
    periodSec = 0.1;
    break;
  case W_SCM::_PERIOD::_200mS:
    periodSec = 0.2;
    break;
  case W_SCM::_PERIOD::_500mS:
    periodSec = 0.5;
    break;
  default:
    ASSD(!"что-то совсем плохо");
  case W_SCM::_PERIOD::_1sec:
  case W_SCM::_PERIOD::DEFAULT:
    periodSec = 1.;
    break;
  case W_SCM::_PERIOD::_2sec:
    periodSec = 2.;
    break;
  }
  ASSD( -1==W->PHASEHOUR );
  ASSD( -1==W->PHASEMIN )
  maxPhaseIndex = periodSec / 0.05;
}
//////////////////////////////////////////////////////////////////////////
class SCM_IMPL : public W_SCM
{
public:
  template<int _S>
  void StepT( SStepCalcParams &dt, S_HANDLER* (&pHandlers)[_S] );
};

void S_SCM::StepT( SStepCalcParams &dt )
{
  /*if( !strcmp(BlockName,"GRAVELFILTER1") )
  {
    KKK();
    //if( W->STATE.V != W_SCM::_STATE::Running )
      //W->COMMAND = W_SCM::_COMMAND::Start;
  }*/
  SCM_IMPL *impl = reinterpret_cast<SCM_IMPL*>(W);
  if( !dt.usePhases )
    impl->StepT( dt, pHandlers );
  else
  {
    if( W->activePhase==W->PHASE )
    {
      TAutoRestore<double> arPeriod(dt.periodSec);
      dt.periodSec = periodSec;
      impl->StepT( dt, pHandlers );
    }
    ++W->activePhase;
    if( W->activePhase >= maxPhaseIndex )
      W->activePhase = 0;
  }
}
//////////////////////////////////////////////////////////////////////////
template<int _S>
void SCM_IMPL::StepT( SStepCalcParams &dt, S_HANDLER* (&pHandlers)[_S] )
{
  const int indexMain = W_HANDLER::_TYPE::Main;
  const int indexCheck = W_HANDLER::_TYPE::Check;
  const int indexAbort = W_HANDLER::_TYPE::Abort;

  switch( COMMAND.V )
  {
  case _COMMAND::Start:
    for( int i=0; i<_countof(pHandlers); ++i )
    {
      ACTIVEHANDLR[i] = false;
      if( !pHandlers[i] || !pHandlers[i]->nSeqCount )
        continue;
      if( pHandlers[i]->W->_SeqsInWork[0] < 0 )
        pHandlers[i]->SelectWorkingSeqs( 0 );
    }
    STATE = _STATE::Running;
    COMMAND = _COMMAND::None;
    return;
    break;
  case _COMMAND::Reset:
    for( int i=0; i<_countof(pHandlers); ++i )
    {
      if( !pHandlers[i] || !pHandlers[i]->nSeqCount )
        continue;
      pHandlers[i]->W->_SeqsInWork[0] = -3;
    }
    if( pHandlers[indexCheck] )
    {
      STATE = _STATE::Checking;
      pHandlers[indexCheck]->SelectWorkingSeqs( 0 );
      if ( 0==pHandlers[indexCheck]->W->_SeqsInWork[0] )
        pHandlers[indexCheck]->ShiftNextFrom0();
      ACTIVEHANDLR[ indexCheck ] = true;
    }
    else
      STATE = _STATE::Idle;
    COMMAND = _COMMAND::None;
    break;
  case _COMMAND::Stop:
    STATE = _STATE::Stopped;
    COMMAND = _COMMAND::None;
    break;
  case _COMMAND::Abort:
    COMMAND = _COMMAND::None;
    {
      if( !pHandlers[indexAbort] || !pHandlers[indexAbort]->nSeqCount )
        break;
      if( pHandlers[indexAbort]->W->_SeqsInWork[0] < 0 )
        pHandlers[indexAbort]->SelectWorkingSeqs( 0 );
      else if ( 0==pHandlers[indexAbort]->W->_SeqsInWork[0] )
      {
        pHandlers[indexAbort]->ShiftNextFrom0();
      }
      ACTIVEHANDLR[ indexAbort ] = true;
      STATE = _STATE::Aborting;
    }
    break;
  }
  
  switch( STATE.V )
  {
  case _STATE::Idle:
    for( int i=0; i<_countof(pHandlers); ++i )
    {
      ACTIVEHANDLR[i] = false;
      if( !pHandlers[i] || !pHandlers[i]->nSeqCount )
        continue;
      if(W_HANDLER::_TYPE::Main != pHandlers[i]->W->TYPE.V )
        continue;
      S_HANDLER::SSeqRec &rec = pHandlers[i]->pSeqs[0];
      if( !rec.pBlock )
        continue;
      if( S_HANDLER::SSeqRec::eTrans != rec.type )
        continue;
      rec.pBlock->StepT( dt );
      {
        SH_TRANSITION *pTrans = (SH_TRANSITION*)rec.pBlock;
        if( pTrans->W->SO )
        {
          COMMAND = _COMMAND::Start;
          return;
        }
      }
    }
  case _STATE::Stopped:
  default:
    return;
  case _STATE::Aborted:
  case _STATE::Complete:
    if( pHandlers[indexCheck] )
      break;
    return;
  case _STATE::Aborting:
  case _STATE::Running:
    break;
  case _STATE::Checking:
    break;
  }

  bool bAnyWork = false;

  for( int i=_countof(pHandlers)-1; i>0; --i )
  {
    if( !pHandlers[i] || !pHandlers[i]->nSeqCount )
      continue;
    if( _EXECSTS::Ok != pHandlers[i]->W->EXECSTS.V )
    {
      ACTIVEHANDLR[i] = false;
      continue;
    }

    if( i > indexMain && pHandlers[indexMain] )
    {
      if( false==ACTIVEHANDLR[indexMain] )
      {
        if(_EXECSTS::Ok == pHandlers[indexMain]->W->EXECSTS.V )
        {//Main еще не активен
          if( false==ACTIVEHANDLR[i] )
            continue;
        }
      }
    }

    if( i==indexCheck && pHandlers[indexCheck] )
    {
      switch( STATE.V )
      {
      case _STATE::Checking:
      case _STATE::Complete:
      case _STATE::Aborted:
        break;
      default:
        continue;
      }
    }

    bAnyWork = true;
    pHandlers[i]->StepT( dt );

    if( (_EXECSTS::Ok == pHandlers[i]->W->EXECSTS.V && pHandlers[i]->W->_SeqsInWork[0] > 0)
        || _EXECSTS::NoTransition == pHandlers[i]->W->EXECSTS.V )
    {
      if( _EXECSTS::Ok == pHandlers[i]->W->EXECSTS.V )
      {
        ACTIVEHANDLR[i] = true;
        if( indexAbort==i )
          STATE = _STATE::Aborting;
        if( indexCheck==i )
          STATE = _STATE::Checking;
      }
      else
      {
        ACTIVEHANDLR[i] = false;
        if( indexMain==i )
          STATE = _STATE::Complete;
        if( indexAbort==i )
          STATE = _STATE::Aborted;
        if( indexCheck==i )
        {
          for( int i=0; i<_countof(pHandlers); ++i )
          {
            if( !pHandlers[i] || !pHandlers[i]->nSeqCount )
              continue;
            pHandlers[i]->W->_SeqsInWork[0] = -3;
          }
          STATE = _STATE::Idle;
        }
      }
      for( int h=0; h<i; ++h )
      {
        if( !pHandlers[h] )
          continue;
        if( indexCheck==h )
          continue;
        ACTIVEHANDLR[h] = false;
        pHandlers[ h ]->W->EXECSTS.V = _EXECSTS::OkPaused;
      }
    }
    else
      ACTIVEHANDLR[i] = false;

    S_HANDLER::SSeqRec &rec = pHandlers[i]->pSeqs[ pHandlers[i]->W->_SeqsInWork[0] ];
    if( S_HANDLER::SSeqRec::eStep != rec.type )
      continue;
    SH_STEP *pStep = (SH_STEP*)rec.pBlock;
    if( !pStep->W->SC.FL )
      continue;

    S_HANDLER *pStop = pHandlers[W_HANDLER::_TYPE::Stop ];
    if( !pStop )
      continue;
    if( 0!=pStop->W->_SeqsInWork[0] )
      continue;
    ACTIVEHANDLR[W_HANDLER::_TYPE::Stop ] = true;
    {
      S_HANDLER::SSeqRec &rec = pStop->pSeqs[ 0 ];
      if( S_HANDLER::SSeqRec::eTrans != rec.type )
        continue;
      pStop->ShiftNextFrom0();
    }
  }

  for( int i=0; i<_countof(ACTIVEHANDLR); ++i )
  {
    if( !ACTIVEHANDLR[i] )
      continue;
    bAnyWork = true;
    break;
  }
  if( !bAnyWork)
  {
    if( _STATE::Running==STATE.V )
      STATE = _STATE::Complete;
  }
}
