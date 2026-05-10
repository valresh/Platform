#pragma once

#include "SoftGDI.h"
//
class CBaseWnd : public CDialog
  {
	DECLARE_DYNAMIC(CBaseWnd)
  public:
	  CBaseWnd(EDataTypes eType,UINT eSxema);
	  virtual ~CBaseWnd();
    //
    virtual bool MyCreate( CWnd* pParent, bool bAviFors=false );
    bool MyCreate( HWND  hParent );
    //
	  virtual void LoadProps() = NULL;
    bool Dlg(CString& dlg);
    void CopyUnicodeToClipboard(CWnd* pParent,wchar_t*str,int nLength);
	void CopyPultNameToClipboardUnicode(CWnd* pParent);
	void ShowVideo(CString strFileName);
	BOOL IsFileInVideoDirectiry(CString strFileName);
	CString GetVideoFileName(); 
	bool ShowVideoForElement();
    //
    CString m_strName;
    EDataTypes m_eType;
    UINT m_eSxema;
  protected:
	  DECLARE_MESSAGE_MAP()
    //
    virtual void PostNcDestroy();
  protected:
    // Поиск специфичных диалогов (например, наличие дублёров)
    virtual void Specific( int& nDialog ) = NULL;
    virtual void Refresh( CShBase* sh );
	void RefreshPipe( CShBase* sh );
  protected:
    int m_nNumber;
  private:
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  protected:
    virtual void OnOK();
    virtual void OnCancel();
  private:
	  bool m_bAviFors;//для специфичного диалого AVI форсунок
  };
//
class CForsWnd : public CBaseWnd, public CShForsunka
  {
  DECLARE_DYNAMIC(CForsWnd)
  public:
    CForsWnd(UINT eSxema)
    : CBaseWnd(id_Forsunka,eSxema){}
	  virtual void LoadProps();
  protected:
    virtual void Specific( int& nDialog );
  //
  };
//
class CGasWnd : public CBaseWnd, public CShGasAnalyz
  {
  DECLARE_DYNAMIC(CGasWnd)
  public:
    CGasWnd(UINT eSxema)
    : CBaseWnd(id_GasAnalyz,eSxema){}
	  virtual void LoadProps();
  protected:
    virtual void Specific( int& nDialog );
  //
  };
//
class CHsWnd : public CBaseWnd, public CShHS
  {
  DECLARE_DYNAMIC(CHsWnd)
  public:
    CHsWnd(UINT eSxema)
    : CBaseWnd(id_HS,eSxema){}
	  virtual void LoadProps();
  protected:
    virtual void Specific( int& nDialog );
  //
  };
//

class CModelWnd : public CBaseWnd, public CShModelData
  {
  DECLARE_DYNAMIC(CModelWnd)
  public:
    CModelWnd(UINT eSxema)
    : CBaseWnd(id_ModelData,eSxema){}
	  virtual void LoadProps();
  protected:
    virtual void Specific( int& nDialog );
  //
  };
//

class CKlapWnd : public CBaseWnd, public CShKlapan
  {
  DECLARE_DYNAMIC(CKlapWnd)
  public:
    CKlapWnd(UINT eSxema)
    : CBaseWnd(id_Klapan,eSxema){}
	  virtual void LoadProps();
  protected:
    virtual void Specific( int& nDialog );
  //
  };
//
class CKvoWnd : public CBaseWnd, public CShKVO
  {
  DECLARE_DYNAMIC(CKvoWnd)
  public:
    CKvoWnd(UINT eSxema)
    : CBaseWnd(id_KVO,eSxema){}
	  virtual void LoadProps();
  protected:
    virtual void Specific( int& nDialog );
  //
  };
//
class CPolyHSWnd : public CBaseWnd, public CShPolyHS
  {
  DECLARE_DYNAMIC(CPolyHSWnd)
  public:
    CPolyHSWnd(UINT eSxema)
    : CBaseWnd(id_PHS,eSxema){}
	  virtual void LoadProps();
  protected:
    virtual void Specific( int& nDialog );
  //
  };

class CPumpWnd : public CBaseWnd, public CShPump
  {
  DECLARE_DYNAMIC(CPumpWnd)
  public:
    CPumpWnd(UINT eSxema)
    : CBaseWnd(id_Pump,eSxema){}
	  virtual void LoadProps();
  protected:
    virtual void Specific( int& nDialog );
  //
  };
//
class CSensWnd : public CBaseWnd, public CShSensor
  {
  DECLARE_DYNAMIC(CSensWnd)
  public:
    CSensWnd(UINT eSxema)
    : CBaseWnd(id_Sensor,eSxema){}
	  virtual void LoadProps();
  protected:
    virtual void Specific( int& nDialog );
	virtual void Refresh( CShBase* sh );
  //
  };
//
class CZadvWnd : public CBaseWnd, public CShZadv
  {
  DECLARE_DYNAMIC(CZadvWnd)
  public:
    CZadvWnd(UINT eSxema)
    : CBaseWnd(id_Zadv,eSxema){}
	  virtual void LoadProps();
  protected:
    virtual void Specific( int& nDialog );
  //
  };
//
class CValueWnd : public CBaseWnd, public CShValue
  {
  DECLARE_DYNAMIC(CValueWnd)
  public:
    CValueWnd(UINT eSxema)
    : CBaseWnd(id_Value,eSxema){}
	  virtual void LoadProps();
  protected:
    virtual void Specific( int& nDialog );
    virtual void Refresh( CShBase* sh );
  //
  };
//