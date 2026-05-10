// ViewTrend.cpp : implementation file
//
#include "stdafx.h"
#include "ARM.h"
#include "AplicFrm.h"
#include "ViewTrend.h"

// CViewTrend
CViewTrend::CViewTrend(LPCTSTR szNumber)
: CViewTrendBase(szNumber,mItem)
, m_nChildData(0)
  {
  }
//
COLORREF CViewTrend::m_arrColor[] =
  {0xffff00,0xffffff,0x00ff00,0x00ffff,0xff00ff,0x0000ff,0xff0000,0xcc0066,
  0x6dd72d,0x2f72df, 0x6228da,0xc7cb61,0xa0a0a0,0x38d678, 0x6ad9d2, 0xc15dd4,
  0x616ad9, 0xc44d02,0xc54f5c,0x64cd72,0x3c6cd6,0x9061cf,0x5c6124,0x5c5c5c,
  0x195c1c,0x216165,0x441357,0x141956,0x6c1a20,0x7f2c4a,0x2d5a22,0x142858};
//
CViewTrend::~CViewTrend()
  {
  }

BEGIN_MESSAGE_MAP(CViewTrend,CViewTrendBase)
  ON_WM_CREATE()
  ON_CBN_SELCHANGE(1023, OnCbnSelchangeData  )
  ON_BN_CLICKED( IDC_SAVE_CONFIG, OnSave_Config)
END_MESSAGE_MAP()
//
int CViewTrend::OnCreate(LPCREATESTRUCT lpCreateStruct)
  {
  if (CViewTrendBase::OnCreate(lpCreateStruct) == -1)
    return -1;
  //
  LoadGrup(0);
  return 0;
  }
//
void CViewTrend::LoadGrup(int nSetNP)
  {
  CDataTrends& mData = Data();
  m_nChildData = mData.ItemNumber(m_nTrend);
  mTrend.ClearToT();
  //
  m_nCount = 8;
  STrends* data = mData.Item(m_nChildData);
  if(!data)
	return;
  if ( nSetNP == 0 )
  memset( mItem, 0, sizeof(mItem) );
  //
  mPipe.Begin();
  mTrend.SetPoint(500);
  for ( int n = 0; n < 8; n++ )
    {
    mItem[n].n = n;
    mItem[n].Fill( nSetNP, mData, data->point[n] );
    //
    const char* point = mData.Text(data->point[n].hPoint);
    const char* param = mData.Text(data->point[n].hParam);
    //
    mItem[n].mDesc   = ::FindExperion( mPipe, point, "desc" );// BLCKCOMMENT1
    mItem[n].mCurr   = ::FindExperion( mPipe, point, param  );
    mTrend.Add(point,param);
    }
  mPipe.End();
  //
  for ( int n = 0; n < m_nCount; n++ )
    mItem[n].SetEngScale( mPipe );
  //
  m_btnEUPR.Work(data->nScaleEng);
  //
  SetCurselPoint ();
  SetCurselRazmah();
  SetCurselIntrvl();
  //
  OnTimer(2);
  }
//
void CViewTrend::SetCurselPoint ()
  {
  FillComboPoint();
  STrends* data = Data().Item(m_nChildData);
  int nSel = data->nSelect;
  nSel = max( nSel, 0 );
  nSel = min( nSel, m_nCount );
  boxPoints.SetCurSel( nSel );
  OnCbnSelchangePoints();
  }

void CViewTrend::SetCurselIntrvl()
  {
  STrends* data = Data().Item(m_nChildData);
  int nSel = data->nInterval;
  nSel = max( nSel, 0 );
  nSel = min( nSel, 5 );
  boxIntrvl.SetCurSel( nSel );
  m_nTimer = SetTimer( ID_TREND_TIMER, arrInterval[nSel].nSec*1000, NULL );
  }

void CViewTrend::SetCurselRazmah()
  {
  STrends* data = Data().Item(m_nChildData);
  int nSel = data->nRazmah;
  nSel = max( nSel, 0 );
  nSel = min( nSel,11 );
  boxPeriod.SetCurSel( nSel );
  }
//
/*virtual*/void CViewTrend::MainTimer(__time64_t nLast)
  {
  int nRazmax = Sec();
  mTrend.TrendPipeMisha( nLast, int(nRazmax) );
  mBeginTime = mTrend.Tot().nBeginTime;
  //
  CTime tm(mTrend.Tot().nLastTime);
  CString str = tm.Format("%H:%M:%S");
  editTim.SetWindowText(str);
  //
  if ( boxData.GetCount() == 0 )
    {
    CTime dt(mTrend.Tot().nLastTime);
    str = dt.Format("%d.%m.%y");
    boxData.AddString(str);
    boxData.SetCurSel( 0 );
    }
  //
  }
//
/*virtual*/void CViewTrend::PostTimer(__time64_t nLast)
  {
  }
//
void CViewTrend::OnCbnSelchangeData()
  {
  int nSel = boxData.GetCurSel();
  if ( nSel == CB_ERR ) return;
  }
//
void CViewTrend::OnSave_Config()
  {
  CDataTrends& mData = Data();
  int nSel;
  m_btnSave.Enable( false );
  STrends* data = mData.Item(m_nChildData);
  data->nSelect = m_nSelect;
  //
  nSel = boxIntrvl.GetCurSel();
  if ( nSel != CB_ERR ) data->nInterval = nSel;
  nSel = boxPeriod.GetCurSel();
  if ( nSel != CB_ERR ) data->nRazmah = nSel;
  data->nScaleEng = m_btnEUPR.Work();
  //
  for ( int n = 0; n < m_nCount; n++ )
    mItem[n].Fall( 0, mData, data->point[n] );
  mData.SaveFile();
  OnTimer(2);
  }
//