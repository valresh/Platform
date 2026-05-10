#include "Y_MainLogic.h"
#include <macros/AutoCloser.h>
#include <macros/AutoRestore.h>
#include <yokoheaders.h>
#include <yokomodbus.h>
#include <macros/StrHelps.h>
#include <vector>
#include <sstream>
#include <math.h>
#include <io.h>
#include <rsus/rsuPaths.h>

using namespace std;

namespace ns_local
{
  struct CRefName
  {
    char Model[16];
    char Name[16];
    char cXZ[32];
  };
}

static CY_Base KEY( true );

int CompYBase ( const void * pP1, const void * pP2 )
{
  CY_Base * pB1 = *(CY_Base**)pP1;
  CY_Base * pB2 = *(CY_Base**)pP2;
  int r = strcmp ( pB1->Name, pB2->Name );
  if( !r )
  {
    if( pB1->Y_Type > 0x1ff && pB2->Y_Type > 0x1ff )
    {
      if( pB1->Y_Type < pB2->Y_Type )
        return -1;
      if( pB1->Y_Type > pB2->Y_Type )
        return 1;
    }
    if( pB1->pDR && pB2->pDR )
    {
      if( pB1->pDR->domain > 0 && pB2->pDR->domain > 0 )
      {
        if( pB1->pDR->domain < pB2->pDR->domain )
          return -1;
        if( pB1->pDR->domain > pB2->pDR->domain )
          return 1;
      }

      if( pB1->pDR->NumbFCS < pB2->pDR->NumbFCS )
        return -1;
      if( pB1->pDR->NumbFCS > pB2->pDR->NumbFCS )
        return 1;
    }
  }
  return r;
}

int CompDiscret ( const void * pP1, const void * pP2 )
{
  CY_DI * pB1 = *(CY_DI**)pP1;
  CY_DI * pB2 = *(CY_DI**)pP2;
  int r = strcmp ( pB1->Name, pB2->Name );
  if( !r )
  {
    if( pB1->domain > 0 && pB2->domain > 0 )
    {
      if( pB1->domain < pB2->domain )
        return -1;
      if( pB1->domain > pB2->domain )
        return 1;
    }
    if( pB1->NumbFCS > 0 && pB2->NumbFCS > 0 )
    {
      if( pB1->NumbFCS < pB2->NumbFCS )
        return -1;
      if( pB1->NumbFCS > pB2->NumbFCS )
        return 1;
    }
  }
  return r;
}

int CompDR ( const void * pP1, const void * pP2 )
{
  CY_DR * pB1 = *(CY_DR**)pP1;
  CY_DR * pB2 = *(CY_DR**)pP2;
  return strcmp ( pB1->FCS, pB2->FCS );
}

template<size_t _x, size_t _y >
bool IsSkipFcs( char (&skipFcs)[_x][_y], LPCSTR pszFcs)
{
  bool bSkip = false;
  for( int i=0; i<_countof(skipFcs); ++i )
  {
    if( !skipFcs[i][0] )
      break;
    if( !_stricmp(skipFcs[i], pszFcs) )
      return true;
  }
  return false;
}

bool CY_MainLogic::ReadProject()
{
  m_bCreateYokoData = true;
//////////////////////////////////////////////////
  CY_MainLogic::InitParm();

//////////////////////////////////////////////////
  char Path[_MAX_PATH];
  char skipFcs[100][8] = {};
  _makepath_s( Path, NULL, m_szDirPrj, "ignore", "fcs" );
  if( 0!=_access( Path, 0) )
  {
    strcpy_s( Path, rsuGetProjectPath().c_str());
    strcat_s( Path, "Data\\Data_Y\\ignore.fcs" );
  }
  {
    FILE *pFile = NULL;
    errno_t res = fopen_s( &pFile, Path, "rt" );
    if( !res )
    {
      int I = 0;
      KAutoCloser<FILE*> file( pFile, fclose );
      while( fgets( Path, _countof(Path)-1, pFile ) )
      {
        size_t L = strlen( Path );
        if( L < 7 )
          continue;
        strncpy_s( skipFcs[I], Path, 7 );
        ++I;
        ASSD( I<_countof(skipFcs) );
        if( I==_countof(skipFcs) )
          break;
      }
    }
  }

  //////////////////////////////////////////////////////////////////////////
  //BCVO>>
  {
    TAutoRestore<int> arDomain(m_domain);
    TAutoRestore<int> arNumbFCS(m_NumbFCS);
    vector<BYTE> readBuf;
    GetProjectFile( "Common\\StnConf.edf", readBuf );
    if( !readBuf.empty() )
    {
      BYTE * pData = &readBuf[0];

      SYokogawa200* hdr = (SYokogawa200*)pData;
      SYokogawaID* dat = FindYokoID( hdr, "STIF" );
      if( dat )
      {
        for( DWORD i=0; i<dat->dwCount; i++ )
        {
          STATION *p = (STATION*)dat->Item( pData, i );

          char szName8[16] = {}, szType8[16] = {};
          strncpy_s( szName8, p->szName, _countof(p->szName) );
          strncpy_s( szType8, p->szType, _countof(p->szType) );

          if( !szName8[0] )
            continue;
          if( !strncmp( szName8, "BCVO", 4) )
          {
            ASSD( !lstrcmp( szType8, "SIOS") );
            ASSD( isdigit( szName8[4]) );

            m_NumbFCS = atoi ( szName8 + 4 ) % CY_DR::s_dwFcsNumModulus;
            m_domain = atoi ( szName8 + 4 ) / CY_DR::s_dwFcsNumModulus;

            strcpy_s( Path, m_szDirPrj );
            strcat_s( Path, szName8 );
            ReadBCVO( Path, szName8 );
            mNumberOfTags.clear();
          }
        }
      }
    }
  }
  //<<BCVO
  //////////////////////////////////////////////////////////////////////////

  int domainTest = 0;
  WIN32_FIND_DATAA FD;
  TAutoRestore<int> arDomain(m_domain);
  TAutoRestore<int> arNumbFCS(m_NumbFCS);
  //FUNCTION_BLOCK>
  {
    _makepath_s( Path, NULL, m_szDirPrj, "FCS*", NULL );
    KAutoCloser<HANDLE> hFind( FindFirstFile( Path, &FD ), FindClose);
    if ( hFind == INVALID_HANDLE_VALUE )
      return false;
    for( BOOL Res = TRUE; Res; Res = FindNextFile( hFind, &FD ) )
    {
		  if ( FD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
        if( IsSkipFcs( skipFcs, FD.cFileName) )
          continue;
        strcpy_s( Path, m_szDirPrj );
			  strcat_s( Path, FD.cFileName );
        ASS( ReadFcsRef( Path ) );
			  strcat_s( Path, "\\FUNCTION_BLOCK\\" );
			  strcpy_s( m_szFCS, FD.cFileName );
        m_NumbFCS = atoi ( FD.cFileName + 3 ) % CY_DR::s_dwFcsNumModulus;
        m_domain = atoi ( FD.cFileName + 3 ) / CY_DR::s_dwFcsNumModulus;
        if( !domainTest )
        {
          domainTest = m_domain;
          m_pCentumSystem->domains[0] = m_domain;
        }
        else if( domainTest != m_domain )
        {
          for( int i=0; i<_countof(m_pCentumSystem->domains); ++i )
          {
            if( !m_pCentumSystem->domains[i] )
            {
              m_pCentumSystem->domains[i] = m_domain;
              break;
            }
            if( m_pCentumSystem->domains[i]==m_domain )
              break;
          }
          domainTest = m_domain;
          s_bFcsMultiDomain = true;
        }
        GetEdf( Path );
        strcpy_s( Path, m_szDirPrj );
			  strcat_s( Path, FD.cFileName );
        ObrDiskrete( Path );
        mNumberOfTags.clear();
			}
    }
  }//<FUNCTION_BLOCK
  for( int i=0; i<_countof(m_pCentumSystem->domains); ++i )
  {
    if( !m_pCentumSystem->domains[i] )
      break;
    CreateRQs( m_pCentumSystem->domains[i] );
  }

  CreateLeftWBs();
  //////////////////////////////////////////////////////////////////////////
  //SCS>
  // Чтобы искать - нужно сортировать
  qsort ( CY_MainLogic::pY_Obj, CY_MainLogic::kY_Obj, sizeof ( CY_Base *), CompYBase );
  qsort ( CY_MainLogic::pD_Obj, CY_MainLogic::kD_Obj, sizeof ( CY_DI *), CompDiscret );
  qsort ( CY_MainLogic::DR, CY_MainLogic::kDR, sizeof ( CY_DR *), CompDR );
#if 1
  {
    strcpy_s( Path, m_szDirPrj );
    strcat_s( Path, "WBWork\\Tag*.o" );
    KAutoCloser<HANDLE> hFind( FindFirstFile( Path, &FD ), FindClose);
    if( hFind != INVALID_HANDLE_VALUE )
    {
      for( BOOL Res = TRUE; Res; Res = FindNextFile( hFind, &FD ) )
      {
        strcpy_s( Path, m_szDirPrj );
        strcat_s( Path, "WBWork\\" );
        strcat_s( Path, FD.cFileName );
        m_domain = atoi( FD.cFileName + 3 ) / CY_DR::s_dwFcsNumModulus;
        ReadSCS_O( Path, FD.cFileName );
      }
    }
  }//<SCS
#endif
//////////////////////////////////////////////////////
// Изменили - снова сортируем
  mNumberOfTags.clear();
  qsort ( CY_MainLogic::pY_Obj, CY_MainLogic::kY_Obj, sizeof ( CY_Base *), CompYBase );
  qsort ( CY_MainLogic::pD_Obj, CY_MainLogic::kD_Obj, sizeof ( CY_DI *), CompDiscret );
  qsort ( CY_MainLogic::DR, CY_MainLogic::kDR, sizeof ( CY_DR *), CompDR );
//////////////////////////////////////////////////////
  _makepath_s( Path, NULL, m_szDirPrj, "FCS*", NULL );
  KAutoCloser<HANDLE> hFind( FindFirstFile( Path, &FD ), FindClose);
  if ( hFind == INVALID_HANDLE_VALUE )
    return false;
  for( BOOL Res = TRUE; Res; Res = FindNextFile( hFind, &FD ) )
  {
	  if ( FD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
    {
      if( IsSkipFcs( skipFcs, FD.cFileName) )
        continue;
      strcpy_s( Path, m_szDirPrj );
	    strcat_s( Path, FD.cFileName );
      strcat_s( Path, "\\PROG\\TunningData.txt" );
      CY_DR forFind;
      m_NumbFCS = forFind.NumbFCS = atoi( FD.cFileName+3 ) % CY_DR::s_dwFcsNumModulus;
      m_domain = forFind.domain = atoi ( FD.cFileName + 3 ) / CY_DR::s_dwFcsNumModulus;
      KEY.pDR = &forFind;
      CY_Base::s_ReadingTunningData = true;
      ObrTunning( Path );
      CY_Base::s_ReadingTunningData = false;
      KEY.pDR = NULL;
	  }
  }
  m_NumbFCS = 0;
  m_domain = 0;
  return true;
}

void CY_MainLogic::GetEdf( char * Root )
{
	char Path[_MAX_PATH];
	strcpy_s( Path, Root );
	strcat_s( Path, "\\*.edf" );
  WIN32_FIND_DATA FD;
  KAutoCloser<HANDLE> hFind( FindFirstFile( Path, &FD ), FindClose );
  if ( hFind == INVALID_HANDLE_VALUE )
    return;
	for( BOOL Res = TRUE; Res; Res = FindNextFile( hFind, &FD ) )
	{
		if ( FD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
      continue;
   	strcpy_s( Path, Root );
		strcat_s( Path, FD.cFileName );
   	strcpy_s( m_szDR, FD.cFileName );
    char * P = strchr( m_szDR, '.' );
    if( !isdigit(FD.cFileName[2]) )
      continue;
    *P = 0;
	  ObrEdf( Path );
    m_szDR[0] = 0;
	}
}

struct SCommDataWW2 : SCommDataWW
{
  char IOZ;
  CY_DI::ePointType TypeZ, modelTypeZ;
  bool bitsCreated[16];
  SCommDataWW2()
  {
    memset( this, SCommDataWW::edf_NULL, sizeof(*this) );
    ZeroMemory( bitsCreated, sizeof(bitsCreated) );
  }
};
static SCommDataWW2 s_CommDataWW[8010];

void CY_MainLogic::ObrDiskrete( char * Root )
{
  ZeroMemory( s_CommDataWW, sizeof(s_CommDataWW) );
	char Path[1024];
/////////////////////////////////////
	strcpy_s( Path, Root );
	strcat_s( Path, "\\MESSAGE\\" );
	FindAllFiles( 0, Path );
/////////////////////////////////////
	strcpy_s( Path, Root );
	strcat_s( Path, "\\IOM\\" );
	FindAllFiles( id__Z, Path );
  /////////////////////////////////////
  strcpy_s( Path, Root );
  strcat_s( Path, "\\IOM2\\" );
  FindAllFiles( id__Z, Path );
/////////////////////////////////////
	strcpy_s( Path, Root );
	strcat_s( Path, "\\SWITCH\\" );
	FindAllFiles( 0, Path );

  for( int wwNumb=0; wwNumb<_countof(s_CommDataWW); ++wwNumb )
  {
     switch( s_CommDataWW[wwNumb].DataType )
    {
    case SCommDataWW::edt_discretInput:
    case SCommDataWW::edt_discretOutput:
    case SCommDataWW::edt_discretSelectIO:
      {
        for( int bit =0; bit<_countof(s_CommDataWW[wwNumb].bitsCreated); ++bit )
        {
          if( s_CommDataWW[wwNumb].bitsCreated[bit] )
            continue;
          s_CommDataWW[wwNumb].bitsCreated[bit] = true;
          char szWbName[64];
          int Numb = wwNumb*100 + bit + 1;
          sprintf_s( szWbName, "%%WB%04d%02d", wwNumb, bit+1 );
          CY_DI * pDI = CreateDobj( szWbName, id__WB, m_NumbFCS, m_domain, Numb );
          strncpy_s( pDI->Name, szWbName, 16 );
          pDI->Name[16] = 0;
          strcpy_s( pDI->szNumber, pDI->Name );
          pDI->Y_Type = id__WB;
          strcpy_s( pDI->Model, "WB" );
          pDI->NumbFCS = m_NumbFCS;
          pDI->domain = m_domain;
          pDI->pointTypeZ = CY_DI::eptDigital;
          pDI->modelTypeZ = CY_DI::eptDigital;
          pDI->IOZ = s_CommDataWW[wwNumb].IOZ;

          ASS(CY_MainLogic::kD_Obj<_countof(pD_Obj));
          CY_MainLogic::pD_Obj[CY_MainLogic::kD_Obj++] = pDI;
          pDI->SetObjName( m_domain * CY_DR::s_dwFcsNumModulus + m_NumbFCS );
        }
      }
      break;
    }
  }
}

bool CY_MainLogic::FindAllFiles( int Type, char * Dir )
{
	char Path[256];
  _makepath_s( Path, NULL, Dir, "*", "*");
	WIN32_FIND_DATA FD;
	HANDLE hFind = FindFirstFile( Path, &FD );
	if ( hFind == INVALID_HANDLE_VALUE )
		return false;
	BOOL Res = TRUE;
	while ( Res )
		{
		if ( FD.cFileName[0] == '.' )
      {
      Res = FindNextFile( hFind, &FD );
			continue;
      }
		if ( FD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
			_makepath_s( Path, NULL, Dir, FD.cFileName, NULL );
			FindAllFiles( Type, Path );
      Res = FindNextFile( hFind, &FD );
			continue;
			}
		_strlwr_s ( FD.cFileName );
		int L = (int)strlen ( FD.cFileName );
		if ( strcmp ( FD.cFileName + L - 4, ".edf" ) != 0 )
      {
      Res = FindNextFile( hFind, &FD );
			continue;
      }
		if ( strcmp ( FD.cFileName + L - 7, "dup.edf" ) == 0 )
      {
      Res = FindNextFile( hFind, &FD );
			continue;
      }
    _makepath_s( Path, NULL, Dir, FD.cFileName, NULL );
    _strupr_s( Path );
		ReadDiskrete( Type, Path );
		Res = FindNextFile( hFind, &FD );
		}
	FindClose ( hFind );
	return true;
}

int GetType( SYokogawa200 *hdr, LPCSTR *ppIO1, DWORD *pC1, LPCSTR *ppIO2, DWORD *pC2 )
{
  int Type = 0;
  if( strncmp ( hdr->opgr, "BKE_WB", 6 ) == 0 ||
      strncmp ( hdr->opgr, "RHE_TAG", 7 ) == 0 )
    Type = id__WB;
  else if ( strncmp( hdr->opgr, "FCGSW", 5 ) == 0 )
    Type = id__GS;
  else if ( strncmp( hdr->opgr, "FCSWT", 5 ) == 0 )
    Type = id__SW;
  else if ( strncmp( hdr->opgr, "FCSQM", 5 ) == 0 )
    Type = id__PR;
  else if ( strncmp( hdr->opgr, "FCOPM", 5 ) == 0 )
    Type = id__OG;
  else if ( strncmp( hdr->opgr, "FCANM", 5 ) == 0 )
    Type = id__AN;
  else if ( strncmp( hdr->opgr, "BKE_COMMDATA", 12 ) == 0 )
    Type = id__WW;
  else if ( strncmp( hdr->opgr, "BKECOMDATAXW", 12 ) == 0 )
    Type = id__XW;
  else if ( strncmp( hdr->opgr, "BKE_XBTAGDEFB", 13 ) == 0 )
    Type = -1;
  else if ( strncmp( hdr->opgr, "FCEVM", 5 ) == 0 )
  {}//Type = id__EV;
  else if ( strncmp( hdr->opgr, "FCSBEB401", 9 ) == 0 )
    Type = -1;//EB401 Remote Node Communication
  else if ( strncmp( hdr->opgr, "FCSBEB402", 9 ) == 0 )
    Type = -1;//EB402 ER Bus Master Interface Module
  else if ( strncmp( hdr->opgr, "FCSBALP111", 10 ) == 0 )
    Type = -1;//Profibus-DP
  else if ( strncmp( hdr->opgr, "FCSBALP121", 10 ) == 0 )
    Type = -1;//Profibus-DP
  else if ( strncmp( hdr->opgr, "FCSBALR111", 10 ) == 0 )
    Type = -1;//RS-232C
  else if ( strncmp( hdr->opgr, "FCSBALR121", 10 ) == 0 
          ||strncmp( hdr->opgr, "FCIOPRS4", 8 ) == 0
          )
    Type = -1;//RS-422/485
  else if ( strncmp( hdr->opgr, "FCSBDEV", 7 ) == 0 )
    Type = -1;//FF
  else if ( strncmp( hdr->opgr, "FCSBALF111", 10 ) == 0 )
    Type = -1;//FF
  else if ( strncmp( hdr->opgr, "FCSBALE111", 10 ) == 0 )
    Type = -1;//Ethernet
  else if ( strncmp( hdr->opgr, "FCSBAGP813", 10 ) == 0 )
    Type = -1;//HighSpeedProtectionModule K1-3-3PROTECT 91проект
  else if ( strncmp( hdr->opgr, "FCSBA2EN402", 11 ) == 0 )
    Type = -1;//N-ESB 151проект
  else if ( strncmp( hdr->opgr, "FCSBADV151", 10 ) == 0
          ||strncmp( hdr->opgr, "FCSBADV169", 10 ) == 0
          ||strncmp( hdr->opgr, "FCSBADV159", 10 ) == 0
          ||strncmp( hdr->opgr, "FCSBADV161", 10 ) == 0
          ||strncmp( hdr->opgr, "FCIO32CSI", 9 ) == 0
          )
  {
    Type = id__Z;
    if( ppIO1 )
      *ppIO1 = "DI";
  }
  else if ( strncmp( hdr->opgr, "FCSBAAV141", 10 ) == 0
          ||strncmp( hdr->opgr, "FCSBAAP135", 10 ) == 0
          ||strncmp( hdr->opgr, "FCSBAAI141", 10 ) == 0
          ||strncmp( hdr->opgr, "FCSBAAI143", 10 ) == 0
          ||strncmp( hdr->opgr, "FCSBAAR145", 10 ) == 0
          ||strncmp( hdr->opgr, "FCSBAAI135", 10 ) == 0
          ||strncmp( hdr->opgr, "FCSBASR133", 10 ) == 0
          ||strncmp( hdr->opgr, "FCSBAST143", 10 ) == 0
          ||strncmp( hdr->opgr, "FCSBAAR181", 10 ) == 0
          ||strncmp( hdr->opgr, "FCSBAAT141", 10 ) == 0
          ||strncmp( hdr->opgr, "FCSBAAV144", 10 ) == 0
          ||strncmp( hdr->opgr, "FCIOMVINC", 9 ) == 0
          )
  {
    Type = id__Z;
    if( ppIO1 )
      *ppIO1 = "AI";
  }
  else if ( strncmp( hdr->opgr, "FCSBADV569", 10 ) == 0
          ||strncmp( hdr->opgr, "FCSBADV551", 10 ) == 0
          ||strncmp( hdr->opgr, "FCSBADV559", 10 ) == 0
          ||strncmp( hdr->opgr, "FCSBADV561", 10 ) == 0
          ||strncmp( hdr->opgr, "FCSBADV557", 10 ) == 0
          ||strncmp( hdr->opgr, "FCIO32COT", 9 ) == 0
          )
  {
    Type = id__Z;
    if( ppIO1 )
      *ppIO1 = "DO";
  }
  else if ( strncmp( hdr->opgr, "FCSBAAI543", 10 ) == 0
          )
  {
    Type = id__Z;
    if( ppIO1 )
      *ppIO1 = "AO";
    if( pC1 )
      *pC1 = 16;
    if( ppIO2 )
      *ppIO2 = "AI";
    if( pC2 )
      *pC2 = 32;
  }
  else if ( strncmp( hdr->opgr, "FCSBAAV544", 10 ) == 0
          )
  {
    Type = id__Z;
    if( ppIO1 )
      *ppIO1 = "AO";
  }
  else if ( strncmp( hdr->opgr, "FCSBAAI841", 10 ) == 0 
         || strncmp( hdr->opgr, "FCSBAAB841", 10 ) == 0 
          )
  {
    Type = id__Z;
    if( ppIO1 )
      *ppIO1 = "AI";
    if( pC1 )
      *pC1 = 8;
    if( ppIO2 )
      *ppIO2 = "AO";
    if( pC2 )
      *pC2 = 8;
  }
  else if ( strncmp( hdr->opgr, "FCSBAAI835", 10 ) == 0 
          )
  {
    Type = id__Z;
    if( ppIO1 )
      *ppIO1 = "AI";
    if( pC1 )
      *pC1 = 4;
    if( ppIO2 )
      *ppIO2 = "AO";
    if( pC2 )
      *pC2 = 4;
  }
  else if ( strncmp( hdr->opgr, "FCIOCNT", 7 ) == 0 
          )
  {
    Type = id__Z;
    if( ppIO1 )
      *ppIO1 = "AJ";
  }
  else if ( strncmp( hdr->opgr, "FCIO16CSI", 7 ) == 0 
          )
  {
    Type = id__Z;
    if( ppIO1 )
      *ppIO1 = "DI";
  }
  else if ( strncmp( hdr->opgr, "FCIO32TSI", 7 ) == 0 
          )
  {
    Type = id__Z;
    if( ppIO1 )
      *ppIO1 = "DI";
  }
  else if ( strncmp( hdr->opgr, "FCOMAOUT", 8 ) == 0 
          )
  {
    Type = id__Z;
    if( ppIO1 )
      *ppIO1 = "AO";
  }
  else if ( strncmp( hdr->opgr, "FCSBAAT145", 10 ) == 0 
          )
  {
    Type = id__Z;
    if( ppIO1 )
      *ppIO1 = "AI";
  }
  else if ( strncmp( hdr->opgr, "FCNIA2MMM843", 12 ) == 0 
          )
  {
    Type = id__Y;
    if( ppIO1 )
      *ppIO1 = "XJ";
  }
  else if ( strncmp( hdr->opgr, "FCSBADV157SS", 12 ) == 0 
          )
  {
    Type = id__Z;
    if( ppIO1 )
      *ppIO1 = "DI";
  }
  else if ( strncmp( hdr->opgr, "FCSBADR541", 10 ) == 0 
          )
  {
    Type = id__Z;
    if( ppIO1 )
      *ppIO1 = "DO";
  }
  else if ( strncmp( hdr->opgr, "FCSBASI133H", 11 ) == 0 
    )
  {
    Type = id__Z;
    if( ppIO1 )
      *ppIO1 = "AI";
  }
  else if ( strncmp( hdr->opgr, "FCSBASI533H", 11 ) == 0 
    )
  {
    Type = id__Z;
    if( ppIO1 )
      *ppIO1 = "AO";
  }
  else if ( strncmp( hdr->opgr, "FCSBADV869", 10 ) == 0 
    )
  {
    Type = id__Z;
    if( ppIO1 )
      *ppIO1 = "DI";
  }
  else if ( strncmp( hdr->opgr, "FCSBASD143", 10 ) == 0 
    )
  {
    Type = id__Z;
    if( ppIO1 )
      *ppIO1 = "DI";
  }
  else if ( strncmp( hdr->opgr, "FCSBA2EN404", 11 ) == 0 
    )
  {
    Type = id__Z;
    if( ppIO1 )
      *ppIO1 = "AI";
  }
  else
  {
    ASSD( !"Неизвестный заголовок" );
    if( ppIO1 )
      *ppIO1 = NULL;
    if( ppIO2 )
      *ppIO2 = NULL;
    if( pC1 )
      *pC1 = UINT_MAX;
    if( pC2 )
      *pC2 = 0;
  }
  return Type;
}

struct SIOID
{
  enum
  {
    etypeHart = 'Н',
    etypeO = 'О',
  };
  DWORD dws1[4];
  char type[4];
  DWORD dws2[4];
  char nomencklatura[30];
};

#pragma pack( push )
#pragma pack( 1 )
struct SMDLtagZ
{
  enum
  {
    ePercentField = 0x1A,
    eHartField = 0x1C,
  };
  DWORD dat1_1;
  DWORD dat0_1;
  char hiScale[16];//106.3
  char loScale[16];//-6.3
  DWORD dat0_2;
  DWORD dat1_2;
  char ll[16];//-17.1875
  BYTE hz[112];
  BYTE percentOrHart;//1A/1C
  char R;
  BYTE dat0_3[3];
  char P;
  WORD dat0_4;
  char hiCurrent[16];//20
  char loCurrent[16];//4
  char euCurrent[16];
  BYTE dat0_5[56];
  bool TestValid()
  {
    if( 1!=dat1_1 || 0!=dat0_1 || 1!=dat1_2 || 0!=dat0_2 || 'R'!=R || 'P'!=P )
    {
      return false;
    }
    return true;
  }
  bool IsPercentValue()
  {
    return ePercentField==percentOrHart;
  }
  bool IsEuValue()
  {
    return eHartField==percentOrHart;
  }
};
#pragma pack( pop )

SMDLtagZ* TestZconversions( int type, SYokogawa200 *hdr, DWORD *pLeftSizeBlock1 = NULL, SMDLtagZ **ppNext = NULL, DWORD *pLeftSizeBlockN = NULL )
{
  if( id__Z!=type )
    return NULL;
  SYokogawaID* datIOID = FindYokoID( hdr, "IOID" );
  if( !datIOID )
  {
    ASSD( datIOID );
    return NULL;
  }
  {
    SIOID *pID = (SIOID*)datIOID->Item( hdr, 0 );
    switch( pID->type[0] )
    {
    case SIOID::etypeHart:
      break;
    default:
      ASSD(0);
    case SIOID::etypeO:
      return NULL;
    }
  }

  SYokogawaID* datUDLB = FindYokoID( hdr, "UDLB" );
  SYokogawaID* datCMDL = FindYokoID( hdr, "CMDL" );
  SYokogawaID* datCDIN = FindYokoID( hdr, "CDIN" );
  if( !datUDLB || !datCMDL )
  {
    ASSD(0);
    return NULL;
  }
  const DWORD cdwShift = 0x460;
  ASSD( datCMDL->dwSize > cdwShift && 1==datCMDL->dwCount );
  BYTE *pc = (BYTE*)datCMDL->Item( hdr, 0 ) + cdwShift;
  if( pLeftSizeBlock1 )
    *pLeftSizeBlock1 = datCMDL->dwSize - cdwShift;
  SMDLtagZ *tag = (SMDLtagZ*)pc;
  if( datCDIN && ppNext )
  {
    ASSD( datCDIN->dwSize > cdwShift && 1==datCDIN->dwCount );
    pc = (BYTE*)datCDIN->Item( hdr, 0 ) + cdwShift;
    if( pLeftSizeBlockN )
      *pLeftSizeBlockN = datCDIN->dwSize - cdwShift;
    *ppNext = (SMDLtagZ*)pc;
  }
  return tag;
}

#pragma pack( push )
#pragma pack( 1 )
struct SMDLtagY
{
  enum eType : BYTE
  {
    //0x94
    eCurrentInput_IS = 0x0,
    eCurrentInputHART = 0x2,
    eCurrentOutputHART = 0x3,
    //0x91
    eCurrentInputDef = 0x0,
    eCurrentInput  = 0x1,
    eStatusInput = 0x4,
    eStatusOutput = 0x6,
    eStatusInputSOE = 0xE,
  };
  char c34;
  BYTE hz0[15+16+16];
  char hiScale[16];//106.3
  char loScale[16];//-6.3
  DWORD dw01;
  char minus[8];
  BYTE hz1[4*16];
  char ten[16];
  char nul[16];
  DWORD dw11;
  BYTE hz2[0x228];
  eType Type;
  char S1;
  WORD wNul1;
  char vznak;
  char S2;
  BYTE hz3[0x13];
  char P;
  WORD wNul2;
  char szNul1[0x10];
  char szNul2[0x10];
  BYTE hvost[0x3C];
};
#pragma pack( pop )
SMDLtagY* TestYconversions( int type, SYokogawa200 *hdr, BYTE &YtypeDevice, DWORD *pLeftSizeBlock1 = NULL )
{
  if( id__Y!=type )
    return NULL;

  SYokogawaID* datIOID = FindYokoID( hdr, "IOID" );
  if( !datIOID )
  {
    ASSD( datIOID );
    return NULL;
  }
  {
    SIOID *pID = (SIOID*)datIOID->Item( hdr, 0 );
    switch( pID->type[0] )
    {
    case (char)0x91:
      YtypeDevice = 0x91;
      break;
    case (char)0x94:
      YtypeDevice = 0x94;
      break;
    default:
      ASSD(0);
      return NULL;
    }
  }

  SYokogawaID* datUDLB = FindYokoID( hdr, "UDLB" );
  SYokogawaID* datCMDL = FindYokoID( hdr, "CMDL" );
  SYokogawaID* datCDIN = FindYokoID( hdr, "CDIN" );
  if( !datUDLB || !datCMDL )
  {
    ASSD(0);
    return NULL;
  }
  const DWORD cdwShift = 0x198;
  ASSD( datCMDL->dwSize > cdwShift && 1==datCMDL->dwCount );
  BYTE *pc = (BYTE*)datCMDL->Item( hdr, 0 );
  pc += cdwShift;
  if( pLeftSizeBlock1 )
    *pLeftSizeBlock1 = datCMDL->dwSize - cdwShift;
  SMDLtagY *tag = (SMDLtagY*)pc;
  return tag;
}

void ExtractInfo( SINFO_WW *sInfo, BYTE &wwCountForData )
{
  char IOZ = 0;
  char AD = 0;
  switch( sInfo->ioType )
  {
  case 1:
    IOZ = 'O';
    break;
  case 0:
    IOZ = 'I';
    break;
  default:
    ASS( 0 );
  }
  BYTE dataType = sInfo->dataType;
  ASS( sInfo->dataType < SCommDataWW::edtCount );
  if( sInfo->dataType>=SCommDataWW::edt_16sI && sInfo->dataType<SCommDataWW::edt_discretInput )
    AD = 'A';
  else if( sInfo->dataType>=SCommDataWW::edt_discretInput && sInfo->dataType<SCommDataWW::edtCount )
    AD = 'D';
  else
    ASS( 0 );

  if( SCommDataWW::edt_discretSelectIO==dataType)
  {
    ASS( 'D'==AD );
    if( 'O'==IOZ )
      dataType = SCommDataWW::edt_discretOutput;
    else if( 'I'==IOZ )
      dataType = SCommDataWW::edt_discretInput;
  }

  switch( dataType )//кол-во XWW на единицу данных
  {
  case SCommDataWW::edt_16sI:
    wwCountForData = 1;
    break;
  case SCommDataWW::edt_32sI:
    wwCountForData = 2;
    break;
  case SCommDataWW::edt_16uI:
    wwCountForData = 1;
    break;
  case SCommDataWW::edt_32uI:
    wwCountForData = 2;
    break;
  case SCommDataWW::edt_32fI:
    wwCountForData = 2;
    break;
  case SCommDataWW::edt_64fI:
    wwCountForData = 4;
    break;
  case SCommDataWW::edt_16sO:
    wwCountForData = 1;
    break;
  case SCommDataWW::edt_32sO:
    wwCountForData = 2;
    break;
  case SCommDataWW::edt_16uO:
    wwCountForData = 1;
    break;
  case SCommDataWW::edt_32uO:
    wwCountForData = 2;
    break;
  case SCommDataWW::edt_32fO:
    wwCountForData = 2;
    break;
  case SCommDataWW::edt_64fO:
    wwCountForData = 4;
    break;
  case SCommDataWW::edt_discretInput:
    wwCountForData = 1;
    break;
  case SCommDataWW::edt_discretOutput:
    wwCountForData = 1;
    break;
  default:
    ASS(0);
  }
}

struct SIOM_Z
{
  LPCSTR pTypeZ;
  DWORD dwDirCount;
};

bool CY_MainLogic::ReadDiskrete( int Type, char *File  )
{
  std::vector<BYTE> btBuf;
  GetProjectFile( File, btBuf );
  if( btBuf.empty() )
    return false;
  BYTE * pData = &btBuf[0];

  SYokogawa200 *hdr = (SYokogawa200*)pData;

  SIOM_Z _Z[2] =
  {
    { "", UINT_MAX },
    { "", 0 },
  };

  if ( Type == 0 )
    Type = GetType( hdr, &_Z[0].pTypeZ, &_Z[0].dwDirCount, &_Z[1].pTypeZ, &_Z[1].dwDirCount );

  if ( Type <= 0 )
  {
    OutputDebugString( "Не определил тип, не обрабатываю\t\t" );
    OutputDebugString( File );
    OutputDebugString( "\r\n" );
		return false;
  }
	BYTE * P_data = pData + 0x50;
	int Len_data = 0;
  int K_data = 0;
	BYTE * P_name = pData + 0x40;
  SYokogawaID* infYoko = NULL;
  SYokogawaID* datAsgn = NULL;
  SYokogawaID* datUDLB = FindYokoID( hdr, "UDLB" );
  SYokogawaID* datIOEX = FindYokoID( hdr, "IOEX" );
  SYokogawaID* datHTLT = FindYokoID( hdr, "HTLT" );
  
  SYokogawaID* datIOID = FindYokoID( hdr, "IOID" );
  SYokogawaID* datIODT = FindYokoID( hdr, "IODT" );
  SYokogawaID* datCENO = FindYokoID( hdr, "CENO" );
  SYokogawaID* datIOCG = FindYokoID( hdr, "IOCG" );
  
  SASGN *pAsgn = NULL;

  DWORD hartLeftMem[2] = {};
  SMDLtagZ *pHartInfoZ[2] = {};
  SMDLtagY *pHartInfoY[2] = {};
  BYTE YtypeDevice = 0;


  if ( Type == id__Z )
  {
    int tType = GetType( hdr, &_Z[0].pTypeZ, &_Z[0].dwDirCount, &_Z[1].pTypeZ, &_Z[1].dwDirCount );
    if( tType < 0 )
    {
      OutputDebugString( "Пропускаю разбор %Z ,т.к. он \"промежуточный\"\t\t" );
      OutputDebugString( File );
      OutputDebugString( "\r\n" );
      return true;
    }
    switch( tType )
    {
    case id__Z:
      ASSD( tType == Type);
      //pHartInfoZ[0] = TestZconversions( tType, hdr, &hartLeftMem[0], &pHartInfoZ[1], &hartLeftMem[1] );
      break;
    case id__Y:
      Type = tType;
      pHartInfoY[0] = TestYconversions( tType, hdr, YtypeDevice, &hartLeftMem[0]/*, &pHartInfoY[1], &hartLeftMem[1]*/ );
      break;
    default:
      ASSD(0);
      break;
    }

    if( !_Z[0].pTypeZ || !_Z[0].pTypeZ[0] )
    {
      OutputDebugString( "Не определил направление обмена для %Z\t\t" );
      OutputDebugString( File );
      OutputDebugString( "\r\n" );

      return false;
    }
	}

  BYTE * P_info;
  int K_info = 0;
  int L_info = 0;
  bool OK = true;
  bool AnalogZ = false;
  bool InZ = false;
  SIOM_Z *_pZ = _Z;
	switch ( Type )
	{
  case id__Y:
    {
      DWORD Shft_data = *(DWORD*)(P_data+4);
      K_data = *(int*)(P_data+8);
      Len_data = *(int*)(P_data+12);
      P_data = pData + Shft_data;
    }
    break;
		case id__Z:
      {
        DWORD Shft_data = *(DWORD*)(P_data+4);
	      K_data = *(int*)(P_data+8);
	      Len_data = *(int*)(P_data+12);
	      P_data = pData + Shft_data;
        AnalogZ = true;
        if( _pZ->pTypeZ[0] == 'D' )
          AnalogZ = false;
        else if( _pZ->pTypeZ[0] != 'A' )
          OK = false;
        InZ = true;
			  if ( _pZ->pTypeZ[1] == 'O'	)
				  InZ = false;
      }
		  break;
		case id__AN:
			if ( strncmp ( hdr->opgr, "FCANM", 5 ) != 0 )
				OK = false;
			if ( strncmp ( (const char*)(P_name), "UDLB", 4 ) != 0 )
				OK = false;
      if( !datHTLT )
				OK = false;
      if ( OK )
        {
        K_data = datHTLT->dwCount;
        Len_data = datHTLT->dwSize;
	      P_data = (BYTE*)datHTLT->Item( pData );
        }
		break;
		case id__WB:
			if ( strncmp ( hdr->opgr, "BKE_WB", 6 ) != 0 &&
				   strncmp ( hdr->opgr, "RHE_TAG", 7 ) != 0 )
				OK = false;
			if ( strncmp ( (const char*)(P_name), "HTL1", 4 ) != 0 )
				OK = false;
      if ( OK )
        {
	      DWORD Shft_data = *(DWORD*)(P_data+4);
	      K_data = *(int*)(P_data+8);
	      Len_data = *(int*)(P_data+12);
	      P_data = pData + Shft_data;
        }
		break;
		case id__GS:
			if ( strncmp ( hdr->opgr, "FCGSW", 5 ) != 0 )
				OK = false;
			if ( strncmp ( (const char*)(P_name), "UDLB", 4 ) != 0 )
				OK = false;
			if ( strncmp ( (const char*)(P_data), "HTLT", 4 ) != 0 )
				OK = false;
      if ( OK )
        {
	      DWORD Shft_data = *(DWORD*)(P_data+4);
	      K_data = *(int*)(P_data+8);
	      Len_data = *(int*)(P_data+12);
	      P_data = pData + Shft_data;
        }
		break;
		case id__SW:
			if ( strncmp ( hdr->opgr, "FCSWT", 5 ) != 0 )
				OK = false;
			if ( strncmp ( (const char*)(P_name), "UDLB", 4 ) != 0 )
				OK = false;
			if ( strncmp ( (const char*)(P_data), "HTLT", 4 ) != 0 )
				OK = false;
      if ( OK )
        {
	      DWORD Shft_data = *(DWORD*)(P_data+4);
	      K_data = *(int*)(P_data+8);
	      Len_data = *(int*)(P_data+12);
	      P_data = pData + Shft_data;
        }
		break;
		case id__WW:
    case id__XW:
			if ( strncmp ( (const char*)(P_name), "UDLB", 4 ) != 0 )
				OK = false;
      P_info = pData + 0xA0;
			if ( strncmp ( (const char*)(P_info), "CNTN", 4 ) != 0 )
				OK = false;
      if ( OK )
      {
        ASS( datUDLB );
	      DWORD Shft_data = datUDLB->dwShift;
	      K_data = datUDLB->dwCount;
	      Len_data = datUDLB->dwSize;
	      P_data = pData + datUDLB->dwShift;
        infYoko = FindYokoID( hdr, "CNTN" );
        ASS( infYoko );

	      Shft_data = infYoko->dwShift;
        K_info = 0;
        L_info = infYoko->dwSize;
        P_info = pData + Shft_data;

        datAsgn = FindYokoID( hdr, "ASGN" );
        ASS( datAsgn );
        ASS( sizeof(*pAsgn)==datAsgn->dwSize );
        pAsgn = (SASGN*)(pData+datAsgn->dwShift);
      }
		break;
		case id__PR:
      {
        if ( strncmp ( hdr->opgr, "FCSQM", 5 ) != 0 )
          OK = false;
        if ( strncmp ( (const char*)(pData+0x80), "SDSM", 4 ) != 0 )
          OK = false;
        if ( OK )
        {
          SYokogawaID* datSDSM = FindYokoID( hdr, "SDSM" );
          K_data = datSDSM->dwCount;
          Len_data = datSDSM->dwSize;
          P_data = pData + datSDSM->dwShift;
        }
      }
		break;
		case id__OG:
      {
			if ( strncmp ( hdr->opgr, "FCOPM", 5 ) != 0 )
				OK = false;
			if ( strncmp ( (const char*)(pData+0x80), "MSOG", 4 ) != 0 )
				OK = false;
      if ( OK )
        {
 	      DWORD Shft_data = *(DWORD*)(pData+0x84);
        P_data = pData+Shft_data;
	      Len_data = *(int*)(pData+0x8C);
        }
      }
		break;
    default:
      OK = false;
      break;
		}
	if ( !OK )
  {
    OutputDebugString( "Не обработан\t\t" );
    OutputDebugString( File );
    OutputDebugString( "\r\n" );
		return false;
  }
//
	DWORD Shft_name = *(DWORD*)(P_name+4);
	DWORD K_name = *(int*)(P_name+8);
	int Len_name = *(int*)(P_name+12);
//
  P_name = pData + Shft_name;
  int FirstSW = 0;
  //
  char OG_Name[128];
  CY_DI::ePointType TypeZ = CY_DI::eptUnknown;
  CY_DI::ePointType modelTypeZ = CY_DI::eptUnknown;
  char IOZ = ' ';
  SCommDataWW::eDataType edtDataType = SCommDataWW::edt_HZ;
  DWORD nZ = 0, nZchange = 0;
  char szTagComment[40], szLabel[40];
  BYTE wwCountForData = 0;
  DWORD nWWSize = 0;
  for( DWORD n = 0; n < K_name; n++, nZ++, P_name+=Len_name, ++nWWSize )
	{
    if( nZ >= _pZ->dwDirCount )
    {
      nZchange++;
      if( nZchange>=_countof(_Z) )
        break;
      nZ = 0;
      _pZ++;
      AnalogZ = true;
      if( _pZ->pTypeZ[0] == 'D' )
        AnalogZ = false;
      else if( _pZ->pTypeZ[0] != 'A' )
      {
        OutputDebugString( "Сбой при смене направления \t\t" );
        OutputDebugString( File );
        OutputDebugString( "\r\n" );
        break;
      }
      InZ = true;
      if ( _pZ->pTypeZ[1] == 'O'	)
        InZ = false;
    }
    char * pComm = NULL;
    char * pComm2 = NULL;
    char * pName = NULL;
    char * pZ = NULL;
    int Numb = -1;
    char * Model = "";
    bool NoRead = false;
    char tmp_Name[128];
    int AlarmLevel = 16;
/////////////////////////////////////////////////
    switch ( Type )
			{
    default:
      ASSD(0);
      continue;
    case id__Y:
      ASSD(pHartInfoY[0]);
      {
        if( n>15 )
          break;
        SMDLtagY *ptagY = pHartInfoY[0] + n;
        SUDLB *pu = (SUDLB*)datUDLB->Item( pData, n );
        pZ = pu->Terminal;
        pComm = pu->ServiceComment;
        memmove( szLabel, pu->Label, _countof(pu->Label) );//pName = pu->Label;
        szLabel[_countof(pu->Label)] = 0;
        pName = szLabel;
        SHTLT *ph = NULL;
        if( datHTLT )
        {
          ph = (SHTLT*)datHTLT->Item( pData, n );
          memmove( szTagComment, ph->TagComment, _countof(ph->TagComment) );//pComm2 = ph->TagComment;
          szTagComment[_countof(ph->TagComment)] = 0;
        }
        pComm2 = szTagComment;
        Numb = atoi( pZ + 2 );
        if( *pName == 0 )
          pName = pZ;

        ASSD( 0x34==ptagY->c34 );
        if( 0x91==YtypeDevice )
        {
          switch( ptagY->Type )//A2BN3D
          {
          default:
            ASSD(0);
            continue;
          case ptagY->eCurrentInput:
          case ptagY->eCurrentInputDef:
          case ptagY->eCurrentInputHART:
            IOZ = 'I';
            modelTypeZ = TypeZ = CY_DI::eptAnalog;
            break;
          case ptagY->eStatusInput:
            IOZ = 'I';
            modelTypeZ = TypeZ = CY_DI::eptDigital;
            break;
          case ptagY->eCurrentOutputHART:
            IOZ = 'O';
            modelTypeZ = TypeZ = CY_DI::eptAnalog;
            break;
          case ptagY->eStatusOutput:
            IOZ = 'O';
            modelTypeZ = TypeZ = CY_DI::eptDigital;
            break;
          case ptagY->eStatusInputSOE:
            modelTypeZ = TypeZ = CY_DI::eptDigital;
            IOZ = 'I';
            break;
          }
        }
        else if( 0x94==YtypeDevice )//A2BN_D
        {
          switch( ptagY->Type )
          {
          case ptagY->eCurrentInput_IS:
          case ptagY->eCurrentInputHART:
            IOZ = 'I';
            modelTypeZ = TypeZ = CY_DI::eptAnalog;
            break;
          case ptagY->eCurrentOutputHART:
            IOZ = 'O';
            modelTypeZ = TypeZ = CY_DI::eptAnalog;
            break;
          default:
            ASSD(0);
            continue;
          }
        }
        else
        {
          ASSD( 0 );
          continue;
        }
        Model = "Y";
      }
      break;
			case id__Z:
        {
          if ( AnalogZ )
          {//Z Аналог
            SUDLB *pu = (SUDLB*)datUDLB->Item( pData, n );
            modelTypeZ = TypeZ = CY_DI::eptAnalog;
            pZ = pu->Terminal;
            IOZ = _pZ->pTypeZ[1];
            if( 'J'==IOZ )
            {
              ASS( datIOCG );
              ASS( datIOCG->dwCount > n );
              DWORD *pcg = (DWORD*)datIOCG->Item( pData, n );
              switch( pcg[1] )
              {
              case 0:
                continue;
              case 0x18C:// 1 Current Input
                IOZ = 'I';
                ASS( 0x180==pcg[4] );
                break;
              case 0x80:// 12 Current Output
                IOZ = 'O';
                ASS( 0x74==pcg[4] );
                break;
              default:
                ASSD(0);
              }
            }
            pName = pu->Label;
            pComm = pu->ServiceComment;
            Numb = atoi( pZ + 2 );
            if( *pName == 0 )
              pName = pZ;
            SHTLT *ph = NULL;
            if( datHTLT )
            {
              ph = (SHTLT*)datHTLT->Item( pData, n );
              pComm2 = ph->TagComment;
            }
          }
          else
          {//Z Дискреты
            SUDLB *pu = (SUDLB*)datUDLB->Item( pData, n );
            SIOEX *pi = NULL;
            if( datIOEX )
              pi = (SIOEX*)datIOEX->Item( pData, n );
            SHTLT *ph = NULL;
            if( datHTLT )
              ph = (SHTLT*)datHTLT->Item( pData, n );
            /*if( *pu->hz )
            {
              ASS( !strcmp( pu->hz, pu->Label ) );
            }*/
            modelTypeZ = TypeZ = CY_DI::eptDigital;
            BYTE _Type = *(P_data + 0x56);
            pZ = pu->Terminal;//(char*)P_name; //%Z...
            IOZ = _pZ->pTypeZ[1];
            memmove( szLabel, pu->Label, _countof(pu->Label) );//pName = pu->Label;
            szLabel[_countof(pu->Label)] = 0;
            pName = szLabel;
            memmove( szTagComment, ph->TagComment, _countof(ph->TagComment) );//pComm2 = ph->TagComment;
            szTagComment[_countof(ph->TagComment)] = 0;
            pComm2 = szTagComment;
            if( pi )
              pComm = pi->P_IDTagName;
            Numb = atoi( pZ + 2 );
            if( *pName == 0 )
            {
              pName = pZ;
            }
          }
          Model = "Z";
        }
			  break;
			case id__AN:
				if ( *(P_data) == 0 )
					{
					P_data += Len_data;
					continue;
					}
//        Numb = n + 1;
        {
          SHTLT *phtlt = (SHTLT*)P_data;
          AlarmLevel = phtlt->btAlarm;
          Numb = n + 1;
          ASS( Numb==phtlt->num );
          pName = (char*)(P_data);
          pComm = (char*)(P_data + 16);
				  P_data += Len_data;
          Model = "AN";
        }
			break;
      case id__XW:
			case id__WW:
        {
          Model = "WW";
          if( id__XW==Type)
            Model = "XW";
          SINFO_WW *sInfo = (SINFO_WW*)P_info;
          Numb = atoi ( (char*)P_name + 3 );
          if( Numb < pAsgn->numStartWW )
            continue;

          if( sInfo->Size == 0 )
          {
            pAsgn->addrCount--;
            if( pAsgn->addrCount < 0 )
            {
              pAsgn->addrCount = 0;
              break;
            }
            if( !pAsgn->addrCount && datAsgn->dwCount )
              pAsgn++, datAsgn->dwCount--;
            while( infYoko->dwCount )
            {
              P_info += L_info;
              infYoko->dwCount--;
              if( !infYoko->dwCount )
                break;
              sInfo = (SINFO_WW*)P_info;
              if( sInfo->Device_and_Address[0] )
                break;
              KKK();
            }
            wwCountForData = 0;
          }
          if( !infYoko->dwCount )
            break;
          if( Numb < (sInfo->endPrev + pAsgn->numStartWW) )
            continue;

          sInfo->Size--;
          if ( K_info <= 0 )
          {
            BYTE idIO = *(P_info + 15 );
            switch( idIO )
            {
            case 1:
              IOZ = 'O';
              break;
            case 0:
              IOZ = 'I';
              break;
            default:
              ASS( 0 );
            }
            // 1- 16s, 2 - 32s, 3- 16u, 4 - 32u, 5- 32f, 6 - 64f, 13 - discret Input, 14 - discret Output
            edtDataType = (SCommDataWW::eDataType)*(P_info + 14 );
            
            /*
            SCommDataWW tmp;
            tmp.DataType = edtDataType;
            if( tmp.isDataTypeBit() )
              TypeZ = CY_DI::eptDigital;
            else
              TypeZ = CY_DI::eptAnalog;
            */

            ASS( edtDataType < SCommDataWW::edtCount );
            if( edtDataType>=SCommDataWW::edt_16sI && edtDataType<SCommDataWW::edt_discretInput )
              TypeZ = modelTypeZ = CY_DI::eptAnalog;
            else if( edtDataType>=SCommDataWW::edt_discretInput && edtDataType<SCommDataWW::edtCount )
              TypeZ = modelTypeZ = CY_DI::eptDigital;
            else
              ASS( 0 );
            nWWSize = 0;
            wwCountForData = 0;
            ExtractInfo( sInfo, wwCountForData );
            K_info = *(short*)( P_info + 2 );//уже за вычетом текущего
          }
          else
            K_info--;

          pName = (char*)( P_name + 0x20 );
          pComm = (char*)( P_name + 0x30 );
            if( nWWSize%wwCountForData )
            {
              if( !*pName && !*pComm )
                continue;
            }
            else
              nWWSize = 0;

          ASS( Numb < _countof(s_CommDataWW) );
          s_CommDataWW[Numb].IOZ = IOZ;
          s_CommDataWW[Numb].TypeZ = TypeZ;
          s_CommDataWW[Numb].modelTypeZ = modelTypeZ;
          s_CommDataWW[Numb].DataType = edtDataType;
          pName = (char*)( P_name + 0x20 );
          if( !pName[0] )
            pName = (char*)P_name;
          pComm = (char*)( P_name + 0x30 );
        }
			break;
			case id__WB:
        {
				if ( *(P_name) == 0 )
					continue;

        BYTE * pNumb = ( P_name + 60 );
        if( 0==*(DWORD*)pNumb )
          continue;
        int Low = *pNumb;
        int Hi = *(pNumb+1);
        Numb = Low % 16;
        if ( Numb == 0 )
        {
          Numb = 16;
          Low -= 16;
        }
        Low = Low >> 4;
        Hi = Hi << 4;
        Hi = Hi + Low;
        Numb += ( Hi + 1 ) * 100; 
        pName = (char*)(P_name);
        pComm = (char*)(P_name + 16);
        DWORD wwNumb = Numb / 100;
        
        ASS( wwNumb < _countof(s_CommDataWW) );
        
        switch( s_CommDataWW[wwNumb].DataType )
        {
        case SCommDataWW::edt_discretInput:
        case SCommDataWW::edt_discretOutput:
        case SCommDataWW::edt_discretSelectIO:
          {
            int bit = (Numb % 100)-1;
            ASS( bit>=0 && bit<_countof(s_CommDataWW[wwNumb].bitsCreated) );
            s_CommDataWW[wwNumb].bitsCreated[bit] = true;
          }
          break;
        }

        switch( s_CommDataWW[wwNumb].IOZ )
        {
        case 'I':
        case 'O':
          break;
        default:
          /*pName = NULL;
        }
        if( !pName )
        {*/
          s_CommDataWW[wwNumb].IOZ = 'X';
          stringstream tr;
          tr << "Неопределен WW (" << m_szFCS << ") для " << P_name << " (WB__" << Numb << ") (No.=" << n+1 <<")\n";
          OutputDebugString( tr.str().c_str() );
          break;
        }
        //ASS( 'D'==s_CommDataWW[wwNumb].TypeZ );
        if( !s_CommDataWW[wwNumb].isDataTypeBit() )
        {
          stringstream tr;
          tr << "Берётся бит из нецелочисленной переменной (" << m_szFCS << " ";
          if( pName )
            tr << pName << " ";
          tr << "WB__" << Numb << ")\n";
          OutputDebugString( tr.str().c_str() );
        }
        modelTypeZ = TypeZ = CY_DI::eptDigital;
        //s_CommDataWW[wwNumb].modelTypeZ;
        IOZ = s_CommDataWW[wwNumb].IOZ;
        }
        Model = "WB";
			break;
			case id__GS:
        /*if ( *(P_data) == 0 )
        {
          P_data += Len_data;
          continue;
        }*/
        Numb = ( n + 1 );// * 100 + m_NumbFCS;
        pName = (char*)(P_data);
        if( !*pName )
          sprintf_s( tmp_Name, "%%GS%03d%02d", Numb, m_NumbFCS );
        pComm = (char*)(P_data + 16);
        P_data += Len_data;
        Model = "GS";
			break;
			case id__SW:
        {
        if ( n == 0 )
          FirstSW = atoi ( (char*)(P_data+3) );
        SUDLB_SMALL *pu = datUDLB ? (SUDLB_SMALL*)datUDLB->Item( pData, n ) : NULL;
        pName = (char*)(P_data);
        pComm = (char*)(P_data + 16);
        if( pu && pu->szTagName[0] )
          pName = pu->szTagName;
        /*bool CanBeEmpty = true;
				if ( *pName != 0 && *pName != '%' )
          CanBeEmpty = false;
        if ( n == 0 )
          CanBeEmpty = false;
				if ( CanBeEmpty && ( *pName == 0 || strncmp ( pComm, "System", 6 ) == 0 || *pComm == 0 ))
					{
	  			P_data += Len_data;
					continue;
					}*/
        int N = *(short*)(P_data+60);
        if ( N == 0 )
          N = FirstSW + n;
        Numb = N;
        //pName = (char*)(P_data);
        /*if ( pName[0] == '%' )
          {
          sprintf_s( OG_Name, "%%SW%06d", Numb );
          pName = OG_Name;
          }*/
        pComm = (char*)(P_data + 16);
				P_data += Len_data;
        Model = "SW";
        }
			break;
			case id__PR:
        {
        //pName = (char*)(P_data);
        pComm = (char*)(P_data + 0x80);
				P_data += Len_data;
        Numb = n + 1;
        tmp_Name[0] = 0;
        pName = tmp_Name;
        if( datUDLB )
        {
          SUDLB *pu = (SUDLB*)datUDLB->Item( pData, n );
          if( pu->hz[0] )
          {
            MoveMemory( tmp_Name, pu->hz, sizeof(pu->hz) );
            tmp_Name[_countof(pu->hz)] = '\0';
          }
        }
        if( !*pName )
          sprintf_s( tmp_Name, "%%PR%04d", Numb );
        Model = "PR";
        }
			break;
			case id__OG:
      {
        pComm = (char*)(P_data + 0x30);
        if ( *pComm == 0 )
          pName = NULL;
        else
        {
          sprintf_s( OG_Name, "%%OG%04d", n + 1 );
          if( datUDLB )
          {
            SUDLB_SMALL *pu = datUDLB ? (SUDLB_SMALL*)datUDLB->Item( pData, n ) : NULL;
            if( pu && pu->szTagName[0] )
            {
              MoveMemory( OG_Name, pu->szTagName, sizeof(pu->szTagName) );
              OG_Name[_countof(pu->szTagName)] = '\0';
            }
          }
          pName = OG_Name;
        }
				P_data += Len_data;
        Numb = n + 1;
      }
      Model = "OG";
			break;
			}
    if ( NoRead )
      continue;
    if ( pName == NULL )
      continue;
    if ( pName[0] == 0 )
      continue;
////////////
    char shrinkName16[20];
    strncpy_s( shrinkName16, pName, 16 );
    CY_DI * pDI = CreateDobj( shrinkName16, Type, m_NumbFCS, m_domain, Numb );
    strncpy_s( pDI->Name, pName, 16 );
		pDI->Name[16] = 0;
    pDI->Y_Type = Type;
    strcpy_s( pDI->Model, Model );
    pDI->NumbFCS = m_NumbFCS;
    pDI->domain = m_domain;
    pDI->pointTypeZ = TypeZ;
    pDI->modelTypeZ = modelTypeZ;
    pDI->IOZ = IOZ;
////////////////////////////
    char Comment[256] = { 0 };
    if ( pComm && pComm[0] )
      {
      if ( pComm2 && pComm2[0] )
        sprintf_s ( Comment, "(%s)%s", pComm, pComm2 );
      else
        strcpy_s( Comment, pComm );
      }
    else
      {
      if ( pComm2 && pComm2[0] )
        strcpy_s( Comment, pComm2 );
      }
    Comment[63] = 0;
    strcpy_s( pDI->Comment, Comment );
//
    if ( Type == id__AN )
    {
      BYTE Level = pComm[0x28];
      BYTE Equip = pComm[0x32];
      pDI->AlarmLevel = AlarmLevel;
    }
    if( Numb > 0 )
    {
      if( 'B'==pDI->Model[1])//WB
        sprintf_s( pDI->szNumber, "%%%s%06d", pDI->Model, Numb );
      else if( 'Y'==pDI->Model[1])
        sprintf_s( pDI->szNumber, "%%%s%05d", pDI->Model, Numb );
      else
        sprintf_s( pDI->szNumber, "%%%s%04d", pDI->Model, Numb );
    }
    pDI->No = n+1;
    ASS(CY_MainLogic::kD_Obj<_countof(pD_Obj));
    CY_MainLogic::pD_Obj[CY_MainLogic::kD_Obj++] = pDI;
    pDI->SetObjName( m_domain * CY_DR::s_dwFcsNumModulus + m_NumbFCS );
	}
	return true;
}

int CY_MainLogic::FindIn( char * ObjRef, int onNumbFCS, int nFrom /*= 0*/ )
{
  for ( nFrom; nFrom < CY_MainLogic::kConnection; nFrom++ )
  {
    if( CY_MainLogic::Connection[nFrom].NumbFCS!=onNumbFCS )
      continue;
    if( CY_MainLogic::Connection[nFrom].domain!=m_domain )
      continue;
    if ( strcmp ( CY_MainLogic::Connection[nFrom].In, ObjRef ))
      continue;
    return nFrom;
  }
  return -1;
}

int CY_MainLogic::FindInSkipThisFCS( char * ObjRef, int NumbFCSskip, int nFrom /*= 0*/ )
{
  for ( nFrom; nFrom < CY_MainLogic::kConnection; nFrom++ )
  {
    if( CY_MainLogic::Connection[nFrom].NumbFCS==NumbFCSskip )
      continue;
    if( CY_MainLogic::Connection[nFrom].domain!=m_domain )
      continue;
    if ( strcmp ( CY_MainLogic::Connection[nFrom].In, ObjRef ))
      continue;
    return nFrom;
  }
  return -1;
}

int CY_MainLogic::FindOut( char * ObjRef, int onNumbFCS, int nFrom /*= 0*/ )
{
  for ( nFrom; nFrom < CY_MainLogic::kConnection; nFrom++ )
  {
    if( CY_MainLogic::Connection[nFrom].domain!=m_domain )
      continue;
    if( CY_MainLogic::Connection[nFrom].NumbFCS!=onNumbFCS )
      continue;
    if ( strcmp ( CY_MainLogic::Connection[nFrom].Out, ObjRef ))
      continue;
    return nFrom;
  }
  return -1;
}

int CY_MainLogic::FindOutSkipThisFCS( char * ObjRef, int NumbFCSskip, int nFrom /*= 0*/ )
{
  for ( nFrom; nFrom < CY_MainLogic::kConnection; nFrom++ )
  {
    if( CY_MainLogic::Connection[nFrom].domain!=m_domain )
      continue;
    if( CY_MainLogic::Connection[nFrom].NumbFCS==NumbFCSskip )
      continue;
    if ( strcmp ( CY_MainLogic::Connection[nFrom].Out, ObjRef ))
      continue;
    return nFrom;
  }
  return -1;
}

int CY_MainLogic::GetConnections( CY_Base * pB, char * pConn, bool SaveConnections )
{
  LPCSTR const cpConn = pConn;
  char Obj[64] = {0};
  strcpy_s( Obj, pB->Name );
  
  char testConnIO[4] = { 0 };
  testConnIO[0] = pB->TestCNCT( pConn );
  ASS( testConnIO[0] == 'I' || testConnIO[0] == 'O' );

  size_t pnt_obj = strlen ( Obj );
  strcat_s( Obj, "." );
  {
    size_t pt = pnt_obj + 1;
    bool bAdd = false;
    while ( 1 )
    {
      char s = *pConn;
      pConn++;
      if ( s == ':' )
        break;
      if ( s == '>' )
      {
        ASS( !"встретить бы такую ситуацию" );
        continue;
      }
      ASS( pt < _countof(Obj) );
      Obj[pt++] = s;
      bAdd = true;
    }
    if( !bAdd )
    {
      pt = pnt_obj;
    }
    ASS( pt < _countof(Obj) );
    Obj[pt] = 0;
  }

//
  char Ref[64];
  int pr = 0;
  int pnt_ref = 0;
  bool bRefOtherFcs = false;
  while ( 1 )
  {
    char s = *pConn;
    pConn++;
    if ( s == '>' )
    {
      bRefOtherFcs = true;
      continue;
    }
    if ( s == ':' )
      break;
    if ( s == '.' && !pnt_ref )
      pnt_ref = pr;
    ASS( pr < _countof(Ref) );
    Ref[pr++] = s;
  }
  if ( pr == 0 )
    return 1;
  ASS( pr < _countof(Ref) );
  Ref[pr++] = 0;
//
  ASS( *pConn == 'I' || *pConn == 'O' );

  if( *pConn != testConnIO[0] )
  {
    std::stringstream ss;
    ss << "Изменение IO для "  << pB->Name << " связь " << cpConn << "\r\n";
    OutputDebugStringA( ss.str().c_str() );
    pConn = testConnIO;
  }

  if ( *pConn == 'I' )
  {
// Вход блока
    int nIn = -1;
    while( 1 )
    {
      if( bRefOtherFcs )
        nIn = FindInSkipThisFCS( Obj, pB->pDR->NumbFCS, nIn+1 );
      else
        nIn = FindIn ( Obj, pB->pDR->NumbFCS, nIn+1 );
      if( nIn < 0 )
        break;
      const CObjConnection &testCon = Connection[nIn];
      if( !strcmp ( testCon.Out, Ref)
        && !strcmp( testCon.In, Obj) )
        break;
      KKK();
    }
    if ( nIn < 0 )
    {
      ASS(CY_MainLogic::kConnection<_countof(CY_MainLogic::Connection) );
      strcpy_s( CY_MainLogic::Connection[CY_MainLogic::kConnection].In, Obj );
      CY_MainLogic::Connection[CY_MainLogic::kConnection].PntIn = (BYTE)pnt_obj;
			strcpy_s( CY_MainLogic::Connection[CY_MainLogic::kConnection].Out, Ref );
      CY_MainLogic::Connection[CY_MainLogic::kConnection].PntOut = pnt_ref;
      CY_MainLogic::Connection[CY_MainLogic::kConnection].NumbFCS = m_NumbFCS;
      CY_MainLogic::Connection[CY_MainLogic::kConnection].domain = m_domain;
      CY_MainLogic::Connection[CY_MainLogic::kConnection].bRefOtherFcsOut = bRefOtherFcs;
      CY_MainLogic::Connection[CY_MainLogic::kConnection].dirIO = *pConn;//'I'

      size_t l = strlen( CY_MainLogic::Connection[CY_MainLogic::kConnection].In );
      for( size_t i=0; i<l; i++ )
        CY_MainLogic::Connection[CY_MainLogic::kConnection].In[i] = toupper( CY_MainLogic::Connection[CY_MainLogic::kConnection].In[i] );
      l = strlen( CY_MainLogic::Connection[CY_MainLogic::kConnection].Out );
      for( size_t i=0; i<l; i++ )
        CY_MainLogic::Connection[CY_MainLogic::kConnection].Out[i] = toupper( CY_MainLogic::Connection[CY_MainLogic::kConnection].Out[i] );

      CY_MainLogic::kConnection++;
      return 0;
    }
    else
    {
      const CObjConnection &testCon = Connection[nIn];
      bool bRefOtherFcsCon = testCon.dirIO=='I' ? testCon.bRefOtherFcsOut : testCon.bRefOtherFcsIn;
      if( !strcmp ( testCon.Out, Ref)
          && bRefOtherFcsCon == bRefOtherFcs 
          && !strcmp( testCon.In, Obj) )
        return nIn;
    }
    return -1;
  }
  if ( *pConn == 'O' )
  {
// Выход блока
    int nOut = -1;
    while( 1 )
    {
      if( bRefOtherFcs )
        nOut = FindOutSkipThisFCS( Obj, pB->pDR->NumbFCS, nOut+1 );
      else
        nOut = FindOut( Obj, pB->pDR->NumbFCS, nOut+1 );
      if( nOut < 0 )
        break;
      const CObjConnection &testCon = Connection[nOut];
      if( !strcmp ( testCon.In, Ref)
        && !strcmp( testCon.Out, Obj) )
       break;
      KKK();
    }

    if( nOut >= 0 && strcmp( CY_MainLogic::Connection[nOut].In, Ref ) )
    {
      ASS( 0 );
      nOut = -1;
    }
    if( nOut < 0 )
    {
      ASS(CY_MainLogic::kConnection<_countof(CY_MainLogic::Connection) );
      strcpy_s( CY_MainLogic::Connection[CY_MainLogic::kConnection].Out, Obj );
      CY_MainLogic::Connection[CY_MainLogic::kConnection].PntOut = (BYTE)pnt_obj;
			strcpy_s( CY_MainLogic::Connection[CY_MainLogic::kConnection].In, Ref );
      CY_MainLogic::Connection[CY_MainLogic::kConnection].PntIn = pnt_ref;
      CY_MainLogic::Connection[CY_MainLogic::kConnection].NumbFCS = m_NumbFCS;
      CY_MainLogic::Connection[CY_MainLogic::kConnection].domain = m_domain;
      CY_MainLogic::Connection[CY_MainLogic::kConnection].bRefOtherFcsIn = bRefOtherFcs;
      CY_MainLogic::Connection[CY_MainLogic::kConnection].dirIO = *pConn;//'O'

      size_t l = strlen( CY_MainLogic::Connection[CY_MainLogic::kConnection].In );
      for( size_t i=0; i<l; i++ )
        CY_MainLogic::Connection[CY_MainLogic::kConnection].In[i] = toupper( CY_MainLogic::Connection[CY_MainLogic::kConnection].In[i] );
      l = strlen( CY_MainLogic::Connection[CY_MainLogic::kConnection].Out );
      for( size_t i=0; i<l; i++ )
        CY_MainLogic::Connection[CY_MainLogic::kConnection].Out[i] = toupper( CY_MainLogic::Connection[CY_MainLogic::kConnection].Out[i] );

      CY_MainLogic::kConnection++;
      return 0;
    }
    else
    {
      const CObjConnection &testCon = Connection[nOut];
      if( testCon.NumbFCS==m_NumbFCS )
      {
        ASS( testCon.bRefOtherFcsOut == testCon.bRefOtherFcsIn );
        bool bRefOtherFcsCon = testCon.dirIO=='O' ? testCon.bRefOtherFcsOut : testCon.bRefOtherFcsIn;
        if( !strcmp ( testCon.In, Ref)
            && bRefOtherFcsCon == bRefOtherFcs 
            && !strcmp( testCon.Out, Obj) )
          return nOut;
      }
      else
      {
        ASS( testCon.bRefOtherFcsOut != testCon.bRefOtherFcsIn );
        bool bRefOtherFcsCon = testCon.dirIO=='I' ? testCon.bRefOtherFcsOut : testCon.bRefOtherFcsIn;
        if( !strcmp ( testCon.In, Ref)
            && bRefOtherFcsCon == bRefOtherFcs 
            && !strcmp( testCon.Out, Obj) )
          return nOut;
      }
    }
    return -2;
  }
  return -3;
}

static char ParmName[8];
static char ParmValue[2014];
static bool ParmUsed;
char * ListCommNoReadParams[] = {
  //"ESCA",//Scan period
  "SCCP",//Scan coefficient+phase
  //"EINP",//Input conversion
  "PIR!",//Pulse Rate, Pulse buffer
  "ETIM",//Tag mark
  "DBLA",//Double authentication
  "MSBP",//Message bypass for changing status
  "DVNO",//Scale division number
  "ELIM",//Security level
  "FLTR",//Filter number
  "HLPM",//Help message number"
  "SQC!",//  Sqrt lo-input cut value"
  "SREV",//Reverse display of scale"
  "UPPG",//Upper screen name
  "OUTL",//Output conversion:LINEAR;SUBSYS
  "SMET",//??
  //"UPPL",//??
  //"TMNC",//0-однократное выполнение, 1- периодическое
  "MV01",//MV instrument chart display
  "MVSR",//Reverse display of MV"
  "OCMK",//Open-close mark"
  "OTL!", //[1.000:0.000:106.25:-17.19]
  //"OUTP", //Output signal conversion"
  "PINS", //Index ( Tag )
  //"PWF!", //Full-stroke value:Backlash compensated value:Min output width
  "SOAC", //Auxiliary output action(POSITION,VELOCITY)"
  "CASM", //CAS mark"
  "CMPM", //CMP mark"
  "HLLI", //??
  "TCAS", // [CAS]
  "CLRL", //Button color(lower)"
  "LBTY", //Instrument chart label"
  "BCLR", //Button color(lower),"Button color(upper)"
  //"SSI!",//При конв. вх.данных(Data_BIAS,Data_GAIN,H_Limit,L_Limit,1.,0.,100.,0.,"SSI!","%D:%D:%D:%D")
  "AUTF",//AUT fallback
  "BKUP",//Computer backup mode
  "BUAC",//Computer backup mode
  "CNTP",//Control period
  "CNPR",//Control period"
};

int CompParams ( const void * p1, const void * p2 )
  {
  char ** n1 = (char**)p1;
  char ** n2 = (char**)p2;
  return strcmp ( *n1, *n2 );
  }
void SortListParams()
  {
  qsort(ListCommNoReadParams,_countof(ListCommNoReadParams),sizeof(char*),CompParams );
  }
bool NoRead ( char * Name )
  {
  void * pRes = bsearch(&Name,ListCommNoReadParams,_countof(ListCommNoReadParams),sizeof(char*),CompParams );
  if ( pRes )
    return true;
  return false;
  }

static LPCSTR pKnownParamBlocks[] = { "CMDL", "DTLD", "CNIF", "BLIF", "EXTB" };//проектные параметры
static LPCSTR pConnectionBlocks[] = { "HSRC", "LCSR" };//выбирает, в том числе, связи

static LPCSTR pTestHeaderNames[] = 
{
  "TAGI","HTLT","FTLT","EDWK","BKCM","EXTB",
  //----------
  "CMDL", "DTLD", "CNIF", "BLIF",
  "HSRC", "LCSR",
  "CLSR",
  "HISR",
  //----------
  "LHED","GBLK","GCNC","GTXT","GCOD",
  "GSTR",//интересный перечень
  "GGRP",
  "GGRM",
  "SDIT",
  "UDIT",
  "UDMT",
  "SDFX",
  "FTCM",
  "PRGN",
  "BUEX",
  //FF
  "FBFP",
  "FBCN",
  //_UTAS
  "GNMT",
  "GNMS",
  "IMOP",
  "GUID",
  "CNEX",
};

void CY_MainLogic::ReadObjParams ( char * EDF, CY_Base * pB, bool SaveConnections )
{
  KAutoCloser<HANDLE> hFile( CreateFile( EDF,
	                                        GENERIC_READ,
	                                        FILE_SHARE_READ|FILE_SHARE_WRITE,
	                                        NULL,
	                                        OPEN_EXISTING,
	                                        0,
	                                        NULL),
                             CloseHandle );
  if ( hFile ==	INVALID_HANDLE_VALUE	)
	  return;

  DWORD L = GetFileSize( hFile, NULL );
  std::vector<BYTE> btBuf;
  btBuf.resize( L+1, '\0' );
  BYTE * pData = &btBuf[0];
  DWORD Lr;
  ReadFile( hFile, pData, L, &Lr, NULL );

  SYokogawa200 *pHdrY = (SYokogawa200*)pData;

#ifdef _DEBUG
  for( int i=0; i<_countof(pHdrY->id); ++i )
  {
    if( !pHdrY->id[i].dwID )
      continue;
    bool bKnown = false;
    for( int j=0;j<_countof(pTestHeaderNames); ++j)
    {
      if( pHdrY->id[i].dwID == *(DWORD*)pTestHeaderNames[j] )
      {
        bKnown = true;
        break;
      }
    }
    if( !bKnown )
    {
      void *p = pHdrY->id[i].Item( pHdrY, 0);
      OutputDebugString( pHdrY->id[i].szID );
      OutputDebugString( "\n" );
      ASS(bKnown);
      KKK();
    }
  }
#endif

	for ( int B = 0; B < _countof(pKnownParamBlocks); B++ )//в последующем можно заменить на выборку этих блоков
  {
    SYokogawaID* pBlk = FindYokoID( pHdrY, pKnownParamBlocks[B] );
    if( !pBlk )
      continue;
    if( !pBlk->dwCount )
      continue;

    LPSTR P = (LPSTR)pBlk->Item( pData, 0 );
    TStringTerminator st( P[pBlk->dwSize] );
		int p = 0;
		bool IsName = true;
		while ( *P )
		{
      if( 0xd==*P || 0xa==*P )
      {
        ++P;
        continue;
      }
			if ( IsName )
			{
        memmove ( ParmName, P, 4 );
				ParmName[4] = 0;
				P += 5;
				IsName = false;
        ZeroMemory( ParmValue, sizeof(ParmValue) );
				continue;
			}
			if ( *P == ';' )
			{
				ParmValue[p] = 0;
				p = 0;
				IsName = true;
				P++;
        if ( strcmp ( ParmName, "CNCT" ) == 0 )
        {
          int Res = GetConnections( pB, ParmValue, SaveConnections );
          ASS( Res>=0 );
				  continue;
				}
        if ( NoRead ( ParmName ))
  				continue;
        bool Res = pB->ReadParm( ParmName, ParmValue, pData );
				continue;
			}
      ASS( p < (_countof(ParmValue)-1) );
      if ( p < (_countof(ParmValue)-1) )
			ParmValue[p++] = *P;
			P++;
		}
//////////////////////////////////////////////////
	}

  for( int iCB=0; iCB<_countof(pConnectionBlocks); iCB++ )
  {
    SYokogawaID* pCB = FindYokoID( pHdrY, pConnectionBlocks[iCB] );
    if( !pCB || !pCB->dwCount )
      continue;

    LPSTR P = (LPSTR)pCB->Item( pData, 0 );
    TStringTerminator st( P[pCB->dwSize] );

		P += 16;
		int p = 0;
		bool IsName = true;
		while ( *P )
		{
			if ( *P == 0x0D || *P == 0x0A )
			{
				P++;
				continue;
			}
// До первого :
			if ( IsName )
			{
				while ( *P != ':' )
					P++;
				P++;
				if ( strncmp ( P, ":FSTB", 5 ) == 0 )
					break;
        memmove ( ParmName, P, 4 );
				ParmName[4] = 0;
				P += 5;
				IsName = false;
				continue;
			}
			if ( *P == ';' && P[-1]!='\\' )
			{
				ParmValue[p] = 0;
				p = 0;
				IsName = true;
				P++;
        if ( strcmp ( ParmName, "CNCT" ) == 0 )
        {
          int Res = GetConnections( pB, ParmValue, SaveConnections );
          ASS( Res>=0 );
				  continue;
		    }
        if ( NoRead ( ParmName ))
  				continue;
        bool Res = pB->ReadParm( ParmName, ParmValue, pData );
				continue;
			}
      ASS( p < (_countof(ParmValue)-1) );
			if ( p < (_countof(ParmValue)-1) )
				ParmValue[p++] = *P;
			P++;
		}
  }

  SYokogawaID* pCLSR = FindYokoID( pHdrY, "CLSR" );
  if( pCLSR )
  {
    static LPCSTR pSource = ":::SOURCE";
    static size_t lSource = strlen( pSource );

    ASS( !strcmp( pB->Model, "CALCU") || !strcmp( pB->Model, "CALCU-C") );
    ASS( pCLSR->dwCount==1 && pCLSR->dwSize > 0 );
    LPSTR Src = (LPSTR)pCLSR->Item( pData, 0 );
    LPSTR Beg = strstr( Src, pSource );
    ASS(Beg && Beg==Src );
    Beg += 27;
    char * End = strstr( Beg, pSource );
    ASS(End);
    End -= 20;
    if( End > Beg )
    {
      TStringTerminator st( *End );
      pB->ReadParm( "CLSR", Beg, pData );
    }
  }

  SYokogawaID* pUDIT = FindYokoID( pHdrY, "UDIT" );
  if( pUDIT )
    pB->ReadParm( "UDIT", NULL, pData );

  SYokogawaID* pHISR = FindYokoID( pHdrY, "HISR" );
  if( pHISR && pHISR->dwCount && pHISR->dwSize )
  {
    LPSTR Src = (LPSTR)pHISR->Item( pData, 0 );
    TStringTerminator st( &Src[pHISR->dwSize] );
    pB->ReadParm( "HISR", Src, pData );
  }
}

void CY_MainLogic::ObrEdf( char * File )
{
  std::vector<BYTE> btBuf;
  GetProjectFile( File, btBuf );
  if( btBuf.empty() )
    return;
  BYTE * pData = &btBuf[0];
	
  SYokogawa200* hdr = (SYokogawa200*)pData;

  SYokogawaID* datRGTL = FindYokoID( hdr, "RGTL" );

  if( !datRGTL || datRGTL->dwCount==0 )
    return;

///////////////////////////////////////////////////////////
  ASS(CY_MainLogic::kDR<_countof(DR) )
  CY_DR * pDR = new CY_DR;
  CY_MainLogic::DR[CY_MainLogic::kDR++] = pDR;
  strcpy_s( pDR->FCS, m_szFCS );
  strcat_s( pDR->FCS, "/" );
  strcat_s( pDR->FCS, m_szDR );
  pDR->NumbFCS = m_NumbFCS;
  pDR->domain = m_domain;
  SYokogawaID* datBKCM = FindYokoID( hdr, "BKCM" );
  if( datBKCM && datBKCM->dwCount > 0 )
    memcpy( pDR->Comment, datBKCM->Item( hdr, 0), __min(_countof(pDR->Comment), datBKCM->dwSize) );
  else
    memset( pDR->Comment, 0, _countof(pDR->Comment) );

  pDR->nYObjects = datRGTL->dwCount;

///////////////////////////////////////////////////////////
// Параметры
///////////////////////////////////////////////////////////
  char Path[1024];
//////////////////////////////////////////////////////////
  SYokogawaID* datGBLK = FindYokoID( hdr, "GBLK" );
  //int kS = datGBLK->dwCount;
#pragma message("!!!!! Проверить на исходном коде")
  //ASS( datGBLK->dwCount==datRGTL->dwCount );

	short * pS = (short*)datGBLK->Item( hdr, 0 );
// Основные блоки 
  CY_Base * Base[200] = { 0 };
  ASS( datRGTL->dwCount < _countof(Base) );
  DWORD kB = datRGTL->dwCount;
  DWORD kBase = datRGTL->dwCount;
  
  char szBlkName[20];

  for ( DWORD n = 0; n < datRGTL->dwCount; n++ )
  {
    SRGTL * pBlk = (SRGTL*)datRGTL->Item( hdr, n );
    ZeroMemory( szBlkName, sizeof(szBlkName) );
    CopyMemory( szBlkName, pBlk->Name, sizeof(pBlk->Name) );

    CY_Base * pB_Old = CY_MainLogic::FindYObj( szBlkName, true, m_NumbFCS, m_domain );
    ASS( !pB_Old );
    if ( pB_Old && pB_Old->pDR->NumbFCS==m_NumbFCS  )
    {
      ASS( !"не должно быть одинаковых имен в одном контроллере" );
      Base[n] = NULL;
      continue;
    }
    CY_Base * pB = CreateYobj( szBlkName, pBlk->Model, pDR->NumbFCS, pDR->domain );
    if( !pB )
	  {
		  pB = CreateYobj( szBlkName, pBlk->cXZ, pDR->NumbFCS, pDR->domain );
      if( pB )
        strcpy_s( pBlk->Model, pBlk->cXZ );
	  }
    //ASS( pB );
    if( pB == NULL )
    {
      OutputDebugString( "!!! нет модели ");
      OutputDebugString( pBlk->Model );
      OutputDebugString( ", блока \"");
      OutputDebugString( szBlkName );
      OutputDebugString( "\"\n");
      Base[n] = NULL;
      continue;
    }

    pB->sequenceNumberInDR = n+1;
    {
      tNamedString_::iterator it = mNumberOfTags.find( szBlkName );
      ASSD( mNumberOfTags.end()!=it );
      if( mNumberOfTags.end()!=it )
        strcpy_s( pB->szNumber, it->second.c_str() );
      else
        strcpy_s( pB->szNumber, szBlkName );
    }
    pB->pDR = pDR;
    pDR->AddYBlk( pB );
    CY_MainLogic::AddYObj ( pB );
    Base[n] = pB;
    memset( pB->Name, 0, sizeof ( pB->Name ));
	  memcpy ( pB->Name, szBlkName, 16 );
    pB->Name[16] = 0;
    memset( pB->Model, 0, sizeof ( pB->Model ));
		strcpy_s ( pB->Model, sizeof ( pB->Model ),	pBlk->Model );
    pB->SetObjName();
    OnYobjFullNameReady( pB );
    pB->SetParmList( );
    strcpy_s( Path, File );
    char * P = strrchr ( Path, '.' );
    *P = 0;
    strcat_s( Path, "\\" );
    strcat_s( Path, pB->Name );
    strcat_s( Path, ".edf" );
		bool SaveConnections;
		if ( datGBLK->dwCount == 0 )
			SaveConnections = true;
		else
			SaveConnections = false;
    pB->firstConnectionK = kConnection;
    ReadObjParams ( Path, pB, SaveConnections );
    pB->lastConnectionK = kConnection;
    KKK();
  }
///////////////////////////////////////////////////////////
// FREF - ссылки (PIO)
///////////////////////////////////////////////////////////
  SYokogawaID* datFREF = FindYokoID( hdr, "FREF" );
  ns_local::CRefName * pRef = NULL;
  DWORD K = 0;
  if( datFREF )
  {
    pRef = (ns_local::CRefName*)datFREF->Item( hdr, 0 );
	  K = datFREF->dwCount;
  }

  CY_Base * PIO[200] = { 0 };
  ASS( K < _countof(PIO) );
//////////////////////////////////////////////////////////
//  pS[0] - признак PIO == 2
//  pS[1] - номер блока
  kB += K;
  DWORD kPIO = K;
  for ( DWORD n = 0; n < K; n++, pRef++ )
  {
    if( !pRef->Name[0] )
    {
      PIO[n] = 0;
      continue;
    }
    CY_Base * pB_Old = CY_MainLogic::FindYObj ( pRef->Name, true, m_NumbFCS, m_domain );
    if ( pB_Old )
    {
      PIO[n] = 0;
      continue;
    }
    if( 'A'==pRef->Model[0] )
    {
      LPCSTR pszP = strchr( pRef->Name, '.' );
      if( !pszP )
      {
        PIO[n] = 0;
        continue;
      }
    }
    CY_Base * pB = CreateYobj( pRef->Name, pRef->Model, pDR->NumbFCS, pDR->domain );
    ASS( pB );
    if ( pB == NULL )
    {
      PIO[n] = 0;
      continue;
    }
    pB->pDR = pDR;
    pB->sequenceNumberInDR = 0;
    CY_MainLogic::AddYObj ( pB );
    PIO[n] = pB;
    memset( pB->Name, 0, sizeof( pB->Name ));
		strcpy_s( pB->Name, pRef->Name );
    memset( pB->Model, 0, sizeof( pB->Model ));
		strcpy_s( pB->Model,	pRef->Model );
    pB->SetObjName();
    OnYobjFullNameReady( pB );
	}

  K = 0;
  for( int i=0; i<_countof(PIO); ++i )
  {
    if(PIO[i])
      ++K;
  }
  pDR->nPIOObjects = K;
  for( int i=0; i<_countof(PIO); ++i )
  {
    if(!PIO[i])
      continue;
    pDR->AddPIO( PIO[i] );
  }
// Координаты
////////////////////////////////////////
  if ( datGBLK->dwCount > 0 )
    {
    for ( DWORD n = 0; n < kB; n++, pS += 16 )
		{
      CY_Base * pB;
      DWORD N = pS[1];
      if ( pS[0] == 1 )
      {
        if( N>=kBase )
          continue;
        pB = Base[N];
      }
      else
      {
// признак PIO
        if( N>=kPIO )
          continue;
        pB = PIO[N];
      }
      if ( pB == NULL )
        continue;

      pB->GeomType = (BYTE)pS[0]; 
		  pB->GeomSymbol = (BYTE)pS[2];
		  pB->x = pS[4];
		  pB->y = pS[5];
    }
  }
  else
  {
    int kX = (int)sqrt( (double)kB ) + 1;
    int nX = 0;
    int dX = 200;
    int dY = 200;
    int x0 = dX;
    int y0 = dY;
    for ( DWORD n = 0; n < kB; n++ )
    {
      CY_Base * pB = Base[n];
      if( pB )
      {
        pB->GeomType = 1; 
		    pB->GeomSymbol = 1;
		    pB->x = x0;
		    pB->y = y0;
      }
      x0 += dX;
      nX++;
      if ( nX >= kX )
      {
        nX = 0;
        x0 = dX;
        y0 += dY;
      }
    }
  }
// GCOD - точки линий рисунка
	ZeroMemory(pDR->LinePnt, sizeof(pDR->LinePnt));
  K = 0;
  pS = NULL;
  SYokogawaID* datGCOD = FindYokoID( hdr, "GCOD" );
  ASS( datGCOD );
  if( datGCOD )
  {
    K = datGCOD->dwCount;
    if( K > 0 )
      pS = (short*)datGCOD->Item( hdr, 0 );
		}

  ASS( K < CY_DR::MAX_DR_PNT )
  pDR->kLinePnt = K;
  if ( K > 0 )
  {
	  for ( DWORD n = 0; n < K; n++ )
    {
		  pDR->LinePnt[n].x = pS[0];
		  pDR->LinePnt[n].y = pS[1];
		  pS += 2;
		}
  }

  SYokogawaID* datCNIF = FindYokoID( hdr, "CNIF" );
  if( !datCNIF || !datCNIF->dwCount )
    return;
  LPSTR P = (LPSTR)datCNIF->Item( pData, 0 );
  TStringTerminator st( P[datCNIF->dwSize] );
  int p = 0;
  bool IsName = true;
  while ( *P )
  {
    if ( IsName )
    {
      memmove ( ParmName, P, 4 );
      ParmName[4] = 0;
      P += 5;
      IsName = false;
      continue;
    }
    if ( *P == ';' )
    {
      ParmValue[p] = 0;
      p = 0;
      IsName = true;
      P++;
      if( strcmp ( ParmName, "CNCT" ) == 0 )
      {
        size_t len = strlen(ParmValue);
        size_t n = strcspn( ParmValue, ".:" );
        if( n < len )
        {
          char *pName = ParmValue;
          CY_Base *pB = NULL;
          {
            TStringTerminator st( ParmValue[n] );
            pB = FindYObj( pName );
            //ASS( pB );
          }
          LPSTR pIO = strrchr( ParmValue, ':' );
          char prev = pIO[1];
          switch( prev )
          {
          case 'I':
            pIO[1] = 'O';
            break;
          case 'O':
            pIO[1] = 'I';
            break;
          default:
            ASS(0)
          }

          if( pB && ':'==ParmValue[n] )
          {
            int Res = GetConnections( pB, &ParmValue[n], false );
            KKK();
          }
          else if( pB && '.'==ParmValue[n] )
          {
            int Res = GetConnections( pB, &ParmValue[n+1], false );
            KKK();
          }
          pIO[1] = prev;
        }
        continue;
      }
    }
    ASS( p < (_countof(ParmValue)-1) );
    if ( p < (_countof(ParmValue)-1) )
      ParmValue[p++] = *P;
    P++;
  }
}


void CY_MainLogic::ObrTunning( char * File )
{
	HANDLE hFile = CreateFile(
		File,
		GENERIC_READ,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL );
	if ( hFile ==	INVALID_HANDLE_VALUE	)
		return;
	DWORD L = GetFileSize( hFile, NULL );
  char * pData = new char[L+1];
  DWORD Lr;
  ReadFile ( hFile, pData, L, &Lr, NULL );
	CloseHandle ( hFile );
  pData[L] = 0;
//////////////////////////////////
  char * P = strchr ( pData, ';' );
  ASS( P );
  while ( P )
  {
    for ( int n = 0; n < 3; n++ )
      {
      P = strchr ( P, ':' );
      if ( P == NULL )
        break;
      P++;
      }
    if ( P == NULL )
      break;
 //
    bool OK = false;
    if ( strncmp ( P, "NON CATEGORY:", 13 ) == 0 )
      {
      OK = true;
      P += 13;
//      %BL0017:17:TR-SS:
      }
    if ( strncmp ( P, "ADL:", 4 ) == 0 )
      {
      OK = true;
      P += 4;
      }
    if ( strncmp ( P, "DR", 2 ) == 0 )
      {
//      DR0020.edf:TRSA1120I:452:PVI:
      OK = true;
      P = strchr ( P, ':' );
      P++;
//      TRSA1120I:452:PVI:
      }
    if ( strncmp ( P, ":SOURCE", 7 ) == 0 )
      {
      break;
      }
    ASS(OK)
    if ( !OK )
      break;
    char * ObjName = P;
    P = strchr ( P, ':' );
    if ( P == NULL )
      break;
    *P = 0;
    P++;
    CY_Base *pY = CY_MainLogic::FindYObj( ObjName, true, m_NumbFCS, m_domain );
    char * Pn = strchr ( P, ';' );
    if ( Pn == NULL )
      break;
    *Pn = 0;
    Pn++;
    if ( pY == NULL )
      {
      P = Pn;
      continue;
      }
    SetTunningValues( pY, P );
    pY->OnReadTunningData();
    P = Pn;
  }
//////////////////////////////////
  delete pData;
}
