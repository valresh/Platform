#pragma once
#include "tag.h"
#include "Shape.h"
#include "TagDIV.h"
// –азные виды радиокнопок, в зависимости от OP
class CTagShpGOPState : public CTagDIV
  {
  public:
    CTagShpGOPState(int n);
    void Analiz ( ANALIZ );
    void Kerdyk ( KERDYK );
    virtual void PaintThis( LOCALS );
	void Action( CHTML& html, HWND hWnd, bool bDown, CPoint * pt );
  };
