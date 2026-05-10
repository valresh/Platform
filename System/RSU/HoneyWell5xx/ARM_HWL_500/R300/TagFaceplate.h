#pragma once
#include "Tag.h"
// Faceplate
class CTagFaceplate : public CTag
  {
  public:
    CTagFaceplate(int n);
    void Analiz ( ANALIZ );
    void Kerdyk ( KERDYK );
  };
