#pragma once
#include "Tag.h"
// ComboBox с параметрами ModeAttr
class CTagConCboModeAttr : public CTag
  {
  public:
    CTagConCboModeAttr(int n);
    void Analiz ( ANALIZ );
    void Kerdyk ( KERDYK );
  };
