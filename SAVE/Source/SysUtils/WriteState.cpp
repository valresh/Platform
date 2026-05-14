#include "crossplatform.h"
#include "WinDraw.h"
#include "Err.h"
#include "WinFiles.h"
#include "DataTypes.h"

#define WRT_VAR(Var)  if ( !file.WF( &Var, sizeof( Var )) ) return -1;
#define WRT_STR(Str)  if ( !file.WS( Str               ) ) return -1;
#define WRT_DATA(Size,Addr)  if ( !file.WF( Addr, Size ) ) return -1;
enum eSaveGroup { Model, RSU, Yoko };


int SaveStateToFile( eSaveGroup Group, char * MainRSU, LPCTSTR File_Path )
{
  int lFile = 0;
  CSRFile file;
  pSRFile = &file;
  //
  char FilePath[_MAX_PATH];
  switch ( Group )
    {
    case Model:
      strcpy_s ( FilePath, sizeof(FilePath), File_Path );
      break;
    case Yoko:
    case RSU:
//      sprintf_s ( Path, sizeof(Path), "%s%s%s_%s%s", drive, dir, MainRSU, fname, ext );
      break;
    }
  //
  // Дубль
    QFile st_file(FilePath);
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
      bool Res = st_file.copy(NewPath);
      KKK();
      }  //
  try
    {
    TakeOffReadOnly( FilePath );
    if ( !file.Create( FilePath ) )
      return -1;
    //
    int Key = 0x9B116357;
    WRT_VAR(Key)
    WRT_VAR(WriteStateDescr)
    //******************
    CBase * pBase = CBase::pFirstBase;
    UINT ID = 0;
    int nFcsNumber = -1;
    char ObjName[256];
    char Class[256];
    char LastObjName[256];
    UINT LastID = 0;
    while ( pBase )
      {
      if ( int(pBase->size) <= 0 )
        return -1;
      strcpy ( LastObjName, (char*)pBase->Name );
      LastID = pBase->ID_CLASS;
      UtfToChar( (unsigned char*)ObjName, 256, (char*)pBase->Name );
      // DWORD RSU_min = 200;
      // DWORD RSU_max = 408;
      // DWORD Y_min = 0x198;
      // DWORD Y_max = 0x53DD;
      // if ( Group == Model )
      //   {
      //   if( pBase->ID_CLASS >= RSU_min && pBase->ID_CLASS <= RSU_max )
      //     continue;
      //   if( pBase->ID_CLASS >= Y_min && pBase->ID_CLASS <= Y_max )
      //     continue;
      //   if( pBase->ID_CLASS >= 22000 )
      //     continue;
      //   }
      // if ( Group == RSU )
      //   {
      //   continue;
      //   }
      // if ( Group == Yoko )
      //   {
      //   continue;
      //   }
      // Структура
      DWORD Key = 0xA9000000 | pBase->ID_CLASS;// Признак структуры
      WRT_VAR( Key );
      WRT_VAR( nFcsNumber );
      WRT_STR( ObjName )
      if ( SaveStruct( pBase ) != 0 )
        return -1;
      pBase = pBase->pNextBase;
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
        char To[256];
        UtfToChar( (unsigned char*)To, 256, pSR->ObjName );
        WRT_STR( To )
        UtfToChar( (unsigned char*)To, 256, pSR->Model );
        WRT_STR( To )
        }
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

int SaveStateToFile( LPCTSTR FilePath )
{
  int Summ = 0;
  int Res = SaveStateToFile( Model, "", FilePath );
  if ( Res < 0 )
    return Res;
  Summ += Res;
  ////////////////////////////////////////
  // int kRSU = GetRsuCount();
  // for ( int n = 0; n < kRSU; n++ )
  // {
  //   if ( RSU_Info[n].pMain == NULL )
  //     continue;
  //   if ( RSU_Info[n].MainName[0] == 'Y' )
  //   {
  //     int Res = SaveStateToFile( Yoko, RSU_Info[n].MainName, FilePath );
  //     if ( Res < 0 )
  //       return Res;
  //     Summ += Res;
  //   }
  //   else
  //   {
  //     int Res = SaveStateToFile( RSU, RSU_Info[n].MainName, FilePath );
  //     if ( Res < 0 )
  //       return Res;
  //     Summ += Res;
  //   }
  // }
  return Summ;
}
