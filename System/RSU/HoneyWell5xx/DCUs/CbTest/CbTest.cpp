#include <stdafx.h>
#include <CommProc.h>
#include <basemodel.h>
#include <Names.h>
#include <Connect.h>
#include <RsuX.h>

#include <rsuPaths.h>
#include <rsuModel.h>
#include <dylib.hpp>

static tShowACSObject s_localShowACSs[20] = { NULL };

void fnRegisterFinderACSobj(tGetFirstACSObject pGetF, tGetNextACSObject pGetN, tShowACSObject pShowACS )
{
  for( int i=0; i<_countof(s_localShowACSs); ++i )
  {
    if( s_localShowACSs[i] )
      continue;
    s_localShowACSs[i] = pShowACS;
    break;
  }
}

#define RD_VAR(Var)  if ( !file.RF( &Var, sizeof( Var )) ) return -1;
#define RD_STR(Str)  if ( !file.RS( Str               ) ) return -1;

BOOL IsIdleMessage(MSG* pMsg)
{
  if (pMsg->message == WM_MOUSEMOVE || pMsg->message == WM_NCMOUSEMOVE)
  {
    return TRUE;
  }
  return pMsg->message != WM_PAINT && pMsg->message != 0x0118;
}

BOOL PumpMessage(MSG* pMsg)
{
  if (!::GetMessage(pMsg, NULL, NULL, NULL))
  {
    return FALSE;
  }
  if (pMsg->message != 0x036A/*WM_KICKIDLE*/ )
  {
    ::TranslateMessage(pMsg);
    ::DispatchMessage(pMsg);
  }
  return TRUE;
}


IRsuModel* LoadServer( LPCSTR DllName, LPCSTR pszObjName )
{
  dylib* lib;
  try
  {
      lib = new dylib(rsuGetExecutionPath(), DllName, true);
  }
  catch (...)
  {
      lib = NULL;
  }
  //_makepath_s( szPath, NULL, rsuGetExecutionPath(), DllName, "dll" );
  //HMODULE hLib = (HMODULE)LoadLibrary(szPath);
  if( NULL==lib )
    return NULL;
  tCreateServer pCreateServer = (tCreateServer)lib->get_symbol( "CreateServer" );
  if( !pCreateServer )
    return NULL;
  IRsuModel *pServer = pCreateServer( pszObjName );
  return pServer;
}

#pragma comment(linker, \
  "\"/manifestdependency:type='Win32' "\
  "name='Microsoft.Windows.Common-Controls' "\
  "version='6.0.0.0' "\
  "processorArchitecture='*' "\
  "publicKeyToken='6595b64144ccf1df' "\
  "language='*'\"")
#include "resource.h"
#include <CommCtrl.h>
#pragma comment(lib, "ComCtl32.lib")

static bool s_bContinuePouse = false;
static bool s_bLoadState = true;
static bool s_bSaveState = false;
static bool s_bSingleStep = false;

INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch(uMsg)
  {
  case WM_COMMAND:
    switch(LOWORD(wParam))
    {
    case IDCANCEL:
      SendMessage(hDlg, WM_CLOSE, 0, 0);
      return TRUE;
    case IDC_POUSE_CONTINUE:
      {
        HWND hBtn = GetDlgItem( hDlg, IDC_POUSE_CONTINUE );
        s_bContinuePouse = !s_bContinuePouse;
        SendMessage( hBtn, WM_SETTEXT, NULL, s_bContinuePouse ? (LPARAM)"В паузу" : (LPARAM)"Продолжить" );
      }
      return TRUE;
    case IDC_SAVE:
      s_bSaveState = true;
      return TRUE;
    case IDC_LOAD:
      s_bLoadState = true;
      return TRUE;
    case IDC_SINGLESTEP:
      s_bContinuePouse = true;
      SendMessage( hDlg, WM_COMMAND, MAKELONG(IDC_POUSE_CONTINUE,0), 0 );
      s_bSingleStep = true;
      return TRUE;
    }
    break;

  case WM_CLOSE:
    if(MessageBox(hDlg, TEXT("HoneyWell5xx перестанет шагать"), TEXT("Завершить CbTest"), MB_ICONQUESTION | MB_YESNO) == IDYES)
    {
      DestroyWindow(hDlg);
    }
    return TRUE;

  case WM_DESTROY:
    PostQuitMessage(0);
    return TRUE;
  }

  return FALSE;
}

int ServerTest( HWND hDlg,
                HINSTANCE hInstance,
                HINSTANCE hPrevInstance,
                LPTSTR    lpCmdLine,
                int       nCmdShow)
{
  if( !s_bContinuePouse )
  {
    s_bContinuePouse = true;
    SendMessage( hDlg, WM_COMMAND, MAKELONG(IDC_POUSE_CONTINUE,0), 0 );
  }

  char szPath2State[_MAX_PATH] = { 0 }, szPath2Params[_MAX_PATH] = { 0 };

  _makepath_s( szPath2State, NULL, GetSavePath(), "testingState", "state");
  _makepath_s( szPath2Params, NULL, GetParamsPath(), "testingParams", "params");

  IRsuModel *models[] =
  {
    LoadServer("ControlBuilderH5xx_Server", "VSB"),
    //LoadServer("ControlBuilderH5xx_Server", "AMMIAK-2"),
    //LoadServer("ControlBuilderH5xx_Server", "Export"),
    //LoadServer("ControlBuilderH5xx_Server", "PIER"),
    
    LoadServer("AlarmsHW5xx_Server","AlarmsHW"),
    LoadServer("QuickBuilderH5xx_Server","РСУ_QB_QBDB"),

    LoadServer("AlarmsQBbridge2HW5xx_Server","AlarmsQB"),

    LoadServer("SafetyManagerH5xx_Server", "РСУ_SMH_ESD_001_1"),
    LoadServer("SafetyManagerH5xx_Server", "РСУ_SMH_ESD_001_2"),
    LoadServer("SafetyManagerH5xx_Server", "РСУ_SMH_ESD_001_3"),
    LoadServer("SafetyManagerH5xx_Server", "РСУ_SMH_ESD_001_4"),
    LoadServer("SafetyManagerH5xx_Server", "РСУ_SMH_ESD_001_5"),
    LoadServer("SafetyManagerH5xx_Server", "РСУ_SMH_ESD_001_6"),
    LoadServer("SafetyManagerH5xx_Server", "РСУ_SMH_ESD_001_7"),
    LoadServer("SafetyManagerH5xx_Server", "РСУ_SMH_ESD_001_8"),
    LoadServer("SafetyManagerH5xx_Server", "РСУ_SMH_ESD_002_1"),
    LoadServer("SafetyManagerH5xx_Server", "РСУ_SMH_ESD_101_1"),
    LoadServer("SafetyManagerH5xx_Server", "РСУ_SMH_ESD_101_2"),
    LoadServer("SafetyManagerH5xx_Server", "РСУ_SMH_ESD_301_1"),
    LoadServer("SafetyManagerH5xx_Server", "РСУ_SMH_ESD_411_1"),
    LoadServer("SafetyManagerH5xx_Server", "РСУ_SMH_ESD_411_2"),
    LoadServer("SafetyManagerH5xx_Server", "РСУ_SMH_ESD_431_1"),
    LoadServer("SafetyManagerH5xx_Server", "РСУ_SMH_ESD_511_1"),
    LoadServer("SafetyManagerH5xx_Server", "РСУ_SMH_ESD_551_1"),
    LoadServer("SafetyManagerH5xx_Server", "РСУ_SMH_ESD_861_1"),
    LoadServer("SafetyManagerH5xx_Server", "РСУ_SMH_ESD_911_1"),
    LoadServer("SafetyManagerH5xx_Server", "РСУ_SMH_ESD_912_1"),
    LoadServer("SafetyManagerH5xx_Server", "РСУ_SMH_ESD_931_1"),
    LoadServer("SafetyManagerH5xx_Server", "РСУ_SMH_ESD_932_1"),
    LoadServer("SafetyManagerH5xx_Server", "РСУ_SMH_FGS_001_1"),
    LoadServer("SafetyManagerH5xx_Server", "РСУ_SMH_FGS_001_2"),
    LoadServer("SafetyManagerH5xx_Server", "РСУ_SMH_FGS_001_3"),
    LoadServer("SafetyManagerH5xx_Server", "РСУ_SMH_FGS_001_4"),
    LoadServer("SafetyManagerH5xx_Server", "РСУ_SMH_FGS_002_1"),
    LoadServer("SafetyManagerH5xx_Server", "РСУ_SMH_FGS_101_1"),
    LoadServer("SafetyManagerH5xx_Server", "РСУ_SMH_FGS_301_1"),
    LoadServer("SafetyManagerH5xx_Server", "РСУ_SMH_FGS_411_1"),
    LoadServer("SafetyManagerH5xx_Server", "РСУ_SMH_FGS_431_1"),
    LoadServer("SafetyManagerH5xx_Server", "РСУ_SMH_FGS_511_1"),
    LoadServer("SafetyManagerH5xx_Server", "РСУ_SMH_FGS_551_1"),
    LoadServer("SafetyManagerH5xx_Server", "РСУ_SMH_FGS_861_1"),
    LoadServer("SafetyManagerH5xx_Server", "РСУ_SMH_FGS_911_1"),
    LoadServer("SafetyManagerH5xx_Server", "РСУ_SMH_FGS_912_1"),
    LoadServer("SafetyManagerH5xx_Server", "РСУ_SMH_FGS_931_1"),
    LoadServer("SafetyManagerH5xx_Server", "РСУ_SMH_FGS_932_1"),


    //LoadServer("HCD_Server","РСУ_HCD_PAZ1"),
    //LoadServer("HCD_Server","РСУ_HCD_PAZ2"),
    //LoadServer("HCD_Server","РСУ_HCD_PAZ3"),
    //LoadServer("HCD_Server","РСУ_HCD_PAZ4"),
    //LoadServer("HCD_Server","РСУ_HCD_PAZ5"),
    //LoadServer("HCD_Server","РСУ_HCD_PAZ6"),
    //LoadServer("HCD_Server","РСУ_HCD_RSU1"),
    //LoadServer("HCD_Server","РСУ_HCD_RSU2"),
    //LoadServer("HCD_Server","РСУ_HCD_RSU3"),
    //LoadServer("HCD_Server","РСУ_HCD_RSU4"),
    //LoadServer("HCD_Server","РСУ_HCD_RSU5"),
    //LoadServer("HCD_Server","РСУ_HCD_SGK_DCS"),
    //LoadServer("RsL5000_Server","Rs5000_253_SGK_ESD"),
  };

  for( int i=0; i<_countof(models); ++i )
  {
    if( !models[i] )
      continue;
    models[i]->Initialize();
  }
  for( int i=0; i<_countof(models); ++i )
  {
    if( !models[i] )
      continue;
    models[i]->AfterInit();
  }
  for( int i=0; i<_countof(models); ++i )
  {
    if( !models[i] )
      continue;
    if( 1!=_countof(models) )
      break;
    if( !s_bLoadState )
      break;
    s_bLoadState = false;
    //models[i]->StateRestore(szPath2State);
    //models[i]->ParamsRestore(szPath2Params);
  }
  for( int i=0; i<_countof(models); ++i )
  {
    if( !models[i] )
      continue;
    models[i]->AfterRestored();
  }
  for( int n=0; n<1; n++)
  {
    for( int i=0; i<_countof(models); ++i )
    {
      if( !models[i] )
        continue;
      models[i]->CalcStep( 360 );
    }
  }
  for( int i=0; i<_countof(models); ++i )
  {
    if( !models[i] )
      continue;
    if( 1!=_countof(models) )
      break;
    //models[i]->StateSave(szPath2State);
    //models[i]->ParamsSave(szPath2Params);
  }
  //models[0]->ShowObject("PIER");
  for( int i=0; i<_countof(models); ++i )
  {
    if( !models[i] )
      continue;
    //models[i]->ShowObject("РСУ_SMH_ESD_001_1");
    //models[i]->ShowObject("FY3703");
    //models[i]->ShowObject("GRAVELFILTER1");
    //models[i]->ShowObject("RSU1");
    //models[i]->ShowObject("AMMIAK-2");
  }

  MSG msg;
  BOOL bIdle = TRUE;
  LONG lIdleCount = 0;

  LONGLONG llEnd   = 0;
  LONGLONG llStart = 0;
  LONGLONG llFreq  = 0;
  QueryPerformanceFrequency((LARGE_INTEGER*)&llFreq);
  QueryPerformanceCounter((LARGE_INTEGER*)&llStart);

  const double deltaStepSec = 0.36;
  // acquire and dispatch messages until a WM_QUIT message is received.
  for (;;)
  {
    // phase1: check to see if we can do idle work
    while (bIdle && !::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE))
    {
      QueryPerformanceCounter((LARGE_INTEGER*)&llEnd);

      double delay = (((double)(llEnd - llStart))/((double)llFreq));
      // call OnIdle while in bIdle state
      lIdleCount++;
      if( delay > deltaStepSec )
      {
        QueryPerformanceCounter((LARGE_INTEGER*)&llStart);
        if( s_bLoadState )
        {
          s_bLoadState = false;
          if( 1==_countof(models) )
          {
            models[0]->AfterInit();
            models[0]->StateRestore(szPath2State);
            models[0]->AfterRestored();
          }
        }
        if( s_bSaveState )
        {
          s_bSaveState = false;
          if( 1==_countof(models) )
            models[0]->StateSave(szPath2State);
        }
        if( !s_bSingleStep && !s_bContinuePouse )
          continue;
        if( s_bSingleStep )
          s_bSingleStep = false;
        for( int i=0; i<_countof(models); ++i )
        {
          if( !models[i] )
            continue;
          models[i]->CalcStep( deltaStepSec * 1000. );
        }
      }
    }

    // phase2: pump messages while available
    do
    {
      // pump message, but quit on WM_QUIT
      if (!PumpMessage(&msg))
      {
        return (int) msg.wParam;
        break;
      }

      // reset "no idle" state after pumping "normal" message
      //if (IsIdleMessage(&m_msgCur))
      if (IsIdleMessage(&msg) )
      {
        bIdle = TRUE;
        lIdleCount = 0;
      }

    } while (::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE));
  }
  return (int) msg.wParam;
}

#include "../ScriptoH/Trans.h"

#include <BaseType_.h>
#include <SmBaseType_.h>
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
  //ASSD( sizeof(CAlarmBase)==sizeof(CSmBase) );
  KAaTranslator trans( NULL );
  std::stringstream ss;
  //ss << "+ FI0800.AUXCALCA.CONST[4]*TI0800.DACA.PV*PI0800.DACA.PV + FI0800.AUXCALCA.CONST[5]*TI0800.DACA.PV*TI0800.DACA.PV + FI0800.AUXCALCA.CONST[6]*PI0800.DACA.PV*PI0800.DACA.PV";
  //ss << "MIN(MAX((TEST_FQ4307.T.P[1]+TEST_FQ4307.T.CONST[1])/TEST_FQ4307.T.CONST[2], -2.9), 1010)";
  //ss << "(PRSA10.DACA.PV-PRSA11.DACA.PV)-0.075>=0?(PRSA10.DACA.PV-PRSA11.DACA.PV)-0.075 :0";
  //ss << "638.476380646502 - 1.33775901252175 * FY603.KorPV2.P[2] -3.92350802580489E-03 * (FY603.KorPV2.P[2]^2) + 2.33243654701352E-02 * FY603.KorPV2.P[3] + 1.45634963261786E-03 * (FY603.KorPV2.P[3]^2)";
  //ss << "NOT PIRCA502.DACA.BADPVALM.FL AND PIRCA502.DACA.PVLLALM.FL? 1 : 0";
  //ss << "TIASH523.DACA.BADPVALM.FL AND TIA501_5.DACA.BADPVALM.FL ? NAN : INT((TIASH523.STOP.C[1]+TIASH523.STOP.C[2])/2.0+1.0)";
  //ss << "(CAEXANEX_T2_CM.MOTOR_IS_PROG.PVFL[1] AND NOT(CAEXANEX_T2_CM.MOTOR_IS_RUN.PVFL[1]) AND NOT(CAEXANEX_T2_CM.MOTOR_GOT_CMD.PVFL[1]))?CAEXANEX_T2_CM.TOTAL_TIME.PV[1]:+INF";
  //ss << "(P_1001_02_03_04.MOTOR_IS_PROG.PVFL[P_1001_02_03_04.BASE_SEQUENCE.C[1]] AND P_1001_02_03_04.MOTOR_IS_RUN.PVFL[P_1001_02_03_04.BASE_SEQUENCE.C[1]])?1:0";
  //ss << "(CAEXANEX1_CM.NUMERICA.PVLOLM:=(CAEXANEX1_CM.NUMERICA.PV=14) AND (AIRSA_1140.2LS.PVFL)?1:CAEXANEX1_CM.PVLO.PV[CAEXANEX1_CM.NUMERICA.PV]";
  //ss << "AVG (TR30.CALC.C[1], TR30.CALC.C[2])";
  //ss << "FY33.T.C[4]=2 ? FY33.T.P[2] : (FY33.T.C[4]=1? FY33.T.P[1] : FY33.T.P[3])";
  //ss << "(CAEXANEX_T2_CM.MOTOR_IS_PROG.PVFL[5] AND CAEXANEX_T2_CM.MOTOR_IS_RUN.PVFL[5])?CAEXANEX_T2_CM.TOTAL_TIME.PV[5]:-INF";
  //ss << "FY01.P.C[4]=1 ? (ISNAN(FY01.P.P[1]) ? FY01.P.C[6]  : FY01.P.P[1]) : FY01.P.C[6]";
  //ss << "P_1001_02_03_04.BASE_SEQUENCE2.C[5]=0.0?P_1001_02_03_04.BASE_SEQUENCE2.C[3]<>0.0?P_1001_02_03_04.BASE_SEQUENCE2.C[3]:P_1001_02_03_04.BASE_SEQUENCE2.C[4]<>0.0?P_1001_02_03_04.BASE_SEQUENCE2.C[4]:P_1001_02_03_04.BASE.PV:P_1001_02_03_04.BASE.PV+1.0";
  //ss << "FY1.TEMPSELECT.C[3] < FY1.TEMPSELECT.C[2] ? (FY1.TEMPSELECT.C[1] > FY1.TEMPSELECT.C[3] ? 3 : 1) : (FY1.TEMPSELECT.C[1] > FY1.TEMPSELECT.C[2] ? 2 : 1)";
  //ss << "CAEXANEX1_CM.NUMERICA.PVLOLM:=(CAEXANEX1_CM.NUMERICA.PV=14) AND (AIRSA_1140.2LS.PVFL)?1:CAEXANEX1_CM.PVLO.PV[CAEXANEX1_CM.NUMERICA.PV]";
  //ss << "GravelFilter1_CM.NUMERICA.PV := NOT(GravelFilter1_CM.STEPPLUS1.PVFL)?   GravelFilter1_CM.NUMERICA.PV: GravelFilter1_CM.PVHI.NNUMERIC<>GravelFilter1_CM.NUMERICA.PV?GravelFilter1_CM.NUMERICA.PV+1:2";
  //trans.Translate( ss, true );
  //return 0;

  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  InitCommonControls();

  HWND hDlg = CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, DialogProc, 0);
  ShowWindow(hDlg, nCmdShow);

  //return ServerTest( hDlg, hInstance, hPrevInstance, lpCmdLine, nCmdShow );

  InitNamesAndVariables( false, false );

  pRegisterFinderACSobj = fnRegisterFinderACSobj;

  LPSTR pszPrjName = "VSB";

  IBaseModel *models[] = { Load_Object("ControlBuilderH5xx_Client", pszPrjName),
                          //, Load_Object("MetsoDNA", pszPrjName2 )
                          //, Load_Object("MetsoDNA", pszPrjName3 )
                          //,Load_Object("MetsoDNAalarms", "DnaAlarms" )
                          //,Load_Object("MetsoDNAmessages", "DnaMessages" ) 
                         };

  for( int i=0; i<_countof(models); ++i )
  {
    if( !models[i] )
      continue;
    models[i]->Init( 0 );
  }

  for( int i=0; i<_countof(models); ++i )
  {
    if( !models[i] )
      continue;
    models[i]->Step0();
  }

#if 0
  char Path[_MAX_PATH];
  sprintf_s( Path, "%sDATA\\STATES\\__%s.dat", PROJECT_ROOT, pszPrjName );
  CSRFile file;
  pSRFile = &file;
//#define RESTORE
#ifndef RESTORE
  if( file.Create( Path ) )
    models[0]->SaveState();
#else
  if( file.Open( Path ) )
  {
    DWORD Key = 0;
    RD_VAR(Key)
    ASS( Key == 0xE1320724 );
    char ParmName[256];
    RD_STR( ParmName )
    int LStruct = 0;
    RD_VAR( LStruct )
    pSRFile->Bytes = LStruct;
    models[0]->RestoreState( ParmName );
  }
#endif
#endif

  for( int i=0; i<_countof(models); ++i )
  {
    if( !models[i] )
      continue;
    models[i]->Step1();
  }

  for( int i=0; i<_countof(s_localShowACSs); ++i )
  {
    if( !s_localShowACSs[i] )
      continue;
    s_localShowACSs[i]( NULL, NULL, pszPrjName, (void*)1 );
    //s_localShowACSs[i]( NULL, NULL, pszPrjName2, (void*)1 );
    break;
  }

  MSG msg;
  BOOL bIdle = TRUE;
  LONG lIdleCount = 0;

  LONGLONG llEnd   = 0;
  LONGLONG llStart = 0;
  LONGLONG llFreq  = 0;
  QueryPerformanceFrequency((LARGE_INTEGER*)&llFreq);
  QueryPerformanceCounter((LARGE_INTEGER*)&llStart);

  const double deltaStepSec = 0.36;
  // acquire and dispatch messages until a WM_QUIT message is received.
  for (;;)
  {
    // phase1: check to see if we can do idle work
    while (bIdle && !::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE))
    {
      QueryPerformanceCounter((LARGE_INTEGER*)&llEnd);

      double delay = (((double)(llEnd - llStart))/((double)llFreq));
      // call OnIdle while in bIdle state
      lIdleCount++;
      if( delay > deltaStepSec )
      {
        QueryPerformanceCounter((LARGE_INTEGER*)&llStart);
        for( int i=0; i<_countof(models); ++i )
        {
          if( !models[i] )
            continue;
          models[i]->StepT( deltaStepSec / 3600. );
          //models[i]->StepT( 1. / 3600. );
        }
      }
    }

    // phase2: pump messages while available
    do
    {
      // pump message, but quit on WM_QUIT
      if (!PumpMessage(&msg))
      {
        //return ExitInstance();
        return (int) msg.wParam;
        break;
      }

      // reset "no idle" state after pumping "normal" message
      //if (IsIdleMessage(&m_msgCur))
      if (IsIdleMessage(&msg) )
      {
        bIdle = TRUE;
        lIdleCount = 0;
      }

    } while (::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE));
  }

	return (int) msg.wParam;
}
