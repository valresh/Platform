#include "stdafx.h"
#include "ProgressDlg.h"
#include "CnfReaderUI.h"


static KProgressDlg * pDlg = NULL;
HINSTANCE hRes = NULL;
// KProgressDlg dialog
void SetProgress( LPCSTR Title, int Proc )
{
  if ( pDlg == NULL)                                                                                                        
  {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    HINSTANCE hOld = AfxGetResourceHandle( );
    //AfxSetResourceHandle( hRes ); 
    pDlg = new KProgressDlg;
    pDlg->Create( KProgressDlg::IDD, NULL/*pMainWnd*/ );
    pDlg->ShowWindow( SW_SHOW );
    AfxSetResourceHandle( hOld ); 
  }
  pDlg->m_Txt.SetWindowText( Title );
  pDlg->m_Progress.SetPos( Proc );
}


void CloseProgress()
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  if( !pDlg )
    return;
  pDlg->DestroyWindow();
  delete pDlg;
  pDlg = NULL;
}

IMPLEMENT_DYNAMIC(KProgressDlg, CDialog)

KProgressDlg::KProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(KProgressDlg::IDD, pParent)
{
}

KProgressDlg::~KProgressDlg()
{
}

void KProgressDlg::OnDestroy()
{
  pDlg = NULL;
  CDialog::OnDestroy();
}

void KProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
  DDX_Control(pDX, IDC_TEXT, m_Txt);
}


BEGIN_MESSAGE_MAP(KProgressDlg, CDialog)
END_MESSAGE_MAP()


// KProgressDlg message handlers
