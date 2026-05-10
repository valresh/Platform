#pragma once
#include "TagDIV.h"

class CTagShpX : public CTagDIV
{
public:
	CTagShpX(int n);
    void Analiz ( ANALIZ );
    void Kerdyk ( KERDYK );
    virtual void PaintThis( LOCALS );
};

class CTagScShpX : public CTagDIV
{
	SParamValueH mVEdit;
public:
	CTagScShpX(int n);
    void Analiz ( ANALIZ );
    void Kerdyk ( KERDYK );
	virtual void Action( CHTML& html, HWND hWnd, bool bDown,CPoint * pt = NULL );
};
