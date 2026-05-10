#include "StdAfx.h"
//
#include "HTMLWndBase.h"
#include "HTML.h"
#include <algorithm>
#include "..\\AplicFrm.h"
//

CHTML_ENUM_LIST2::CHTML_ENUM_LIST2(CHTML& _html, std::vector<COMBO_LIST2> *pDataList, bool bFullTagName) : CHTMLComboBox( _html )
{ 
    m_DataList = *pDataList;
    for(int ii=0; ii<m_DataList.size(); ii++)
       {
        SParamValueH val;
        if(!bFullTagName)
          val = FindExperion(html, html.m_strGroup.GetBuffer(), m_DataList[ii].paramRead.c_str());
        else val = FindExperion(html, m_DataList[ii].paramRead.c_str(), "");
        mValReads.push_back(val);
        if(!bFullTagName)
          val = FindExperion(html, html.m_strGroup.GetBuffer(), m_DataList[ii].paramWrite.c_str());
        else val = FindExperion(html, m_DataList[ii].paramWrite.c_str(), "");
        mValWrites.push_back(val);
       }
     _html.m_bNeedPipeEnd=true;
}

bool CHTML_ENUM_LIST2::WindowPos( WND_POS )
  {
  bool bAddString = ( !::IsWindow( m_hWnd ) );
  if ( !CHTMLComboBox::WindowPos( hParent, nNumber, hFont, fScale, rc ) )
    return false;

  if ( bAddString )
    {
        for(int ii=0; ii<m_DataList.size(); ii++)
        {
         LRESULT N = OnMessage( CB_ADDSTRING, ii, (LPARAM)m_DataList[ii].showItem.c_str() );
         OnMessage( CB_SETITEMDATA, N, (LPARAM)ii);
        }
    }
  return true;
}

LRESULT CHTML_ENUM_LIST2::OnMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
  {
  if ( uMsg == WM_COMMAND )
  if ( HIWORD(wParam) == CBN_SELCHANGE )
    {
     LRESULT nSel = CHTMLComboBox::OnMessage( CB_GETCURSEL, 0, 0  );
     if( (m_DataList.size() > (size_t)nSel) && mValWrites[nSel].def)
       {
         SParamValueH& Val = mValWrites[nSel];
	     void *vv = html.pipe.ParamValue_H(Val);                                
         if(vv)
         {
         SSendToModel send;
         lstrcpy( send.szValue, Val.def->name );
         send.Set( Val.def->eVal, &vv, &m_DataList[nSel].val2set.odouble );
         html.pipe.SendDataEx( Val.nNumber, send, true );
         }
       }
     if( (m_DataList.size() > (size_t)nSel) && mValReads[nSel].def)
       {
         SParamValueH& Val = mValReads[nSel];
	     void *vv = html.pipe.ParamValue_H(Val);                                
         if(vv)
         {
         SSendToModel send;
         lstrcpy( send.szValue, Val.def->name );
         send.Set( Val.def->eVal, &vv, &m_DataList[nSel].val2set.odouble );
         html.pipe.SendDataEx( Val.nNumber, send, true );
         }
       }
    }
  return CHTMLComboBox::OnMessage( uMsg, wParam, lParam );
  }

void CHTML_ENUM_LIST2::Timer() 
{
	if (!::IsWindow(m_hWnd)) 
        return;
    if( GetFocus() == m_hWnd)
      return;

    if(mValReads.size())
    {
       SParamValueH& Val = mValReads[0];
	   void *vv = html.pipe.ParamValue_H(Val);    
       int ind = -1;
       if(vv)
       {
            switch(Val.def->eVal)
              {
               case _enumValueDbl:                 
                   ind = (int)*(double*)vv;
                   break;                   
               case _enumValueInt: 
                   ind = *(int*)vv;
                   break;
               case _enumValueBol: 
               case _enumValueChr: 
                   ind = (int)*(BYTE*)vv;
                   break;
               default: AAA();
                   break;
              }
       }

      if(ind != -1)
	    ::SendMessage(m_hWnd, CB_SETCURSEL, ind-1, 0);
    }
}
