#include "crossplatform.h"
#include "WinDraw.h"
#include "Err.h"
#include "WinFiles.h"
#include "DataTypes.h"

#if 0
int CompObjNames ( const void * p1, const void * p2 )
{
  IBaseModel * pM_1 = *(IBaseModel**)p1;
  IBaseModel * pM_2 = *(IBaseModel**)p2;
  return lstrcmp( pM_1->ObjName, pM_2->ObjName );
}


//#define ALL
int SaveStateToFile( LPCTSTR FilePath )
{
  int Summ = 0;
  int Res = SaveStateToFile( Model, "", FilePath );
  if ( Res < 0 )
    return Res;
  Summ += Res;
  ////////////////////////////////////////
  int kRSU = GetRsuCount();
  for ( int n = 0; n < kRSU; n++ )
  {
    if ( RSU_Info[n].pMain == NULL )
      continue;
    if ( RSU_Info[n].MainName[0] == 'Y' )
    {
      int Res = SaveStateToFile( Yoko, RSU_Info[n].MainName, FilePath );
      if ( Res < 0 )
        return Res;
      Summ += Res;
    }
    else
    {
      int Res = SaveStateToFile( RSU, RSU_Info[n].MainName, FilePath );
      if ( Res < 0 )
        return Res;
      Summ += Res;
    }
  }
  return Summ;
}



////////////////////////////////////////////////////////////////////////////////

int GetStateFileInfo ( LPCTSTR Path )
{
  Char<256> InfoPath;
  InfoPath.Prt( "%sMemory\\States.csv", PROJECT_ROOT );
  void * F = Fopen ( InfoPath, "wt" );
  if ( F == NULL )
    return -2;
  Fprintf ( F, "Объект;Модель;Длина состояния\n" );
  //
  char ObjName[256];
  char Model[256];
  bool TestModel = true;
  char ParmName[256];
  try
  {
    CSRFile file;
    pSRFile = &file;
    if ( !file.Open ( Path ))
      return -1;
    //
    int Key = 0;
    RD_VAR(Key)
    if ( Key != 0x2F75371B )
    {
      if ( Key == 0x9B116357 )
      {
        RD_VAR(ReadStateDescr)
      }
      else
      {
        if ( Key != 0x1329AED7 )
          return -1;
        int _Release;
        RD_VAR(_Release)
      }
    }
    //
    IBaseModel * pSR = NULL;
    int ID_CLASS;
    CBase * pStruct = NULL;
    int Count = 0;
    while ( file.IsData() )
    {
      nRestCnt++;
      DWORD Key = 0;
      RD_VAR(Key)
      if ( Key == 0x27000000 )// Признак модели
      {
        pStruct = NULL;
        pSR = NULL;
        RD_STR( ObjName )
        TestModel = false;
        pSR = IBaseModel::Find ( ObjName );
        if ( pSR == NULL )
          Fprintf ( F, "%s;Не найден;;\n", ObjName );
        continue;
      }
      if ( Key == 0x2700000F )// Признак модели
      {
        pStruct = NULL;
        pSR = NULL;
        RD_STR( ObjName )
        RD_STR( Model )
        TestModel = true;
        pSR = IBaseModel::Find ( ObjName );
        if ( pSR == NULL )
          Fprintf ( F, "%s;Не найден;;\n", ObjName );
        continue;
      }
      if ( ( Key & 0xFF000000 ) == 0xA9000000 )// Признак структуры
      {
        pSR = NULL;
        int nFcsNumber = -1;
        RD_VAR( nFcsNumber )
        RD_STR( ObjName )
        ID_CLASS = Key & 0x00FFFFFF;
        pStruct = NULL;
        pStruct = ::FindConnPoint ( ObjName, ID_CLASS, nFcsNumber );
        continue;
      }
      if ( Key == 0xE1320724 )// Признак параметра
      {
        RD_STR( ParmName )
        int LStruct = 0;
        RD_VAR( LStruct )
        file.Bytes = LStruct;
        Count++;
        if ( pSR )
        {
          Fprintf ( F, "%s;%s;%d;\n", ObjName, pSR->Model, LStruct );
          file.Skip ( LStruct );
          continue;
        }
        if ( pStruct )
        {
          Fprintf ( F, "%s;Структура;%d;\n", ObjName, LStruct );
          file.Skip ( LStruct );
          continue;
        }
        file.Skip ( LStruct );
        continue;
      }
      MsgErr ( NULL, "Ошибка восстановления состояния. Последний объект '%s'.'%s'", ObjName, ParmName );
      return -1;
    }
  }
  catch(...)
  {
    MsgErr ( NULL, "Прерывание при восстановлении состояния. Последний объект '%s'.'%s'", ObjName, ParmName );
    Fclose ( F );
    return -1;
  };
  Fclose ( F );
  return 0;
}



int RestoreObjectsStateFromFile ( LPCTSTR Path, tRestoreObjectState pRestoreObjectState )
{
  //
  char ObjName[256];
  char Model[256];
  bool TestModel = true;
  char ParmName[256];
  try
  {
    CSRFile file;
    pSRFile = &file;
    if ( !file.Open ( Path ))
      return -1;
    //
    int Key = 0;
    RD_VAR(Key)
    if ( Key != 0x2F75371B )
    {
      if ( Key == 0x9B116357 )
      {
        RD_VAR(ReadStateDescr)
      }
      else
      {
        if ( Key != 0x1329AED7 )
          return -1;
        int _Release;
        RD_VAR(_Release)
      }
    }
    //
    IBaseModel * pSR = NULL;
    int ID_CLASS;
    CBase * pStruct = NULL;
    int Count = 0;
    while ( file.IsData() )
    {
      nRestCnt++;
      DWORD Key = 0;
      RD_VAR(Key)
      if ( Key == 0x27000000 )// Признак модели
      {
        pStruct = NULL;
        pSR = NULL;
        RD_STR( ObjName )
        TestModel = false;
        pSR = IBaseModel::Find ( ObjName );
        continue;
      }
      if ( Key == 0x2700000F )// Признак модели
      {
        pStruct = NULL;
        pSR = NULL;
        RD_STR( ObjName )
        RD_STR( Model )
        TestModel = true;
        pSR = IBaseModel::Find ( ObjName );
        continue;
      }
      if ( ( Key & 0xFF000000 ) == 0xA9000000 )// Признак структуры
      {
        pSR = NULL;
        int nFcsNumber = -1;
        RD_VAR( nFcsNumber )
        RD_STR( ObjName )
        ID_CLASS = Key & 0x00FFFFFF;
        pStruct = NULL;
        pStruct = ::FindConnPoint ( ObjName, ID_CLASS, nFcsNumber );
        //        if ( pStruct == NULL )
        //          GiveCommData( ObjName, ID_CLASS, (void**)&pStruct );
        continue;
      }
      if ( Key == 0xE1320724 )// Признак параметра
      {
        RD_STR( ParmName )
        int LStruct = 0;
        RD_VAR( LStruct )
        file.Bytes = LStruct;
        Count++;
        DWORD CurrPos = file.m_dwCurr;
        if ( pSR )
        {
          if ( pRestoreObjectState && !pRestoreObjectState( ObjName ))
          {
            file.Skip ( LStruct );
            continue;
          }
          if ( pSR->RestoreState ( ParmName ) != 0 )
          {
            file.Skip ( LStruct );
          }
          continue;
        }
        if ( pStruct )
        {
          if ( pRestoreObjectState && !pRestoreObjectState( ObjName ))
          {
            file.Skip ( LStruct );
            continue;
          }
          if ( RestoreState ( pStruct, ParmName ) != 0 )
          {
            file.Skip ( LStruct );
          }
          continue;
        }
        file.Skip ( LStruct );
        continue;
      }
      MsgErr ( NULL, "Ошибка восстановления состояния. Последний объект '%s'.'%s'", ObjName, ParmName );
      return -1;
    }
  }
  catch(...)
  {
    MsgErr ( NULL, "Прерывание при восстановлении состояния. Последний объект '%s'.'%s'", ObjName, ParmName );
    return -1;
  };
  return 0;
}

#endif

#define RD_VAR(Var)  if ( !file.RF( &Var, sizeof( Var )) ) return -1;
#define RD_STR(Str)  if ( !file.RS( Str               ) ) return -1;
#define RD_DATA(Size,Addr)  if ( !file.RF( Addr, Size ) ) return -1;
int nRestCnt = 0;
int nCnt = 0;

void * Trace = NULL;
int nObj = 0;
int Q_DECL_EXPORT RestoreStateFromFile ( LPCTSTR File_Path )
{
  //CTestTime test( "C:/read.model", "STATE" );
  // char drive[ _MAX_DRIVE],dir[_MAX_DIR],fname[_MAX_FNAME],ext[_MAX_EXT];
  // _splitpath_s( File_Path,
  //              drive, sizeof(drive),
  //              dir  , sizeof(dir  ),
  //              fname, sizeof(fname),
  //              ext  , sizeof(ext  ) );
  char Path[_MAX_PATH];
  strcpy_s ( Path, sizeof(Path), File_Path );
  //
  char ObjName[256];
  char Model[256];
  bool TestModel = true;
  char ParmName[256];
  try
  {
    CSRFile file;
    pSRFile = &file;
    if ( !file.Open ( Path ))
      return -1;
    //
    nCnt = 0;
    int Key = 0;
    RD_VAR(Key)
    if ( Key != 0x2F75371B )
    {
      if ( Key == 0x9B116357 )
      {
        RD_VAR(ReadStateDescr)
        ToUTF8( ReadStateDescr, 256 );
      }
      else
      {
        if ( Key != 0x1329AED7 )
          return -1;
        int _Release;
        RD_VAR(_Release)
      }
    }
    //
    IBaseModel * pSR = NULL;
    int ID_CLASS;
    CBase * pStruct = NULL;
    int Count = 0;
    while ( file.IsData() )
    {
      nRestCnt++;
      nCnt++;
      if ( nRestCnt >= 77512 )
        KKK();
      DWORD Key = 0;
      RD_VAR(Key)
      if ( Key == 0x27000000 )// Признак модели
      {
        pStruct = NULL;
        pSR = NULL;
        RD_STR( ObjName )
        ToUTF8( ObjName, 256 );
        if ( strstr (ObjName, "Регуляторы"))
            KKK();
        TestModel = false;
        pSR = IBaseModel::Find ( ObjName );
        if ( pSR && pSR->Model.Str && strcmp ( pSR->Model, "Temp" ) == 0 )
          pSR = NULL;
        continue;
      }
      if ( Key == 0x2700000F )// Признак модели
      {
        pStruct = NULL;
        pSR = NULL;
        RD_STR( ObjName )
        ToUTF8( ObjName, 256 );
        RD_STR( Model )
        ToUTF8( Model, 256 );
        if ( strstr ( Model, "Stage" ))
          KKK();
        TestModel = true;
        pSR = IBaseModel::Find ( ObjName );
        if ( pSR )
          strcpy ( Model, pSR->Model.Str );
        else
          KKK();
        continue;
      }
      if ( ( Key & 0xFF000000 ) == 0xA9000000 )// Признак структуры
      {
        pSR = NULL;
        int nFcsNumber = -1;
        RD_VAR( nFcsNumber )
        RD_STR( ObjName )
        ToUTF8( ObjName, 256 );
        ID_CLASS = Key & 0x00FFFFFF;
        pStruct =  FindStruct( ID_CLASS, ObjName );
        if ( pStruct == NULL )
          pStruct =  CreateStruct( ID_CLASS, ObjName );
        else
          KKK();
        continue;
      }
      if ( Key == 0xE1320724 )// Признак параметра
      {
        RD_STR( ParmName )
        int LStruct = 0;
        RD_VAR( LStruct )
        ToUTF8( ParmName, 256 );
        file.Bytes = LStruct;
        Count++;
        if ( pSR )
        {
          if ( pSR && pSR->Model.Str && strcmp ( pSR->Model, "Temp" ) == 0 )
          {
            file.Skip ( LStruct );
            continue;
          }
          DWORD CurrPos = file.m_dwCurr;
          if ( pSR->RestoreState ( ParmName ) != 0 )
          {
            file.Skip ( LStruct );
          }
          continue;
        }
        if ( pStruct )
        {
          if ( RestStruct ( pStruct, ParmName ) != 0 )
          {
            file.Skip ( LStruct );
          }
          continue;
        }
        file.Skip ( LStruct );
        continue;
      }
      if(Key == 0x20EB68D0) //Со временем убрать, когда во всех проектах пересохраниться состояние
      {
        char szName[256];
        file.RS(szName);
        const int nRegSize = 48;
        BYTE Buf[nRegSize+16];
        file.RD (Buf, nRegSize);
        continue;
      }
      MsgErr ( NULL, "Ошибка восстановления состояния. Последний объект '%s'.'%s'", ObjName, ParmName );
      return -1;
    }
  }
  catch(...)
  {
    MsgErr ( NULL, "Прерывание при восстановлении состояния. Последний объект '%s'.'%s'", ObjName, ParmName );
    return -1;
  };
  return 0;
}

