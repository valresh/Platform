#include "crossplatform.h"
#include "WinDraw.h"
#include "Err.h"
#include "WinFiles.h"
#include "Datatypes.h"

#define WRT_VAR(Var)  if ( !file.WF( &Var, sizeof( Var )) ) return -1;
#define WRT_STR(Str)  if ( !file.WS( Str               ) ) return -1;
#define WRT_DATA(Size,Addr)  if ( !file.WF( Addr, Size ) ) return -1;

#if 0
int SaveStateToFile( eSaveGroup Group, char * MainRSU, LPCTSTR File_Path )
  {
  int lFile = 0;
  CSRFile file;
  pSRFile = &file;
  //
  char drive[ _MAX_DRIVE],dir[_MAX_DIR],fname[_MAX_FNAME],ext[_MAX_EXT];
  _splitpath_s( File_Path, drive, sizeof(drive), dir  , sizeof(dir  ), fname, sizeof(fname), ext  , sizeof(ext  ) );
  char Path[_MAX_PATH];
  switch ( Group )
  {
    case Model:
      strcpy_s ( Path, sizeof(Path), File_Path );
      break;
    case Yoko:
    case RSU:
      sprintf_s ( Path, sizeof(Path), "%s%s%s_%s%s", drive, dir, MainRSU, fname, ext );
      break;
  }
  //Заменяем ':' в имени на '_' кроме двоеточия диска
  while(true)
  {
    char *p = NULL;
    p = strchr(&Path[2], ':');
    if(p == NULL)
      break;
    else
      *p = '_';

  }
  //
  // Дубль
  _HANDLE hFile = CreateFile( Path,
                            GENERIC_READ,
                            FILE_SHARE_READ,
                            NULL,
                            OPEN_EXISTING,
                            0,
                            NULL );
  if ( hFile != INVALID_HANDLE_VALUE )
  {
    FILETIME FT;
    if ( GetFileTime( hFile, NULL, NULL, &FT ) )
    {
      CloseHandle ( hFile );
      char PathOld[_MAX_PATH];
      time_t Time = ::_time64(NULL);
      struct tm Tm;
      localtime_s ( &Tm, &Time );
      char drive[ _MAX_DRIVE],dir[_MAX_DIR],fname[_MAX_FNAME],ext[_MAX_EXT];
      _splitpath_s( Path, drive, sizeof(drive), dir  , sizeof(dir  ), fname, sizeof(fname), ext  , sizeof(ext  ) );
      //
      sprintf_s ( PathOld, sizeof(PathOld), "%s%sSave", drive, dir );
      CreateDirectory	( PathOld, NULL );
      sprintf_s ( PathOld, sizeof(PathOld), "%s%sSave\\%02d-%02d_%02d-%02d_%s%s",
                drive, dir,
                Tm.tm_mday, Tm.tm_mon + 1,
                Tm.tm_hour, Tm.tm_min, fname, ext );
      MoveFile( Path, PathOld );
    }
    else
      CloseHandle ( hFile );
  }
  //
  try
  {
    if ( !ClearRO( Path ))
      return -1;
    if ( !file.Create( Path ) )
      return -1;
    //
    /*********/
    int Key = 0x9B116357;
    WRT_VAR(Key)
    WRT_VAR(WriteStateDescr)
    /*******/
    /*********
    int Key = 0x1329AED7;
    WRT_VAR(Key)
    int CurrRelease = LastRelease;
    WRT_VAR(CurrRelease)
 /*******/
    //******************
    CBase* pBase;
    UINT ID = 0;
    int nFcsNumber = -1;
    char ObjName[256];
    char Class[256];
    while ( WhileBase( ID, id_Unknown, &pBase, NULL, &nFcsNumber ) )
    {
      if ( pBase->ID_CLASS == 6163 )
        continue;
      if ( int(pBase->size) <= 0 )
        return -1;
      GetName( pBase->ID_PNT_NAME, Class, 255, ObjName, 255 );
      //200 - 202 DeltaV
      //210 - 218 RsLogix
      //230       Intouch
      //240 -     ArchestrA Wonderware
      //250 - 294 Honeywell
      //0x199(409)- OneMnemo, Centum
      //0x200 - 0x53DD Centum
      DWORD RSU_min = 200;
      DWORD RSU_max = 408;
      DWORD Y_min = 0x198;
      DWORD Y_max = 0x53DD;
      if ( Group == Model )
      {
        if( pBase->ID_CLASS >= RSU_min && pBase->ID_CLASS <= RSU_max )
          continue;
        if( pBase->ID_CLASS >= Y_min && pBase->ID_CLASS <= Y_max )
          continue;
        if( pBase->ID_CLASS >= 22000 )
          continue;
      }
      if ( Group == RSU )
      {
        continue;
      }
      if ( Group == Yoko )
      {
        continue;
      }
      // Структура
      DWORD Key = 0xA9000000 | pBase->ID_CLASS;// Признак структуры
      WRT_VAR( Key );
      WRT_VAR( nFcsNumber );
      WRT_STR( ObjName )
      if ( SaveState( pBase ) != 0 )
        return -1;
    }
    // Модели
    IBaseModel * pSR = NULL;
    //		FILE * F = fopen ( "X:\\State.txt", "wt" );
    for ( int n = 0; n < IBaseModel::kObjects; n++ )
    {
      pSR = IBaseModel::AllObjects[n];
      //
      if ( Group ==  Model )
      {
        if ( pSR->TypeObj == IBaseModel::Y_Obj || pSR->TypeObj == IBaseModel::RSU_Obj )
          continue;
      }
      else
      {
        if ( Group ==  Yoko )
        {
          if ( pSR->TypeObj != IBaseModel::Y_Obj )
            continue;
        }
        else
        {
          // RSU
          if ( strcmp ( pSR->ObjName, MainRSU ) != 0 )
            continue;
        }
      }
      if ( pSR->Model.Str == NULL || pSR->Model.Str[0] == 0 )
      {
        DWORD Key = 0x27000000;// Признак объекта
        WRT_VAR( Key )
        WRT_STR( pSR->ObjName )
      }
      else
      {
        DWORD Key = 0x2700000F;// Признак объекта + модели
        WRT_VAR( Key )
        WRT_STR( pSR->ObjName )
        WRT_STR( pSR->Model )
      }
      //      WRT_STR( pSR->Model )
      //			fprintf ( F, "[%s]\n", pSR->ObjName );
      if( pSR->SaveState( ) != 0 )
        return -1;
    }
    // 		fclose ( F );
    lFile = file.Close();
    //
  }
  catch(...)
  {
    return -1;
  };
  return lFile;
}

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

