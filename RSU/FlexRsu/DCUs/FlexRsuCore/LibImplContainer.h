#pragma once
#include "FlexBlockBase.h"

typedef void (*tfOnProjectReady)( KFlexBlockBase::IProjectContainer *pCont );

class FLEXRSUCORE_API KLibImplContainer
{
public:
  KLibImplContainer( LPCSTR pszName, tfOnProjectReady pfnCall );
  static void OnProjectReady( KFlexBlockBase::IProjectContainer *pCont );
private:
  KLibImplContainer()
  {}
private:
  static KLibImplContainer* pFirst;
  static KLibImplContainer* pLast;
private:
  KLibImplContainer* pNext;
  LPCSTR pszName;
  tfOnProjectReady pfnOnProjectReady;
};
