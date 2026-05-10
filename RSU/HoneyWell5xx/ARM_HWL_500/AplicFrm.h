// // AplicFrm.h : interface of the CAplicFrm class
// #pragma once

// #include <vector>
// #include <map>
// #include "SxemaFrm.h"
// #include "DialControl.h"
// #include "ChoiseBar.h"
// #include "DownBar.h"
// #include "MyStatusBar.h"
// #include "ControlFrm.h"
// #include "AmmFrm.h"
// #include "AmmFrmSTB.h"
// #include "SensorFrm.h"
// #include "MsgAL.h"
// #include "MsgSA.h"
// #include "MsgOG.h"
// #include "PopupMnemo.h"
// #include "TouchKbd.h"
// #include "MyToolBar.h"
// #include "SocketClient.h"
// #include "AlarmsForMnemo.h"
// #include "AmmFrmDubl.h"
// #include "MnemoStates.h"
// #include "../ButtonFrm.h"

// //
// #define  ID_MAIN_TIMER		1001
// #define _MAX_WND  50
// #define _MAX_FACE  4
// //
// class CAplicFrm : public CButtonFrm
//   {
//   // DECLARE_DYNAMIC(CAplicFrm)
//   // HWND m_hActiveMnemo;
//   //
//   bool   m_bNoPCYWork;// Отказ РСУ
//   // CTouchKbd m_wndOper;// Клавиатура в операторной
//   // CTouchKbd m_wndPole;// Клавиатура в поле
//   // CSensorFrm m_wndSens;
//   //
//   HWND  m_hWndHW;// Окно клавиатуры Honeywell
//   HWND  m_hWndMN;// Окно перехода по мнемосхемам
//   //
//   // CAmmFrm m_wndOne;
//   // CAmmFrm* m_wndDubl;
//   // CAmmFrmSTB m_wndThird;
//   // CAmmFrmSTB m_wndFourth;
//   // CAmmFrm m_wndTop;
//   // CAmmFrm m_wndLeft1;
//   // CAmmFrm m_wndLeft2;
//   // CAmmFrm m_wndLeft3;
//   // CAmmFrm m_wndLeft4;
//   // CMyToolBar m_wndTool;

//   //SUniUndo m_GlobalUndo;
// typedef struct
// {
//     std::string name;
//     std::string param;
//     int nPosition;
// }
//   UndoRedoRec;
//   std::vector<UndoRedoRec> m_UndoList;
//   std::vector<UndoRedoRec> m_RedoList;

//   int m_nOneDublThirdFourthCounter;  //номер очередного окна из четырёх 0-3
//   int m_nLeftWindowsCounter;         //номер очередного окна из четырёх 0-3

//   static HWND& GetTouchHWND(CAplicFrm * ap)
//   {
// 	  return Version == LG35_8_KF ? ap->m_wndSens.m_hWnd : ap->m_wndOper.m_hWnd;
//   }
//   //
//   CReBar m_wndTopReBar;
//   // Для режима 0
//   CControlFrm* m_pFaceplate;
//   // Для режима 1 & 2
//   CControlFrm* m_arrFace[_MAX_FACE];
//   CControlFrm* m_arrFace4x4[4][4];
//   CPopupMnemo* m_arrWnd [_MAX_WND ];
//   HWND m_hLastActiveMnemoForPopups;
//   //
//   CMyStatusBar  m_wndStatusBar;
//   CDownBar      m_wndDownBar;
//   int m_nActiveFaceplate;
//   int m_nActiveFaceplates[4];
//   bool m_bUpdate;

//     CButtonFrm* m_pActiveButtonFrm;
//   private:
// 	    CChoiseBar m_wndChoiseBar;
// 		//логика для запоминания точки и нажатия Ack и Trend для конкретного тэга
// 		//KillFocus приходит и при нажатии на Ack и Trend
// 		//поэтому приходится очищать через 3 секунды
// 		KHoneyPipeClient* m_ptr_pipeForInfoBar;
// 		int m_nNumberForInfoBar;
// 		bool m_bMustClearPipeAndNumberForKillFocus;
// 		DWORD m_dTimeOfKillFocus;
//         std::map<CString, int>  m_MnemosAlarm;   //имена менмосхем для которых подсчитывалюся алармы и их номер в pipe
//   public:
//     CAplicFrm();
//     virtual ~CAplicFrm();

//   // Attributes
//   public:
//     //
//     CMsgAL m_msgAL;
//     CMsgSA m_msgSA;
//     CMsgOG m_msgOG;
//     //
//     CButtonFrm* GetActiveButtonFrm()
//       {
//       return m_pActiveButtonFrm;
//       }
// 	bool IsGlobalFocus(){
// 		return !m_wndOne.m_bGalkaOn || !m_wndDubl->m_bGalkaOn || !m_wndThird.m_bGalkaOn || !m_wndFourth.m_bGalkaOn;
// 	}
//     void SetActiveButtonFrm(CButtonFrm* pFrm)
//       {
//       if ( pFrm != this )
//       m_pActiveButtonFrm = pFrm;
//       }
//     //
// 	void HideNotPinPopups(CButtonFrm * parent);
//     void DelMnemo( CPopupMnemo* wnd );
//     void AddMnemo( CPopupMnemo* wnd );
//     void ShowMnemos(int nShow);
//     bool FndMnemo( const char* pszName, const char* pszPointName=NULL );
//     CPopupMnemo* FindMnemoByTitle( LPCSTR Title );
//     bool FndMnemoAndName( const char* pszMnemoName, const char* pszHeader);
//     void ToolbarR( const char* pszComm );
// 	void SetLastActiveMnemoForPopups(HWND hWnd);
//     //
//     HWND GetActiveMnemo(){ return m_hActiveMnemo;}
//     void SetActiveMnemo(HWND v){ m_hActiveMnemo = v;}
//     void SetUpdate(bool b)
//       {
//       m_bUpdate=b;
//       m_wndStatusBar.SetGUS();
//       }
//     //
//     std::string GetHoneywellStartNameCSVbyCAmmFrm(CAmmFrm *pParentAmmFrm);
//   // Overrides
//   public:
//     virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
//     virtual void PostNcDestroy();
//     virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
//     virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
//     virtual BOOL DestroyWindow();
//     void ShowBar( BARS bar );
// 	void ShowInfoBar(BARS barType,CString strInfo);
// 	void HideInfoBar(bool bFromKillFocus=false);
//     void SetFocusInpunZone(){m_wndTool.SetFocusInpunZone();}

//     void ZoomMnemo(int nChoise);
//     CAmmFrm*    ActiveFrm();
//   	CAmmFrm* GetFirstFrame(){return &m_wndOne;};
//     CControlFrm** FaceplateArr(){ return m_arrFace;};
//     CControlFrm* (*FaceplateArr4x4())[4] { return m_arrFace4x4;};
//     void OnMenuR300(UINT nCmdID);
// 	CChoiseBar* GetChoiseBar(){ return &m_wndChoiseBar;};

//     void HoneywellKBD(WORD nCode,LPARAM lParam);
//     void PoleOper(bool bOper);
//     bool IsShowSxeme();
//     void Group (LPCTSTR tag);
//     void Trend (LPCTSTR tag);
//     void Detail(LPCTSTR tag);
//     void Assocs(LPCTSTR tag);
//   protected:
//     void ChangeFast();
//     void ChangeAlphaShift();
//     void GotoScheme();
//     void GotoPole(int nCorrelate);
//     void GotoOper(int nCorrelate);
//     void SwitchWnd(int nCorrelate);
//     void AckMnemo();
//     void IntellectGotoPage( const char* pszName );
//     void IntellectForsunka( WPARAM wParam,LPARAM lParam );
//     // Generated message map functions
//     LRESULT OnPressQueue(WPARAM wParam,LPARAM lParam);
//   public:
//     KHoneyPipeClient mPipe;
//     CSocketClient mSocket;
//     CAmmFrm* GiveAmm(HWND hWnd = NULL, bool exceptOne = false);
//     CAmmFrm* GiveRegularAmm(HWND hWnd);
//     CAmmFrm* CAplicFrm::GivePrevRegularAmm();
//     CAmmFrm* GiveRegularLeftAmm(HWND hWnd);
//     CAmmFrm* GiveAmmByhWnd(HWND hWnd);
//     CAmmFrm* GiveAmmByPosition(int nPosition);
//     void SetOnliOneAmm(CAmmFrm *ammFrm);
//     CAmmFrm *GetSelectedLeftFrm();
//     CAmmFrm *GetSelectedMnemoFrm();
//     void ButtonOk( enum BARS bar, const char* pszText, HWND hWnd = NULL );
//     void ChoiseFaceplate( const char* name, CAmmFrm* ammFrm = NULL, const char * fp = 0, const char* pv = 0, const char * rst = 0, const char * desc = 0 );
//     void SetInfoToChoiseBar(CString strName,CString strValue);
//     void SetInfoToDownBar(CString strInfo,int nNumber);
//     void MyClientRect( RECT& rect );
// 	void SetPipeAndNumberForInfoBar(KHoneyPipeClient* prt,int nNumber);
//     void GlobalUndo();
//     void GlobalRedo();
//     void GlobalUndoListPushBack(UndoRedoRec &rec);
// 	SAlarmsOnMnemo GetAlarmsForMnemo(CString strMnemoName, bool bFromSensor=false);
//     static bool IsOpenFaceplateByScTxtSelectBox() {return Version == LG35_8_KF || Version == AM2_NAK || Version == BERTH_ONPZ || Version == ITH_NAK || Version == DMW_NAK;}
// 	CMnemoStates *m_pMnemoStates;
//     BYTE GetLampState(BYTE nArrayNumber);
//     // Generated message map functions
// 	  DECLARE_MESSAGE_MAP()
//   protected:
//     afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
//     afx_msg void OnDestroy();
//     afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
//     afx_msg void OnTimer(UINT_PTR nIDEvent);
//     afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
//     afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT item);
//     afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT item);
//     // DialControl Messages
//     afx_msg  BOOL OnNcActivate(BOOL bActive);
//     afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
//     afx_msg void OnSwitchStd();
//     afx_msg void OnSwitchFrm();
//     afx_msg void OnSwitchDab();
//     afx_msg void OnCallKlavaBot();
//     afx_msg void OnCallKlawaDop();
//     afx_msg void OnHideKlawaDop();
//     afx_msg void OnKvitAll();
//     afx_msg void OnUpdateWorker(CCmdUI *ui);
//   private:
//     void OnButtonOk();
//     void OnButtonCancel();
//     void OnCommandChange();
//   private:
//     CDialControl m_wndPanel;
// 	  UINT_PTR  m_nTimer;
// 	std::map<std::string,int> mapPointInTrendsGroup;
//     afx_msg void OnActivateApp(BOOL bActive, DWORD dwThreadID);
//     afx_msg void OnPaint();
//     afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
//     bool CallGroup();
//     bool CallTrendDetail(BARS bar);
// 	bool IsGoodPointFromInfo();
// 	bool CallTrendGroupFromPoint();
// };
