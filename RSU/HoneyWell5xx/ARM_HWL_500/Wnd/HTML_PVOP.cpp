#include "StdAfx.h"
//
#include "HTMLWndBase.h"
#include "HTML.h"
#include "../BlackConfirmDlg.h"
#include "SafetyManagerStruct.h"
#include "SmShData.h"
//
bool CHTML_PVOP::WindowPos( WND_POS )
  {
  bool bAddString = ( !::IsWindow( m_hWnd ) );
  if ( !CHTMLComboBox::WindowPos( hParent, nNumber, hFont, fScale, rc ) )
    return false;
  //
  if ( bAddString )
  if ( !AS( mVal.def != NULL ) )
    {
    switch ( mVal.def->dwLog )
      {
      case id_FLAG:
        {
        CShFLAG sh;
        if ( html.pipe.GetData( mVal.nNumber, &sh ) )
        for ( int n = 0; n < 2; n++ )
          {
          LRESULT N = ::SendMessage( m_hWnd, CB_ADDSTRING  , 0, (LPARAM)sh.STATETEXT[n] );
                      ::SendMessage( m_hWnd, CB_SETITEMDATA, N, (LPARAM)n    );
          }
        }
      break;
      case id_DEVCTL:
        {
        CShDEVCTL sh;
        if ( html.pipe.GetData( mVal.nNumber, &sh ) )
        for ( int n = 4; n < 7; n++ )
          {
          if ( n == 6 )
            {
            char* ptr = sh.STATETEXT[n];
            while ( *ptr == ' ' ) ptr++;
            if ( !*ptr ) break;
            }
		  if (!_stricmp(sh.STATETEXT[n], "Not Used") /*&& Version == LG35_8_KF*/) 
              continue;
          LRESULT N = ::SendMessage( m_hWnd, CB_ADDSTRING  , 0, (LPARAM)sh.STATETEXT[n] );
                      ::SendMessage( m_hWnd, CB_SETITEMDATA, N, (LPARAM)n    );
          }
        }
      break;
	  //MIHAIL_HWL нет такой структуры
      //case id_ST:
      //  {
      //  CShST sh;
      //  if ( html.pipe.GetData( mVal.nNumber, &sh ) )
      //    {
      //    int nCount = sh.NumberOfOutputStates;
      //    nCount = max( nCount, 0 );
      //    nCount = min( nCount, 7 );
      //    for ( int n = 0; n < nCount; n++ )
      //      {
      //      char* str = NULL;
      //      switch ( n )
      //        {
      //        case 0: str = sh.UNITS ;break;
      //        case 1: str = sh.UNITS1;break;
      //        case 2: str = sh.UNITS2;break;
      //        case 3: str = sh.UNITS3;break;
      //        case 4: str = sh.UNITS4;break;
      //        case 5: str = sh.UNITS5;break;
      //        case 6: str = sh.UNITS6;break;
      //        case 7: str = sh.UNITS7;break;
      //        };
      //      LRESULT N = ::SendMessage( m_hWnd, CB_ADDSTRING  , 0, (LPARAM)str );
      //                  ::SendMessage( m_hWnd, CB_SETITEMDATA, N, (LPARAM)n   );
      //      }
      //    }
      //  }
      //break;
      default:
        ASSERT(0);
      }
    Timer();
    
    if(LG35_8_KF == Version)
    {//для фейсплейта сенсорного экрана
     HWND hParWnd = m_hWnd;
     extern HWND hSensorFaceplateWnd;
     while(hParWnd = GetParent(hParWnd))
       {
       if(hParWnd == hSensorFaceplateWnd)
         {//нашли
          ::PostMessage(GetParent(hParWnd), WM_USER, (WPARAM)&mVal, 1<<16); //команда загрузки фейсплейта
          break;
         }
       }
     }

    }
  return true;
  }
//
void CHTML_PVOP::Timer()
  {
  if ( !::IsWindow( m_hWnd ) ) return;
  if (mVal.def->dwLog == id_FLAG && mVal.def->eVal == enumValueStr)
  {
//---
  char * val = html.pipe.ValueS_H( mVal );
  if ( !val ) return;
  //if (m_sOldSelect && !strcmp(m_sOldSelect, val)) return;
  //m_sOldSelect = val;
  int nCount = (int)::SendMessage( m_hWnd, CB_GETCOUNT, 0, 0 );
  for ( int n = 0; n < nCount; n++ )
    {
	char item[64];
    int len = ::SendMessage( m_hWnd, CB_GETLBTEXT, (WPARAM)n, (LPARAM)item );
    if ( strcmp(item, val) ) continue;
    ::SendMessage( m_hWnd, CB_SETCURSEL, n, 0 );
    return;
    }
  ::SendMessage( m_hWnd, CB_SETCURSEL, -1, 0 );
//---
  }
  else if (mVal.def->eVal == enumValueChr)
  {
  BYTE val = html.pipe.ValueB_H( mVal );
  if ( val == 0xff ) return;
  //
  if ( BYTE(m_nOldSelect) == val ) return;
  m_nOldSelect = int(val);
  //
  int nCount = (int)::SendMessage( m_hWnd, CB_GETCOUNT, 0, 0 );
  for ( int n = 0; n < nCount; n++ )
    {
    BYTE nS = (BYTE)::SendMessage( m_hWnd, CB_GETITEMDATA, n, 0 );
    if ( val != nS ) continue;
    ::SendMessage( m_hWnd, CB_SETCURSEL, n, 0 );
    return;
    }
  ::SendMessage( m_hWnd, CB_SETCURSEL, -1, 0 );
  }
  }
//***************************************************************************************
LRESULT CHTML_PVOP::OnMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
  {
  if ( Version == LG35_8_KF && uMsg == WM_MOUSEWHEEL && this->html.m_strFile == "sysdtlDEVCTLA_fp") return 1;
  if ( uMsg == WM_COMMAND )
  if ( HIWORD(wParam) == CBN_SELCHANGE )
    {
    LRESULT nSel = ::SendMessage( m_hWnd, CB_GETCURSEL, 0, 0 );
    if ( nSel != CB_ERR )
      {
	if (mVal.def->eVal == enumValueStr)
	{
	  char item[64];
	  int len = ::SendMessage( m_hWnd, CB_GETLBTEXT, (WPARAM)nSel, (LPARAM)item );
      char * nOld = html.pipe.ValueS_H( mVal );
      if (nOld && item && strcmp(item, nOld) )
        {
        SSendToModel send;
        lstrcpy( send.szValue, "PVFL" );
        if (Version == LG35_8_KF)
          {
           const char* name = html.pipe.GetModelName(mVal.nNumber);
           if(name && (strcmp(name, "PK2E1.START")==0) || (strcmp(name, "PK1E1.START")==0))
             {
                send.eType = enumValueChr;
                if(strcmp( nOld, "ВКЛ")==0)
                {
                    send.nOld = 1;
                    send.nNew = 0;
                }
                else if(strcmp( nOld, "ВЫКЛ")==0)
                {
                    send.nOld = 0;
                    send.nNew = 1;
                }
             }
          }
        else
        send.Set( enumValueStr, nOld, item );
        html.pipe.SendData( mVal.nNumber, send, true );
        }
	}
	else if (mVal.def->eVal == enumValueChr)
	{
      BYTE nNew = (BYTE)::SendMessage( m_hWnd, CB_GETITEMDATA, nSel, 0 );
      BYTE nOld = html.pipe.ValueB_H( mVal );
      bool bInterlockError = false;
	  if (/*Version == LG35_8_KF &&*/ html.m_strFile.CompareNoCase("sysdtlDEVCTLA_fp")==0){
		  char oi0 = html.pipe.ValueB_H(FindExperion(html, html.m_strGroup, "DEVCTLA.OI[0]"));
		  char oi1 = html.pipe.ValueB_H(FindExperion(html, html.m_strGroup, "DEVCTLA.OI[1]"));
		  char oi2 = html.pipe.ValueB_H(FindExperion(html, html.m_strGroup, "DEVCTLA.OI[2]"));
		  char pi0 = html.pipe.ValueB_H(FindExperion(html, html.m_strGroup, "DEVCTLA.PI[0]"));
		  char pi1 = html.pipe.ValueB_H(FindExperion(html, html.m_strGroup, "DEVCTLA.PI[1]"));
		  char pi2 = html.pipe.ValueB_H(FindExperion(html, html.m_strGroup, "DEVCTLA.PI[2]"));
		  if (oi0 == 1 || oi1 == 1 || oi2 == 1) 
             {
              nNew = nOld;
              bInterlockError = true;
             }
		  //if (pi0 == 0 || pi1 == 0 || pi2 == 0) return;
		  //MIHAIL отключили блокировку по PI
		  //if (pi0 == 0 && nNew == 4) nNew = nOld;
		  //else if (pi1 == 0 && nNew == 5) nNew = nOld;
		  //else if (pi2 == 0 && nNew == 6) nNew = nOld;
	  }

      if (Version != LG35_8_KF)
      {
      if( bInterlockError )
        {
          ::MessageBox(m_hWnd, "Interlock Error", "", MB_OK);
          return CHTMLComboBox::OnMessage( uMsg, wParam, lParam );
        }
      SParamValueH val = FindExperion(html, html.m_strGroup, "DEVCTLA.MODEATTR");
      if(val.def)
        { 
         if(val.def->eVal == enumValueChr)
           {
           char MDAttr = html.pipe.ValueB_H(val);
           if( MDAttr == 2) //PROGRAMM ?
             {
               ::MessageBox(m_hWnd, "Invalid Mode Attribute", "", MB_OK);
               return CHTMLComboBox::OnMessage( uMsg, wParam, lParam );
             }
           }
        }
      }

      // Чавой-то не срослось
      if ( nOld == 0xff ){}
      else
      // Чтобы не занимать сеть
      if ( nOld == nNew ){
		  m_nOldSelect = -1;
		  Timer();
	  }
      else
        {
        SSendToModel send;
        lstrcpy( send.szValue, mVal.def->name );
        send.Set( enumValueChr, &nOld, &nNew );
        html.pipe.SendDataEx( mVal.nNumber, send, true );
        }
      }
	}
    }
  if((WM_LBUTTONDOWN == uMsg) && (theApp.IsAllInOne()))
    {//чтобы не выпадало меню combobox-а для фейсплейта сенсорного экрана
     HWND hParWnd = m_hWnd;
     bool bSkipMesage = false; 
     extern HWND hSensorFaceplateWnd;
     while(hParWnd = GetParent(hParWnd))
       {
       if(hParWnd == hSensorFaceplateWnd)
         {
          bSkipMesage = true;
          ::PostMessage(GetParent(hParWnd), WM_USER, (WPARAM)&mVal, 1<<16 | 1); //команда открыть комбобокс
          break;
         }
       }
     if(bSkipMesage)
       return 1;
     }
  return CHTMLComboBox::OnMessage( uMsg, wParam, lParam );
  }

int CHTML_PVOP::HWBTN( WORD nCode, UINT uKey )
  {
  /*
  BYTE New = 0;
  switch ( uKey )
    {
    case hw_MAN : New =    0; break;
    case hw_AUTO: New =    1; break;
    case hw_NORM: New = 0xff; break;
    default: return 0;
    }
  mVar.SendValue( html.pipe, New );
  ::SetFocus( m_hWnd );
  return 1;
  */
  return 0;
  }

//
bool CHTML_PVOP_M::WindowPos( WND_POS )
{
  bool bAddString = ( !::IsWindow( m_hWnd ) );
  if(bAddString)
  if ( !Create( hParent, "combobox", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL ) )
    return false;
  ::SetWindowPos( m_hWnd, NULL, rc.X, rc.Y, rc.Width, rc.Height*10, SWP_NOZORDER );
  mScale = fScale;
  SetCtrlFont( hFont, fScale );
  //
  if ( bAddString )
  { 
      int ii=0;
      for(std::vector<std::string>::iterator it=m_strings.begin(); it!=m_strings.end(); it++, ii++)
      {   
      UINT nType = 0;
      std::string name, value;
      size_t nn = (*it).find('.');
      if(nn != std::string::npos)
      {
          name = (*it).substr(0,nn);
          value = (*it).substr(nn+1);
      }
      else name = (*it);
      SParamValueH Val = FindExperion( html, name.c_str(), value.c_str(), &nType );
      m_Vals.push_back(Val);
      if(Val.def!=NULL && Val.def->dwLog==id_SMDICOM) 
      {  
         CShSMDICOM sh;
         if( html.pipe.GetData( Val.nNumber, &sh ) )
         {
          LRESULT N = ::SendMessage( m_hWnd, CB_ADDSTRING, ii, (LPARAM)&sh.STATE1TEXT);
          AAA();
         }
      }
      else
      if(Val.def!=NULL && Val.def->dwLog==id_SMDI) 
      {  
         CShSMDI sh;
         if( html.pipe.GetData( Val.nNumber, &sh ) )
         {
          LRESULT N = ::SendMessage( m_hWnd, CB_ADDSTRING, ii, (LPARAM)&sh.STATE1TEXT);
          AAA();
         }
      }
      {
      }
      html.m_bNeedPipeEnd = true;
      }     
  }
  return true;
}
//
void CHTML_PVOP_M::Timer()
{
  if ( !::IsWindow( m_hWnd ) ) 
      return;
  if ( GetFocus() == m_hWnd )  
      return;

  ::SendMessage( m_hWnd, CB_SELECTSTRING, -1, (LPARAM)m_pszScriptValue); 
  //::SendMessage( m_hWnd, CB_SETCURSEL, -1, 0 );
}
//***************************************************************************************
LRESULT CHTML_PVOP_M::OnMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
  if ( uMsg == WM_COMMAND )
  if ( HIWORD(wParam) == CBN_SELCHANGE )
  {
    LRESULT nSel = ::SendMessage( m_hWnd, CB_GETCURSEL, 0, 0 );
    if ( nSel != CB_ERR )
    {
     if((int)nSel < (int)m_Vals.size() && m_Vals[nSel].def)
	 {
      bool bNew = true;
      bool bOld = html.pipe.ValueBOOL_H( m_Vals[nSel] );  
      if ( bOld == bNew )
       {
       }
     else
       {
         SSendToModel send;
         lstrcpy( send.szValue, m_Vals[nSel].def->name );
         send.Set( enumValueBol, &bOld, &bNew );
         html.pipe.SendDataEx( m_Vals[nSel].nNumber, send, true );
         //send.Set( enumValueBol, &bNew, &bOld );
         //html.pipe.SendDataEx( m_Vals[nSel].nNumber, send, true );
         }
       }
    }
  }
  return CHTMLWndBase::OnMessage( uMsg, wParam, lParam );
}

int CHTML_PVOP_M::HWBTN( WORD nCode, UINT uKey )
{
  return 0;
}

