#include "stdafx.h"
#include "Gag.h"
#include "Err.h"
#include "CommProc.h"
#include "SetMatr.h"
#include "Work.h"
#include "Connect.h"
#include "TestNodes.h"
#include "HydroStruct.h"

NODE_LIST
NODE_IN  ( 1, 0, "i","Вход" )
NODE_OUT ( 1, 1, "o","Выход")
END_LIST

#undef IF
#define IF(Node) if ( lstrcmp ( Node, Name ) == 0 )
extern "C"
EXP int TestNodes( char * ObjName,  int kNodes, struct CObjectPoint ** ppNodes )
  {
  int kIn = 0;
  int kOut = 0;
  int kErr = 0;
  for ( int n = 0; n < kNodes; n++ )
    {
    CObjectPoint * pOP = ppNodes[n];
    pOP->Type = eГидравлика;
    char * Name = pOP->PntName;
    IF("i")
      { kIn++; continue; };
    IF("o")
      { kOut++; continue; };
    ModMsg ( "У заглушки '%s' ошибочный вход '%s'", ObjName, pOP->PntName ), kErr++;
    }
  if ( kIn != 0 && kIn != 1 )
    ModMsg ( "У заглушки '%s' неправильно заданы вход", ObjName ), kErr++;
  if ( kOut != 0 && kOut != 1 )
    ModMsg ( "У заглушки '%s' неправильно заданы выход", ObjName ), kErr++;
  if ( kIn == 0 && kOut == 0 )
    ModMsg ( "У заглушки '%s' не заданы входы", ObjName ), kErr++;
  return kErr;
  }

int CGag::TestNodes(  int kNodes, struct CObjectPoint ** ppNodes  )
  {
  int Res = ::TestNodes( ObjName, kNodes, ppNodes );
  if ( Res )
    return Res;
  int kIn = 0;
  int kOut = 0;
  for ( int n = 0; n < kNodes; n++ )
    {
    CObjectPoint * pOP = ppNodes[n];
    pOP->Type = eГидравлика;
    char * Name = pOP->PntName;
    IF("i")
      { kIn++; continue; };
    IF("o")
      { kOut++; continue; };
    }
//
  OnlyIn = false;
  OnlyOut = false;
  if ( kIn == 0 )
    OnlyOut = true;
  if ( kOut == 0 )
    OnlyIn = true;
  //if ( kIn == 1 && kOut == 1 )
  //  {
  //  NoUsed = false;
  //  if ( pTestNoUsed )
  //    NoUsed = (*pTestNoUsed ) ( ObjName );
  //  }
   for ( int n = 0; n < kNodes; n++ )
    {
    CObjectPoint * pOP = ppNodes[n];
    char * Name = pOP->PntName;
    pOP->Type = eГидравлика;
    IF("i")
      { 
      if ( NoUsed )
        {
        pOP->PntGroup = 4; 
        pOP->NumbInGroup = 0;
        pOP->IO = '+';
        continue; 
        }
      if ( OnlyIn )
        {
        pOP->PntGroup = 2;  
        pOP->NumbInGroup = 0;
        }
      else
        {
        pOP->PntGroup = 1;  
        pOP->NumbInGroup = 0;
        }
      pOP->IO = '+';
      continue; 
      };
    IF("o")
      {
      if ( NoUsed )
        {
        pOP->PntGroup = 5; 
        pOP->NumbInGroup = 0;
        pOP->IO = '-';
        continue; 
        }
      if ( OnlyOut )
        {
        pOP->PntGroup = 3;  
        pOP->NumbInGroup = 0;
        }
      else
        {
        pOP->PntGroup = 1;  
        pOP->NumbInGroup = 1;
        }
      pOP->IO = '-';
      continue; 
      };
    ASS(0)
    }
  return 0;
  }

#include "SetMatr.h"

int CGag::SetMatr( void * pExternals, int PntGroup,  double dt, 
                  double A[/* kExkV */], double B[ /*kE*/ ] )
  {
  double Omega = CalcOmega( ) * kSS;
  if ( Заглушка == false )
    kSS = 1.;
  switch ( PntGroup )
    {
    case 1:
      MATR_OMEGA(Omega)
    break;
    case 2://OnlyIn
      A[P_0] = Omega;
      A[F_0] = -1.;
      B[0] = Omega;
    break;
    case 3: //OnlyOut
      A[P_0] = Omega;
      A[F_0] = 1.;
      B[0] = Omega;
    break;
    case 4: 
      // Вход упрощенный
      MATR_IN(1,1e-6)
    break;
    case 5: //OnlyOut
      // Выход упрощенный
      MATR_OUT(1,1e-6)
    break;
    }
  return 0;
  }

#include "SetVar.h"

int CGag::SetVar( void * pExternals, int PntGroup,  double dt, 
                 double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
  {
  switch ( PntGroup )
    {
    case 1:
      {
      F = Var[F_0];
      pF = (CFlow*)pVar[0];
      CFlow * pFlow_in = (CFlow*)pVar[0];
      CFlow * pFlow_out = (CFlow*)pVar[1];
      if ( F > 0. )
        *pFlow_out = *pFlow_in;
      else
        *pFlow_in = *pFlow_out;
      P_In = Var[P_0];
      P_Out = Var[P_1];
      dP = P_In - P_Out;
      }
    break;
    case 2://OnlyIn
      {
      F = Var[F_0];
      pF = (CFlow*)pVar[0];
      P_In = Var[P_0];
      P_Out = 1.;
      dP = P_In - P_Out;
      }
    break;
    case 3: //OnlyOut
      {
      F = Var[F_0];
      pF = (CFlow*)pVar[0];
      P_In = 1.;
      P_Out = Var[P_0];
      dP = P_In - P_Out;
      }
    break;
    case 4:
      {
      F = Var[F_0];
      pF = (CFlow*)pVar[0];
      P_In = Var[P_0];
      }
    break;
    case 5:
      {
      P_Out = Var[P_0];
      dP = P_In - P_Out;
      }
    break;
    }
  if ( GasGag )
    {
    if ( P_In > P_Out )
      {
      double Ksi = ( P_In - P_Out ) / P_In;
      if ( Ksi > 0.5 )
        kSS = 0.9 * kSS + 0.05 / Ksi;
      else
        kSS = 0.9 * kSS + 0.1;
      }
    else
      {
      double Ksi = ( P_Out - P_In ) / P_Out;
      if ( Ksi > 0.5 )
        kSS = 0.9 * kSS + 0.05 / Ksi;
      else
        kSS = 0.9 * kSS + 0.1;
      }
    }
  else
    kSS = 1.;
  return 0;
  }
