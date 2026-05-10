#ifndef TAGSCRIPT_H
#define TAGSCRIPT_H
//
#include "HTML.h"
#include "VBScript.h"
//
enum ELanguage
  {
  langUnk = -1,
  langVBScript,
  langJScript ,
  };
//
enum EventType
  {
  eventUnk = -1,
  eventOnUpdate,
  eventOnDataChange,
  eventOnClick,
  eventOnChange,
  eventOnShapeLoad,
  eventOnMouseDown,
  eventOnMouseUp,
  eventOnPeriodIcupdate,
  eventOnPageComplete,
  };
//
EventType GetEventType(const char * value);
//
class CTagSCRIPT : public CTag
  {
  public:
    CTagSCRIPT(void);
    virtual ~CTagSCRIPT() {}
    void Matrix( LOCALS );
    void Analiz( ANALIZ );
    void Kerdyk( KERDYK );
    void Script( KERDYK );
  //
  int RunScript();
  //
  CTag*     m_pLinkS;
  ELanguage m_eLang;
  EventType m_eType;
  CVBScript mScript;
  };

#endif
