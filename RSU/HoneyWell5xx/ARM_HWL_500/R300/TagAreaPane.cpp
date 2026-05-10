#include "StdAfx.h"
#include "../ARM.h"
#include "../AplicFrm.h"
#include "TagAreaPane.h"
#include <string>


IMPLEMENT_DYNCREATE(LocTree,CTreeCtrl)

BEGIN_MESSAGE_MAP(LocTree, CTreeCtrl)
  ON_NOTIFY(TVN_SELCHANGED, 37000, OnItemSelect)
  ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

LocTree::LocTree()
	: m_imgList(NULL)
	, m_pAlarmView(NULL)
	, msgAL(&GetAppWnd()->m_msgAL)
	, mpParent(NULL)
	, itemAlmGroups(NULL)
	, itemAssets(NULL)
	, items()
	, gitems()
	, html(NULL) { }
LocTree::LocTree(CHTML_AreaPane* parent, CHTML& _html)
	: m_imgList(NULL)
	, m_pAlarmView(NULL)
	, msgAL(&GetAppWnd()->m_msgAL)
	, mpParent(parent)
	, itemAlmGroups(NULL)
	, itemAssets(NULL)
	, items()
	, gitems()
	, html(&_html) { }

BOOL LocTree::PreCreateWindow(CREATESTRUCT& cs)
{
	m_imgList =  new CImageList();
	m_imgList->Create(16, 16, ILC_COLOR32, 8, 1);
	
	static std::string ID[] =
    {
		"blueNotAck.png", "blueAck.png"  , "yellNotAck.png",
		"yellAck.png"   , "redNotAck.png", "redAck.png"    ,
		"ghost.png"     , "shelved.png"  , "node_clh.png"
    };

	for (int i = 0; i< 9; i++)
	{
		std::string path = GetBasePath();
		path += "Data\\Honeywell_Alarms\\";
		path += ID[i];

		CImage img;
		img.Load(path.c_str());
		CBitmap bmp;
		bmp.Attach(img.Detach());

		//m_imgList->Add(&bmp, (COLORREF)0xffffffff);
		m_imgList->Add(&bmp, RGB(0,0,0));
	}	
	return TRUE;
}

BOOL LocTree::MyCreate()
{
	RECT r = {0, 65, 250, 785};
	RECT r1;
	this->CreateEx(WS_EX_COMPOSITED, TVS_HASLINES | TVS_HASBUTTONS | /*TVS_LINESATROOT |*/ WS_BORDER | WS_CHILD, r, this->html->mWnd, 37000);
	this->SetImageList(m_imgList, TVSIL_NORMAL);
	std::vector<std::string>* assets;

	this->itemAlmGroups = this->InsertItem("Alarm Groups", 8, 8, TVI_ROOT, TVI_LAST);
	this->SetItemState(this->itemAlmGroups, TVIS_EXPANDED, TVIS_EXPANDED);
	assets = GetAlmGroups();
	for (std::vector<std::string>::iterator itr = assets->begin(); itr != assets->end(); itr++)
	{
		TreeItem ti;
		ti.location = *itr;
		ti.item = this->InsertItem(itr->c_str(), 8, 8, this->itemAlmGroups);
		ti.count = 0;
		gitems.push_back(ti);
	}

	this->itemAssets = this->InsertItem("Assets", 8, 8, TVI_ROOT, TVI_LAST);
	this->SetItemState(this->itemAssets, TVIS_EXPANDED, TVIS_EXPANDED);
	assets = GetAssets();
	for (std::vector<std::string>::iterator itr = assets->begin(); itr != assets->end(); itr++)
	{
		TreeItem ti;
		ti.location = *itr;
		ti.item = this->InsertItem(itr->c_str(), 8, 8, this->itemAssets);
		ti.count = 0;
		items.push_back(ti);
	}/**/

	::ShowWindow(this->m_hWnd, SW_HIDE);
	return TRUE;
}
void LocTree::ItemsTimer()
{
	for (std::vector<TreeItem>::iterator it = items.begin(); it != items.end(); it++)
	{
		it->count = 0;
		it->prior = 0;
		it->ack = 1;
	}
	SAlarms* out = msgAL->Data();
	int nCount   = msgAL->GetCount();
	for (int i = 0; i< nCount; i++)
	{
		for (std::vector<TreeItem>::iterator it = items.begin(); it != items.end(); it++)
		{
			if (out[i].szLocation && !it->location.compare( out[i].szLocation))
			{
				it->prior = (it->prior < out[i].btEHLN)?(out[i].btEHLN):(it->prior);
				it->ack *= out[i].btKvit;
				it->count++;
			}

		}			
	}
	//HTREEITEM  sel = this->wnd->GetSelectedItem();
	for (std::vector<TreeItem>::iterator it = items.begin(); it != items.end(); it++)
	{
		if (it->count > 0)
		{
			char label[32];
			sprintf(label, "%s (%d)", it->location.c_str(), it->count);
			CString str = this->GetItemText(it->item);
			//if (str.Compare(label))
			{
				this->SetItemText(it->item, label);
				this->SetItemState(it->item, TVIS_BOLD, TVIS_BOLD);
				switch(it->prior)
				{
				case 0: 
					this->SetItemImage(it->item, 0, 0); 
					break;
				case 3: 
					this->SetItemImage(it->item, 3, 3); 
					break;
				case 4: 
					this->SetItemImage(it->item, 5, 5); 
					break;
				default: 
					this->SetItemImage(it->item, 6, 6); 
					break;
				}
			}
		}
		else
		{
			CString str = this->GetItemText(it->item);
			//if (str.Compare(it->location.c_str()))
			{
				this->SetItemText(it->item, it->location.c_str());
				this->SetItemState(it->item, TVIS_BOLD, 0);
				this->SetItemImage(it->item, 8, 8);
			}
		}
	}
}
void LocTree::GItemsTimer()
{
	for (std::vector<TreeItem>::iterator it = gitems.begin(); it != gitems.end(); it++)
	{
		it->count = 0;
		it->prior = 0;
		it->ack = 1;
	}
	SAlarms* out = msgAL->Data();
	int nCount   = msgAL->GetCount();
	for (int i = 0; i< nCount; i++)
	{
		for (std::vector<TreeItem>::iterator it = gitems.begin(); it != gitems.end(); it++)
		{
			if (out[i].szGrp && !it->location.compare( out[i].szGrp))
			{
				it->prior = (it->prior < out[i].btEHLN)?(out[i].btEHLN):(it->prior);
				it->ack *= out[i].btKvit;
				it->count++;
			}

		}			
	}
	//HTREEITEM  sel = this->wnd->GetSelectedItem();
	for (std::vector<TreeItem>::iterator it = gitems.begin(); it != gitems.end(); it++)
	{
		if (it->count > 0)
		{
			char label[32];
			sprintf(label, "%s (%d)", it->location.c_str(), it->count);
			CString str = this->GetItemText(it->item);
			//if (str.Compare(label))
			{
				this->SetItemText(it->item, label);
				this->SetItemState(it->item, TVIS_BOLD, TVIS_BOLD);
				switch(it->prior)
				{
				case 0: 
					this->SetItemImage(it->item, 0, 0); 
					break;
				case 3: 
					this->SetItemImage(it->item, 3, 3); 
					break;
				case 4: 
					this->SetItemImage(it->item, 5, 5); 
					break;
				default: 
					this->SetItemImage(it->item, 6, 6); 
					break;
				}
			}
		}
		else
		{
			CString str = this->GetItemText(it->item);
			//if (str.Compare(it->location.c_str()))
			{
				this->SetItemText(it->item, it->location.c_str());
				this->SetItemState(it->item, TVIS_BOLD, 0);
				this->SetItemImage(it->item, 8, 8);
			}
		}
	}
}
void LocTree::Timer()
{
	this->ItemsTimer();
	this->GItemsTimer();
	this->UpdateWindow();
}
void LocTree::OnKillFocus(CWnd* pNewWnd)
{
	mpParent->Hide();
}
void LocTree::OnItemSelect(NMHDR* pNMHDR, LRESULT* l)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	if (this->m_pAlarmView )
	{
		for (std::vector<TreeItem>::iterator it = items.begin(); it != items.end(); it++)
		{
			if (it->item == pNMTreeView->itemNew.hItem)
			{
				this->m_pAlarmView->ClearAllFilters();
				this->m_pAlarmView->SetFilterByLocation(it->location.c_str());
				return;
			}
		}
		for (std::vector<TreeItem>::iterator it = gitems.begin(); it != gitems.end(); it++)
		{
			if (it->item == pNMTreeView->itemNew.hItem)
			{
				this->m_pAlarmView->ClearAllFilters();
				this->m_pAlarmView->SetFilterByAlmGroup(it->location.c_str());
				return;
			}
		}
	}
}

CHTML_AreaPane::CHTML_AreaPane(CHTML& _html)
	: CHTMLWndBase( _html )
	, msgAL(&GetAppWnd()->m_msgAL)
	, wnd(NULL)
	, m_pAlarmView(NULL)
	, m_bShown(FALSE) {}

CHTML_AreaPane::~CHTML_AreaPane()
{
	delete this->wnd;
	this->wnd = NULL;
}

bool CHTML_AreaPane::WindowPos( WND_POS )
{
	if (!this->wnd )
	{
		this->wnd = new LocTree(this, html);
		return this->wnd->MyCreate();
	}
	return true;
}

void CHTML_AreaPane::Timer() 
{
	this->wnd->m_pAlarmView = m_pAlarmView;
	this->wnd->Timer();
}

void CHTML_AreaPane::SetViewAlarm(CViewAlarm* alarmView)
{
	this->m_pAlarmView = alarmView;
}

void CHTML_AreaPane::Show()
{
	this->m_bShown= TRUE;
	this->wnd->ShowWindow(SW_SHOW);
	::SetFocus(this->wnd->m_hWnd);
}

void CHTML_AreaPane::Hide()
{
	this->m_bShown= FALSE;
	this->wnd->ShowWindow(SW_HIDE);
}

CTagAreaPane::CTagAreaPane()
{
}
//
void CTagAreaPane::Analiz( ANALIZ )
{
	//AnalizDIV(this, html, teg);
	//AnalizDIV( this, html, teg );
}
//
void CTagAreaPane::Kerdyk( KERDYK )
{
	CTag::Kerdyk( hWnd, html );
	m_pWnd = new CHTML_AreaPane(html);
	m_pWnd->WindowPos(NULL, 0, 0, 0, Gdiplus::Rect());
	//CTag** tags = this->m_pPar->GetData();
}

void CTagAreaPane::Matrix ( LOCALS )
{
  CTag::Matrix(hDC, html);
  if(m_pWnd==NULL)
    return;
  if (((CHTML_AreaPane*)this->m_pWnd)->m_bShown)
  {
	  ::ShowWindow(this->m_pWnd->m_hWnd, SW_SHOW);
	  this->m_pWnd->Timer();
  }
  else{
	  ::ShowWindow(this->m_pWnd->m_hWnd, SW_HIDE);
  }
}
void CTagAreaPane::Show()
{
	((CHTML_AreaPane*)this->m_pWnd)->m_bShown = TRUE;
	((CHTML_AreaPane*)this->m_pWnd)->Show();
}
void CTagAreaPane::Hide()
{
	((CHTML_AreaPane*)this->m_pWnd)->m_bShown = FALSE;
	((CHTML_AreaPane*)this->m_pWnd)->Hide();
}
void CTagAreaPane::ToggleView()
{
	if (((CHTML_AreaPane*)this->m_pWnd)->m_bShown){
		this->Hide();
	}
	else{
		this->Show();
	}
}
void CTagAreaPane::SetViewAlarm(CViewAlarm* alarmView)
{
	((CHTML_AreaPane*)m_pWnd)->SetViewAlarm(alarmView);
}
bool CTagAreaPane::Close()
{
	AAA();
	return CTag::Close();
}
void CTagAreaPane::Release()
{
	AAA();
}
CTagAreaPane::~CTagAreaPane()
{
	delete m_pWnd;
	m_pWnd = NULL;
}
//
