#pragma once
#include "TagDIV.h"
// Уровни сигнализаций
class CTagConShpAlmPr : public CTagDIV
  {
	char ttip[1024];
	HWND m_hToolTip;
	TOOLINFO m_ToolInfo;
  public:
    CTagConShpAlmPr(int n);
    void Matrix ( LOCALS );
    void Analiz ( ANALIZ );
    void Kerdyk ( KERDYK );
  };
