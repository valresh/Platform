#include "stdafx.h"
#include "SafetyManager.h"
#include "ComList.h"


// KComList

IMPLEMENT_DYNCREATE(KComList, KFldComList)

KComList::KComList()
{
  m_SelectItemType = LFscBase::COM;
}

KComList::~KComList()
{
}

BEGIN_MESSAGE_MAP(KComList, KFldComList)
END_MESSAGE_MAP()


// KComList diagnostics

#ifdef _DEBUG
void KComList::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void KComList::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// KComList message handlers
