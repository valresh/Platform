//#define SAVE_PARAM_IN_SR_FILE // Для теста сохранения параметров также как состояния через CSRFile
#include "crossplatform.h"
#include "WinDraw.h"
#include "Err.h"
#include "WinFiles.h"
#include "../zlib/zlib.h"
#include "../zlib/contrib/minizip/unzip.h"

enum eSaveGroup { Model, RSU, Yoko };

void CreateCharBuf( );
void CloseCharBuf( );
DWORD SaveToCharBuf( const char * pStr );
DWORD SaveToCharBuf( int Len, const void * pData );
void GetReopen( DWORD & kReopenBlk, DWORD & kReopenMem, DWORD & kFailedHash, DWORD & kTotal );
void GetCharBuf( DWORD & Len, char ** ppMem );

char ReadState[_MAX_PATH];
char ReadStateDescr[256];
char Read_Params[64];
char ReadParamsDescr[256];
char WriteState[_MAX_PATH];
char WriteStateDescr[256];
char WriteParams[64];
char WriteParamsDescr[256];


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
    void Set ( CParams & Param, DWORD Shft )
    {
      char * ParamName = Param.ParamName;
      char s1 = *ParamName;
      if ( s1 == '+' || s1 == '-' )
        ParamName++;
      Name = SaveToCharBuf( ParamName );
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
            Str = SaveToCharBuf( S );
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
              Str = SaveToCharBuf( (char*)*S );
            else
              Str = 0;
          }
          break;
        case 'X':
          {
            IFreeParam * pIF = (IFreeParam*)Addr;
            Len = pIF->Len();
            if ( Len > 0 )
              Str = SaveToCharBuf( Len, pIF->Data() );
            else
              Str = 0;
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

    void Get ( IBaseModel * pModel, CParams & Param, char * pMem, DWORD Shft )
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
#if 0
#define WRT_VAR(Var)  if ( !file.WF( &Var, sizeof( Var )) ) return -1;
#define WRT_STR(Str)  if ( !file.WS( Str               ) ) return -1;
#define WRT_DATA(Size,Addr)  if ( !file.WF( Addr, Size ) ) return -1;
#define RD_VAR(Var)  if ( !file.RF( &Var, sizeof( Var )) ) return -1;
#define RD_STR(Str)  if ( !file.RS( Str               ) ) return -1;
#define RD_DATA(Size,Addr)  if ( !file.RF( Addr, Size ) ) return -1;


void CreateCharBuf( );
void CloseCharBuf( );
DWORD SaveToCharBuf( const char * pStr );
DWORD SaveToCharBuf( int Len, const void * pData );
void GetReopen( DWORD & kReopenBlk, DWORD & kReopenMem, DWORD & kFailedHash, DWORD & kTotal );
void GetCharBuf( DWORD & Len, char ** ppMem );

    void Set ( CParams & Param, DWORD Shft )
    {
      char * ParamName = Param.ParamName;
      char s1 = *ParamName;
      if ( s1 == '+' || s1 == '-' )
        ParamName++;
      Name = SaveToCharBuf( ParamName );
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
            Str = SaveToCharBuf( S );
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
              Str = SaveToCharBuf( (char*)*S );
            else
              Str = 0;
          }
          break;
        case 'X':
          {
            IFreeParam * pIF = (IFreeParam*)Addr;
            Len = pIF->Len();
            if ( Len > 0 )
              Str = SaveToCharBuf( Len, pIF->Data() );
            else
              Str = 0;
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

    void Get ( IBaseModel * pModel, CParams & Param, char * pMem, DWORD Shft )
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

struct CPropInfo
{
    char PropName[MAX_PARM_NAME+1];
    char Type;
    int Len;
    union
    {
        BYTE F;
        bool B;
        int I;
        double D;
        char Str[256];
        float K;
    };
    void Set ( CParams & Param )
    {
      strcpy_s( PropName, sizeof( PropName), Param.ParamName );
      Type = Param.Type;
      Len = Param.Len;
      BYTE * Addr = (BYTE*)Param.Addr;
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
            ASS(Len<255)
            strcpy_s( Str, sizeof( Str ), (char*)S );
          }
          break;
        case 'U':
          {
            CStr * S = (CStr*)Addr;
            Len = lstrlen ( (char*)S );
            ASS(Len<255)
            strcpy_s( Str, sizeof( Str ), (char*)S );
          }
          break;
        case 'X':
          {
            IFreeParam * pIF = (IFreeParam*)Addr;
            Len = pIF->Len();
            ASS( Len <= 256 )
            memcpy( Str, pIF->Data(), Len );
          }
          break;
        default:
          ASS(0)
          break;
      }
    }
    void Get ( CParams & Param )
    {
      BYTE * Addr = (BYTE*)Param.Addr;
      switch( Type )
      {
        case 'L':
        case 'I':
          ASS( Param.Len == 4 )
          *(int*)Addr = I;
          break;
        case 'D':
          ASS( Param.Len == 8 )
          *(double*)Addr = D;
          break;
        case 'K':
          ASS( Param.Len == 4 )
          *(float*)Addr = D;
          break;
        case 'B':
          ASS(Param.Len==1)
          *(bool*)Addr = B;
          break;
        case 'S':
          {
            char * S = (char*)Addr;
            ASS( Len < Param.Len );
            lstrcpy ( S, Str );
          }
          break;
        case 'U':
          {
            CStr * S = (CStr*)Addr;
            *S = Str;
          }
          break;
        case 'X':
          {
            IFreeParam * pIF = (IFreeParam*)Addr;
            ASS( pIF->SetData( Len, Str ));
          }
          break;
        default:
          ASS(0)
          break;
      }
    }

    void Init()
    {
      memset ( this, 0, sizeof ( *this ));
    }
};

bool CompressParams = false;

static int kSaveObj = 0;
static CModInfo * pModInfo = NULL;
static bool CallPrepare = true;
int PrepareSaveParams()
{
  CallPrepare = false;
  CreateCharBuf( );
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
  return 0;
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
    CNoTransTxt No;
    if ( CallPrepare )
      PrepareSaveParams();
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
    CParams::SaveAllParams = true;
    //
    int nSaveObj = 0;
    for ( int n = 0; n < IBaseModel::kObjects; n++ )
    {
      IBaseModel * pModel = IBaseModel::AllObjects[n];
      //      if ( pModel->TypeObj == IBaseModel::Y_Obj )
      //        continue;
      pModInfo[nSaveObj].Init();
      pModInfo[nSaveObj].Name = SaveToCharBuf( pModel->ObjName );
      //
      DWORD Shft = 0;
      CParams * pModelParams = NULL;
      int kModelParams = 0;
      /*
      if ( pModel->GetParamsList( &pModelParams, &kModelParams ))
        {
        Shft = (DWORD)pModel;
        }
      else
*/
      {
        CParams::Reset();
        pModel->GetParams( NULL );
        pModelParams = CParams::Params;
        kModelParams = CParams::kParams;
      }
      //      CParams::Reset();
      //      pModel->GetParams( NULL );
      int kParams = 0;
      for ( int j = 0; j < kModelParams; j++ )
      {
        CParams & Param = pModelParams[j];
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
        CParams & Param = pModelParams[j];
        if ( Param.Type == 'Z' )
        {
          ASS(!CParams::Test_Default)
          CParams::Test_Default = true;
          continue;
        }
        if ( Param.Type == 'z' )
        {
          ASS(CParams::Test_Default)
          CParams::Test_Default = false;
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
    DWORD LenTxt = 0;
    char * pMem = NULL;
    GetCharBuf( LenTxt, &pMem );
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
    CParams::SaveAllParams = false;
    DWORD kReopenBlk, kReopenMem, kFailedHash, kTotal;
    GetReopen( kReopenBlk, kReopenMem, kFailedHash, kTotal );
#ifdef UNICUM
    Prt_kParams( );
#endif
    return Pos;
  }
  catch(...)
  {
    CParams::SaveAllParams = false;
    CParams::FreeParams();
    return -1;
  };
  return 0;
}
/////////////////////////////////////////////////////////////////
int SavePropsToFile( LPCTSTR FilePath )
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
    CNoTransTxt No;
    if ( CallPrepare )
      PrepareSaveParams();
    CHead Head;
    Head.Key = 0x73261345;
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
    CParams::SaveAllParams = true;
    CParams::ReadProps = true;
    //
    int nSaveObj = 0;
    for ( int n = 0; n < IBaseModel::kObjects; n++ )
    {
      IBaseModel * pModel = IBaseModel::AllObjects[n];
      if ( !pModel->HaveProperties )
        continue;
      pModInfo[nSaveObj].Init();
      pModInfo[nSaveObj].Name = SaveToCharBuf( pModel->ObjName );
      //
      DWORD Shft = 0;
      CParams * pModelParams = NULL;
      int kModelParams = 0;
      CParams::Reset();
      pModel->GetParams( NULL );
      pModelParams = CParams::Params;
      kModelParams = CParams::kParams;
      int kParams = 0;
      for ( int j = 0; j < kModelParams; j++ )
      {
        CParams & Param = pModelParams[j];
        if ( Param.Property )
          kParams++;
      }
      pModInfo[nSaveObj].AddrParam = Pos;
      pModInfo[nSaveObj].kParams = kParams;
      int kS = 0;
      for ( int j = 0; j < kModelParams; j++ )
      {
        CParams & Param = pModelParams[j];
        if ( !Param.Property )
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
    ////////////////
    Head.kObjects = nSaveObj;
    Head.kParams = kSavedParams;
    Head.AddrTxt = Pos;
    DWORD CurrPos = SetFilePointer( hFile, 0, NULL, FILE_CURRENT );
    ASS( Pos == CurrPos )
    // Сохраняем тексты
    DWORD LenTxt = 0;
    char * pMem = NULL;
    GetCharBuf( LenTxt, &pMem );
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
    CParams::SaveAllParams = false;
    CParams::ReadProps = false;
    DWORD kReopenBlk, kReopenMem, kFailedHash, kTotal;
    GetReopen( kReopenBlk, kReopenMem, kFailedHash, kTotal );
    return Pos;
  }
  catch(...)
  {
    CParams::SaveAllParams = false;
    CParams::ReadProps = false;
    CParams::FreeParams();
    return -1;
  };
  return 0;
}

//extern void * Trace;

int GetParamsFromFile ( LPCTSTR FilePath, char * ObjName )
{
  _HANDLE hFile = INVALID_HANDLE_VALUE;
  try
  {
    hFile = CreateFile(
      FilePath,
      GENERIC_READ,
      0,
      NULL,
      OPEN_EXISTING,
      0,
      NULL );
    if ( hFile == INVALID_HANDLE_VALUE )
      return __LINE__;
    DWORD Lr;
    CHead Head;
    ReadFile( hFile, &Head, sizeof ( Head ), &Lr, NULL );
    if ( Head.Key != 0x2F75371B )
    {
      CloseHandle( hFile );
      return -1;
    }
    int kObjects = Head.kObjects;
    CModInfo * pModInfo = new CModInfo[kObjects];
    DWORD Size = sizeof(CModInfo) * kObjects;
    ReadFile( hFile, pModInfo, Size, &Lr, NULL );
    char * pMem = new char[Head.LenTxt];
    DWORD NewPos = SetFilePointer( hFile, Head.AddrTxt, NULL, FILE_BEGIN );
    ReadFile( hFile, pMem, Head.LenTxt, &Lr, NULL );
    //
    NewPos = SetFilePointer( hFile, Head.AddrParams, NULL, FILE_BEGIN );
    CModInfo * pMI = NULL;
    for ( int n = 0; n < kObjects; n++ )
    {
      if ( lstrcmp ( ObjName, pMem + pModInfo[n].Name ) != 0 )
        continue;
      pMI = &pModInfo[n];
      break;
    }
    if ( pMI == NULL )
      return __LINE__;
    NewPos = SetFilePointer( hFile, pMI->AddrParam, NULL, FILE_BEGIN );
    CParamInfo ParamInfo;
    int kParam = pMI->kParams;
    for ( int j = 0; j < kParam; j++ )
    {
      ReadFile( hFile, &ParamInfo, sizeof ( CParamInfo ), &Lr, NULL );
      CParams & P = CParams::Get();
      ParamInfo.Get ( NULL, P, pMem, 0 );
    }
    CloseHandle( hFile );
    delete pMem;
    delete pModInfo;
  }
  catch(...)
  {
    if ( hFile != INVALID_HANDLE_VALUE )
      CloseHandle( hFile );
    CParams::FreeParams();
    return __LINE__;
  };
  return 0;
}

bool CompareParams( char * Param0, char * Param1 )
{
  //
  if ( *Param0 != *Param1 )
    return false;
  bool e0, e1;
  while ( 1 )
  {
    char s0 = *Param0++;
    char s1 = *Param1++;
    bool e0 = s0 == 0 || s0 == '•';
    bool e1 = s1 == 0 || s1 == '•';
    if ( e0 && e1 == 0 )
      return true;
    if ( s0 != s1 )
      return false;
  }
  return false;
}

int RestorePropsFromFile ( LPCTSTR FilePath )
{
  _HANDLE hFile = INVALID_HANDLE_VALUE;
  try
  {
    hFile = CreateFile(
      FilePath,
      GENERIC_READ,
      0,
      NULL,
      OPEN_EXISTING,
      0,
      NULL );
    if ( hFile == INVALID_HANDLE_VALUE )
      return -1;
    DWORD Lr;
    CHead Head;
    ReadFile( hFile, &Head, sizeof ( Head ), &Lr, NULL );
    if ( Head.Key != 0x2F75371B )
    {
      if ( Head.Key != 0x73261345 )
      {
        CloseHandle( hFile );
        return -1;
      }
      ReadFile( hFile, ReadParamsDescr, 256, &Lr, NULL );
    }
    else
      memset ( ReadParamsDescr, 0, 256 );
    int kObjects = Head.kObjects;
    CModInfo * pModInfo = new CModInfo[kObjects];
    DWORD Size = sizeof(CModInfo) * kObjects;
    ReadFile( hFile, pModInfo, Size, &Lr, NULL );
    char * pMem = new char[Head.LenTxt];
    DWORD NewPos = SetFilePointer( hFile, Head.AddrTxt, NULL, FILE_BEGIN );
    ReadFile( hFile, pMem, Head.LenTxt, &Lr, NULL );
    //
    CParamInfo ParamInfo;
    CLockParams Lock;
    CParams::SaveAllParams = true;
    CParams::ReadProps = true;
    void * ErrInParams = NULL;
    NewPos = SetFilePointer( hFile, Head.AddrParams, NULL, FILE_BEGIN );
    for ( int n = 0; n < kObjects; n++ )
    {
      char * ObjName = pMem + pModInfo[n].Name;
      IBaseModel * pModel = NULL;
      IBaseModel * pM = IBaseModel::pFirst;
      while ( pM )
      {
        if ( pM->HaveProperties )
        {
          if ( pM->ObjName == ObjName )
          {
            pModel = pM;
            break;
          }
        }
        pM = pM->pNext;
      }
      if ( pModel == NULL )
        continue;
      if ( !pModel->HaveProperties )
        continue;
      int kParam = pModInfo[n].kParams;
      if ( kParam <= 0 )
        continue;
      ////////////////////////
      NewPos = SetFilePointer( hFile, pModInfo[n].AddrParam, NULL, FILE_BEGIN );
      //
      DWORD Shft = 0;
      CParams * pModelParams = NULL;
      int kModelParams = 0;
      CParams::Reset();
      pModel->GetParams( NULL );
      pModelParams = CParams::Params;
      kModelParams = CParams::kParams;
      int nP = 0;
      bool WasT = false;
      for ( int j = 0; j < kParam; j++ )
      {
        ReadFile( hFile, &ParamInfo, sizeof ( CParamInfo ), &Lr, NULL );
        char * ParamName = pMem + ParamInfo.Name;
        ///
        for ( int i = 0; i < kModelParams; i++ )
        {
          CParams & P = pModelParams[i];
          if ( !P.Property )
            continue;
          char * P_ParamName = P.ParamName;
          if ( lstrcmp ( P_ParamName, ParamName ) != 0 )
            continue;
          if ( P.Type != ParamInfo.Type )
            continue;
          ParamInfo.Get ( pModel, P, pMem, Shft );
          break;
        }
      }
    }
    CloseHandle( hFile );
    if ( ErrInParams )
      Fclose( ErrInParams );
    delete pMem;
    delete pModInfo;
    CParams::ReadProps = false;
    CParams::SaveAllParams = false;
  }
  catch(...)
  {
    if ( hFile != INVALID_HANDLE_VALUE )
      CloseHandle( hFile );
    CParams::FreeParams();
    CParams::ReadProps = false;
    CParams::SaveAllParams = false;
    return -1;
  };
  return 0;
}

int GetParamsInformation ( LPCTSTR FilePath )
{
  _HANDLE hFile = INVALID_HANDLE_VALUE;
  void * F = NULL;
  try
  {
    hFile = CreateFile(
      FilePath,
      GENERIC_READ,
      0,
      NULL,
      OPEN_EXISTING,
      0,
      NULL );
    if ( hFile == INVALID_HANDLE_VALUE )
      return -1;
    //
    Char<256> InfoPath;
    InfoPath.Prt( "%sMemory\\Params.csv", PROJECT_ROOT );
    F = Fopen ( InfoPath, "wt" );
    if ( F == NULL )
    {
      CloseHandle( hFile );
      return -2;
    }
    Fprintf ( F, "Объект;Модель;Число параметров\n" );
    DWORD Lr;
    CHead Head;
    ReadFile( hFile, &Head, sizeof ( Head ), &Lr, NULL );
    if ( Head.Key != 0x2F75371B )
    {
      if ( Head.Key != 0x73261345 )
      {
        CloseHandle( hFile );
        return -3;
      }
      ReadFile( hFile, ReadParamsDescr, 256, &Lr, NULL );
    }
    else
      memset ( ReadParamsDescr, 0, 256 );
    int kObjects = Head.kObjects;
    CModInfo * pModInfo = new CModInfo[kObjects];
    DWORD Size = sizeof(CModInfo) * kObjects;
    ReadFile( hFile, pModInfo, Size, &Lr, NULL );
    char * pMem = new char[Head.LenTxt];
    DWORD NewPos = SetFilePointer( hFile, Head.AddrTxt, NULL, FILE_BEGIN );
    ReadFile( hFile, pMem, Head.LenTxt, &Lr, NULL );
    //
    CParamInfo ParamInfo;
    NewPos = SetFilePointer( hFile, Head.AddrParams, NULL, FILE_BEGIN );
    for ( int n = 0; n < kObjects; n++ )
    {
      char * ObjName = pMem + pModInfo[n].Name;
      IBaseModel * pModel = IBaseModel::Find( ObjName );
      if ( pModel == NULL )
        continue;
      //
      //      NewPos = SetFilePointer( hFile, pModInfo[n].AddrParam, NULL, FILE_BEGIN );
      int kParam = pModInfo[n].kParams;
      if ( kParam <= 0 )
        continue;
      Fprintf ( F, "%s;%s;%d\n", ObjName, pModel->Model, kParam );
    }
    CloseHandle( hFile );
    Fclose ( F );
    delete pMem;
    delete pModInfo;
  }
  catch(...)
  {
    if ( hFile != INVALID_HANDLE_VALUE )
      CloseHandle( hFile );
    if ( F )
      Fclose ( F );
    return -1;
  };
  return 0;
}

int SaveChangedParams( LPCTSTR FilePath )
{
  _HANDLE hFile = INVALID_HANDLE_VALUE;
  void * F = NULL;
  try
  {
    hFile = CreateFile(
      FilePath,
      GENERIC_WRITE,
      0,
      NULL,
      OPEN_ALWAYS,
      0,
      NULL );
    if ( hFile == INVALID_HANDLE_VALUE )
      return -1;
    DWORD Lw;
    DWORD Key = 0x6F397345;
    WriteFile( hFile, &Key, 4, &Lw, NULL );
    //
    char Buf[1024]; // 4 - сумм. длина + 4 - ключ
    Key = 0x728539AF;
    memmove ( Buf + 4, &Key, 4 );
    CParams * pParam = CParams::pFirstChanged;
    while ( pParam )
    {
      ASS(pParam->pModel)
      if ( pParam->pModel == NULL )
      {
        pParam = pParam->pNextChanged;
        continue;
      }
      int Lbuf = 8; //Длина имени + имя + 0
      int L = sprintf_s( Buf + Lbuf + 4, 1024-Lbuf-4, "%s;%s", (char*)(pParam->pModel->ObjName), pParam->ParamName );
      memmove ( Buf + Lbuf, &L, 4 );
      Lbuf += L + 5;
      memmove ( Buf + Lbuf, &pParam->Type, 1 );// Тип
      Lbuf++;
      memmove ( Buf + Lbuf, &pParam->Len, 4 ); // Длина
      Lbuf += 4;
      memmove ( Buf + Lbuf, pParam->Addr, pParam->Len ); // Параметр
      Lbuf += pParam->Len;
      memmove ( Buf, &Lbuf, 4 ); // Сумм длина в начало
      WriteFile( hFile, Buf, Lbuf, &Lw, NULL );
      pParam = pParam->pNextChanged;
    }
    Key = 0x336AEBD0;
    memmove ( Buf + 4, &Key, 4 );
    int L = 8;
    memmove ( Buf, &L, 4 );
    WriteFile( hFile, Buf, L, &Lw, NULL );
    CloseHandle( hFile );
  }
  catch(...)
  {
    if ( hFile != INVALID_HANDLE_VALUE )
      CloseHandle( hFile );
    return -1;
  };
  return 0;
}

int RestoreChangedParams( LPCTSTR FilePath )
{
  _HANDLE hFile = INVALID_HANDLE_VALUE;
  void * F = NULL;
  try
  {
    hFile = CreateFile(
      FilePath,
      GENERIC_READ,
      0,
      NULL,
      OPEN_EXISTING,
      0,
      NULL );
    if ( hFile == INVALID_HANDLE_VALUE )
      return -1;
    DWORD Lr;
    DWORD Key = 0;
    ReadFile( hFile, &Key, 4, &Lr, NULL );
    if ( Key != 0x6F397345 )
    {
      CloseHandle( hFile );
      return -2;
    }
    //
    char Buf[1024]; // 4 - сумм. длина + 4 - ключ
    int Err = 0;
    IBaseModel * pModel = NULL;
    CLockParams Lock;
    while ( 1 )
    {
      ReadFile( hFile, Buf, 4, &Lr, NULL );
      int L = *(int*)Buf;
      if ( !ReadFile( hFile, Buf+4, L-4, &Lr, NULL ))
      {
        Err = -3;
        break;
      }
      Key = *(DWORD*)(Buf+4);
      if ( Key == 0x336AEBD0 )
      {
        break;
      }
      if ( Key != 0x728539AF )
      {
        Err = -4;
        break;
      }
      L =  *(int*)( Buf + 8 );
      char * Name = Buf + 12;
      if ( Name[L] != 0 )
      {
        Err = -5;
        break;
      }
      char * Parm = strchr ( Name, ';' );
      if ( Parm == NULL )
      {
        Err = -5;
        break;
      }
      *Parm = 0;
      Parm++;
      char * Type = Name + L + 1;
      if ( 1 )//pModel == NULL || lstrcmp ( pModel->ObjName, Name ) != 0 )
      {
        pModel = IBaseModel::Find ( Name );
        if ( pModel == NULL )
          continue;
        CParams::Reset();
        pModel->GetParams( NULL );
        for ( int j = 0; j < CParams::kParams; j++ )
        {
          CParams & P = CParams::Params[j];
          if ( !P.IsWrite())
            continue;
          if ( lstrcmp ( P.ParamName, Parm ) != 0 )
            continue;
          if ( *Type != P.Type )
            continue;
          int L = *(int*)(Type+1);
          ASS( L == P.Len )
          memmove ( P.Addr, Type+5, L );
          P.WasChanged();
          break;
        }
      }
      //////////////////
    }
    CloseHandle( hFile );
  }
  catch(...)
  {
    if ( hFile != INVALID_HANDLE_VALUE )
      CloseHandle( hFile );
    return -1;
  };
  return 0;
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

int SaveParamsToFile_Ex(eSaveGroup Group, char* MainRSU, LPCTSTR szFilePath)
{
  char drive[ _MAX_DRIVE],dir[_MAX_DIR],fname[_MAX_FNAME],ext[_MAX_EXT];
  _splitpath_s( szFilePath, drive, sizeof(drive), dir  , sizeof(dir), fname, sizeof(fname), ext, sizeof(ext));
  char Path[_MAX_PATH];
  switch ( Group )
  {
    case Model:
      strcpy_s ( Path, sizeof(Path), szFilePath );
      break;
    case Yoko:
    case RSU:
      sprintf_s ( Path, sizeof(Path), "%s%s%s_%s%s", drive, dir, MainRSU, fname, ext );
      break;
  }
  //
  char ObjName[256];
  char Model[256];
  bool TestModel = true;
  CSRFile file;
  pSRFile = &file;
  //
  try
  {
    if (!file.Create(Path))
      return -1;
    int Key = 0x2F75371B;
    WRT_VAR(Key)
    WRT_STR( WriteParamsDescr );
    // Модели
    CLockParams Lock;
    CParams::SaveAllParams = true;
    for ( int n = 0; n < IBaseModel::kObjects; n++ )
    {
      IBaseModel * pModel = IBaseModel::AllObjects[n];
      if (((pModel->TypeObj == IBaseModel::RSU_Obj || pModel->TypeObj == IBaseModel::Y_Obj) && Group == ::Model) || (pModel->TypeObj != IBaseModel::RSU_Obj && Group == ::RSU) || (pModel->TypeObj != IBaseModel::Y_Obj && Group == ::Yoko))
        continue;
      CParams::Reset();
      pModel->GetParams( NULL );
      int kParams = CParams::kParams;
      if ( kParams == 0 )
        continue;
      // Подсчет числа записываемых
      int kWriteParams = 0;
      for ( int n = 0; n < kParams; n++ )
      {
        CParams * pParam = &CParams::Params[n];
        if ( ! pParam->IsWrite( ))
          continue;
        kWriteParams++;
      }
      if ( kWriteParams == 0 )
        continue;
      ///
      int KeyNewModel = 0x5A8E4310;
      WRT_VAR( KeyNewModel )
      WRT_STR( pModel->ObjName )
      WRT_VAR( kWriteParams )
      int kWrite = 0;
      for ( int n = 0; n < kParams; n++ )
      {
        CParams * pParam = &CParams::Params[n];
        if ( ! pParam->IsWrite( ))
          continue;
        kWrite++;
        WRT_STR( pParam->ParamName );
        WRT_VAR( pParam->Type );
        WRT_VAR( pParam->Len );
        WRT_DATA( pParam->Len, pParam->Addr );
      }
      ASS(kWrite == kWriteParams);
    }
    //Key = 0x8AB15F34;
    //WRT_VAR(Key)
  }
  catch(...)
  {
    return -1;
  };
  return 0;
}

int SaveParamsToFile_Ex(LPCTSTR szFilePath)
{
  int Res = SaveParamsToFile_Ex(Model, "", szFilePath);
  if(Res)
    return Res;
  int kRSU = GetRsuCount();
  int SummRes = 0;
  for ( int n = 0; n < kRSU; n++ )
  {
    if ( RSU_Info[n].pMain == NULL )
      continue;
    if ( RSU_Info[n].MainName[0] == 'Y' )
    {
      Res = SaveParamsToFile_Ex (Yoko, RSU_Info[n].MainName, szFilePath);
      if ( Res )
        SummRes = Res;
    }
    else
    {
      Res = SaveParamsToFile_Ex(RSU, RSU_Info[n].MainName, szFilePath);
      if ( Res )
        SummRes = Res;
    }
  }
  return Res;
}


void Get ( CParams * pParam, BYTE * pMem, int Len )
{
  switch( pParam->Type )
  {
    case 'L':
    case 'I':
      ASS( Len == 4 && pParam->Len == 4 )
      memmove ( pParam->Addr, pMem, 4 );
      break;
    case 'D':
      ASS( Len == 8 && pParam->Len == 8 )
      memmove ( pParam->Addr, pMem, 8 );
      break;
    case 'B':
      ASS( Len == 1 && pParam->Len == 1 )
      memmove ( pParam->Addr, pMem, 1 );
      break;
    case 'S':
      {
        ASS( Len <= pParam->Len )
        char * S = (char*)pParam->Addr;
        memmove ( S, pMem, Len );
        //S[Len] = 0;
      }
      break;
    case 'U':
      {
        CStr * S = (CStr*)pParam->Addr;
        *S = (char*)(pMem);
      }
      break;
    case 'T':
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
}

int RestoreParamsFromFile_Ex(eSaveGroup Group, char* MainRSU, LPCTSTR szFilePath)
{
  char drive[ _MAX_DRIVE],dir[_MAX_DIR],fname[_MAX_FNAME],ext[_MAX_EXT];
  _splitpath_s( szFilePath, drive, sizeof(drive), dir  , sizeof(dir), fname, sizeof(fname), ext, sizeof(ext));
  char Path[_MAX_PATH];
  switch ( Group )
  {
    case Model:
      strcpy_s ( Path, sizeof(Path), szFilePath );
      break;
    case Yoko:
    case RSU:
      sprintf_s ( Path, sizeof(Path), "%s%s%s_%s%s", drive, dir, MainRSU, fname, ext );
      break;
  }
  //
  char ObjName[256];
  char Model[256];
  bool TestModel = true;
  try
  {
    CSRFile file;
    pSRFile = &file;
    if ( !file.Open( Path ))
      return -1;
    //
    int Key = 0;
    RD_VAR(Key);
    if ( Key != 0x2F75371B )
      return -1;
    RD_STR( ReadParamsDescr );
    //
    char ObjName[256];
    int kObjParams = 0;
    bool Model = false;
    CLockParams Lock;
    CParams::SaveAllParams = true;
    while ( file.IsData() )
    {
      IBaseModel * pModel = NULL;
      kObjParams = 0;
      int KeyNewModel;
      RD_VAR( KeyNewModel )
      if ( KeyNewModel != 0x5A8E4310 )
        return -1;
      RD_STR( ObjName )
      pModel = IBaseModel::Find ( ObjName );
      if ( pModel )
      {
        CParams::Reset();
        pModel->GetParams( NULL );
        kObjParams = CParams::kParams;
      }
      //
      int kSavedParams = 0;
      RD_VAR(kSavedParams)
      char ParamName[256];
      BYTE Buf[1024];
      char Type;
      int Len;
      int kRead = 0;
      for ( int n = 0; n < kSavedParams; n++ )
      {
        RD_STR( ParamName );
        RD_VAR( Type );
        RD_VAR( Len );
        bool Skip = true;
        for ( int p = 0; p < kObjParams; p++ )
        {
          CParams * pParam = &CParams::Params[p];
          if ( !pParam->IsRead ( ))
            continue;
          if ( lstrcmp ( pParam->ParamName, ParamName ) != 0 )
            continue;
          if ( pParam->Type != Type )
            continue;
          if ( pParam->Len != Len )
            continue;
          kRead++;
          ASS(Len<1024)
          RD_DATA( Len, Buf );
          Get( pParam, Buf, Len );
          //					if ( pModel )
          //						pModel->UpdateParam ( CParams::Params[p] );
          Skip = false;
          break;
        }
        if ( Skip )
          file.Skip ( Len );
      }
      KKK();
    }
    CParams::SaveAllParams = false;
  }
  catch(...)
  {
    CParams::SaveAllParams = false;
    return -1;
  };
  CParams::SaveAllParams = false;
  return 0;
}

int RestoreParamsFromFile_Ex(LPCTSTR szFilePath)
{
  int Res = RestoreParamsFromFile_Ex(Model, "", szFilePath);
  if(Res)
    return Res;
  int kRSU = GetRsuCount();
  int SummRes = 0;
  for ( int n = 0; n < kRSU; n++ )
  {
    if ( RSU_Info[n].pMain == NULL )
      continue;
    if ( RSU_Info[n].MainName[0] == 'Y' )
    {
      Res = RestoreParamsFromFile_Ex (Yoko, RSU_Info[n].MainName, szFilePath);
      if ( Res )
        SummRes = Res;
    }
    else
    {
      Res = RestoreParamsFromFile_Ex(RSU, RSU_Info[n].MainName, szFilePath);
      if ( Res )
        SummRes = Res;
    }
  }
  return Res;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#include <macros/AutoCloser.h>
#include <contrib/minizip/unzip.h>
#include <contrib/minizip/zip.h>
#include <zlibH.h>
///////////////////////////////////////////////////////////////////////////////

int SaveParamsToFile( LPCTSTR FilePath )
{
  //
  // Дубль
  _HANDLE hFile = CreateFile(
    FilePath,
    GENERIC_READ,
    FILE_SHARE_READ,
    NULL,
    OPEN_EXISTING,
    0,
    NULL );
  if ( hFile != INVALID_HANDLE_VALUE )
  {
    FILETIME FT;
    if ( GetFileTime( hFile,
                    NULL,
                    NULL,
                    &FT ))
    {
      CloseHandle ( hFile );
      char PathOld[_MAX_PATH];
      time_t Time = ::_time64(NULL);
      struct tm Tm;
      localtime_s ( &Tm, &Time );
      char drive[ _MAX_DRIVE];
      char dir[_MAX_DIR];
      char fname[_MAX_FNAME];
      char ext[_MAX_EXT];
      _splitpath_s( FilePath,
                   drive, sizeof(drive),
                   dir  , sizeof(dir  ),
                   fname, sizeof(fname),
                   ext  , sizeof(ext  ) );
      //
      sprintf_s ( PathOld, sizeof(PathOld), "%s%sSave", drive, dir );
      CreateDirectory	( PathOld, NULL );
      sprintf_s ( PathOld, sizeof(PathOld), "%s%sSave\\%02d-%02d_%02d-%02d_%s%s",
                drive, dir,
                Tm.tm_mday, Tm.tm_mon + 1,
                Tm.tm_hour, Tm.tm_min, fname, ext );
      MoveFile( FilePath, PathOld );
    }
    else
      CloseHandle ( hFile );
  }
  //
#ifdef SAVE_PARAM_IN_SR_FILE
  int lFile = SaveParamsToFile_Ex(FilePath);
  return lFile;
#else
  int lFile = New_SaveParamsToFile( FilePath );
#endif
  //////////////////////////////////////////////////////
  // Сжатие
  char TmpFile[_MAX_PATH];
  strcpy_s( TmpFile, FilePath );
  strcat_s( TmpFile, ".tmp" );
  //
  if ( !MoveFile( FilePath, TmpFile ) )
    return -1;
  //
  char fname[_MAX_FNAME];
  char ext[_MAX_EXT];
  char filenameinzip[_MAX_FNAME] = { 0 };
  _splitpath_s( FilePath, NULL, 0, NULL, 0, fname, _countof(fname), ext, _countof(ext) );
  _makepath_s( filenameinzip, _countof(filenameinzip), NULL, NULL, fname, ext );
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
  hFile = CreateFile(
    TmpFile,
    GENERIC_READ,
    FILE_SHARE_READ,
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
}


int GetParamsFileInfo ( LPCTSTR FilePath )
{
  char TmpFile[_MAX_PATH];
  strcpy_s( TmpFile, FilePath );
  unzFile uf = unzOpen ( FilePath );
  bool DeleteTmp = false;
  if ( uf )
  {
    char filename_inzip[256];
    unz_file_info file_info;
    int err = unzGetCurrentFileInfo(uf,&file_info,filename_inzip,256,NULL,0,NULL,0);
    err = unzOpenCurrentFilePassword( uf, NULL );
    //////////////////////////////////
    const int size_buf = 8192;
    BYTE buf[size_buf];
    strcat_s( TmpFile, ".tmp" );
    _HANDLE hFile = CreateFile(
      TmpFile,
      GENERIC_WRITE,
      0,
      NULL,
      CREATE_ALWAYS,
      0,
      NULL );
    DWORD Lw;
    while ( 1 )
    {
      err = unzReadCurrentFile( uf, buf, size_buf );
      if ( err > 0 )
      {
        WriteFile( hFile, buf, err, &Lw, NULL );
      }
      else
        break;
    }
    unzClose ( uf );
    CloseHandle( hFile );
    DeleteTmp = true;
  }
  int Res = GetParamsInformation( TmpFile );
  if ( DeleteTmp )
    DeleteFile( TmpFile );
  return Res;
}

struct CModelParams
{
    char * Model;
    int kParamMin, kParamMax;
    CModelParams()
    {
      Model = NULL;
      kParamMin = MAX__PARAMS;
      kParamMax = 0;
    }
};

#ifdef UNICUM
#define MAX_MODEL 500
CModelParams ModelParams[MAX_MODEL];
int kModelParams = 0;

void Prt_kParams( );
void Set_kParams( char * Model, int kParams )
{
  int nP = -1;
  for ( int n = 0; n < kModelParams; n++ )
  {
    if ( lstrcmp ( ModelParams[n].Model, Model ) == 0 )
    {
      nP = n;
      break;
    }
  }
  if ( nP < 0 )
  {
    ASS( kModelParams < MAX_MODEL)
    nP = kModelParams++;
    ModelParams[nP].Model = Model;
  }
  if ( ModelParams[nP].kParamMin > kParams )
    ModelParams[nP].kParamMin = kParams;
  if ( ModelParams[nP].kParamMax < kParams )
    ModelParams[nP].kParamMax = kParams;
}

void Prt_kParams ( )
{
  void * F = Fopen ( "Params.csv", "wt" );
  for ( int n = 0; n < kModelParams; n++ )
  {
    Fprintf ( F, "%s;%d;%d;", ModelParams[n].Model, ModelParams[n].kParamMin, ModelParams[n].kParamMax );
    if ( ModelParams[n].kParamMin == ModelParams[n].kParamMax )
      Fprintf ( F, ";\n" );
    else
      Fprintf ( F, "##############;\n" );
  }
  Fclose ( F );
}
#endif
#endif

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
enum eTypeSaveRest { Модели_и_РСУ, Модели, РСУ };
typedef bool (*tRestoreObjectParams)( LPCTSTR ObjName );

int RestoreParamsFromFile_1 ( LPCTSTR FilePath )
{
  //$$  ASS(Create_1_2_Data( "2_THREAD.csv" ));
  _HANDLE hFile = INVALID_HANDLE_VALUE;
  try
    {
    hFile = CreateFile(
      FilePath,
      GENERIC_READ,
      0,
      NULL,
      OPEN_EXISTING,
      0,
      0 );
    if ( hFile == INVALID_HANDLE_VALUE )
    {
      return -1;
    }
    DWORD Lr;
    CHead Head;
    ReadFile( hFile, &Head, sizeof ( Head ), &Lr, NULL );
    if ( Head.Key != 0x2F75371B )
      {
      if ( Head.Key != 0x73261345 )
        {
        CloseHandle( hFile );
        return -1;
        }
      ReadFile( hFile, ReadParamsDescr, 256, &Lr, NULL );
      ToUTF8( ReadParamsDescr, 256 );
      KKK();
      }
    else
      memset ( ReadParamsDescr, 0, 256 );
    int kRSU = -1;
//    if ( TypeSaveRest == РСУ )
//      kRSU = GetRsuCount();
    int kObjects = Head.kObjects;
    CModInfo * pModInfo = new CModInfo[kObjects];
    DWORD Size = sizeof(CModInfo) * kObjects;
    ReadFile( hFile, pModInfo, Size, &Lr, NULL );
    char * pMem = new char[Head.LenTxt];
    DWORD NewPos = SetFilePointer( hFile, Head.AddrTxt, NULL, FILE_BEGIN );
    ReadFile( hFile, pMem, Head.LenTxt, &Lr, NULL );
    //
    CParamInfo ParamInfo;
    CLockParams Lock;
    CParams::SaveAllParams = true;
    void * ErrInParams = NULL;
    NewPos = SetFilePointer( hFile, Head.AddrParams, NULL, FILE_BEGIN );
    for ( int n = 0; n < kObjects; n++ )
    {
      char * ObjName = pMem + pModInfo[n].Name;
      char ObjNameUTF[256];
      CharToUTF8( ObjNameUTF, 256, ObjName );
      IBaseModel * pModel = IBaseModel::Find( ObjNameUTF );
      if ( pModel == NULL )
        continue;
      if ( pModel->Model.Str && strcmp ( pModel->Model.Str, "Temp" ) == 0 )
        continue;
      if ( pModel->Model.Str && strcmp ( pModel->Model.Str, "Химия" ) == 0 )
        KKK();
      int kParam = pModInfo[n].kParams;
      if ( kParam <= 0 )
        continue;
      if ( pModel == NULL )
        break;
        ////////////////////////
      NewPos = SetFilePointer( hFile, pModInfo[n].AddrParam, NULL, FILE_BEGIN );
        //
      DWORD Shft = 0;
      CParams * pModelParams = NULL;
      int kModelParams = 0;
      CParams::Reset();
      pModel->GetParams( NULL );
      pModelParams = CParams::Params;
      kModelParams = CParams::kParams;
      int nP = 0;
       bool WasT = false;
      for ( int j = 0; j < kParam; j++ )
        {
         if ( j == 5 )
          KKK();
        ReadFile( hFile, &ParamInfo, sizeof ( CParamInfo ), &Lr, NULL );
        char * ParamName_Win = pMem + ParamInfo.Name;
        char wParamName[256];
        CharToUTF8( wParamName, 256, ParamName_Win );
        Cut( wParamName, MAX_PARM_NAME );
        char * ParamName = wParamName;
        if ( ParamName[0] == '@' )
          continue;
        bool OK = false;
        while ( nP < kModelParams )
          {
          CParams & P = pModelParams[nP];
          if ( !P.IsWrite())
            {
              nP++;
              continue;
            }
          char * P_ParamName = P.ParamName;
//          char pParamName[256];
//          strcpy ( pParamName, P_ParamName );
          if ( strcmp ( P_ParamName, ParamName ) == 0 && P.Type == ParamInfo.Type )
            {
            ParamInfo.Get( pModel, P, pMem, Shft );
            OK = true;
            nP++;
            break;
            }
            nP++;
          }
        if ( OK )
          continue;
        for ( int i = 0; i < kModelParams; i++ )
          {
          CParams & P = pModelParams[i];
          char * P_ParamName = P.ParamName;
          char s1 = *P_ParamName;
          if ( s1 == '+' || s1 == '-' )
            P_ParamName++;
          if ( lstrcmp ( P_ParamName, ParamName ) != 0 )
            continue;
          if ( P.Type != ParamInfo.Type )
            continue;
          ParamInfo.Get ( pModel, P, pMem, Shft );
          OK = true;
          nP = i+1;
          break;
          }
        if ( !OK )
          {
            KKK();
          // if ( ErrInParams == NULL )
          //   ErrInParams = Fopen ( "Ошибки чтения параметров.csv", "wt" );
          // if ( ErrInParams )
          //   Fprintf ( ErrInParams, "%s;%s;Параметр не найден;\n", ObjName, ParamName );
          }
        }
      }
    CloseHandle( hFile );
    // if ( ErrInParams )
    //   Fclose( ErrInParams );
    delete pMem;
    delete pModInfo;
    CParams::SaveAllParams = false;
  }
  catch(...)
    {
    if ( hFile != INVALID_HANDLE_VALUE )
      CloseHandle( hFile );
    CParams::FreeParams();
    CParams::SaveAllParams = false;
    return -1;
    };
  return 0;
  }//$$

//$$


int RestoreParamsFromFile_0 ( LPCTSTR FilePath )
  {
  char TmpFile[_MAX_PATH];
  strcpy( TmpFile, "/home/resh/TEMP/Params.dat" );//FilePath );
  DeleteFile( TmpFile );
  unzFile uf = unzOpen ( FilePath );
  bool DeleteTmp = false;
  if ( uf )
  {
    char filename_inzip[256];
    unz_file_info file_info;
    int err = unzGetCurrentFileInfo(uf,&file_info,filename_inzip,256,NULL,0,NULL,0);
    err = unzOpenCurrentFilePassword( uf, NULL );
    //////////////////////////////////
    const int size_buf = 8192;
    BYTE buf[size_buf];
//    strcat_s( TmpFile, ".tmp" );
    _HANDLE hFile = CreateFile(
      TmpFile,
      GENERIC_WRITE,
      0,
      NULL,
      CREATE_ALWAYS,
      0,
      NULL );
    if ( hFile == NULL )
      {
      unzClose ( uf );
      return -1;
      }
    DWORD Lw;
    while ( 1 )
      {
      err = unzReadCurrentFile( uf, buf, size_buf );
      if ( err > 0 )
        {
        WriteFile( hFile, buf, err, &Lw, NULL );
        }
      else
        break;
      }
    unzClose ( uf );
    CloseHandle( hFile );
    DeleteTmp = true;
    }
  int Res = RestoreParamsFromFile_1(TmpFile);
  //  Fclose( Trace );
  if ( DeleteTmp )
    DeleteFile( TmpFile );
  return Res;
}//$$

int Q_DECL_EXPORT RestoreParamsFromFile(LPCTSTR szFilePath)
  {
    int Res = RestoreParamsFromFile_0( szFilePath );
    if(Res)
      return Res;
    // int kRSU = GetRsuCount();
    // int SummRes = 0;
    // for ( int n = 0; n < kRSU; n++ )
    // {
    //   if ( RSU_Info[n].pMain == NULL )
    //     continue;
    //   if ( RSU_Info[n].MainName[0] == 'Y' )
    //   {
    //     Res = RestoreParamsFromFile_Ex (Yoko, RSU_Info[n].MainName, szFilePath);
    //     if ( Res )
    //       SummRes = Res;
    //   }
    //   else
    //   {
    //     Res = RestoreParamsFromFile_Ex(RSU, RSU_Info[n].MainName, szFilePath);
    //     if ( Res )
    //       SummRes = Res;
    //   }
    // }
    // return Res;
  }

