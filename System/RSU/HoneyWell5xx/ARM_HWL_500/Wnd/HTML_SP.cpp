#include "StdAfx.h"
//
#include "HTMLWndBase.h"
#include "HTML.h"
#include "..\\AplicFrm.h"
#include <map>
//
int CHTML_SP::HWBTN( WORD nCode, UINT uKey )
  {
  if ( uKey == hw_SP )
    {
    Pressed();
    return 1;
    }
  return CHTMLEditBox::HWBTN( nCode, uKey );
  }

void CHTML_SP::SetInfoBar()
{
	if (GetPointSPO()!=NULL)
	{
		SParamValueH mCurr=GetPointSPO()->GetCurrnetPoint();
		if (mCurr.nNumber>=0)
		{
			LPCTSTR name = html.pipe.GetMnemoName(mCurr.nNumber);
			CString strName(name);
			CString strNameARM(theApp.GetHoneywellInfoName().c_str());
			  CString strInfo;
			  if (mCurr.def!=NULL)
			  {
				  CString strDefName(mCurr.def->name);
				  strInfo=strNameARM+":"+strName+"."+strDefName+" = ";
				  double dValue = html.pipe.ValueF_H( mCurr );
				  char str[512];
				  if ( GetPointSPO()->ValueSt( html.pipe, str ) )
				  {
					  CString strValue(str);
					  strValue.Format("%.8f",dValue);
					  strInfo+=strValue;
					  SParamValueH val = ::FindExperion( html.pipe, strName, "desc" );
					  char*  text = html.pipe.ValueS_H( val );
					  if ( text && *text )
						{
						strInfo += " (";
						strInfo += text;
						strInfo += ")";
						}
					  if(GetAppWnd()!=NULL)
					  {
						GetAppWnd()->ShowInfoBar(barINFO,strInfo);
						GetAppWnd()->SetPipeAndNumberForInfoBar(&html.pipe,mCurr.nNumber);
					   }
				  }
			  }
		}
	}
}

bool CHTML_SP::WindowPos( HWND hParent, SParamValueH &mVal, HFONT hFont, FLOAT fScale, Rect& rc )
{
   bool bInit = ( !::IsWindow( m_hWnd ) );
   bool bRet = __super::WindowPos( hParent, mVal, hFont, fScale, rc );

   if( bInit && mSPO && m_Val.pMulti )
     {
       for(std::map<std::string, std::pair<int, SValueDef*>>::iterator it = m_Val.pMulti->mDefs.begin(); it!=m_Val.pMulti->mDefs.end(); it++)
         {
             CSendSP *pSendSP = new CSendSP(mSPO->m_szFiltr);
             pSendSP->Init(html.pipe, it->second.first);
             if(pSendSP)
               m_SPOs.insert(std::pair<std::string, CSpPvOp*>(it->first, pSendSP));
         }
     }

   return bRet;
}

void CHTML_SP::Timer()
{
   if( mSPO && m_Val.pMulti )
    {
        if(IsChangeParamValue( m_Val ))
           {
           ::SendMessage( m_hWnd, WM_KILLFOCUS, 0, 0 );
           std::map<std::string, CSpPvOp*>::iterator it;
           if( (it = m_SPOs.find(m_Val.pMulti->curkeyval)) != m_SPOs.end())
             {
             mSPO->mCurr = it->second->mCurr;
             mSPO->mHiLm = it->second->mHiLm;
             mSPO->mLoLm = it->second->mLoLm;
             mSPO->mMode = it->second->mMode;
             mSPO->mPVFM = it->second->mPVFM;
             mSPO->mTole = it->second->mTole;
             }
           }
    }
    __super::Timer();

}