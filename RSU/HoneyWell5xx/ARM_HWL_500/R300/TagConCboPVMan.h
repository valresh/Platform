#pragma once
#include "Tag.h"
// ComboBox с параметрами PV
class CTagConCboPVMan : public CTag
  {
  public:
    CTagConCboPVMan(int n);
    void Analiz ( ANALIZ );
    void Kerdyk ( KERDYK );
  };
