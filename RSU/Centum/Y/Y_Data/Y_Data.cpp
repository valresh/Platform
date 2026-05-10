#include <Y_Data.h>
#include "StdDef.h"
#include <math.h>
#include <Y_Graf.h>
#include <ConnectY.h>
#include "ReadParms.h"
#include <rsus/rsuPaths.h>
#include <macros/AutoCloser.h>

tFindRef pFindRef = NULL;

const double CY_Base::s_ControlPeriod = 1.;
bool CY_Base::s_flagBasicScanControl = false;

DWORD CY_Base::s_nCalcSteps = 0;
bool CY_Base::s_IsBlk = true;
__time64_t CY_Base::s_ModelFullTimeT = 0;// Модельное полное время 

bool CY_Base::s_ReadingTunningData = false;
bool CY_Base::s_flagReadAllTunningValues = true;

CY_Base::CY_Base( bool No )
{
  Clear();
}

CY_Base::CY_Base()
{
  Clear();
}

void CY_Base::Clear()
{
  BreakPoint = false;
  m_pFlagUseInW = NULL;
  Name[0] = 0;
  szAkaObjName[0] = 0;
  Model[0] = 0;
  ZeroMemory( Info, sizeof(Info) );
  kInfo = 0;
  xInfo = 40;
  yInfo = -70;
  ShowInfo = true;
  pMode = NULL;
  pFound = NULL;
  pComm = "";
  IsWork = true;
  memset ( &rc, 0, sizeof ( rc ));
  sequenceNumberInDR = -1;
  ZeroMemory( szNumber, sizeof(szNumber) );
  ZeroMemory( szUPPL, sizeof(szUPPL) );
  firstConnectionK = lastConnectionK = -1;
  bExecutedAtStep = true;
  pDR = NULL;
  Y_Type = 0;
  pClassBase = NULL;
  ClassParmInfo = NULL;
  kClassParmInfo = 0;
  ClassVarInfo = NULL;
  kClassVarInfo = 0;
  pClass_P = NULL;
  L_Class_P = 0;
  Used = 0;
  bConnectedByACS = FALSE;
  controllerType = ectRSU;
  XAF = 0;
  PV_BAD = 0;
  ESCA = ESCA_ScanPeriod_basic;
  m_nBasicControlPeriodCounter = 0;
  pInitialClass_W = NULL;
  L_InitialClass_W = 0;
}

char * CY_Base::GetModelName()
  {
  return Model;
  }

CY_DI* CY_Base::GetNextDI( CY_Base *pObj )
{
  ASS(pObj);
  if( !pObj )
    return NULL;
  switch( pObj->Y_Type )
  {
  case id__WB:
    {
      CY_DI *pDI = (CY_DI *)pObj; 
      int N = pDI->Numb;
      int N0 = N % CY_DR::s_dwFcsNumModulus;
      int N1 = N / CY_DR::s_dwFcsNumModulus;
      if ( N0 < 16 )
        N0++;
      else
      {
        N0 = 1;
        N1++;
      }
      N = N1 * CY_DR::s_dwFcsNumModulus + N0;
      pDI = (*pFindDI)( pDI->NumbFCS, pDI->domain, N, id__WB );
      ASS( pDI );
      return pDI;
    }
    break;
  case id__Z:
    {
      CY_DI *pDI = (CY_DI *)pObj; 
      int N = pDI->Numb;
      N++;
      pDI = (*pFindDI)( pDI->NumbFCS, pDI->domain, N, id__Z );
      //ASS( pDI );
      return pDI;
    }
    break;
  case id__Y:
    {
      CY_DI *pDI = (CY_DI *)pObj; 
      int N = pDI->Numb;
      N++;
      pDI = (*pFindDI)( pDI->NumbFCS, pDI->domain, N, id__Y );
      //ASS( pDI );
      return pDI;
    }
    break;
  case id__SW:
    {
      CY_DI *pDI = (CY_DI *)pObj; 
      int N = pDI->Numb;
      ++N;
      pDI = (*pFindDI)( pDI->NumbFCS, pDI->domain, N, id__SW );
      ASS( pDI );
      return pDI;
    }
    break;
  default:
    ASS(FALSE);
  }
  return NULL;
}

void CY_Base::InitFlagUse( CAlarmBase::tFlagUse_ &FlagUse )
{
  FlagUse &= !CAlarmBase::fuY_Mask;
  m_pFlagUseInW = &FlagUse;
}

CAlarmBase::tFlagUse_ *CY_Base::GetFlagUse()
{
  return m_pFlagUseInW;
}

int CY_Base::SaveState( )
{
	SAVE_CLASS("WorkBase",CY_Base_W)
	return 0;
}

int CY_Base::RestoreState( char * StrName )
{
	REST_CLASS("WorkBase",CY_Base_W)
	return 1;
}

int CY_Base::SetData( int TypeData, void * pData )
  {
  return 0;
  }

void CY_Base::SetParmList( )
  {
  }

void CY_Base::SetObjName( int NumbFCS /*= 0*/, char chStationType /*= 'F'*/ )
{
  if( NumbFCS )
  {
    ASSD( !"Устаревшая реализация, использовать следующий метод" );
    if( NumbFCS < CY_DR::s_dwFcsNumModulus )
      sprintf_s( szAkaObjName, "%cCS00%02d/", chStationType, NumbFCS );
    else
      sprintf_s( szAkaObjName, "%cCS%04d/", chStationType, NumbFCS );
  }
  else
  {
    strcpy_s( szAkaObjName, pDR->FCS );
    strcat_s( szAkaObjName, "/" );
  }
  strcat_s( szAkaObjName, Name );
}

void CY_Base::SetObjName( const char *pTextedName )
{
  sprintf_s( szAkaObjName, "%s/%s", pTextedName, Name );
}

bool CY_Base::SaveParms( HANDLE hFile )
{
  return false;
}
bool CY_Base::RestParms( HANDLE hFile )
{
  return false;
}

static CYVarInfo Key;
bool CY_Base::GetVar( char * VarName, void ** pVar, void ** pTuneVar, char * Type, eVarType *Work, CAlarmBase::tFlagUse_ **ppFlagUse /*= NULL*/, USHORT *pVarSize /*= NULL*/ )
{
  if ( pClassBase == NULL )
    return false;
  if( ppFlagUse )
    *ppFlagUse = GetFlagUse();
  char buf[64] = { "CALI" };
  if( !strcmp( VarName, "PV=CAL") )
    Key.VarName = buf;
  else
    Key.VarName = VarName;
  CYVarInfo * pI = (CYVarInfo*)bsearch ( &Key, ClassVarInfo, kClassVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  if( pI == NULL )
  {
    LPSTR pNul = strchr( VarName, '[' );
    if( pNul )
    {
      size_t l = pNul-VarName;
      strncpy_s( buf, VarName, l );
      int n = atoi( pNul+1 );
      if( n > 9 )
      {
        strcat_s( buf, pNul+1 );
        pNul = strchr( buf, ']' );
        if( pNul )
          *pNul = 0;
      }
      else
        sprintf_s( buf + l, _countof(buf)-l, "%02d", n);
      return GetVar( buf, pVar, pTuneVar, Type, Work, ppFlagUse, pVarSize );
    }
    pNul = strchr( VarName, '0' );
    if( pNul )
    {
      strncpy_s( buf, VarName, pNul-VarName );
      strcat_s( buf, pNul+1 );
      Key.VarName = buf;
    }
    else if( 'P'==VarName[0] && (VarName[1]>'0'&&VarName[1]<='9')  )
    {
      sprintf_s( buf, "P0%s", VarName+1 );
      Key.VarName = buf;
    }
    if( !_strcmpi(VarName,"XAF") )
    {
      if( Type )
        *Type = 'W';
      if( pTuneVar )
        *pTuneVar = NULL;
      if( Work )
        *Work = CY_Base::evtP;
      if( pVar )
       *pVar = &XAF;
      return true;
    }
    if( !_strcmpi(VarName,"PV=BAD") )
    {
      *Type = 'I';
      *pTuneVar = NULL;
      *Work = CY_Base::evtP;
      *pVar = &PV_BAD;
      return true;
    }
    pI = (CYVarInfo*)bsearch ( &Key, ClassVarInfo, kClassVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  }
  if( pI == NULL )
  {
    CSignal *pSigOut = GetOUT( VarName );
    if( pSigOut && Type && pVar )
    {
      if( Work )
        *Work = evtSignal;
      switch ( pSigOut->Type )
      {
      case CSignal::Int:
        if( Type )
          *Type = 'I';
        if( pVar )
          *pVar = pSigOut->pInt;
        break;
      case CSignal::Double:
        if( Type )
          *Type = 'D';
        if( pVar )
          *pVar = pSigOut->pDbl;
        break;
      case CSignal::Bool:
        if( Type )
          *Type = 'B';
        if( pVar )
          *pVar = pSigOut->pBool;
        break;
      case CSignal::NoType:
        if( Type )
          *Type = 'I';
        if( pVar )
          *pVar = pSigOut->pInt;
        break;
      case CSignal::Byte:
        if( Type )
          *Type = 'W';
        if( pVar )
          *pVar = pSigOut->pByte;
        break;
      case CSignal::String:
        if( Type )
          *Type = 'S';
        if( pVar )
          *pVar = pSigOut->pChar;
        if( pVarSize )
          *pVarSize = pSigOut->stringSize;
        break;
      default:
        ASS(FALSE);
        if( Work )
          *Work = CY_Base::evtP;
        return false;
      }
      return true;
    }
    return false;
  }
  BYTE * pV = pClassBase + pI->sVar;
  if( pVar )
    *pVar = pV;
  if( pVarSize )
    *pVarSize = pI->szVarSize;
  if( pTuneVar )
  {
    if( pI->sVarTune )
    {
      pV = pClassBase + pI->sVarTune;
      *pTuneVar = pV;
    }
    else
      *pTuneVar = NULL;
  }
  if( Type )
    *Type = pI->Type;
  if( Work )
    *Work = pI->Work ? CY_Base::evtWork : CY_Base::evtP;
  return true;
}

static char Class[64];
char * CY_Base::GetClass()
{
  strcpy_s( Class, "CY_" );
  strcat_s( Class, Model );
  return Class;
}

int CY_Base::GetParams( KYBridge2SysParam &params )
{
  #include "IO_Parms.h"
	PARM(Name, "#Имя" )
  if( pDR )
	  PARM_CALLBACK(pDR->FCS, "#FCS" )
  
  void * pTuneVar = NULL;
  void *pVar = NULL;
  CY_Base::eVarType Work = CY_Base::evtP;
  char varType = ' ';
  if( CY_Base::GetVar( "@ALRM", &pVar, &pTuneVar, &varType, &Work ) && 'S'==varType )
  {
    PARM( (LPSTR)pVar, "#@ALRM" )
  }
  if( szUPPL[0] )
    PARM( szUPPL, "#Upper Equipment Name" )
  return 0;
}

static LPCSTR s_Name = "CY_Base";
int CY_Base::StateSave( IYokoStateSer* pStateSer )
{
  DWORD nVars = 0;
  if( !pClassBase )
  {
    WriteFullStruct( pStateSer, eKeyVarCountOfObj, s_Name, nVars );
    return -1;
  }
  for( int i=0; i<kClassVarInfo; ++i )
  {
    CYVarInfo &var = ClassVarInfo[i];
    if( 1!=var.Work )
      continue;
    ++nVars;
  }
  WriteFullStruct( pStateSer, eKeyVarCountOfObj, s_Name, nVars );
  for( int i=0; i<kClassVarInfo; ++i )
  {
    CYVarInfo &var = ClassVarInfo[i];
    if( 1!=var.Work )
      continue;
    BYTE *pV = pClassBase + var.sVar;
    pStateSer->WriteStr( var.VarName );
    pStateSer->SimpleWrite( var.Type );
    pStateSer->SimpleWrite( var.szVarSize );
    pStateSer->SimpleWrite( pV, var.szVarSize );
  }
  return 0;
}

int CY_Base::StateRestore( IYokoStateSer* pStateSer )
{
  DWORD vars = 0;
  if( ReadNamedFullStruct( pStateSer, eKeyVarCountOfObj, s_Name, vars ) )
    return 1;
  char szVarName[24];
  char type;
  USHORT varSize;
  for( DWORD n=0; n<vars; ++n )
  {
    if( !Read( pStateSer, szVarName ) )
      return 2;
    Read( pStateSer, type );
    Read( pStateSer, varSize );
    void *pV = NULL;
    char rType;
    USHORT rSize;
    eVarType work;
    bool b = GetVar( szVarName, &pV, NULL, &rType, &work, NULL, &rSize );
    if( !b || !pV )
      pStateSer->Shift( varSize );
    else if( !work )
      pStateSer->Shift( varSize );
    else
    {
      if( type==rType && varSize==rSize )
        pStateSer->SimpleRead( pV, varSize );
      else
      {
        pStateSer->Shift( varSize );
      }
    }
  }

  return 0;
}

void CY_Base::OnParamsInitialized()
{
  if( !L_Class_P || !pClassBase )
    return;
  L_InitialClass_W = L_Class_P + sizeof(CBase);
  pInitialClass_W = (BYTE*)KMemoryServiceY::Instance().NewMemCentum( L_InitialClass_W );
  if( !pInitialClass_W )
    return;
  CopyMemory( pInitialClass_W, pClassBase, L_InitialClass_W );
}

static LPCSTR s_ChangedParams = "ChangedParams";
int CY_Base::ChangedParamsSave( IYokoStateSer* pStateSer )
{
  DWORD nVars = 0;
  int i = 0;
  if( !pInitialClass_W || !L_InitialClass_W || !memcmp( pInitialClass_W, pClassBase, L_InitialClass_W ) )
    i = kClassVarInfo;

  for( i; i<kClassVarInfo; ++i )
  {
    CYVarInfo &varI = ClassVarInfo[i];
    if( varI.Work )
      continue;
    switch( varI.Type )
    {
    case 'D':
    case 'I':
    case 'B':
      break;
    default:
      continue;
    }
    BYTE *pWork = pClassBase + varI.sVar;
    BYTE *pInitial = pInitialClass_W + varI.sVar;
    if( !memcmp( pWork, pInitial, varI.szVarSize ) )
      continue;
    if( 'D'==varI.Type && sizeof(double)==varI.szVarSize )
    {
      double d1 = *(double*)pWork;
      double d2 = *(double*)pInitial;
      if( abs(d1-d2) < 0.001 )
        continue;
    }
    ++nVars;
  }
  WriteFullStruct( pStateSer, eKeyVarCountOfObj, s_ChangedParams, nVars );
  if( !nVars )
    return 0;

  DWORD nSavedVars = 0;
  for( i=0; i<kClassVarInfo; ++i )
  {
    CYVarInfo &varI = ClassVarInfo[i];
    if( varI.Work )
      continue;
    switch( varI.Type )
    {
    case 'D':
    case 'I':
    case 'B':
      break;
    default:
      continue;
    }
    BYTE *pWork = pClassBase + varI.sVar;
    BYTE *pInitial = pInitialClass_W + varI.sVar;
    if( !memcmp( pWork, pInitial, varI.szVarSize ) )
      continue;
    if( 'D'==varI.Type && sizeof(double)==varI.szVarSize )
    {
      double d1 = *(double*)pWork;
      double d2 = *(double*)pInitial;
      if( abs(d1-d2) < 0.001 )
        continue;
    }
    ASSD( nSavedVars<nVars );
    if( nSavedVars==nVars )
      break;
    ++nSavedVars;

    pStateSer->WriteStr( varI.VarName );
    pStateSer->SimpleWrite( varI.Type );
    pStateSer->SimpleWrite( varI.szVarSize );
    pStateSer->SimpleWrite( pWork, varI.szVarSize );
  }
  return 0;
}

int CY_Base::ChangedParamsRestore( IYokoStateSer* pStateSer )
{
  DWORD vars = 0;
  if( ReadNamedFullStruct( pStateSer, eKeyVarCountOfObj, s_ChangedParams, vars ) )
    return 1;
  char szVarName[24];
  char type;
  USHORT varSize;

  for( DWORD n=0; n<vars; ++n )
  {
    if( !Read( pStateSer, szVarName ) )
      return 2;
    Read( pStateSer, type );
    Read( pStateSer, varSize );
    void *pV = NULL;
    char rType;
    USHORT rSize;
    eVarType work;
    bool b = GetVar( szVarName, &pV, NULL, &rType, &work, NULL, &rSize );
    if( !b || !pV )
      pStateSer->Shift( varSize );
    else
    {
      if( type==rType && varSize==rSize )
        pStateSer->SimpleRead( pV, varSize );
      else
      {
        pStateSer->Shift( varSize );
      }
    }
  }

  return 0;
}

bool CY_Base::CreatePSMGraf( CGrafParm ** pGP )
{
  if( pGP )
    *pGP = NULL;
	return false;
}

void CY_Base::LinkToDI()
{
}

void CY_Base::Init( )
	{
	}

int CY_Base::Prepare0()
{
	return 0;
}

int CY_Base::Prepare1()
{
	return 0;
}

void CY_Base::ResetAlarms()
{
}

BOOL CALLBACK EnumChildProc( HWND hwnd, LPARAM lParam )
	{
	char Txt[1024];
	char Title[1024];
	GetClassName( hwnd,	Txt, 1023 );
	if ( strcmp ( Txt, "ComboBox" ) == 0 )
		{
		GetWindowText ( hwnd, Title, 1023 );
		SendMessage( hwnd, WM_SETTEXT, 0, (LPARAM)lParam ); 
	  return TRUE;
		}
	if ( strcmp ( Txt, "Button" ) == 0 )
		{
		GetWindowText ( hwnd, Title, 1023 );
		if ( strcmp ( Title, "OK" ) == 0 )
			{
			PostMessage ( hwnd, WM_LBUTTONDOWN, 0, 0x000A000A	);
			PostMessage ( hwnd, WM_LBUTTONUP, 0, 0x000A000A	);
			}
	  return FALSE;
		}
	return TRUE;
	}


void CY_Base::ShowCENTUM()
{
	HWND hWnd = FindWindow( NULL, "System Message Monitor" );
	if ( hWnd == NULL )
		return;

  char FullName[128];
	strcpy_s ( FullName, "O " );
	strcat_s ( FullName, Name );
	if ( GetAsyncKeyState ( VK_SHIFT ) & 0x8000 )
		strcat_s ( FullName, 128, " DRAW" );
	COPYDATASTRUCT Copy;
	Copy.dwData = 2;
	Copy.cbData = lstrlen ( FullName ) + 1;
	Copy.lpData = FullName;
	SendMessage ( hWnd, WM_COPYDATA, NULL, (LPARAM)&Copy );
}

bool CY_Base::SetLimits( )
  {
  return true;
  }

CSignal * CY_Base::GetOUT ( char * Name )
  {
	return NULL;
	}

CSignal ** CY_Base::GetIN ( char * Name )
  {
	return NULL;
	}

void CY_Base::Work ( double dt )
  {
  ASS(FALSE)
  }

double * CY_Base::GetValueAddr() 
  { 
  return NULL; 
  };

bool CY_Base::ReadParm ( char * ParmName, char * ParmValue, BYTE * pData )
{
  if( strstr( ParmName, "UPPL" ) )
  {
    strcpy_s( szUPPL, ParmValue );
    return true;
  }
  if( strstr( ParmName, "ESCA" ) )
  {
    char Fld[4][4];
    int K = SetFld_X( ParmValue, ':', Fld );
    switch( K )
    {
    case 1:
      K = 0;
      break;
    case 2:
      ASSD('1'==Fld[0][0])
      K = 1;
      break;
    default:
      ASS(0)
      return false;
    }
    switch( Fld[K][0] )
    {
    case 'S':
      ESCA = ESCA_ScanPeriod_basic;
      break;
    case 'M':
      ESCA = ESCA_ScanPeriod_medium;
      break;
    case 'H':
      ESCA = ESCA_ScanPeriod_high;
      break;
    default:
      ASS(0);
      return false;
    }
    return true;
  }

  if ( pClassBase == NULL )
    return false;
  if ( ParmName[0] == 'T' && ParmName[1] == 'P' && ParmName[2] == 'F' && ParmName[3] == 'X' )
    {
    return GetTPFX ( this, pClassBase, kClassVarInfo, ClassVarInfo,  ParmValue );
    }
//
  CYParmInfo * pI = GetParm ( kClassParmInfo, ClassParmInfo, ParmName );
  if ( pI == NULL )
    {
    return false;
    }
  if ( pI->Type[0] == 'V' )
    {
// VARIANT
    char FullName[16];
    strcpy_s( FullName, ParmName );
    char * P = strchr ( ParmValue, ':' );
    ASS(P)
    *P = 0;
    strcat_s( FullName, ":" );
    strcat_s( FullName, ParmValue );
    *P = ':';
    ParmValue = P + 1;
    pI = GetParm ( kClassParmInfo, ClassParmInfo, FullName );
    if ( pI == NULL )
      {
      return false;
      }
    }
  if ( pI->ParmFmt == NULL )
    {
    return true;
    }
  if ( pI->Read == 0 )
    {
    bool Res = StdGet ( pClassBase, pI, ParmValue );
    return false;
    }
  ASS(FALSE)
  return false;
}

void CY_Base::NoRead ( int * pVar, char * Name, char * Fmt )
{
}

bool CY_Base::Draw (CDrawData * pDD, int x0, int y0 )
  {
	if ( kInfo <= 0 || !ShowInfo )
		return false;
  if ( kInfo == 1 )
    {
	  SetTextColor ( pDD->hDC, RGB ( 255, 255, 255 ));
	  SelectObject ( pDD->hDC, pDD->hFont );
    char Txt[255];
		Info[0]->Out( Txt );
		TextOut ( pDD->hDC, x0, y0, Txt, (int)strlen ( Txt )); 
    return false;
    }
	SelectObject ( pDD->hDC, pDD->hPenInfo );
	int w0 = 40;
	int w1 = 60;
	int h1 = 16;
	int w = w0 + w1;
  int h = ( kInfo + 1 ) * h1;
	MoveToEx ( pDD->hDC, x0, y0, NULL );
	LineTo ( pDD->hDC, x0 + w, y0 );
	LineTo ( pDD->hDC, x0 + w, y0 + h );
	LineTo ( pDD->hDC, x0, y0 + h );
	LineTo ( pDD->hDC, x0, y0 );
	int y_l = y0 + h1;
	MoveToEx ( pDD->hDC, x0 + w0, y_l, NULL );
	LineTo ( pDD->hDC, x0 + w0, y0 + h );
	SelectObject ( pDD->hDC, pDD->hFontInfo );
	SetTextColor ( pDD->hDC, RGB ( 255, 255, 255 ));
	SetTextAlign ( pDD->hDC, TA_CENTER | TA_BASELINE );
  char Title[64];
  if ( pMode )
    {
    strcpy_s ( Title, Name );
    if ( pMode->M == SMode::MAN )
      strcat_s ( Title, " MAN" );
    if ( pMode->M == SMode::AUT )
      strcat_s ( Title, " AUT" );
    if ( pMode->M == SMode::CAS )
      strcat_s ( Title, " CAS" );
    if ( pMode->M == SMode::RCAS )
      strcat_s ( Title, " RCAS" );
    if ( pMode->M == SMode::ROUT )
      strcat_s ( Title, " ROUT" );
    if ( pMode->M == SMode::OS )
      strcat_s ( Title, " O/S" );
    else
      {
      if ( pMode->IMAN )
        {
        strcat_s ( Title, " IMAN" );
        }
      else
        {
        if ( pMode->TRK )
          strcat_s ( Title, " TRK" );
        }
      }
    }
  else
    strcpy_s ( Title, Name ); 
	TextOut ( pDD->hDC, x0 + w / 2, y_l - 2, Title, (int)strlen ( Title )); 
	SetTextAlign ( pDD->hDC, TA_LEFT|TA_TOP );
	char Txt[256];
	for ( int p = 0; p < kInfo; p++ )
		{
		MoveToEx ( pDD->hDC, x0, y_l, NULL );
		LineTo ( pDD->hDC, x0 + w, y_l );
		TextOut ( pDD->hDC, x0 + 5, y_l + 2, Info[p]->ParamName, (int)strlen ( Info[p]->ParamName )); 
		Info[p]->Out ( Txt );
		TextOut ( pDD->hDC, x0 + w0 + 5, y_l + 2, Txt, (int)strlen ( Txt )); 
		y_l += h1;
		}
	SetTextColor ( pDD->hDC, RGB ( 255, 255, 255 ));
  SelectObject ( pDD->hDC, pDD->hFont );
	return false;
  }

char CY_Base::TestCNCT( LPSTR pConn )
{
  LPCSTR pIO = strrchr( pConn, ':' );
  if( !pIO )
    return ' ';
  return pIO[1];
}

bool CY_Base::TransferConnection( LPCSTR pszFieldInName )
{
  return true;
}

CBase* CY_Base::GetStructure()
{
  return (CBase*)pClassBase;
}

bool CY_Base::EnumUsedObj( DWORD &pos, LPCSTR *ppName, CY_Base **ppObj )
{
  if( ppName )
    *ppName = NULL;
  if( ppObj )
    *ppObj = NULL;
  return false;
}

/////////////////////////////////////////////////////////////
STrace * STrace::pFirst = NULL;
STrace * STrace::pLast = NULL;

STrace::STrace()
	{
	memset ( this, 0, sizeof ( STrace ));
	if ( pFirst == NULL )
		pFirst = this;
	if ( pLast )
		pLast -> pNext = this;
	pLast = this;
	pNext = NULL;
	}

#define REPL(From,To) \
	if ( strcmp ( pModel, From ) == 0 )\
		return To;

char * STrace::ConvModel ( char * pModel )
	{
	REPL("MC-2","MC-X")
	REPL("MC-3","MC-X")
	REPL("MC-2E","MC-X")
	REPL("MC-3E","MC-X")
	REPL("MC-3E","MC-X")
	REPL("SI-1","SX-X") 
	REPL("SI-1E","SX-X")
	REPL("SI-2","SX-X")
	REPL("SI-2E","SX-X")
	REPL("SO-1","SX-X")
	REPL("SO-1E","SX-X")
	REPL("SO-2","SX-X")
	REPL("SO-2E","SX-X")
	REPL("SIO-11","SX-X")
	REPL("SIO-11E","SX-X")
	REPL("SIO-12","SX-X")
	REPL("SIO-12E","SX-X")
	REPL("SIO-21","SX-X")
	REPL("SIO-21E","SX-X")
	REPL("SIO-22","SX-X")
	REPL("SIO-22E","SX-X")
	REPL("SIO-12P","SX-X")
	REPL("SIO-12PE","SX-X")
	REPL("SIO-22P","SX-X")
	REPL("SIO-22PE","SX-X")
	return pModel;
	}

#define PRT_DATA
#define FILE_DATA

char * GetFldName( char * Mod_Name );
bool TestName ( char * Name );

struct CSKInfo
{
  char Fld_Name[32];
  char Model_Name[32];
};

CSKInfo SKInfo[2000];
int kInfo;
char * All_Names[2000];
int kAll_Names;

bool TestName ( char * Name )
  {
  for ( int n = 0; n < kAll_Names; n++ )
    {
    if ( strcmp ( All_Names[n], Name ) == 0 )
      return false;
    }
  ASS(kAll_Names < _countof(All_Names) );
  All_Names[kAll_Names++] = Name;
  return true;
  }

int CompInfo ( const void * p1, const void * p2 )
  {
  CSKInfo * pI1 = (CSKInfo*)p1;
  CSKInfo * pI2 = (CSKInfo*)p2;
  int Res = strcmp ( pI1->Model_Name, pI2->Model_Name ); 
//  ASS( Res != 0 )
  return Res;
  }

__declspec(dllexport) void ReadSKInfo()
  {
  kInfo = 0;
  kAll_Names = 0;
  char Path[1024];
  sprintf_s ( Path, 1024, "%sData\\Acy\\Данные.csv", rsuGetProjectPath());
  FILE * F = fopen ( Path, "rt" );
  if ( F == NULL )
    return;
  char Str[1024];
  while ( fgets ( Str, 1023, F ))
    {
    if ( Str[0] != ';' )
      continue;
    char * Fld_Name = strchr ( Str + 1, ';' );
    Fld_Name++;
    char * Model_Name = strchr ( Fld_Name, ';' );
    *Model_Name = 0;
    Model_Name++;
    char * End = strchr ( Model_Name, ';' );
    *End = 0;
    ASS(kInfo < _countof(SKInfo) );
    strcpy_s( SKInfo[kInfo].Fld_Name, Fld_Name );
    strcpy_s( SKInfo[kInfo].Model_Name, Model_Name );
    kInfo++;
    }
  fclose ( F );
  qsort ( SKInfo, kInfo, sizeof ( CSKInfo ), CompInfo );
  }

static CSKInfo InfoKey;
char * GetFldName( char * Mod_Name )
  {
  lstrcpy ( InfoKey.Model_Name, Mod_Name );
  CSKInfo * pRes = (CSKInfo*)bsearch ( &InfoKey, SKInfo, kInfo, sizeof ( CSKInfo ), CompInfo );
  if ( pRes == NULL )
    return NULL;
  return pRes->Fld_Name;
  }

