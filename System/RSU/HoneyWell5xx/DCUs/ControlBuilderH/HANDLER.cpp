#include <rsuErr.h>
#include <H_Data.h>

static SModuleCreate HANDLER( "HANDLER", S_HANDLER::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(S_HANDLER,W_HANDLER,280)

static S_HANDLER _S;

void S_HANDLER::InitParm()
{
#include "Blocks/HANDLER.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/HANDLER_P.h"

  static char szActiveNames_Name[nStep2TransConnects][18*4]; 
  for( int i=0; i<_countof(szActiveNames_Name); ++i ) 
  {
    sprintf_s( szActiveNames_Name[i], "^ActiveName[%d]", i );
    SVarInfo* pV = InitInfo( evtString, _S, &_S.szActiveNames[i], szActiveNames_Name[i]);
    pV->varSize = _countof(_S.szActiveNames[i]);
  }
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class HANDLER_IMPL : public W_HANDLER
{
public:
  void StepT( SStepCalcParams &dt, S_HANDLER::SSeqRec *pSeqs, int nSeqCount );
  void SelectWorkingSeqs( S_HANDLER::SSeqRec *pSeqs, int nSeqCount, int seqNum );

  void Deactivate( S_HANDLER::SSeqRec *pSeqs, int nSeqCount, int num );
  void ShiftWorkingSeqs( S_HANDLER::SSeqRec *pSeqs, int nSeqCount, int seqNum );
};

void S_HANDLER::StepT( SStepCalcParams &dt )
{
  HANDLER_IMPL *impl = reinterpret_cast<HANDLER_IMPL*>(W);
  impl->StepT( dt, pSeqs, nSeqCount );
  ActualizeNames();
}

void S_HANDLER::SelectWorkingSeqs( int seqNum )
{
  HANDLER_IMPL *impl = reinterpret_cast<HANDLER_IMPL*>(W);
  impl->SelectWorkingSeqs( pSeqs, nSeqCount, seqNum );
  W->EXECSTS = __EXECSTS::Ok;
  ActualizeNames();
}

void S_HANDLER::ActualizeNames()
{
  for( int a=0; a<_countof(W->_SeqsInWork); ++a )
  {
    if( W->_SeqsInWork[a] < 0 )
    {
      szActiveNames[a][0] = 0;
      continue;
    }
    S_HANDLER::SSeqRec &rec = pSeqs[ W->_SeqsInWork[a] ];
    if( !rec.pBlock )
    {
      szActiveNames[a][0] = 0;
      continue;
    }
    LPCSTR pName = rec.pBlock->BlockName;
    LPCSTR pEnd = strrchr( pName, '.' );
    if( pEnd )
      strcpy_s( szActiveNames[a], pEnd + 1 );
    else
      strcpy_s( szActiveNames[a], pName );
  }
}

void S_HANDLER::StepAfterRestoreState()
{
  for( int a=0; a<_countof(W->_SeqsInWork); ++a )
  {
    if( W->_SeqsInWork[a] < 0 )
      break;
    S_HANDLER::SSeqRec &rec = pSeqs[ W->_SeqsInWork[a] ];
    rec.pBlock->SelectedInSCM();
  }
}

void S_HANDLER::ShiftNextFrom0()
{
  HANDLER_IMPL *impl = reinterpret_cast<HANDLER_IMPL*>(W);
  for( int d=0; d<_countof(impl->_SeqsInWork); ++d )
    impl->Deactivate( pSeqs, nSeqCount, impl->_SeqsInWork[d] );
  impl->ShiftWorkingSeqs( pSeqs, nSeqCount, impl->_SeqsInWork[0] );
}
//////////////////////////////////////////////////////////////////////////
void HANDLER_IMPL::StepT( SStepCalcParams &dt, S_HANDLER::SSeqRec *pSeqs, int nSeqCount )
{
  if( _EXECSTS::Ok != EXECSTS.V )
    return;
  for( int a=0; a<_countof(_SeqsInWork); ++a )
  {
    bool bBreak = false;
    if( _SeqsInWork[a] < 0 )
      break;
    ASSD( _SeqsInWork[a] < nSeqCount );
    S_HANDLER::SSeqRec &rec = pSeqs[ _SeqsInWork[a] ];
    switch( rec.type )
    {
    case S_HANDLER::SSeqRec::eTrans:
      if( !rec.pBlock )
        break;
      rec.pBlock->StepT( dt );
      {
        SH_TRANSITION *pTrans = (SH_TRANSITION*)rec.pBlock;
        if( pTrans->W->SO )
        {
          for( int d=0; d<_countof(_SeqsInWork); ++d )
            Deactivate( pSeqs, nSeqCount, _SeqsInWork[d] );
          bBreak = true;
          ShiftWorkingSeqs( pSeqs, nSeqCount, _SeqsInWork[a] );
        }
      }
      break;
    case S_HANDLER::SSeqRec::eStep:
      if( !rec.pBlock )
        break;
      rec.pBlock->StepT( dt );
      {
        SH_STEP *pStep = (SH_STEP*)rec.pBlock;
        if( pStep->W->SC.FL )
        {
          bBreak = true;
          EXECSTS = _EXECSTS::OkPaused;
          break;
        }
        if( pStep->W->COMPLETED )
        {
          for( int d=0; d<_countof(_SeqsInWork); ++d )
            Deactivate( pSeqs, nSeqCount, _SeqsInWork[d] );
          bBreak = true;
          ShiftWorkingSeqs( pSeqs, nSeqCount, _SeqsInWork[a] );
        }
      }
      break;
    }
    if( bBreak )
      break;
  }
}

void HANDLER_IMPL::Deactivate( S_HANDLER::SSeqRec *pSeqs, int nSeqCount, int num )
{
  if( num<0 || num>=nSeqCount )
    return;
  S_HANDLER::SSeqRec &rec = pSeqs[num];
  switch( rec.type )
  {
  case S_HANDLER::SSeqRec::eTrans:
    if( !rec.pBlock )
      break;
    {
      SH_TRANSITION *pTrans = (SH_TRANSITION*)rec.pBlock;
      pTrans->W->COMPLETED = 0;
    }
    break;
  case S_HANDLER::SSeqRec::eStep:
    if( !rec.pBlock )
      break;
    {
      SH_STEP *pStep = (SH_STEP*)rec.pBlock;
      pStep->W->ACTIVEFL = 0;
      pStep->W->COMPLETED = 0;
    }
    break;
  }
}

void HANDLER_IMPL::SelectWorkingSeqs( S_HANDLER::SSeqRec *pSeqs, int nSeqCount, int seqNum )
{
  if( seqNum<0 || seqNum>=nSeqCount )
  {
    memset( _SeqsInWork, -1, sizeof(_SeqsInWork) );
    return;
  }
  S_HANDLER::SSeqRec &rec = pSeqs[seqNum];
  switch( rec.type )
  {
  case S_HANDLER::SSeqRec::eStep:
  case S_HANDLER::SSeqRec::eTrans:
    rec.pBlock->SelectedInSCM();
    _SeqsInWork[0] = seqNum;
    ASSD( rec.number == seqNum );
    for( int i=1; i<_countof(_SeqsInWork); ++i )
    {
      _SeqsInWork[i] = -1;
    }
    break;
  }
}

void HANDLER_IMPL::ShiftWorkingSeqs( S_HANDLER::SSeqRec *pSeqs, int nSeqCount, int seqNum )
{
  if( seqNum<0 || seqNum>=nSeqCount )
  {
    memset( _SeqsInWork, -1, sizeof(_SeqsInWork) );
    return;
  }
  bool bHaveNexts = false;
  S_HANDLER::SSeqRec &rec = pSeqs[seqNum];
  switch( rec.type )
  {
  case S_HANDLER::SSeqRec::eTrans:
  case S_HANDLER::SSeqRec::eStep:
    for( int i=0; i<_countof(rec.nextRecs); ++i )
    {
      _SeqsInWork[i] = rec.nextRecs[i];
      int index = _SeqsInWork[i];
      if( index<0 || index>=nSeqCount )
        continue;
      if( index>=nSeqCount )
      {
        ASSD(0);
        continue;
      }
      pSeqs[ index ].pBlock->SelectedInSCM();
      bHaveNexts = true;
    }
    break;
  }
  if( !bHaveNexts )
    EXECSTS = _EXECSTS::NoTransition;
}
