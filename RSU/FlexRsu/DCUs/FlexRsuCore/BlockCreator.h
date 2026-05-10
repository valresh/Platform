#pragma once
#include "FlexBlockBase.h"

class FLEXRSUCORE_API KBlockCreator
{
  typedef KFlexBlockBase* (*tfCreateNP)();

  static KBlockCreator * pFirst;
  static KBlockCreator * pLast;

  KBlockCreator * pNext;
  LPCSTR pszType;
  tfCreateNP fnCreateNP;
public:
  KBlockCreator( LPCSTR pType, tfCreateNP pCreate );
  static KFlexBlockBase * Create( LPCSTR pType );
};
