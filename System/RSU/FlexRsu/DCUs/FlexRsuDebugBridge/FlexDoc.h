#pragma once
#include "../FlexRsuCore/FlexDR.h"
#include <macros/INotifier.h>
#include <vector>

enum eEventsPublic
{
  eSelectedDR = 100,
  eSelectedBlock,
  eScrollToBlock,
};

class KFlexDoc 
  : public CDocument
  , private INotifierOwner
{
	DECLARE_DYNCREATE(KFlexDoc)

public:
	KFlexDoc();
	virtual ~KFlexDoc();
  void operator = ( const SPrjCfg &thr );
  bool IsEmpty();
  const SPrjCfg& GetConfig();
  INotifier* GetEventNotifier( int id );

#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
public:
   CString m_initialFind;
protected:
  notifiers_vector m_Notifiers;
  std::vector<INotifier::CONNECTION> m_connections;

  notifier_ptr CreateNotifier();
  //INotifierOwner
  virtual void OnAdvised(){}
  virtual void OnUnadvised(){}

  SPrjCfg m_Config;
protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
};
