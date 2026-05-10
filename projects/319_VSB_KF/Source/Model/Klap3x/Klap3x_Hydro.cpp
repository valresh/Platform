#include "stdafx.h"
#include "Klap3x.h"
#include "Err.h"
#include "CommProc.h"
#include "HydroStruct.h"
#include "SetMatr.h"
#include "SetVar.h"
#include "Work.h"
#include "TestNodes.h"

/*
//
NODE_IN(1,"i","Вход") 
NODE_IN(1,"o1","Выход 1(F = 100)") 
NODE_IN(1,"o2","Выход 2(F = 0)")
//
NODE_IN(2,"o","Выход") 
NODE_IN(2,"i1","Вход 1") 
NODE_IN(2,"i2","Вход 2")
*/

NODE_LIST
NODE_IN  ( 1, 0, "i","Вход" )
NODE_IN  ( 1, 1, "o1","Выход 1(F = 100)") 
NODE_IN  ( 1, 2, "o2","Выход 2(F = 0)")

NODE_IN  ( 2, 0, "o","Выход") 
NODE_IN  ( 2, 1, "i1","Вход 1") 
NODE_IN  ( 2, 2, "i2","Вход 2")
END_LIST

#undef IF
#define IF(Node) if ( lstrcmp ( Node, Name ) == 0 )
STATIC int TestNodes( char * ObjName,  int kNodes, struct CObjectPoint ** ppNodes  )
  {
  int k_i = 0;
  int k_o1 = 0;
  int k_o2 = 0;
  int k_o = 0;
  int k_i1 = 0;
  int k_i2 = 0;
  int kErr = 0;
  for ( int n = 0; n < kNodes; n++ )
    {
    CObjectPoint * pOP = ppNodes[n];
    char * Name = pOP->Point;
    IF("i")
      { k_i++; continue; };
    IF("o1")
      { k_o1++; continue; };
    IF("o2")
      { k_o2++; continue; };
    IF("o")
      { k_o++; continue; };
    IF("i1")
      { k_i1++; continue; };
    IF("i2")
      { k_i2++; continue; };
    ModMsg ( "У клапана '%s' ошибочный вход '%s'", ObjName, pOP->Point ), kErr++;
    }
  if ( k_i == 1 )
    {
    if ( k_o1 == 1 && k_o2 == 1 )
      return 0;
    }
  if ( k_o == 1 )
    {
    if ( k_i1 == 1 && k_i2 == 1 )
      return 0;
    }
  ModMsg ( "У клапана '%s' неправильно заданы входы", ObjName ), kErr++;
  return kErr;
  }

int CKlap3x::TestNodes(  int kNodes, struct CObjectPoint ** ppNodes  )
  {
  int Res = ::TestNodes( ObjName, kNodes, ppNodes );
  if ( Res )
    return Res;
  for ( int n = 0; n < kNodes; n++ )
    {
    CObjectPoint * pOP = ppNodes[n];
    char * Name = pOP->Point;
    IF("i")
      { 
      pOP->GrpPoint = 1;  
      pOP->IntNumb = 0;
      pOP->TypeF = '+';
      continue; 
      }
    IF("o1")
      { 
      pOP->GrpPoint = 1;  
      pOP->IntNumb = 1;
      pOP->TypeF = '+';
      continue; 
      }
    IF("o2")
      { 
      pOP->GrpPoint = 1;  
      pOP->IntNumb = 2;
      pOP->TypeF = '+';
      continue; 
      }
    IF("o")
      { 
      pOP->GrpPoint = 2;  
      pOP->IntNumb = 0;
      pOP->TypeF = '+';
      continue; 
      }
    IF("i1")
      { 
      pOP->GrpPoint = 2;  
      pOP->IntNumb = 1;
      pOP->TypeF = '+';
      continue; 
      }
    IF("i2")
      { 
      pOP->GrpPoint = 2;  
      pOP->IntNumb = 2;
      pOP->TypeF = '+';
      continue; 
      }
    }
  return 0;
  }

#include "SetMatr.h"
double O( double V, double X1, double Y1, double X2, double Y2 )
  {
  if ( V < X1 )
    return Y1;
  if ( V > X2 )
    return Y2;
  double Y = Y1 + ( Y2 - Y1 ) * ( V - X1 ) / ( X2 - X1 );
  return Y;
  }

int CKlap3x::SetMatr( void * pExternals, int PntGroup,  double dt, 
                     double A[/* kExkV */], double B[ /*kE*/ ] )
  {
  SET_BP BreakPoint;
  //if ( pPos )
  //  {
  //  if ( Defect == 1 )
  //    pPos->Vent = pPos->VentZ = Fixed_Pos;
  //  Vent = pPos->Vent;
  //  }
  if ( Defect == 1 )
    Vent = Fixed_Pos;
  double O_1 = ::O(Vent, Xo1_1, Yo1_1, Xo1_2, Yo1_2 ) * 0.01;
  if ( O_1 < 0.03 )
    {
    Конц_откр_o1 = 0;
    Конц_закр_o1 = 1;
    }
  else
    {
    if ( O_1 > 0.97 )
      {
      Конц_откр_o1 = 1;
      Конц_закр_o1 = 0;
      }
    else
      {
      Конц_откр_o1 = 0;
      Конц_закр_o1 = 0;
      }
    }
  double O_2 = ::O(Vent, Xo2_1, Yo2_1, Xo2_2, Yo2_2 ) * 0.01;
  if ( O_2 < 0.03 )
    {
    Конц_откр_o2 = 0;
    Конц_закр_o2 = 1;
    }
  else
    {
    if ( O_2 > 0.97 )
      {
      Конц_откр_o2 = 1;
      Конц_закр_o2 = 0;
      }
    else
      {
      Конц_откр_o2 = 0;
      Конц_закр_o2 = 0;
      }
    }
  O_1 += 1e-5;
  O_2 += 1e-5;
  O_1 *= Omega;
  O_2 *= Omega;
  switch ( PntGroup )
    {
    case 1:// i,o1,o2
      {
      double * Eq = A;
      Eq[P_0] = -O_1;
      Eq[P_1] = O_1;
      Eq[F_1] = -1.;
      //
      Eq += 6;
      Eq[P_0] = -O_2;
      Eq[P_2] = O_2;
      Eq[F_2] = -1.;
      // Сумма = 0
      Eq += 6;
      Eq[F_0] = 1.;
      Eq[F_1] = 1.;
      Eq[F_2] = 1.;
      }
    return 0;
    case 2:// o,i1,i2
      {
      double * Eq = A;
      Eq[P_0] = O_1;
      Eq[P_1] = -O_1;
      Eq[F_1] = 1.;
      //
      Eq += 6;
      Eq[P_0] = O_2;
      Eq[P_2] = -O_2;
      Eq[F_2] = 1.;
      // Сумма = 0 
      Eq += 6;
      Eq[F_0] = 1.;
      Eq[F_1] = 1.;
      Eq[F_2] = 1.;
      }
    return 0;
    default:
      ASS(0)
    break;
    }
  return 0;
  }

#include "SetVar.h"

int CKlap3x::SetVar( void * pExternals, int PntGroup, double dt, 
                    double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
  {
  SET_BP BreakPoint;
  switch ( PntGroup )
    {
    case 1:
      {
      for ( int n = 0; n < 3; n++ )
        {
        pFlow[n] = (CFlow*)pVar[n];
        }
      }
    return 0;
    case 2:
      {
      for ( int n = 0; n < 3; n++ )
        {
        pFlow[n] = (CFlow*)pVar[n];
        }
      }
    return 0;
    default:
      ASS(0)
    break;
    }
  return 0;
  }

