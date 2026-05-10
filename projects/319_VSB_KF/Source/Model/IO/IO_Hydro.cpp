#include "stdafx.h"
#include "IO.h"
#include "Err.h"
#include "CommProc.h"
#include "Work.h"
#include "TestNodes.h"
#include "HydroStruct.h"


NODE_LIST
  NODE_IN (1, 0, "i","Вход потребителя")
  NODE_IN (1, 0, "a","Выход в атмосферу")
  NODE_OUT(1, 0, "o","Выход источника")
//
//ANALOG_IN ( 2, "Управление","Управление выходом источника")
//ANALOG_OUT( 3, "Выход",     "Выход источника")
END_LIST


#undef IF
#define IF(Node) if ( lstrcmp ( Node, Name ) == 0 )

int CIO::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
  {
  if ( kNodes <= 0 )
    {
    ModMsg ( "У источника '%s' не заданы входы", ObjName.Str );
    return 1;
    }
  if ( kNodes > 2 )
    {
    ModMsg ( "У источника '%s' более 2-х входов", ObjName.Str );
    return 1;
    }
  if ( kNodes == 2 )
    {
    CObjectPoint * pOP_0 = ppNodes[0];
    pOP_0->Type = eГидравлика;
    CObjectPoint * pOP_1 = ppNodes[1];
    pOP_1->Type = eГидравлика;
    char * Name_0 = pOP_0->Point;
    char * Name_1 = pOP_1->Point;
    bool OK = false;
    if ( Name_0[0] == 'i' && Name_1[0] == 'o' )
      {
      pOP_0->NumbInGroup = 0;
      pOP_1->NumbInGroup = 1;
      pOP_0->IO = '+';
      pOP_1->IO = '-';
      OK = true;
      }
    if ( Name_0[0] == 'o' && Name_1[0] == 'i' )
      {
      pOP_1->NumbInGroup = 0;
      pOP_0->NumbInGroup = 1;
      pOP_1->IO = '+';
      pOP_0->IO = '-';
      OK = true;
      }
    if ( !OK )
      {
      ModMsg ( "У источника '%s' неправильные входы ('%s','%s')", ObjName.Str, pOP_0->Point.Str, pOP_1->Point.Str );
      return 2;
      }
    pOP_0->PntGroup = 1;  // i,o
    pOP_1->PntGroup = 1;
    Type = IO;
    return 0;
    }
  if ( kNodes == 1 )
    {
    CObjectPoint * pOP = ppNodes[0];
    char * Name = pOP->Point;
    if ( Name[0] == 'i' )
      {
      Type = In;
      pOP->PntGroup = 2;  // i
      pOP->NumbInGroup = 0;
      pOP->IO = '+';
      return 0;
      }
    if ( Name[0] == 'a' )
      {
      Type = Atm;
      pOP->PntGroup = 2;  // i,a
      pOP->NumbInGroup = 0;
      pOP->IO = '+';
      return 0;
      }
    if ( Name[0] == 'o' )
      {
      Type = Out;
      pOP->PntGroup = 3;  // o
      pOP->NumbInGroup = 0;
      pOP->IO = '-';
      return 0;
      }
    ModMsg ( "У источника '%s' неправильный вход '%s'", ObjName.Str, pOP->Point.Str );
    }
  return 1;
  }

#include "SetMatr.h"

int CIO::SetMatr( void * pExternals, int PntGroup,  double dt, 
                 double A[/* kExkV */], double B[ /*kE*/ ] )   // A * X = B
  {
  SET_BP BreakPoint;
  switch ( PntGroup )
    {
    case IO:
      {
      if ( Закупорен )
        {
        MATR_OMEGA(1e-3)
        return 0;
        }
      if ( WorkAsSrc )
        {
        Matr_1_In( dt, A, B );
        Matr_1_Out( dt, A+6, B+1 );
        return 0;
        }
      if ( Reg_Flow_Z_kg.IsOn())
        {
        if ( pFlow_in )
          Mw_src = _Mw = ::Mw ( pFlow_in->Get_Comp());
        else
          Mw_src = 18.;
        double F = Reg_Flow_Z_kg / Mw_src;
        double * Eq = A;
        Eq[F_0] = 1.;
        B[0] = F;
        Eq += 4;
        Eq[F_1] = 1.;
        B[1] = F;
        return 0;
        }
      if ( Reg_Flow_Z_m3.IsOn())
        {
        double F = Reg_Flow_Z_m3 / M3;
        double * Eq = A;
        Eq[F_0] = 1.;
        B[0] = F;
        Eq += 4;
        Eq[F_1] = 1.;
        B[1] = F;
        return 0;
        }
      if ( Reg_dP_Z.IsOn() )
        {
        double * Eq = A;
        Eq[P_0] = 1.;
        Eq[P_1] = -1.;
        B[0] = Reg_dP_Z;
        Eq += 4;
        Eq[F_0] = -1.;
        Eq[F_1] = 1.;
        return 0;
        }
      double * Eq = A;
      Eq[P_0] = 1.;
      Eq[P_1] = -1.;
      Eq += 4;
      Eq[F_0] = -1.;
      Eq[F_1] = 1.;
      }
    return 0;
    case In: // in
      {
      Matr_1_In( dt, A, B );
      }
    return 0;
    case Out: // out
      {
      Matr_1_Out( dt, A, B );
      }
    return 0;
    default:
      ASS(0)
    break;
    }
  return 0;
  }

#include "SetVar.h"

int CIO::SetVar( void * pExternals, int PntGroup, double dt, 
                // Переменные на входах
                double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
  {
  SET_BP BreakPoint;
  switch ( PntGroup )
    {
    case IO:
      {
      pFlow_in = (CFlow*)pVar[0];
      pFlow_out = (CFlow*)pVar[1];
      //dPZ = pFlow_in->P - pFlow_out->P;
      }
    return 0;
    case In:
      {
      pFlow_in = (CFlow*)pVar[0];
      }
    return 0;
    case Out:
      {
      pFlow_out = (CFlow*)pVar[0];
      }
    return 0;
    default:
      ASS(0)
    break;
    }
  return 0;
  }
