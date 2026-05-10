#include <crossplatform.h>
#include "BlockCreator.h"
#include <rsuStr.h>

KBlockCreator * KBlockCreator::pFirst = NULL;
KBlockCreator * KBlockCreator::pLast = NULL;

KBlockCreator::KBlockCreator( LPCSTR pType,	tfCreateNP pCreate )
{
  if( pFirst == NULL )
    pFirst = this;
  if( pLast )
    pLast->pNext = this;
  pLast = this;
  pNext = NULL;
  this->pszType = pType;
  fnCreateNP = pCreate;
}


KFlexBlockBase * KBlockCreator::Create( LPCSTR pType )
{
  for( KBlockCreator * pC = pFirst; pC; pC = pC->pNext )
  {
    if(_Kstricmp( pType, pC->pszType ) )
      continue;
    KFlexBlockBase *pB = NULL;
    if( pC->fnCreateNP )
      pB = (*(pC->fnCreateNP))();
    if( pB )
      pB->m_pszType = pC->pszType;
    return pB;
  }
  return NULL;
}
