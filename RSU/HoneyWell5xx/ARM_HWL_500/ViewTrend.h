#pragma once

#include "PipeTrends.h"
#include "ViewTrendBase.h"
#include "Trend3.h"
//
class CViewTrend : public CViewTrendBase
  {
  STrendItem mItem[8];
  static COLORREF m_arrColor[];
  //
    int  m_nChildData;
    void SetCurselPoint ();
    void SetCurselIntrvl();
    void SetCurselRazmah();
  public:
    CViewTrend(LPCTSTR szNumber);
    virtual ~CViewTrend();
    static CDataTrends& Data();
  protected:
    virtual void LoadGrup(int nSetNP);
    virtual void MainTimer(__time64_t nLast);
    virtual void PostTimer(__time64_t nLast);
    DECLARE_MESSAGE_MAP()
  public:
    afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSave_Config();
    void OnCbnSelchangeData  ();
    //
  };
