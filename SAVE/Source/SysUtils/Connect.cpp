#include "CommProc.h"
#include "BaseModel.h"
#include "Extern_Pnt.h"
#include "ACS_Def.h"

void * ConnectPoint( const char * Ref, char & Type )
  {
  if ( Ref == NULL )
    return NULL; // Внутренняя
  char Point[256];
  lstrcpy ( Point, Ref );
  char * P = strchr ( Point, '.' );
  if ( P == NULL )
    return NULL; // Нет точки
*P = 0;
  char * szParam = P + 1;
  char * Obj = Point;
  IBaseModel * pModel = IBaseModel::Find ( Obj );
  if ( pModel == NULL )
    return NULL;
  // ACS
  CExtern_Pnt * pPnt = NULL;
  if ( pModel )
    pPnt = (CExtern_Pnt*)::Find( &pModel->Points, szParam, false );
  if ( pPnt )
    {
    if ( pPnt->Type[0] == 'A' )
      {
      CAnalog * pA = (CAnalog*)pPnt;
      double * pAddr = (double*)&(pA->Value);
      Type = 'D';
      return pAddr;
      }
    if ( pPnt->Type[0] == 'D' )
      {
      CDiscrete * pD = (CDiscrete*)pPnt;
      int * pAddr = (int*)&(pD->Value);
      Type = 'I';
      return pAddr;
      }
    }
  // Параметры
//  CLockParams Lock;
  void * pValue = NULL;
  if(IBaseModel::Find(pModel->ObjName, szParam, Type, &pValue) == false)
    return NULL;
  return pValue;
  }

