#include "stdafx.h"
#include "CtrlConn.h"
#include "Err.h"
#include "SysDataTypes.h"


#undef IF_DEFECT
#define IF_DEFECT(Def,Comment) \
  if ( pDefect == NULL || pDefect == Def )\
  {\
  Defect = e##Def;

#define END_IF \
  if ( pDefect )\
  return 1;\
  }

#undef SET_DEF
#define SET_DEF(DefectVar,Comment) \
  {\
  LastDefect = NewDefect( );\
  Conn.pDef->DefectVar = GetDefect( LastDefect );\
  Conn.pDef->DefectVar->Parent = RootNode;\
  strcpy_s ( Conn.pDef->DefectVar->Name, Comment );\
  strcpy_s ( Conn.pDef->DefectVar->ObjName, SensName );\
  Conn.pDef->DefectVar->Flags = CDef::Defects|CDef::Defect|CDef::Active;\
  Conn.pDef->DefectVar->pModel = this;\
  }

#undef SET_PARM
#define SET_PARM(Var, vName, vDef, vMin, vMax ) \
  {\
  Conn.pDef->Var = NewDefectPram( LastDefect );\
  Conn.pDef->Var->Type = CDefParam::tDbl;\
  strcpy_s(Conn.pDef->Var->Name, vName );\
  Conn.pDef->Var->Min = (float)vMin;\
  Conn.pDef->Var->Max = (float)vMax;\
  Conn.pDef->Var->Dbl = Conn.pDef->Var->_Dbl = (double)(vDef);\
  }

extern DWORD ClassRootNodeA;
extern DWORD ClassRootNodeD;

CMem<CConnect,256,256> * pConnections;
int * OdrConn;
int kSensConn;

int CompConn ( const void * p1, const void * p2 )
  {
  int n1 = *(int*)p1;
  int n2 = *(int*)p2;
  CConnect & C1 = (*pConnections)[n1];
  CConnect & C2 = (*pConnections)[n2];
  return lstrcmp ( C1.Sens, C2.Sens );
  }

int CompConnName ( const void * p1, const void * p2 )
  {
  char * Name = (char*)p1;
  int n2 = *(int*)p2;
  CConnect & C2 = (*pConnections)[n2];
  return lstrcmp ( Name, C2.Sens );
  }

void CCtrlConn::SetConnDefects( )
  {
  int L = Connections.L;
  OdrConn = NewArr(int,L);
  kSensConn = 0;
  pConnections = &Connections;
  for ( int n = 0; n < Connections.L; n++ )
    {
    CConnect & S = Connections[n];
    if ( S.Sens.Str == NULL || *(char*)S.Sens == 0 )
      continue;
    if ( S.Type[0] == 'A' || S.SetDefect ) 
      {
      SetDefectsA( S );
      }
    else
      {
      if ( !DiscreteDefects )
        continue;
      SetDefectsD( S );
      }
    OdrConn[kSensConn++] = n;
    }
  qsort (OdrConn, kSensConn, sizeof(int), CompConn );
  }

BOOL FindConnInfo( const char* szName, double * pValue )
  {
  int * pN = (int*)bsearch ( szName, OdrConn, kSensConn, sizeof(int), CompConnName );
  if ( pN == NULL )
    return FALSE;
  CConnect & C = (*pConnections)[*pN];
  *pValue = C.PreDstValue;
  return TRUE;
  }

// Последний взгляд
void CCtrlConn::SetDefectsA( CConnect & Conn )
  {
  if ( Conn.Sens.Str == NULL )
    return;
  char * SensName = Conn.Sens;
  if ( SensName[0] == 0 )
    return;
  if ( ClassRootNodeA == 0 )
    {
    ClassRootNodeA = NewDefect( );
    CDef * pRoot = GetDefect( ClassRootNodeA );
    strcpy_s ( pRoot->Name, "Датчики" );
    pRoot->Flags = CDef::Defects|CDef::Node|CDef::Active;
    pRoot->Parent = GetRootDefects( );
    }
  //
//  Fsend ("Defects.txt","%s\n",SensName);
  DWORD LastDefect = 0;
  DWORD RootNode = NewDefect( );
  CDef * pNode = GetDefect( RootNode );
  pNode->Parent = ClassRootNodeA;
  //////////////////
  char * P = strchr ( SensName, '|' );
  if ( P )
    *P = 0;
  strcpy_s ( pNode->Name, SensName );//pDef->Connection );
  char Ref[256];
  strcpy_s ( Ref, Conn.RefFrom );
  P = strchr ( Ref, '.' );
  if ( P )
    *P = 0;
  strcpy_s ( pNode->ObjName, Ref );
//??  strcpy_s ( pNode->ObjName, ObjName );
  pNode->Flags = CDef::Defects|CDef::Node|CDef::Active;
  //
  Conn.pDef = new CConnectDefect();
  //
  SET_DEF(Исправен, "Исправен" );
  SET_DEF(Показания_не_изменяются, "Показания не изменяются" );
  SET_DEF(Завышение_показаний, "Завышение показаний" );
  SET_PARM( parm_Завышение_показаний, "Завышение", 1.2, 1., 10. )
    SET_DEF(Занижение_показаний, "Занижение показаний" );
  SET_PARM( parm_Занижение_показаний, "Занижение", 1.2, 1., 10. )
  // if ( Version != MK2_YAR )
  //   {
  //   SET_DEF(Уход_показаний_на_максимум, "Уход показаний на максимум" );
  //   //		  SET_PARM( time_Уход_показаний_на_максимум, "Время изменения с", 0., 0., 3600. )
  //   SET_DEF(Уход_показаний_на_начало_шкалы, "Уход показаний на начало шкалы" );
  //   //		  SET_PARM( time_Уход_показаний_на_начало_шкалы, "Время изменения с", 0., 0., 3600. )
  //   }
  SET_DEF(Самопроизвольное_изменение_показаний, "Самопроизвольное изменение показаний" );
    SET_PARM( parm_Самопроизвольное_изменение_показаний, "Конечное значение", 0., -100., 10000. )
    SET_PARM( time_Самопроизвольное_изменение_показаний, "Время изменения с", 0., 0., 3600. )
  SET_DEF(Хаотическое_изменение_показаний, "Хаотическое изменение показаний" );
    SET_PARM( min_Хаотическое_изменение_показаний, "Минимальное значение", 0., -100., 10000. )
    SET_PARM( max_Хаотическое_изменение_показаний, "Максимальное значение", 0., -100., 10000. )
  }

void CCtrlConn::SetDefectsD( CConnect & Conn )
  {
  if ( Conn.Sens.Str == NULL )
    return;
  char * SensName = Conn.Sens;
  if ( SensName[0] == 0 )
    return;
  if ( ClassRootNodeD == 0 )
    {
    ClassRootNodeD = NewDefect( );
    CDef * pRoot = GetDefect( ClassRootNodeD );
    strcpy_s ( pRoot->Name, "Дискреты" );
    pRoot->Flags = CDef::Defects|CDef::Node|CDef::Active;
    pRoot->Parent = GetRootDefects( );
    }
  //
  DWORD LastDefect = 0;
  DWORD RootNode = NewDefect( );
  CDef * pNode = GetDefect( RootNode );
  pNode->Parent = ClassRootNodeD;
  //////////////////
  char * P = strchr ( SensName, '|' );
  if ( P )
    *P = 0;
  strcpy_s ( pNode->Name, SensName );//pDef->Connection );
  char Ref[256];
  strcpy_s ( Ref, Conn.RefFrom );
  P = strchr ( Ref, '.' );
  if ( P )
    *P = 0;
  strcpy_s ( pNode->ObjName, Ref );
  pNode->Flags = CDef::Defects|CDef::Node|CDef::Active;
  //
  Conn.pDef = new CConnectDefect();
  SET_DEF(Восстановлен, "Восстановлен" );
  SET_DEF(Постоянно_0, "Постоянно 0" );
  SET_DEF(Постоянно_1, "Постоянно 1" );
  }

// Отказы для дискретов
int CCtrlConn::SetDefectD(  CConnect & Conn, struct CDef * pDefect )
  {
  if ( Conn.Type[0] != 'D' || Conn.Type[1] != 'I' )
    return 1;
  if ( Conn.pSrc == NULL || ( ((CACSPnt*)(Conn.pSrc))->pV && ((CACSPnt*)(Conn.pSrc))->pV->Type[0] == 'A' ))
    {
    Conn.pDef->SetDefect( pDefect );
    return 0;
    }
  if ( Conn.pSrc->ID == 1 )
    {
    CACSPnt * pD_0 = (CACSPnt*)Conn.pSrc;
    CExtern_Pnt * pD_1 = pD_0->pV;
    if ( pD_1->Type[0] != 'D' || pD_1->Type[1] != 'O' )
      return 3;
  ////
    CDiscrete * pD = (CDiscrete*)pD_1;
    if ( lstrcmp ( pDefect->Name, "Восстановлен" ) == 0 )
      {
      pD->Defect = -1;
      return 0;
      }
    if ( lstrcmp ( pDefect->Name, "Постоянно 0" ) == 0 )
      {
      pD->Defect = 0;
      return 0;
      }
    if ( lstrcmp ( pDefect->Name, "Постоянно 1" ) == 0 )
      {
      pD->Defect = 1;
      return 0;
      }
    return 0;
    }
  ASS(0)
  return 0;
  }

int CCtrlConn::SetSensDefect( struct CDef * pDefect )
  {
  char ObjName[64];
  lstrcpy ( ObjName, pDefect->ObjName );
  for ( int n = 0; n < Connections.L; n++ )
    {
    CConnect & Conn = Connections[n];
    if ( Conn.Sens == ObjName )
      {
      if ( Conn.pDef )
        {
        int Res = 0;
        if ( Conn.Type[0] == 'D' )
          Res = SetDefectD( Conn, pDefect );
        else
          Res = Conn.pDef->SetDefect( pDefect );
        if ( Res )
          return Res;
        }
      }
    }
  return 0;
  }

int CConnectDefect::SetDefect ( struct CDef * pDefect )
  {
  FixCurrValue = false;
  IF_DEFECT(Исправен, "Исправен" );
  END_IF
    //
  IF_DEFECT(Показания_не_изменяются, "Показания не изменяются" );
//    Finish = pValue->dVal;
    FixCurrValue = true;
  END_IF
  IF_DEFECT(Завышение_показаний, "Завышение показаний" );
    p1 = parm_Завышение_показаний->Dbl;
    p2 = 0.;
  END_IF
  IF_DEFECT(Занижение_показаний, "Занижение показаний" );
    p1 = parm_Занижение_показаний->Dbl;
    p2 =  0.;
  END_IF
  IF_DEFECT(Уход_показаний_на_максимум, "Уход показаний на максимум" );
    Finish = 100000.;//SH * 1.001;
  END_IF
  IF_DEFECT(Уход_показаний_на_начало_шкалы, "Уход показаний на начало шкалы" );
    Finish = 0.;////SL * 0.999;
  END_IF
  IF_DEFECT(Самопроизвольное_изменение_показаний, "Самопроизвольное изменение показаний" );
    Alfa = 0.;
    FixCurrValue = true;
//    Start = pValue->dVal;
    Finish = parm_Самопроизвольное_изменение_показаний->Dbl;// "Конечное значение", 0., 0., 300. )
    Time = time_Самопроизвольное_изменение_показаний->Dbl;// "Время изменения с", 0., 0., 3600. )
  END_IF
  IF_DEFECT(Хаотическое_изменение_показаний, "Хаотическое изменение показаний" );
    p1 = min_Хаотическое_изменение_показаний->Dbl;// "Минимальное значение", 0., 0., 1000. )
    p2 = max_Хаотическое_изменение_показаний->Dbl;// "Максимальное значение", 0., 0., 1000. )
    Prop = 0.9;
    if ( p1 > p2 )
      {
      double w = p1;
      p1 = p2;
      p2 = w;
      }
    FixCurrValue = true;
//    Finish = pValue->dVal;
  END_IF
  IF_DEFECT(Сигнализация_HH, "Сигнализация HH" );
    Finish = HH * 1.001;
  END_IF
  IF_DEFECT(Сигнализация_H, "Сигнализация HI" );
    Finish = PH * 1.001;
  END_IF
  IF_DEFECT(Сигнализация_L, "Сигнализация LO" );
    Finish = PL * 0.999;
  END_IF
  IF_DEFECT(Сигнализация_LL, "Сигнализация LL" );
    Finish = LL * 0.999;
  END_IF
  IF_DEFECT(Сигнализация_IOPp, "Сигнализация IOP+" );
    Finish = SH + 5.;
  END_IF
  IF_DEFECT(Сигнализация_IOPm, "Сигнализация IOP-" );
    Finish = SL - 5.;
  END_IF
  IF_DEFECT(Восстановлен,"")
    Defect = 0;
  END_IF
  IF_DEFECT(Постоянно_0,"")
  END_IF
  IF_DEFECT(Постоянно_1,"")
  END_IF
  return 0;
  }

#undef IF_DEFECT
#define IF_DEFECT(Def,Comment) \
  if ( Defect == e##Def )\
  {

#undef END_IF
#define END_IF \
    }

double CConnectDefect::SetDefect( double V )
  {
  if ( Defect < 0 )
    return V;
  IF_DEFECT(Исправен, "Исправен" );
    return V;
  END_IF
    //
  IF_DEFECT(Показания_не_изменяются, "Показания не изменяются" );
    if ( FixCurrValue )
      {
      Finish = V;
      FixCurrValue = false;
      }
    return Finish;
  END_IF
  IF_DEFECT(Завышение_показаний, "Завышение показаний" );
    return V * p1;
  END_IF
  IF_DEFECT(Занижение_показаний, "Занижение показаний" );
    return V / p1;
  END_IF
  IF_DEFECT(Уход_показаний_на_максимум, "Уход показаний на максимум" );
    return Finish;
  END_IF
  IF_DEFECT(Уход_показаний_на_начало_шкалы, "Уход показаний на начало шкалы" );
    return Finish;
  END_IF
  IF_DEFECT(Самопроизвольное_изменение_показаний, "Самопроизвольное изменение показаний" );
    if ( FixCurrValue )
      {
      Start = V;
      FixCurrValue = false;
      }
    double dt = pSys->dt * 3600.;
    if ( Alfa < 1. )
      {
      if ( Time <= dt )
        Alfa = 1.;
      else
        {
        Alfa += dt / Time;
        if ( Alfa > 1. )
          Alfa = 1.;
        }
      }
    return Start + Alfa * ( Finish - Start );
  END_IF
  IF_DEFECT(Хаотическое_изменение_показаний, "Хаотическое изменение показаний" );
    if ( FixCurrValue )
      {
      Finish = V;
      FixCurrValue = false;
      }
    double Ksi = r01();
    double V = Finish;
    double A = 0.2;
    if ( Ksi < Prop )
      V = ( 1. - A ) * V + A * (double)p1;
    else
      V = ( 1. - A ) * V + A * (double)p2;
    Finish = V;
    if ( fabs ( V - p1 ) < 0.05 * ( p2 - p1 ))
      Prop = 0.1;
    if ( fabs ( V - p2 ) < 0.05 * ( p2 - p1 ))
      Prop = 0.9;
    return Finish;
  END_IF
  IF_DEFECT(Сигнализация_HH, "Сигнализация HH" );
    return Finish;
  END_IF
  IF_DEFECT(Сигнализация_H, "Сигнализация HI" );
    return Finish;
  END_IF
  IF_DEFECT(Сигнализация_L, "Сигнализация LO" );
    return Finish;
  END_IF
  IF_DEFECT(Сигнализация_LL, "Сигнализация LL" );
    return Finish;
  END_IF
  IF_DEFECT(Сигнализация_IOPp, "Сигнализация IOP+" );
    return Finish;
  END_IF
  IF_DEFECT(Сигнализация_IOPm, "Сигнализация IOP-" );
    return Finish;
  END_IF
  return 0;
  }

