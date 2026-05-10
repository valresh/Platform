#pragma once
#include "tag.h"
#include "Shape.h"
#include "TagDIV.h"
// –азные виды радиокнопок, в зависимости от PVFL
class CTagShpPVFLState : public CTagDIV
  {
  public:
    CTagShpPVFLState(int n);
    void Analiz ( ANALIZ );
    void Kerdyk ( KERDYK );
    virtual void PaintThis( LOCALS );
	void Action( CHTML& html, HWND hWnd, bool bDown );
  };
