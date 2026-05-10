#include "stdafx.h"
#include "UniHydro.h"
#include "Err.h"


static CGroupRef Example;
void CGroupRef::Init()
  {
  memmove ( this, &Example, sizeof ( void* ));
  Numb = -1;
  pFirstPool = NULL;
  pLastPool = NULL;
  }

void CGroupRef::Add( CVarPool * pPool )
  {
  if ( pFirstPool == NULL )
    pFirstPool = pPool;
  if ( pLastPool )
    pLastPool -> pNextPool = pPool;
  pLastPool = pPool;
  pPool->pNextPool = NULL;
  }

CGroupObject & CGroupRef::Add( CObjectRef * pObj, CObjectPoint * pPnt )
  {
  for ( int n = 0; n < Objects.L; n++ )// исключаем повторный вызов объекта
    {
    CGroupObject & Old = Objects[n];
    if ( Old.ObjName == pObj->ObjName && Old.Model == pObj->Model && Old.PntGroup == pPnt->PntGroup )
      return Old;
    }
  CGroupObject & Obj = Objects.push_back();
  Obj.Init();
  Obj.ObjName = pObj->ObjName;
  Obj.Model = pObj->Model;
  Obj.Shema = pObj->Shema;
  Obj.pObject = pObj->pObject;
  Obj.pGroup = this;
  return Obj;
  }

