#pragma once
#include "TagDIV.h"
// Индикация сигнализации
class CTagAlarmState001 : public CTagDIV
  {
  public:
    CTagAlarmState001(int n);
    void Matrix ( LOCALS );
    void Analiz ( ANALIZ );
    void Kerdyk ( KERDYK );
  };
