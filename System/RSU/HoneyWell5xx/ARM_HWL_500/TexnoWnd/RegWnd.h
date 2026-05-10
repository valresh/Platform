#pragma once

#include "BaseWnd.h"
#if 0
class CValEdit : public CEdit
  {
  DECLARE_DYNCREATE(CValEdit)
  DECLARE_MESSAGE_MAP()
  protected:
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  };
//
class CRegWnd : public CBaseWnd, public CShReg
  {
  DECLARE_DYNAMIC(CRegWnd)
  void SetMode(CReg::eMode mode);
  bool SetSP(double dVal);
  bool SetOP(double dVal);
  void IncSP(double dPersent);
  void IncOP(double dPersent);
  enum EInputType
    {
    no_input = 0,
    sp_input = 1,
    op_input = 2,
    }; 
  void BeginEdit(EInputType eInputType);
  void EndEdit();
  CValEdit   m_wndEdit;
  EInputType m_eInputType;
  HFONT m_h16,m_h18;
  //
  SParamValueH mPV;
  SParamValueH mSP;
  SParamValueH mOP;
  SParamValueH mMD;

  CStatic m_wndPV,m_wndSP,m_wndOP;
  CButton m_btnSP,m_btnOP;
  public:
    CRegWnd(KSmartValueTypeDV eSxema);
   ~CRegWnd();
    //
    virtual bool MyCreate( CWnd* pParent, bool bAviFors=false );
	  virtual void LoadProps();
  protected:
    virtual void OnOK();
    virtual void Specific( int& nDialog );
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	  DECLARE_MESSAGE_MAP()
    afx_msg void OnKickIdle();
    afx_msg void OnPaint();
    afx_msg void OnReturn(NMHDR * pNotifyStruct, LRESULT * result);
    afx_msg void OnSpinOP(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnSpinSP(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnSP();
    afx_msg void OnOP();
    afx_msg void OnMAN();
    afx_msg void OnAUTO();
    afx_msg void OnCAS();
  //
  };
#endif
