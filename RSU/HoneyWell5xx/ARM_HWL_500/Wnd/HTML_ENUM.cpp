#include "StdAfx.h"
#include <winuser.h>
//
#include "HTMLWndBase.h"
#include "HTML.h"
#include <algorithm>
#include "..\\AplicFrm.h"
#include "Style.h"
#include "SafetyManagerStruct.h"
#include "SmShData.h"
//
CHTML_ENUM::CHTML_ENUM(CHTML& _html, SParamValueH& val, CTag *pTag)//SBaseDSD* dsd)
    : CHTMLComboBox( _html )
    , mVal(val)
    , m_HiddenElements(0)
    , m_nAcronymNumber(-1)
    , m_nAcronymRecord(-1)
    , m_bButDown(false)
    , m_bColorCombobox(false)
    , m_clrFill(RGB(255,255,255))
    , m_clrText(RGB(0,0,0))
    , m_bNeedRedraw(false)
{
    if(Version==DOTF_PM)
      m_bColorCombobox = true;

    SBaseDSD* dsd = NULL;
    if(pTag)
    {
      if(m_bColorCombobox)
        {
        CStyle& style = pTag->Style(_html);
        m_clrFill =  style.m_clrFill;
        m_clrText =  style.m_clrText;
        }
       dsd = _html.Dsd(_html);
    }
    else m_bColorCombobox = false;

    if(dsd)
    {
    const char* szName = dsd->GetStr( html, "AcronymSource" );
    if(szName && strcmp(szName, "0"))
      {//строки из системного файла acrnym.src
      szName = dsd->GetStr( html, "AcronymNumber" );
      if(szName)
        {
         int nn = atoi(szName);
         szName = dsd->GetStr( html, "AcronymRecord" );
         if(szName)
          {
              m_nAcronymNumber = nn;
              m_nAcronymRecord = atoi(szName);
          }
        }
      }
    }
}
//
bool CHTML_ENUM::WindowPos( WND_POS )
  { 
  bool bAddString = ( !::IsWindow( m_hWnd ) );
  if ( !CHTMLComboBox::WindowPos( hParent, nNumber, hFont, fScale, rc ) )
    return false;
  //
  if ( bAddString )
    {
       //if(m_bColorCombobox && m_hWnd)
       // {
       // HBRUSH hbrush = CreateSolidBrush(m_clrFill);
       // SetClassLongPtr(m_hWnd, GCLP_HBRBACKGROUND, (LONG_PTR)hbrush);
       // }

			BYTE n = 0;
#ifdef _DEBUG
            const char* name = html.pipe.GetModelName(mVal.nNumber);
#endif
			while ( 1 )
			{      
                    std::string strText;
                    if(mVal.def!=NULL && mVal.def->eVal==enumValueStr) 
                    {  
                        if(mVal.def->dwLog == id_FLAG)
                        {
                        CShFLAG sh;
						if ( html.pipe.GetData( mVal.nNumber, &sh ) )
						   strText=sh.STATETEXT[n];
                        if ( strText=="" )
						   break;
                        }
                        else
                        if(mVal.def->dwLog == id_SWITCH)
                        {
                        CShSWITCH sh;
						if ( html.pipe.GetData( mVal.nNumber, &sh ) )
						   strText=sh.XDESC[n];
                        if ( strText=="" )
                          {
                           if(::SendMessage( m_hWnd, CB_GETCOUNT, 0, 0 )==0)
                             {
                             ++n;
						     continue;
                             }
                           else break;
                          }
                        }
                        else
                        if(mVal.def->dwLog == id_DIGACQ)
                        {
                        CShDIGACQ sh;
						if ( html.pipe.GetData( mVal.nNumber, &sh ) )
						   strText=sh.STATETEXT[n];
                        if ( strText=="" )
						   break;
                        }
                        else break;
                    }
                    else
                   if(mVal.def!=NULL && mVal.def->eVal==enumValueDbl) 
                    {  
                      static const char* mode[] = {"Режим0", "Режим1"}; //нужно чем-то заменить
                      if(n<_countof(mode))
                        strText = mode[n]; 
                      else break;
                    }
                   else
                   if(mVal.def!=NULL && mVal.def->eVal==enumValueChr && mVal.def->dwLog==id_SMDICOM) 
                    {  
                      CShSMDICOM sh;
                      if( html.pipe.GetData( mVal.nNumber, &sh ) )
                      {
                         if(n==0)
                         {
						   strText=sh.STATE0TEXT;
                         }
                         else 
                         if(n==1)
                           strText=sh.STATE1TEXT;
                         else break;
                      }
                    }
                    else
                    {
					strText = EnumStrH( *mVal.def, n );
					if ( strText=="" )
						break;
					if (mVal.def != NULL ) 
					{
					  if ( mVal.def->dwLog == id_DEVCTL)
						if (n>=4&&n<=6)
						{
							CShDEVCTL sh;
							if ( html.pipe.GetData( mVal.nNumber, &sh ) )
									strText=sh.STATETEXT[n];
						}
					}
                    }
					std::transform(strText.begin(), strText.end(),strText.begin(), ::toupper);
                    if(Version==DOTF_PM)
                      {
                        if(strcmp(mVal.def->name, "ALMENBSTATE")==0)
                          {
                          if(n==0)
                             strText = "Disabled";
                          else if(n==1)
                             strText = "Enabled";
                          else break;
                          }
                        else
                        if( (::SendMessage( m_hWnd, CB_GETCOUNT, 0, 0 )==0) &&
                            (strText=="NULL" || strText=="INBET" || strText=="ACTIVE" || strText=="SAFE" || strText=="NONE" ))
                         {
                         ++m_HiddenElements;
                         ++n;
                         continue;
                         }
                      }
                      
					if(Version == L35_11600_KF && 
						(!stricmp(strText.c_str(), "BCAS") 
							|| (!stricmp(strText.c_str(), "NONE") 
								&& mVal.def && !stricmp((*mVal.def).name, "MODE"))))
						{ n++; continue; }
					if(Version == T1165P_ONPZ
						&& html.m_strFile == "sysdtlana_Rev2_fp")
					{
						if(!stricmp(strText.c_str(), "OFFLINE")) strText = "Ручной";
						else if(!stricmp(strText.c_str(), "ONLINE")) strText = "Авто";
						else { n++; continue; }
					}

					LRESULT N = ::SendMessage( m_hWnd, CB_ADDSTRING  , n-m_HiddenElements, (LPARAM)strText.c_str() );
					::SendMessage( m_hWnd, CB_SETITEMDATA, N, (LPARAM)(n-m_HiddenElements)    );
					n++;
			}
			Timer();

    if(theApp.IsAllInOne())
    {//для фейсплейта сенсорного экрана
     HWND hParWnd = m_hWnd;
     extern HWND hSensorFaceplateWnd;
     while(hParWnd = GetParent(hParWnd))
       {
       if(hParWnd == hSensorFaceplateWnd)
         {//нашли
		  bool bNoSendMessage=((Version==L35_11600_KF||Version==LG35_8_KF)&&(mVal.def!=NULL)&&(lstrcmp(mVal.def->name,"MODEATTR")==0));
          if (!bNoSendMessage)
				::PostMessage(GetParent(hParWnd), WM_USER, (WPARAM)&mVal, 0); //команда загрузки фейсплейта
          break;
         }
       }
     }

    }
  return true;
  }
//
void CHTML_ENUM::Timer()
  {
  if ( !::IsWindow( m_hWnd ) ) return;
  //
  if( ::GetFocus() == m_hWnd)
    return;
  //
  if(m_bButDown)
  {
      m_bButDown = false;
      m_bNeedRedraw = true;
  }
  //
  if (mVal.def->eVal == enumValueStr)
    {
    char *val = html.pipe.ValueS_H( mVal );
    if ( val == NULL ) return;
    char szT[256];
    LRESULT nSel = ::SendMessage( m_hWnd, CB_GETCURSEL, 0, 0 );
    ::SendMessage( m_hWnd, CB_GETLBTEXT, nSel, (LPARAM)szT );
    if(_stricmp(szT, val)!=0)
    {
      ::SendMessage( m_hWnd, CB_SELECTSTRING, -1, (LPARAM)val );
      m_bNeedRedraw=true;
    }
    }
  else
  if (mVal.def->eVal == enumValueDbl)
  {
  int val = (int)html.pipe.ValueF_H( mVal );
  //
  if ( BYTE(m_nOldSelect) != val )
  {
  m_nOldSelect = val;
  ::SendMessage( m_hWnd, CB_SETCURSEL, val-m_HiddenElements, 0 );
  m_bNeedRedraw=true;
  }
  }
  else
  {
  BYTE val = html.pipe.ValueB_H( mVal );
  if ( val == 0xff ) return;
  //
  if ( BYTE(m_nOldSelect) != val ) 
  {
  m_nOldSelect = int(val);
  
  if (Version == L35_11600_KF && !strcmp(mVal.def->name, "MODE") && val > 3)
		val = 3;

  ::SendMessage( m_hWnd, CB_SETCURSEL, val-m_HiddenElements, 0 );
  m_bNeedRedraw=true;
  }
  }

  if(m_bNeedRedraw)
    {
    ReDraw();
    m_bNeedRedraw = false;
    }
  }
//***************************************************************************************
LRESULT CHTML_ENUM::OnMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
  {
  if ( uMsg == WM_COMMAND )
  {
  if ( HIWORD(wParam) == CBN_SELCHANGE )
    {
    LRESULT nSel = ::SendMessage( m_hWnd, CB_GETCURSEL, 0, 0 );
	LPCTSTR name = html.pipe.GetMnemoName(mVal.nNumber);
	SetInfoBar(name);
    if ( nSel != CB_ERR )
      {
      if (mVal.def->eVal == enumValueStr)
      {
       char szNew[256];
       ::SendMessage( m_hWnd, CB_GETLBTEXT, nSel, (LPARAM)szNew );
       char *pOld = html.pipe.ValueS_H( mVal );
       if (strcmp(pOld, szNew ) !=0)
         {
         SSendToModel send;
         lstrcpy( send.szValue, mVal.def->name );
         send.Set( enumValueStr, pOld, szNew );
         html.pipe.SendData( mVal.nNumber, send, true );
         if(Version == DOTF_PM)
           {
            if(strcmp(name, "IS2123_FL.SWITCHA")==0 && strcmp(mVal.def->name, "SELXDESC")==0)
            {
            SParamValueH val = ::FindExperion( html.pipe, name, "SELXINP" );
            if(val.nNumber>=0)
              {
                int iOld = html.pipe.ValueI_H(val);
                SSendToModel send2;
                strcpy_s( send2.szValue, val.def->name );
                int iNew = int(nSel+1);
                send2.Set( enumValueInt, &iOld,  &iNew);
                html.pipe.SendData( val.nNumber, send2, true );
              }
            }              
           }
         }
      }
      else
      if (mVal.def->eVal == enumValueDbl)
      {
       double dNew = (BYTE)nSel + m_HiddenElements;
       double dOld = html.pipe.ValueF_H( mVal );
       if ( dOld != dNew )
       {
        SSendToModel send;
        lstrcpy( send.szValue, mVal.def->name );
        send.Set( enumValueDbl, &dOld, &dNew );
        html.pipe.SendData( mVal.nNumber, send, true );
       }
      }
      else
      {
      BYTE nNew = (BYTE)nSel + m_HiddenElements;
      BYTE nOld = html.pipe.ValueB_H( mVal );
      bool bCanWrite = true; 
      if ((Version != LG35_8_KF) && (strcmp(mVal.def->name, "MODE")==0))
      {
      SParamValueH val = FindExperion(html, html.m_strGroup, "PIDA.MODEATTR");
      if(val.def)
        { 
         if(val.def->eVal == enumValueChr)
           {
           char MDAttr = html.pipe.ValueB_H(val);
           if( MDAttr == 2) //PROGRAMM ?
             {
               ::MessageBox(m_hWnd, "Invalid Mode Attribute", "", MB_OK);
               bCanWrite=false;
             }
           }
        }
      }
      if(bCanWrite)
      {
      if ( nOld == 0xff ){}
      else
      // Чтобы не занимать сеть
      if ( nOld == nNew ){}
      else
        {
        SSendToModel send;
        lstrcpy( send.szValue, mVal.def->name );
        send.Set( enumValueChr, &nOld, &nNew );
        html.pipe.SendData( mVal.nNumber, send, true );
        }
      }
      }
      m_bNeedRedraw=true;
      }
    }
  }
  else
  if(m_bColorCombobox)
   {
  /*if(WM_CTLCOLORLISTBOX == uMsg)
    {  //для окрашивание выпадающего списка
        HDC hdcLB = (HDC) wParam; 
        HWND hwndLB = (HWND) lParam;
        if(hdcLB)
        {
        ::SetTextColor(hdcLB, m_clrText);
        ::SetBkColor(hdcLB, m_clrFill);
        return (LRESULT)::CreateSolidBrush(m_clrFill);
        }
    }
    else*/
    if(WM_PAINT == uMsg)
      {
          LONG_PTR style = GetWindowLongPtr(m_hWnd, GWL_STYLE);
          if(!(style & CBS_DROPDOWNLIST))
              return 0;
  
          RECT rc;
          GetClientRect(m_hWnd, &rc);
          PAINTSTRUCT ps;
          HDC hdc = BeginPaint(m_hWnd, &ps);
          DWORD bkcolor = m_clrFill;
          HBRUSH brush = CreateSolidBrush(bkcolor);
          HPEN pen = CreatePen(PS_SOLID, 1, m_clrFill);
          HBRUSH oldbrush = (HBRUSH)SelectObject(hdc, brush);
          HPEN oldpen = (HPEN)SelectObject(hdc, pen);
          SelectObject(hdc, (HFONT)SendMessage(m_hWnd, WM_GETFONT, 0, 0));
          SetBkColor(hdc, bkcolor);
          SetTextColor(hdc, m_clrText);
  
          ::Rectangle(hdc, 0, 0, rc.right, rc.bottom);
  
          RECT temp = rc;
          temp.left = temp.right -19;
          InflateRect(&temp, -1, -1);
  
          int index = (int)SendMessage(m_hWnd, CB_GETCURSEL, 0, 0);
          if(index >= 0)
          {
              int buflen = (int)SendMessage(m_hWnd, CB_GETLBTEXTLEN, index, 0);
              TCHAR *buf = new TCHAR[(buflen + 1)];
              SendMessage(m_hWnd, CB_GETLBTEXT, index, (LPARAM)buf);
              rc.left += 4;
              rc.right--;
              DrawText(hdc, buf, -1, &rc, DT_EDITCONTROL|DT_LEFT|DT_VCENTER|DT_SINGLELINE);
              delete[]buf;
          }
  
          ++temp.top;
          HBITMAP hBmp= NULL;
          if(m_bButDown)
             hBmp = LoadBitmap(NULL, MAKEINTRESOURCE(32742));
          else hBmp = LoadBitmap(NULL, MAKEINTRESOURCE(32752));
          if(hBmp)
            {
            BITMAPINFO info={0};
            ::GetObject(hBmp, sizeof(BITMAP), &info);
            HDC     hMemDc = CreateCompatibleDC(hdc);
            SelectObject(hMemDc, hBmp);
            StretchBlt(hdc, temp.left, temp.top, temp.right-temp.left, temp.bottom-temp.top, hMemDc,
                            0, 0, info.bmiHeader.biWidth, info.bmiHeader.biHeight,SRCCOPY);
            DeleteDC(hMemDc);
            DeleteObject(hBmp);
            }

          SelectObject(hdc, oldpen);
          SelectObject(hdc, oldbrush);
          DeleteObject(brush);
          DeleteObject(pen);
  
          EndPaint(m_hWnd, &ps);
          return 0;
      }
    else
    if(WM_KILLFOCUS == uMsg) 
    {
      m_bButDown = false;
      ReDraw();
      return 0;
    }
   }

  if(WM_LBUTTONDOWN == uMsg)
  {
    m_bButDown = true;
    if(theApp.IsAllInOne())
    {//чтобы не выпадало меню combobox-а для фейсплейта сенсорного экрана
     HWND hParWnd = m_hWnd;
     bool bSkipMesage = false; 
     extern HWND hSensorFaceplateWnd;
     while(hParWnd = GetParent(hParWnd))
       {
       if(hParWnd == hSensorFaceplateWnd)
         {
          bSkipMesage = true;
          ::PostMessage(GetParent(hParWnd), WM_USER, (WPARAM)&mVal, 1); //команда открыть комбобокс
          break;
         }
       }
     if(bSkipMesage)
       return 1;
     }
  }
  else
  if(WM_LBUTTONUP == uMsg)
    {
     if(m_bButDown)
     {
       m_bButDown = false;
       if(m_bColorCombobox)
       {
       ReDraw();
       return 0;
       }
     }
    }
  return CHTMLComboBox::OnMessage( uMsg, wParam, lParam );
  }

int CHTML_ENUM::HWBTN( WORD nCode, UINT uKey )
  {
  if ( mVal.def == NULL )
    return 0;
  if ( lstrcmpi( mVal.def->name, "MODE" ) )
    return 0;
  BYTE nNew = 0;
  BYTE nOld = html.pipe.ValueB_H( mVal );
  switch ( uKey )
    {
    case hw_MAN : nNew = 0; break;
    case hw_AUTO: nNew = 1; break;
    case hw_NORM: nNew = 5; break;
    default: return 0;
    }
  SSendToModel send;
  lstrcpy( send.szValue, mVal.def->name );
  send.Set( enumValueChr, &nOld, &nNew );
  html.pipe.SendData( mVal.nNumber, send, true );
  if((Version == AM2_NAK)||(Version==DMW_NAK)) ::SetFocus( NULL);
  else ::SetFocus( m_hWnd );
  return 1;
  }

void CHTML_ENUM::SetInfoBar(CString strName)
  {
  CString strNameARM(theApp.GetHoneywellInfoName().c_str());
  CString strInfo;
  strInfo=strNameARM+":"+strName+".MODE = ";
  LRESULT nSel = ::SendMessage( m_hWnd, CB_GETCURSEL, 0, 0 );
  char str[255];
  ::SendMessage( m_hWnd, CB_GETLBTEXT, nSel, (LPARAM)str );
  CString strValue(str);
  strInfo+=strValue;
  //
  SParamValueH val = ::FindExperion( html.pipe, strName, "desc" );
  char*  text = html.pipe.ValueS_H( val );
  if ( text && *text )
    {
    strInfo += " (";
    strInfo += text;
    strInfo += ")";
    }
  GetAppWnd()->ShowInfoBar(barINFO,strInfo);
  }

void CHTML_ENUM::ReDraw()
{
    if(m_bColorCombobox)
     {
      RECT rc;
      ::GetClientRect(m_hWnd, &rc);
      ::InvalidateRect(m_hWnd, &rc, TRUE);
    }
}