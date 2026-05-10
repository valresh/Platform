#pragma once
#include "TagDIV.h"

class CTagConShpLstatus : public CTagDIV
{
public:
	CTagConShpLstatus(int n);
	void Matrix ( LOCALS );
    void Kerdyk ( KERDYK );
	void PaintThis( LOCALS );
};
