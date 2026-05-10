#include "StdAfx.h"
//
#include "HTMLWndBase.h"
#include "HTML.h"
#include "..\\AplicFrm.h"
//
int CHTML_PV::HWBTN( WORD nCode, UINT uKey )
  {
  return 0;
  }

void CHTML_PV::SetInfoBar()
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