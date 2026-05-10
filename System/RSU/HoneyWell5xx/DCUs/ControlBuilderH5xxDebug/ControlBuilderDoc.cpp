#include "stdafx.h"
#include "ControlBuilderDoc.h"
#include "ControlBuilderH5xxDebug.h"
#include <io.h>

// KControlBuilderDoc

IMPLEMENT_DYNCREATE(KControlBuilderDoc, CDocument)

KControlBuilderDoc::KControlBuilderDoc()
: m_entryModule( NULL )
, m_pcb( NULL )
{
}

BOOL KControlBuilderDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

KControlBuilderDoc::~KControlBuilderDoc()
{
}

LPCSTR KControlBuilderDoc::GetFile()
{
  return m_szActiveXml.c_str();
}

LPCSTR KControlBuilderDoc::GetBlkName()
{
  return m_szActiveBlk.c_str();
}

BEGIN_MESSAGE_MAP(KControlBuilderDoc, CDocument)
END_MESSAGE_MAP()


// KControlBuilderDoc diagnostics

#ifdef _DEBUG
void KControlBuilderDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void KControlBuilderDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// KControlBuilderDoc serialization

void KControlBuilderDoc::Serialize(CArchive& ar)
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


// KControlBuilderDoc commands
void KControlBuilderDoc::OnSelectCnf( LPCSTR pszCnfFile, LPCSTR pszBlkName )
{
  char Path[_MAX_PATH] = { 0 };
  m_pcb->GetPathToCnfFile( Path, _countof(Path), pszCnfFile, NULL );

  errno_t Res = _access_s( Path, 4 );
  if( Res )
    return;

  SetTitle( pszBlkName );
  m_szActiveBlk = pszBlkName;

  m_szActiveXml = Path;

  POSITION pos = GetFirstViewPosition();
  CView *pView = GetNextView( pos );
  if( pView )
  {
    CFrameWnd *pFr = pView->GetParentFrame();
    if( pFr )
      pFr->SendMessageToDescendants( WM_USER, (WPARAM)pszCnfFile, (LPARAM)pszBlkName, TRUE, TRUE);
  }
}
