#include <vector>
#include "Y_MainLogic.h"
#include <macros/AutoCloser.h>
//#include <rsuPaths.h>
#include <CommonRSUs.h>
//#include "CommProc.h"

//#include "sr.h"
#include <SR_DCU.h>

static LPCSTR s_szTemporaryExt = "tmp";

int CompYBase( const void * pP1, const void * pP2 );

void CY_MainLogic::SaveAllTo( std::fstream& hFile )
{
  DWORD Lw;
  WRT_KEY( kKeyVersion )
  WRT_VAR( s_CurVersion );
// DR
  WRT_KEY(eKeyDrCount)
  WRT_VAR(kDR)
  for ( int n = 0; n < kDR; n++ )
  {
    CY_DR * pDR = DR[n];
    WRT_KEY(eKeyDrNext)
    WRT_DATA(sizeof(CY_DR),pDR)
  }
  WRT_KEY(eKeyDrEnd)// Конец DR
// Объекты
  WRT_KEY(eKeyObjCount)// Объекты
  WRT_VAR(kY_Obj)
  for ( int n = 0; n < kY_Obj; n++ )
  {
    CY_Base * pY = pY_Obj[n];
    WRT_KEY(eKeyObjNext)
    WRT_STR(pY->Name)
    WRT_STR(pY->Model)
    WRT_VAR(pY->Y_Type) 
    WRT_STR(pY->pDR->FCS)
    WRT_VAR(pY->GeomType)
    WRT_VAR(pY->GeomSymbol)
    WRT_VAR(pY->x) 
    WRT_VAR(pY->y)
    WRT_VAR(pY->sequenceNumberInDR)
    WRT_STR(pY->szNumber)
    WRT_STR(pY->szUPPL)
    WRT_VAR(pY->firstConnectionK) 
    WRT_VAR(pY->lastConnectionK)
    WRT_VAR(pY->controllerType)
    WRT_VAR(pY->ESCA)
    if ( !pY->SaveParms( hFile ))
    {
      WRT_VAR(pY->L_Class_P)
      if ( pY->L_Class_P > 0 && pY->pClass_P )
      {
        WRT_DATA(pY->L_Class_P,pY->pClass_P)
      }
    }
  }
  WRT_KEY(eKeyObjEnd)// Конец объектов
// Дискреты
  WRT_KEY(eKeyDiscretCount)
  WRT_VAR(kD_Obj)
  for ( int n = 0; n < kD_Obj; n++ )
  {
    CY_DI * pY = (CY_DI*)pD_Obj[n];
    WRT_KEY(eKeyDiscretNext)
    WRT_STR(pY->Name)
    WRT_VAR(pY->Y_Type)
    WRT_VAR(pY->Numb)
    WRT_VAR(pY->NumbFCS)
    WRT_VAR(pY->domain)
    WRT_STR(pY->Comment)
    WRT_VAR(pY->pointTypeZ)
    WRT_VAR(pY->modelTypeZ)
    WRT_STR(pY->szAkaObjName);
    WRT_VAR(pY->AlarmLevel);
    WRT_VAR(pY->IOZ);
    WRT_STR(pY->szNumber)
    WRT_STR(pY->szUPPL)
    WRT_VAR(pY->firstConnectionK) 
    WRT_VAR(pY->lastConnectionK)
    WRT_VAR(pY->controllerType)
    WRT_VAR(pY->ESCA)
  }
  WRT_KEY(eKeyDiscretEnd)// Конец дискретов
// Связи
  WRT_KEY(eKeyConnectionCount)
  WRT_VAR(kConnection)
  WRT_DATA(kConnection*sizeof(CObjConnection),Connection)
  WRT_KEY(eKeyConnectionEnd)
}

void CY_MainLogic::SaveAll()
{
  char Path[_MAX_PATH];
  GetYokoDatPath( Path, s_szTemporaryExt );
  {
      std::fstream hFile(Path, std::ios::binary | std::ios::trunc);
      if (!hFile.is_open())
      {
          //Fsend ( "Y_err.txt", "Ошибка открытия %s\n", Path );
          return;
      }

      SaveAllTo(hFile);
  }
  char drive[_MAX_DRIVE];
  char dir[_MAX_DIR];
  char fname[_MAX_FNAME];
  char ext[_MAX_EXT];
  char path_buffer[_MAX_PATH] = { 0 };
  char filenameinzip[_MAX_FNAME] = { 0 };

  _splitpath_s( Path, drive, _countof(drive), dir, _countof(dir), fname, _countof(fname), ext, _countof(ext) );
  _makepath_s( path_buffer, _countof(path_buffer), drive, dir, fname, "dat" );

  sprintf_s( filenameinzip, "%s.dat", fname );

  int r = RsuPackager( Path, path_buffer, filenameinzip );
  ASS( !r );
  if( r )
    return;
  DeleteFile( Path );
}

template<class T>
bool ReadKey( T k, std::fstream &hFile )
{
  READ_KEY( k, false );
  return true;
}

#undef READ_KEY
#define READ_KEY( Key )\
  if( !ReadKey(Key, hFile) )\
    return err_WrongKey;

CY_MainLogic::e_RestResult CY_MainLogic::RestAll()
{
  OutputDebugString( "Y restoring...\n" );
  ULONGLONG s = GetTickCount64();
  char Path[1024];
  GetYokoDatPath( Path );

  std::fstream hFile;
  if (!RsuExtractor(hFile, Path))
  {
      return err_CantOpen;
  }

  //KAutoCloser<HANDLE> hFile( RsuExtractor(Path), CloseHandle );
	//if ( !hFile || INVALID_HANDLE_VALUE==hFile	)
		//return err_CantOpen;

  DWORD Lr;
  DWORD ver = 0;
  READ_KEY( kKeyVersion )
  READ_VAR(ver);
  if( s_CurVersion!=ver )
  {
    if( s_CurVersion < ver )
      return err_DatVerGreaterSrcVer;
    return err_WrongVer;
  }

  InitParm();
// DR
  int domainTest = 0;
  READ_KEY(eKeyDrCount)
  READ_VAR(kDR)
  ASS( kDR > 0 && kDR < _countof(DR) );
  for ( int n = 0; n < kDR; n++ )
  {
    CY_DR * pDR = new CY_DR;
    DR[n] = pDR;
    READ_KEY(eKeyDrNext)
    READ_DATA(sizeof(CY_DR),pDR)
    pDR->m_pBlks = NULL;
    pDR->m_pPios = NULL;
    if( !domainTest )
    {
      domainTest = pDR->domain;
      m_pCentumSystem->domains[0] = pDR->domain;
    }
    else if( domainTest != pDR->domain )
    {
      for( int i=0; i<_countof(m_pCentumSystem->domains); ++i )
      {
        if( !m_pCentumSystem->domains[i] )
        {
          m_pCentumSystem->domains[i] = pDR->domain;
          break;
        }
        if( m_pCentumSystem->domains[i]==pDR->domain )
          break;
      }
      s_bFcsMultiDomain = true;
      domainTest = pDR->domain;
    }
  }
  READ_KEY(eKeyDrEnd)// Конец DR
// Объекты
  READ_KEY(eKeyObjCount)// Объекты
  READ_VAR(kY_Obj)
  ASS( kY_Obj > 0 && kY_Obj < _countof(pY_Obj));
	char Name[64*4];
	char Model[20*4];
  char FCS[32*4];
  int Y_Type;
  CY_DR * pDR = NULL;
  for ( int n = 0; n < kY_Obj; n++ )
  {
    READ_KEY(eKeyObjNext)
    READ_STR(Name)
    READ_STR(Model)
    READ_VAR(Y_Type) 
    READ_STR(FCS)
    pDR = FindDR ( FCS );
    ASSR( pDR, err_CommonErr );
    
    CY_Base * pY = CreateYobj( Name, Model, pDR->NumbFCS, pDR->domain );

    ASSR( pY, err_CommonErr );
    strcpy_s( pY->Name, Name );
    strcpy_s( pY->Model, Model );
    pY->Y_Type = Y_Type;
    pY->pDR = pDR;
    pY_Obj[n] = pY;
    READ_VAR(pY->GeomType)
    READ_VAR(pY->GeomSymbol)
    READ_VAR(pY->x) 
    READ_VAR(pY->y)
    READ_VAR(pY->sequenceNumberInDR)
    READ_STR(pY->szNumber)
    READ_STR(pY->szUPPL)
    READ_VAR(pY->firstConnectionK) 
    READ_VAR(pY->lastConnectionK)
    READ_VAR(pY->controllerType)
    READ_VAR(pY->ESCA)
    pDR->AddYBlk( pY );
    pDR->AddPIO( pY );
///////////////////////////////////////////////////////
    pY->SetObjName();
    OnYobjFullNameReady( pY );

    pY->SetParmList( );
    if ( !pY->RestParms( hFile ))
      {
      int L = 0;
      READ_VAR(L)
      if ( L > 0 && pY->pClass_P )
        {
        READ_DATA( min(pY->L_Class_P, L),pY->pClass_P)
        }
      }
    pY = NULL;
  }
  READ_KEY(eKeyObjEnd)// Конец объектов
  qsort ( pY_Obj, kY_Obj, sizeof ( CY_Base *), CompYBase );
// Дискреты
  READ_KEY(eKeyDiscretCount)
  READ_VAR(kD_Obj)
  for ( int n = 0; n < kD_Obj; n++ )
  {
    READ_KEY(eKeyDiscretNext)
    READ_STR(Name)
    READ_VAR(Y_Type)
    int Numb = -1;
    int NumbFCS = -1;
    int domain = -1;
    READ_VAR(Numb)
    READ_VAR(NumbFCS)
    READ_VAR(domain)
    CY_DI * pY = CreateDobj( Name, Y_Type, NumbFCS, domain, Numb );
    ASS( pY );
    pD_Obj[n] = pY;
    pY->NumbFCS = NumbFCS;
    pY->domain = domain;
    READ_STR(pY->Comment)
    READ_VAR(pY->pointTypeZ)
    READ_VAR(pY->modelTypeZ)
    char buff[64*4];
    READ_STR(buff);
    
    strcpy_s( pY->szAkaObjName, buff );
    
    READ_VAR(pY->AlarmLevel)
    READ_VAR(pY->IOZ);
    READ_STR(pY->szNumber)
    READ_STR(pY->szUPPL)
    READ_VAR(pY->firstConnectionK) 
    READ_VAR(pY->lastConnectionK)
    READ_VAR(pY->controllerType)
    READ_VAR(pY->ESCA)

    char * Model = "";
    switch ( pY->Y_Type )
		{
      case id__Y:
        Model = "Y";
      break;
			case id__Z:
        Model = "Z";
			break;
			case id__AN:
        Model = "AN";
			break;
			case id__WB:
        Model = "WB";
			break;
			case id__GS:
        Model = "GS";
			break;
			case id__SW:
        Model = "SW";
			break;
			case id__PR:
        Model = "PR";
			break;
			case id__OG:
        Model = "OG";
			break;
      case id__RQ:
        Model = "RQ";
        break;
      case id__WW:
        Model = "WW";
        break;
      case id__XW:
        Model = "XW";
        break;
      default:
        ASS( 0 );
			}
    strcpy_s( pY->Model, Model );
  }
  READ_KEY(eKeyDiscretEnd)// Конец дискретов
// Связи
  READ_KEY(eKeyConnectionCount)
  READ_VAR(kConnection)
  READ_DATA(kConnection*sizeof(CObjConnection),Connection)
  READ_KEY(eKeyConnectionEnd)

  ULONGLONG d = GetTickCount64() - s;
  char szDebug[64];
  sprintf_s( szDebug, "Y restored (%u ms)\n", d );
  OutputDebugString( szDebug );

  return err_Ok;
}

void CY_MainLogic::Delete_Data_Y()
{
  char Path[1024];
	GetYokoDatPath( Path );
  DeleteFile( Path );
}

#include <io.h>

bool CY_MainLogic::Test_Data_Y()
{
  char Path[_MAX_PATH];
	GetYokoDatPath( Path );
//
  errno_t Res = _access_s( Path, 4 );
  if ( Res == 0 )
    return true;
  return false;
}
