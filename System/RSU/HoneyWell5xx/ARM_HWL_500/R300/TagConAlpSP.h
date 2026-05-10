#pragma once
#include "Tag.h"
// EditBox для значения SP
class CTagConAlpSP : public CTag
  {
	int m_nSelected;
  public:
    CTagConAlpSP(int n);
    void Matrix ( LOCALS );
    void Analiz ( ANALIZ );
    void Kerdyk ( KERDYK );
	void Select();
	void DeSelect();
  };
