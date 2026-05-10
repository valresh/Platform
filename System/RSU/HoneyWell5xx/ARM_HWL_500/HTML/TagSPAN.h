#ifndef TAGSPAN_H
#define TAGSPAN_H
#include "tag.h"
#include "TagINPUT.h"

typedef bool (*AcceptCallback)(CHTML& html, CTag* tag, void* arg);
//
class CTagSPAN : public CTag
  {
  cross::SParamValueH mMin,mMax;
  public:
    CTagSPAN(void);
    virtual ~CTagSPAN() {}
    void Matrix( LOCALS );
    void Analiz( ANALIZ );
    void Kerdyk( KERDYK );
	static bool Accept(CHTML& html, CTag *_this, void *arg);
	virtual TagType GetType(){ return TT_SPAN; }

    COLORREF Bindind  ( HDC hDC, CHTML& html, double& dValue );
	CTagINPUT* m_pCheckBoxInput;
    AcceptCallback m_pAcceptCheckboxValueCallback;
  };

#endif
