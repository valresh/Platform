#pragma once
#include "tag.h"
#include "Shape.h"
//#include "../ViewAlarm.h"
#include "linuxTypes/CString.h"
#include "linuxTypes/cpoint.h"

#include "HTML.h"
//
enum LinkType
{
  ltUnknown,
  ltEmbedded,
  ltLinked
};

class CTagDIV : public CTag
  {
  void AlarmEnbState( LOCALS );
  public:
	void ApplyStyles(LOCALS);
    CTagDIV(void);
    virtual ~CTagDIV();
    void Matrix ( LOCALS );
    void Analiz ( ANALIZ );
    void Kerdyk ( KERDYK );
    void TextBox( HDC hDC, CHTML& html );
    void SpecificAlarm ( CHTML& html );
    void SpecificButton( CHTML& html );
  //  void Action( CHTML& html, HWND hWnd, bool bDown,CPoint * pt = NULL );
	void Change140ProjectAlarmFillColor( HDC hDC, CHTML& html, CStyle& style,char* id );
  //  virtual void PaintThis( LOCALS );
    //
    bool Shape( CHTML& html );
    bool Shape( CHTML& html, CFileXML& xml );
    bool Shape( CHTML& html, CFileXML& xml, HXML nShape, CTag* shape );
    bool Group( CHTML& html, CFileXML& xml, HXML nGroup, CTag* shape );
    bool Eleme( CHTML& html, CFileXML& xml, HXML nItem , CTag* shape );
	bool DrawTSoft2PumpsIn1(HDC hDC,CHTML& html, CStyle& style);
    bool IsBadPropertiesInProject(cross::CString strFileName);
    cross::CString GetCorrectPropertiesString(cross::CString strFileName, cross::CString strValue);
    void SetAlamColor(CHTML& html, CStyle& style);

    int m_useFirstShapeForBadValue;
    int m_numberOfShapesAnimated;
    cross::SParamValueH m_valHI; // max для mVal
    cross::SParamValueH m_valLO; // min для mVal
  //  CViewAlarm* m_pViewAlarm;
    int m_nDivValue;
    LinkType m_linkType;
    //
  };
