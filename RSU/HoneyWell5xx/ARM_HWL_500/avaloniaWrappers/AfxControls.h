#ifndef AFCCONTROLS_H
#define AFCCONTROLS_H
#include <string>
#include <Api.h>

#include <crosswindows.h>

namespace cross {
using namespace std;

enum BARS
{
    barPAGE,
    barTREND,
    barUnitTrend,
    barGROUP,
    barDETAIL,
    barACK,
    barINFO
};

// int  AfxMessageBox(std::string msg){
//    // return m_RenderCallbacks._ShowMessageBoxCallback(0, msg.c_str(),0x00000000L );
// }
// class CWnd{
//     //  HWND m_hWnd;



// public :
//     int MessageBox(std::string msg,   std::string caption, UINT type) {
//                                                                     {

//  //       return m_RenderCallbacks._ShowMessageBoxCallback(0, msg.c_str(), type );
//     }

//     int MessageBox(std::string msg) {
//  //       return m_RenderCallbacks._ShowMessageBoxCallback(0, msg.c_str(), 0x00000000L );
//             return 0;
//     }

//     void ButtonOk( enum BARS bar, std::string pszText,HWND hWnd  = NULL){

//         switch (bar)
//         {
//         case barPAGE:
//         {
//             // //bool N = TrueMnemoName( pszText );
//             // bool N = CHTML::IsTrueMemoname( pszText );
//             // if ( !N )
//             // {
//             //     if (lstrcmp(pszText,""))
//             //         AfxMessageBox("ERROR: WRONG PAGE NAME");
//             // }
//             // else
//             // {
//             //     if(theApp.bUseWDL())
//             //     {
//             //         std::string SafeviewCategory = CHTML::GetSafeviewCategory(pszText);
//             //         CAmmFrm* pafrm = theApp.GetCategotyFrm(SafeviewCategory);
//             //         if(pafrm)
//             //             pafrm->GoToPage(pszText, NULL);
//             //     }
//             //     else
//             //     {
//             //         if (!theApp.IsAllInOne() && !theApp.m_bFourMonitors)
//             //             GiveAmm(hWnd)->GoToPage(pszText,NULL);
//             //         else{
//             //             CAplicFrm *aplcFrm = GetAppWnd();
//             //             if ( aplcFrm->m_wndSens && hWnd == aplcFrm->m_wndSens.m_wndMnemo->m_hWnd )
//             //                 GetAppWnd()->m_wndSens.m_wndMnemo->BuildQueue(pszText, NULL);
//             //             else GiveAmm(hWnd)->GoToPage(pszText, NULL);
//             //         }
//             //     }
//             // }
//         }
//         break;
//         case barTREND:
//         {
//             // int N = atoi( pszText );
//             // if ( !(0 < N && N <= 3000) )
//             //     AfxMessageBox("ERROR: WRONG TREND NUMBER");
//             // else
//             //     GetAppWnd()->GiveRegularAmm(hWnd)->GoToPage("sysTnd03",pszText);
//         }
//         break;
//         case barGROUP:
//         {
//             // int N = TrueGroupNumber( pszText );
//             // if ( N == -1 )
//             //     AfxMessageBox("ERROR: WRONG GROUP NUMBER");
//             // else
//             // {
//             //     int n = ReadInt( "GroupView", 0 ) % 3;
//             //     _static char* file[] = { "sysGrpDetail", "sysGrpTrend", "sysGrpNumeric" };
//             //     GiveAmm(hWnd)->GoToPage(file[n],pszText);
//             // }
//         }
//         break;
//         case barUnitTrend:
//         {
//             // int N = TrueGroupNumber( pszText );
//             // if ( N == -1 )
//             //     AfxMessageBox("ERROR: WRONG GROUP NUMBER");
//             // else
//             // {
//             //     SaveInt( "GroupView", 1 );
//             //     int n = ReadInt( "GroupView", 0 ) % 3;
//             //     _static char* file[] = { "sysGrpDetail", "sysGrpTrend", "sysGrpNumeric" };
//             //     GiveAmm(hWnd)->GoToPage(file[n],pszText);
//             // }
//         }
//         break;
//         case barDETAIL:
//         {
//             // UINT nType;
//             // char szObj[_MAX_PATH]="";
//             // char szHoney[_MAX_PATH];
//             // LPCTSTR szTag = QueryFaceplate( pszText, szHoney, &nType );
//             // if ( szTag && ::IsAcyEx(nType) )
//             // {
//             //     if(strcmp(szTag, "SCANGRPDTL")==0)
//             //     {
//             //         CShCONTROLMODULE sh;
//             //         char *pp = NULL;
//             //         if(pp = strchr(szHoney, '.'))
//             //             *pp = 0;
//             //         if(mPipe.GetData( id_CONTROLMODULE, szHoney, &sh ))
//             //             strcpy_s(szObj, sh.SCANPNTDTL);
//             //         if(szObj[0]==0)
//             //         {
//             //             sprintf_s( szObj, "sysdtl%s", szTag );
//             //         }
//             //         if(pp)
//             //             *pp = '.';
//             //     }
//             //     else
//             //         if(strcmp(szTag, "GROUPDETAILDISPLAY")==0)
//             //         {
//             //             char *pp = NULL;
//             //             if(pp = strchr(szHoney, '.'))
//             //                 *pp = 0;

//             //             //CShSMANALOG sh;
//             //             //CShSMDISCRET sh;
//             //             switch(nType)
//             //             {
//             //             case id_SMFLD:
//             //             {
//             //                 CShSMFLD sh;
//             //                 if(mPipe.GetData( nType, szHoney, &sh ))
//             //                     strcpy_s(szObj, sh.POINTDETAILDISPLAY);
//             //                 if(szObj[0]==0)
//             //                 {
//             //                     sprintf_s( szObj, "sysdtl%s", szTag );
//             //                 }
//             //                 if(pp)
//             //                     *pp = '.';
//             //             }
//             //             break;
//             //             }
//             //         }
//             //         else
//             //             sprintf_s( szObj, "sysdtl%s", szTag );
//             //     GiveAmm(hWnd)->GoToPage( szObj, pszText );
//             // }
//             // else
//             //     AfxMessageBox("ERROR: WRONG TAG NAME");
//         }
//             // break;
//             // case barACK:
//             //     AckMnemo();
//             //     break;
//         }

//     }

//     CWnd*    GetParent() {

//         return this;
//     }



// };


// class CWinApp {
// public:
//     CWnd* m_pMainWnd;       // main window (usually same AfxGetApp()->m_pMainWnd)
//     CWnd* m_pActiveWnd;     // active main window (may not be m_pMainWnd)

// };



// }

#endif // AFCCONTROLS_H



