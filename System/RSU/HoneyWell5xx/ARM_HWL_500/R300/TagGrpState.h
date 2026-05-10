#pragma once
#include "tag.h"
#include "Shape.h"
#include "TagDIV.h"
// Видимость группы по тексту
// DEVCTLA.STATETEXT(6)
class CTagGrpState : public CTagDIV
  {
  public:
    CTagGrpState(int n);
    void Matrix ( LOCALS );
    void Analiz ( ANALIZ );
    void Kerdyk ( KERDYK );
    virtual void PaintThis( LOCALS );
  };
