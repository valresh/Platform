#include "stdafx.h"
#define DLL_COLUMN
#include "Column.h"
#include "Err.h"
#include "CommProc.h"
#include "Work.h"
#include "HydroStruct.h"
#include "SetMatr.h"
#include "SetVar.h"
#include "Work.h"
#include "TestNodes.h"
#include "HydroStruct.h"

#define GRP_OUT_GAS 1
#define GRP_HEAT 2
#define GRP_STAGE 10

NODE_LIST
NODE_IN ( 1, 0, "i_#","Вход(#-номер тарелки сверху)")
NODE_OUT( 1, 0, "o_#","Выход(#-номер тарелки сверху)")
END_LIST
////
//NODE_IN(1,"вход_теплообмен","Вход змеевика теплообмена")
//NODE_OUT(1,"выход_теплообмен","Выход змеевика теплообмена")
//#define PipeName pOP->pPool->Name                                        

int CColumn::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
  {
  if ( kNodes <= 0 )
    {
    ModMsg ( "У колонны '%s' не заданы входы", ObjName.Str );
    return 1;
    }
  pNodes = new CNode[kNodes];
  kIO = kNodes;
  bool Err = false;
  kStage = 0;
	n1St = -1;
  OmegaHeatStage = NULL;
  kHeatStage = 0;
  for ( int n = 0; n < kNodes; n++ )
    {
    CObjectPoint * pOP = ppNodes[n];
    pNodes[n].Numb = n;
    pOP->IntNumb = 0;
    pOP->GrpPoint = n + GRP_STAGE;
    pNodes[n].Type = ' ';
    pNodes[n].OutLiqCub = false;
    pNodes[n].OutGasTop = false;
    pNodes[n].SkipTestType = false;
    pNodes[n].pOP = pOP;
    char * Name = pOP->Point;
    pNodes[n].Name = Name;
    if ( strncmp ( Name, "вход_нагрев", 11 ) == 0 && Name[11] == '_' )
      {
      int N = atoi ( Name + 12 );
      if ( N <= 0 || N > MAX_HEAT )
        {
        ModMsg ( "У колонны '%s' неправильный вход '%s'", ObjName.Str, Name );
        Err = true;
        continue;
        }
      if ( N > kHeatStage )
        kHeatStage = N;
      pNodes[n].Numb = n;
      pOP->IntNumb = 0;
      pOP->GrpPoint = GRP_HEAT + N;
      pOP->TypeF = '+';
      pNodes[n].Type = 'H';
      pNodes[n].nStage = 0;
      pNodes[n].Name = Name;
      continue;
      }
    if ( strncmp ( Name, "выход_нагрев", 12 ) == 0 && Name[12] == '_' )
      {
      int N = atoi ( Name + 13 );
      if ( N <= 0 || N > MAX_HEAT )
        {
        ModMsg ( "У колонны '%s' неправильный вход '%s'", ObjName.Str, Name );
        Err = true;
        continue;
        }
      if ( N > kHeatStage )
        kHeatStage = N;
      pNodes[n].Numb = n;
      pOP->IntNumb = 1;
      pOP->GrpPoint = GRP_HEAT + N;
      pOP->TypeF = '-';
      pNodes[n].Type = 'H';
      pNodes[n].nStage = 0;
      continue;
      }
    if ( lstrcmp ( Name, "вход_теплообмен" ) == 0 )
      {
      pNodes[n].Numb = n;
      pOP->IntNumb = 0;
      pOP->GrpPoint = GRP_HEAT;
      pOP->TypeF = '+';
      pNodes[n].Type = 'H';
      pNodes[n].nStage = 0;
      continue;
      }
    if ( lstrcmp ( Name, "выход_теплообмен" ) == 0 )
      {
      pNodes[n].Numb = n;
      pOP->IntNumb = 1;
      pOP->GrpPoint = GRP_HEAT;
      pOP->TypeF = '-';
      pNodes[n].Type = 'H';
      pNodes[n].nStage = 0;
      continue;
      }
    if ( Name[0] == 'i' )
      {
      pNodes[n].Type = 'G';
      pOP->TypeF = '+';
      }
    if ( Name[0] == 'o' )
      {
      pNodes[n].Type = 'L';
      pOP->TypeF = '+';
      }
    if ( Name[0] == 'g' )
      {
      pNodes[n].Type = 'G';
      pOP->TypeF = '+';
      }
    if ( Name[0] == 'x' )
      {
      pNodes[n].Type = 'G';
      }
    if ( pNodes[n].Type == ' ' )
      {
      ModMsg ( "У колонны '%s' неправильный тип входа '%s'", ObjName.Str, Name );
      Err = true;
      }
    char * Stage = Name;
    while( *Stage )
      {
      if ( *Stage > '9' || *Stage < '0' )
        Stage++;
      else
        break;
      }
    int nStage = atoi ( Stage );
    pNodes[n].nStage = nStage;
    if ( nStage <= 0 || nStage > 256 )
      {
      ModMsg ( "У колонны '%s' неправильный номер входа '%s'", ObjName.Str, Name );
      Err = true;
      continue;
      }
    if ( kStage < nStage )
      kStage = nStage;
    if ( nStage == 1 && ( Name[0] == 'o' || strcmp ( Name, "g_1" ) == 0 ))
      {
      if ( n1St >= 0 )
        {
        ModMsg ( "У колонны '%s' несколько выходов с первой тарелки", ObjName.Str );
        Err = true;
        }
      n1St = n;
      }
    if ( Name[0] == 'x' )
      {
      GasColumn = true;
      Column.GasColumn = true;
      pNodes[n].Type = 'G';
      pOP->GrpPoint = GRP_OUT_GAS;
      if ( nStage == 1 )
        {
        nGasNode_1 = n;
        pOP->IntNumb = 1;
        pOP->TypeF = '+';
        }
      else
        {
        nGasNode_x = n;
        pOP->IntNumb = 0;
        pOP->TypeF = '+';
        }
      }
    }
  if ( Err )
    return 2;
  if ( kHeatStage > 0 )
    {
    OmegaHeatStage = NewArr(double,kHeatStage);
    KSHeatStage = NewArr(double,kHeatStage);
    for ( int n = 0; n < kHeatStage; n++ )
      {
      OmegaHeatStage[n] = 10000.;
      KSHeatStage[n] = 1000000.;
      }
    }
  for ( int n = 0; n < kNodes; n++ )
    {
    if ( pNodes[n].nStage == 1 && ( pNodes[n].Name[0] == 'o' || strcmp ( pNodes[n].Name, "g_1" ) == 0 || strcmp ( pNodes[n].Name, "x_1" ) == 0 ))
      {
      pNodes[n].OutGasTop = true;
      }
    if ( pNodes[n].nStage == kStage && pNodes[n].Name[0] == 'o' )
      {
      pNodes[n].OutLiqCub = true;
      }
    }
  return 0;
  }

#include "SetMatr.h"

int CColumn::SetMatr( void * pExternals, int PntGroup,  double dt, 
    double A[/* kExkV */], double B[ /*kE*/ ] )
  {
  SET_BP BreakPoint;
  if ( PntGroup == GRP_OUT_GAS )
    {
// Проход газа
    double * Eq = A;
	  Eq[P_0] = Omega_Gas;
	  Eq[P_1] = -Omega_Gas;
	  Eq[F_1] = 1.;
    Eq += 4;
    Eq[F_0] = kGasOut;
    Eq[F_1] = 1.;
    return 0;
    }
  if ( PntGroup == GRP_HEAT )
    {
// Проход тепла
    MATR_OMEGA(Omega_Heat)
    return 0;
    }
  if ( PntGroup < GRP_STAGE )
    {
// Проход тепла
    int N = PntGroup - GRP_HEAT - 1;
    MATR_OMEGA(OmegaHeatStage[N])
    return 0;
    }
  int n = PntGroup - GRP_STAGE;
  CNode & Node = pNodes[n];
  if ( n == n1St )
    {
    if ( Column.Разгерметизация_Верха )
      {
      A[P_0] = Column.Omega_Def;
      A[F_0] = 1.; // выход 
      B[0] = Column.Omega_Def;
      return 0;
      }
    if ( Column.P_Top_Z > 0.) 
      {
      double O = Omega[n];
      A[P_0] = O;
      A[F_0] = -1.; // вход
      B[0] = O * Column.P_Top_Z;
      return 0;
      }
//    if ( Column.Vol.Work )
//      {
//      Mf[0] = -1.; // вход
//      V[0] = Column.Flow_1;
//      return 0;
//      }
    double O = Omega[n];
    if ( OK_IO && Flow_kg[n] < 0. )
      O = 1e-5;
    A[P_0] = O;
    A[F_0] = -1.; // вход
    double P = Press[n] + Hydro_dP[n];
    if ( pNodes[n].OutLiqCub && Use_Lev_hydro )
      P += dP_hydro;
    B[0] = O * P;
    return 0;
    }
  if ( Hydro_dPmax[n] <= 0. )
    Hydro_dP[n] = Hydro_dPmax[n];
  double O = Omega[n];
  if ( OK_IO && Flow_kg[n] < 0. )
    O = 1e-5;
  A[P_0] = O;
  A[F_0] = -1.; // вход
  double P = Press[n] + Hydro_dP[n];
  if ( pNodes[n].OutLiqCub && Use_Lev_hydro )
    P += dP_hydro;
  B[0] = O * P;
  return 0;
  }

#include "SetVar.h"

  int CColumn::SetVar( void * pExternals, int PntGroup, double dt, 
    double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
  {
  SET_BP BreakPoint;
  if ( PntGroup == GRP_OUT_GAS )
    {
// Проход газа
    Column.P_top = Press[0];
    pFlow_Gas_in = (CFlow*)pVar[0];
    pFlow_Gas_out = (CFlow*)pVar[1];
    Src[nGasNode_x]->Set( pFlow_Gas_in );
    Src[nGasNode_1]->Set( pFlow_Gas_out );
    return 0;
    }
  if ( PntGroup == GRP_HEAT )
    {
// Проход охлаждения/нагрева
    pFlow_Heat_in = (CFlow*)pVar[0];
    pFlow_Heat_out = (CFlow*)pVar[1];
    *pFlow_Heat_out = *pFlow_Heat_in;
    return 0;
    }
  if ( PntGroup < GRP_STAGE )
    {
    // Проход тепла
    int N = PntGroup - GRP_HEAT - 1;
    pFlow_ExtHeat_in[N] = (CFlow*)pVar[0];
    pFlow_ExtHeat_out[N] = (CFlow*)pVar[1];
    *(pFlow_ExtHeat_out[N]) = *(pFlow_ExtHeat_in[N]);
    ASS(Column.pHeat_In[N])
    ASS(Column.pHeat_In[N]->SetIn( pFlow_ExtHeat_in[N]));
    ASS(Column.pHeat_Out[N])
    ASS(Column.pHeat_Out[N]->SetOut(pFlow_ExtHeat_out[N]));
    return 0;
    }
  int n = PntGroup - GRP_STAGE;
//  CNode & Node = pNodes[n];
  Src[n]->Set( (CFlow*)pVar[0] );
  Flow_kg[n] = Src[n]->pFlow->Flow_kg();
  if ( n == n1St )
    {
    if ( Column.P_Top_Z == 0.) 
      {
      //Column.P_top = Press[0] - Flow[0] / Column.Omega_Top;
      //if ( Column.P_top > Column.P_max )
      //  Column.P_top = Column.P_max;
      }
    Column.Flow_out = Var[F_0];
    Column.pOut_Gas = (CFlow*)pVar[0];
    return 0;
    }
  return 0;
  }

