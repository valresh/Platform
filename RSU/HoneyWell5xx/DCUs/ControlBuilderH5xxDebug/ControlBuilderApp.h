#pragma once
#include "resource.h"
#include <H_Data.h>
#include <IControlBuilder.h>


// KControlBuilderApp

class KControlBuilderApp : public CWinThread
{
	DECLARE_DYNCREATE(KControlBuilderApp)

public:
	KControlBuilderApp();           // protected constructor used by dynamic creation
	virtual ~KControlBuilderApp();

	virtual BOOL InitInstance();
	virtual int ExitInstance();

  void Open( IControlBuilder *pcb, SH_Module * entryModule, LPCSTR pszSelectName );
  CFrameWnd* IsAnyOpen( SH_Module *pEntry );

protected:
  CDocManager* m_pDocManager;
protected:
	DECLARE_MESSAGE_MAP()
};

//extern KControlBuilderApp theApp;
