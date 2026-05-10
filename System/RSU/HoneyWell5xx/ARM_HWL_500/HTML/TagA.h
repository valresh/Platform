// #pragma once
#include "tag.h"
#include <crosswindows.h>
//
class CTagA : public CTag
  {
  public:
    std::string href;

    CTagA();
    virtual ~CTagA() {}
    void Matrix( LOCALS );
    void Analiz( ANALIZ );
    void Kerdyk( KERDYK );
    virtual void Action( CHTML& html, HWND hWnd, bool bDown, cross::CPoint * pt = NULL );
  };
