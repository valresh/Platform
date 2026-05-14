#include "crossplatform.h"
#include "WinDraw.h"
#include "Err.h"
#include "WinFiles.h"
#include "CommProc.h"
#include "../zlib/zlib.h"
#include "../zlib/contrib/minizip/zip.h"
#include <QFileInfo>

int New_SaveParamsToFile( LPCTSTR FilePath );
DWORD Save_ToCharBuf( const char * Str );

int SaveParamsToFile( LPCTSTR FilePath )
{
  //
  // Дубль
  // _HANDLE hFile = CreateFile(
  //   FilePath,
  //   GENERIC_READ,
  //   0,
  //   NULL,
  //   OPEN_EXISTING,
  //   0,
  //   NULL );
  QFile file(FilePath);
  QFileInfo fileInfo(FilePath);
  QString fileName = fileInfo.fileName();
  if ( QFile::exists(FilePath) )
    {
//    CloseHandle ( hFile );
    char PathOld[_MAX_PATH];
    QDir Dir = fileInfo.absoluteDir();
    char NewPath[1024];
    Sprintf (NewPath, "%s/Save", STR(Dir.absolutePath()));
    Dir.mkdir( NewPath );
    QString Curr = QDateTime::currentDateTime().toString("dd-MM_hh-mm");

    sprintf (NewPath, "%s/Save/%s_%s",
             STR(Dir.absolutePath()), STR(Curr), STR(fileName));
    bool Res = file.copy(NewPath);
    KKK();
    }
  //
  int lFile = New_SaveParamsToFile( FilePath );
  //////////////////////////////////////////////////////
  // Сжатие
  char TmpFile[1024];
  strcpy_s( TmpFile, 1024, FilePath );
  strcat_s( TmpFile, 1024, ".tmp" );
  //
  if ( !file.copy(TmpFile))
    return -1;
  //
  char filenameinzip[_MAX_FNAME];
  strcpy_s( filenameinzip, _MAX_FNAME, STR(fileName));
  //
  zipFile zf = zipOpen( FilePath, 0);
  if( !zf )
    return -1;
  zip_fileinfo zi;
  unsigned long crcFile=0;
  const int size_buf = 8192;
  BYTE buf[size_buf];
  zi.tmz_date.tm_sec = zi.tmz_date.tm_min = zi.tmz_date.tm_hour =
    zi.tmz_date.tm_mday = zi.tmz_date.tm_mon = zi.tmz_date.tm_year = 0;
  zi.dosDate = 0;
  zi.internal_fa = 0;
  zi.external_fa = 0;
  filetime( TmpFile, &zi.tmz_date,&zi.dosDate);
  //
  int err = getFileCrc( TmpFile, buf, size_buf, &crcFile);
  //
  err = zipOpenNewFileInZip3( zf,filenameinzip,&zi,NULL,0,NULL,0,NULL,Z_DEFLATED,8,0,-MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY, NULL,crcFile);
  QFile File(TmpFile);
  _HANDLE hFile = CreateFile(
    TmpFile,
    GENERIC_READ,
    0,
    NULL,
    OPEN_EXISTING,
    0,
    NULL );
  //
  DWORD L = GetFileSize( hFile, NULL );
  DWORD P = 0;
  DWORD Lr;
  while ( P < L )
  {
    DWORD S = L - P;
    if ( S > size_buf )
      S = size_buf;
    ReadFile( hFile, buf, S, &Lr, NULL );
    err = zipWriteInFileInZip ( zf, buf, S );
    P += S;
  }
  CloseHandle( hFile );
  zipClose( zf, NULL);
  DeleteFile( TmpFile );
  return lFile;
 return 0;
}

struct CModInfo
{
    DWORD Name;
    DWORD AddrParam;
    DWORD kParams;
    void Init()
    {
      memset ( this, 0, sizeof ( *this ));
    }
};

struct CHead
{
    DWORD Key;
    DWORD kObjects;
    DWORD AddrParams;
    DWORD kParams;
    DWORD AddrTxt;
    DWORD LenTxt;
    CHead()
    {
      memset ( this, 0, sizeof ( *this ));
    }
};
struct CParamInfo
{
    DWORD Name;
    char Type;
    int Len;
    union
    {
        BYTE F;
        bool B;
        int I;
        double D;
        DWORD Str;
        float K;
    };
    void Set ( QParams & Param, DWORD Shft )
    {
      char * ParamName = Param.ParamName;
      char s1 = *ParamName;
      if ( s1 == '+' || s1 == '-' )
        ParamName++;
      char To[1024];
      UtfToChar( (unsigned char*)To, 1024, ParamName );
      Name = Save_ToCharBuf( To );
      Type = Param.Type;
      Len = Param.Len;
      if ( Param.AbsAddr )
        Shft = 0;
      BYTE * Addr = (BYTE*)Param.Addr + Shft;
      if ( Addr == NULL )
        return;
      switch( Type )
      {
        case 'L':
        case 'I':
          ASS(Len==4)
          I = *(int*)Addr;
          break;
        case 'D':
          ASS(Len==8)
          D = *(double*)Addr;
          break;
        case 'K':
          ASS(Len==4)
          K = *(float*)Addr;
          break;
        case 'B':
          ASS(Len==1)
          B = *(bool*)Addr;
          break;
        case 'S':
          {
            char * S = (char*)Addr;
            Len = lstrlen ( S );
            char To[1024];
            UtfToChar( (unsigned char*)To, 1024, S );
            Str = Save_ToCharBuf( To );
//            Str = SaveToCharBuf( S );
          }
          break;
        case 'U':
          {
            CStr * S = (CStr*)Addr;
            if ( (char*)S )
              Len = lstrlen ( (char*)S );
            else
              Len = 0;
            if ( Len > 0 )
              {
              char To[1024];
              UtfToChar( (unsigned char*)To, 1024, (char*)(S->Str));
              Str = Save_ToCharBuf( To );
              }
            else
              Str = 0;
          }
          break;
        case 'X':
          {
            ASS(0)
            // IFreeParam * pIF = (IFreeParam*)Addr;
            // Len = pIF->Len();
            // if ( Len > 0 )
            //   Str = SaveToCharBuf( Len, pIF->Data() );
            // else
            //   Str = 0;
          }
          break;
        case 'F':
          {
            ASS(Len==1)
            F = *(BYTE*)Addr;
          }
          break;
        case 'T':
        case 'E':
        case 'R':
        case 'W':
        case 'M':
        case 'm':
          {
            I = 0;
            Len = 4;
          }
          break;
        default:
          ASS(0)
          break;
      }
    }

    void Get ( IBaseModel * pModel, QParams & Param, char * pMem, DWORD Shft )
    {
      if ( Param.AbsAddr )
        Shft = 0;
      BYTE * Addr = (BYTE*)Param.Addr + Shft;
      bool Update = false;
      switch( Type )
      {
        case 'L':
        case 'I':
          ASS( Param.Len == 4 )
          if ( *(int*)Addr != I )
            Update = true;
          *(int*)Addr = I;
          break;
        case 'D':
          ASS( Param.Len == 8 )
          if ( _finite ( D ))
          {
            if ( *(double*)Addr != D )
              Update = true;
            *(double*)Addr = D;
          }
          break;
        case 'K':
          ASS( Param.Len == 4 )
          if ( *(float*)Addr != D )
            Update = true;
          *(float*)Addr = D;
          break;
        case 'B':
          ASS(Param.Len==1)
          if ( *(bool*)Addr != B )
            Update = true;
          *(bool*)Addr = B;
          break;
        case 'S':
          {
            char * S = (char*)Addr;
            ASS( Len < Param.Len );
            if ( Len >= Param.Len )
              KKK();
            if ( lstrcmp ( S, pMem + Str ) != 0 )
              Update = true;
            lstrcpy ( S, pMem + Str );
            ToUTF8(S,Param.Len);
          }
          break;
        case 'U':
          {
            CStr * S = (CStr*)Addr;
            if ( Str > 0 )
            {
              if ( *S != pMem + Str )
                Update = true;
              *S = pMem + Str;
            }
          }
          break;
        case 'X':
          {
            IFreeParam * pIF = (IFreeParam*)Addr;
            if ( Str > 0 )
            {
              pIF->SetData( Len, pMem + Str );
            }
          }
          break;
        case 'F':
          {
            ASS(Param.Len==1)
            if ( *(BYTE*)Addr != F )
              Update = true;
            *(BYTE*)Addr = F;
          }
          break;
        case 'T':
        case 'E':
        case 'R':
        case 'W':
        case 'M':
        case 'm':
          {
          }
          break;
        default:
          ASS(0)
          break;
      }
      //    if ( Update && pModel )  - не все модели готовы
      //      pModel->UpdateParam ( Param );
    }

    void Init()
    {
      memset ( this, 0, sizeof ( *this ));
    }
};

static int kSaveObj = 0;
static CModInfo * pModInfo = NULL;
static bool CallPrepare = true;
#define MAX_BUF 30000000
static BYTE * Buf = NULL;
static DWORD PosBuf;
int PrepareSaveParams()
{
  CallPrepare = false;
//??  CreateCharBuf( );
  int kObjects = IBaseModel::kObjects;
  // Исключаем Yoko
  kSaveObj = kObjects;
  //for ( int n = 0; n < kObjects; n++ )
  //  {
  //  if ( IBaseModel::AllObjects[n]->TypeObj == IBaseModel::Y_Obj )
  //    continue;
  //  kSaveObj++;
  //  }
  pModInfo = new CModInfo[kSaveObj];
  if ( Buf == NULL )
    Buf = (BYTE*)NewMem(MAX_BUF);
  PosBuf = 0;
  return 0;
}

DWORD Save_ToCharBuf( const char * Str )
{
  DWORD L = strlen ( Str ) + 1;
  ASS( PosBuf + L < MAX_BUF )
  DWORD Addr = PosBuf;
  strcpy ( (char*)(Buf + PosBuf), Str );
  PosBuf += L;
  return Addr;
}

int New_SaveParamsToFile( LPCTSTR FilePath )
  {
  TakeOffReadOnly ( FilePath );
  _HANDLE hFile = CreateFile(
    FilePath,
    GENERIC_WRITE,
    0,
    NULL,
    CREATE_ALWAYS,
    0,
    NULL );
  if ( hFile == INVALID_HANDLE_VALUE )
    return -1;
  try
  {
    //
//    CNoTransTxt No;
    if ( CallPrepare )
      PrepareSaveParams();
    else
      PosBuf = 0;
    CHead Head;
    Head.Key = 0x73261345;
    Head.kObjects = kSaveObj;
    //
    DWORD Lw;
    DWORD Pos = 0;
    DWORD PosHead = Pos;
    Pos += sizeof(Head);
    DWORD NewPos = SetFilePointer( hFile, sizeof(Head), NULL, FILE_BEGIN );
    ASS(Pos == NewPos)
    Pos += 256;
    NewPos = SetFilePointer( hFile, sizeof(Head)+256, NULL, FILE_BEGIN );
    ASS(Pos == NewPos)
    //
    DWORD Size = sizeof(CModInfo) * kSaveObj;
    Pos += Size;
    Head.AddrParams = Pos;
    NewPos = SetFilePointer( hFile, Size, NULL, FILE_CURRENT );
    ASS(Pos == NewPos)
    //
    DWORD kSavedParams = 0;
    CParamInfo ParamInfo;
    CLockParams Lock;
    QParams::SaveAllParams = true;
    //
    int nSaveObj = 0;
    for ( int n = 0; n < IBaseModel::kObjects; n++ )
    {
      IBaseModel * pModel = IBaseModel::AllObjects[n];
      //      if ( pModel->TypeObj == IBaseModel::Y_Obj )
      //        continue;
      pModInfo[nSaveObj].Init();
      char To[256];
      UtfToChar( (unsigned char*)To, 256, pModel->ObjName );
      pModInfo[nSaveObj].Name = Save_ToCharBuf( To );
      //
      DWORD Shft = 0;
      QParams * pModelParams = NULL;
      int kModelParams = 0;
      /*
      if ( pModel->GetParamsList( &pModelParams, &kModelParams ))
        {
        Shft = (DWORD)pModel;
        }
      else
*/
      {
        QParams::Reset();
        pModel->GetParams( NULL );
        pModelParams = QParams::Params;
        kModelParams = QParams::kParams;
      }
      //      QParams::Reset();
      //      pModel->GetParams( NULL );
      int kParams = 0;
      for ( int j = 0; j < kModelParams; j++ )
      {
        QParams & Param = pModelParams[j];
        if ( Param.Type == 'N' )
          continue;
        if ( Param.IsWrite())
          kParams++;
      }
      pModInfo[nSaveObj].AddrParam = Pos;
      pModInfo[nSaveObj].kParams = kParams;
#ifdef UNICUM
      Set_kParams( pModel->Model, kParams );
#endif
      int kS = 0;
      for ( int j = 0; j < kModelParams; j++ )
      {
        QParams & Param = pModelParams[j];
        if ( Param.Type == 'Z' )
        {
          ASS(!QParams::Test_Default)
          QParams::Test_Default = true;
          continue;
        }
        if ( Param.Type == 'z' )
        {
          ASS(QParams::Test_Default)
          QParams::Test_Default = false;
          continue;
        }
        if ( Param.Type == 'N' )
          continue;
        if ( !Param.IsWrite())
          continue;
        ParamInfo.Set( Param, Shft );
        WriteFile( hFile, &ParamInfo, sizeof ( CParamInfo ), &Lw, NULL );
        kS++;
        kSavedParams++;
        Pos += sizeof ( CParamInfo );
      }
      ASS(kS == kParams)
      nSaveObj++;
    }
    ASS(nSaveObj == kSaveObj)
    ////////////////
    Head.kParams = kSavedParams;
    Head.AddrTxt = Pos;
    DWORD CurrPos = SetFilePointer( hFile, 0, NULL, FILE_CURRENT );
    ASS( Pos == CurrPos )
    //
    // Сохраняем тексты
    DWORD LenTxt = PosBuf;
    char * pMem = (char*)Buf;
    Head.LenTxt = LenTxt;
    WriteFile( hFile, pMem, LenTxt, &Lw, NULL );
    Pos += LenTxt;
    //    DWORD kReopenBlk, kReopenMem;
    //    GetReopen( kReopenBlk, kReopenMem );
    //    CloseCharBuf( );
    CurrPos = SetFilePointer( hFile, 0, NULL, FILE_CURRENT );
    ASS( Pos == CurrPos )
    //////////
    NewPos = SetFilePointer( hFile, 0, NULL, FILE_BEGIN );
    WriteFile( hFile, &Head, sizeof ( Head ), &Lw, NULL );
    WriteFile( hFile, WriteParamsDescr, 256, &Lw, NULL );
    WriteFile( hFile, pModInfo, Size, &Lw, NULL );
    CurrPos = SetFilePointer( hFile, 0, NULL, FILE_CURRENT );
    ASS( Head.AddrParams == CurrPos )
    CloseHandle( hFile );
    //    delete pModInfo;
    QParams::SaveAllParams = false;
    // DWORD kReopenBlk, kReopenMem, kFailedHash, kTotal;
    //GetReopen( kReopenBlk, kReopenMem, kFailedHash, kTotal );
#ifdef UNICUM
    Prt_kParams( );
#endif
    return Pos;
  }
  catch(...)
  {
    QParams::SaveAllParams = false;
    QParams::FreeParams();
    return -1;
  };
  return 0;
}
