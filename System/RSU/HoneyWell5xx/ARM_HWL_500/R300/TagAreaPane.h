#pragma once
#include "Tag.h"

struct TreeItem{
	std::string location;
	HTREEITEM item;
	int count;
	int prior;
	int ack;
};

class CHTML_AreaPane : public CHTMLWndBase
{
public:
	BOOL m_bShown;
	CViewAlarm* m_pAlarmView;
	CMsgAL* msgAL;
	class LocTree* wnd;
	CHTML_AreaPane(CHTML& _html);
	~CHTML_AreaPane();
	bool WindowPos( WND_POS );
	void Timer() ;
	void SetViewAlarm(CViewAlarm* alarmView);
	void Show();
	void Hide();
};

class LocTree: public CTreeCtrl{
	CHTML_AreaPane* mpParent;
	CImageList* m_imgList;
	HTREEITEM itemAlmGroups;
	HTREEITEM itemAssets;
public:
	CHTML* html;
	std::vector<TreeItem> items;
	std::vector<TreeItem> gitems;
	CMsgAL* msgAL;
	CViewAlarm* m_pAlarmView;
	LocTree();
	LocTree(CHTML_AreaPane* parent, CHTML& _html);
	void Timer();
	void ItemsTimer();
	void GItemsTimer();
	BOOL MyCreate();
	BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	void OnItemSelect(NMHDR* n, LRESULT* l);	
// As soon as this edit loses focus, kill it.
	void OnKillFocus(CWnd* pNewWnd);
    DECLARE_DYNCREATE(LocTree)	
	DECLARE_MESSAGE_MAP()
};

class CTagAreaPane : public CTag
  {
  public:
    CTagAreaPane();
    virtual ~CTagAreaPane();
    void Analiz ( ANALIZ );
    void Kerdyk ( KERDYK );
    void Matrix ( LOCALS );
	bool Close();
	void Release();
    void Show();
    void Hide();
	void ToggleView();
	void SetViewAlarm(CViewAlarm*);
  };
