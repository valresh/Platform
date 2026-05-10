 #pragma once
#ifndef TagBODY_H
#define TagBODY_H
#include "tag.h"

class CСTagBODY : public CTag
  {
  public:
    CСTagBODY(void);
    virtual ~CСTagBODY(void) {}
    void Analiz( ANALIZ );
    void Kerdyk ( KERDYK );
  };
#endif
