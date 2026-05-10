#include "stdafx.h"
#include "Zadv3x.h"
#include "Err.h"
#include "CommProc.h"
#include "Work.h"
#include "TestNodes.h"
#include "HydroStruct.h"

NODE_LIST
NODE_IN(1, 0, "i","Вход") 
NODE_IN(1, 1, "o1","Выход 1") 
NODE_IN(1, 2, "o2","Выход 2")
////
NODE_IN(2, 0, "o","Выход") 
NODE_IN(2, 1, "i1","Вход 1") 
NODE_IN(2, 2, "i2","Выход 2")
END_LIST



#undef IF
#define IF(Node) if ( lstrcmp ( Node, Name ) == 0 )
extern "C"
EXP int TestNodes( char * ObjName,  int kNodes, struct CObjectPoint ** ppNodes  )
  {
  if ( kNodes != 3 )
    {
    ModMsg ( "У 3-х задвижки '%s' число входов не равно трем", ObjName );
    return 1;
    }
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
    pOP->Type = eГидравлика;
    char * Name = NodeNoNumb ( pOP->PntName );
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
    ModMsg ( "У 3-х задвижки '%s' ошибочный вход '%s'", ObjName, pOP->PntName ), kErr++;
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
  ModMsg ( "У 3-х задвижки '%s' неправильно заданы входы", ObjName ), kErr++;
  return kErr;
  }

int CZadv3x::TestNodes(  int kNodes, struct CObjectPoint ** ppNodes  )
  {
  int Res = ::TestNodes( ObjName, kNodes, ppNodes );
  if ( Res )
    return Res;
  for ( int n = 0; n < kNodes; n++ )
    {
    CObjectPoint * pOP = ppNodes[n];
    pOP->Type = eГидравлика;
    char * Name = pOP->PntName;
    pOP->Type = eГидравлика;
    IF("i")
      { 
      pOP->PntGroup = 1;  
      pOP->NumbInGroup = 0;
      pOP->IO = '+';
      Type = 1;
      continue; 
      }
    IF("o1")
      { 
      pOP->PntGroup = 1;  
      pOP->NumbInGroup = 1;
      pOP->IO = '+';
      continue; 
      }
    IF("o2")
      { 
      pOP->PntGroup = 1;  
      pOP->NumbInGroup = 2;
      pOP->IO = '+';
      continue; 
      }
    IF("o")
      { 
      pOP->PntGroup = 2;  
      pOP->NumbInGroup = 0;
      pOP->IO = '+';
      Type = 2;
      continue; 
      }
    IF("i1")
      { 
      pOP->PntGroup = 2;  
      pOP->NumbInGroup = 1;
      pOP->IO = '+';
      continue; 
      }
    IF("i2")
      { 
      pOP->PntGroup = 2;  
      pOP->NumbInGroup = 2;
      pOP->IO = '+';
      continue; 
      }
    ASS(0)
    }
  return 0;
  }

#include "SetMatr.h"

int CZadv3x::SetMatr( void * pExternals, int PntGroup,  double dt, 
    double A[/* kExkV */], double B[ /*kE*/ ] )
  {
  SET_BP BreakPoint;
  // 0->1 открыт при V Угол_Закрытия ... 100
  // 0->2 открыт при V 100-Угол_Закрытия ... 0
  O01 += 1e-5;
  O02 += 1e-5;
  double * Eq = A;
  Eq[P_1] = O01;
  Eq[P_0] = -O01;
  Eq[F_1] = -1.;
  Eq += 6;
  Eq[P_2] = O02;
  Eq[P_0] = -O02;
  Eq[F_2] = -1.;
  Eq += 6;
  Eq[F_0] = 1.;
  Eq[F_1] = 1.;
  Eq[F_2] = 1.;
  return 0;
  }

#include "SetVar.h"

int CZadv3x::SetVar( void * pExternals, int PntGroup,  double dt, 
		double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
  {
  SET_BP BreakPoint;
  for ( int n = 0; n < 3; n++ )
    {
    pFlow[n] = (CFlow*)pVar[n];
    }
  return 0;
  }

