#include "Y_Data.h"
#include "MemAlloc.h"

const DWORD CY_DR::s_dwFcsNumModulus = 100;

int SortBySequence( const void * pP1, const void * pP2 )
{
  CY_Base * pB1 = *(CY_Base**)pP1;
  CY_Base * pB2 = *(CY_Base**)pP2;
  if( !pB1 || !pB2 )
    return 0;
  if( pB1->sequenceNumberInDR < pB2->sequenceNumberInDR )
    return -1;
  if( pB1->sequenceNumberInDR > pB2->sequenceNumberInDR )
    return 1;
  return 0;
}

void CY_DR::Prepare0()
{
  if( !m_pBlks || !nYObjects )
    return;
  qsort( m_pBlks, nYObjects, sizeof(CY_Base*), SortBySequence );
}

void CY_DR::AddYBlk( CY_Base *pY )
{
  if( !nYObjects )
    return;
  if( pY->sequenceNumberInDR <=0 )
    return;
  if( !m_pBlks )
    m_pBlks = MemAlloc<CY_Base*>(nYObjects);
  for( int i=0; i<nYObjects; i++ )
  {
    if( NULL==m_pBlks[i] )
    {
      m_pBlks[i] = pY;
      return;
    }
  }
  ASS( 0 );
}

void CY_DR::AddPIO( CY_Base *pPIO )
{
  if( !nPIOObjects )
    return;
  if( pPIO->sequenceNumberInDR > 0 )
    return;
  if( !m_pPios )
    m_pPios = MemAlloc<CY_Base*>(nPIOObjects);
  for( int i=0; i<nPIOObjects; i++ )
  {
    if( NULL==m_pPios[i] )
    {
      m_pPios[i] = pPIO;
      return;
    }
  }
  ASS( 0 );
}

void CY_DR::WorkYBlks( BYTE Level, double dt, IConnectionsImpl *pCI )
{
  //типа PIO, AREAIN, AREAOUT
  for( int i=0; i<nPIOObjects; i++ )
  {
    if( NULL==m_pPios[i] )
      continue;
    if( m_pPios[i]->sequenceNumberInDR <= 0 && m_pPios[i]->sequenceNumberInDR > -10 )
      m_pPios[i]->Work( dt );
  }

  for( int i=0; i<nYObjects; i++ )
  {
    if( m_pBlks && m_pBlks[i] )
    {
      if( pCI && m_pBlks[i]->bExecutedAtStep )
        pCI->ConnectionExec( m_pBlks[i]->firstConnectionK, m_pBlks[i]->lastConnectionK, dt, 'I' );
      m_pBlks[i]->Work( dt );
      if( pCI && m_pBlks[i]->bExecutedAtStep )
        pCI->ConnectionExec( m_pBlks[i]->firstConnectionK, m_pBlks[i]->lastConnectionK, dt, 'O' );
    }
  }

  //типа PIO, AREAIN, AREAOUT
  for( int i=0; i<nPIOObjects; i++ )
  {
    if( NULL==m_pPios[i] )
      continue;
    if( (m_pPios[i]->sequenceNumberInDR <= 0 && m_pPios[i]->sequenceNumberInDR > -10) || -200==m_pPios[i]->sequenceNumberInDR )
      m_pPios[i]->Work( dt );
  }
}
