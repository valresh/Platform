#include "LibImplContainer.h"

KLibImplContainer* KLibImplContainer::pFirst = NULL;
KLibImplContainer* KLibImplContainer::pLast = NULL;

KLibImplContainer::KLibImplContainer( LPCSTR pszName, tfOnProjectReady pfnCall )
{
  if( pFirst == NULL )
    pFirst = this;
  if( pLast )
    pLast->pNext = this;
  pLast = this;
  pNext = NULL;
  this->pszName = pszName;
  pfnOnProjectReady = pfnCall;
}

void KLibImplContainer::OnProjectReady( KFlexBlockBase::IProjectContainer *pCont )
{
  for( KLibImplContainer* pC = pFirst; pC; pC = pC->pNext )
  {
    if( !pC->pfnOnProjectReady )
      continue;
    pC->pfnOnProjectReady( pCont );
  }
}
