#include "QuickBuilder.h"
#include <locale.h>
#include <vector>
#include "QB_Class.h"
#include <macros/StrHelps.h>
#include <macros/AutoRestore.h>
#include <macros/AutoCloser.h>
//#include <macros/DirFileCreator.h>
#include <rsuErr.h>
#include <rsuPaths.h>
//#include <io.h>
#include <crosspath.h>
#include <crossstring.h>

using namespace std;


KQuickBuilder::tItemChnCont_::iterator KQuickBuilder::FilterForChn( KQbBase* p, LPCSTR pszFuncDet, LPCSTR pszRTUaddress, char IO )
{
  tItemChnCont_::iterator it = m_chn2name.find( pszRTUaddress );
  if( m_fAI && m_chn2name.end()!=it )
  {
    //if( !strcmp( it->second.chn.c_str(), "220PSA") )
    {
      fprintf( m_fAI, "%s.%s;;", p->PointName.c_str(), pszFuncDet );
      LPCSTR pszDescr = NULL;
      switch( p->QB_Type )
      {
      case id_QBSTATUS:
        fprintf( m_fAI, "D");
        if( ((SQB_STATUS*)p)->W )
          pszDescr = ((SQB_STATUS*)p)->W->DESC;
        break;
      case id_QBANALOG:
        fprintf( m_fAI, "A");
        if( ((SQB_ANALOG*)p)->W )
          pszDescr = ((SQB_ANALOG*)p)->W->DESC;
        break;
      }
      fprintf( m_fAI, "%c;", IO);
      /*switch( *pszFuncDet )
      {
      case 'P':
        fprintf( m_fAI, "I;");
        break;
      case 'O':
      case 'S':
        fprintf( m_fAI, "O;");
        break;
      default:
        ASSD(0);
      }*/
      if( pszDescr )
        fprintf( m_fAI, "%s", pszDescr );
      fprintf( m_fAI, ";;;;;\n" );
    }
  }
  return it;
}

int KQuickBuilder::ReadProject()
{
  char* Q = setlocale( LC_ALL, "Russian" );
  char *pdec = localeconv()->decimal_point;
  if( pdec && '.'!=*pdec )
    setlocale( LC_NUMERIC, "C" );
  pdec = localeconv()->decimal_point;
  ASS( '.'==*pdec );

  //char szPath[_MAX_PATH] = { 0 };

  char name[128*4];
  sprintf_s( name, "%s%s%s", GetCsvPrefix(), m_szObjName, ".csv");
  fs::path path = fs::path(rsuGetMemoryPath()) / name;
  //sprintf_s(szPath, "%s", rsu_cp::makepath_cp(rsuGetMemoryPath(), name, "").string().c_str());
  //_makepath_s( szPath, NULL, rsuGetMemoryPath(), name, "csv" );
  if( rsu_cp::cp_access(path, rsu_cp::cp_path_access::F) || m_bCreateData)
  {
    m_fAI = fopen(path.generic_string().c_str(), "wt");
    if( NULL!=m_fAI )
      fprintf( m_fAI, "ED#Точка;ED#Alt;TypeDV#Тип;ST#Описание;LE,Objects#Модель;LS,Type#ТипМодели;Points#ACS;ED#Формула;ST#EngUnit;\n" );
  }

  auto Path = GetHdwFile();
  ReadHdws( Path.c_str() );

  Path = GetPntFile();
  ReadPnts( Path.c_str() );


  /*
  char szMem[_MAX_PATH] = { 0 };
  GetQdbFile( szPath );
  _makepath_s( szMem, NULL, GetMemoryPath(), ObjName, NULL );
  CreateDirectory( szMem, NULL );
  HMODULE h = LoadLibrary("QuickBuilderH_EX.dll");
  if( !h )
    return -1;
  typedef int(*lpEnumTables)(LPCSTR, LPCSTR );
  lpEnumTables fnEnumeration = (lpEnumTables)GetProcAddress( h, "EnumTables" );
  if( fnEnumeration )
  {
    fnEnumeration( szPath, szMem );
  }*/

  if( m_fAI )
    fclose( m_fAI );
    
  m_fAI = NULL;

  return 0;
}

enum EStateSelectField
{
  essfHZ,
  essfItem,
  essfDEL,
  essfADD,
  essfHandler,
  essIgnore,
};

int KQuickBuilder::ReadHdws( LPCSTR pszHdwFile )
{
  char szsLine[1024] = { 0 };
  char szLine[1024*4] = { 0 };
  FILE *file = NULL;
  if( fopen_s( &file, pszHdwFile, "rt" ) )
    return -1;

  KAutoCloser<FILE*> fc( file, fclose );

  char szItemName[32*4] = { 0 };
  char szChn[5][32*4] = { 0 };
  size_t lenItemName = 0;
  EStateSelectField state = essfHZ;

  for( int nLine = 1; fgets(szsLine, _countof(szsLine), file); ++nLine )
  {
    cross::string::convert_to_utf8(1251, szsLine, szLine, sizeof(szLine));

    if( '\n'==szLine[0] || '\r' == szLine[0])
      continue;
   
    for( LPSTR p = szLine; *p; ++p )
    {
      switch( *p )
      {
      case '\r':
      case '\n':
        *p = 0;
        break;
      }
    }

    size_t l = strlen(szLine);
    ASS( l < (_countof(szLine)-1) );

    switch( state )
    {
    default:
      ASS(0);
      break;
    case essfItem:
      if( !strncmp(szLine, "ADD", 3) )
      {
        LPSTR p = NULL;
        for( p = szLine+4; ' '!=*p; ++p )
        {
          KKK();
        }
        TStringTerminator st( p );
        int i = 0;
        for( i=0; i<_countof(szChn); ++i )
        {
          if( !szChn[i][0] )
          {
            strcpy_s( szChn[i], szLine+4 );
            break;
          }
        }
        ASS( i<_countof(szChn) );
      }
      else if( !strncmp(szLine, "DEF", 3) )
      {
        char arr[50][128*4] = { };
        int cArr = SplitString( szLine, arr, ' ', false );
        LPSTR p = NULL;
        for( p = szLine+4; ' '!=*p; ++p )
        {
          KKK();
        }
        TStringTerminator st( p );
        char ref[32*4] = { 0 };
        strcpy_s( ref, szLine+4 );
        if( !strncmp(ref, "CHN", 3) )
        {
          if( !strncmp(arr[3], "NAME=", 5) )
          {
            ASS( !strcmp(arr[3]+5, szItemName) );
          }
          else if( !strncmp(arr[4], "NAME=", 5) )
          {
            ASS( !strcmp(arr[4]+5, szItemName) );
          }
          else
          {
            ASSD(0);
          }
          tItemChnCont_::iterator it = m_chn2name.find( ref+3 );
          ASS( m_chn2name.end()==it );
          SChnInfo ch;
          ch.chn = szItemName;
          if( !strcmp(arr[2], "MOD") )
          {
            if( !strcmp(szItemName,"RSLOGIX_CH") )
              ch.flags = ch.eRSLOGIX_MOD;
            else
              ch.flags = ch.eMOD;
          }
          else if( !strcmp(arr[2], "OPC") )
            ch.flags = ch.eOPC;
          else if( !strcmp(arr[2], "SMPKSETH") )
            ch.flags = ch.eSMPKSETH;
          else if( !strcmp(arr[2], "DB") )
            ch.flags = ch.eDB;
          else if( !strcmp(arr[2], "UMB") )
            ch.flags = ch.eUMB;
          else
          {
            ASSD(0);
          }
          m_chn2name[ref+3] = ch;
        }
        else if( !strncmp(ref, "RTU", 3) )
        {
          if( !strncmp(arr[3], "NAME=", 5) )
          {
            ASS( !strcmp(arr[3]+5, szItemName) );
          }
          else if( !strncmp(arr[4], "NAME=", 5) )
          {
            ASS( !strcmp(arr[4]+5, szItemName) );
          }
          else
          {
            ASS(0);
          }
          p = ref;
          for( p; '.'!=*p; ++p )
          {
            KKK();
          }
          TStringTerminator sts(p);
          ++p;
          tItemChnCont_::iterator it = m_chn2name.find( ref+3 );
          ASS( m_chn2name.end()!=it );
          tItemChnCont_::iterator itN = m_chn2name.find( p );
          ASS( m_chn2name.end()==itN );
          SChnInfo ch = it->second;
          ch.ID = atoi( arr[2] );
          switch( ch.flags )
          {
          case SChnInfo::eRSLOGIX_MOD:
            {
              SControlInfo ci;
              strcpy_s( ci.Name, szItemName );
              ASSD( !strncmp(arr[7], "IP=", 3) );
              strcpy_s( ci.IP, arr[7]+3 );
              ASSD( !strncmp(arr[8], "IP2=", 4) );
              strcpy_s( ci.IP2, arr[8]+4 );
              ci.chFlags = ch.flags;
              m_control2name[p] = ci;
            }
          case  SChnInfo::eMOD:
            if( strcmp(arr[7], "OFFSET=0") )
            {
              if( strcmp(arr[8], "OFFSET=0") )
              {
                if( strcmp(arr[9], "OFFSET=0") )
                {
                  if( strcmp(arr[10], "OFFSET=0") )
                  {
                    ASS( !strcmp(arr[12], "OFFSET=0") );
                  }
                }
              }
            }
            ASS( 1==ch.ID || 0==ch.ID || 3==ch.ID || 4==ch.ID );
            m_chn2name[p] = ch;
            break;
          case SChnInfo::eSMPKSETH:
            //ASS( !strcmp(arr[8], "FSC_Type=1") );
            {
              bool b = false;
              for( int i=0; i<cArr; ++i )
              {
                if( !strncmp(arr[i], "FSC_Type=1", 9) )
                {
                  b = true;
                  break;
                }
              }
              ASSD( b );
            }
            ASS( 1==ch.ID || 2==ch.ID || 3==ch.ID );
            m_chn2name[p] = ch;
            break;
          default:
            ASS(0);
          case SChnInfo::eUMB:
            {
              SControlInfo ci;
              strcpy_s( ci.Name, szItemName );
              for( int i=0; i<cArr; ++i )
              {
                if( !strncmp(arr[i], "DEVTYPE=", 8) )
                {
                  strcpy_s( ci.DeviceType, arr[i]+8 );
                  continue;
                }
                if( !strncmp(arr[i], "IP=", 3) )
                {
                  strcpy_s( ci.IP, arr[i]+3 );
                  continue;
                }
                if( !strncmp(arr[i], "OFFSET=", 7) )
                {
                  ci.offset = atoi(arr[i]+7);
                  continue;
                }
              }
              ASSD( ci.IP[0] );
              ci.chFlags = ch.flags;
              m_control2name[p] = ci;
            }
          case SChnInfo::eOPC:
            break;
          case SChnInfo::eDB:
            m_chn2name[p] = ch;
            break;
          }
        }
        else if( !strncmp(ref, "LPT", 3) )
          KKK();
        else if( !strncmp(ref, "STN", 3) )
          KKK();
        else
        {
          ASS(0);
        }
        state = essfHZ;
      }
      else if( !strncmp(szLine, "DEL", 3) )
        KKK();
      else
      {
        ASS(0);
      }
      break;
    case essfHZ:
      if( !strncmp(szLine, "&Item:", 6) )
      {
        strcpy_s( szItemName, &szLine[6] );
        lenItemName = strlen(szItemName);
        state = essfItem;
        ZeroMemory( szChn, sizeof(szChn) );
      }
      else if( !strcmp(szLine, "&") )
        break;
      else
      {
        ASS(0);
      }
      break;
    }
  }
  return 0;
}

int CompQb( const void * pP1, const void * pP2 );
int KQuickBuilder::ReadPnts( LPCSTR pszPntFile )
{
  char szsLine[1024*1] = { 0 };
  char szLine[1024 * 4] = { 0 };
  FILE *file = NULL;
  if( fopen_s( &file, pszPntFile, "rt" ) )
    return -1;

  KAutoCloser<FILE*> fc( file, fclose );

  typedef vector<KQbBase*> tPointsCont_;
  tPointsCont_ points;

  KQbBase* pCurPoint = NULL;

  char szItemName[64*4] = { 0 };
  size_t lenItemName = 0;
  EStateSelectField state = essfHZ;
  for( int nLine = 1; fgets( szsLine, _countof(szsLine)-1, file); ++nLine )
  {
    cross::string::convert_to_utf8(1251, szsLine, szLine, sizeof(szLine));
    //OutputDebugString(std::to_string(nLine).c_str());
    //OutputDebugString("\n");
    //OutputDebugString(szLine);
    if( '\n'==szLine[0] || '\r'==szLine[0])
      continue;
    for( LPSTR p = szLine; *p; ++p )
    {
      switch( *p )
      {
      case '\r':
      case '\n':
        *p = 0;
        break;
      }
    }

    size_t l = strlen(szLine);
    ASS( l < (_countof(szLine)-1) );
    switch( state )
    {
    case essfHZ:
      if( !strncmp(szLine, "&Item:", 6) )
      {
        ASS( '\t'==szLine[6] );
        strcpy_s( szItemName, &szLine[7] );
        for( LPSTR p = szItemName; *p; ++p )
        {
          switch( *p )
          {
          case ' ':
          case '\r':
          case '\n':
            *p = 0;
            break;
          }
        }
        lenItemName = strlen(szItemName);
        state = essfItem;
      }
      else
      {
        ASS(0);
      }
      break;
    case essfItem:
      ASS( !strncmp(szLine, "DEL", 3) );
      for( LPSTR p = szLine+3; *p; ++p )
      {
        if( ' '!=*p && '\t'!=*p )
        {
          state = essfDEL;
          ASS( !strncmp(p, szItemName, lenItemName) );
          break;
        }
      }
      break;
    case essfDEL:
      ASS( !strncmp(szLine, "ADD", 3) );
      szLine[3] = 0;
      for( LPSTR p = szLine+4; *p; ++p )
      {
        if( ' '!=*p && '\t'!=*p )
        {
          state = essfADD;
          ASS( !strncmp(p, szItemName, lenItemName) );
          p += lenItemName;
          ASS( ' '==*p );
          p++;
          if( !strncmp(p,"STA00000", 8) )
          {
            m_FieldHandler = &KQuickBuilder::OnStatePntField;
            pCurPoint = SQB_STATUS::Create( szItemName );
          }
          else if( !strncmp(p,"ANA00000", 8) )
          {
            m_FieldHandler = &KQuickBuilder::OnAnalogPntField;
            pCurPoint = SQB_ANALOG::Create( szItemName );
          }
          else if( !strncmp(p,"CON00000", 8) )
          {
            m_FieldHandler = &KQuickBuilder::OnContainerField;
            pCurPoint = NULL;
            state = essfHandler;
            break;
          }
          else if( !strncmp(p,"PSA00000", 8) )
          {
            m_FieldHandler = &KQuickBuilder::OnPsaPntField;
            pCurPoint = SQB_PSA::Create( szItemName );
            // state = essIgnore;
            // break;
          }
          else
          {
            ASSD(0);
            return -2;
          }
          p += 8;
          state = essfHandler;
          ASS( ' '==*p );
          p++;
          ASSD( m_FieldHandler );
          points.push_back( pCurPoint );
          //OutputDebugString("Read point ");
          //OutputDebugString(pCurPoint->PointName.c_str());
          //OutputDebugString("\n");
          pCurPoint->SetParmList();
          (this->*m_FieldHandler)( pCurPoint, szLine, p, szItemName, &points );
          _Ktoupper( szItemName );
          pCurPoint->SetValue( "TAGNAME", szItemName );
          break;
        }
      }
      break;
    case essIgnore:
      if( '\n'==szLine[0] )
        break;
      if( '&'==szLine[0] )
      {
        state = essfHZ;
        m_FieldHandler = NULL;
        pCurPoint = NULL;
        break;
      }
      break;
    case essfHandler:
      if( '\n'==szLine[0] )
        break;
      if( '&'==szLine[0] )
      {
        state = essfHZ;
        m_FieldHandler = NULL;
        pCurPoint = NULL;
        break;
      }
      {
        LPSTR pszField = szLine;
        LPSTR pszValue = NULL;
        LPSTR p = szLine;
        for( p; *p; ++p )
        {
          if( ' '==*p || '\t'==*p )
          {
            *p = 0;
            ++p;
            for( p; *p; ++p )
            {
              if( ' '==*p || '\t'==*p )
                continue;
              break;
            }
            break;
          }
        }
        for( p; *p; ++p )
        {
          if( ' '==*p || '\t'==*p )
            continue;
          ASS( !_strnicmp(p, szItemName, lenItemName) );
          p += lenItemName;
          ASSD( ' '==*p || '\t'==*p );
          p++;
          break;
        }
        for( p; *p; ++p )
        {
          if( ' '==*p || '\t'==*p )
            continue;
          pszValue = p;
          break;
        }
        ASSD( m_FieldHandler );
        (this->*m_FieldHandler)( pCurPoint, pszField, pszValue, szItemName, &points );
      }
      break;
    default:
      ASS(0);
    }
  }

  if( !points.empty() )
  {
    size_t s = sizeof(KQbBase*)*points.size();
    m_ppPoints = (KQbBase**)KMemoryServiceQB5xx::Instance().NewMem( s );
    m_nPoints = (DWORD)points.size();
    memcpy( m_ppPoints, &points[0], s );

    qsort( m_ppPoints, m_nPoints, sizeof( KQbBase* ), CompQb );
  }
  return 0;
}

typedef void(KQuickBuilder::*fnHandler)(KQbBase*, LPSTR, LPCSTR );

struct SValueHandler
{
  LPCSTR pszName;
  fnHandler handler;
  LPCSTR pszFuncDet;
};

void KQuickBuilder::FuncDet_Handler(KQbBase* p, LPSTR pszValue, LPCSTR pszFuncDet )
{
  if( !pszFuncDet || !*pszFuncDet )
    return;
  p->SetValue( pszFuncDet, pszValue );
}

void KQuickBuilder::ADD_Handler(KQbBase* p, LPSTR pszValue, LPCSTR pszFuncDet )
{
  p->SetValue( "DESC", pszValue );
}

static LPCSTR s_S_UNITS[] = { "UNITS", "UNITS1", "UNITS2","UNITS3","UNITS4","UNITS5","UNITS6","UNITS7"};
void KQuickBuilder::STATEDES_Handler(KQbBase* p, LPSTR pszValue, LPCSTR pszFuncDet )
{
  int n = 0;
  char buf[32 * 2] = {};
  char arr[16][16 * 2] = {};
  const int C = SplitString( pszValue, arr, ' ', false );
  ASSD( C <= _countof(s_S_UNITS) );
  for( int n=0; n<C; ++n )
  {
    sprintf_s( buf, "TARGETPVFOROPSTATE%d", n );
    p->SetValue( buf, arr[n] );
    sprintf_s( buf, "STATEDES%d", n );
    p->SetValue( buf, arr[n] );
    p->SetValue( s_S_UNITS[n], arr[n] );
  }
}

void KQuickBuilder::TARGET_Handler(KQbBase* p, LPSTR pszValue, LPCSTR pszFuncDet )
{
    char buf[16][16 * 4] = {};
  char name[16 * 4] = {};
  int C = SplitString( pszValue, buf, ' ', false );
  ASSD( C < 9 );
  for( int i=0; i<C; ++i )
  {
    sprintf_s( name, "OPSTATE%d", i );
    if( 'F'==buf[i][0] )
      sprintf_s( buf[i], "%d", -'F' );
    p->SetValue( name, buf[i] );
  }
}

void KQuickBuilder::SOURCE_Handler(KQbBase* p, LPSTR pszValue, LPCSTR pszFuncDet )
{
  if( !*pszValue )
    return;
  char buf[10][64*4] = { 0 };
  int C = SplitString( pszValue, buf, ' ', false );
  ASSD( C>=2 );

  //if( !strcmp(p->PointName, "") )
  
  char addr[64*4] = { 0 };
  char addr2[64*4] = { 0 };
  char name[16*4] = { 0 };
  sprintf_s( name, "%sSOURCE", pszFuncDet );

  sprintf_s( addr, "??? %s", pszValue );
  p->SetValue( name, addr );

  tItemChnCont_::iterator it = FilterForChn( p, pszFuncDet, buf[0], 'I' );
  if( m_chn2name.end()!=it )
  {
    SChnInfo &ch = it->second;
    if( SChnInfo::eSMPKSETH==ch.flags )
    {
      p->SetSourceAddressSM( ch.chn.c_str(), buf[1], pszFuncDet );
      sprintf_s( addr, "%s %s", ch.chn.c_str(), buf[1] );
    }
    else if( SChnInfo::eMOD==ch.flags )
    {
      p->SetSourceAddressMOD( ch.chn.c_str(), buf[1], buf[2], ch.ID, pszFuncDet );
      sprintf_s( addr, "%s %s %s %d", ch.chn.c_str(), buf[1], buf[2], ch.ID );
    }
    else if( SChnInfo::eRSLOGIX_MOD==ch.flags && 3==C )
    {
      tItemContrlCont_::iterator itC = m_control2name.find( buf[0] );
      ASSD( m_control2name.end()!=itC );

      SControlInfo &ctrl = itC->second;

      for( LPCSTR t=buf[1]; *t; ++t )
      {
        ASSD( isdigit(*t) );
      }
      bool bError = false;
      bool bUseIP2 = false;
      int off = atoi(buf[1]);
      int bitOff = 0;
      int bit = -1;
      ASSD( off > 0 );
      if( off > 30000 )
      {
        int nf = off - 28501-1900;
        bitOff = nf / 16;
        bit = nf % 16;
        bitOff += 1901;
      }
      else if( off > 5000 )
      {
        off -= 5000;
        bUseIP2 = true;
      }
      else if( off > 3000 )
      {
        ASSD(0);
      }
      sprintf_s( addr, "%s[%d]", bUseIP2 ? ctrl.IP2 : ctrl.IP, off-1 );
      if( !strcmp(buf[2],"REAL") )
        sprintf_s( addr2, "%s[%d]", bUseIP2 ? ctrl.IP2 : ctrl.IP, off );
      else if( !strcmp(buf[2],"INT2") )
      {}
      else if( !strcmp(buf[2],"0") )
      {
        ASSD( bit >=0 && bit <=15 );
        ASSD( W_QBSTATUS::TypeID==p->QB_Type )
        sprintf_s( addr, "%s[%d].%d", ctrl.IP, bitOff-1, bit );
      }
      else if( !strcmp(buf[2],"INT4") )
        sprintf_s( addr2, "%s[%d]", bUseIP2 ? ctrl.IP2 : ctrl.IP, off );
      else
      {
        if( W_QBANALOG::TypeID==p->QB_Type )
        {
          ASSD(0);
        }
        bError = true;
      }

      if( !bError )
      {
        p->SetSourceAddressRsLogixMod( addr, addr2, buf[2], pszFuncDet );
        sprintf_s( addr, "%s %s", addr, buf[2] );
      }
    }
    p->SetValue( name, addr );
    return;
  }
  tItemContrlCont_::iterator itC = m_control2name.find( buf[0] );
  if( m_control2name.end()==itC )
    return;
  SControlInfo &ctrl = itC->second;

  sprintf_s( addr, "??? (%s) %s", ctrl.Name, pszValue );
  p->SetValue( name, addr );

  ASSD( SChnInfo::eUMB==ctrl.chFlags );
  switch( buf[1][0] )
  {
  case 0:
    ASSD( !strcmp(ctrl.DeviceType,"HC900") );
    break;
  case '1':
  case '4':
    ASSD( !strcmp(ctrl.DeviceType,"HC900") );
    ASSD( ':'==buf[1][1]);
    _Ktoupper( buf[1]+4 );
    sprintf_s( addr, "%s:%s", ctrl.IP, buf[1]+2 );
    p->SetSourceAddressHC900_UMB( addr, pszFuncDet );
    sprintf_s( addr, "(%s)%s:%s", ctrl.Name, ctrl.IP, buf[1]+2 );
    p->SetValue( name, addr );
    break;
  case 'T':
    ASSD( !strcmp(ctrl.DeviceType,"HC900") );
    ASSD( !strcmp(buf[3],"VALUE") );
    ASSD( !strcmp(buf[1],"TAG") );
    sprintf_s( addr, "%s:%s%s", ctrl.IP, buf[1], buf[2] );
    p->SetSourceAddressHC900_UMB( addr, pszFuncDet );
    sprintf_s( addr, "(%s)%s:%s %s %s", ctrl.Name, ctrl.IP, buf[1], buf[2], buf[3] );
    p->SetValue( name, addr );
    break;
  case 'M':
    ASSD( !strcmp(ctrl.DeviceType,"HC900") );
    ASSD( !strcmp(buf[3],"VALUE") );
    ASSD( !strcmp(buf[1],"MATH_VAR") );
    sprintf_s( addr, "%s:%s%s", ctrl.IP, buf[1], buf[2] );
    p->SetSourceAddressHC900_UMB( addr, pszFuncDet );
    sprintf_s( addr, "(%s)%s:%s %s %s", ctrl.Name, ctrl.IP, buf[1], buf[2], buf[3] );
    p->SetValue( name, addr );
    break;
  case 'L':
    break;
  case 'S':
    break;
  case 'F':
    break;
  default:
    ASSD(0);
    break;
  }
}

void KQuickBuilder::DESTIN_Handler(KQbBase* p, LPSTR pszValue, LPCSTR pszFuncDet )
{
  if( !*pszValue )
    return;
  char buf[5][64*4] = { 0 };
  char addr[64*4] = { 0 };
  char addr2[64*4] = { 0 };
  char name[16*4] = { 0 };
  sprintf_s( name, "%sDESTINATION", pszFuncDet );

  int C = SplitString( pszValue, buf, ' ', false );
  ASSD( C>=2 );
  tItemChnCont_::iterator it = FilterForChn( p, pszFuncDet, buf[0], 'O' );
  if( m_chn2name.end()!=it )
  {
    SChnInfo ch = it->second;
    if( SChnInfo::eSMPKSETH==ch.flags )
    {
      p->SetDestinctAddressSM( ch.chn.c_str(), buf[1], pszFuncDet );
      sprintf_s( addr, "%s %s", ch.chn.c_str(), buf[1] );
    }
    else if( SChnInfo::eMOD==ch.flags )
    {
      p->SetDestinctAddressMOD( ch.chn.c_str(), buf[1], buf[2], ch.ID, pszFuncDet );
      sprintf_s( addr, "%s %s %s %d", ch.chn.c_str(), buf[1], buf[2], ch.ID );
    }
    else if( SChnInfo::eRSLOGIX_MOD==ch.flags && 3==C )
    {
      tItemContrlCont_::iterator itC = m_control2name.find( buf[0] );
      ASSD( m_control2name.end()!=itC );

      SControlInfo &ctrl = itC->second;

      for( LPCSTR t=buf[1]; *t; ++t )
      {
        ASSD( isdigit(*t) );
      }
      bool bError = false;
      bool bUseIP2 = false;
      int off = atoi(buf[1]);
      ASSD( off > 0 );
      if( off > 30000 )
        KKK();
      if( off > 5000 )
      {
        off -= 5000;
        bUseIP2 = true;
      }
      else if( off > 3000 )
      {
        ASSD(0);
      }
      sprintf_s( addr, "%s[%d]", bUseIP2 ? ctrl.IP2 : ctrl.IP, off-1 );
      if( !strcmp(buf[2],"REAL") )
        sprintf_s( addr2, "%s[%d]", bUseIP2 ? ctrl.IP2 : ctrl.IP, off );
      else if( !strcmp(buf[2],"INT2") )
      {}
      else if( !strcmp(buf[2],"INT4") )
        sprintf_s( addr2, "%s[%d]", bUseIP2 ? ctrl.IP2 : ctrl.IP, off );
      else
      {
        if( W_QBANALOG::TypeID==p->QB_Type )
        {
          ASSD(0);
        }
        bError = true;
      }

      if( !bError )
      {
        p->SetDestinctAddressRsLogixMod( addr, addr2, buf[2], pszFuncDet );
        sprintf_s( addr, "%s %s", addr, buf[2] );
      }
    }
    p->SetValue( name, addr );
    return;
  }
  tItemContrlCont_::iterator itC = m_control2name.find( buf[0] );
  if( m_control2name.end()==itC )
    return;
  SControlInfo &ctrl = itC->second;
  ASSD( SChnInfo::eUMB==ctrl.chFlags );
  switch( buf[1][0] )
  {
  case '1':
  case '4':
    ASSD( !strcmp(ctrl.DeviceType,"HC900") );
    ASSD( ':'==buf[1][1]);
    _Ktoupper( buf[1]+4 );
    sprintf_s( addr, "%s:%s", ctrl.IP, buf[1]+2 );
    p->SetDestinctAddressHC900_UMB( addr, pszFuncDet );
    sprintf_s( addr, "(%s)%s:%s", ctrl.Name, ctrl.IP, buf[1]+2 );
    p->SetValue( name, addr );
    break;
  case 'T':
    ASSD( !strcmp(ctrl.DeviceType,"HC900") );
    ASSD( !strcmp(buf[3],"VALUE") );
    ASSD( !strcmp(buf[1],"TAG") );
    sprintf_s( addr, "%s:%s%s", ctrl.IP, buf[1], buf[2] );
    p->SetDestinctAddressHC900_UMB( addr, pszFuncDet );
    sprintf_s( addr, "(%s)%s:%s %s %s", ctrl.Name, ctrl.IP, buf[1], buf[2], buf[3] );
    p->SetValue( name, addr );
    break;
  case 'M':
    ASSD( !strcmp(ctrl.DeviceType,"HC900") );
    ASSD( !strcmp(buf[3],"VALUE") );
    ASSD( !strcmp(buf[1],"MATH_VAR") );
    sprintf_s( addr, "%s:%s%s", ctrl.IP, buf[1], buf[2] );
    p->SetDestinctAddressHC900_UMB( addr, pszFuncDet );
    sprintf_s( addr, "(%s)%s:%s %s %s", ctrl.Name, ctrl.IP, buf[1], buf[2], buf[3] );
    p->SetValue( name, addr );
    break;
  case 'L':
    break;
  case 'S':
    break;
  case 'F':
    break;
  default:
    ASSD(0);
    return;
  }
}

void KQuickBuilder::RANGE_S_Handler(KQbBase* p, LPSTR pszValue, LPCSTR pszFuncDet )
{
  LPCSTR pFields[] = { "RANGEMIN", "RANGEMAX" };
  char buf[2][16 * 4] = {};
  const int C = SplitString( pszValue, buf, ' ', false );
  ASSD( C <= _countof(buf) );
  for( int i=0; i<C; ++i )
    p->SetValue( pFields[i], buf[i] );
}

void KQuickBuilder::ALARM_S_Handler(KQbBase* p, LPSTR pszValue, LPCSTR pszFuncDet )
{
    char szBuf[12][12 * 4] = {};
  int C = SplitString( pszValue, szBuf, ' ', false );
  char val[2] = { 0, 0 };
  char tmp[32*4];
  int k = 0;
  for ( int i=0; i<C; ++i )
  {
    if ( strlen(szBuf[i])>7 )
	{
	  k = i;
	  break;
	}
  }
  ASSD(k);
  for( size_t i=0, e(strlen(szBuf[k])); i<e; ++i )
  {
    val[0] = szBuf[k][i];
    sprintf_s( tmp, "AL%dARMED", i+1 );
    p->SetValue(tmp,val);
  }
  int index = 4;
  if( !isdigit(szBuf[index][0]) )
    ++index;
  ASSD( isdigit(szBuf[index][0]) );
  int J = 1;
  for( int i=index; i<C; ++i, ++J )
  {
    sprintf_s( tmp, "AL%dPRI", J );
    p->SetValue(tmp,szBuf[i]);
    sprintf_s( tmp, "STATE%dALARMPRIORITY", J-1 );
    p->SetValue(tmp,szBuf[i]);
  }
  p->SetValue( "CONTROLFAILALARMPRIORITY", szBuf[1] );
}

static LPCSTR s_ALMXCHG_Ss[] = { "PVEXTERNALCHANGEALARMENABLE","OPEXTERNALCHANGEALARMENABLE","MDEXTERNALCHANGEALARMENABLE"};
void KQuickBuilder::ALMXCHG_S_Handler(KQbBase* p, LPSTR pszValue, LPCSTR pszFuncDet )
{
  char val[2] = { 0, 0 };
  for( size_t i=0, e(strlen(pszValue)); i<e; ++i )
  {
    val[0] = pszValue[i];
    p->SetValue(s_ALMXCHG_Ss[i],val);
  }
}

static SValueHandler s_StatesFields[] = {
  { "ADD", &KQuickBuilder::ADD_Handler, NULL },
  { "ENTNAM", &KQuickBuilder::FuncDet_Handler, "FULLITEMNAME" },
  { "RANGE", &KQuickBuilder::RANGE_S_Handler, NULL },	
  { "DISPLAY", &KQuickBuilder::FuncDet_Handler, "ASSOCIATEDDISPLAY" },
  { "ALMMSG", &KQuickBuilder::FuncDet_Handler, "ALARMMESSAGEINDEX" },
  { "PNTDTLPG", NULL, NULL },
  { "GRPDTLPG", NULL, NULL },
  { "AREA", NULL, NULL },
  { "OPWIDTH", &KQuickBuilder::FuncDet_Handler, "OPWIDTH" },
  { "PVSOURCE", &KQuickBuilder::SOURCE_Handler, "PV" },
  { "PVPERIOD", &KQuickBuilder::FuncDet_Handler, "PVPERIOD" },
  { "HISTSLOW", NULL, NULL },
  { "HISTFAST", NULL, NULL },
  { "HISTEXTD", NULL, NULL },
  { "OPSOURCE", &KQuickBuilder::SOURCE_Handler, "OP" },
  { "OPPERIOD", &KQuickBuilder::FuncDet_Handler, "OPPERIOD" },
  { "OPDESTIN", &KQuickBuilder::DESTIN_Handler, "OP" },
  { "ENTERZON", NULL, NULL },
  { "MDSOURCE", &KQuickBuilder::SOURCE_Handler, "MD" },
  { "MDPERIOD", &KQuickBuilder::FuncDet_Handler, "MDPERIOD" },
  { "MDDESTIN", &KQuickBuilder::DESTIN_Handler, "MD" },
  { "STATEDES", &KQuickBuilder::STATEDES_Handler, NULL },
  { "ALARM", &KQuickBuilder::ALARM_S_Handler, NULL },
  { "ALMXCHG", &KQuickBuilder::ALMXCHG_S_Handler, NULL },
  { "OPPULSE", &KQuickBuilder::FuncDet_Handler, "PULSEWIDTH" },
  { "TARGET", &KQuickBuilder::TARGET_Handler, NULL },
  { "MDNORMAL", &KQuickBuilder::FuncDet_Handler, "NORMALMODE" },
  { "MDDISABL", &KQuickBuilder::FuncDet_Handler, "MODECHECKDISABLED" },
  { "AKDESTIN", NULL, NULL },
  { "CNTRLTO", NULL, NULL },	
  { "CNTRLLVL", &KQuickBuilder::FuncDet_Handler, "CONTROLLEVEL" },
  { "REVERSE", NULL, NULL },	
  { "CCONFIRM", NULL, NULL },
  { "ALMINH", NULL, NULL },
  { "ONSCAN", &KQuickBuilder::FuncDet_Handler, "ONSCAN" },
  { "PARAM", &KQuickBuilder::PARAM_Handler, NULL },

  { "PVREVERS", NULL, NULL },
  { "PVDYNSCN", NULL, NULL },
  { "OPREVERS", NULL, NULL },
  { "OPDYNSCN", NULL, NULL },
  { "MDDYNSCN", NULL, NULL },
  { "S0ONDLY", NULL, NULL },
  { "S1ONDLY", NULL, NULL },
  { "S2ONDLY", NULL, NULL },
  { "S3ONDLY", NULL, NULL },
  { "S4ONDLY", NULL, NULL },
  { "S5ONDLY", NULL, NULL },
  { "S6ONDLY", NULL, NULL },
  { "S7ONDLY", NULL, NULL },
  { "S0OFFDLY", NULL, NULL },
  { "S1OFFDLY", NULL, NULL },
  { "S2OFFDLY", NULL, NULL },
  { "S3OFFDLY", NULL, NULL },
  { "S4OFFDLY", NULL, NULL },
  { "S5OFFDLY", NULL, NULL },
  { "S6OFFDLY", NULL, NULL },
  { "S7OFFDLY", NULL, NULL },
  { "JNLONLY", NULL, NULL },

  { "PVALGO", NULL, NULL },
  { "ALG(01)", NULL, NULL },
  { "ALG(02)", NULL, NULL },
  { "ALG(03)", NULL, NULL },
  { "ALG(04)", NULL, NULL },
  { "ALG(05)", NULL, NULL },
  { "ALG(06)", NULL, NULL },
  { "ALG(07)", NULL, NULL },
  { "ALG(08)", NULL, NULL },
  { "ALG(09)", NULL, NULL },
  { "ALG(10)", NULL, NULL },
  { "ALG(11)", NULL, NULL },
  { "ALG(12)", NULL, NULL },
  { "ALG(13)", NULL, NULL },
  { "ALG(14)", NULL, NULL },
  { "ALG(15)", NULL, NULL },
  { "ALG(16)", NULL, NULL },

  { "SCRIPT", &KQuickBuilder::SCRIPT_Handler, NULL },
};

void KQuickBuilder::OnStatePntField( KQbBase* pPoint, LPCSTR pszName, LPSTR pszValue, LPSTR szItemName, tPointsCont_ *pPntCont)
{
  if( !pszValue )
    return;
  for( int i=0; i<_countof(s_StatesFields); ++i )
  {
    if( !strcmp(pszName, s_StatesFields[i].pszName) )
    {
      if( s_StatesFields[i].handler )
        (this->*s_StatesFields[i].handler)( pPoint, pszValue, s_StatesFields[i].pszFuncDet );
      return;
    }
  }
  ASSD(0);
}

void KQuickBuilder::RANGE_A_Handler(KQbBase* p, LPSTR pszValue, LPCSTR pszFuncDet )
{
  LPCSTR pFields[] = { "RANGEAT0", "RANGEAT100", "UNITS" };
  char buf[3][16 * 4] = {};
  const int C = SplitString( pszValue, buf, ' ', false );
  ASSD( C <= _countof(buf) );
  for( int i=0; i<C; ++i )
    p->SetValue( pFields[i], buf[i] );
}

void KQuickBuilder::PVALGO_Handler(KQbBase* p, LPSTR pszValue, LPCSTR pszFuncDet )
{
    char szBuf[5][12 * 4] = {};
  int C = SplitString( pszValue, szBuf, ' ', false );
  ASS( 2==C );
  p->SetValue( "PVALGONUMBER", szBuf[0], szBuf[1] );
}

void KQuickBuilder::ACTALGO_Handler(KQbBase* p, LPSTR pszValue, LPCSTR pszFuncDet )
{
    char szBuf[5][12 * 4] = {};
  int C = SplitString( pszValue, szBuf, ' ', false );
  ASS( 2==C );
  p->SetValue( "ACTIONALGONUMBER", szBuf[0], szBuf[1] );
}

void KQuickBuilder::ALG_Handler(KQbBase* p, LPSTR pszValue, LPCSTR pszFuncDet )
{
  if( !pszFuncDet || !*pszFuncDet )
    return;
  p->SetValue( pszFuncDet, pszValue );
}

void KQuickBuilder::LIMIT_Handler(KQbBase* p, LPSTR pszValue, LPCSTR pszFuncDet )
{
    char szBuf[2][12 * 4] = {};
  char name[20*4] = { 0 };
  int C = SplitString( pszValue, szBuf, ' ', false );
  ASS( 2==C );
  sprintf_s( name, "%sLOWLIMIT", pszFuncDet );
  p->SetValue( name, szBuf[0] );
  sprintf_s( name, "%sHIGHLIMIT", pszFuncDet );
  p->SetValue( name, szBuf[1] );
}

void KQuickBuilder::ALMLIM_Handler(KQbBase* p, LPSTR pszValue, LPCSTR pszFuncDet )
{
    char szBuf[3][12 * 4] = {};
  char name[20*4] = { 0 };
  int C = SplitString( pszValue, szBuf, ' ', false );
  ASS( 3==C );
  sprintf_s( name, "AL%s", pszFuncDet );
  p->SetValue( name, szBuf[0] );
  sprintf_s( name, "ALARMTYPE%s", pszFuncDet );
  p->SetValue( name, szBuf[1] );
  if( (pszFuncDet[0]-'0') > 4 )
    sprintf_s( name, "AL%sPRI", pszFuncDet );
  else
    sprintf_s( name, "ALARM%sPRIORITY", pszFuncDet );
  p->SetValue( name, szBuf[2] );
}

void KQuickBuilder::ALARM_A_Handler(KQbBase* p, LPSTR pszValue, LPCSTR pszFuncDet )
{
    char szBuf[4][12 * 4] = {};
  int C = SplitString( pszValue, szBuf, ' ', false );
  ASS( 4==C );
  p->SetValue( "CONTROLFAILALARMSUBPRIORITY", szBuf[0] );
  p->SetValue( "CONTROLFAILALARMPRIORITY", szBuf[1] );
}

static LPCSTR s_ALMXCHG_As[] = { "PVEXTERNALCHANGEALARMENABLE","SPEXTERNALCHANGEALARMENABLE"
                                ,"OPEXTERNALCHANGEALARMENABLE","MDEXTERNALCHANGEALARMENABLE"
                                ,"A1EXTERNALCHANGEALARMENABLE","A2EXTERNALCHANGEALARMENABLE"
                                ,"A3EXTERNALCHANGEALARMENABLE","A4EXTERNALCHANGEALARMENABLE"};
void KQuickBuilder::ALMXCHG_A_Handler(KQbBase* p, LPSTR pszValue, LPCSTR pszFuncDet )
{
  char val[2] = { 0, 0 };
  for( size_t i=0, e(strlen(pszValue)); i<e; ++i )
  {
    val[0] = pszValue[i];
    p->SetValue(s_ALMXCHG_As[i],val);
  }
}

void KQuickBuilder::PARAM_Handler(KQbBase* p, LPSTR pszValue, LPCSTR pszFuncDet )
{
  if( !strcmp(p->PointName,"OMACS") )
    return;
  p->AddUserDefined( pszValue );
}

void KQuickBuilder::SCRIPT_Handler(KQbBase* p, LPSTR pszValue, LPCSTR pszFuncDet )
{
  if( !strcmp(pszValue,"BEGINSCRIPT") )
  {
    m_scriptXML.clear();
  }
  else if( !strcmp(pszValue,"ENDSCRIPT") )
  {
    if( p->AddScript( m_scriptXML.c_str() ) )
    {
#ifdef _DEBUG1
      char t[_MAX_PATH];
      _makepath_s( t, NULL, rsuGetProjectPath(), "QB", NULL );
      KDirCreator dc( t, _countof(t), m_szObjName );
      KFileCreator fc( t, _countof(t), p->PointName, "xml" );
      FILE *file = NULL;
      if( 0==fopen_s( &file, t, "wb" ) )
      {
        KAutoCloser<FILE*> afc( file, fclose );
        fputs( m_scriptXML.c_str(), file );
      }
#endif
    }
    m_scriptXML.clear();
  }
  else
  {
    m_scriptXML.append( pszValue );
    m_scriptXML.append( "\n" );
  }
}

static SValueHandler s_AnalogsFields[] = {
  { "ADD", &KQuickBuilder::ADD_Handler, NULL },
  { "ENTNAM", &KQuickBuilder::FuncDet_Handler, "FULLITEMNAME" },
  { "RANGE", &KQuickBuilder::RANGE_A_Handler, NULL },
  { "DISPLAY", &KQuickBuilder::FuncDet_Handler, "ASSOCIATEDDISPLAY" },
  { "ALMMSG", &KQuickBuilder::FuncDet_Handler, "ALARMMESSAGEINDEX" },
  { "PNTDTLPG", NULL, NULL },
  { "GRPDTLPG", &KQuickBuilder::FuncDet_Handler, "INSTRUCTIONDISPLAY" },
  { "AREA", NULL, NULL },
  { "A1NAME", NULL, NULL },
  { "A2NAME", NULL, NULL },
  { "A3NAME", NULL, NULL },
  { "A4NAME", NULL, NULL },
  { "PVSOURCE", &KQuickBuilder::SOURCE_Handler, "PV" },
  { "PVPERIOD", &KQuickBuilder::FuncDet_Handler, "PVPERIOD" },
  { "HISTSLOW", NULL, NULL },
  { "HISTFAST", NULL, NULL },
  { "HISTEXTD", NULL, NULL },
  { "OPSOURCE", &KQuickBuilder::SOURCE_Handler, "OP" },
  { "OPPERIOD", &KQuickBuilder::FuncDet_Handler, "OPPERIOD" },
  { "OPDESTIN", &KQuickBuilder::DESTIN_Handler, "OP" },
  { "MDSOURCE", &KQuickBuilder::SOURCE_Handler, "MD" },
  { "MDPERIOD", &KQuickBuilder::FuncDet_Handler, "MDPERIOD" },
  { "MDDESTIN", &KQuickBuilder::DESTIN_Handler, "MD" },
  { "SPSOURCE", &KQuickBuilder::SOURCE_Handler, "SP" },
  { "SPPERIOD", &KQuickBuilder::FuncDet_Handler, "SPPERIOD" },
  { "SPDESTIN", &KQuickBuilder::DESTIN_Handler, "SP" },
  { "A1SOURCE", &KQuickBuilder::SOURCE_Handler, "A1" },
  { "A1PERIOD", NULL, NULL },
  { "A1DESTIN", &KQuickBuilder::DESTIN_Handler, "A1" },
  { "A2SOURCE", &KQuickBuilder::SOURCE_Handler, "A2" },
  { "A2PERIOD", NULL, NULL },
  { "A2DESTIN", &KQuickBuilder::DESTIN_Handler, "A2" },
  { "A3SOURCE", &KQuickBuilder::SOURCE_Handler, "A3" },
  { "A3PERIOD", NULL, NULL },
  { "A3DESTIN", &KQuickBuilder::DESTIN_Handler, "A3" },
  { "A4SOURCE", &KQuickBuilder::SOURCE_Handler, "A4" },
  { "A4PERIOD", NULL, NULL },
  { "A4DESTIN", &KQuickBuilder::DESTIN_Handler, "A4" },
  { "ALARM", &KQuickBuilder::ALARM_A_Handler, NULL },
  { "ALMLIM1", &KQuickBuilder::ALMLIM_Handler, "1" },
  { "ALMLIM2", &KQuickBuilder::ALMLIM_Handler, "2" },
  { "ALMLIM3", &KQuickBuilder::ALMLIM_Handler, "3" },
  { "ALMLIM4", &KQuickBuilder::ALMLIM_Handler, "4" },
  { "ALMLIM5", &KQuickBuilder::ALMLIM_Handler, "5" },
  { "ALMLIM6", &KQuickBuilder::ALMLIM_Handler, "6" },
  { "ALMLIM7", &KQuickBuilder::ALMLIM_Handler, "7" },
  { "ALMLIM8", &KQuickBuilder::ALMLIM_Handler, "8" },
  { "ALMXCHG", &KQuickBuilder::ALMXCHG_A_Handler, NULL },
  { "DRIFTDB", &KQuickBuilder::FuncDet_Handler, "DRIFTDEADBANDPERCENT" },
  { "ALARMDB", NULL, NULL },
  { "CNTRLDB", NULL, NULL },
  { "PVCLAMP", NULL, NULL },
  { "MDNORMAL", &KQuickBuilder::FuncDet_Handler, "NORMALMODE" },
  { "MDDISABL", &KQuickBuilder::FuncDet_Handler, "MODECHECKDISABLED" },
  { "CNTRLTO", NULL, NULL },
  { "CNTRLLVL", &KQuickBuilder::FuncDet_Handler, "CONTROLLEVEL" },
  { "REVERSE", NULL, NULL },
  { "CCONFIRM", NULL, NULL },
  { "ONSCAN", NULL, NULL },
  { "OPLIMIT", &KQuickBuilder::LIMIT_Handler, "OUTPUT" },
  { "SPLIMIT", &KQuickBuilder::LIMIT_Handler, "SETPOINT" },
  { "ALMINH", NULL, NULL },
  { "PVALGO", &KQuickBuilder::PVALGO_Handler, NULL },
  { "ACTALGO", &KQuickBuilder::ACTALGO_Handler, NULL },
  { "ALG(01)", &KQuickBuilder::ALG_Handler, "ALG(01)" },
  { "ALG(02)", &KQuickBuilder::ALG_Handler, "ALG(02)" },
  { "ALG(03)", &KQuickBuilder::ALG_Handler, "ALG(03)" },
  { "ALG(04)", &KQuickBuilder::ALG_Handler, "ALG(04)" },
  { "ALG(05)", &KQuickBuilder::ALG_Handler, "ALG(05)" },
  { "ALG(06)", &KQuickBuilder::ALG_Handler, "ALG(06)" },
  { "ALG(07)", &KQuickBuilder::ALG_Handler, "ALG(07)" },
  { "ALG(08)", &KQuickBuilder::ALG_Handler, "ALG(08)" },
  { "ALG(09)", &KQuickBuilder::ALG_Handler, "ALG(09)" },
  { "ALG(10)", &KQuickBuilder::ALG_Handler, "ALG(10)" },
  { "ALG(11)", &KQuickBuilder::ALG_Handler, "ALG(11)" },
  { "ALG(12)", &KQuickBuilder::ALG_Handler, "ALG(12)" },
  { "ALG(13)", &KQuickBuilder::ALG_Handler, "ALG(13)" },
  { "ALG(14)", &KQuickBuilder::ALG_Handler, "ALG(14)" },
  { "ALG(15)", &KQuickBuilder::ALG_Handler, "ALG(15)" },
  { "ALG(16)", &KQuickBuilder::ALG_Handler, "ALG(16)" },
  { "ALG(19)", &KQuickBuilder::ALG_Handler, "ALG(19)" },
  { "ALG(21)", &KQuickBuilder::ALG_Handler, "ALG(21)" },
  { "PARAM", &KQuickBuilder::PARAM_Handler, NULL },
  { "SCRIPT", &KQuickBuilder::SCRIPT_Handler, NULL },
  { "TREND", NULL, NULL },
  { "GROUP", NULL, NULL },

  { "JNLONLY", NULL, NULL },
  { "PVDYNSCN", NULL, NULL },
  { "SPDYNSCN", NULL, NULL },
  { "OPDYNSCN", NULL, NULL },
  { "MDDYNSCN", NULL, NULL },
  { "A1DYNSCN", NULL, NULL },
  { "A2DYNSCN", NULL, NULL },
  { "A3DYNSCN", NULL, NULL },
  { "A4DYNSCN", NULL, NULL },
  { "GNONDLY", NULL, NULL },
  { "GNOFFDLY", NULL, NULL },
  { "A1ONDLY", NULL, NULL },
  { "A2ONDLY", NULL, NULL },
  { "A3ONDLY", NULL, NULL },
  { "A4ONDLY", NULL, NULL },
  { "A5ONDLY", NULL, NULL },
  { "A6ONDLY", NULL, NULL },
  { "A7ONDLY", NULL, NULL },
  { "A8ONDLY", NULL, NULL },
  { "A1OFFDLY", NULL, NULL },
  { "A2OFFDLY", NULL, NULL },
  { "A3OFFDLY", NULL, NULL },
  { "A4OFFDLY", NULL, NULL },
  { "A5OFFDLY", NULL, NULL },
  { "A6OFFDLY", NULL, NULL },
  { "A7OFFDLY", NULL, NULL },
  { "A8OFFDLY", NULL, NULL },
};

void KQuickBuilder::OnAnalogPntField( KQbBase* pPoint, LPCSTR pszName, LPSTR pszValue, LPSTR szItemName, tPointsCont_ *pPntCont)
{
  if( !pszValue )
    return;
  for( int i=0; i<_countof(s_AnalogsFields); ++i )
  {
    if( !strcmp(pszName, s_AnalogsFields[i].pszName) )
    {
      if( s_AnalogsFields[i].handler )
        (this->*s_AnalogsFields[i].handler)( pPoint, pszValue, s_AnalogsFields[i].pszFuncDet );
      return;
    }
  }
  ASSD(0);
}

void KQuickBuilder::OnContainerField( KQbBase* pPoint, LPCSTR pszName, LPSTR pszValue, LPSTR szItemName, tPointsCont_ *pPntCont )
{
  if( strcmp(pszName,"PARAM") )
    return;
  char szParts[4][32] = {};
  int C = SplitString( pszValue, szParts, ' ', false );
  ASSD( 3==C );
  DWORD types[] = {W_QBANALOG::TypeID, W_QBSTATUS::TypeID };
  for( DWORD i=0; i<_countof(types); ++i )
  {
    CBase *pSrc = KMemoryServiceQB5xx::Instance().FindIOsStruct( szParts[2], types[i], 0, NULL );
    if( !pSrc )
      continue;
    sprintf_s( szParts[3], "%s.%s", szItemName, szParts[0] );
    KMemoryServiceQB5xx::Instance().CreateRef( szParts[3], pSrc, 0 );
    return;
  }
  ASSD(0);
}

static SValueHandler s_PsasFields[] = {
  { "ADD", &KQuickBuilder::ADD_Handler, NULL },
  { "ENTNAM", &KQuickBuilder::FuncDet_Handler, "FULLITEMNAME" },
  { "DISPLAY", &KQuickBuilder::FuncDet_Handler, "ASSOCIATEDDISPLAY" },
  { "PNTDTLPG", NULL, NULL },
  { "PNTSRVTP", NULL, NULL },
  { "GRPDTLPG", &KQuickBuilder::FuncDet_Handler, "INSTRUCTIONDISPLAY" },
  { "AREA", NULL, NULL },
  { "PARAM", &KQuickBuilder::PARAM_Handler, NULL },
  { "SCRIPT", &KQuickBuilder::SCRIPT_Handler, NULL },
  { "HISTFAST", NULL, NULL },
};
void KQuickBuilder::OnPsaPntField( KQbBase* pPoint, LPCSTR pszName, LPSTR pszValue, LPSTR szItemName, tPointsCont_ *pPntCont )
{
  if( !pszValue )
    return;
  for( int i=0; i<_countof(s_PsasFields); ++i )
  {
    if( !strcmp(pszName, s_PsasFields[i].pszName) )
    {
      if( s_PsasFields[i].handler )
        (this->*s_PsasFields[i].handler)( pPoint, pszValue, s_PsasFields[i].pszFuncDet );
      return;
    }
  }
  ASSD(0);
}