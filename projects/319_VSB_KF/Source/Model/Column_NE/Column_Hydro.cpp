#include "stdafx.h"
#define DLL_COLUMN
#include "Column_NE.h"
#include "Err.h"
#include "CommProc.h"
#include "HydroStruct.h"
#include "SetMatr.h"
#include "SetVar.h"
#include "Work.h"
#include "TestNodes.h"

#define GRP_OUT_GAS 1001
#define GRP_HEAT    2001
#define GRP_STAGE     1

NODE_LIST
NODE_IN ( 1, 0, "i_#","Вход(#-номер тарелки сверху)")
NODE_OUT( 1, 0, "o_#","Выход(#-номер тарелки сверху)")
END_LIST
////
//NODE_IN(1,"вход_теплообмен","Вход змеевика теплообмена")
//NODE_OUT(1,"выход_теплообмен","Выход змеевика теплообмена")
//#define PipeName pOP->pPool->Name  

int CColumn_NE::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
  {
  if ( kNodes <= 0 )
    {
    ModMsg ( "У колонны '%s' не заданы входы", ObjName );
    return 1;
    }
  pNodes = new CNode[kNodes];
  kIO = kNodes;
  bool Err = false;
  kStage = 0;
  n1St_out = -1;
  nCub_out = -1;
  OmegaHeatStage = NULL;
  kHeatStage = 0;
  for ( int n = 0; n < kNodes; n++ )
    {
    CObjectPoint * pOP = ppNodes[n];
    pOP->Type = eГидравлика;
    pNodes[n].Numb = n;
//    pNodes[n].PipeName = pOP->pPipe->ObjName;
    pOP->NumbInGroup = 0;
    pOP->PntGroup = n + GRP_STAGE;
    pNodes[n].Type = ' ';
    pNodes[n].OutLiqCub = false;
    pNodes[n].SkipTestType = false;
    char * Name = pOP->PntName;
    pNodes[n].Name = Name;
    pNodes[n].pOP = pOP;
    pOP->IO = '+';
///////////////////////
    if ( strncmp ( Name, "вход_нагрев", 11 ) == 0 && Name[11] == '_' )
      {
      int N = atoi ( Name + 12 );
      if ( N <= 0 || N > MAX_HEAT )
        {
        ModMsg ( "У колонны '%s' неправильный вход '%s'", ObjName, Name );
        Err = true;
        continue;
        }
      if ( N > kHeatStage )
        kHeatStage = N;
      pNodes[n].Numb = n;
      pOP->NumbInGroup = 0;
      pOP->PntGroup = GRP_HEAT + N;
      pOP->IO = '+';
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
        ModMsg ( "У колонны '%s' неправильный вход '%s'", ObjName, Name );
        Err = true;
        continue;
        }
      if ( N > kHeatStage )
        kHeatStage = N;
      pNodes[n].Numb = n;
      pOP->NumbInGroup = 1;
      pOP->PntGroup = GRP_HEAT + N;
      pOP->IO = '-';
      pNodes[n].Type = 'H';
      pNodes[n].nStage = 0;
      continue;
      }
    if ( lstrcmp ( Name, "вход_теплообмен" ) == 0 )
      {
      pNodes[n].Numb = n;
      pOP->NumbInGroup = 0;
      pOP->PntGroup = GRP_HEAT;
      pOP->IO = '+';
      pNodes[n].Type = 'H';
      pNodes[n].nStage = 0;
      continue;
      }
    if ( lstrcmp ( Name, "выход_теплообмен" ) == 0 )
      {
      pNodes[n].Numb = n;
      pOP->NumbInGroup = 1;
      pOP->PntGroup = GRP_HEAT;
      pOP->IO = '-';
      pNodes[n].Type = 'H';
      pNodes[n].nStage = 0;
      continue;
      }
///////////////////////
    if ( Name[0] == 'i' )
      {
      pNodes[n].Type = 'X';
      pOP->IO = '+';
      }
    if ( Name[0] == 'o' )
      {
      pNodes[n].Type = 'L';
      pOP->IO = '+';
      }
///////////////////////////////
    if ( Name[0] == 'g' )
      {
      pNodes[n].Type = 'G';
      pOP->IO = '+';
      }
///////////////////////////////
    if ( Name[0] == 'x' )
      {
      pNodes[n].Type = 'X';
      pOP->IO = '+';
      }
    if ( pNodes[n].Type == ' ' )
      {
      ModMsg ( "У колонны '%s' неправильный тип входа '%s'", ObjName, Name );
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
      ModMsg ( "У колонны '%s' неправильный номер входа '%s'", ObjName, Name );
      Err = true;
      continue;
      }
    if ( kStage < nStage )
      {
      kStage = nStage;
      }
    if ( nStage == kStage && Name[0] == 'o' )
      nCub_out = n;
///////////////////
    if ( nStage == 1 && ( Name[0] == 'o' || Name[0] == 'g' ))
      {
      if ( n1St_out >= 0 )
        {
        ModMsg ( "У колонны '%s' несколько выходов с первой тарелки", ObjName );
        Err = true;
        }
      n1St_out = n;
      pOP->NumbInGroup = 0;
      pOP->IO = '+';
      pNodes[n].Type = 'G';      // Matrix
      continue;
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
    if ( pNodes[n].nStage == kStage && pNodes[n].Name[0] == 'o' )
      {
      pNodes[n].OutLiqCub = true;
      }
    }
  return 0;
  }

#include "SetMatr.h"

int CColumn_NE::SetMatr( void * pExternals, int PntGroup,  double dt, 
    double A[/* kExkV */], double B[ /*kE*/ ] )
  {
  SET_BP BreakPoint;
  if ( PntGroup >= GRP_HEAT &&  PntGroup < GRP_HEAT + MAX_HEAT)
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
  int nSt = Node.nStage;
  if ( nSt == 1 || nSt == kStage )
    KKK();
//  if ( n == nCub_out || n == n1St_out )
//    KKK();
 //if ( n == n1St_out && Static_Calc_Mix && Static_Fix_P )
 //   {
 //   A[F_0] = -1.;
 //   B[0] = Column.pStage[nSt]->Vout;
 //   FINITE(B[0])
 //   return 0;
 //   }
  //    return 0;
  //  }
  double O = Src[n]->Omega;
  if ( OK_IO && Flow_kg[n] < 0. )
    {
    if ( Node.Name[0] == 'o' && Flow_kg[n] > 0. )
      O = 1e-5;
    if ( Node.Name[0] == 'i' && Flow_kg[n] < 0. )
      O = 1e-5;
    }
  double P = *(Src[n]->pPress );
//  if ( strstr ( ObjName, "К-6" ))
//    KKK();
  if ( n == n1St_out )
    {
    A[P_0] = O;
    A[F_0] = -1.; // вход
// 
    if ( P_Top_Z != 0. )
      P = P_Top_Z + 1.;
    B[0] = O * P;
    return 0;
    }
//
  if ( n == nCub_out )
    {
    //if ( LevCtrl_Cub )
    //  {
    //  CStage_NE * pCub = Column.pStage[kStage];
    //  double Delta = pCub->SummFlow();
    //  A[F_0] = 1.;
    //  B[0] = Flow_Cub_out - 0.5 * Delta;//Src[n]->pFlow->Flow_mol - Delta; 
    //  return 0;
    //  }
    A[P_0] = O;
    A[F_0] = -1.; // вход
    B[0] = O * P;
    return 0;
    }

  if ( Src[n]->Gas )
    {
    if ( Src[n]->pFlow )
      {
      double Max = Src[n]->Max_Gas_Flow * 0.95;
      double Pz = P - Max / O;
      if ( Src[n]->pFlow->P > Pz )
        {
        A[P_0] = O;
        A[F_0] = -1.; // вход
        B[0] = O * P;
        return 0;
        }
      double Oz = 10000.;
      A[P_0] = 1.;
      A[F_0] = -Oz; 
      B[0] = Pz + Oz * Max;
      return 0;
      }
#if 0
  if ( Src[n]->Gas )
    {
    if ( Src[n]->pFlow )
      {
      double Flow_gas = -Src[n]->pFlow->Flow_mol;
      double Max = Src[n]->Max_Gas_Flow;
      if ( Flow_gas < Max * 0.90 )
        {
        A[P_0] = O;
        A[F_0] = -1.; // вход
        B[0] = O * P;
        return 0;
        }
      Flow_gas = 0.9 * Flow_gas + 0.1 * Max * 0.99;
      A[F_0] = 1.; // вход
      B[0] = -Flow_gas;
      return 0;
      }

    if ( Src[n]->pFlow && Src[n]->pFlow->Flow_mol < -Src[n]->Max_Gas_Flow * 0.95 && Src[n]->pFlow->P < P * 1.001 )
      {

/*
      double Fz = -Src[n]->Max_Gas_Flow * 0.95;
      double Pz = Fz / O + P;
      double Oz = 0.1 * O;
      A[P_0] = 1.;
      A[F_0] = -Oz; // вход
      B[0] = Pz - Oz * Fz;
      return 0;
*/
      double Fz = 0.9 * Src[n]->pFlow->Flow_mol - 0.1 * Src[n]->Max_Gas_Flow * 0.95;
      A[F_0] = 1.; // вход
      B[0] = Fz;
      return 0;
      }
#endif
    }
  A[P_0] = O;
  A[F_0] = -1.; // вход
  B[0] = O * P;
  FINITE(B[0])
  return 0;

  //if ( pLevCtrl && pLevCtrl->TypeReg == 2 && n == nRegCtrl )
  //  {
  //  A[F_0] = 1.; 
  //  B[0] = -pLevCtrl->FlowGasOut;
  //  FINITE(B[0])
  //    return 0;
  //  }
    //CKarman * pKarman = Src[n]->pKarman;
    //if ( pKarman )
    //  {
    //  if ( Static_Fix_L )
    //    {
    //    double P_max = *(Src[n]->pPress ) - pKarman->F_Karman_Out_Z / O_max;
    //    if ( pKarman->P_out > P_max || fabs ( pKarman->P_out - 0.011111 ) < 1e-8 )
    //      {
    //      double O = O_max * 100.;
    //      A[P_0] = O;
    //      A[F_0] = -1.; // вход
    //      B[0] = pKarman->F_Karman_Out_Z + O * P_max;
    //      double Ft = O * ( pKarman->P_out - P_max );
    //      FINITE(B[0])
    //      }
    //    else
    //      {
    //      A[F_0] = -1.; // вход
    //      B[0] = pKarman->F_Karman_Out_Z;
    //      FINITE(B[0])
    //      }
    //    return 0;
    //    }
    //  }
////////////////////////////////////////////////////////
  return 0;
  }

#include "SetVar.h"

int CColumn_NE::SetVar( void * pExternals, int PntGroup, double dt, 
                       double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
  {
  SET_BP BreakPoint;
  if ( PntGroup >= GRP_HEAT &&  PntGroup < GRP_HEAT + MAX_HEAT)
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
  CFlow * pF = (CFlow*)pVar[0];
  Src[n]->Set( pF );
  Flow_kg[n] = pF->Flow_kg();
  if ( n == n1St_out )
    {
    FlowGasOut = -((CFlow*)pVar[0])->Flow_mol;
    PressOut = Var[P_0];
    }
  //  double Pfact = Var[P_0];
  //  double F = pF->Flow_mol;
  //  double O = Src[n]->Omega;
  //  double Pflow = Column.pStage[1]->P + F / O;
  //  Static_dP_Fix_P = Pfact - Pflow;
  //  return 0;
  //  }
  if ( n == nCub_out )
    Flow_Cub_out = Var[F_0]; 
 // if ( n == nCub_out && LevCtrl_Cub )
 //   {
 //// O * P - F = O * H
 //   double O = Src[n]->Omega;
 //   if ( OK_IO && Flow_kg[n] < 0. )
 //     O = 1e-5;
 //   double H = Press[n];// + Hydro_dP[n];
 //   double Flow0 = O * ( Var[P_0] - H );
 //   dFlow_Lev_Cub = Flow_Cub_out - Flow0;
 //   return 0;
 //   }
  CKarman * pKarman = Src[n]->pKarman;
  if ( pKarman )
    {
    double P_fact = Var[P_0];
    pKarman->P_out = P_fact;
    double F = Var[F_0];
    pKarman->F_Karman_Out = F;
//    double P_flow = *(Src[n]->pPress ) + F / Src[n]->Omega;
//    pKarman->dP_Out = P_fact - P_flow;
    }
  return 0;
  }

