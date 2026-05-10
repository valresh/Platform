#pragma once
#include "Resource.h"
#include "UniBuffer.h"
#include "ArrayObj.h"
#include "TexData.h"
#include "afxwin.h"
#include <vector>
#include "TexChild.h"
//
class CTexChild;
// Диалог, содержащий все элементы на всех схемах
class CDialName : public CDialog, public CTexData
  {
	DECLARE_DYNAMIC(CDialName)
  CTexChild* m_pSend;
  public:
	  CDialName();
	  virtual ~CDialName();
	  // создание немодального диалога
	  virtual BOOL Create(CTexChild* pParent);
    // Dialog Data
	  enum { IDD = IDD_FINDNAME };
    //
  protected:
	  CTreeCtrl  m_wndTree;
	  CImageList m_imgList;
    CEdit m_MaskCtrl;
    int m_TreeYbottom, m_MaskYtop;
    bool FillTree( const char* pSubString = NULL );//true если используется фильтр
    std::vector<BYTE> m_vFlagForShow;
    //
  protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	  DECLARE_MESSAGE_MAP()
    //
    virtual BOOL OnInitDialog();
    virtual void OnOK();
//    virtual void OnCancel();
    afx_msg void OnSize(UINT nType, int cx, int cy);
	  afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	  afx_msg void OnTvnItemexpandedTree1(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnEnChangeMask();
};
