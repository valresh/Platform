#ifndef TAGINPUT_H
#define TAGINPUT_H
#include "tag.h"



enum EInputType
  {
  inputButton,
  inputText,
  inputCheckBox,
  };
//
class CTagINPUT : public CTag
  {
  public:
    CTagINPUT(void);
    virtual ~CTagINPUT() {}
    virtual bool Close();
    void Matrix( LOCALS );
    void Analiz( ANALIZ );
    void Action( CHTML& html, HWND hWnd, bool bDown,cross::CPoint * pt = NULL );
	bool ActionLG35_8_KF(CHTML& html, HWND hWnd, bool bDown);
	void CheckboxCallbackFromScript(CHTML& html, int v);
    //
    EInputType m_eInputType;
    char  m_szPopup[64];
    POINT m_ptPopup;
    int   m_nPos;
    //
  };

#endif
