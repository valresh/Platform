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


void CCtrlConn::CreateStruct_MM(  char * Name )
  {
   KKK();
#if 0
  Char<256>Path;
  Path.Prt( "%sData\\Acy\\%s.csv", PROJECT_ROOT, Name );
  FILE * F = fopen ( Path, "rt" );
  if ( F == NULL )
    {
    Fprintf ( F_err, ";;;;;Ошибка открытия '%s'\n", Name );
//??    SysMSG ( "#Ошибка чтения '%s'", Name );
    return;
    }
  Err.File = Name;
  Err.F = F_err;
  bool WasScript = false;
  //Объект1	Тип1	Точка1	IO	Описание	Объект2	Тип2	Точка2	Формула
  char Str[256];
  char Cols[10][256];
  fgets ( Str, 255, F );
  int Line = 1;
  while ( fgets ( Str, 255, F ))
    {
    Line++;
    Err.Line = Line;
    int L = lstrlen ( Str ) - 1;
    Str[L] = 0;
    if ( Str[0] == ';' )
      continue;
    if ( Str[0] == '#' )
      continue;
    Split ( Str, Cols );
    Cstring Объект1 = Cols[0];
    Cstring Тип1 = Cols[1];
    Cstring Точка1 = Cols[2];
    Cstring IO = Cols[3];	
    Cstring Описание = Cols[4];
    Cstring Объект2 = Cols[5];
    Cstring Тип2 = Cols[6];
    Cstring Точка2 = Cols[7];
    Cstring Формула = Cols[8];
    IMM * pStr = NULL;
    if ( Тип1[0] != 'R' && Тип1[0] != 'M' && Тип1[0] != 0 )
      pStr = GetStructPnt( &Err, Объект1, Тип1, NULL );
    if ( Тип2[0] != 'R' && Тип2[0] != 'M' && Тип2[0] != 0 )
      pStr = GetStructPnt( &Err, Объект2, Тип2, NULL );
    }
  fclose ( F );
    ///
#endif
  }

void CCtrlConn::ReadConnections_MM( char * Name )
  {
  Char<256>Path;
  Path.Prt( "%sDATA\\Acy\\%s.csv", PROJECT_ROOT, Name );
  FILE * F = fopen ( Path, "rt" );
  if ( F == NULL )
    {
    Fprintf ( F_err, ";;;;;Ошибка открытия '%s'\n", Name );
    SysMSG ( "#Ошибка чтения '%s'", Name );
    return;
    }
  Err.File = Name;
  Err.F = F_err;
  bool WasScript = false;
  //Объект1	Тип1	Точка1	IO	Описание	Объект2	Тип2	Точка2	Формула
  char Str[256];
  char Cols[10][256];
  fgets ( Str, 255, F );
  int Line = 1;
  while ( fgets ( Str, 255, F ))
    {
    Line++;
    Err.Line = Line;
    int L = lstrlen ( Str ) - 1;
    Str[L] = 0;
    if ( Str[0] == ';' )
      continue;
    if ( Str[0] == '#' )
      continue;
    if ( Line >= 16 )
      KKK();
    Split ( Str, Cols );
    Cstring Объект1 = Cols[0];
    Cstring Тип1 = Cols[1];
    Cstring Точка1 = Cols[2];
    Cstring IO = Cols[3];	
    Cstring Описание = Cols[4];
    Cstring Объект2 = Cols[5];
    Cstring Тип2 = Cols[6];
    Cstring Точка2 = Cols[7];
    Cstring Формула = Cols[8];
    Cstring Отказы = Cols[9];
    ///////////
    ///
   if ( IO[0] == 'N' )
      {
       ASS(0)
// // 'N' New
//       void * pNew = NULL;
//       switch ( Тип1[0] )
//         {
//         case 'S':
//           GiveCommData ( Объект1, CSensor::TypeID, &pNew );
//         break;
//         case 'K':
//           GiveCommData ( Объект1, CKlapan::TypeID, &pNew );
//         break;
//         case 'H':
//           GiveCommData ( Объект1, CHs::TypeID, &pNew );
//         break;
//         case 'P':
//           GiveCommData ( Объект1, CPolyHs::TypeID, &pNew );
//         break;
//         default:
//           Err.Head();Fprintf( Err.F, "%s;;;Недопустимый тип объекта 1;\n", Тип1 );
//         break;
//         }
//       if ( pNew == NULL )
//         {
//         Err.Head();Fprintf( Err.F, "%s;;;Ошибка создания объекта 1;\n", Объект1 );
//         }
      continue;
      }
////////////////////////////////////////////////////////////////////
    if ( Тип1[0] == 'R' || Тип2[0] == 'R' )
      {
      if ( Тип1[0] != 'R' || Тип2[0] != 'R' )
        {
        Err.Head();Fprintf( Err.F, "%s;%s;;Несоответствие типов объектов;\n", (char*)Тип1, (char*)Тип2 );
        continue;
        }
// Прямые ссылки
      IMM MM;
     MM.Description = Описание;
      IBaseModel * pM1 = IBaseModel::Find( Объект1 );
      if ( pM1 == NULL )
        {
        Err.Head();Fprintf( Err.F, "%s;;;;Объект не найден\n", Объект1 );
        continue;
        }
      IBaseModel * pM2 = IBaseModel::Find( Объект2 );
      if ( pM2 == NULL )
        {
        Err.Head();Fprintf( Err.F, "%s;;;;Объект не найден\n", Объект2 );
        continue;
        }
      if ( IO[0] == '<' )
        {
        MM.pSrc = pM2;
        MM.pDst = pM1;
        int Res = pM2->SrcMM( Точка2, &MM );
        if ( Res )
          {
          Err.Head();Fprintf( Err.F, "%s;%s;;;Ошибочная точка\n", Объект2, Точка2 );
          continue;
          }
        Res = pM1->DstMM( Точка1, &MM );
        if ( Res )
          {
          Err.Head();Fprintf( Err.F, "%s;%s;;;Ошибочная точка\n", Объект1, Точка1 );
          continue;
          }
        }
      else
        {
        MM.pSrc = pM1;
        MM.pDst = pM2;
        int Res = pM1->SrcMM( Точка1, &MM );
        if ( Res )
          {
          Err.Head();Fprintf( Err.F, "%s;%s;%d;Ошибочная точка\n", Объект1, Точка1, Res );
          continue;
          }
        Res = pM2->DstMM( Точка2, &MM );
        if ( Res )
          {
          Err.Head();Fprintf( Err.F, "%s;%s;%d;Ошибочная точка\n", Объект2, Точка2, Res );
          continue;
          }
        }
      continue;
      }
////////////////////////////////////////////////////////////////////
    if ( strcmpi ( Тип2, "Cmd" ) == 0 )
      {
// Команда
      if ( strcmpi ( Точка2, "Добавить точку" ) == 0 )
        {
        //AddNewPoint( Объект1, Тип1, Точка1, Объект2 ); 
			ModMsg("#Функция не задействована ReadMMCsv.cpp  Приблизительно 193 строка");
        continue;
        }
      }
////////////////////////////////////////////////////////////////////
    IMM * pMM1 = NULL;
    IMM * pMM2 = NULL;
    bool * pConnection1 = NULL;
    bool * pConnection2 = NULL;
    bool IsConstFormula = false;
 //
    pMM1 = NULL;//GetStructPnt( &Err, Объект1, Тип1, Точка1 );
    if ( pMM1 == NULL )
      {
      switch ( Тип1[0] )
        {
        case 'M':
 //       case 'М':
          pMM1 = GetObjPnt( &Err, Объект1, Точка1, "", &pConnection1 ); 
        break;
 //        case 'S':
 //          pMM1 = CreateSensor( &Err, Объект1, Точка1 );
 //          if ( strcmp ( Точка1, "Добавить" ) == 0 )
 //            strcpy ( Формула, "#+" );
 //        break;
 //        case 'K':
 //  //      case 'К':
 //          pMM1 = CreateKlapan ( &Err, Объект1, Точка1 );
 //        break;
 //        case 'H':
 //       case 'Н':
          pMM1 = CreateHS ( &Err, Объект1, Точка1 );
        break;
 //        case 'P':
 // //       case 'Р':
 //          {
 //          int Value_1 = atoi( Точка1 );
 //          bool Inverse = false;
 //          if ( Формула[0] == '#' && Формула[1] == '0' )
 //            Inverse = true;
 //          pMM1 = GetPolyHSPnt( &Err, Объект1, Value_1, Inverse );
 //          }
 //        break;
        case 'C':
 //       case 'С':
          pMM1 = GetConnPnt( &Err, Объект1, Точка1 ); 
          if ( IO[0] != '>' )
            {
            Err.Head();Fprintf( Err.F, "%s;;;Существующая связь может использоваться только как источник;\n", Объект1 );
            }
        break;
        default:
          if ( Формула[0] == '#' && Формула[1] == '=' )
            {
            IsConstFormula = true;
            break;  
            }
          if ( IO[0] != 0 )
            {
            Err.Head();Fprintf( Err.F, "%s;;;Неизвестный тип объекта 1;\n", Тип1 );
            }
        break;
        }
      }
    if ( pMM1 == NULL && IO[0] != 0 )
      {
      Err.Head();Fprintf( Err.F, "%s;%s;;;Не найден объект 1;\n", Объект1, Точка1 );
      }
    pMM2 = NULL;//GetStructPnt( &Err, Объект2, Тип2, Точка2 );
    if ( pMM2 == NULL )
      {
      switch ( Тип2[0] )
        {
        case 'M':
//        case 'М':
          pMM2 = GetObjPnt( &Err, Объект2, Точка2, "", &pConnection2 ); 
        break;
 //        case 'S':
 //          pMM2 = CreateSensor( &Err, Объект2, Точка2 );
 //          if ( strcmp ( Точка2, "Добавить" ) == 0 )
 //            strcpy ( Формула, "#+" );
 //        break;
 //        case 'K':
 // //       case 'К':
 //          pMM2 = CreateKlapan ( &Err, Объект2, Точка2 );
 //        break;
 //        case 'H':
  //      case 'Н':
          pMM2 = CreateHS ( &Err, Объект2, Точка2 ); 
        break;
  //       case 'P':
  // //      case 'Р':
  //         {
  //         int Value_1 = atoi( Точка2 );
  //         bool Inverse = false;
  //         if ( Формула[0] == '#' && Формула[1] == '0' )
  //           Inverse = true;
  //         pMM2 = GetPolyHSPnt( &Err, Объект2, Value_1, Inverse );
  //         }
  //       break;
        case 'C':
 //       case 'С':
          pMM2 = GetConnPnt( &Err, Объект2, Точка2 ); 
          if ( IO[0] != '<' )
            {
            Err.Head();Fprintf( Err.F, "%s;;;Существующая связь может использоваться только как источник;\n", Объект1 );
            }
        break;
        default:
          if ( Формула[0] == '#' && Формула[1] == '=' )
            {
            IsConstFormula = true;
            break;  
            }
          if ( IO[0] != 0 && Тип2[0] )
            {
            Err.Head();Fprintf( Err.F, "%s;;;Неизвестный тип объекта 2;\n", Тип2 );
            }
        break;
        }
      }
    if ( pMM2 == NULL && IO[0] != 0 && Объект2[0] && Точка2[0] )
      {
      Err.Head();Fprintf( Err.F, "%s;%s;;;Не найден объект 2;\n", Объект2, Точка2 );
      }
//
    if (( pMM1 == NULL || pMM2 == NULL ) && !IsConstFormula )
      {
      if ( pConnection1 )
        *pConnection1 = false;
      if ( pConnection2 )
        *pConnection2 = false;
      continue;
      }
    if ( IO[0] == 0 )
      {
// Создание
      continue;
      }
    if ( IO[0] != '<' && IO[0] != '>'  && IO[0] != 'X' && IO[0] != 'x' && IO[0] != '=' && IO[0] != '$' )
      {
      Err.Head();Fprintf( Err.F, "%s;;;Неправильное направление;\n", IO );
      continue;
      }
    //if ( Тип1 != Тип2 )
    //  {
    //  Err.Head();Fprintf( Err.F, ";;;Типы переменных не совпадают;\n", Тип2 );
    //  }
    if ( pConnection1 )
      *pConnection1 = true;
    if ( pConnection2 )
      *pConnection2 = true;
    CConnect & Conn = Connections.push_back();
    Conn.Init();
    Conn.Type[0] = ' ';
    Conn.Desc = Описание;
    Conn.SetDefect = false;
    if ( IO[0] == 'X' || IO[0] == 'x' )
      {
      Conn.Type[0] = IO[0];
      Conn.pSrc = pMM1;
      Conn.pDst = pMM2;
      memset ( &Conn.SrcOld, 0, sizeof ( Conn.SrcOld ));
      memset ( &Conn.DstOld, 0, sizeof ( Conn.DstOld ));
      //strcpy_s ( Conn.Obj, Объект1 );
      //strcpy_s ( Conn.Pnt, Точка1 );
      Char<256>Ref;
      Ref.Prt ( "%s.%s", Объект1, Точка1 );
      Conn.RefFrom = Ref;
      Ref.Prt ( "%s.%s", Объект2, Точка2 );
      Conn.RefTo = Ref;
      }
    else
      {
      if ( IO[0] == '<' )
        {
        Conn.pDst = pMM1;
        Conn.pSrc = pMM2;
        //strcpy_s ( Conn.Obj, Объект2 );
        //strcpy_s ( Conn.Pnt, Точка2 );
        Char<256>Ref;
        Ref.Prt ( "%s.%s", Объект1, Точка1 );
        Conn.RefTo = Ref;
        Ref.Prt ( "%s.%s", Объект2, Точка2 );
        Conn.RefFrom = Ref;
        if ( pMM1 && pMM2 &&  Отказы[0] )
          {
          Conn.Sens = Отказы;
          Conn.SetDefect = true;
          }
        if ( pMM1 && pMM1->ID == 1 )
          {
          CACSPnt * pPnt = (CACSPnt*)pMM1;
          pPnt->pV->Connection = (char*)Conn.RefFrom; 
//          pPnt->pV->IsConnection = true; 
          }
        if ( pMM2 && pMM2->ID == 1 )
          {
          CACSPnt * pPnt = (CACSPnt*)pMM2;
          pPnt->pV->Connection = (char*)Conn.RefTo; 
//          pPnt->pV->IsConnection = true; 
         }
        }
      else
        {
        Conn.pDst = pMM2;
        Conn.pSrc = pMM1;
        //strcpy_s ( Conn.Obj, Объект1 );
        //strcpy_s ( Conn.Pnt, Точка1 );
        Char<256>Ref;
        Ref.Prt ( "%s.%s", Объект1, Точка1 );
        Conn.RefFrom = Ref;
        Ref.Prt ( "%s.%s", Объект2, Точка2 );
        Conn.RefTo = Ref;
        if ( pMM1->ID == 1 )
          {
          CACSPnt * pPnt = (CACSPnt*)pMM1;
          pPnt->pV->Connection = (char*)Conn.RefTo; 
//          pPnt->pV->IsConnection = true; 
          }
        if ( pMM2->ID == 1 )
          {
          CACSPnt * pPnt = (CACSPnt*)pMM2;
          pPnt->pV->Connection = (char*)Conn.RefFrom; 
//          pPnt->pV->IsConnection = true; 
          }
        }
      }
//////////////////////
    if ( Тип1[0] == 'S' )
      {
        ASS(0)
      // Conn.Sens = Объект1;
      // IMM * pMM = Conn.pDst;
      // if ( pMM && pMM->ID == CSensorPnt::TypeID )
      //   {
      //   #define IS_CONNECTION 0x800
      //   CSensorPnt * pSens = (CSensorPnt*)pMM;
      //   if ( pSens->pV )
      //     pSens->pV ->On_Type( IS_CONNECTION );
      //   }
      // SetDefectsA ( Conn );
      }
/////////////////////
    CFormula Formula;
    if ( Формула[0] )
      {
      if ( SimpleFormula( Conn, Формула ))
        continue;
      Err.Head();Fprintf( Err.F, "%s;;;Неизвестная формула\n", Формула );
      }
    //      
    }
  Fclose ( F );
  //
//#endif
  }

void CCtrlConn::AddNewPoint( char * Объект, char * ТипТочки, char * ИмяТочки, char * Объект_Модель )
  {
  if ( strcmpi ( Объект_Модель, "Модель" ) == 0 )
    {
    bool OK = false;
    for ( int n = 0; n < IBaseModel::kObjects; n++ )
      {
      IBaseModel * pModel = IBaseModel::AllObjects[n];
      if ( pModel->Model == Объект )
        {
        OK = true;
        int Res = AddNewPoint( pModel, ТипТочки, ИмяТочки );
        if ( Res )
          break;
        }
      }
    if ( !OK )
      {
      Err.Head();Fprintf( Err.F, "%s;;;Модель не найдена\n", Объект );
      return;
      }
    return;
    }
  if ( strcmpi ( Объект_Модель, "Объект" ) == 0 )
    {
    IBaseModel * pModel = IBaseModel::Find( Объект );
    if ( pModel == NULL )
      {
      Err.Head();Fprintf( Err.F, "%s;;;;Объект не найден\n", Объект );
      return;
      }
    AddNewPoint( pModel, ТипТочки, ИмяТочки );
    return;
    }
  Err.Head();Fprintf( Err.F, "%s;;;Допустимо только 'Объект' или 'Модель'\n", Объект_Модель );
  }

int CCtrlConn::AddNewPoint( IBaseModel * pModel, char * ТипТочки, char * ИмяТочки )
  {
  if ( ТипТочки[0] != 'A' && ТипТочки[0] != 'D' )
    {
    Err.Head();Fprintf( Err.F, "%s;;;Неправильный тип точки\n", ТипТочки );
    return 1;
    }
  CExtern_Pnt * pPnt = NULL;
  if ( ТипТочки[0] == 'A' )
    {
    CAnalog * pA = new CAnalog ( pModel, ИмяТочки, ТипТочки  );
    pPnt = pA;
    }
  if ( ТипТочки[0] == 'D' )
    {
    CDiscrete * pD = new CDiscrete ( pModel, ИмяТочки, ТипТочки  );
    pPnt = pD;
    }
  pModel->AddPoint( pPnt );
  pModel->SetData ( sd_NewPoint, pPnt );
  return 0;
  }

void CCtrlConn::ConnectObjects( char * ОбъектДитя, char * Создать_Найти, char * ОбъектОтец, char * ТипПривязки )
  {
  if ( strcmpi ( Создать_Найти, "Создать" ) == 0 )
    {

    }
  if ( strcmpi ( Создать_Найти, "Найти" ) == 0 )
    {
    IBaseModel * pModel = IBaseModel::Find( ОбъектДитя );
    if ( pModel == NULL )
      {
      Err.Head();Fprintf( Err.F, "%s;;;;Объект не найден\n", ОбъектДитя );
      return;
      }
    }
  }

