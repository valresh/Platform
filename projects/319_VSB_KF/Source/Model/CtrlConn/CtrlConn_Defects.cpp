#include "stdafx.h"
#include "CtrlConn.h"
#include "Err.h"
#include "SysDataTypes.h"


bool CCtrlConn::FireDefect( CDef * pDef )
  {
  if ( pDef == NULL || pDef->pModel == NULL )
    return false;
  if ( pDef->pModel->OnDefect ( pDef ))
    return true;
  return false;
  }

int CCtrlConn::OnDefect( CDef * pDefect )
	{
  SET_BP BreakPoint;
  //CDef * pDef;
  if ( pDefect == Resource[0] )
    {
    double Ta = ResParm_T[0]->Dbl;
    T_air = Ta;
    pExt->T_Air = Ta;
  END_IF
  if ( lstrcmp ( pDefect->Name, "Прекращение подачи электроэнергии на установку") == 0 )
    {
		for(int i = 0; i < IBaseModel::kObjects; i++)
		{
			IElectroModel* pElectro = dynamic_cast<IElectroModel*>(IBaseModel::AllObjects[i]);
			if(pElectro)
				pElectro->ElectroOff();
		}
    pExt->IsElectro_380 = false;
    pExt->IsElectro_6000 = false;
    return 1;
    }
  if ( lstrcmp ( pDefect->Name, "Возобновление подачи электроэнергии на установку") == 0 )
    {
		for(int i = 0; i < IBaseModel::kObjects; i++)
		{
			IElectroModel* pElectro = dynamic_cast<IElectroModel*>(IBaseModel::AllObjects[i]);
			if(pElectro)
				pElectro->ElectroOn();
		}
    pExt->IsElectro_380 = true;
    pExt->IsElectro_6000 = true;
    return 1;
    }
  if ( lstrcmp ( pDefect->Name, "Кратковременное прекращение подачи электроэнергии") == 0 )
    {
		for(int i = 0; i < IBaseModel::kObjects; i++)
		{
			IElectroModel* pElectro = dynamic_cast<IElectroModel*>(IBaseModel::AllObjects[i]);
			if(pElectro)
				pElectro->ElectroOff();
		}
    pExt->IsElectro_380 = false;
    pExt->IsElectro_6000 = false;
    NoElectroShort = true;
    TimeNoElectroShort = 1. / 3600.; // 5 сек
    return 1;
    }
  int nEvent = -1;
  for ( int n = 0; n < kEvents; n++ )
    {
    if ( pDefect == Events[n] )
      {
      nEvent = n;
      break;
      }
    }
  if ( nEvent >= 0 )
    {
    for ( int n = 0; n < kDefects; n++ )
      {
      if ( Defects[n].BaseN == nEvent )
        {
        FireDefect( Defects[n].pDef );
        }
      }
    return 0;
    }
    SetSensDefect( pDefect );
	return 0;
  }


void CCtrlConn::SetSensDefects()
  {
  for ( int n = 0; n < IBaseModel::kObjects; n++ )
    {
    IBaseModel * pModel = IBaseModel::AllObjects[n];
    if ( pModel->TypeObj !=  IBaseModel::MainModel )
      continue;
    CExtern_Pnt * pPnt = (CExtern_Pnt*)GetFirst( &pModel->Points );
    while ( pPnt )
      {
      if ( pPnt->Type[0] != 'A' || pPnt->Type[1] != 'O' || pPnt->Type[2] != 'D' )
        {
        pPnt = (CExtern_Pnt*)GetNext( );
        continue;
        }
      SetDefects((CAnalogD*)pPnt );
      pPnt = (CExtern_Pnt*)GetNext( );
      }
    }
  }

#undef SET_DEF
#define SET_DEF(DefectVar,Comment) \
  {\
  LastDefect = NewDefect( );\
  pDef->DefectVar = GetDefect( LastDefect );\
  pDef->DefectVar->Parent = RootNode;\
  strcpy_s ( pDef->DefectVar->Name, Comment );\
  sprintf_s( pDef->DefectVar->ObjName, 64, "%s@%s",  pDef->pModel->ObjName, pDef->Point );\
  pDef->DefectVar->Flags = CDef::Defects|CDef::Defect|CDef::Active;\
  pDef->DefectVar->pModel = this;\
  }

#undef SET_PARM
#define SET_PARM(Var, vName, vDef, vMin, vMax ) \
  {\
  pDef->Var = NewDefectPram( LastDefect );\
  pDef->Var->Type = CDefParam::tDbl;\
  strcpy_s(pDef->Var->Name, vName );\
  pDef->Var->Min = (float)vMin;\
  pDef->Var->Max = (float)vMax;\
  pDef->Var->Dbl = pDef->Var->_Dbl = (double)(vDef);\
  }

int CompSensInfo ( const void * p1, const void * p2 )
  {
  CSensInfo * s1 = (CSensInfo*)p1;
  CSensInfo * s2 = (CSensInfo*)p2;
  return lstrcmp ( s1->SensName, s2->SensName );
  }

#define MAX_SENS 1000
CSensInfo SensInfo[MAX_SENS];
int kSensInfo = 0;
bool SortSensInfo = true;

BOOL FindConnInfo( const char* szName, double * pValue );

BOOL FindSensInfo( const char* szName, double * pValue )
  {
//  if ( Version == УКПГ )
    {
    return FindConnInfo( szName, pValue );
    }
  if ( SortSensInfo )
    {
    SortSensInfo = false;
    qsort (SensInfo, kSensInfo, sizeof(CSensInfo), CompSensInfo );
    }
  CSensInfo Key;
  lstrcpy ( Key.SensName, szName );
  CSensInfo * pRes = (CSensInfo*)bsearch ( &Key, SensInfo, kSensInfo, sizeof(CSensInfo), CompSensInfo );
  if ( pRes == NULL )
    return FALSE;
  *pValue = *(pRes->pA);
  return TRUE;
  }

DWORD ClassRootNode = 0;
DWORD ClassRootNodeA = 0;
DWORD ClassRootNodeD = 0;
// Устарело
void CCtrlConn::SetDefects( struct CAnalogD * pDef )
  {
  if ( !pDef->IsConnection )
    return;
  if ( pDef->pModel == NULL )
    return;
  if ( (char*)pDef->Connection == NULL || (char*)pDef->Connection[0] == 0 )
    return;
  if ( strstr ( pDef->pModel->ObjName, "_out" ))
    return;
  char SensName[128] = { 0 };
//
  if ( pDef->pModel->Model == "Датчик" )
    {
    if ( pDef->pModel->ObjName[1] == '_' )
      return;
    }
  char * P = strchr ( SensName, '|' );
  if ( P )
    *P = 0;
  if ( ClassRootNode == 0 )
    {
    ClassRootNode = NewDefect( );
    CDef * pRoot = GetDefect( ClassRootNode );
    strcpy_s ( pRoot->Name, "Датчики" );
    pRoot->Flags = CDef::Defects|CDef::Node|CDef::Active;
    pRoot->Parent = GetRootDefects( );
    }
//
  DWORD LastDefect = 0;
  DWORD RootNode = NewDefect( );
  CDef * pNode = GetDefect( RootNode );
  pNode->Parent = ClassRootNode;
//
  if ( pDef->pModel->Model == "Датчик" )
    {
    strcpy_s ( SensName, pDef->pModel->ObjName );
    }
  else
    {
    if ( pDef->Connection[0] == '[' )
      {
//      Fsend ( "Нет отказов.txt", "%s.%s", pDef->pModel->ObjName, pDef->Point );
      return;
      }
    strcpy_s ( SensName, (char*)pDef->Connection );
    }
//
  ASS(kSensInfo<MAX_SENS)
  strcpy_s ( SensInfo[kSensInfo].SensName, SensName );//pDef->Connection );
  SensInfo[kSensInfo].pA = pDef;
  kSensInfo++;
  strcpy_s ( pNode->Name, SensName);//pDef->Connection );
  pNode->Flags = CDef::Defects|CDef::Node|CDef::Active;
//
  SET_DEF(Исправен, "Исправен" );
  SET_DEF(Показания_не_изменяются, "Показания не изменяются" );
  SET_DEF(Завышение_показаний, "Завышение показаний" );
  SET_PARM( parm_Завышение_показаний, "Завышение", 1.2, 1., 10. )
    SET_DEF(Занижение_показаний, "Занижение показаний" );
  SET_PARM( parm_Занижение_показаний, "Занижение", 1.2, 1., 10. )
    SET_DEF(Уход_показаний_на_максимум, "Уход показаний на максимум" );
  //		  SET_PARM( time_Уход_показаний_на_максимум, "Время изменения с", 0., 0., 3600. )
  SET_DEF(Уход_показаний_на_начало_шкалы, "Уход показаний на начало шкалы" );
  //		  SET_PARM( time_Уход_показаний_на_начало_шкалы, "Время изменения с", 0., 0., 3600. )
  SET_DEF(Самопроизвольное_изменение_показаний, "Самопроизвольное изменение показаний" );
    SET_PARM( parm_Самопроизвольное_изменение_показаний, "Конечное значение", 0., -100., 10000. )
    SET_PARM( time_Самопроизвольное_изменение_показаний, "Время изменения с", 0., 0., 3600. )
  SET_DEF(Хаотическое_изменение_показаний, "Хаотическое изменение показаний" );
    SET_PARM( min_Хаотическое_изменение_показаний, "Минимальное значение", 0., -1000., 10000. )
    SET_PARM( max_Хаотическое_изменение_показаний, "Максимальное значение", 0., -1000., 10000. )
  }


int CCtrlConn::SetDefect( struct CDef * pDefect )
  {
  char ObjName[64];
  char Point[64];
  lstrcpy ( ObjName, pDefect->ObjName );
  char * P = strchr ( ObjName, '@' );
  if ( P == NULL )
    return 0;
  *P = 0;
  lstrcpy ( Point, P + 1 );
  for ( int n = 0; n < IBaseModel::kObjects; n++ )
    {
    IBaseModel * pModel = IBaseModel::AllObjects[n];
    if ( pModel->TypeObj !=  IBaseModel::MainModel )
      continue;
    CExtern_Pnt * pPnt = (CExtern_Pnt*)GetFirst( &pModel->Points );
    while ( pPnt )
      {
      if ( pPnt->Type[0] != 'A' || pPnt->Type[1] != 'O' || pPnt->Type[2] != 'D' )
        {
        pPnt = (CExtern_Pnt*)GetNext( );
        continue;
        }
      if ( lstrcmp ( Point, pPnt->Point ) != 0 )
        {
        pPnt = (CExtern_Pnt*)GetNext( );
        continue;
        }
      if ( lstrcmp ( ObjName, pPnt->pModel->ObjName ) == 0 )
        {
        CAnalogD * pA = (CAnalogD*)pPnt;
        int Res = SetDefect( pA, pDefect );
        if ( Res )
          return Res;
        KKK();
        }
      pPnt = (CExtern_Pnt*)GetNext( );
      }
    }
  return 0;
  }

int CCtrlConn::SetDefect( struct CAnalogD * pPnt, struct CDef * pDefect )
  {
  return pPnt->SetDefect ( pDefect );
  }

template<size_t STR_LEN, size_t K_COL, size_t K_LEN>
int SplitStr ( char (&Str)[STR_LEN], char (&Params)[K_COL][K_LEN] )
  {
  for ( size_t n = 0; n < K_COL; n++ )
  {
    memset( Params[n], 0, K_LEN );
  }
  char * P = Str;
  int L = lstrlen(P)-1;
  if ( L < 2 )
    return -1;
  P[L] = 0;  
  for ( size_t n = 0; n < K_COL; n++ )
    {
    char * Pn = strchr ( P, ';' );
    if ( Pn )
      *Pn = 0;
    if ( lstrlen(P) >= K_LEN )
      {
      memcpy ( Params[n], P, K_LEN );
      Params[n][K_LEN-1] = 0;
      Fsend ( "ДлинныеИмена.txt", "%s;%s;\n", P, Params[n] );
      }
    else
      strcpy_s( Params[n], P );
    if ( Pn == NULL )
      return n + 1;
    P = Pn + 1;
    }
  return K_COL;
  }

void CCtrlConn::SetEvents( )
  {
  char * File = "";
  File = "Аварии";
  DWORD Evevts;
  Evevts = RootNode = GetRootEvents( );
  Char<1024>Path;
  Path.Prt ( "%sData\\Equipment\\%s.csv", PROJECT_ROOT, File );
  FILE * F = fopen ( Path, "rt" );
  if ( F == NULL )
    return;
  DWORD LastDefect;
  char Str[512];
  fgets ( Str, 511, F );
  while ( fgets ( Str, 511, F ))
    {
    char Params[3][64];
    int K = SplitStr( Str, Params );
//    if ( Params[0][0] == 0 )
//      continue;
    if ( Params[0][0] == '#' )
      {
// Папка
      LastDefect = NewDefect( );
      CDef * pDef = GetDefect( LastDefect );
      pDef->Parent = Evevts;
      lstrcpy ( pDef->Name, Params[0]+1 );
      lstrcpy ( pDef->ObjName, ObjName );
      pDef->Flags = CDef::Events|CDef::Node|CDef::Active;
      RootNode = LastDefect;
      continue;
      }
    if ( Params[0][0] )
      {
      LastDefect = NewDefect( );
      CDef * pDef = GetDefect( LastDefect );
      pDef->Parent = RootNode;
      lstrcpy ( pDef->Name, Params[0] );
      lstrcpy ( pDef->ObjName, ObjName );
      pDef->Flags = CDef::Events|CDef::Defect|CDef::Active;
      pDef->pModel = this;
      ASS (kEvents < MAX_EVENTS )
      if ( kEvents >= MAX_EVENTS )
        break;
      Events[kEvents++] = pDef;
      }
    //
    CDef * pDef = GetDefect( Params[1], Params[2] );
    if ( pDef == NULL )
      {
      if ( Params[1][0] )
        Fsend ( "ErrDef.txt", "He найден отказ '%s.%s'\n", Params[1], Params[2] );
      continue;
      }
    ASS (kDefects < MAX_DEFECTS )
    if ( kDefects >= MAX_DEFECTS )
      break;
    Defects[kDefects].pDef = pDef;
    Defects[kDefects].BaseN = kEvents - 1;
    kDefects++;
    }
  fclose ( F );
  }
