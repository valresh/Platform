#include "stdafx.h"
#include "Pipe.h"
#include "Err.h"
#include "HydroStruct.h"
#include "Data.h"

#if 0
CObjectPoint ** pPoints;      // Упорядоченные ссылки на CObjectPoint по порядковым номерам в пределах внутренней группы
int * kPointsVar;             // Число скалярных переменных у каждой точки
int kPoints;                  // Чисто точек
int kLinearVar;               // Чисто скалярных переменных у объекта
int * NumbVarInGroup/*[kV]*/; // Номера скалярных переменных в общем списке группы
double ** AddrVar/*[kV]*/;    // Адреса скалярных переменных
int kLinearEq;                // Общее число уравнений для скалярных переменных
int Order;                    // Порядковый номер при вызове
#endif

int CPipe::InitPool ( struct CVarPool * pPool, struct CGroupObject * pObj ) //
  {
  pObj->pObject = this;
  pObj->ObjName = ObjName;
  pObj->Model = Model;
//  pObj->ObjShema = pPool->S
  CObjectPoint * pPnt = pPool->pFirstPoint;
  Char<256> Name;
  int nPress = pPool->FirstGroupNumbVar;
  int nVar = 1;
  int kPoints = 0;
  while ( pPnt )
    {
    Name.Prt ( "%s.%s|%s", pPnt->ObjName.Str, pPnt->PntName.Str, pPool->Name.Str );
    CFlow * pFlow = (CFlow*)Create ( Name, CFlow::TypeID, nVar );
    pFlow->Init();
    pPnt->pVar = pFlow;
    pPnt->kScalarVar = 2;
    pPnt->kScalarEq = 1;
    pPnt->GroupNumbVar = NewArr(int,2);
    pPnt->GroupNumbVar[0] = nPress;
    pPnt->GroupNumbVar[1] = nPress + nVar;
    pPnt->AddrVar = NewArr(double*,2);
    pPnt->AddrVar[0] = &pFlow->P;
    pPnt->AddrVar[1] = &pFlow->Flow_mol;
    nVar++;
    kPoints++;
    pPnt = pPnt->pNextPoolPoint;
    }
  int Max = 10;
  if ( kPoints > Max )
    KKK();
  kF = kPoints;
  Type = NewArr(char,kPoints);
  pNodes = NewArr(CNode,kPoints);
  pFlow = NewArr(CFlow *,kPoints);
  h_F = NewArr(double,kPoints);
  pComp_in = NewArr(CComp *,kPoints);
  Flow_mol = NewArr(double,kPoints);
  pShowFlow = NewArr(CShowF*,kPoints);
  Flow_kg = NewArr(double,kPoints);
//
  pPnt = pPool->pFirstPoint;
  pPool->kScalarVar = nVar;
  pPool->kPoints = kPoints;
  pPool->pStruct = NewArr(CBaseStruct*,kPoints);
  kDrain = 0;
  while ( pPnt )
    {
    if ( pPnt->pObj && pPnt->pObj->Model == "Дренаж" )
      kDrain++;
    pPnt = pPnt->pNextPoolPoint;
    }
  if ( kDrain > 0 )
    {
    CalcT = true;
    pDrainModel = NewArr(CDrainage*,kDrain);
    }
  pPnt = pPool->pFirstPoint;
  int nPoints = 0;
  int nDrain = 0;
  while ( pPnt )
    {
    Type[nPoints] = pPnt->IO;
    pNodes[nPoints].ObjName = pPnt->ObjName;
    pNodes[nPoints].Point = pPnt->PntName;
    if ( pPnt->pObj && pPnt->pObj->Model == "Дренаж" )
      {
      pDrainModel[nDrain++] = (CDrainage*)pPnt->pObj->pObject;
      if ( strchr ( pPnt->PntName, 'g' ))
        pNodes[nPoints].Drainage = 1;
      else
        pNodes[nPoints].Drainage = -1;
      }
    else
      pNodes[nPoints].Drainage = 0;
    pPool->pStruct[nPoints++] = pPnt->pVar;
    pPnt = pPnt->pNextPoolPoint;
    }
//
  pObj->kPoints = kPoints;
  pObj->PntGroup = 1;
  pObj->pPoints = NewArr(CObjectPoint*,kPoints);// Упорядоченные ссылки на CObjectPoint по порядковым номерам в пределах внутренней группы
  pObj->kPointsVar = NewArr(int,kPoints); // Число скалярных переменных у каждой точки
  pObj->kPoints = kPoints;  // Чисто точек
  pObj->kLinearVar = kPoints + 1; // Чисто скалярных переменных у объекта
  pObj->NumbVarInGroup = NewArr(int,kPoints+1);/*[kV]*/; // Номера скалярных переменных в общем списке группы
  pObj->AddrVar = NewArr(double*,kPoints+1);    // Адреса скалярных переменных
  pObj->kLinearEq = 1;                // Общее число уравнений для скалярных переменных
//.........................................................
  pPnt = pPool->pFirstPoint;
  int nPnt = 0;
  pObj->NumbVarInGroup[0] = nPress;
  pObj->AddrVar[0] = pPool->pFirstPoint->AddrVar[0];//Давление
  nVar = 1;
  while ( pPnt )
    {
    pObj->pPoints[nPnt] = pPnt;
    pObj->kPointsVar[nPnt] = 2;
    nPnt++;
    pObj->NumbVarInGroup[nVar] = nPress + nVar;
    pObj->AddrVar[nVar] = pPnt->AddrVar[1]; //Потоки
    nVar++;
    pPnt = pPnt->pNextPoolPoint;
    }
  /////////////////////////////////////////////////////
  pObj->l_A = pObj->kLinearEq * pObj->kLinearVar;
  pObj->A = NewArr ( double, pObj->l_A ); // Ур-я имеют вид A * X = B  
  Matr = NewArr ( double, pObj->l_A );
  pObj->Ind_A = NewArr ( int, pObj->l_A );
  pObj->B = NewArr ( double, pObj->kLinearVar );                                
  pObj->IsMatrInd = false;
  return 0;
  }
