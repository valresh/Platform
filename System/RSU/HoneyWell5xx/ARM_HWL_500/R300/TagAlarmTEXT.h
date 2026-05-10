#pragma once
#include "Tag.h"
// Тип сигнализации
class CTagAlarmTEXT : public CTag
  {
  public:
    CTagAlarmTEXT(int n);
    void Matrix ( LOCALS );
    void Analiz ( ANALIZ );
    void Kerdyk ( KERDYK );
  };
