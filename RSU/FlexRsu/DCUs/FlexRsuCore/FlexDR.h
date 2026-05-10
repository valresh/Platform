#pragma once
#include "FlexBlockBase.h"

class FLEXRSUCORE_API KFlexDR
{
public:
  KFlexDR();
  void *operator new( size_t size );
  KFlexBlockBase *GetBlock( GUID id );
  KFlexBlockBase *GetBlock( LPCSTR pszName );
  void LinkConnections();
  void OnProjectReady( KFlexBlockBase::IProjectContainer *pCont );
  int StepCalc( KFlexBlockBase::SCallParams &params );
  void OnAfterRestoreState();
public:
  char m_szFileName[64 * 4];
  DWORD m_nBlocks;
  KFlexBlockBase **m_ppBlocks;
  DWORD m_nCons;
  KFlexConnection **m_ppConnections;
protected:
};
