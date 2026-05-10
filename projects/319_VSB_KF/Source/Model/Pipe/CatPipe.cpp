#include "stdafx.h"
#include "Pipe.h"
#include "Err.h"
#include "CommProc.h"
//#include "Data.h"
#include "SetData.h"
#include "Work.h"
#include "HydroStruct.h"
#include "Data.h"


CCatPipe::CCatPipe( char * _ObjName )
  {
  UNIMODEL
  POOLCTRL
  ObjName = _ObjName;
  Model = "Пересылка катализатора";
  Type = NULL;
  Matr = NULL;
  IsMF = false;
  P = 0.;
  T = T_air;
  Flow_kg = 0.;
  }

CCatPipe::~CCatPipe()
  {

  }

int CCatPipe::StepT(double dt)
  {
  SET_BP BreakPoint;
  double FS = 0.;
  double HFS = 0.;
  CFlow * pFlow_max = NULL;
  double F_max = 0.;
  for ( int n = 0; n < kF; n++ )
    {
    if ( pFlow[n] == NULL )
      continue;
    double F = pFlow[n]->Flow_mol;
    if ( Type[n] == '+' )
      F = -F;
    if ( F > 0. )
      {
      FS += F;
      HFS += F * pFlow[n]->h_Flow;
      if ( F > F_max )
        {
        F_max = F;
        pFlow_max = pFlow[n];
        }
      }
    }
  const double Mw_Cat = 100.;//??
  Flow_kg = FS * 100.;
  if ( FS >= 0.01 )
    {
    double h = HFS / FS;
    const double Cp_cat = 100.;//??
    T = h / Cp_cat + 25.;
    for ( int n = 0; n < kF; n++ )
      {
      if ( pFlow[n] == NULL )
       continue;
      double F = pFlow[n]->Flow_mol;
      if ( Type[n] == '+' )
        F = -F;
      if ( F < 0. )
        {
        *pFlow[n] = *pFlow_max;
        pFlow[n]->h_Flow = h;
        }
      }
    }
  return 0;
  }

#include "IO_Parms.h"
int CCatPipe::GetParams(  char * StrName )
  {
  TAB("Транспорт катализатора", 1   )
  PARM( P, "#Давление" );
  PARM( T, "#Температура" );
  PARM( Flow_kg, "#Поток, кг" );
  ETAB
  return 0;
  }

int CCatPipe::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
  {
  ASS(0)
  return 0;
  }

int CCatPipe::SetMatr( void * pExternals, int PntGroup,  double dt, 
            double A[/* kExkV */], double B[ /*kE*/ ] )   // A * X = B
  {
//  int kF = 2;
  SET_BP BreakPoint;
  if ( kF != 2 )
    KKK();
  if ( IsMF )
    {
    memcpy ( A, Matr, ( kF + 1 ) * sizeof(double) );
    }
  else
    {
    for ( int n = 0; n < kF; n++ )
      {
      if ( Type[n] == '+' )
        A[n + 1] = 1.;
      else
        A[n + 1] = -1.;
      }
    memcpy ( Matr, A, ( kF + 1 ) * sizeof(double) );
    IsMF = true;
    }
  return 0;
  }

int CCatPipe::SetVar( void * pExternals, int PntGroup, double dt, 
                   // Переменные на входах
                      double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
  {
  SET_BP BreakPoint;
  P = Var[0];
  for ( int n = 0; n < kF; n++ )
    {
    pFlow[n] = (CFlow*)pVar[n];
    }
//
  return 0;
  }

int CCatPipe::InitPool ( struct CVarPool * pPool, struct CGroupObject * pObj ) //
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
  kF = kPoints;
  Type = NewArr(char,kPoints);
  //pNodes = NewArr(CNode,kPoints);
  pFlow = NewArr(CFlow*,kPoints);
  //h_F = NewArr(double,kPoints);
  //pComp_in = NewArr(CComp *,kPoints);
  //Flow_mol = NewArr(double,kPoints);
  //pShowFlow = NewArr(CShowF*,kPoints);
  //Flow_kg = NewArr(double,kPoints);
  //
  pPnt = pPool->pFirstPoint;
  pPool->kScalarVar = nVar;
  pPool->kPoints = kPoints;
  pPool->pStruct = NewArr(CBaseStruct*,kPoints);
  pPnt = pPool->pFirstPoint;
  int nPoints = 0;
  int nDrain = 0;
  while ( pPnt )
    {
    Type[nPoints] = pPnt->IO;
//    pNodes[nPoints].ObjName = pPnt->ObjName;
//    pNodes[nPoints].Point = pPnt->PntName;
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

