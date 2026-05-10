#pragma once
#include "Tag.h"
// EditBox для значения OP
class CTagConAlpOP : public CTag
  {
	int m_nSelected;
  public:
    CTagConAlpOP(int n);
    void Matrix ( LOCALS );
    void Analiz ( ANALIZ );
    void Kerdyk ( KERDYK );
	void Select();
	void DeSelect();
  };
