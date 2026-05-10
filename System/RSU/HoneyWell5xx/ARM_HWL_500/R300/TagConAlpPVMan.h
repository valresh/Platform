#pragma once
#include "Tag.h"
// EditBox для значения PV
class CTagConAlpPVMan : public CTag
  {
  public:
    CTagConAlpPVMan(int n);
    void Matrix ( LOCALS );
    void Analiz ( ANALIZ );
    void Kerdyk ( KERDYK );
  };
