#pragma once
#include "tag.h"
#include "Shape.h"
#include "TagDIV.h"
// –азные виды радиокнопок, в зависимости от PV
class CTagShpGPVState : public CTagDIV
  {
  public:
    CTagShpGPVState(int n);
    void Analiz ( ANALIZ );
    void Kerdyk ( KERDYK );
    virtual void PaintThis( LOCALS );
  };
