#include "stdafx.h"
#include "Ejector.h"
#include "Err.h"
#include "CommProc.h"
#include "Work.h"
#include "HydroStruct.h"
#include "TestNodes.h"

NODE_LIST

NODE_IN  ( 1, 0, "motive","Активный газ высокого давления" )
NODE_OUT ( 1, 1, "outlet","Выход эжектора")
NODE_IN (  1, 2, "inlet","Вход откачиваемого газа")

END_LIST

#undef IF
#define IF(Node) if ( lstrcmp ( Node, Name ) == 0 )
extern "C"
EXP int TestNodes( char * ObjName,  int kNodes, struct CObjectPoint ** ppNodes  )
  {
  if ( kNodes != 3 )
    {
    ModMsg ( "У эжектора '%s' неправильные входы", ObjName );
    return 1;
    }
  int kIO = 0;
  int kErr = 0;
  for ( int n = 0; n < kNodes; n++ )
    {
    CObjectPoint * pOP = ppNodes[n];
    pOP->Type = eГидравлика;
    char * Name = pOP->PntName;
    IF("motive")
      { kIO++; continue; };
    IF("outlet")
      { kIO++; continue; };
    IF("inlet")
      { kIO++; continue; };
    ModMsg ( "У эжектора '%s' ошибочный вход '%s'", ObjName, pOP->PntName ), kErr++;
    }
  if ( kIO != 3 )
    ModMsg ( "У эжектора '%s' неправильно заданы входы/выходы", ObjName ), kErr++;
  return kErr;
  }

int CEjector::TestNodes(  int kNodes, struct CObjectPoint ** ppNodes )
  {
  if ( kNodes != 3 )
    {
    ModMsg ( "У эжектора '%s' неправильные входы", ObjName );
    return 1;
    }
  int kN = 0;
  for ( int n = 0; n < 3; n++ )
    {
    CObjectPoint * pOP = ppNodes[n];
    pOP->Type = eГидравлика;
    if ( pOP->PntName == "motive" )
      {
      pOP->PntGroup = 1;  
      pOP->NumbInGroup = 0;
      pOP->IO = '+';
      pOP->Type = eГидравлика;
      kN++;
      continue;
      }
    if ( pOP->PntName == "outlet" )
      {
      pOP->PntGroup = 1;  
      pOP->NumbInGroup = 1;
      pOP->IO = '-';
      pOP->Type = eГидравлика;
      kN++;
      continue;
      }
    if ( pOP->PntName == "inlet" )
      {
      pOP->PntGroup = 1;  
      pOP->NumbInGroup = 2;
      pOP->IO = '+';
      pOP->Type = eГидравлика;
      kN++;
      continue;
      }
    }
  if ( kN == 3 )
    return 0;
  ModMsg ( "У эжектора '%s' неправильные входы", ObjName );
  return -1;
  }

#include "SetMatr.h"

int CEjector::SetMatr( void * pExternals, int PntGroup,  double dt, 
                      double A[/* kExkV */], double B[ /*kE*/ ] )
  {
  SET_BP BreakPoint;
//  Row_3 * MP = (Row_3*)Mp;
//  Row_3 * MF = (Row_3*)Mf;
  double * Eq = A;
  double O = Omega;
  if ( Defect < 0 )
    O = 1e-5;
  if ( F_in <= 0.1 )
    {
    Eq[P_0] = O;
    Eq[P_1] = -O;
    Eq[F_0] = -1.;
    Eq += 6;
    //
    Eq[F_2] = 1.;
    //
    Eq += 6;
    Eq[F_0] = 1.;
    Eq[F_1] = -1.;
    }
  else
    {
    Eq[P_0] = O;
    Eq[P_1] = -O;
    Eq[F_0] = -1.;
    if ( Defect > 0 )
      {
      Eq[P_0] += O_Razgerm;
      B[0] += O_Razgerm;
      }
    Eq += 6;
    //
    double O = OmegaPodsos;
    if ( P_podsos < P_cam )
      O = 1e-8;
    Eq[P_2] = O;
    Eq[F_2] = -1.;
    if ( !_finite ( P_cam ))
      P_cam = 0.;
    B[1] = O * P_cam;
    //
    Eq += 6;
    Eq[F_0] = 1.;
    Eq[F_1] = -1.;
    Eq[F_2] = 1.;
    }
  return 0;
  }

#include "SetVar.h"

int CEjector::SetVar( void * pExternals, int PntGroup, double dt, 
                     double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
  {
  SET_BP BreakPoint;
  p_In = (CFlow*)pVar[0];
  p_Out = (CFlow*)pVar[1];
  p_Podsos = (CFlow*)pVar[2];
  _P_in = Var[P_0] - 1.;
  _P_out = Var[P_1] - 1.;
  P_podsos = _P_podsos = Var[P_2];
  if ( F_in <= 0.1 )
    P_cam = P_podsos;
  //_P_in_mm = _P_in * 760.;
  //_P_out_mm = _P_out * 760.;
  //_P_podsos_mm = _P_podsos * 760.;
  _Flow_main = p_In->Flow_mol * p_In -> To_kg;
  _Flow_podsos = p_Podsos->Flow_mol * p_Podsos -> To_kg;
  return 0;
  }

