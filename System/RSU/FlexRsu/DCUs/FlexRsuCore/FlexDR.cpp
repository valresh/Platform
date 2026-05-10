#include <crossplatform.h>
#include "FlexDR.h"
#include <FlexRsuConnect.h>
#include <rsuErr.h>
#include <rsuStr.h>
#include <macros/AutoRestore.h>

KFlexDR::KFlexDR()
: m_nBlocks( 0 )
, m_ppBlocks( NULL )
, m_ppConnections( NULL )
, m_nCons( 0 )
{
}

void *KFlexDR::operator new( size_t size )
{
  return pFlexMemoryAllocator( (DWORD)size ); 
}

KFlexBlockBase *KFlexDR::GetBlock( GUID id )
{
  for( DWORD i=0; i<m_nBlocks; ++i )
  {
    KFlexBlockBase *pBlk = m_ppBlocks[i];
    if( !pBlk )
      continue;
    if( IsEqualGUID( id, pBlk->m_id) )
      return pBlk;
  }
  return NULL;
}

KFlexBlockBase *KFlexDR::GetBlock( LPCSTR pszName )
{
  for( DWORD i=0; i<m_nBlocks; ++i )
  {
    KFlexBlockBase *pBlk = m_ppBlocks[i];
    if( !pBlk )
      continue;

    std::string s (pBlk->m_szVisibleName);
    if( !_Kstricmp( pszName, s) )
      return pBlk;
  }
  return NULL;
}

void KFlexDR::LinkConnections()
{
  for( DWORD b=0; b<m_nBlocks; ++b )
  {
    DWORD nInCon = 0, nOutCon=0;
    KFlexConnection *pInc[200] = { NULL }, *pOutc[200] = { NULL };

    KFlexBlockBase *pBlk = m_ppBlocks[b];
    for( DWORD i=0; i<m_nCons; ++i )
    {
      KFlexConnection *pCon = m_ppConnections[i];
      if( IsEqualGUID( pBlk->m_id, pCon->m_SourceID) )
      {
        pOutc[nOutCon++] = pCon;
        //ASS( nOutCon<_countof(pOutc) );
      }
      else if( IsEqualGUID( pBlk->m_id, pCon->m_SinkID) )
      {
        pInc[nInCon++] = pCon;
        //ASS( nInCon<_countof(pInc) );
      }
    }
    if( nInCon || nOutCon )
      pBlk->AssignConnections( pInc, nInCon, pOutc, nOutCon );
    KKK();
  }
  /*for( int i=0; i<m_nCons; ++i )
  {
    KFlexConnection *pCon = m_ppConnections[i];
    if( !pCon )
      continue;

    KFlexBlockBase *pSrc = GetBlock( pCon->m_SourceID );
    KFlexBlockBase *pDst = GetBlock( pCon->m_SinkID );
    if( !pSrc || !pDst )
    {
      ASS( !"чего-то наворотили" );
      continue;
    }
  }*/
}

void KFlexDR::OnProjectReady( KFlexBlockBase::IProjectContainer *pCont )
{
  for( DWORD b=0; b<m_nBlocks; ++b )
  {
    KFlexBlockBase *pBlk = m_ppBlocks[b];
    pBlk->OnProjectReady( pCont );
  }
}

int KFlexDR::StepCalc( KFlexBlockBase::SCallParams &params )
{
  TAutoRestore<LPCSTR> ar( params.pszDrName );
  params.pszDrName = m_szFileName;
  params.ioMask = KFlexBlockBase::exnInputsIO;
  for( DWORD b=0; b<m_nBlocks; ++b )
  {
    KFlexBlockBase *pBlk = m_ppBlocks[b];
    if( pBlk->m_execOrder )
      continue;
    if( !(pBlk->m_nullOrderExec&params.ioMask) )
      continue;
    pBlk->StepCalc( params );
  }

  params.ioMask = 0;
  for( DWORD b=0; b<m_nBlocks; ++b )
  {
    KFlexBlockBase *pBlk = m_ppBlocks[b];
    if( !pBlk->m_execOrder )
      continue;
    pBlk->StepCalc( params );
  }

  params.ioMask = KFlexBlockBase::exnOutputsIO;
  for( DWORD b=0; b<m_nBlocks; ++b )
  {
    KFlexBlockBase *pBlk = m_ppBlocks[b];
    if( pBlk->m_execOrder )
      continue;
    if( !(pBlk->m_nullOrderExec&params.ioMask) )
      continue;
    pBlk->StepCalc( params );
  }
  return 0;
}

void KFlexDR::OnAfterRestoreState()
{
  for( DWORD b=0; b<m_nBlocks; ++b )
  {
    KFlexBlockBase *pBlk = m_ppBlocks[b];
    pBlk->OnAfterRestoreState();
  }
}
