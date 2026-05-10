#pragma once
#include "Tag.h"
// Значение PV
class CTagConAlpPV : public CTag
  {
  public:
    CTagConAlpPV(int n);
    void Matrix ( LOCALS );
    void Analiz ( ANALIZ );
    void Kerdyk ( KERDYK );
  };
