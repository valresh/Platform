 #pragma once
#include "..\\ARM.h"
//#include "..\\DlgDebug.h"
#include "Common.h"
#include "HTML/BaseHTML.h"
//
#undef  WND_POS
#define WND_POS HWND hParent, int nNumber, HFONT hFont, FLOAT fScale, Rect& rc
//
// #include "..\\SendSP.h"
// #include "..\\SendPV.h"
// #include "..\\SendOP.h"
#include <string>
#include <map>


#include <HoneyMnemoExtend.h>
#include <HTML/BaseHTML.h>
//
class CHTMLWndBase
  {
  protected:
    int  m_nFontHeight;
    CHTML&       html;
    WNDPROC m_oldProc;
    virtual bool Create( HWND hParent, const char* pszClass, DWORD dwStyle, DWORD dwExStyle = 0 );
    void SetCtrlFont( HFONT hFont, FLOAT fScale );
    bool MessageB( SValueDef* def, char* value );
  public:
    HWND m_hWnd;
    CHTMLWndBase(CHTML& _html);
    virtual ~CHTMLWndBase(void);
 //   virtual bool WindowPos( WND_POS ) = 0;
    virtual LRESULT OnMessage( UINT uMsg, WPARAM wParam, LPARAM lParam );
    virtual void Timer() = 0;
    virtual void Pressed(){};
    virtual void Release(){};
    virtual int HWBTN(WORD nCode,UINT uKey) { return 0; };
    //
    virtual bool IsAlarmWnd() { return false; }
  };
//
class CHTMLComboBox : public CHTMLWndBase
  {
  protected:
	float mScale;
    int m_nOldSelect;
  public:
    CHTMLComboBox(CHTML& _html);
    virtual ~CHTMLComboBox(void);
 //  virtual bool WindowPos( WND_POS );
  };
//
// class CHTMLEditBox : public CHTMLWndBase
//   {
//   protected:
//     CSpPvOp* mSPO;
//     SParamValueH m_Val;
//     std::map<std::string, CSpPvOp*> m_SPOs;
//   public:
//     CHTMLEditBox(CHTML& _html,CSpPvOp* _SPO);
// 	CSpPvOp* GetSPO(){return mSPO;};
//     virtual ~CHTMLEditBox(void);
//     virtual bool WindowPos( WND_POS );
//     virtual bool WindowPos( HWND hParent, SParamValueH &mVal, HFONT hFont, FLOAT fScale, Rect& rc );
//     virtual LRESULT OnMessage( UINT uMsg, WPARAM wParam, LPARAM lParam );
//     virtual void Timer();
//     virtual void Pressed();
//     virtual void Release();
//     virtual int HWBTN(WORD nCode,UINT uKey);
// 	virtual void SetInfoBar(){;};
// 	CSpPvOp* GetPointSPO(){ return mSPO;};
//     bool IsChangeParamValue( SParamValueH& var );
//     bool m_bEdited;
//   };
//
// Нумераторы
class CHTML_ENUM : public CHTMLComboBox
  {
  cross::SParamValueH& mVal;
  int m_HiddenElements;
  int m_nAcronymNumber;
  int m_nAcronymRecord;
  bool m_bButDown;
  bool m_bColorCombobox;
  DWORD m_clrFill;
  DWORD m_clrText;
  bool m_bNeedRedraw;
  void ReDraw();
  public:
    CHTML_ENUM(CHTML& _html, cross::SParamValueH& val, CTag *pTag = NULL);
    virtual ~CHTML_ENUM(){}
 //   virtual bool WindowPos( WND_POS );
    virtual LRESULT OnMessage( UINT uMsg, WPARAM wParam, LPARAM lParam );
    virtual void Timer();
    virtual int HWBTN(WORD nCode,UINT uKey);
    void SetInfoBar(cross::CString strName);
  };
// Нумераторы
class CHTML_ENUM2 : public CHTMLComboBox
  {
  cross::SParamValueH& mVal;
  public:
    CHTML_ENUM2(CHTML& _html,cross::SParamValueH& val)
    : CHTMLComboBox( _html ), mVal(val) {}
    virtual ~CHTML_ENUM2() {}
//    virtual bool WindowPos( WND_POS );
    virtual LRESULT OnMessage( UINT uMsg, WPARAM wParam, LPARAM lParam );
    virtual void Timer();
    virtual int HWBTN(WORD nCode,UINT uKey);
  };
// Нумераторы
typedef struct _COMBO_LIST
{
    char *pshowItem;
    char *pparam;
    int  val2set;
}  COMBO_LIST;
class CHTML_ENUM_LIST : public CHTMLComboBox
  {
  std::vector<cross::SParamValueH> mVals;
  COMBO_LIST *m_pDataList;
  int m_dListCount;
  public:
    CHTML_ENUM_LIST(CHTML& _html, COMBO_LIST *DataList);
    virtual ~CHTML_ENUM_LIST(){}
    virtual LRESULT OnMessage( UINT uMsg, WPARAM wParam, LPARAM lParam );
//    virtual bool WindowPos( WND_POS );
    virtual void Timer() {}
  };
//
typedef struct _COMBO_LIST2
{
    std::string showItem;
    std::string paramRead;
    std::string paramWrite;
    O_VAL  val2set;
}  COMBO_LIST2;
// class CHTML_ENUM_LIST2 : public CHTMLComboBox
//   {
//   std::vector<SParamValueH> mValReads;
//   std::vector<SParamValueH> mValWrites;
//   std::vector<COMBO_LIST2> m_DataList;
//   public:
//     CHTML_ENUM_LIST2(CHTML& _html, std::vector<COMBO_LIST2> *pDataList, bool bFullTagName = false);
//     virtual ~CHTML_ENUM_LIST2(){}
//     virtual LRESULT OnMessage( UINT uMsg, WPARAM wParam, LPARAM lParam );
//     virtual bool WindowPos( WND_POS );
//     virtual void Timer();
//   };
// //
// // Нумераторы
// class CHTML_PVOP : public CHTMLComboBox
//   {
//   SParamValueH& mVal;
//   public:
//     CHTML_PVOP(CHTML& _html,SParamValueH& val)
//     : CHTMLComboBox( _html ), mVal(val) {}
//     virtual ~CHTML_PVOP() {}
//     virtual bool WindowPos( WND_POS );
//     virtual LRESULT OnMessage( UINT uMsg, WPARAM wParam, LPARAM lParam );
//     virtual void Timer();
//     virtual int HWBTN(WORD nCode,UINT uKey);
//   };
// class CHTML_PVOP_M : public CHTMLWndBase
//   {
//   float mScale;
//   const char* m_pszScriptValue;
//   std::vector<std::string> m_strings;
//   std::vector<SParamValueH> m_Vals;
//   public:
//       CHTML_PVOP_M(CHTML& _html, const char* pszScriptValue, std::vector<std::string>& strings )
//     : CHTMLWndBase( _html ), m_pszScriptValue(pszScriptValue), m_strings(strings),mScale(1.) {}
//     virtual ~CHTML_PVOP_M() {}
//     virtual bool WindowPos( WND_POS );
//     virtual LRESULT OnMessage( UINT uMsg, WPARAM wParam, LPARAM lParam );
//     virtual void Timer();
//     virtual int HWBTN(WORD nCode,UINT uKey);
//   };
// class ViewCombo : public CHTMLComboBox
// {
// 	CTag* almtag;
//   public:
// 	ViewCombo(CHTML& _html);
//     virtual ~ViewCombo() {}
// 	virtual bool WindowPos( WND_POS );
// 	virtual LRESULT OnMessage( UINT uMsg, WPARAM wParam, LPARAM lParam );
// 	virtual void Timer();
// 	virtual int HWBTN(WORD nCode,UINT uKey);
// };
// //
// class CHTML_SP : public CHTMLEditBox
//   {
//   public:
//     CHTML_SP(CHTML& _html,const char* pszFormat)
//     : CHTMLEditBox( _html, new CSendSP(pszFormat) ){}
//     ~CHTML_SP() {}
//     virtual int HWBTN(WORD nCode,UINT uKey);
// 	virtual void SetInfoBar();
//     virtual bool WindowPos( HWND hParent, SParamValueH &mVal, HFONT hFont, FLOAT fScale, Rect& rc );
//     virtual void Timer();
//   };
// //
// class CHTML_PV : public CHTMLEditBox
//   {
//   public:
//     CHTML_PV(CHTML& _html,const char* pszFormat)
//     : CHTMLEditBox( _html, new CSendPV(pszFormat) ){}
//     virtual ~CHTML_PV() {}
//     virtual int HWBTN(WORD nCode,UINT uKey);
// 	virtual void SetInfoBar();
//   };
// //
// class CHTML_OP : public CHTMLEditBox
//   {
//   public:
//     CHTML_OP(CHTML& _html,const char* pszFormat, bool bOP1=false)
//     : CHTMLEditBox( _html, new CSendOP(_html, pszFormat,bOP1) ), m_bOP1(bOP1) {}
//     virtual ~CHTML_OP() {}
//     virtual int HWBTN(WORD nCode,UINT uKey);
// 	virtual void SetInfoBar();
//     virtual bool WindowPos( HWND hParent, SParamValueH &mVal, HFONT hFont, FLOAT fScale, Rect& rc );
//     virtual void Timer();
//   private:
// 	bool m_bOP1;
//   };
// //
// class CHTML_Alarm : public CHTMLWndBase
//   {
//   COLORREF m_txt,m_bkg;
//   public:
//   class CViewBase* wnd;
//   public:
//     CHTML_Alarm(CHTML& _html,COLORREF txt,COLORREF bkg)
//       : CHTMLWndBase( _html ), wnd(NULL), m_txt(txt), m_bkg(bkg) {}
//    ~CHTML_Alarm();
//     virtual bool WindowPos( WND_POS );
//     virtual void Timer();
//     virtual int HWBTN(WORD nCode,UINT uKey);
//     //
//     virtual bool IsAlarmWnd() { return true; }
//   };
// //
// class CHTML_Trend : public CHTMLWndBase
//   {
//   class CViewTrendBaseBase* wnd;
//   COLORREF m_txt,m_bkg;
//   int mDsd;
//   int mTr3;
//   public:
//     CHTML_Trend(CHTML& _html,COLORREF txt,COLORREF bkg,int nDsd)
//     : CHTMLWndBase( _html ), wnd(NULL), m_txt(txt), m_bkg(bkg), mDsd(nDsd)
//     , mTr3(0)
//       {
//       }
//    ~CHTML_Trend();
//     virtual bool WindowPos( WND_POS );
//     virtual void Timer();
//     virtual int HWBTN(WORD nCode,UINT uKey);
//   };
// //
// class CHTML_Chart : public CHTMLWndBase
//   {
//   class CFrameChart* wnd;
//   public:
//     CHTML_Chart(CHTML& _html)
//       : CHTMLWndBase( _html ), wnd(NULL) {}
//    ~CHTML_Chart();
//     virtual bool WindowPos( WND_POS );
//     virtual void Timer();
//   };
// //
// class CHTML_Mnemo : public CHTMLWndBase
//   {
//   class CViewMnemo* wnd;
//   public:
//     CHTML_Mnemo(CHTML& _html)
//       : CHTMLWndBase( _html ), wnd(NULL) {}
//    ~CHTML_Mnemo();
//     virtual bool WindowPos( WND_POS );
//     virtual void Timer();
//   };
// //
// // Список имён групп
// class CHTML_CboName: public CHTMLComboBox
//   {
//   public:
//     CHTML_CboName(CHTML& _html)
//     : CHTMLComboBox( _html ){}
//     virtual bool WindowPos( WND_POS );
//     virtual LRESULT OnMessage( UINT uMsg, WPARAM wParam, LPARAM lParam );
//     virtual void Timer();
//   };
// //
// // Вид окна трендов
// class CHTML_CboView: public CHTMLComboBox
//   {
//   public:
//     CHTML_CboView(CHTML& _html)
//     : CHTMLComboBox( _html ){}
//     virtual bool WindowPos( WND_POS );
//     virtual LRESULT OnMessage( UINT uMsg, WPARAM wParam, LPARAM lParam );
//     virtual void Timer();
//   };
// //Простой editbox
// class CHTML_Edit : public CHTMLEditBox
//   {
//   protected:
//     class CTagSCRIPT* m_pScript;
//     char* m_pszValue;
//   public:
//     CHTML_Edit(CHTML& _html,CTagSCRIPT* pScript, char* pszValue)
//     : CHTMLEditBox( _html, NULL )
//     , m_pScript(pScript), m_pszValue(pszValue) {}
//     virtual bool WindowPos( WND_POS );
//     virtual LRESULT OnMessage( UINT uMsg, WPARAM wParam, LPARAM lParam );
//     virtual void Pressed();
//     virtual void Release();
//     virtual void Timer();
//     virtual int HWBTN(WORD nCode,UINT uKey);
//   };
// class CHTML_LoopTuneEdit : public CHTMLEditBox
//   {
//   protected:
//     SParamValueH* m_Val;
//   public:
//     CHTML_LoopTuneEdit(CHTML& _html,SParamValueH* mval)
//     : CHTMLEditBox( _html, NULL )
//     , m_Val(mval) {}
//     virtual bool WindowPos( WND_POS );
//     virtual LRESULT OnMessage( UINT uMsg, WPARAM wParam, LPARAM lParam );
//     virtual void Pressed();
//     virtual void Release();
//     virtual void Timer();
//     virtual int HWBTN(WORD nCode,UINT uKey);
//   };
// //
// class CHTML_OTHER : public CHTMLEditBox
//   {
//   protected:
//     SParamValueH* m_Val;
//     const char *m_pszFormat;
//     void SetText();
//   public:
//     CHTML_OTHER(CHTML& _html, SParamValueH* mVal, const char* pszFormat)
//     : CHTMLEditBox( _html, NULL )
//     , m_Val(mVal)
//     , m_pszFormat(pszFormat) {}
//     virtual bool WindowPos( WND_POS );
//     virtual LRESULT OnMessage( UINT uMsg, WPARAM wParam, LPARAM lParam );
//     virtual void Pressed();
//     virtual void Release();
//     virtual void Timer();
//     virtual int HWBTN(WORD nCode,UINT uKey);
//   };
