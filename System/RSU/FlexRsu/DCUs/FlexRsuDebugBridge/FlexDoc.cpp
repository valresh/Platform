#include "stdafx.h"
#include "FlexRsuDebugBridge.h"
#include "FlexDoc.h"


// KFlexDoc

IMPLEMENT_DYNCREATE(KFlexDoc, CDocument)

KFlexDoc::KFlexDoc()
{
  m_Notifiers.insert( notifiers_vector::value_type( eSelectedDR, CreateNotifier() ) );
  m_Notifiers.insert( notifiers_vector::value_type( eSelectedBlock, CreateNotifier() ) );
  m_Notifiers.insert( notifiers_vector::value_type( eScrollToBlock, CreateNotifier() ) );
}

BOOL KFlexDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

KFlexDoc::~KFlexDoc()
{
}

void KFlexDoc::operator = ( const SPrjCfg &thr )
{
  m_Config = thr;
}

bool KFlexDoc::IsEmpty()
{
  return m_Config.ppDR ? false : true;
}

const SPrjCfg& KFlexDoc::GetConfig()
{
  return m_Config;
}

notifier_ptr KFlexDoc::CreateNotifier()
{
  return notifier_ptr( new notifier(this) );
}

INotifier* KFlexDoc::GetEventNotifier( int id )
{
  notifiers_vector::iterator iter = m_Notifiers.find( id );
  if( iter==m_Notifiers.end() )
  {
    ASSERT( 0 );
    return NULL;
  }

  return iter->second.get();
}

BEGIN_MESSAGE_MAP(KFlexDoc, CDocument)
END_MESSAGE_MAP()


// KFlexDoc diagnostics

#ifdef _DEBUG
void KFlexDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void KFlexDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// KFlexDoc serialization

void KFlexDoc::Serialize(CArchive& ar)
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


// KFlexDoc commands
