#include "stdafx.h"
#include "UniHydro.h"
#include "Err.h"


#ifdef LINUX
#else
CCSVData * pModelDLL;//Проект;Модель;DLL;Описание
#endif


char * CObjectRef::NameCoord()
  {
  static char Name[128];
  sprintf_s ( Name, 128, "%s[%4.0lf,%4.0lf]", (char*)ObjName, X, Y );
  return Name;
  }

//////////////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////
CMem<CObjectRef,256,256> * pObjRefs = NULL;

int CompObjByName ( const void * p1, const void * p2 )
  {
  int n1 = *(int*)p1;
  int n2 = *(int*)p2;
  CObjectRef & Obj1 = (*pObjRefs)[n1];
  CObjectRef & Obj2 = (*pObjRefs)[n2];
  return lstrcmp ( Obj1.ObjName, Obj2.ObjName );
  }

int FindObjByName ( const void * p1, const void * p2 )
  {
  char * n1 = (char*)p1;
  int n2 = *(int*)p2;
  CObjectRef & Obj2 = (*pObjRefs)[n2];
  return lstrcmp ( n1, Obj2.ObjName );
  }

CMem<CVarPool,256,256> * pVarConn = NULL;
int CompConnByName ( const void * p1, const void * p2 )
  {
  int n1 = *(int*)p1;
  int n2 = *(int*)p2;
  CVarPool & C1 = (*pVarConn)[n1];
  CVarPool & C2 = (*pVarConn)[n2];
  return lstrcmp ( C1.Name, C2.Name );
  }

CMem<CObjectPoint,512,256> * pObjPoints;
int CompObjPoints ( const void * p1, const void * p2 )
  {
  int n1 = *(int*)p1;
  int n2 = *(int*)p2;
  CObjectPoint & P1 = (*pObjPoints)[n1];
  CObjectPoint & P2 = (*pObjPoints)[n2];
  int Res = lstrcmp ( P1.ObjName, P2.ObjName );
  if ( Res )
    return Res;
  return lstrcmp ( P1.PntName, P2.PntName );
  }

int FindObjPoints ( const void * p1, const void * p2 )
  {
  CPoolPoint * Key = (CPoolPoint*)p1;
  int n2 = *(int*)p2;
  CObjectPoint & P2 = (*pObjPoints)[n2];
  int Res = lstrcmp ( Key->ObjName, P2.ObjName );
  if ( Res )
    return Res;
  return lstrcmp ( Key->PntName, P2.PntName );
  }

//.................................................................
//typedef 
//struct IBaseModel* (*tCreateObject)( LPCTSTR ObjName );

//char PathDllCat[256];
#ifdef LINUX
#else
IBaseModel * Load_Obj ( const char * ObjName, char * Model )
  {
  char * DLL = NULL;
  char * DIR = NULL;
  char * SubType = NULL;
  char ** pRes = NULL;
  if ( lstrcmp ( Model, "Pipe" ) == 0 )
    {
		pRes = pModelDLL->Find( Model );////MODEL;PROJECT;DLL;DESCR
		if(pRes)
		{
			DIR = pRes[2];
			DLL = pRes[3];
			SubType = pRes[1];
		}
		else
		{
			DIR = "SYSTEM";
			DLL = "Pipe";
			SubType = "";
		}
//     if ( PathDllCat[0] && ( ObjName[0] == 'c' || ObjName[0] == 'C' ))
//       {
//       DIR = PathDllCat;
//       DLL = "PipeCat";
//       }
    }
  else
    if ( lstrcmp ( Model, "Signal" ) == 0 )
      {
      DIR = "SYSTEM";
      DLL = "Signal";
      SubType = "";
      }
    else
      {
      pRes = pModelDLL->Find( Model );////MODEL;PROJECT;DLL;DESCR
      if ( pRes == NULL )
        {
//        SysMSG("Не найдена модель '%s", Model );
//        return NULL;
        DIR = "SYSTEM";
        DLL = Model;
        SubType = "";
        }
      else
        {
        DIR = pRes[2];
        DLL = pRes[3];
        SubType = pRes[1];
        }
      }
  HMODULE hLib = NULL;
  if( !strcmp( DIR, "SYSTEM"))
	  hLib = (HMODULE)Load_Dll( DLL );
  else 
	  hLib = (HMODULE)Load_Dll( DLL, DIR );
  if ( hLib == NULL )
    {
    if ( pRes == NULL )
      SysMSG( "Модель '%s' не найдена в ModelDll.csv", Model );
    else
      SysMSG( "Ошибка загрузки DLL '%s' для модели '%s'", DLL, Model );
    //    Fsend ( "NoModel.csv", "%s;%s;\n", DLL, Model );
    return NULL;
    }
  tCreateObject pCreate = (tCreateObject)::GetProcAddress( hLib, "CreateObject");
  if ( pCreate == NULL )
    {
    SysMSG("Ошибочная структура DLL '%s'", DLL );
    return NULL;
    }
  IBaseModel * pModel = (*pCreate) ( ObjName, SubType );
  if ( pModel == NULL )
    {
    SysMSG("Ошибка создания объекта '%s'('%s')", ObjName, Model );
    return NULL;
    }
  pModel->Model = Model;
  //pModel->SubType = SubType;
  return pModel;
  }
#endif
//////////////////////////////////////////////////////////////////////////
#undef SET
#define SET(Var) Var = From.Var;
void CObjectRef::operator = ( CObjectRef & From )
  {
  SET(ObjName)
  SET(Model)
  SET(Shema)
  SET(Master)
  SET(Data3)
  SET(Numb)
  SET(kPoints)
  SET(kProps)
  SET(Flags)
  SET(nGroup)
  SET(PntGroup)
  SET(X)
  SET(Y)
  SET(L)
  SET(R)
  SET(T)
  SET(B)
  SET(Selected)
  SET(pFirstPoint)
  SET(pLastPoint)
  SET(pFirstProp)
  SET(pLastProp)
  SET(pMain)
  SET(pObject)
  CObjectPoint * pPnt = pFirstPoint;
  while ( pPnt )
    {
    ASS(lstrcmp ( pPnt->ObjName, ObjName ) == 0 )
    pPnt->pObj = this;
    pPnt = pPnt->pNext;
    }
  }

void CUniHydro::CompressObjList()
  {
// Исключаем повторы
  int N0 = SortObjByName[0];
  CObjectRef * pPrev = &ObjRefs[N0];
  pPrev->Selected = true;
  CObjectRef * pCurr = NULL;
  int p = 0;
  for ( int n = 1; n < ObjRefs.L; n++ )
    {
    int N = SortObjByName[n];
    pCurr = &ObjRefs[N];
    for ( int i = 0; i < VarPool.L; i++ )
    {
      CVarPool & Pool = VarPool[i];
      if (Pool.Name == pCurr->ObjName )
        SysMSG("Имя турбы совпадает с именем объекта '%s'." , (char*)pCurr->ObjName);
    }
    if ( lstrcmp ( pCurr->ObjName, pPrev->ObjName ) != 0 )
      {
// Разные
      pCurr->Selected = true;
      pPrev = pCurr;
      p++;
      if ( p == n )
        continue;
      continue;
      }
    if (!( pPrev->Model == pCurr->Model ))
      {
        SysMSG("Ошибка объект '%s' имеет разные модели. Схемы '%s' и '%s'" , (char*)pCurr->ObjName,  pCurr->Shema, pPrev->Shema);
      }
// Объединение списков точек
    CObjectPoint * pPnt = pCurr->pFirstPoint;
    while ( pPnt )
      {
      CObjectPoint * pNext = pPnt->pNext;
      pPrev->Add ( pPnt );
      pPnt = pNext;
      }
// Объединение Props
    CObjProps * pProp = pCurr->pFirstProp;
    while ( pProp )
      {
      CObjProps * pNext = pProp->pNext;
      pPrev->Add ( pProp );
      pProp = pNext;
      }
/*
    CDynConnector * pLine = pCurr->pFirstLine;
    while ( pLine )
      {
      CDynConnector * pNext = pLine->pNext;
      pLine->pNext = pPrev->pFirstLine;
      pPrev->pFirstLine = pLine;
      pLine = pNext;
      }
*/
//!!!???
    if ( strstr ( pPrev->Shema, pCurr->Shema ) == 0 )
      {
      char Shema[1024];
      strcpy_s ( Shema, 1024, pPrev->Shema );
      strcat_s ( Shema, 1024, pCurr->Shema );
      pPrev->Shema = Shema;
      }
    pCurr->Selected = false;
    }
  int q = 0;
  for ( int n = 0; n < ObjRefs.L; n++ )
    {
    if ( ObjRefs[n].Selected )
      {
      if ( q == n )
        {
        q++;
        continue;
        }
      ObjRefs[q++] = ObjRefs[n];
      }
    else
      KKK();
    }
  ASS( q == p + 1 )
  if ( ObjRefs.L == p + 1 )
    {
    for ( int n = 0; n < ObjRefs.L; n++ )
      {
      CObjectRef & Obj = ObjRefs[n];
      Obj.Numb = n;
      }
    }
  else
    {
    ObjRefs.L = p + 1;
    for ( int n = 0; n < ObjRefs.L; n++ )
      {
      CObjectRef & Obj = ObjRefs[n];
      Obj.Numb = n;
      SortObjByName[n] = n;
      }
    qsort ( SortObjByName, ObjRefs.L, sizeof ( int ), CompObjByName );
    }
//
  }

void CUniHydro::CompressConnectionList()
  {
// Исключаем повторы
  int N0 = SortPoolByName[0];
  CVarPool * pPrev = &VarPool[N0];
  pPrev->Selected = true;
  CVarPool * pCurr = NULL;
  int p = 0;
  for ( int n = 1; n < VarPool.L; n++ )
    {
    int N = SortPoolByName[n];
    pCurr = &VarPool[N];
    if ( lstrcmp ( pCurr->Name, pPrev->Name ) != 0 )
      {
// Разные
      pCurr->Selected = true;
      pPrev = pCurr;
      p++;
      if ( p == n )
        continue;
      continue;
      }
    if (!( pPrev->Type == pCurr->Type ))
      {
      KKK();
      }
// Объединение списков точек
    CObjectPoint * pPnt = pCurr->pFirstPoint;
    while ( pPnt )
      {
      CObjectPoint * pNext = pPnt->pNext;
      pPrev->Add ( pPnt );
      pPnt = pNext;
      }
// Объединение Props
    CObjProps * pProp = pCurr->pFirstProp;
    while ( pProp )
      {
      CObjProps * pNext = pProp->pNext;
      pPrev->Add ( pProp );
      pProp = pNext;
      }
    pCurr->Selected = false;
    }
  int q = 0;
  for ( int n = 0; n < VarPool.L; n++ )
    {
    if ( VarPool[n].Selected )
      {
      if ( q == n )
        {
        q++;
        continue;
        }
      VarPool[q++] = VarPool[n];
      }
    else
      KKK();
    }
  ASS( q == p + 1 )
  if ( VarPool.L == p + 1 )
    {
    for ( int n = 0; n < VarPool.L; n++ )
      {
      CVarPool & Var = VarPool[n];
      Var.Numb = n;
      }
    }
  else
    {
    VarPool.L = p + 1;
    for ( int n = 0; n < VarPool.L; n++ )
      {
      CVarPool & Var = VarPool[n];
      Var.Numb = n;
      SortPoolByName[n] = n;
      }
    qsort ( SortPoolByName, VarPool.L, sizeof ( int ), CompConnByName );
    }
  }

//////////////////////////////////////////////////////////////////////////////////////
int SortByStep ( const void * p1, const void * p2 )
  {
  CStepActionVar * v1 = (CStepActionVar*)p1;
  CStepActionVar * v2 = (CStepActionVar*)p2;
  if ( v1->Step < v2->Step )
    return -1;
  if ( v1->Step > v2->Step )
    return 1;
  return 0;
  }
