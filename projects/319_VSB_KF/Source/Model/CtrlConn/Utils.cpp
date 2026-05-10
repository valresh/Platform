#include "stdafx.h"
#include "CtrlConn.h"
#include "CommProc.h"
#include "Connect.h"
#ifdef LINUX
#include "RsuX.h"
#else
#include <RsuX.h>
#endif
#include <MM.h>

IMM * CCtrlConn::GetRSUPnt( CErr * pErr, char * RSUPnt, char * Type, char * File )
  {
  CRSUPnt * pRSU = new CRSUPnt;
  int res = pRSU->LinkTo( RSUPnt, Type, File );
  if( res )
    {
    pErr->Head();Fprintf( pErr->F, "%s;;;Не подключиться\n", RSUPnt );
    return NULL;
    }
  return pRSU; 
  }

IMM * CCtrlConn::GetObjPnt( CErr * pErr, char * ObjName, char * Point, char * TypeDCS, bool ** pConnection )
  {
  if ( ObjName[0] == 0 )
    return NULL;
  IBaseModel * pModel = IBaseModel::Find( ObjName ); 
  if ( pModel == NULL )
    {
//???    pErr->Head();Fprintf( pErr->F, "%s;;;;Объект не найден\n", ObjName );
    return NULL;
    }
  return GetObjPnt( pErr, pModel, Point, TypeDCS, pConnection );
  }

IMM * CCtrlConn::GetObjPnt( CErr * pErr, IBaseModel * pModel, char * Point, char * TypeDCS, bool ** pConnection )
  {
  if ( pModel == NULL )
    {
    //???    pErr->Head();Fprintf( pErr->F, "%s;;;;Объект не найден\n", ObjName );
    return NULL;
    }
//  if ( lstrcmp ( Point, "Обороты" ) == 0 )
//    KKK();
  CExtern_Pnt * pPnt = pModel->GetPoint( Point );
  if ( pPnt == NULL )
    {
    pErr->Head();Fprintf( pErr->F, "%s;%s;%s;%s;Точка не найдена\n", pModel->ObjName, (char*)pModel->Model, Point, TypeDCS );
    return NULL;
    }
  if ( pPnt->IsConnection && pPnt->Type[1] == 'I' )
    {
    pErr->Head();Fprintf( pErr->F, "%s;%s;%s;%s;Точка подключается повторно\n", pModel->ObjName, (char*)pModel->Model, Point, TypeDCS );
    }
  pPnt->IsConnection = true;
  *pConnection = &pPnt->IsConnection;
  CACSPnt * pACS = new CACSPnt;
  pACS->pV = pPnt;
  return pACS;
  }

IMM * CCtrlConn::GetConnPnt( CErr * pErr, char * ObjName, char * Point )
  {
  if ( lstrcmp ( Point, "Sens" ))
    {
    pErr->Head();Fprintf( pErr->F, "%s;%s;;Ссылка возможна только по имени датчика\n", ObjName, Point );
    return NULL;
    }
  int c = -1;
  for ( int n = 0; n < Connections.L; n++ )
    {
    CConnect & Conn = Connections[n];
    if ( Conn.Sens.Str )
      KKK();
    if ( Conn.Sens == ObjName )
      {
      c = n;
      break;
      }
    }
  if ( c < 0 )
    {
    pErr->Head();Fprintf( pErr->F, "%s;;;Ссылка не найдена\n", ObjName );
    return NULL;
    }
  CConnect & Conn = Connections[c];
  CDoublePnt * pD = new CDoublePnt;
  pD->ObjName = ObjName;
  pD->pV = &Conn.PreDstValue;
  return pD;
  }

 IMM * CCtrlConn::GetHSPnt( CErr * pErr, char * HSName )
   {
   CHSPnt * pHS = new CHSPnt;
   if ( pHS == NULL || pHS->Create ( HSName ) != 0 )
     {
     pErr->Head();Fprintf( pErr->F, "%s;;;Ошибка создания объекта\n", HSName );
     return NULL;
     }
   return pHS;
   }

 IMM * CCtrlConn::GetPolyHSPnt( CErr * pErr, char * PolyHSName, int Value_1, bool Inverse )
   {
#ifdef LINUX
       ASS(0)
       return NULL;
#else
   CPolyHSPnt * pHS = new CPolyHSPnt;
   if ( pHS == NULL || pHS->Create ( PolyHSName ) != 0 )
     {
     pErr->Head();Fprintf( pErr->F, "%s;;;Ошибка создания объекта\n", PolyHSName );
     return NULL;
     }
   pHS->Value_1 = Value_1;
   pHS->Inverse = Inverse;
   return pHS;
#endif`
   }

 IMM * CCtrlConn::GetValuePnt( CErr * pErr, char * ValueName, char * ValueType )
   {
#ifdef LINUX
     ASS(0)
     return NULL;
#else
   CValuePnt * pV = new CValuePnt;
   if ( pV == NULL || pV->Create ( ValueName, ValueType[0] ) != 0 )
     {
     pErr->Head();Fprintf( pErr->F, "%s;;;Ошибка создания объекта\n", ValueName );
     return NULL;
     }
   return pV;
#endif`
   }

bool CCtrlConn::SimpleFormula( CConnect & Conn, char * Формула )
  {
  if ( *Формула == '#' )
    Формула++;
  while ( *Формула == ' ' )
    Формула++;
  if ( Формула[0] == 0 )
    return false;
//
  if ( Формула[0] == '>' )
    {
// >
    Conn.Formula.C = atof ( Формула + 1 );
    strcpy( Conn.Formula.Func, "#>" ); //Спец функция
    return true;
    }
  if ( Формула[0] == '(' &&  Формула[1] == 'X' && Формула[2] == '>' )
    {
    // (X>...)
    Conn.Formula.C = atof ( Формула + 3 );
    strcpy( Conn.Formula.Func, "#>" ); //Спец функция
    return true;
    }
  if ( Формула[0] == '<' )
    {
// <
    Conn.Formula.C = atof ( Формула + 1 );
    strcpy( Conn.Formula.Func, "#<" ); //Спец функция
    return true;
    }
  if ( Формула[0] == '(' &&  Формула[1] == 'X' && Формула[2] == '<' )
    {
    // (X>...)
    Conn.Formula.C = atof ( Формула + 3 );
    strcpy( Conn.Formula.Func, "#<" ); //Спец функция
    return true;
    }
  if ( Формула[0] == '=' && Формула[1] == '=' )
    {
// ==
    Conn.Formula.C = atof ( Формула + 2 );
    strcpy( Conn.Formula.Func, "#~" ); //Спец функция
    return true;
    }
  if ( Формула[0] == '0' )
    {
// Инверсия
    strcpy( Conn.Formula.Func, "#!" ); //Спец функция
    return true;
    }
  if ( Формула[0] == '1' )
    {
    return true;
    }
  if ( Формула[0] == '+' )
    {
// Добавлять к имеющемуся
    strcpy( Conn.Formula.Func, "#+" ); //Спец функция
    return true;
    }
  if ( Формула[0] == '=' )
    {
// Константа
    Conn.Formula.C = atof ( Формула + 1 );
    strcpy( Conn.Formula.Func, "#=" ); //Спец функция
    return true;
    }
  if ( Формула[0] == '*' )
    {
// Множитель
    Conn.Formula.C = atof ( Формула + 1 );
    char * P = strchr ( Формула + 1, '+' );
    if ( P == NULL )
      {
      strcpy( Conn.Formula.Func, "#*" ); //Спец функция
      return true;
      }
    Conn.Formula.A = atof ( P + 1 );
    strcpy( Conn.Formula.Func, "#*+" ); //Спец функция
    return true;
    }
  if ( Формула[0] == '/' )
    {
    Conn.Formula.C = atof ( Формула + 1 );
    strcpy( Conn.Formula.Func, "#/" ); //Спец функция
    return true;
    }
  if ( Формула[0] == 'L' ) //Уровнемер
    {
    Conn.Formula.A = atof ( Формула + 1 );
    char * P = strchr ( Формула + 1, ',');
    if ( P == NULL )
      Conn.Formula.C = 100.;
    else
      Conn.Formula.C = atof ( P + 1 );
    strcpy( Conn.Formula.Func, "#L" ); //Спец функция
    return true;
    }
  if ( strncmp ( Формула, "RNDT", 4 ) == 0 )  
    {
    lstrcpy( Conn.Formula.Func, "RNDT" );
    Conn.Formula.A = r01() * 0.005 * atof ( Формула + 4 );
    Conn.Formula.C = 0.;
    return true;
    }
  if ( strncmp ( Формула, "RND", 3 ) == 0 )  
    {
    lstrcpy( Conn.Formula.Func, "RND" );
    Conn.Formula.A = r01() * 0.005 * atof ( Формула + 3 );
    Conn.Formula.C = 0.;
    return true;
    }
  if ( Формула[0] == '(' )
    {
// Полная формула 
    return Translate ( Conn, Формула );
    }
  if(strncmp(Формула, "ScaleN", 6 ) == 0 )
  {
	  char *p = strchr(Формула, '(');
	  if(!p)
		  return false;
	  p++;
	  const int nParam = 4; 
	  double Param[nParam] = {};
	  Param[0] = atof(p);
	  for(int i = 1; i < nParam; i++)
	  {
		  p = strchr(p, ',');
		  if(!p)
			  return false;
		  p++;
		  Param[i] = atof(p);		
	  }
	  lstrcpy(Conn.Formula.Func, "ScaleN" );
	  if ( Param[0] < Param[1] )
	  {
		  Conn.Formula.Min = Param[0]; 
		  Conn.Formula.Max = Param[1]; 
	  }
	  else
	  {
		  Conn.Formula.Min = Param[1]; 
		  Conn.Formula.Max = Param[0]; 
	  }
	  Conn.Formula.A = (Param [3] - Param[2]) / (Param[1] - Param[0]);
	  Conn.Formula.C = (Param[2] * Param[1] - Param[3] * Param[0]) / (Param[1] - Param[0]);
	  Conn.Type[0] = 'A';
	  return true;
  }
  if(strstr(Формула, m_szScale))
  {
	  char *p = strchr(Формула, '(');
	  if(!p)
		  return false;
	  p++;
	  const int nParam = 4; 
	  double Param[nParam] = {};
	  Param[0] = atof(p);
	  for(int i = 1; i < nParam; i++)
	  {
		p = strchr(p, ',');
		if(!p)
			return false;
		p++;
		Param[i] = atof(p);		
	  }
	  lstrcpy(Conn.Formula.Func, m_szScale);
	  Conn.Formula.A = (Param [3] - Param[2]) / (Param[1] - Param[0]);
	  Conn.Formula.C = (Param[2] * Param[1] - Param[3] * Param[0]) / (Param[1] - Param[0]);
	  return true;
  }
  return false;
  }

//#ifndef LINUX
 IMM * CCtrlConn::CreateSensor( CErr * pErr, char * ObjName, char * Point )
   {
#ifdef LINUX
     ASS(0)
     return NULL;
#else
   CSensorPnt * pS = new CSensorPnt;
   if ( pS->Create ( ObjName ))
     {
     Err.Head();Fprintf( Err.F, "%s;;;Ошибка создания датчика\n", ObjName );
     return NULL;
     }
   return pS;
#endif
   }

 IMM * CCtrlConn::CreateKlapan( CErr * pErr, char * ObjName, char * Point )
   {
#ifdef LINUX
       ASS(0)
       return NULL;
#else
   CKlapanPnt * pK = new CKlapanPnt;
   if ( pK->Create ( ObjName, Point ))
     {
     Err.Head();Fprintf( Err.F, "%s;;;Ошибка создания клапана\n", ObjName );
     return NULL;
     }
   return pK;
#endif
   }

 IMM * CCtrlConn::CreateHS( CErr * pErr, char * ObjName, char * Point )
   {
   CHSPnt * pHS = new CHSPnt;
   if ( pHS->Create ( ObjName ))
     {
     Err.Head();Fprintf( Err.F, "%s;;;Ошибка создания клапана\n", ObjName );
     return NULL;
     }
   return pHS;
   }

struct CTypeID 
  {
  char Type[16];
  int ID;
  };

#define MAX_STRUCT_PARAM 128
static SValueDef * BaseListValueDef[MAX_STRUCT_PARAM];

#define MAX_ID 50
static CTypeID TypeID[MAX_ID];
static int kTypeID = -1;


IMM * CCtrlConn::GetStructPnt( CErr * pErr, char * Object, char * Type, char * Point )
  {
  ASS(0)
#if 0
  if ( kTypeID < 0 )
    {
    kTypeID = 0;
    Char<512>Path;
    Path.Prt( "%sINI\\StructID.csv", SYSTEM_PATH );
    FILE * F = fopen ( Path, "rt" );
    if ( F )
      {
      char Str[256];
      fgets ( Str, 255, F );
      while (fgets ( Str, 255, F ))
        {
        char * P = strchr ( Str, ';' );
        if ( P == NULL )
          continue;
        *P = 0;
        ASS(kTypeID <= MAX_ID)
        strcpy_s ( TypeID[kTypeID].Type, 16, Str );
        TypeID[kTypeID].ID = atoi ( P + 1 );
        kTypeID++;
        }
      fclose ( F );
      }
//    BaseListValueDef = NewArr(SValueDef*,MAX_STRUCT_PARAM);
    }
  int ID = -1;
  for ( int n = 0; n < kTypeID; n++ )
    {
    if ( strcmpi ( TypeID[n].Type, Type ) == 0 )
      {
      ID = TypeID[n].ID;
      break;
      }
    }
  if ( ID < 0 )
    {
    if ( Point != NULL && Type[0] )
      {
      Err.Head();Fprintf( Err.F, "%s;;;;Ошибочное название структуры\n", Type );
      }
    return NULL;
    }
  if ( Point == NULL )
    {
    void * pStr = NULL;
    GiveCommData( Object, ID, &pStr );
    return (IMM*)pStr;
    }
  //if ( ClassBuildList[ID] == NULL )
  //  {
  //  void * pStr = NULL;
  //  GiveCommData( Object, ID, &pStr );
  //  }
  //tBuildList * pList = ClassBuildList;
  //if ( ClassBuildList[ID] == NULL )
  //  {
  //  if ( Point != NULL )
  //    Err.Head();Fprintf( Err.F, "%s;;;;Ошибочное название структуры\n", Type );
  //  return NULL;
  //  }
// 
  kValueDef = 0;
  ListValueDef = BaseListValueDef;
  GetBuildList( ID );
  ListValueDef = NULL;
  if ( kValueDef >= MAX_STRUCT_PARAM )
    {
    Err.Head();Fprintf( Err.F, "%s;;;Структура имеет слишком много параметров\n", Type );
    return NULL;
    }
  int Numb = -1;
  int K = kValueDef;
  char * PointName = Point;
  if ( ID == 120 )
    PointName = "SP";
  for ( int n = 0; n < kValueDef; n++ )
    {
    SValueDef * pV = BaseListValueDef[n];
    if ( pV == NULL )
      continue;  
    const char * Name = pV->name;
    if ( strcmp( Name, PointName ) != 0 )
      continue;  
    Numb = n;
    break;
    }
  if ( Numb < 0 )
    {
    Err.Head();Fprintf( Err.F, "%s;%s;;Структура не имеет указанную точку\n", Type, Point );
    return NULL;
    }
  SValueDef * pV = BaseListValueDef[Numb];
  DWORD Shift = pV->dwShift;
  DWORD Flag = pV->dwFlag;
  if ( ID == 120 )
    Flag = atoi ( Point );
  char TypeVar = ' ';
  switch ( pV->eVal )
    {
    case enumValueDbl:
      TypeVar = 'A';
    break;
    case enumValueInt:
      TypeVar = 'D';
    break;
    case enumValueBol:
      TypeVar = 'B';
    break;
    }
  if ( TypeVar == ' ' )
    {
    Err.Head();Fprintf( Err.F, "%d;;;Ошибочный тип переменной структуры\n", pV->eVal );
    return NULL;
    }
  CStructPnt * pStr = new CStructPnt;
  if ( pStr->Create ( Object, ID, TypeVar ))
    {
    Err.Head();Fprintf( Err.F, "%s;;;Ошибка создания связи со структурой\n", ObjName );
    return NULL;
    }
  BYTE * Addr = (BYTE*)pStr->pV;
  Addr += Shift;
  pStr->pV = Addr;
  pStr->Flag = Flag;
  if ( Type[0] == '^' )
    pStr->Reset = true;
  else
    pStr->Reset = false;
  return pStr;
#endif
  return NULL;
  }
