#include "stdafx.h"
#include "Furnace.h"
#include "Err.h"
#include "CommProc.h"
#include "Work.h"
#include "HydroStruct.h"
#include "TestNodes.h"

NODE_LIST
NODE_IN(1, 0, "in_product_#", "Подача продукта" )
NODE_IN(1, 1, "out_product_#", "Вывод продукта" )
NODE_IN(1, 0, "in_topgas", "Подача топливного газа" )
NODE_IN(2, 0, "in_pilot", "Подача пилотног газа" )
NODE_IN(3, 0, "in_air", "Подача воздуха" )
NODE_IN(4, 0, "in_mazut", "Подача мазута" )
NODE_IN(5, 0, "in_steam", "Подача пара на распыл мазута" )
NODE_IN(6, 0, "in_par_topka", "Подача пара в топку" )
NODE_IN(7, 0, "in_add", "Дополнительный вход горючих материалов" )
NODE_OUT(8, 0, "out_smoke", "Выход дыма" )
END_LIST


int kIn_Product;
int kOut_Product;
int k_Gas;
int k_Pilot;
int k_Air;
int k_Mazut;
int k_Steam;

extern "C"
EXP int TestNodes( char * ObjName, int kNodes, struct CObjectPoint ** ppNodes )
  {
  kIn_Product = 0;
  kOut_Product = 0;
  k_Gas = 0;
  k_Pilot = 0;
  k_Air = 0;
  k_Mazut = 0;
  k_Steam = 0;
  int Err = 0; 
  int alt_k_Gas = 0;
  int alt_k_Pilot = 0;
  for ( int n = 0; n < kNodes; n++ )
    {
    CObjectPoint * pOP = ppNodes[n];
    if ( CompNodeM ( pOP->PntName, "in_product" ))
      {
      kIn_Product++;
      continue;
      }
    if ( CompNodeM ( pOP->PntName, "out_product" ))
      {
      kOut_Product++;
      continue;
      }
    if ( CompNodeM ( pOP->PntName, "in_topgas" ))
      {
      k_Gas++;
      continue;
      }
    if ( CompNodeM ( pOP->PntName, "in_pilot" ))
      {
      k_Pilot++;
      continue;
      }
    if ( CompNodeM ( pOP->PntName, "in_air" ))
      {
      k_Air++;
      continue;
      }
    if ( CompNodeM ( pOP->PntName, "in_mazut" ))
      {
      k_Mazut++;
      continue;
      }
    if ( CompNodeM ( pOP->PntName, "in_steam" ))
      {
      k_Steam++;
      continue;
      }
    if ( CompNodeM ( pOP->PntName, "in_par_topka" ))
      {
      continue;
      }
    if ( CompNodeM ( pOP->PntName, "in_add" ))
      {
      continue;
      }
    if ( CompNodeM ( pOP->PntName, "out_smoke" ))
      {
      continue;
      }
    ModMsg ( "В '%s' неизвестный вход '%s'", ObjName, pOP->PntName );
    Err = 1;
    }
  if ( kIn_Product != kOut_Product )
    {
    ModMsg ( "В '%s' число входов продукта не равно числу выходов", ObjName );
    Err = 2;
    }
  if ( kIn_Product == 0 )
    {
		ModMsg ( "#В '%s' не заданы входы/выходы продукта", ObjName );
    //Err = 3;
    }
  if ( k_Gas != 1 )
    {
    ModMsg ( "#В '%s' не задана подача газа", ObjName );
    Err = 4;
    }
  if ( k_Pilot != 1 )
    {
		ModMsg ( "#В '%s' не задана подача пилотного газа", ObjName );
    //Err = 5;
    }
  if ( k_Mazut > 0 && k_Steam <= 0 )
    {
    ModMsg ( "В '%s' не задана подача пара на распыл мазута", ObjName );
    Err = 6;
    }
  if ( alt_k_Gas != 0 && alt_k_Gas != 4 )
    {
    ModMsg ( "В '%s' ошибка подачи газа", ObjName );
    Err = 7;
    }
  if ( alt_k_Pilot != 0 && alt_k_Pilot != 4 )
    {
    ModMsg ( "В '%s' ошибка подачи пилотного газа", ObjName );
    Err = 8;
    }
  return Err;
  }

int CFurnace::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
  {
  int Res = ::TestNodes( ObjName, kNodes, ppNodes );
  if ( Res )
    return 1;
//  ShowNode.Set( this, kNodes, ppNodes );
  kProduct = kIn_Product;
  pTube = NewArr(CRadTube *,kProduct );
  pNodeTube = NewArr(CNodeTube,kProduct );
  W_Tube = NewArr(double,kProduct );
  int nIn_Product = 0;
  int Err = 0; 
  char Out_Name[128] = "out_";
  for ( int n = 0; n < kNodes; n++ )
    {
    CObjectPoint * pOP = ppNodes[n];
    if ( CompNodeM ( pOP->PntName, "in_product" ))
      {
      pOP->PntGroup = nIn_Product;  
      pOP->NumbInGroup = 0;
      pOP->IO = '+';
      pNodeTube[nIn_Product].In = NodeNoNumb ( pOP->PntName );
      lstrcpy( Out_Name + 4, NodeNoNumb ( pOP->PntName ) + 3 );
      bool OK = false;
      for ( int j = 0; j < kNodes; j++ )
        {
        CObjectPoint * pOP_Out = ppNodes[j];
        if ( lstrcmp ( NodeNoNumb ( pOP_Out->PntName ), Out_Name ) == 0 )
          {
          OK = true;
          pNodeTube[nIn_Product].Out = NodeNoNumb ( pOP_Out->PntName );
          pOP_Out->PntGroup = nIn_Product;  
          pOP_Out->NumbInGroup = 1;
          pOP_Out->IO = '-';
          break;
          }
        }
      nIn_Product++;
      if ( !OK )
        {
        ModMsg ( "Не найден выход, соответствующий входу '%s'", NodeNoNumb ( pOP->PntName ));
        Err = 1;
        }
      continue;
      }
    if ( CompNodeM ( pOP->PntName, "out_product" ))
      {
      continue;
      }
    if ( CompNodeM ( pOP->PntName, "in_topgas" ))
      {
      pOP->PntGroup = 101;  
      pOP->NumbInGroup = 0;
      pOP->IO = '+';
      continue;
      }
    if ( CompNodeM ( pOP->PntName, "in_pilot" ))
      {
      pOP->PntGroup = 102;  
      pOP->NumbInGroup = 0;
      pOP->IO = '+';
      continue;
      }
    if ( CompNodeM ( pOP->PntName, "in_air" ))
      {
      pOP->PntGroup = 103;  
      pOP->NumbInGroup = 0;
      pOP->IO = '+';
      continue;
      }
    if ( CompNodeM ( pOP->PntName, "in_mazut" ))
      {
      pOP->PntGroup = 104;  
      pOP->NumbInGroup = 0;
      pOP->IO = '+';
      continue;
      }
    if ( CompNodeM ( pOP->PntName, "in_steam" ))
      {
      pOP->PntGroup = 105;  
      pOP->NumbInGroup = 0;
      pOP->IO = '+';
      continue;
      }
    if ( CompNodeM ( pOP->PntName, "in_par_topka" ))
      {
      pOP->PntGroup = 106;  
      pOP->NumbInGroup = 0;
      pOP->IO = '+';
      continue;
      }
    if ( CompNodeM ( pOP->PntName, "in_add" ))
      {
      pOP->PntGroup = 107;  
      pOP->NumbInGroup = 0;
      pOP->IO = '+';
      continue;
      }
    if ( CompNodeM ( pOP->PntName, "out_smoke" ))
      {
      pOP->PntGroup = 120;  
      pOP->NumbInGroup = 0;
      pOP->IO = '-';
      continue;
      }
    ModMsg ( "Ошибочная точка '%s' объекта '%s'", NodeNoNumb ( pOP->PntName ), ObjName );
    Err = 2;
    }
  return Err;
  }

#include "SetMatr.h"

  int CFurnace::SetMatr( void * pExternals, int PntGroup,  double dt, 
    double A[/* kExkV */], double B[ /*kE*/ ] )
  {
  SET_BP BreakPoint;
  if ( PntGroup < 100 )
    {
    int n = PntGroup;
    double O = Omega_Product * ( 1. - pTube[n]->E_Coke );
    double * Eq = A;
    Eq[P_0] = O;
    Eq[P_1] = -O;
    Eq[F_0] = -1.;
    //
    Eq += 4;
    Eq[F_0] = -1.;
    Eq[F_1] = 1.;
    //
    Eq[P_0] = pTube[n]->O_Прогар;
    B[1] = pTube[n]->O_Прогар;
    SET_BP BreakPoint;
    return 0;
    }
  double Pb = 1. + P_Под;
  double Pt = 1. + P_Верх;
  switch ( PntGroup )
    {
    case 101:
      {
      SET_BP BreakPoint;
      MATR_IN(1.,(Omega_Topl_Gas*O_Gas))
      }
    return 0;
    case 102:
      {
      SET_BP BreakPoint;
      MATR_IN(Pb,(Omega_Pilot*O_Pilot))
      }
    return 0;
    case 103:
      {
      SET_BP BreakPoint;
      MATR_IN(Pb,(Omega_Air*O_Air))
      }
    return 0;
    case 104:
      {
      SET_BP BreakPoint;
      MATR_IN(Pb,(Omega_Mazut*O_Mazut))
      }
    return 0;
    case 105:
      {
      SET_BP BreakPoint;
      MATR_IN(Pb,(Omega_Mazut_Steam*O_Steam))
      }
    return 0;
    case 106:
      {
      SET_BP BreakPoint;
      MATR_IN(Pb,Omega_Steam)
      }
    return 0;
    case 107:
      {
      SET_BP BreakPoint;
      MATR_IN(Pb,(Omega_Add_Gas*O_Add_Gas))
      }
    return 0;
    case 120:
      {
      if ( No_Friction )
        {
        SET_BP BreakPoint;
        A[F_0] = 1.;
        B[0] = Flow_Smoke * Alfa_Anti_Smoke;
        return 0;
        }
      if ( New_Calc )
        {
        Mw_podsos = Mw_air;
        A[F_0] = Omega_out_smoke * Mw_smoke + Omega_podsos * Mw_podsos;
        A[P_0] = Omega_out_smoke * Omega_podsos * Mw_podsos;
        B[0] = Omega_out_smoke * ( 
          Flow_topl_gas * Mw_gas + Flow_mazut * Mw_mazut + Flow_steam * Mw_steam +
          Flow_air_fors * Mw_air + Omega_podsos * Mw_podsos 
          );
        SET_BP BreakPoint;
        return 0;
        }
      SET_BP BreakPoint;
//??        MATR_OUT(Pt,Omega_Smoke)
      A[P_0] = 1.;
      A[F_0] = dt * k_P;
      B[0] = Pt + dt * k_P * Flow_Smoke;
      }
    return 0;
    default:
      ASS(0)
    break;
    }
  return 0;
  }

#include "SetVar.h"

  int CFurnace::SetVar( void * pExternals, int PntGroup, double dt, 
    double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
  {
  SET_BP BreakPoint;
  if ( PntGroup < 100 )
    {
    int n = PntGroup;
    if ( Var[F_0] > 0. )
      {
      pTube[n]->pFlow_in = (CFlow*)pVar[0];
      pTube[n]->pFlow_out = (CFlow*)pVar[1];
      pTube[n]->F_Прогар = fabs ( Var[F_0] - Var[F_1] );
      }
    else
      {
      pTube[n]->pFlow_out = (CFlow*)pVar[0];
      pTube[n]->pFlow_in = (CFlow*)pVar[1];
      pTube[n]->F_Прогар = fabs ( Var[F_1] - Var[F_0] );
      }
    SET_BP BreakPoint;
    return 0;
    }
  switch ( PntGroup )
    {
    case 101:
      {
      pTopl_Gas = (CFlow*)pVar[0];
      if ( Comp_GasZ[0] )
        pTopl_Gas->ID_COMP = ID_CompGasZ;
      SET_BP BreakPoint;
      }
    return 0;
    case 102:
      {
      pPilot = (CFlow*)pVar[0];
      if ( Comp_GasZ[0] )
        pPilot->ID_COMP = ID_CompGasZ;
      SET_BP BreakPoint;
      }
    return 0;
    case 103:
      {
      pAir = (CFlow*)pVar[0];
      SET_BP BreakPoint;
      }
    return 0;
    case 104:
      {
      pMazut = (CFlow*)pVar[0];
      SET_BP BreakPoint;
      }
    return 0;
    case 105:
      {
      pMazutSteam = (CFlow*)pVar[0];
      SET_BP BreakPoint;
      }
    return 0;
    case 106:
      {
      pSteam = (CFlow*)pVar[0];
      SET_BP BreakPoint;
      }
    return 0;
    case 107:
      {
      pAdd_Gas = (CFlow*)pVar[0];
      SET_BP BreakPoint;
      }
    return 0;
    case 120:
      {
      if ( No_Friction )
        {
        pSmoke = (CFlow*)pVar[0];
        if ( Var[P_0] > 1. + P_out_smoke_max_mm * 1e-4 )
          {
          double P = Var[P_0] - 1.;
          double Pm = P_out_smoke_max_mm * 1e-4;
          P = Pm + LimdX ( P - Pm, 0.5 * Pm );
          Var[P_0] = P + 1.;
          double K = ( P - Pm ) / Pm;
          Alfa_Anti_Smoke *= 1. - K;
          }
        else
          Alfa_Anti_Smoke = 0.9 * Alfa_Anti_Smoke + 0.1;
        P_out_smoke = Var[P_0];
        F_out_smoke = Var[F_0];
        P_fors = 1. - Alfa_P_fors * ( 1. - P_out_smoke );
        F_out_smoke_m3_fact = F_out_smoke * M3 * 1e-3;
        Flow_air_add = 0.1 * Omega_podsos * ( 1. - P_fors );
        F_air_m3_fact = Flow_air_add * M3 * 1e-3;
        //
        P_out_smoke_mm = ( 1. - P_out_smoke ) * 1e4;
        P_fors_mm = ( 1. - P_fors ) * 1e4;
        P_Верх_мм = P_out_smoke_mm;
        P_Под_мм = P_fors_mm;
        P_Под = P_fors - 1.;
        double Rasr = P_Под_мм - pD_Разряжение_в_поде;
        Разряжение_в_поде = Rasr;
        Отрицательное_разряжение_в_поде = -Rasr;
        Разряжение_на_перевале = P_Верх_мм - pD_Разряжение_на_перевале;
//        Разряжение_на_перевале_1 = P_Верх_мм - pD_Разряжение_на_перевале;
//        Разряжение_на_перевале_2 = P_Верх_мм - pD_Разряжение_на_перевале;
        double T1 = 
          ( Omega_out_smoke * Mw_smoke + Omega_podsos * Mw_podsos ) * F_out_smoke + 
          Omega_out_smoke * Omega_podsos * Mw_podsos * P_out_smoke - 
          Omega_out_smoke * ( Flow_topl_gas * Mw_gas + Flow_air_fors * Mw_air + Omega_podsos * Mw_podsos );
        double T2 = F_out_smoke - Omega_out_smoke * ( P_fors - P_out_smoke );
        double T3 = F_out_smoke * Mw_smoke - (
          Flow_topl_gas * Mw_gas + Flow_mazut * Mw_mazut + Flow_steam * Mw_steam + 
          Flow_air_fors * Mw_air + Flow_air_add * Mw_podsos );
        SET_BP BreakPoint;
        return 0;
        }
      if ( New_Calc )
        {
        pSmoke = (CFlow*)pVar[0];
        if ( kStep >= 2 )
          {
          P_out_smoke = 0.9 * P_out_smoke + 0.1 * Var[P_0];
          F_out_smoke = 0.9 * F_out_smoke + 0.1 * Var[F_0];
          P_fors = 0.9 * P_fors + 0.1 * ( P_out_smoke + F_out_smoke / Omega_out_smoke );
          }
        else
          {
          P_out_smoke = Var[P_0];
          F_out_smoke = Var[F_0];
          P_fors = P_out_smoke + F_out_smoke / Omega_out_smoke;
          }
        F_out_smoke_m3_fact = F_out_smoke * M3 * 1e-3;
        Flow_air_add = Omega_podsos * ( 1. - P_fors );
        F_air_m3_fact = Flow_air_add * M3 * 1e-3;
        //
        P_out_smoke_mm = ( 1. - P_out_smoke ) * 1e4;
        P_fors_mm = ( 1. - P_fors ) * 1e4;
        P_Верх_мм = -P_out_smoke_mm;
        P_Под_мм = -P_fors_mm;
        P_Под = P_fors - 1.;
        double Rasr = P_Под_мм - pD_Разряжение_в_поде;
        Разряжение_в_поде = Rasr;
        Отрицательное_разряжение_в_поде = -Rasr;
        Разряжение_на_перевале = P_Верх_мм - pD_Разряжение_на_перевале;
//        Разряжение_на_перевале_1 = P_Верх_мм - pD_Разряжение_на_перевале;
//        Разряжение_на_перевале_2 = P_Верх_мм - pD_Разряжение_на_перевале;
        double T1 = 
          ( Omega_out_smoke * Mw_smoke + Omega_podsos * Mw_podsos ) * F_out_smoke + 
          Omega_out_smoke * Omega_podsos * Mw_podsos * P_out_smoke - 
          Omega_out_smoke * ( Flow_topl_gas * Mw_gas + Flow_air_fors * Mw_air + Omega_podsos * Mw_podsos );
        double T2 = F_out_smoke - Omega_out_smoke * ( P_fors - P_out_smoke );
        double T3 = F_out_smoke * Mw_smoke - (
          Flow_topl_gas * Mw_gas + Flow_mazut * Mw_mazut + Flow_steam * Mw_steam + 
          Flow_air_fors * Mw_air + Flow_air_add * Mw_podsos );
        SET_BP BreakPoint;
        return 0;
        }
///////////////////////////////////
      pSmoke = (CFlow*)pVar[0];
      F_out_smoke = Var[F_0];
      F_out_smoke_m3_fact = F_out_smoke * M3 * 1e-3;
      SET_BP BreakPoint;
      double Pt = Var[P_0];
      P_Верх = Pt - 1.;
      }
    return 0;
    default:
      ASS(0)
    break;
    }
  return 0;
  }

