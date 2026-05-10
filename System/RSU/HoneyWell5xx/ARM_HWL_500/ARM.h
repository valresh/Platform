// APM.h : main header file for the APM application

#pragma once
#ifndef ARM_H
#define ARM_H

// #ifndef __AFXWIN_H__
// 	#error include 'stdafx.h' before including this file for PCH
// #endif

//#include "resource.h"       // main symbols

//#include "HoneyMnemoExtend.h"
//#include "Function.h"
//#include "InlineGDI.h"
//#include "KeybCOM.h"
//#include "MenuSTB.h"
//#include "BtnHWMS.h"
//#include "Profile.h"
//#include "BaseType_.h"
//#include <ShData.h>
//#include <CommonRSUs.h>
//#include "DlgFromCSV.h"

#include "XmlHelperPanoram.h"

//#include "TexnoWnd/TexnoCSV.h"
#include <UtilsH.h>
//#include <SocketCLient.h>
//#include "SensorsCSV.h"
//#include "ARMSharedMemory.h"
#include <vector>
#include <string>
#include <map>
#include <linuxTypes/cstring.h>


#include <BaseType.h>
#include <FormatScn.h>
#include <FileXML.h>

//#include <FileXML.h>
//#include "HTML/Tag.h"



//custom includes - not imported
#include <avaloniaWrappers/AfxControls.h>

class CStyle;
class CTag;

struct SFontArray
{
    LOGFONT lf;
    HFONT font;
};
//
enum EAliasType :  char
{
    aliasUnk = 0,
    aliasPoint,
    aliasPar,
    aliasText,
    aliasNum,
};
//
struct SParamsData
{
    int nName;
    int nValue;
    //EAliasType type;
};
//
struct _WINDOW
{
    RECT rect;
    std::string name;
    bool bClosable;
    bool bSizeable;
    bool bMainWindow;
    //     CAmmFrm* pAmmFrm;
    std::vector<std::string> SafeviewCategories;
    //   _WINDOW() {memset(&rect, 0, sizeof rect); bClosable= bSizeable= bMainWindow= false; pAmmFrm = NULL;}
};
enum GROUPTYPE
{
    gtUnknown = 0,
    gtFirstMatch,
    gtRoundRobin,
};
struct _GROUP
{
    GROUPTYPE gType;
    std::string name;
    int nWnd;  //теукщий индекс window в группе
    std::vector<std::string> SafeviewCategories;
    std::vector<_GROUP> groups;
    std::vector<_WINDOW> windows;
    _GROUP() {gType = gtUnknown; nWnd = 0;}
};
struct _WVAL
{
    EValueType eType;
    union
    {
        int    ii;
        double dd;
        bool   bb;
    };
    _WVAL() {memset(this, 0, sizeof(_WVAL));}
};
struct _WDL
{
    int xresolution;
    int yresolution;
    RECT WDLrect;
    std::map<std::string, _WVAL> parameterv;
    std::vector<_GROUP> groups;
    std::vector<_WINDOW*> staticWindows;
    std::vector<_WINDOW*> popupWindows;
    _WDL() {xresolution = yresolution = 0; WDLrect.left=WDLrect.top=999999; WDLrect.right=WDLrect.bottom=-999999;};
};
//
#define MAX_IMAGES_NUMBER 7

typedef int(__cdecl* StartUdpServer)();
typedef int(__cdecl* SendUdpName)(const char*);
typedef void(__cdecl* StopUdpServer)();
//
// CAPMApp:
class CAPMApp  //: public cross::CWinApp//,
    //CArray<SFontArray>, public CMenuSTB
{
public:
    CAPMApp();
    ~CAPMApp();
    // virtual BOOL InitInstance();
    // virtual int  ExitInstance();

    ///   inline operator HINSTANCE () { return m_hInstance; }
    //    bool Instructor  () {return m_cmd.bInstructor;}
    // bool DisableKirishiView() {return m_cmd.bDisableKirishiView;}
    // bool DebugKirishiMonitorInfo() {return m_cmd.bDebugKirishiMonitorsInfo;}
    bool IsAllInOne();
    // bool bUseWDL();
    // HACCEL AccelTable() {return m_hAccel;}
    // HMENU  MenuWindow() {return m_hMenu;}
    ///   void SetIcon( CWnd* pWnd );
    //
    // HFONT    GiveFont(LOGFONT& lf);
    // LOGFONT& GiveLogF(HFONT hFont);
    // //
    STegScn* ParsXML( char*& ptr, int& nCount );
    // STegScn* ParsEXT( char*& ptr, int& nCount );
    // STegScn* ParsSCN( char*& ptr, int& nCount );
    //
    char  m_szTempMessZoneText[512];
    int   m_nLevelSec;
    //
    // afx_msg void OnAppAbout();
    // afx_msg void OnTuning();
public:
    bool m_bDND;
    CTag* m_pTag;
    void DND(CTag* tag){
        m_bDND = true;
        m_pTag = tag;
    }
    //   bool GetFaceToolbar(){return true;}//MIHAIL_HWL m_cmd.bFaceToolbar;};
    //   int  GetKlawaNumber(){return 1;}//MIHAIL_HWL m_cmd.nKlawaHoney;};
    //    void DrawAlarmIcon( HDC hDC, POINT pt, SIZE* sz, BYTE btEHLN, BYTE btKvit, BYTE btOnOf, int nShelved );
    //    void ViewRedMessage(CString strText,CString strDate);
    //  void SetupIniFile();
    //    bool IsModelDataType(UINT type);
    // bool IsSpecificModelDataType(UINT type);
public:
    //  int GetDialogNumber(CString strKlapanName, int nDialog);
    //  CString GetPultName(CString strName);
    //  bool IsSensorNameInFile(CString strName);
    //  std::string GetTexnoshemeNameCSV();
    //  std::string GetHoneywellStartNameCSV();
    //  std::string GetHoneywellMenuNameCSV();
    //  std::string GetHoneywellInfoName();
    //  std::string GetTexnoValueByKey(std::string key);
    //  int GetStandartDialogNumberFromCSVFile(EDataTypes eData, UINT eScheme, std::string strName);
    //  std::string GetElectroZadvDialogCSV();
    //  bool IsDefaultKlapanDialog(std::string strDialogName);
    //  CString FindPointInArray(UINT nType, CString strName);
    //  CString GetPointNameByType(UINT eData);
    bool FileExists(std::string strFileName);
    //  void StartPanoram3D(CString strPoint="");
    //  CString GetServerParamsForPanoram();
    //  CString GetPointParamsForPanoram(CString strPoint);
    //  int GetWindow1FromCSV();
    //  int GetWindow4FromCSV();
    //  std::string GetSensorWndRectString();
    bool IsNewPanoram() {return m_bNewPanoram;};
    //  void SetPanoramNameInSharedMemory(std::string write);
    //  void StartNewPanoram3D();
    bool m_bFourMonitors;
    //  std::string GetSpecialPanoramsDir();
    //  std::string GetExePanoramPath();
    //  bool GetOlderSensorKbd(){return m_bOlderSensorKbd;};
    //  void SetOlderSensorKbd(bool b){m_bOlderSensorKbd=b;};
    //
    //  CAmmFrm* CAPMApp::GetCategotyFrm(const std::string& SafeviewCategory);
    //
    //   DECLARE_MESSAGE_MAP()
protected:
    //   afx_msg void OnUpdateTuning(CCmdUI *pCmdUI);
private:
    //   SCmdLine m_cmd;
    // HICON  m_hIcon;
    // HMENU  m_hMenu;
    // HACCEL m_hAccel;
    bool m_bOnceAlarmIconMessageBox;
    //
    STegScn* m_scnXML;int m_nCountXML;
    STegScn* m_scnEXT;int m_nCountEXT;
    STegScn* m_scnSCN;int m_nCountSCN;

    //CDlgFromCSV dlgFromCSV;
    // CSensorsCSV sensorsFromCSV;
    // CTexnoCSV texnoCSV;
    // CXMLOperations xml;
    cross::CString strPointsFromXMLArray[MAX_POINTS_IN_ARRAY][3];
    int m_nPointsFromXMLArrayCount;
    //  ARMSharedMemory* sharedMemoryForPanoram;
    bool m_bNewPanoram;
    HINSTANCE udplib;
    StartUdpServer fnStartUdpServer;
    SendUdpName fnSendUdpName;
    StopUdpServer fnStopUdpServer;
    //
    void LoadWDL();
    //  CAmmFrm* CAPMApp::GetCategotyFrm(_GROUP& grp, const std::string& SafeviewCategory);
    //
    HBITMAP bmpAlarm[MAX_IMAGES_NUMBER];
public:
    bool m_bPanoramaInTop;
    //  CSocketClient mSocketTexno;
    static bool m_bPole;
    int m_nFaceplatsPerScreen; //число фейсплейтов на одном экране
    bool m_bOlderSensorKbd;
    _WDL  m_wdl;
    //
    CFileXML m_DictionaryXML;   //для скриптов
};

// The one and only CAPMApp object
extern CAPMApp theApp;
extern unsigned long dwMainTimer;
extern unsigned long dwTypeFrame;
extern const  double mWindSize;
//
inline bool PulsTimer()
  {
  return (dwMainTimer % 2) == 1;
  }
//
// inline bool MorgTimer(bool bNoAck)
//   {
//   return ( bNoAck && PulsTimer() ) || !bNoAck;
//   }
// Объявление функций
// inline class CAplicFrm* GetAppWnd()
//   {
//   return (CAplicFrm*)theApp.m_pMainWnd;
//   }
//replace base types:
// inline class cross::CWnd* GetAppWnd()
//   {
//   return theApp.m_pMainWnd;
//   }


class KHoneyPipeClient& Client();
//
#define R300 _T("R410")
//
int Wnd1(bool** b = NULL);
int Wnd2(bool** b = NULL);
int Wnd3(bool** b = NULL);
int Wnd4(bool** b = NULL);
int Wnd5(bool** b = NULL);
RECT RectWnd(int nNumber,bool** b = NULL,int nPosition = -1);
void Monitors();
//
bool IsNaN(const char* ptr,double& val);
//
const char* MnemoToSxema( const char* name );
const char* DispPrev(const char* name);
const char* DispNext(const char* name);
const char* Empty();
inline void AAA(){};

//
bool FloatMenu( LPMEASUREITEMSTRUCT item, HWND hWnd );
bool FloatMenu( LPDRAWITEMSTRUCT    item );
void ContextMenu( HWND hWnd, bool bAlarm );
void ScaleMenu( HWND hWnd, int x, int y );
//
int SetLapa ();
int SetGoto ();
int SetArrow();
int SetHand ();
//
#define FindLocal( Text )  (SLocal*)SearchName( Text, list, nCount, sizeof(list[0]) )
#define InitLocal ;\
static int nCount = sizeof(list)/sizeof(list[0]);\
    static bool bInit = true;\
    Qsort( list, nCount, sizeof(list[0]), bInit )
//
#endif
