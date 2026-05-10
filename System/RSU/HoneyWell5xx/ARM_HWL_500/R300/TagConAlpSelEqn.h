#pragma once
#include "TagDIV.h"

class CTagConAlpSelEqn : public CTagDIV
{
public:
	CTagConAlpSelEqn(int n);
    void Matrix ( LOCALS );
    void Analiz ( ANALIZ );
    void Kerdyk ( KERDYK );
};
