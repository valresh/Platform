#include <rsuErr.h>
#include "ControlBuilder.h"
#include <macros/AutoCloser.h>
#include <SR_DCU.h>
#include "stateKeys.h"
#include <macros/StrHelps.h>

#include <minizip/unzip.h>
#include <minizip/zip.h>
#include <zlibH.h>

static const DWORD s_CurVersionPrj = 0x5100001F;

void KControlBuilder::SaveProject( SH_Module *pM )
{
  char Path[_MAX_PATH];
  GetDatFilePath( Path, "tmp" );

  {
      std::fstream hFile = std::fstream(Path, std::ios::out | std::ios::binary | std::ios::trunc);
    if( !hFile.is_open())
      return;
    DWORD Lw;
    WRT_KEY( kKeyVersion )
    WRT_VAR( s_CurVersionPrj );

    SaveModule( pM, hFile );

    WRT_KEY( kKeyEnd )
  }
  char path_buffer[_MAX_PATH] = { 0 };
  GetDatFilePath( path_buffer );

  zipFile zf = zipOpen( path_buffer, 0);
  if( !zf )
    return;

  fs::path path(Path);
  std::string filenameinzip = path.replace_extension(".datx").filename().string();

  zip_fileinfo zi;
  unsigned long crcFile=0;
  int size_buf = 8192;
  std::vector<BYTE> buf;
  buf.resize( size_buf );

  /*zi.tmz_date.tm_sec = zi.tmz_date.tm_min = zi.tmz_date.tm_hour =
    zi.tmz_date.tm_mday = zi.tmz_date.tm_mon = zi.tmz_date.tm_year = 0;
  zi.dosDate = 0;
  zi.internal_fa = 0;
  zi.external_fa = 0;
  filetime(Path,&zi.tmz_date,&zi.dosDate);*/

  int err = getFileCrc( Path, &buf[0], size_buf, &crcFile);

  err = zipOpenNewFileInZip3( zf,filenameinzip.c_str(), &zi, NULL, 0, NULL, 0, NULL, Z_DEFLATED, 8, 0, -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY, NULL, crcFile);

  {
    KAutoCloser<FILE*> fin( fopen(Path,"rb"), fclose);
    if( fin )
    {
      int size_read = 0;
      do
      {
        err = ZIP_OK;
        size_read = (int)fread( &buf[0],1,size_buf,fin);
        if (size_read < size_buf)
          if (feof(fin)==0)
          {
            err = ZIP_ERRNO;
          }

          if (size_read>0)
          {
            err = zipWriteInFileInZip (zf, &buf[0],size_read);
          }
      } while ((err == ZIP_OK) && (size_read>0));
      err = zipCloseFileInZip(zf);
    }
  }
  zipClose( zf, NULL);
  fs::remove(fs::path( Path ));
}

void KControlBuilder::SaveModule( SH_Module *pM, std::fstream& hFile )
{
  SaveBase( pM, hFile );
  DWORD Lw;
  WRT_VAR( pM->inConsC );
  if( pM->inConsC )
    WRT_DATA( pM->inConsC*sizeof(*pM->pInConns), pM->pInConns );
  WRT_VAR( pM->nSubModules );
  WRT_VAR( pM->nBlocks );
  WRT_STR( pM->szFile );
  WRT_KEY( kKeyBlockData );
  pM->SaveBlock( hFile );
  for( size_t i=0; i < pM->nSubModules; ++i )
  {
    SaveModule( pM->ppSubModules[i], hFile );
  }

  for( size_t i=0; i < pM->nBlocks; ++i )
  {
    SaveBlock( pM->ppBlocks[i], hFile );
  }
}

void KControlBuilder::SaveBlock( SH_Block *pB, std::fstream& hFile )
{
  SaveBase( pB, hFile );
  DWORD Lw;
  WRT_VAR( pB->inConsC );
  if( pB->inConsC )
    WRT_DATA( pB->inConsC*sizeof(*pB->pInConns), pB->pInConns );
  WRT_KEY( kKeyBlockData );
  pB->SaveBlock( hFile );
}

void KControlBuilder::SaveBase( KBmBase *pB, std::fstream& hFile )
{
  DWORD Lw;
  WRT_KEY( kKeyBlockBegin );
  WRT_STR( pB->BlockName );
  WRT_STR( pB->EntityName );
  WRT_STR( pB->TypeName );
  WRT_STR( pB->AssignedTo );
  WRT_STR( pB->Container );
  WRT_VAR( pB->L_Class_FullW );
  if ( pB->L_Class_FullW > 0 && pB->pClass_FullW )
    WRT_DATA( pB->L_Class_FullW, pB->pClass_FullW );
}

std::fstream KControlBuilder::GetSavedProject( LPCSTR pszFile )
{
  //DWORD dwFlagsAndAttributes = FILE_FLAG_SEQUENTIAL_SCAN;

  KAutoCloser<unzFile> uf( unzOpen( pszFile ), unzClose );
  char path_buffer[_MAX_PATH] = { 0 };

  if( NULL!=uf )
  {
    fs::path path(pszFile);
    std::string tmpFileName = path.replace_extension(path.extension().string() + "tmp").string();

    int err = UNZ_OK;

    char filename_inzip[256];
    unz_file_info file_info;
    err = unzGetCurrentFileInfo(uf,&file_info,filename_inzip,_countof(filename_inzip),NULL,0,NULL,0);
    if( err==UNZ_OK )
    {
      err = unzOpenCurrentFilePassword( uf, NULL );
      KAutoCloser<FILE*> fout(fopen(tmpFileName.c_str(), "wb"), fclose);
      if( fout!=NULL )
      {
        std::vector<BYTE> buf;
        buf.resize( 8192 );
        do
        {
          err = unzReadCurrentFile( uf, &buf[0], (unsigned int)buf.size() );
          if (err>0)
          {
            if( fwrite( &buf[0], err, 1, fout )!=1)
            {
              err = UNZ_ERRNO;
              break;
            }
          }
        }while (err>0);
        if (UNZ_OK == err)
        {
            std::fstream res(tmpFileName.c_str(), std::ios::in | std::ios::binary);
            std::remove(tmpFileName.c_str());//, dwFlagsAndAttributes |= FILE_FLAG_DELETE_ON_CLOSE;
            return res; 
        }
      }
    }
  }
  return std::fstream(pszFile, std::ios::in | std::ios::binary);
  }

KControlBuilder::e_RestResult KControlBuilder::RestoreProject( SH_Module *&pRoot )
{
  char Path[_MAX_PATH];
  GetDatFilePath( Path );

  auto hFile = GetSavedProject( Path );

  if( !hFile.is_open() )
  {
    return err_CantOpen;
  }

  DWORD Lr;
  DWORD ver = 0;
  READ_KEY( kKeyVersion, err_WrongKey )
  READ_VAR(ver);
  if( s_CurVersionPrj!=ver )
  {
    if( s_CurVersionPrj < ver )
      return err_DatVerGreaterSrcVer;
    return err_WrongVer;
  }

  RestoreModule( pRoot, hFile );

  return err_Ok;
}

KControlBuilder::e_RestResult KControlBuilder::RestoreModule( SH_Module *&pM, std::fstream& hFile )
{
  pM = (SH_Module*)RestoreBase( hFile );
  DWORD Lr = 0;
  READ_VAR( pM->inConsC );
  if( pM->inConsC )
  {
    size_t memSize = pM->inConsC * sizeof(*pM->pInConns);
    pM->pInConns = (SConnectionMB*)KMemoryServiceH5xx::Instance().NewMem( memSize );
    READ_DATA( pM->inConsC*sizeof(*pM->pInConns), pM->pInConns );
  }
  READ_VAR( pM->nSubModules );
  READ_VAR( pM->nBlocks );
  READ_STR( pM->szFile );
  
  READ_KEY( kKeyBlockData, err_WrongKey );
  if( !pM->RestoreBlock( hFile ) )
    return err_CommonErr;

  if( pM->nSubModules )
  {
    size_t memSize = pM->nSubModules * sizeof(SH_Module*);
    pM->ppSubModules = (SH_Module**)KMemoryServiceH5xx::Instance().NewMem( memSize );
    ZeroMemory( pM->ppSubModules, memSize );
    for( size_t i=0; i<pM->nSubModules; ++i )
    {
      RestoreModule( pM->ppSubModules[i], hFile );
    }
  }

  if( pM->nBlocks )
  {
    size_t memSize = pM->nBlocks * sizeof(SH_Block*);
    pM->ppBlocks = (SH_Block**)KMemoryServiceH5xx::Instance().NewMem( memSize );
    ZeroMemory( pM->ppBlocks, memSize );
    for( size_t i=0; i<pM->nBlocks; ++i )
    {
      RestoreBlock( pM->ppBlocks[i], hFile );
    }
  }

  return err_Ok;
}

KControlBuilder::e_RestResult KControlBuilder::RestoreBlock( SH_Block *&pB, std::fstream& hFile )
{
  pB = (SH_Block*)RestoreBase( hFile );
  DWORD Lr = 0;
  READ_VAR( pB->inConsC );
  if( pB->inConsC )
  {
    size_t memSize = pB->inConsC * sizeof(*pB->pInConns);
    pB->pInConns = (SConnectionMB*)KMemoryServiceH5xx::Instance().NewMem( memSize );
    READ_DATA( pB->inConsC*sizeof(*pB->pInConns), pB->pInConns );
  }
  READ_KEY( kKeyBlockData, err_WrongKey );
  if( !pB->RestoreBlock( hFile ) )
    return err_CommonErr;
  return err_Ok;
}

KBmBase* KControlBuilder::RestoreBase(std::fstream& hFile )
{
  DWORD Lr;
  DWORD key = eKeyNULL;
  READ_VAR( key );
  ASS( kKeyBlockBegin==key );
  if( kKeyBlockBegin!=key )
    return NULL;

  char BlockName[KBmBase::sizeofBlockName];
  char EntityName[KBmBase::sizeofBlockName];
  char TypeName[KBmBase::sizeofTypeName];
  char AssignedTo[KBmBase::sizeofAssignedTo];
  char Container[KBmBase::sizeofContainer];
  char ref2Block[KBmBase::sizeofBlockName] = {};

  READ_STR( BlockName );
  READ_STR( EntityName );
  READ_STR( TypeName );

  READ_STR( AssignedTo );
  READ_STR( Container );

  if( Container[0] && strcmp(AssignedTo, Container) )
  {
    char splt[5][KBmBase::sizeofBlockName] = {};
    int C = SplitString( BlockName, splt, '.' );
    ASS( C<=_countof(splt) );
    if( !strcmp(splt[0], Container) )
    {
      strcpy_s( ref2Block, AssignedTo );
      strcat_s( ref2Block, "." );
      strcat_s( ref2Block, BlockName );
    }
  }

  SH_Block *pB = NULL;
  SH_Module *pM = NULL;
  KBmBase *pBase = NULL;
  int o = Create( BlockName, EntityName, ref2Block, TypeName, &pM, &pB );
  switch( o )
  {
  case 1:
    pBase = pM;
    break;
  case 2:
    pBase = pB;
    break;
  default:
    ASS(0);
    return NULL;
  }

  strcpy_s( pBase->AssignedTo, AssignedTo );
  strcpy_s( pBase->Container, Container );
  READ_VAR( pBase->L_Class_FullW );
  if ( pBase->L_Class_FullW > 0 && pBase->pClass_FullW )
    READ_DATA( pBase->L_Class_FullW, pBase->pClass_FullW );
  return pBase;
}
