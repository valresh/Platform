#include "Y_MainLogic.h"
#include <Y_Data.h>
#include <ConnectY.h>
#include <CommonY.h>
#include <macros/AutoCloser.h>

#include <../Y_Data/ReadParms.h>

//#include <io.h>
#include <macros/FileFind.h>
#include <rsus/rsuPaths.h>

const DWORD CY_MainLogic::s_CurVersion = 0x00000148;

CY_Base * FindYObj ( char * ObjName, int domain );
CY_Base * FindYObjN( char * ObjName, int NumbFcs, int domain );// 0 - без учета номера FCS, >0 - точное совпадение, <0 точно не с этого FCS
CY_Base * FindYObjTyped( char * ObjName, int domain, int y_type );
CY_DI * FindDObj ( char * ObjName, int NumbFcs /*= 0*/, int domain );
void GetDR ( int * kDR, struct CY_DR *** DR );
void GetAllYObj ( int * kObj, struct CY_Base *** pppObj );
void GetDYObj ( int * kDObj, struct CY_DI *** pppDObj );

#ifdef _WIN32
bool FindAndShowBlk ( HWND hWndMain, char * FCS, char * SelectedObject );
void ShowCalcuCDebugWindow( HWND hWndMain, LPSTR pszName );
#endif

CY_MainLogic * pYMainLogic = NULL;

CY_Base * CY_MainLogic::pY_Obj[MAX_YOBJ] = { NULL };
int CY_MainLogic::kY_Obj = 0;
CY_DI * CY_MainLogic::pD_Obj[MAX_DOBJ] = { NULL };
CY_DI ** CY_MainLogic::m_ppDigNumbTypeSorted = NULL;
int CY_MainLogic::kD_Obj = 0;
CObjConnection CY_MainLogic::Connection[MAX_YCONN];
int CY_MainLogic::kConnection = 0;
CY_DR * CY_MainLogic::DR[MAX_DR] = { NULL };
int CY_MainLogic::kDR = 0;


static LPCSTR s_szYName = "РСУ_Y";
static LPCSTR s_szYExt = ".csv";

int CompDiscretNumbType( const void * pP1, const void * pP2 );

_W_main::_W_main()
{
  memset ( this, 0, sizeof ( _W_main ));
}

bool CY_MainLogic::s_bFcsMultiDomain = false;
//bool CY_MainLogic::s_bScsMultiDomain = false;

CY_MainLogic::CY_MainLogic( LPCSTR _ObjName ) 
: m_timeHolder( 0 )
{
  strcpy_s( m_szObjName, _ObjName );
  pYMainLogic = this;
  pFindYObj = ::FindYObj;
  pFindYObjN = ::FindYObjN;
  pFindYObjTyped = ::FindYObjTyped;
  pFindDObj = ::FindDObj;
  //pFindAndShowYBlk = ::FindAndShowBlk;
  //pShowCalcuCDebugWindow = ::ShowCalcuCDebugWindow;
  ZeroMemory( pY_Obj, sizeof(pY_Obj) );
  ZeroMemory( pD_Obj, sizeof(pD_Obj) );
  ZeroMemory( DR, sizeof(DR) );
  KMemoryServiceY::Instance().AddStaticMemoryCentum( sizeof(pY_Obj) );
  KMemoryServiceY::Instance().AddStaticMemoryCentum( sizeof(pD_Obj) );
  KMemoryServiceY::Instance().AddStaticMemoryCentum( sizeof(DR) );
  KMemoryServiceY::Instance().AddStaticMemoryCentum( sizeof(Connection) );
  pGetDR = GetDR;
  pFindDI = CY_MainLogic::FindDI;
  pFindObjByNumb = CY_MainLogic::FindObjByNumb;
  pExecConnectionTransfer = CY_MainLogic::ExecConnectionTransfer;
  m_NumbFCS = 0;
  m_bCreateYokoData = false;

  CY_MainLogic::InitParm();
  std::filesystem::path m_szCsvFile = std::filesystem::path(rsuGetMemoryPath()) / s_szYName;
  m_szCsvFile.replace_extension(s_szYExt);

  m_pCentumSystem = NULL;
  m_szDirPrj[0] = 0;
}

CY_MainLogic::~CY_MainLogic()
{
}

LPCSTR CY_MainLogic::GetCsvPrefix()
{
  return s_szYName;
}

static LPCSTR s_szMagicFile = "Project.atr";

int CY_MainLogic::InitL()
{

  strcpy_s( m_szDirPrj, rsuGetPath2Rsu().c_str() );
  bool bFoudPrjdDir = false;
  std::filesystem::path Path = std::filesystem::path(m_szDirPrj) / s_szMagicFile;
  if( std::filesystem::exists(Path) )
  {
      std::string v_mask = std::string("*.*");
      KFileFind ff(m_szDirPrj, v_mask);

      for (const auto& s : ff.files())
      {
          Path = std::filesystem::path(s) / s_szMagicFile;
          if (std::filesystem::exists(Path))
              continue;
          bFoudPrjdDir = true;
          break;
      }
  }
  else
  {
      bFoudPrjdDir = true;
  }

  ASS( bFoudPrjdDir );
  if( !bFoudPrjdDir )
    return -1;

  size_t len = strlen( m_szDirPrj );
  if( 0==len )
    return -2;
  if( '/'!=m_szDirPrj[len-1] )
    strcat_s( m_szDirPrj, "/" );

  rsuSetPath2Rsu( m_szDirPrj );

  if( !CreateAndCheckSystemVer() )
  {
    m_pCentumSystem = NULL;
    KMemoryServiceY::Instance().ClearStructs();
    if( !CreateAndCheckSystemVer() )
    {
      MessageBox( NULL, "Дальнейшая работа невозможна.\n\nОчистите Memory.", "Критическое изменение данных", MB_OK|MB_ICONERROR );
      return 2;
    }
  }

  if( m_pCentumSystem )
  {
    m_pCentumSystem->bYinCalc = false;
    ZeroMemory( m_pCentumSystem->domains, sizeof(m_pCentumSystem->domains) );
  }
  else
    return 3;

	int Res = 0;
  if( m_bCreateYokoData )
  {
    Delete_Data_Y();
    ReadProject();
    SaveAll();
  }
  else
  {
    if( Test_Data_Y())
    {
      e_RestResult res = RestAll();
      switch( res )
      {
      case err_DatVerGreaterSrcVer:
        {
          if( IDYES!=MessageBox( NULL, "Использовать текущую версию будете на свой страх и риск.\n"
            "Продолжить?", "Устаревшая версия Centum. Обновите Y.", MB_TOPMOST|MB_YESNO|MB_ICONWARNING|MB_DEFBUTTON2) )
          {
            exit( 1 );
          }
        }
      case err_WrongKey:
      case err_WrongVer:
      case err_CantOpen:
        Delete_Data_Y();
        ReadProject();
        SaveAll();
        break;
      default:
      case err_CommonErr:
        return 1;
      case err_Ok:
        break;
      }
    }
    else
    {
      ReadProject();
      SaveAll();
    }
  }
  ObjInit( );

  if( kD_Obj )
  {
    size_t size = sizeof(CY_DI*)*CY_MainLogic::kD_Obj;
    m_ppDigNumbTypeSorted = (CY_DI**)KMemoryServiceY::Instance().NewMemCentum( size );
    CopyMemory( m_ppDigNumbTypeSorted, CY_MainLogic::pD_Obj, size );
    for( int i=0; i<CY_MainLogic::kD_Obj; ++i )
    {
      ASSD( m_ppDigNumbTypeSorted[i]->Y_Type );
      //ASSD( m_ppDigNumbTypeSorted[i]->Numb );
      if( id__RQ != m_ppDigNumbTypeSorted[i]->Y_Type )
      {
        ASSD( m_ppDigNumbTypeSorted[i]->NumbFCS );
      }
      KKK();
    }
    qsort( m_ppDigNumbTypeSorted, CY_MainLogic::kD_Obj, sizeof( CY_DI* ), CompDiscretNumbType );
  }

  return 0;
}

bool CY_MainLogic::CreateAndCheckSystemVer()
{
  CBase* pBase = KMemoryServiceY::Instance().CreateIOsStruct( "Весь Centum", CCentumSystem::TypeID, 0, 0 );
  ASS( pBase );
  if( !pBase )
    return false;

  m_pCentumSystem = (CCentumSystem*)pBase;
  if( m_pCentumSystem->size != sizeof(CCentumSystem) )
    return false;

  if( CY_MainLogic::s_CurVersion!=m_pCentumSystem->mainLogicVer && 0!=m_pCentumSystem->mainLogicVer )
    return false;

  m_pCentumSystem->mainLogicVer = CY_MainLogic::s_CurVersion;

  return true;
}

static char * Printed[100];
static int kPrinted = 0;

bool Prt ( char * Class )
  {
  for ( int n = 0; n < kPrinted; n++ )
    {
    if ( strcmp ( Printed[n], Class ) == 0 )
      return false;
    }
  Printed[kPrinted++] = Class;
  return true;
  }

void CY_MainLogic::ObjInit(  )
  {
  for ( int n = 0; n < kY_Obj; n++ )
    {
    pY_Obj[n]->Init();
    }
  }

CY_DI *  CY_MainLogic::GetDObj( char * Name, int _NumbFCS, int domain, bool bSameNumbFCS /*= true*/ )
{
  CY_DI * pObj = NULL;
  if ( strncmp ( Name, "%GS", 3 ) == 0 )
  {
    int Numb;
    int NumbFCS;
    if ( Name[6] )
    {
      Numb = atoi ( Name + 3 );
      NumbFCS = Numb % 100;
      Numb = Numb / 100;
    }
    else
    {
      Numb = atoi ( Name + 3 );
      NumbFCS = _NumbFCS;
    }
    pObj = (*pFindDI)( NumbFCS, domain, Numb, id__GS );
    if( !pObj )
    {
      OutputDebugString( "GetDObj/1 не нашел ");OutputDebugString( Name );OutputDebugString("\n");
    }
    return pObj;
  }
  if ( strncmp ( Name, "%WB", 3 ) == 0 )
  {
    ASS( Name[6] );
    int Numb = atoi ( Name + 3 );
    int NumbFCS = _NumbFCS;
    if( Name[9]=='S' )
    {
      NumbFCS = atoi( Name + 10 );
      NumbFCS %= CY_DR::s_dwFcsNumModulus;
    }
    pObj = (*pFindDI)( NumbFCS, domain, Numb, id__WB );
    if( !pObj )
    {
      OutputDebugString( "GetDObj/2 не нашел ");OutputDebugString( Name );OutputDebugString("\n");
    }
    return pObj;
  }
  if ( strncmp ( Name, "%WW", 3 ) == 0 )
  {
    ASS( Name[6] );
    int Numb = atoi ( Name + 3 );
    pObj = (*pFindDI)( _NumbFCS, domain, Numb, id__WW );
    if( !pObj )
    {
      OutputDebugString( "GetDObj/3 не нашел ");OutputDebugString( Name );OutputDebugString("\n");
    }
    return pObj;
  }
  if ( strncmp ( Name, "%XW", 3 ) == 0 )
  {
    ASS( Name[6] );
    int Numb = atoi ( Name + 3 );
    pObj = (*pFindDI)( _NumbFCS, domain, Numb, id__XW );
    if( !pObj )
    {
      OutputDebugString( "GetDObj/3 не нашел ");OutputDebugString( Name );OutputDebugString("\n");
    }
    return pObj;
  }
  if ( strncmp ( Name, "%SW", 3 ) == 0 )
  {
    int Numb = atoi ( Name + 3 );
    int NumbFCS = _NumbFCS;
    if( Name[7] && isdigit(Name[7]) )
    {
      NumbFCS = Numb % 100;
      Numb = Numb / 100;
    }
    else if( Name[7]=='S' )
    {
      NumbFCS = atoi( Name + 10 );
      NumbFCS %= CY_DR::s_dwFcsNumModulus;
    }
    else
    {
      if( Name[7] && !isdigit(Name[7]) )
      {
        ASSD( !strcmp( &Name[7], ".PV") );
      }
    }
    pObj = (*pFindDI)( NumbFCS, domain, Numb, id__SW );
    if( !pObj )
    {
      OutputDebugString( "GetDObj/4 не нашел ");OutputDebugString( Name );OutputDebugString("\n");
    }
    return pObj;
  }
  if ( strncmp ( Name, "%PR", 3 ) == 0 )
  {
    int Numb;
    int NumbFCS;
    Numb = atoi ( Name + 3 );
    NumbFCS = _NumbFCS;
    pObj = (*pFindDI)( NumbFCS, domain, Numb, id__PR );
    if( !pObj )
    {
      OutputDebugString( "GetDObj/5 не нашел ");OutputDebugString( Name );OutputDebugString("\n");
    }
    return pObj;
  }
  if ( strncmp ( Name, "%Z", 2 ) == 0 )
  {
    int Numb;
    int NumbFCS;
    Numb = atoi ( Name + 2 );
    NumbFCS = _NumbFCS;
    if( Name[8]=='S' )
    {
      NumbFCS = atoi ( Name + 9 );
      if( strlen(Name+9)==6 )
        NumbFCS /= 100;
      NumbFCS %= CY_DR::s_dwFcsNumModulus;
      pObj = (*pFindDI)( NumbFCS, domain, Numb, id__Z );
    }
    else
      pObj = (*pFindDI)( NumbFCS, domain, Numb, id__Z );
    if( !pObj )
    {
      OutputDebugString( "GetDObj/6 не нашел ");OutputDebugString( Name );OutputDebugString("\n");
    }
    return pObj;
  }
  if ( strncmp ( Name, "%Y", 2 ) == 0 )
  {
    int Numb;
    int NumbFCS;
    Numb = atoi ( Name + 2 );
    NumbFCS = _NumbFCS;
    if( Name[9]=='S' )
    {
      NumbFCS = atoi ( Name + 10 );
      if( strlen(Name+10)==6 )
        NumbFCS /= 100;
      NumbFCS %= CY_DR::s_dwFcsNumModulus;
      pObj = (*pFindDI)( NumbFCS, domain, Numb, id__Y );
    }
    else
      pObj = (*pFindDI)( NumbFCS, domain, Numb, id__Y );
    if( !pObj )
    {
      OutputDebugString( "GetDObj/7 не нашел ");OutputDebugString( Name );OutputDebugString("\n");
    }
    return pObj;
  }
  if ( strncmp ( Name, "%AN", 3 ) == 0 )
  {
    int Numb;
    int NumbFCS;
    Numb = atoi ( Name + 3 );
    NumbFCS = _NumbFCS;
    if( Name[7]=='S' )
    {
      NumbFCS = atoi ( Name + 8 );
      if( strlen(Name+8)==6 )
        NumbFCS /= 100;
      NumbFCS %= CY_DR::s_dwFcsNumModulus;
    }
    pObj = (*pFindDI)( NumbFCS, domain, Numb, id__AN );
    if( !pObj )
    {
      OutputDebugString( "GetDObj/8 не нашел ");OutputDebugString( Name );OutputDebugString("\n");
    }
    return pObj;
  }
  return pObj;
}

CY_Base * CY_MainLogic::GetObj ( char * Name, int _NumbFCS, int domain, bool bSameNumbFCS /*= true*/ )
{  
  CY_Base * pObj = GetDObj( Name, _NumbFCS, domain, bSameNumbFCS );
  if( pObj )
    return pObj;
  if( !strncmp( Name, "%BL", 3 ) || !strncmp( Name, "%WB", 3 ) )
  {
    using namespace std;
    char szBL[20] = { 0 };
    int NumbFcsIn = _NumbFCS;
    string szName( Name );
    string::size_type pos = szName.find_first_not_of( "0123456789", 3 );
    if( string::npos!=pos )
    {
      ASS( 'S'==Name[pos] );
      strncpy_s( szBL, Name, pos );
      string::size_type lenStation = szName.length()-pos-1;
      NumbFcsIn = atoi( Name+pos+1 );
      switch( lenStation )
      {
      case 4:
        break;
      case 6:
        NumbFcsIn = NumbFcsIn / 100;
        break;
      default:
        ASSD(0);
      }
      NumbFcsIn = NumbFcsIn % CY_DR::s_dwFcsNumModulus;
    }
    else
      strcpy_s( szBL, Name );
    pObj = CY_MainLogic::FindObjByNumb( szBL, NumbFcsIn, domain );
    if ( pObj == NULL )
    {
      /*OutputDebugString("Не найдена точка ");
      OutputDebugString( Name );
      OutputDebugString( "\n" );
      Fsend ( "BL.txt", "%s\n", Name );*/
    }
    return pObj;    
  }
  pObj = FindYObj( Name, bSameNumbFCS, _NumbFCS, domain );
  if( pObj == NULL && bSameNumbFCS && pFindDObj )
    pObj = pFindDObj( Name, _NumbFCS, domain );
  if ( pObj == NULL )
    pObj = FindDObj( Name, bSameNumbFCS, _NumbFCS, domain );
  if ( pObj == NULL )
    pObj = (*pFindDObj)( Name, _NumbFCS, domain );
  return pObj;
}

void CY_MainLogic::SetCsv( LPSTR pszPath )
{
    m_szCsvFile = std::filesystem::path(pszPath) / s_szYName;
    m_szCsvFile.replace_extension(s_szYExt);
    std::memcpy( pszPath, m_szCsvFile.string().c_str(), m_szCsvFile.string().length() + 1);
    m_bCreateYokoData = true;
}

void SortListParams();
bool CY_MainLogic::InitParams = true;

void CY_MainLogic::InitParm()
{
  if ( !InitParams )
    return;
  InitParams = false;
  CY_AREAIN::InitParm();
  CYS_ANLG_S::InitParm();
  CY_ARITH::InitParm();
  CY_LD::InitParm();
  CY_AS_HML::InitParm();
  CY_AVE::InitParm();
  CY_AVE_C::InitParm();
  CY_AVE_M::InitParm();
  CY_BAND::InitParm();
  CY_BNOT::InitParm();
  CY_CI::InitParm();
  CY_CO::InitParm();
  CY_CTS::InitParm();
//  CY_DI::InitParm();
  CY_CALCU::InitParm();
  CY_DLAY::InitParm();
  CY_DSET::InitParm();
  CY_DSET_PVI::InitParm();
  CY_DSW_16::InitParm();
  CY_DSW_16C::InitParm();
  CY_FOUT::InitParm();
  CY_HAS3C::InitParm();
  CY_INDST3::InitParm();
//  CY_IO::InitParm();
  CY_LAG::InitParm();
  CY_LC64::InitParm();
  CY_MC_NE::InitParm();
  CY_MLD::InitParm();
  CY_MLD_PVI::InitParm();
  CY_MLD_SW::InitParm();
  CY_OFFD::InitParm();
  CY_OND::InitParm();
  CY_ONOFF::InitParm();
  CY_PBS5C::InitParm();
  CY_PID::InitParm();
  CY_PVI::InitParm();
  CY_FUNC_VAR::InitParm();
  CY_PG_L13::InitParm();
  CY_RATIO::InitParm();
  CY_RL::InitParm();
  CY_SIO_X::InitParm();
  CY_SFCSW::InitParm();
  CY_SFCAS::InitParm();
  CY_SFCPB::InitParm();
  CY_UTAS::InitParm();
  CY_UTSW::InitParm();
  CY_UTPB::InitParm();
  CY_UTPB_N::InitParm();
  CY_SPLIT::InitParm();
  CY_SRSX_X::InitParm();
  CY_SS::InitParm();
  CY_SS_DUAL::InitParm();
  CY_ST16::InitParm();
  CY_SW_33::InitParm();
  CY_SW_91::InitParm();
  CY_TM::InitParm();
  CY_TON::InitParm();
  CY_TOFF::InitParm();
  CY_TPCFL::InitParm();
  CY_VELLIM::InitParm();
  CY_MOB_11::InitParm();
  CY_MOB_21::InitParm();
  CY_FF_AI::InitParm();
  CY_FF_AO::InitParm();
  CY_MOB_RS::InitParm();
  CY_OVR_B::InitParm();
  CY_S_MOA::InitParm();
  SortListParams();
}

void CY_MainLogic::GetYokoDatPath( LPSTR pszPath, DWORD _S, LPCSTR pszExt )
{
  strcpy_s( pszPath, _S, rsuGetProjectPath().c_str());
  strcat_s( pszPath, _S, "Data\\Data_Y\\Yoko" );
  if( !pszExt || !*pszExt )
    return;
  if( '.'!=*pszExt )
    strcat_s( pszPath, _S, "." );
  strcat_s( pszPath, _S, pszExt );
}

int CY_MainLogic::GetProjectFile(const std::filesystem::path &pszFile, std::vector<BYTE> &readBuf )
{
  readBuf.clear();

  std::fstream hClose(pszFile, std::ios::in | std::ios::binary);
  if( !hClose.is_open())
  {
      return -1;
  }
  hClose.seekg(0, std::ios::end);
  DWORD L = hClose.tellg();
  hClose.seekg(0, std::ios::beg);

  readBuf.resize( L, 0 );
  BYTE * pData = &readBuf[0];
  hClose.read(reinterpret_cast<char*>(pData), L);
  return 0;
}