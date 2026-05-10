#include "stdafx.h"
#include "HeatChange.h"
#include "Err.h"
#include "CommProc.h"
#include "HydroStruct.h"
#include "SetMatr.h"
#include "SetVar.h"
#include "Work.h"
#include "TestNodes.h"


NODE_LIST
NODE_IN  ( 1, 0, "i_tube","Подключение входа в трубное пространство")
NODE_OUT ( 1, 1, "o_tube","Подключение выхода из трубного пространства")
NODE_IN  ( 2, 0, "i_shell","Подключение входа в межтрубное пространство")
NODE_OUT ( 2, 1, "o_shell","Подключение выхода из межтрубного пространства")
END_LIST

//STD_TEST

STATIC int TestNodes( char * ObjName, int kNodes, struct CObjectPoint ** ppNodes )
  {
  if ( strstr (ObjName, "3009"))
    KKK();
  int kIO = 0;
  int kErr = 0;
  if ( kNodes > kVariablesData )
    {
    ModMsg ( "У '%s' число переменных на входе велико ( %d > %d )", ObjName, kNodes, kVariablesData );
    return 1;
    }
  for ( int v = 0; v < kVariablesData; v++ )
    VariablesData[v].WasSet = false;
  for ( int n = 0; n < kNodes; n++ )
    {
    CObjectPoint * pObjVar = ppNodes[n];
    char * Point = pObjVar->PntName;
    bool OK = false;
    for ( int v = 0; v < kVariablesData; v++ )
      {
      if ( lstrcmpi_m ( VariablesData[v].SetVarName, Point ) == 0 )
        {
        OK = true;
        if ( VariablesData[v].WasSet )
          {
          ModMsg ( "У  '%s' переменная '%s' задается повторно", ObjName, Point ), kErr++;
          }
        else
          {
          VariablesData[v].WasSet = true;
          pObjVar->Type = VariablesData[v].TypeVarStruct;
          pObjVar->PntGroup = VariablesData[v].GroupInModel;
          pObjVar->NumbInGroup = VariablesData[v].NumbInGroup;
          pObjVar->IO = VariablesData[v].IO;
          }
          break;
        }
      }
      if ( !OK )
        ModMsg ( "У  '%s' ошибочный вход '%s'", ObjName, Point ), kErr++;
    }
  return kErr;
  }


int CHeatChange::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
  {
  if ( strstr (ObjName, "3009"))
    KKK();
  int Res = ::TestNodes( ObjName, kNodes, ppNodes );
  if ( Res )
    return Res;
  return 0;
  }

#include "SetMatr.h"

int CHeatChange::SetMatr( void * pExternals, int PntGroup,  double dt, 
                         double A[/* kExkV */], double B[ /*kE*/ ] )
  {
  switch ( PntGroup )
    {
    case 1:
      {
      SET_BP BreakPoint;
      double Omega = OmegaTrub * 1e4;
      if ( Omega_Defect )
        Omega *= k_Omega;
      //	MATR_OMEGA(IO_Trub,Omega)
      double * Eq = A;
      Eq[P_0] = Omega;
      Eq[P_1] = -Omega;
      Eq[F_0] = -Xi_Trub;
      //
      Eq += 4;
      Eq[F_0] = k_Trub;
      Eq[F_1] = -1.;
      }
    return 0;  
    case 2:
      {
      SET_BP BreakPoint;
      double O_Vol = OmegaVol * 1e4;
      //if ( Def_Zasor )
      //  O_Vol = def_Omega_Zasor;
      double * Eq = A;
      Eq[P_0] = O_Vol;
      Eq[P_1] = -O_Vol;
      Eq[F_0] = -Xi_Vol;;
      B[0] = -O_Vol * dP_Hydro;
      //
      Eq += 4;
      Eq[F_0] = k_Vol;
      Eq[F_1] = -1.;
      }
    return 0;
    default:
      ASS(0)
    break;
    }
  return 0;
  }

#include "SetVar.h"

int CHeatChange::SetVar( void * pExternals, int PntGroup, double dt, 
                        double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
  {
  switch ( PntGroup )
    {
    case 1:
      {
      SET_BP BreakPoint;
      if ( Var[1] >= 0. )
        {
        In_Trub_0 = (CFlow*)pVar[0];
        Out_Trub = (CFlow*)pVar[1];
        }
      else
        {
        In_Trub_0 = (CFlow*)pVar[1];
        Out_Trub = (CFlow*)pVar[0];
        }
      P_out_trub = Out_Trub->P - 1.;
      dP_trub = In_Trub_0->P - Out_Trub->P;
//      F_Trub_old = 0.9 * F_Trub_old + 0.1 * In_Trub_0->Flow_mol;
      _F_Trub_kg = In_Trub_0->Flow_mol * In_Trub_0->To_kg;
      _dP_Trub = Var[0] - Var[2];
      if ( Reg_F_trub_Z.Use )
        {
        double F_trub_Z = Reg_F_trub_Z;
        double F_trub	= fabs ( _F_Trub_kg );
        double K;
        if ( F_trub > 2. * F_trub_Z )
          K = 0.5;
        else
          if ( F_trub < 0.5 * F_trub_Z )
            K = 2.;
          else
            K = F_trub_Z / F_trub;
        OmegaTrub *= LimdX ( K - 1., Reg ) + 1.;
        if ( OmegaTrub > 100. )
          OmegaTrub = 100.;
        if ( OmegaTrub < 1e-10 )
          OmegaTrub = 1e-10;
        }
      if ( Reg_dP_trub_Z.Use )
        {
        double dP_trub_Z = Reg_dP_trub_Z;
        double dP_trub	= fabs ( _dP_Trub );
        double K;
        if ( dP_trub > 2. * dP_trub_Z )
          K = 2.;
        else
          if ( dP_trub < 0.5 * dP_trub_Z )
            K = 0.5;
          else
            K = dP_trub / dP_trub_Z;
        OmegaTrub *= LimdX ( K - 1., Reg ) + 1.;
        if ( OmegaTrub > 100. )
          OmegaTrub = 100.;
        if ( OmegaTrub < 1e-10 )
          OmegaTrub = 1e-10;
        }
      }
      return 0;
    case 2:
      {
      SET_BP BreakPoint;
      if ( Var[1] >= 0. )
        {
        In_Vol_0 = (CFlow*)pVar[0];
        Out_Vol = (CFlow*)pVar[1];
        }
      else
        {
        In_Vol_0 = (CFlow*)pVar[1];
        Out_Vol = (CFlow*)pVar[0];
        }
      P_out_vol = Out_Vol->P - 1.;
      dP_vol = In_Vol_0->P - Out_Vol->P;
//      F_Vol_old = 0.9 * F_Vol_old + 0.1 * In_Vol_0->Flow_mol;
      _F_Vol_kg = In_Vol_0->Flow_mol * In_Vol_0->To_kg;
      _dP_Vol = Var[0] - Var[2];
      if ( Reg_F_vol_Z.Use )
        {
        double F_vol_Z = Reg_F_vol_Z;
        double F_vol	= fabs ( _F_Vol_kg );
        double K;
        if ( F_vol < 2. * F_vol_Z )
          K = 2.;
        else
          if ( F_vol > 0.5 * F_vol_Z )
            K = 0.5;
          else
            K = F_vol_Z / F_vol;
        OmegaVol *= LimdX ( K - 1., Reg ) + 1.;
        if ( OmegaVol > 100. )
          OmegaVol = 100.;
        if ( OmegaVol < 1e-10 )
          OmegaVol = 1e-10;
        }
      if ( Reg_dP_vol_Z.Use )
        {
        double dP_vol_Z = Reg_dP_vol_Z;
        double dP_vol	= fabs ( _dP_Vol );
        double K;
        if ( dP_vol > 2. * dP_vol_Z )
          K = 2.;
        else
          if ( dP_vol < 0.5 * dP_vol_Z )
            K = 0.5;
          else
            K = dP_vol / dP_vol_Z;
        OmegaVol *= LimdX ( K - 1., Reg ) + 1.;
        if ( OmegaVol > 100. )
          OmegaVol = 100.;
        if ( OmegaVol < 1e-10 )
          OmegaVol = 1e-10;
        }
      }
      return 0;
    default:
      ASS(0)
        break;
    }
  return 0;
  }
