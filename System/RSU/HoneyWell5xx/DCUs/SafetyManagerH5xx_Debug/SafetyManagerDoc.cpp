// SafetyManagerDoc.cpp : implementation file
//

#include "stdafx.h"
#include "SafetyManager.h"
#include "SafetyManagerDoc.h"


// KSafetyManagerDoc

IMPLEMENT_DYNCREATE(KSafetyManagerDoc, CDocument)

KSafetyManagerDoc::KSafetyManagerDoc()
: m_pOwner( NULL )
{
}

BOOL KSafetyManagerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

KSafetyManagerDoc::~KSafetyManagerDoc()
{
}


BEGIN_MESSAGE_MAP(KSafetyManagerDoc, CDocument)
END_MESSAGE_MAP()


// KSafetyManagerDoc diagnostics

#ifdef _DEBUG
void KSafetyManagerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void KSafetyManagerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// KSafetyManagerDoc serialization

void KSafetyManagerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}
#endif


// KSafetyManagerDoc commands
void KSafetyManagerDoc::OnSelectFsc( LPCSTR pszFsc, UINT nFile )
{
  POSITION pos = GetFirstViewPosition();
  CView *pView = GetNextView( pos );
  if( pView )
  {
    CFrameWnd *pFr = pView->GetParentFrame();
    if( pFr )
      pFr->SendMessageToDescendants( WM_USER, (WPARAM)pszFsc, (LPARAM)nFile, TRUE, TRUE);
  }
}
