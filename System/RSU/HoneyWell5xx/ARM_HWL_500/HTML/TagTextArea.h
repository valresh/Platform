#ifndef TAGTEXTAREA_H
#define TAGTEXTAREA_H
#include "tag.h"
//
#include "TagSPAN.h"


class CTagTextArea : public CTag
  {
  public:
	void ApplyStyles(LOCALS);
    CTagTextArea(void);
    virtual ~CTagTextArea() {}
    void Matrix( LOCALS );
    void Analiz( ANALIZ );
    void Kerdyk( KERDYK );
    //ниже определение действий при записи в привязанную пременную Value
    virtual TagType GetType(){ if(m_pAcceptValueCallback) return TT_TEXTAREAWITHCALLBACK; return TT_TAG; }
    static bool Accept(CHTML& html, CTag *_this, void *arg);
    AcceptCallback m_pAcceptValueCallback;
  };

#endif
