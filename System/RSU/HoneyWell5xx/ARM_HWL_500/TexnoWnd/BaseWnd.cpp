#include "stdafx.h"
#include "../ARM.h"
#include "TexnoFrm.h"
#include "BaseWnd.h"
//#include "InlineGDI.h"
#include "Queue.h"

#include "LogFile.h"

// CBaseWnd
IMPLEMENT_DYNAMIC(CBaseWnd,CDialog)
CBaseWnd::CBaseWnd(EDataTypes eType,UINT eSxema)
: m_eType (eType )
, m_eSxema(eSxema)
, m_nNumber(-1)
, m_bAviFors(false)
  {
  }

CBaseWnd::~CBaseWnd()
  {
  }

BEGIN_MESSAGE_MAP( CBaseWnd, CDialog )
  ON_WM_PAINT()
  ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

bool CBaseWnd::Dlg(CString& dlg)
  {
  int nTeg = ::FindTagEx( m_strName, m_eType );
  int nDialog = 0;
  CString str = "#";// Поиск в директории Dialogs
  switch ( m_eType )
    {
    default           : str += "Unknown"     ; break;
    case id_Zadv      : str += "Задвижки"    ; break;
    case id_Sensor    : str += "Датчики"     ; break;
    case id_KVO       : str += "КВО"         /*, nDialog = 2*/; break;
	case id_PHS       : str += "ППК", nDialog=-1;break;
    case id_Pump      : str += "Насосы"      , nDialog = 15; break;
    case id_HS        : str += "Ключи"       ; break;
    case id_Klapan    : str += "Клапана"     ; break;
    case id_Reg       : str += "Регуляторы"  ; break;
    case id_Forsunka  : str += "Форсунки"    ; break;
    case id_GasAnalyz : str += "Анализаторы" , nDialog = 1; break;
    case id_HeatChange: str += "Теплообмен"  ; break;
    case id_Value     : str = "@Пульты/Пульт"; break;
	case id_ModelData : str += "Модели"  ; break;
    };
  if ( m_eType != id_Value ) str += "/Dialog";
  //
  if (Version>=H2O2_VP)
  if ((m_eType==id_Sensor)&&(nDialog==0))
      nDialog=4;

  int nStandartDialogFromCSV=theApp.GetStandartDialogNumberFromCSVFile(m_eType,m_eSxema, m_strName.GetBuffer());
  m_strName.ReleaseBuffer();
  if (nStandartDialogFromCSV!=-1)
	  nDialog=nStandartDialogFromCSV;
	
  nDialog = TegValueI( nTeg, "dlg", nDialog );
  if ((m_eType==id_Klapan)||(m_eType==id_Pump)||(m_eType==id_Zadv)||(m_eType==id_KVO)||(m_eType==id_ModelData)||(m_eType==id_GasAnalyz)
	  ||(m_eType==id_HS)||(m_eType==id_Sensor))
		nDialog=theApp.GetDialogNumber( m_strName,nDialog);

   if ((m_eSxema==id_Sensor_45)||(m_eSxema==id_Sensor_46)||(m_eSxema==id_Sensor_47))
	  nDialog=(int)m_eSxema;
  if ( nDialog == -1 ) return false;
  //
  if ( nDialog == 0 ) Specific(nDialog);
  if ( nDialog <  0 ) return false;
  //
  if ( nDialog == 0 ) dlg = str;
  else dlg.Format( "%s-%d", str, nDialog );
  return true;
  }

void CBaseWnd::CopyUnicodeToClipboard(CWnd* pParent,wchar_t*str,int nLength)
{
	if (pParent->OpenClipboard())
	{
		size_t len = (nLength+1)*(sizeof(wchar_t));
		HGLOBAL hMem =  GlobalAlloc(GMEM_MOVEABLE, len);
		memcpy(GlobalLock(hMem), str, len);
		GlobalUnlock(hMem);
		EmptyClipboard();
		UINT uFormat=CF_UNICODETEXT;
		SetClipboardData(uFormat, hMem);
		CloseClipboard();
	}
}

void CBaseWnd::CopyPultNameToClipboardUnicode(CWnd* pParent)
{
  CString text=m_strName;
  int nLength=text.GetLength();
  const int nLengthMax=128;
  wchar_t strUnicode[nLengthMax];
  if (nLength<nLengthMax)
  {
	  mbstowcs(strUnicode, text.GetBuffer(), nLength);
	  if( (pParent!=NULL)&& text.GetLength()) 
			  CopyUnicodeToClipboard(pParent,strUnicode,nLength);
	  text.ReleaseBuffer();
  }
}

CString CBaseWnd::GetVideoFileName()
{
  CString strFileName=GetBasePath();
  strFileName+="Data\\Video\\";
  strFileName+=m_strName;
  strFileName+=".wmv";
  return strFileName;
}

BOOL CBaseWnd::IsFileInVideoDirectiry(CString strFileName)
{
  CFileFind find;
  BOOL bFileFind = find.FindFile (strFileName);
  return bFileFind;
}

void CBaseWnd::ShowVideo(CString strFileName)
{
	  //CVideoDialog video_dlg;
	  //video_dlg.SetFileName(strFileName);
	  //video_dlg.DoModal();
}

bool CBaseWnd::ShowVideoForElement()
{
	if ( (GetAsyncKeyState ( VK_CONTROL ) & 0x8000))
	{
		CString strFileName=GetVideoFileName();
		if (IsFileInVideoDirectiry(strFileName))
      {
			ShowVideo(strFileName);
      return true;
      }
	}
return false;
}

bool CBaseWnd::MyCreate( CWnd* pParent, bool bAviFors )	
  {
	  CopyPultNameToClipboardUnicode(pParent);
	  ShowVideoForElement();
	  char add = 0;
	  m_bAviFors=bAviFors;
	  if ( m_bAviFors ) add = ' ';
	  //
	  CString dlg;
	  if ( !Dlg(dlg) )
		return false;
	  //
	  DWORD dwStyle = WS_SYSMENU | WS_CAPTION | WS_POPUP
				  | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_THICKFRAME;
	  DWORD dwExStyle = WS_EX_TOOLWINDOW | WS_EX_WINDOWEDGE;
	  CRect rect(0,0,0,0);
	  BOOL bRes = CreateEx( dwExStyle, CLASS_TEXNO, m_strName, dwStyle, rect, pParent, 0 );
	  if ( !bRes ) return false;
	  //
	  SetOwner( pParent );
	  //
	  if ( m_eType == id_Value ) MainPipe->Obj( m_strName );
	  else
		{
		if ((m_eType==id_Forsunka)||(m_eType==id_GasAnalyz))
		{
			m_nNumber = MainPipe->FindObj(m_eType,m_strName,m_strName);
			MainPipe->End();
			if (!m_bAviFors)
			  MainPipe->ChangeBusy(m_eType,m_strName,true);
		}
		else
		{
			m_nNumber =MainSocket->FindObj(m_eType,m_strName,m_strName);
			MainSocket->Refresh();
		}
		if (!m_bAviFors)
		{
		  MainSocket->ChangeBusy(m_eType,m_strName,true);
		}
		}
  if ( m_eType == id_Klapan )
  {
	    CShKlapan* sh = (CShKlapan*)MainSocket->Sh( m_nNumber );
	    if (IsTypeOk)
	    {
			if (sh->Is_Type(CKlapan::IS_ELECTRO_ZADV))
			{
				if (theApp.IsDefaultKlapanDialog(dlg.GetBuffer()))
				{
					std::string strElectoDialog=theApp.GetElectroZadvDialogCSV();
					if (strElectoDialog!="")
						dlg="#Клапана/Dialog-"+CString(strElectoDialog.c_str());
					else
						dlg="#Клапана/Dialog-24";
				}
				dlg.ReleaseBuffer();
			}
		}
  }
	  ::SetMessageWnd( m_hWnd, pParent->m_hWnd );
	  ::SendMessage  ( m_hWnd, nBuildQueue, BUILD_MNEMO, (LPARAM)(LPCTSTR)dlg );
	  if ( add ) m_strName += add;
	  //
	  if ( m_eType == id_Value ) MainPipe->Obj( "" ), MainPipe->End();
	  //
	  return true;
  }

bool CBaseWnd::MyCreate( HWND hParent )
  {
  //
  CString dlg;
  if ( !Dlg(dlg) )
    return false;
  //
  DWORD dwStyle = WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_MAXIMIZE;
  DWORD dwExStyle = WS_EX_TOOLWINDOW;
  BOOL bRes = CreateEx( dwExStyle, CLASS_TEXNO, m_strName, dwStyle, 0,0,0,0, hParent, HMENU(AFX_IDW_PANE_FIRST) );
  if ( !bRes ) return false;
  //
  //SetOwner( pParent );
  //
  if ( m_eType == id_Value ) MainPipe->Obj( m_strName );
  else
    {
		if ((m_eType==id_Forsunka)||(m_eType==id_GasAnalyz))
		{
			m_nNumber = MainPipe->FindObj(m_eType,m_strName,m_strName);
			MainPipe->End();
			if (!m_bAviFors)
			  MainPipe->ChangeBusy(m_eType,m_strName,true);
		}
		else
		{
			m_nNumber =MainSocket->FindObj(m_eType,m_strName,m_strName);
			MainSocket->Refresh();
			MainSocket->ChangeBusy(m_eType,m_strName,true);
		}
    }
    if ( m_eType == id_Klapan )
  {
	    CShKlapan* sh = (CShKlapan*)MainSocket->Sh( m_nNumber );
	    if (IsTypeOk)
	    {
			if (sh->Is_Type(CKlapan::IS_ELECTRO_ZADV))
			{
				if (theApp.IsDefaultKlapanDialog(dlg.GetBuffer()))
				{
					std::string strElectoDialog=theApp.GetElectroZadvDialogCSV();
					if (strElectoDialog!="")
						dlg="#Клапана/Dialog-"+CString(strElectoDialog.c_str());
					else
						dlg="#Клапана/Dialog-24";
				}
				dlg.ReleaseBuffer();
			}
		}
  }
  ::SetMessageWnd( m_hWnd, hParent );
  ::SendMessage  ( m_hWnd, nBuildQueue, BUILD_MNEMO, (LPARAM)(LPCTSTR)dlg );
  //
  if ( m_eType == id_Value ) MainPipe->Obj( "" ), MainPipe->End();
  //
  return true;
  }

void CBaseWnd::Refresh( CShBase* SH )
  {
  //CShBase* sh = MainPipe->Sh( m_nNumber );
  CShBase* sh = MainSocket->Sh( m_nNumber );
  if ( IsTypeOk )
  if ( memcmp( SH, sh, SH->m_nSize ) != 0 || sh->Busy > 1 )
    {
    memcpy( SH, sh, SH->m_nSize );
    CDC* pDC = GetDC();
    if ( pDC )
      {
      PaintDlg( m_hWnd, *pDC, dwMainTimer );
      ReleaseDC( pDC );
      }
    }
  }

void CBaseWnd::RefreshPipe( CShBase* SH )
  {
  CShBase* sh = MainPipe->Sh( m_nNumber );
  if ( IsTypeOk )
  if ( memcmp( SH, sh, SH->m_nSize ) != 0 || sh->Busy > 1 )
    {
    memcpy( SH, sh, SH->m_nSize );
    CDC* pDC = GetDC();
    if ( pDC )
      {
      PaintDlg( m_hWnd, *pDC, dwMainTimer );
      ReleaseDC( pDC );
      }
    }
  }

void CBaseWnd::PostNcDestroy()
  {
  if (!m_bAviFors)
  {
  	MainSocket->ChangeBusy(m_eType,m_strName,false);
  }
  delete this;
  }

void CBaseWnd::OnPaint()
  {
  CPaintDC dc(this); // device context for painting
  PaintDlg( m_hWnd, dc, dwMainTimer );
  }

BOOL CBaseWnd::OnEraseBkgnd(CDC* pDC)
  {
  return FALSE;//CDialog::OnEraseBkgnd(pDC);
  }

void CBaseWnd::OnOK()
  {
  CDialog::OnOK();
  DestroyWindow();
  }

void CBaseWnd::OnCancel()
  {
  CDialog::OnCancel();
  DestroyWindow();
  }
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CForsWnd,CBaseWnd)
extern int g_nForsDialog;
// Поиск специфичных диалогов (например, наличие дублёров)
/*virtual*/void CForsWnd::Specific( int& nDialog )
  {
  // MainPipe->GetData( m_eType, m_strName, this );
  /*
  STeg teg;
  int nTeg = ::FindTegEx( m_strName, teg, m_eType );
  nDialog = TegValueI( nTeg, "dlg", 0 );
  if ( nDialog == 0 ) nDialog = -1;
  */
  nDialog = g_nForsDialog;
  g_nForsDialog = -1;
  }

/*virtual*/ void CForsWnd::LoadProps()
  {
  RefreshPipe( this );
  }
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CGasWnd,CBaseWnd)
// Поиск специфичных диалогов (например, наличие дублёров)
/*virtual*/void CGasWnd::Specific( int& nDialog )
  {
  // MainPipe->GetData( m_eType, m_strName, this );
  }

/*virtual*/ void CGasWnd::LoadProps()
  {
  RefreshPipe( this );
  }
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CHsWnd,CBaseWnd)
// Поиск специфичных диалогов (например, наличие дублёров)
/*virtual*/void CHsWnd::Specific( int& nDialog )
  {
  MainSocket->GetData( m_eType, m_strName, this );
  //nDialog =  4;return;
  //
  if ((m_eSxema==id_ShZaslonka)&&(nDialog!=0))
	  return;
  switch ( m_eSxema )
    {
    case id_ShZaslonka : nDialog = 1; break;
    }
  //
  }

/*virtual*/ void CHsWnd::LoadProps()
  {
  Refresh( this );
  }

IMPLEMENT_DYNAMIC(CModelWnd,CBaseWnd)
// Поиск специфичных диалогов (например, наличие дублёров)
/*virtual*/void CModelWnd::Specific( int& nDialog )
  {
  MainSocket->GetData( m_eType, m_strName, this );
  if (m_eSxema)
	  nDialog=m_eSxema;
  //
  }

/*virtual*/ void CModelWnd::LoadProps()
  {
  Refresh( this );
  }
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CKlapWnd,CBaseWnd)
// Поиск специфичных диалогов (например, наличие дублёров)
/*virtual*/void CKlapWnd::Specific( int& nDialog )
  {
  MainSocket->GetData( m_eType, m_strName, this );
  //nDialog = 7; return;
  //
  int nType = (Type_Flags & CKlapan::DUBLER_YES);
  if ( nType == CKlapan::AUTO_ZACEP )
    nDialog = 3;
  else
  if ( nType == CKlapan::DUBL_HYDRO )
    nDialog = 1;
  else
  if ( nType == CKlapan::DUBL_ZACEP )
    nDialog = 2;
  //
  }

/*virtual*/ void CKlapWnd::LoadProps()
  {
  Refresh( this );
  }
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CKvoWnd,CBaseWnd)
// Поиск специфичных диалогов (например, наличие дублёров)
/*virtual*/void CKvoWnd::Specific( int& nDialog )
  {
  //nDialog = 1;return;
  //MainPipe->GetData( m_eType, m_strName, this );
  }

/*virtual*/ void CKvoWnd::LoadProps()
  {
  Refresh( this );
  }
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CPolyHSWnd,CBaseWnd)
// Поиск специфичных диалогов (например, наличие дублёров)
/*virtual*/void CPolyHSWnd::Specific( int& nDialog )
  {
  //nDialog = 1;return;
  //MainPipe->GetData( m_eType, m_strName, this );
	  nDialog = 3;
  }

/*virtual*/ void CPolyHSWnd::LoadProps()
  {
  Refresh( this );
  }
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CPumpWnd,CBaseWnd)
// Поиск специфичных диалогов (например, наличие дублёров)
/*virtual*/void CPumpWnd::Specific( int& nDialog )
  {
  //nDialog = 16; return;
  // MainPipe->GetData( m_eType, m_strName, this );
  }

/*virtual*/ void CPumpWnd::LoadProps()
  {
  Refresh( this );
  }
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CSensWnd,CBaseWnd)
// Поиск специфичных диалогов (например, наличие дублёров)
/*virtual*/void CSensWnd::Specific( int& nDialog )
  {
  // MainPipe->GetData( m_eType, m_strName, this );
  }

/*virtual*/ void CSensWnd::LoadProps()
  {
  Refresh( this );
  }

/*virtual*/ void CSensWnd::Refresh( CShBase* sh )
  {
  CDC* pDC = GetDC();
  if ( pDC )
    {
    PaintDlg( m_hWnd, *pDC, dwMainTimer );
    ReleaseDC( pDC );
    }
  }
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CZadvWnd,CBaseWnd)
// Поиск специфичных диалогов (например, наличие дублёров)
/*virtual*/void CZadvWnd::Specific( int& nDialog )
  {
  MainSocket->GetData( m_eType, m_strName, this );
  //nDialog = 12;return;
  //
  switch ( m_eSxema )
    {
    case id_3xZadv     : nDialog = 10; break;
    case id_ShBallZadv : nDialog =  8; break;
    case id_Ventil     : nDialog =  4; break;
    case id_ShZaslonka : nDialog = 11; break;
	case id_ZadvZaglushka : nDialog = 22; break;
    }
  //
  }

/*virtual*/ void CZadvWnd::LoadProps()
  {
  Refresh( this );
  }
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CValueWnd,CBaseWnd)
// Поиск специфичных диалогов (например, наличие дублёров)
/*virtual*/void CValueWnd::Specific( int& nDialog )
  {
  switch ( m_eSxema )
    {
    case id_Turbina: nDialog = 4; break;
    };
  }

/*virtual*/ void CValueWnd::LoadProps()
  {
  Refresh( this );
  }

/*virtual*/ void CValueWnd::Refresh( CShBase* sh )
  {
  CDC* pDC = GetDC();
  if ( pDC )
    {
    PaintDlg( m_hWnd, *pDC, dwMainTimer );
    ReleaseDC( pDC );
    }
  }
//-----------------------------------------------------------------------------
