#include "StdAfx.h"
//
#include "HTMLWndBase.h"
#include "HTML.h"
#include "..\\AplicFrm.h"
#include <map>
//
int CHTML_OP::HWBTN( WORD nCode,UINT uKey )
  {
  if ( uKey == hw_OUT )
    {
    Pressed();
    return 1;
    }
  else
  if ( uKey ==hw_STEP_UP )
  {
	  if (GetSPO())
	  {
		  GetSPO()->SendValue( html.pipe, m_hWnd, nCode == HW_KEYUP ? 0 : 10 );Timer();return 1;
	  }
  }
  else
  if ( uKey ==hw_STEP_DOWN )
    {
	  if (GetSPO())
	  {
		  GetSPO()->SendValue( html.pipe, m_hWnd, nCode == HW_KEYUP ? 0 :-10 );Timer();return 1;
	  }
	}
  return CHTMLEditBox::HWBTN( nCode, uKey );
  }

void CHTML_OP::SetInfoBar()
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
					  //
					  if (m_bOP1)
					  {
						  //для PCA2193.P_2193_2.OP[1] искать надо DESC PCA2193.PIDA
						  int nFindPoint=strName.Find('.');
						  if (nFindPoint!=-1)
						  {
							  strName=strName.Left(nFindPoint);
							  strName+=".PIDA";
						  }
					  }
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

bool CHTML_OP::WindowPos( HWND hParent, SParamValueH &mVal, HFONT hFont, FLOAT fScale, Rect& rc )
{
   bool bInit = ( !::IsWindow( m_hWnd ) );
   bool bRet = __super::WindowPos( hParent, mVal, hFont, fScale, rc );

   if( bInit && mSPO && m_Val.pMulti )
     {
       for(std::map<std::string, std::pair<int, SValueDef*>>::iterator it = m_Val.pMulti->mDefs.begin(); it!=m_Val.pMulti->mDefs.end(); it++)
         {
             CSendOP *pSendOP = new CSendOP(html, mSPO->m_szFiltr, m_bOP1);
             pSendOP->Init(html.pipe, it->second.first);
             if(pSendOP)
               m_SPOs.insert(std::pair<std::string, CSpPvOp*>(it->first, pSendOP));
         }
     }

   return bRet;
}

void CHTML_OP::Timer()
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
