#include "stdafx.h"
#include "HeatAntiFlow.h"
#include "Err.h"
#include "CommProc.h"
#include "Work.h"


#include "TestNodes.h"


NODE_LIST
NODE_IN  ( 1, 0, "i_tube","Подключение входа в трубное пространство")
NODE_OUT ( 1, 1, "o_tube","Подключение выхода из трубного пространства")
NODE_IN  ( 2, 0, "i_shell","Подключение входа в межтрубное пространство")
NODE_OUT ( 2, 1, "o_shell","Подключение выхода из межтрубного пространства")
END_LIST

STATIC int TestNodes( char * ObjName, int kNodes, struct CObjectPoint ** ppNodes )
  {
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

int CHeatAntiFlow::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
  {
  if ( strstr (ObjName, "C-3001S-E1S"))
    KKK();
  int Res = ::TestNodes( ObjName, kNodes, ppNodes );
  if ( Res )
    return Res;
  return 0;
  }

#include "SetMatr.h"

int CHeatAntiFlow::SetMatr( void * pExternals, int PntGroup,  double dt, 
                           double A[/* kExkV */], double B[ /*kE*/ ] )
  {
  SET_BP BreakPoint;
//	MATR_OMEGA(IO_Vol,OmegaVol)
  if ( PntGroup == 2 )
    {
    double Omega = OmegaVol * 1e4;
    double * Eq = A;
    Eq[P_0] = Omega;
		Eq[P_1] = -Omega;
		Eq[F_0] = -1.;
    B[0] = -Omega * dP_Hydro; 
//
    Eq += 4;
    Eq[F_0] = -1.;
    Eq[F_1] = 1.;
    Eq[P_0] = Def_Trub;
    B[1] = Def_Trub * P_Аневризма;
    return 0;
    }
//	MATR_OMEGA(IO_Trub,Omega)
  if ( PntGroup == 1 )
    {
    double Omega = OmegaTrub * 1e4;
    if ( Omega_Defect )
      Omega *= k_Omega;
    double * Eq = A;
    Eq[P_0] = Omega;
	  Eq[P_1] = -Omega;
		Eq[F_0] = -1.;
    B[0] = -Omega * dP_Circ; 
//
    Eq += 4;
    Eq[F_0] = -1.;
    Eq[F_1] = 1.;
    Eq[P_0] = Def_Trub;
    B[1] = Def_Trub * P_Аневризма;
    return 0;
    }
  ASS(0)
  return 0;
  }

#include "SetVar.h"

int CHeatAntiFlow::SetVar( void * pExternals, int PntGroup, double dt, 
                          double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
  {
  SET_BP BreakPoint;
  if ( PntGroup == 1 )
		{
    if ( Var[F_0] > 0. )
      {
      Trub_0 = In_Trub = (CFlow*)pVar[0];
      Trub_1 = Out_Trub = (CFlow*)pVar[1];
      }
    else
      {
      Trub_1 = In_Trub = (CFlow*)pVar[1];
      Trub_0 = Out_Trub = (CFlow*)pVar[0];
      }
    Перепад_труб = ( In_Trub->P - Out_Trub->P ) * kПерепад_труб;
    _F_Trub_kg = In_Trub->Flow_mol * In_Trub->To_kg;
    _dP_Trub = Var[P_0] - Var[P_1];
    if ( !_finite( In_Trub->h_Flow ))
      In_Trub->h_Flow = 0.;
    Heat.pIn_0 = In_Trub;
    Heat.pOut_0 = Out_Trub; 
    if ( Перемешивание && Переток_Trub < 0. )
      {
      FirstMix = true;
 // Поток идет в трубы
      if ( MixMax )
        {
        Add( fabs(Переток_Trub), h_Аневризма, Comp_Аневризма );
        if ( In_Trub->h_Flow > Out_Trub->h_Flow )
          Add( fabs(In_Trub->Flow_mol), In_Trub->h_Flow, In_Trub->Get_Comp());
        else
          Add( fabs(Out_Trub->Flow_mol), Out_Trub->h_Flow, Out_Trub->Get_Comp());
        }
      else
        {
        Add( fabs(Переток_Trub), h_Аневризма, Comp_Аневризма );
        if ( Var[F_0] > 0. )
          {
          CFlow * pF = (CFlow*)pVar[0];
          Add( pF->Flow_mol, pF->h_Flow, pF->Get_Comp());
          }
        if ( Var[F_1] < 0. )
          {
          CFlow * pF = (CFlow*)pVar[1];
          Add( -pF->Flow_mol, pF->h_Flow, pF->Get_Comp());
          }
        }
//////////////
      Norm( IsCompMix, CmolMix );
      Comp_Mix->Set( IsCompMix, CmolMix );
      h_Mix /= F_Mix;
      }
		return 0;
		}
  if ( PntGroup == 2 )
		{
    if ( Var[F_0] > 0. )
      {
      Vol_0 = In_Vol = (CFlow*)pVar[0];
      Vol_1 = Out_Vol = (CFlow*)pVar[1];
      }
    else
      {
      Vol_1 = In_Vol = (CFlow*)pVar[1];
      Vol_0 = Out_Vol = (CFlow*)pVar[0];
      }
    Перепад_объема = ( In_Vol->P - Out_Vol->P ) * kПерепад_объема;
    _F_Vol_kg = In_Vol->Flow_mol * In_Vol->To_kg;
    _dP_Vol = Var[P_0] - Var[P_1];
    Heat.pIn_1 = In_Vol;
    Heat.pOut_1 = Out_Vol;
    if ( Перемешивание && Переток_Vol < 0. )
      {
// Поток идет в объем
      FirstMix = true;
      if ( MixMax )
        {
        Add( fabs(Переток_Vol), h_Аневризма, Comp_Аневризма );
        if ( In_Vol->h_Flow > Out_Vol->h_Flow )
          Add( fabs(In_Vol->Flow_mol), In_Vol->h_Flow, In_Vol->Get_Comp());
        else
          Add( fabs(Out_Vol->Flow_mol), Out_Vol->h_Flow, Out_Vol->Get_Comp());
        }
      else
        {
        Add( fabs(Переток_Vol), h_Аневризма, Comp_Аневризма );
        if ( Var[F_0] > 0. )
          {
          CFlow * pF = (CFlow*)pVar[0];
          Add( pF->Flow_mol, pF->h_Flow, pF->Get_Comp());
          }
        if ( Var[F_1] < 0. )
          {
          CFlow * pF = (CFlow*)pVar[1];
          Add( -pF->Flow_mol, pF->h_Flow, pF->Get_Comp());
          }
        }
      Norm( IsCompMix, CmolMix );
      Comp_Mix->Set( IsCompMix, CmolMix );
      h_Mix /= F_Mix;
      }
		return 0;
		}
  return 0;
  }

void CHeatAntiFlow::Add( double Flow, double h_Flow, CComp * pComp )
  {
  if ( FirstMix )
    {
    CLEAR(IsCompMix)
    CLEAR(CmolMix)
    h_Mix = 0.;
    F_Mix = 0.;
    FirstMix = false;
    }
  F_Mix += Flow;
  h_Mix += Flow * h_Flow;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( pComp->IsComp[n] )
      {
      IsCompMix[n] = true;
      CmolMix[n] += pComp->Cmol[n] * Flow;
      }
    }
  }

