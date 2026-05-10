#include "stdafx.h"
#include "Collector.h"
#include "Err.h"
#include "CommProc.h"
#include "Work.h"
#include "HydroStruct.h"
#include "TestNodes.h"


NODE_LIST
NODE_IN  ( 1, 0, "i","Вход(суммарно <=64)")
NODE_OUT ( 1, 1, "o","Выход(суммарно <=64)")
END_LIST

extern "C"
EXP int TestNodes( char * ObjName,  int kNodes, struct CObjectPoint ** ppNodes  )
  {
  if ( kNodes <= 0 )
    {
    ModMsg ( "У коллектора '%s' не заданы входы", (char*)ObjName );
    return 1;
    }
  bool Err = false;
  int kIn = 0;
  int kOut = 0;
  int kU = 0;
  int kV = 0;
  for ( int n = 0; n < kNodes; n++ )
    {
    CObjectPoint * pOP = ppNodes[n];
    pOP->Type = eГидравлика;
    char Type = ' ';
    char * Name = pOP->PntName;
    kIn++;
    //if ( Name[0] == 'i' )
    //  {
    //  kIn++;
    //  continue;
    //  }
    //if ( lstrcmp ( Name, "вход" ) == 0 || Name[0] == 'v' )
    //  {
    //  kV++;
    //  continue;
    //  }
    //if ( Name[0] == 'o' )
    //  {
    //  kOut++;
    //  continue;
    //  }
    //if ( lstrcmp ( Name, "выход" ) == 0 || Name[0] == 'u'  )
    //  {
    //  kU++;
    //  continue;
    //  }
    //if ( Type == ' ' )
    //  {
    //  if ( pOP->pPool )
    //    ModMsg ( "У коллектора '%s' неправильный тип входа '%s'(к %s)", (char*)ObjName, Name, (char*)pOP->pPool->Name );
    //  else
    //    ModMsg ( "У коллектора '%s' неправильный тип входа '%s'(к %s)", (char*)ObjName, Name );
    //  Err = true;
    //  }
    }
  if ( kV > 1 )
    {
    Err = true;
    ModMsg ( "#У коллектора '%s' больше одного супервхода", (char*)ObjName );
    }
  if ( kU > 1 )
    {
    Err = true;
    ModMsg ( "#У коллектора '%s' больше одного супервыхода", (char*)ObjName );
    }
  if ( kV > 0 && kU > 0 )
    {
    Err = true;
    ModMsg ( "#У коллектора '%s' задан и супервход и супервыход", (char*)ObjName );
    }
  // Правильные варианты
  // kIn == 1, kOut >= 1
  // kIn >= 1, kOut == 1
/********
  bool ErrIO = true; 
  if ( kIn == 1 && kOut >= 1 )
    {
    ErrIO = false;
    }
  if ( kIn == 0 && kOut >= 1 )
    {
    ErrIO = false;
    ModMsg ( "#У коллектора '%s' только выходы. Нужно задать правильные параметры.", (char*)ObjName );
    }
  if ( kIn >= 1 && kOut == 1 )
    {
    ErrIO = false;
    }
  if ( kIn >= 1 && kOut == 0 )
    {
    ErrIO = false;
    ModMsg ( "#У коллектора '%s' только входы. Нужно задать правильное давление.", (char*)ObjName );
    }
  if ( ErrIO )
    {
    ModMsg ( "У коллектора '%s' нестандартное сочетание входов и выходов", (char*)ObjName );
    Err = true;
    }
*******/
  if ( Err )
    return 1;
  return 0;
  }

int CCollector::TestNodes(  int kNodes, struct CObjectPoint ** ppNodes )
  {
  if ( kNodes <= 0 )
    {
    ModMsg ( "У коллектора '%s' не заданы входы", (char*)ObjName );
    return 1;
    }
  int Res = ::TestNodes( (char*)ObjName, kNodes, ppNodes );
  if ( Res )
    return Res;
  pNodes = new CNode[kNodes];
  FlowSumm = NewArr(double,kNodes);
  kIO = kNodes;
  bool Err = false;
  int kIn = 0;
  int kOut = 0;
  int kU = 0;
  int kV = 0;
//  NumbIn = -1;
//  NumbOut = -1;
  for ( int n = 0; n < kNodes; n++ )
    {
    CObjectPoint * pOP = ppNodes[n];
    pOP->Type = eГидравлика;
    pNodes[n].Numb = pOP->PntGroup = n + 1;
    pOP->NumbInGroup = 0;
    pOP->IO = '+';
    pOP->Type = eГидравлика;
    pNodes[n].pOP = pOP;
    //if ( lstrcmp ( Name, "вход" ) == 0 || Name[0] == 'v' )
    //  NumbIn = n;
    //if ( lstrcmp ( Name, "выход" ) == 0 || Name[0] == 'u'  )
    //  NumbOut = n;
    kIn++;
    pNodes[n].Type = 'i';
    pNodes[n].Name = pOP->PntName;
    }
  return 0;
  }

#include "SetMatr.h"

int CCollector::SetMatr( void * pExternals, int PntGroup,  double dt, 
                        double A[/* kExkV */], double B[ /*kE*/ ] )
  {
  SET_BP BreakPoint;
  int n = PntGroup-1;
  if ( Defect )
    {
    A[P_0] = 1e-3;
    A[F_0] = -1.;
    B[0] = 0.;
    return 0;                  
    }
  if ( UseOut && ( Summ_In - 1 == n || Summ_Out - 1 == n ))
    {
    if ( Pio < P_min )
      {
      double O = Omega[n] * 1e5;
      A[P_0] = O;
      A[F_0] = -1.;
      B[0] = O * P_min - Flow_io;
      return 0;
      }
    if ( Pio > P_max )
      {
      double O = Omega[n] * 1e5;
      A[P_0] = O;
      A[F_0] = -1.;
      B[0] = O * P_max - Flow_io;
      return 0;
      }
    A[F_0] = 1.;
    B[0] = Flow_io;
    return 0;
    }
  if ( Use_Pz )
    {
    double O = Omega[n];// * kOmega[n];
    A[P_0] = O;
    A[F_0] = -1.;
    B[0] = O * P;
    return 0;
    }
/////////////////////////////////////////////
  double O = Omega[n];// * kOmega[n];
  double Ksi = dt * k_In;
  A[P_0] = -O;
  A[F_0] = 1. + Ksi * O;
  B[0] = -O * ( P + Ksi * FlowSumm[n] );
/*
  A[P_0] = O;
  A[F_0] = -1.;
  B[0] = O * P;
*/
  return 0;
  }

#include "SetVar.h"

int CCollector::SetVar( void * pExternals, int PntGroup, double dt, 
                       double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
  {
  SET_BP BreakPoint;
  int n = PntGroup - 1;
  if ( pNodes[n].Numb == PntGroup )
    {
    pFlow[n] = (CFlow*)pVar[0];
    if ( n == Summ_In - 1 || n == Summ_Out - 1 )
      {
        Flow_IO = Var[F_0];
      Pio = Var[P_0];
      }
    return 0;
    }
  return 0;
  }
