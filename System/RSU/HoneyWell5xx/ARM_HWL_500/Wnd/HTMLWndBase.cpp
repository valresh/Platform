#include "StdAfx.h"
#include "../ARM.h"
#include "../AplicFrm.h"
#include <map>
//
#include "HTMLWndBase.h"
//
CHTMLWndBase::CHTMLWndBase(CHTML& _html)
: m_hWnd(NULL)
, html(_html)
, m_oldProc(NULL)
, m_nFontHeight(0)
  {
  }
//
#ifndef _WIN64
#define XY_PTR  LONG)(LONG_PTR
#else
#define XY_PTR  LONG_PTR
#endif
#define YX_PTR  WNDPROC)(LONG_PTR
//
CHTMLWndBase::~CHTMLWndBase(void)
  {
  if ( ::IsWindow( m_hWnd ) )
    {
    if ( m_oldProc )
      ::SetWindowLongPtr( m_hWnd, GWLP_WNDPROC, (XY_PTR)m_oldProc );
    HFONT hFont = (HFONT)::SendMessage( m_hWnd, WM_GETFONT, 0, 0 );
    if ( hFont != NULL )
      {
      ::SendMessage( m_hWnd, WM_SETFONT, 0, 0 );
      ::DeleteObject( hFont );
      }
    ::DestroyWindow( m_hWnd );
    }
  }
//
LRESULT CHTMLWndBase::OnMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
  {
//return ::DefWindowProc( m_hWnd, uMsg, wParam, lParam );
  return ::CallWindowProc( m_oldProc, m_hWnd, uMsg, wParam, lParam );
  }
//
LRESULT CALLBACK MyWindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
  {
#ifndef _WIN64
  CHTMLWndBase* pWnd = (CHTMLWndBase*)(LONG_PTR)::GetWindowLongPtr(hWnd,GWL_USERDATA);
#else
  CHTMLWndBase* pWnd = (CHTMLWndBase*)(LONG_PTR)::GetWindowLongPtr(hWnd,GWLP_USERDATA);
#endif
  if ( pWnd != NULL )
    return pWnd->OnMessage(uMsg,wParam,lParam);
  return ::DefWindowProc( hWnd, uMsg, wParam, lParam );
  }
//
bool CHTMLWndBase::Create( HWND hParent, const char* pszClass, DWORD dwStyle, DWORD dwExStyle )
  {
  if (  ::IsWindow( m_hWnd ) ) return true;
  m_hWnd = ::CreateWindowEx(dwExStyle,pszClass,"",dwStyle,0,0,0,0,hParent,0,0,0);
  if ( !::IsWindow( m_hWnd ) ) return false;
  //
#ifndef _WIN64
  ::SetWindowLongPtr( m_hWnd, GWL_USERDATA, (XY_PTR)this );
#else
  ::SetWindowLongPtr( m_hWnd, GWLP_USERDATA, (XY_PTR)this );
#endif
  m_oldProc = (YX_PTR)::SetWindowLongPtr( m_hWnd, GWLP_WNDPROC, (XY_PTR)MyWindowProc );
  return true;
  }
//
void CHTMLWndBase::SetCtrlFont( HFONT hFont, FLOAT fScale )
  {
  LOGFONT lf = theApp.GiveLogF(hFont);
  lf.lfHeight = (int)(lf.lfHeight*fScale);
  if ( m_nFontHeight != lf.lfHeight )
    {
    m_nFontHeight  = lf.lfHeight;
    HFONT hNewFont = ::CreateFontIndirect( &lf );//theApp.GiveFont(lf);
    HFONT hOldFont = (HFONT)::SendMessage( m_hWnd, WM_GETFONT, 0, 0 );
    ::SendMessage( m_hWnd, WM_SETFONT, (WPARAM)hNewFont, 1 );
    if ( hOldFont != NULL )
    ::DeleteObject( hOldFont );
    }
  }
//
bool CHTMLWndBase::MessageB( SValueDef* def, char* value )
  {
  /*
  if ( def == NULL ) return true;
  SParamValueH a;
  a.nNumber = mVal->nNumber;
  a.def = def;
  BYTE val = html.pipe.ValueB( a );
  if ( val != 0 )
    {
    CString str;
    str.Format( "Для изменения %s, параметр %s\nдолжен находиться в состоянии '%s'",
                mVal->def->name, def->name, value );
    GetAppWnd()->MessageBox( str, NULL, MB_ICONEXCLAMATION );
    }
  return val == 0;
  */
  return true;
  }
//***************************************************************************************
//
CHTMLComboBox::CHTMLComboBox(CHTML& _html)
: CHTMLWndBase( _html )
, m_nOldSelect(-1)
  {
  }
//
CHTMLComboBox::~CHTMLComboBox(void)
  {
  }
//
bool CHTMLComboBox::WindowPos( WND_POS )
  {
  if ( !Create( hParent, "combobox", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL ) )
    return false;
  ::SetWindowPos( m_hWnd, NULL, rc.X, rc.Y, rc.Width, rc.Height*10, SWP_NOZORDER );
  this->mScale = fScale;
  SetCtrlFont( hFont, fScale );
  return true;
  }
//***************************************************************************************
//
CHTMLEditBox::CHTMLEditBox( CHTML& _html, CSpPvOp* _SPO )
: CHTMLWndBase( _html )
, mSPO(_SPO)
, m_bEdited(false)
  {
  }
//
CHTMLEditBox::~CHTMLEditBox(void)
  {
  if ( mSPO )
    delete mSPO;
  
  for( std::map<std::string, CSpPvOp*>::iterator it = m_SPOs.begin(); it!=m_SPOs.end(); it++)
     if(it->second)
       {
       delete it->second;
       it->second = NULL;
       }
  }
//
bool CHTMLEditBox::WindowPos( WND_POS )
  {
  bool bInit = ( !::IsWindow( m_hWnd ) );
  if ( !Create( hParent, "edit", WS_CHILD | WS_BORDER | ES_RIGHT | ES_AUTOHSCROLL, 0 ) )//WS_EX_CLIENTEDGE ) )
    return false;
  ::SetWindowPos( m_hWnd, NULL, rc.X, rc.Y, rc.Width, rc.Height+1, SWP_NOZORDER );
  SetCtrlFont( hFont, fScale );
  if ( bInit && mSPO )
    {
    mSPO->Init( html.pipe, nNumber );

    if(theApp.IsAllInOne())
    {//для фейсплейта сенсорного экрана
     HWND hParWnd = m_hWnd;
     extern HWND hSensorFaceplateWnd;
     while(hParWnd = GetParent(hParWnd))
       {
       if(hParWnd == hSensorFaceplateWnd)
         {//нашли
          if(typeid(*mSPO) == typeid(CSendSP))
            ::PostMessage(GetParent(hParWnd), WM_USER, (WPARAM)&mSPO->mCurr, 2<<16); //команда загрузки фейсплейта
          else if(typeid(*mSPO) == typeid(CSendOP))
                 ::PostMessage(GetParent(hParWnd), WM_USER, (WPARAM)&mSPO->mCurr, 3<<16); //команда загрузки фейсплейта
          break;
         }
       }
     }
    }
  return true;
  }
//
bool CHTMLEditBox::WindowPos( HWND hParent, SParamValueH &mVal, HFONT hFont, FLOAT fScale, Rect& rc )
  {
  m_Val =  mVal;
  return WindowPos( hParent, mVal.nNumber, hFont, fScale, rc );
  }
//
void CHTMLEditBox::Release()
  {
  if ( !::IsWindow( m_hWnd ) ) return;
  ::SendMessage( m_hWnd, WM_KILLFOCUS, 0, 0 );
  }
//
void CHTMLEditBox::Pressed()
  {
  if ( !::IsWindow( m_hWnd ) ) return;
  //
  ::ShowWindow( m_hWnd, SW_SHOW );
  ::SendMessage( m_hWnd, EM_SETSEL, 0, -1 );
  ::SetFocus( m_hWnd );
  SetInfoBar();
  }
//
LRESULT CHTMLEditBox::OnMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
  {
  if ( uMsg == WM_KILLFOCUS )
    {
    m_bEdited = false;
    ::ShowWindow( m_hWnd, SW_HIDE );
    Timer();
	GetAppWnd()->HideInfoBar(true);
    }
  else
  if (( uMsg == WM_KEYUP) && (wParam == VK_ESCAPE ))
  {
    ::ShowWindow( m_hWnd, SW_HIDE );
	GetAppWnd()->HideInfoBar();
  }
  else
  if ( uMsg == WM_KEYDOWN && wParam == VK_RETURN )
    {
    if ( mSPO )
      {
      char szText[512];
      ::GetWindowText( m_hWnd, szText, 512 );
      char* p = strchr( szText, ',' );
      if ( p ) *p = '.';
      double dValue = atos(szText);
      mSPO->SendValue( html.pipe, dValue, true, true );
      }
    ::ShowWindow( m_hWnd, SW_HIDE );
	GetAppWnd()->HideInfoBar();
    m_bEdited = false;
    Timer();
    }
  else
  if ( uMsg == WM_KEYDOWN && wParam == VK_UP )
    return HWBTN( HW_KEYDOWN, hw_STEP_UP );
  else
  if ( uMsg == WM_KEYDOWN && wParam == VK_DOWN )
    return HWBTN( HW_KEYDOWN, hw_STEP_DOWN );
  else
  if ( uMsg == WM_KEYUP && wParam == VK_UP )
    return HWBTN( HW_KEYUP, hw_STEP_UP );
  else
  if ( uMsg == WM_KEYUP && wParam == VK_DOWN )
    return HWBTN( HW_KEYUP, hw_STEP_DOWN );
  else
  if ( uMsg == WM_SETFOCUS)
  {
   m_bEdited = true;
   if(theApp.IsAllInOne())
    {//для фейсплейта сенсорного экрана
     HWND hParWnd = m_hWnd;
     extern HWND hSensorFaceplateWnd;
     while(hParWnd = GetParent(hParWnd))
       {
       if(hParWnd == hSensorFaceplateWnd)
         {//нашли
          if(typeid(*mSPO) == typeid(CSendSP))
            ::PostMessage(GetParent(hParWnd), WM_USER, (WPARAM)&mSPO->mTole, 4<<16); //команда начала редактирования
          else if(typeid(*mSPO) == typeid(CSendOP))
                 ::PostMessage(GetParent(hParWnd), WM_USER, (WPARAM)&mSPO->mHiLm, 5<<16); //команда начала редактирования
          ::ShowWindow( m_hWnd, SW_HIDE );
          Timer();
          return 1;
         }
       }
     }
  }
  //
  return CHTMLWndBase::OnMessage( uMsg, wParam, lParam );
  }
//
int CHTMLEditBox::HWBTN( WORD nCode, UINT uKey )
  {
  if ( ::IsWindowVisible(m_hWnd) && mSPO )
    {
    switch ( uKey )
      {
      case hw_STEP_UP  : mSPO->SendValue( html.pipe, m_hWnd, nCode == HW_KEYUP ? 0 : 1 );Timer();return 1;
      case hw_STEP_DOWN: mSPO->SendValue( html.pipe, m_hWnd, nCode == HW_KEYUP ? 0 :-1 );Timer();return 1;
      case hw_MOVE_UP  : mSPO->SendValue( html.pipe, m_hWnd, nCode == HW_KEYUP ? 0 : 2 );Timer();return 1;
      case hw_MOVE_DOWN: mSPO->SendValue( html.pipe, m_hWnd, nCode == HW_KEYUP ? 0 :-2 );Timer();return 1;
      default: return 0;
      }
    }
  return 0;
  }
//
void CHTMLEditBox::Timer()
  {
  if ( !::IsWindow( m_hWnd ) ) return;
  if ( ::IsWindowVisible( m_hWnd ) ) return;
  //
  if ( mSPO )
    {
    char txt[512]="";
    mSPO->ValueSt( html.pipe, txt );
    ::SetWindowText( m_hWnd, txt );
    }
  }

bool CHTMLEditBox::IsChangeParamValue( SParamValueH& var )
{  //проверка поменялись ли перемннные - если да то подмена на их параметры nNumber и def
   if(var.pMulti)
   {
       SParamValueHMulti *pMulti = var.pMulti;   
       std::map<std::string, std::string>::iterator it;
       if( (it = pMulti->dataRepository.find(pMulti->key)) != pMulti->dataRepository.end())
         {
             if( _stricmp(pMulti->key, it->second.c_str()) != 0)
               {
               std::map<std::string, std::string>::iterator it2;
               if ( (it2 = pMulti->dataRepository.find(it->second)) != pMulti->dataRepository.end() )
                 {
                   it = it2;
                 }
               }
             if( _stricmp(pMulti->curkeyval, it->second.c_str()) != 0)
             { //поменялся ключевой параметр
                 strcpy_s(pMulti->curkeyval, it->second.c_str());
                 std::map<std::string, std::pair<int, SValueDef*>>::iterator it = pMulti->mDefs.find(pMulti->curkeyval);
                 if(it != pMulti->mDefs.end())
                   {
                    var.nNumber = it->second.first;
                    var.def = it->second.second;
                    return true;
                   }
             }
         }
   }
   return false;
}
//***************************************************************************************
