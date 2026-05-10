#include "stdafx.h"
#include "CtrlConn.h"
#include "CommProc.h"
#include "SetData.h"
#ifdef LINUX
#include "RsuX.h"
#else
#include <RsuX.h>
#endif
#include <MM.h>

void CCtrlConn::CreateStruct_Rsu( )
  {
  // Char<256>Path;
  // Path.Prt( "%sData\\Acy\\*.csv", PROJECT_ROOT );
  // int NumbFile = 1;
  // WIN32_FIND_DATA FD;
  // HANDLE hFind = FindFirstFile( Path, &FD );
  // if( hFind == INVALID_HANDLE_VALUE )
  //   return;
  // BOOL Res = TRUE;
  // int kErr = 0;
  // Char<128>File;
  // while ( Res )
  //   {
  //   if ( FD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
  //     {
  //     Res = FindNextFile( hFind, &FD );
  //     continue;
  //     }
  //   File = FD.cFileName;
  //   char * P = strchr ( File, '.' );
  //   if ( P )
  //     *P = 0;
  //   if ( strncmp ( File, "Связи_", 6 ) == 0 )
  //     {
  //     NumbFile++;
  //     CreateStruct_MM( File );
  //     //      ReadConnections_MM( NumbFile, File );
  //     Res = FindNextFile( hFind, &FD );
  //     continue;
  //     }
  //   if ( strncmp ( File, "РСУ_", 4 ) == 0 )
  //     {
  //     NumbFile++;
  //     ////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  //     CreateStruct_Rsu( File );
  //     Res = FindNextFile( hFind, &FD );
  //     continue;
  //     }
  //   Res = FindNextFile( hFind, &FD );
  //   }
  }

void CCtrlConn::CreateStruct_Rsu( char * File )
  {
#if 0
  Char<256>Path;
  Path.Prt( "%sData\\Acy\\%s.csv", PROJECT_ROOT, File );
  FILE * F = NULL;
  F = fopen ( Path, "rt" );
  if ( F == NULL )
    {
    return;
    }
  char StrXLS[16][256] = { 0 };
  char Str[256] = { 0 };
  bool PAZ = strstr ( File, "PAZ" ) != NULL;
  // РСУ
  // 0, A - Точка РСУ	
  // 1, B - Alt
  // 2, C - ТипDV	
  // 3, D - Описание	
  // 4, E - Объект	
  // 5, F - Тип	
  // 6, G - Точка	

  // 7, H - Формула	
  // 8, I - Размерность
  // 9, J - min
  //10, K - max;
  //11 -L - датчик
  //12 - Позиция  модуля;
  //13 - Контроллер;
  //14 - Станция управления;
  //15 - Номер канала
  fgets ( Str, 255, F );
  while( 1 )
    {
    if( !fgets( Str, _countof(Str)-1, F ) )
      break;
    int L = strlen( Str ) - 1;
    Str[L] = 0;
    if ( L < 3 )
      continue;
    if ( Str[0] == '#' || Str[0] == ';' )
      continue;
    Split( Str, StrXLS );
    char * TypeObj = StrXLS[5];
    if ( TypeObj[0] == 'M' )
      continue;
    // bool HS =
    //   strcmp ( TypeObj, "H" ) == 0 ||
    //   strcmp ( TypeObj, "HS->RSU" ) == 0 ||
    //   strcmp ( TypeObj, "RSU->HS" ) == 0;
    // bool PHS =
    //   strcmp ( TypeObj, "PHS->RSU" ) == 0 ||
    //   strcmp ( TypeObj, "RSU->PHS" ) == 0;
    // bool V =
    //   strcmp ( TypeObj, "V->RSU" ) == 0 ||
    //   strcmp ( TypeObj, "RSU->V" ) == 0;
    // if ( !HS && !PHS && !V )
    //   continue;
    // char * Name = StrXLS[4];
    // if ( HS )
    //   {
    //   CHs * pHS = NULL;
    //   GiveCommData ( Name, CHs::TypeID, (void**)&pHS );
    //   continue;
    //   }
    // if ( PHS )
    //   {
    //   CPolyHs * pPHS = NULL;
    //   GiveCommData ( Name, CPolyHs::TypeID, (void**)&pPHS );
    //   continue;
    //   }
    // if ( V )
    //   {
    //   CValue * pV = NULL;
    //   GiveCommData ( Name, CValue::TypeID, (void**)&pV );
    //   continue;
    //   }
    ASS(0)
    }
#endif
  }
