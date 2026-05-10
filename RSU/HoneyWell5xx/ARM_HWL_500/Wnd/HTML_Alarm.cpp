#include "StdAfx.h"
//
#include "HTMLWndBase.h"
#include "../ViewAlarm.h"
#include "../ViewSysStatus.h"
#include "../ViewEvent.h"
#include "../ViewMessages.h"
#include "../R300/TagAreaPane.h"
#include "HTML.h"
//
static SHeader stdData[] =
	{
  {""               ,   20, true, false, CMsgAL::Image       , 0},
  {"Date && Time "  ,  100, true, false, CMsgAL::DateTime    , 1},
// Sergej В душе не парю, что здесь выводить
  {"Location Tag  " ,   85, true, false, CMsgAL::LocationTag , 2},
  {"Source "        ,  150, true, false, CMsgAL::Source      , 3},
  {"Condition "     ,   85, true, false, CMsgAL::Condition   , 4},
  {"Priority "      ,   75, true, false, CMsgAL::Priority    , 5},
  {"Description "   ,  230, true, false, CMsgAL::Description , 6},
  {"Trip Value "    ,   90, true, false, CMsgAL::TripValue   , 7},
  {"Live Value "    ,   90, true, false, CMsgAL::LiveValue   , 8},
  {"Units "         ,10000, true, false, CMsgAL::Units       , 9},
  {"AlmGroup "      ,    0, true, false, CMsgAL::LocationTag ,10},
  {"END"            ,    0, true, false},
  {NULL}
	};
//
static SHeader stdDataRus[] =
	{
  {""               ,   20, true, false, CMsgAL::Image       , 0},
  {"Дата и время "  ,  100, true, false, CMsgAL::DateTime    , 1},
  {"Ассет "         ,   85, true, false, CMsgAL::LocationTag , 2},
  {"Источник "      ,  150, true, false, CMsgAL::Source      , 3},
  {"Условие "       ,   85, true, false, CMsgAL::Condition   , 4},
  {"Приоритет "     ,   75, true, false, CMsgAL::Priority    , 5},
  {"Описание "      ,  230, true, false, CMsgAL::Description , 6},
  {"Уставка "       ,   90, true, false, CMsgAL::TripValue   , 7},
  {"Текущее значение ", 90, true, false, CMsgAL::LiveValue   , 8},
  {"Ед. изм "       ,10000, true, false, CMsgAL::Units       , 9},
  {"AlmGroup "      ,    0, true, false, CMsgAL::LocationTag ,10},
  {"END"            ,    0, true, false},
  {NULL}
	};

static SHeader stdDataMess[] =
	{
  {""               ,   20, true, false, CMsgOG::Image       , 0},
  {"Date && Time "  ,  150, true, false, CMsgOG::DateTime    , 1},
  {"Description "   ,  320, true, false, CMsgOG::Description , 2},
  {"END"            ,    0, true, false},
  {NULL}
	};

static SHeader stdDataEvent[] =
	{
  {""               ,   20, true, false, CMsgSA::Image       , 0},
  {"Date && Time "  ,  150, true, false, CMsgSA::DateTime    , 1},
  {"Description "   ,  320, true, false, CMsgSA::Description , 2},
  {"END"            ,    0, true, false},
  {NULL}
	};

CHTML_Alarm::~CHTML_Alarm()
  {
  if ( wnd ) delete wnd;
  }
//
bool CHTML_Alarm::WindowPos( WND_POS )
  {
  bool bInit = ( !::IsWindow( m_hWnd ) );
  if ( wnd == NULL )
    {
    if ( m_txt == -1 ) m_txt = 0xffffff;
    if ( m_bkg == -1 ) m_bkg = 0x000000;

    if ( lstrcmpi( html.m_strFile, "sysAlarmSummary" ) == 0 )
      {
      wnd = new CViewAlarm();
	  if (html.m_pAreaPane)
		  ((CTagAreaPane*)html.m_pAreaPane)->SetViewAlarm((CViewAlarm*)wnd);
      if(Version == BERTH_ONPZ)
          wnd->SetHeader( stdDataRus, m_txt, m_bkg );
      else wnd->SetHeader( stdData, m_txt, m_bkg );
	  if (Version == LG35_8_KF || Version == DOTF_PM || Version == L35_11600_KF
		  || Version >= AM2_NAK)
	  {
		  int x = rc.X;
		  rc.X = 0;
		  rc.Width+=x;
		  rc.Height+=3;		  
	  }
      }
    else
    if ( lstrcmpi( html.m_strFile, "sysSystemSummary" ) == 0 )
      {
      wnd = new CViewSysStatus();
      wnd->SetHeader( stdData, m_txt, m_bkg );
      }
    else
    if ( lstrcmpi( html.m_strFile, "sysEventSummary" ) == 0 )
      {
      wnd = new CViewEvent();
      wnd->SetHeader( stdDataEvent, m_txt, m_bkg );
      }
    else
    if ( lstrcmpi( html.m_strFile, "sysMessageSummary" ) == 0 )
      {
      wnd = new CViewMessages();
      wnd->SetHeader( stdDataMess, m_txt, m_bkg );
      }
    else
      {
      ASSERT(0);
      }
    //
    if ( wnd != NULL )
      {
      if ( wnd->MyCreate( CWnd::FromHandle(hParent), 0 ) )
      m_hWnd = wnd->m_hWnd;
      }
    }
  else
	  if ( lstrcmpi( html.m_strFile, "sysAlarmSummary" ) == 0 && Version == DOTF_PM || Version == L35_11600_KF || Version >= AM2_NAK)
	  {//съезжает вправо
		  int x = rc.X;
		  rc.X = 0;
		  rc.Width+=x;
		  rc.Height+=3;		  
	  }
  //
  if ( wnd != NULL )
    {
    ::SetWindowPos( m_hWnd, NULL, rc.X, rc.Y, rc.Width, rc.Height, SWP_NOZORDER );
    //
    LOGFONT lf = theApp.GiveLogF(hFont);
    lf.lfHeight = (int)(lf.lfHeight*fScale);
    wnd->SetMyFont( lf );
    wnd->SetScaleFromSmall(fScale);
    }
  //
  if ( bInit )
    {
    }
  return true;
  }
//
void CHTML_Alarm::Timer()
  {
  if ( !::IsWindow( m_hWnd ) ) return;
  if ( !::IsWindowVisible( m_hWnd ) ) return;
  wnd->MainTimer();
  }
//
//***************************************************************************************
/*virtual*/ int CHTML_Alarm::HWBTN( WORD nCode, UINT uKey )
  {
	if ( !::IsWindow( m_hWnd ) ) return 0;
	if ( !::IsWindowVisible( m_hWnd ) ) return 0;
	if ( wnd == NULL ) return 0;
  //
  switch ( uKey )
    {
    case hw_ACK:
      wnd->OnAckGroup(true);
    return 1;
    case hw_ACK_ONE:
      wnd->OnAckOne();
    return 1;
    };
  //
  return 0;
  };
