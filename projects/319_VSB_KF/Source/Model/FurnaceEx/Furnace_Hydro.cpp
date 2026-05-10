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
NODE_IN(1, 0, "in_topgas_#", "Подача топливного газа" )
NODE_IN(2, 0, "in_pilot_#", "Подача пилотног газа" )
NODE_IN(3, 0, "in_air_#", "Подача воздуха" )
NODE_IN(4, 0, "in_mazut_#", "Подача мазута" )
NODE_IN(5, 0, "in_steam_#", "Подача пара на распыл мазута" )
NODE_IN(6, 0, "in_par_topka", "Подача пара в топку" )
NODE_IN(7, 0, "in_add", "Дополнительный вход горючих материалов" )
NODE_OUT(8, 0, "out_smoke", "Выход дыма" )
END_LIST


bool ToplIn ( char * Type, char * Name, int & Numb )
  {
  char Gas[128];
//  while( *Name >= '0' && *Name <= '9' )
//    Name++;
  lstrcpy ( Gas, Name );
  char * P = strchr ( Gas, '_' );
  if ( P == NULL )
    {
    if ( strcmpi( Gas, Type ) == 0 )
      return true;
    return false;
    }
  char * P2 = strchr ( P + 1, '_' );
  if ( P2 == NULL )
    {
    if ( strcmpi( Gas, Type ) != 0 )
      return false;
    Numb = 1;
    return true;
    }
  if ( P2 )
    P = P2;
  *P = 0;
  if ( strcmpi( Gas, Type ) != 0 )
    return false;
  Numb = atoi ( P + 1 );
  return true;
  }

int _kIn_Product = 0;
int _kOut_Product = 0;
int _k_Gas = 0;
int _k_Smoke_in = 0;
int _k_Smo_ke = 0;
int _k_Pilot = 0;
int _k_Air = 0;
int _k_Add = 0;
int _k_Mazut = 0;
int _k_Steam = 0;

extern "C"
EXP int TestNodes( char * ObjName, int kNodes, struct CObjectPoint ** ppNodes )
  {
  int Err = 0; 
  _kIn_Product = 0;
  _kOut_Product = 0;
  _k_Gas = 0;
  _k_Smo_ke = 0;
  _k_Pilot = 0;
  _k_Air = 0;
  _k_Add = 0;
  _k_Mazut = 0;
  _k_Steam = 0;
  for ( int n = 0; n < kNodes; n++ )
    {
/////////
    CObjectPoint * pOP = ppNodes[n];
    pOP->Type = eГидравлика;
    if ( CompNodeM ( pOP->PntName, "in_product" ))
      {
      _kIn_Product++;
      continue;
      }
    if ( CompNodeM ( pOP->PntName, "out_product" ))
      {
      _kOut_Product++;
      continue;
      }
    if ( CompNodeM ( pOP->PntName, "in_par_topka" ))
      {
      continue;
      }
    if ( CompNodeM ( pOP->PntName, "out_smoke" ))
      {
      continue;
      }
/////////
    int N;
    if ( ToplIn( "out_smoke", pOP->PntName, N ))
      {
      _k_Smo_ke++;
      continue;
      }
    if ( ToplIn( "газ", pOP->PntName, N ) || ToplIn( "in_topgas", pOP->PntName, N ))
      {
      _k_Gas++;
      continue;
      }
    if ( ToplIn( "in_smoke", pOP->PntName, N ))
      {
      _k_Smoke_in++;
      continue;
      }
    if ( ToplIn( "пилот", pOP->PntName, N )||ToplIn( "in_pilot", pOP->PntName, N ) )
      {
      _k_Pilot++;
      continue;
      }
    if ( ToplIn( "мазут", pOP->PntName, N )||ToplIn( "in_mazut", pOP->PntName, N ))
      {
      _k_Mazut++;
      continue;
      }
    if ( ToplIn( "пар", pOP->PntName, N )||ToplIn( "in_steam", pOP->PntName, N ))
      {
      _k_Steam++;
      continue;
      }
    if ( ToplIn( "воздух", pOP->PntName, N )||ToplIn( "in_air", pOP->PntName, N ))
      {
      _k_Air++;
      continue;
      }
    if ( ToplIn( "in_add", pOP->PntName, N ))
      {
      continue;
      }
    ModMsg ( "В '%s' неизвестный вход '%s'", ObjName, pOP->PntName );
    Err = 1;
    }
  if ( _kIn_Product != _kOut_Product )
    {
    ModMsg ( "В '%s' число входов продукта не равно числу выходов", ObjName );
    Err = 2;
    }
  if ( _kIn_Product == 0 )
    {
    ModMsg ( "#В '%s' не заданы входы/выходы продукта", ObjName );
    Err = 0;
    }
/////////////////////////////////////////////////
  if ( _k_Gas == 0 && _k_Mazut == 0 )
    {
    ModMsg ( "#В '%s' не задана подача газа", ObjName );
    Err = 4;
    }
  if ( _k_Pilot == 0 )
    {
    ModMsg ( "#В '%s' не задана подача пилотного газа", ObjName );
    }
  if ( _k_Mazut > 0 && _k_Steam <= 0 )
    {
    ModMsg ( "В '%s' не задана подача пара на распыл мазута", ObjName );
    Err = 6;
    }
  if ( _k_Air > 0 && _k_Air != _k_Gas )
    {
    ModMsg ( "В '%s' число входов воздуха не равно числу входов основного газа", ObjName );
    }
  return Err;
  }

int CFurnace::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
  {
  if ( kNodes <= 0 )
    return 0;
  int Res = ::TestNodes( ObjName, kNodes, ppNodes );
  if ( Res )
    return 1;
  kProduct = ::_kIn_Product;
  if ( kProduct > 0 )
    {
    pTube = NewArr(CRadTube *,kProduct );
    pNodeTube = NewArr(CNodeTube,kProduct );
    W_Tube = NewArr(double,kProduct );
    }
  int nIn_Product = 0;
  int Err = 0; 
  char Out_Name[128] = "out_";
// подсчет числа входов топлива
  k_Smoke = 0;
  k_Gas = 0;
  k_Pilot = 0;
  k_Air = 0;
  k_Add = 0;
  k_Mazut = 0;
  k_Steam = 0;
  k_Smoke_in = 0;
  for ( int n = 0; n < kNodes; n++ )
    {
    CObjectPoint * pOP = ppNodes[n];
    pOP->Type = eГидравлика;
    int N;
    if ( ToplIn( "out_smoke", pOP->PntName, N ))
      {
      k_Smoke++;
      continue;
      }
    if ( ToplIn( "газ", pOP->PntName, N ) || ToplIn( "in_topgas", pOP->PntName, N ))
      {
      k_Gas++;
      continue;
      }
    if ( ToplIn( "пилот", pOP->PntName, N )||ToplIn( "in_pilot", pOP->PntName, N ) )
      {
      k_Pilot++;
      continue;
      }
    if ( ToplIn( "мазут", pOP->PntName, N )||ToplIn( "in_mazut", pOP->PntName, N ))
      {
      k_Mazut++;
      continue;
      }
    if ( ToplIn( "in_add", pOP->PntName, N ))
      {
      k_Add++;
      continue;
      }
    if ( ToplIn( "in_smoke", pOP->PntName, N ))
      {
      k_Smoke_in++;
      continue;
      }
    if ( ToplIn( "пар", pOP->PntName, N )||ToplIn( "in_steam", pOP->PntName, N ))
      {
      k_Steam++;
      continue;
      }
    if ( ToplIn( "воздух", pOP->PntName, N )||ToplIn( "in_air", pOP->PntName, N ))
      {
      k_Air++;
      continue;
      }
    }
//////////////////////////////////////////////////////////////////
  kFors = k_Gas;
  if ( k_Pilot > kFors )
    kFors = k_Pilot;
  if ( k_Mazut > kFors )
    kFors = k_Mazut;
  if ( k_Steam > kFors )
    kFors = k_Steam;
  if ( k_Air > kFors )
    kFors = k_Air;
  if ( k_Smoke > kFors )
    kFors = k_Smoke;
//
  if ( kFors <= 0 )
    {
    ModMsg ( "В печи '%s' не задана подача топлива", ObjName );
    return 1;
    }
  if ( kFors > MAX_FORS )
    {
    ModMsg ( "В печи '%s' форсунок > %d", ObjName, (int)MAX_FORS );
    return 1;                                                   
    }
  bool InvalidPoints = false; 
  if ( k_Gas > 0 && k_Gas != kFors )
    InvalidPoints = true;
  if ( k_Pilot > 0 && k_Pilot != kFors )
    InvalidPoints = true;
  if ( k_Mazut > 0 && k_Mazut != kFors )
    InvalidPoints = true;
  if ( k_Steam > 0 && k_Steam != kFors )
    InvalidPoints = true;
  if ( k_Air > 0 && k_Air != kFors )
    InvalidPoints = true;
  if ( k_Smoke > 0 && k_Smoke != 1 )
    InvalidPoints = true;
  if ( k_Smoke_in > 0 && k_Smoke_in != kFors )
    InvalidPoints = true;
  if ( InvalidPoints )
    {
    ModMsg ( "Несоответствие различных входов в печи '%s'", ObjName, (int)MAX_FORS );
    return 1;                                                   
    }
  Pz = NewArr(double,kFors);
  Pz_mm = NewArr(double,kFors);
  Flow_Air = NewArr(double,kFors);
  pSmoke = NewArr(CFlow*,kFors);
  P_Out = NewArr(double,kFors);
  P_Smoke = NewArr(double,kFors);
  F_Smoke_m3 = NewArr(double,kFors);
  P_Gas = NewArr(double,kFors);
  F_Gas_m3 = NewArr(double,kFors);
  F_Mazut_kg = NewArr(double,kFors);
  F_Steam_kg = NewArr(double,kFors);
  P_Pilot = NewArr(double,kFors);
  F_Pilot_m3 = NewArr(double,kFors);
//??  P_Air = NewArr(double,kFors);
  F_Air_m3 = NewArr(double,kFors);
  F_Air_podsos_m3 = NewArr(double,kFors);
  if ( k_Gas > 0 )
    {
    pTopl_Gas = NewArr(CFlow*,kFors);
    memset(pTopl_Gas, 0, 4*kFors);
    }
  if ( k_Pilot > 0 )
    {
    pPilot = NewArr(CFlow*,kFors);
    memset(pPilot, 0, 4*kFors);
    }
  if ( k_Mazut > 0 )
    {
    pMazut = NewArr(CFlow*,kFors);
    memset(pMazut, 0, 4*kFors);
    }
  if ( k_Smoke_in > 0 )
    {
    pFlow_Smoke_in = NewArr(CFlow*,kFors);
    memset(pFlow_Smoke_in, 0, 4*kFors);
    }
  if ( k_Steam > 0 )
    {
    pMazutSteam = NewArr(CFlow*,kFors);
    memset(pMazutSteam, 0, 4*kFors);
    }
  if ( k_Air > 0 )
    {
    pAir = NewArr(CFlow*,kFors);
    memset(pAir, 0, 4*kFors);
    }
  if ( k_Add > 0 )
    {
    pAdd = NewArr(CFlow*,kFors);
    memset(pAdd, 0, 4*kFors);
    }
  Use = NewArr(DWORD,kFors);
  memset(Use, 0, 4*kFors);
#define GAS 0x0001
#define PILOT 0x0002
#define MAZUT 0x0004
#define STEAM 0x0008
#define AIR 0x0010
#define SMOKE_IN 0x0020
#define ADD 0x0040
  for ( int n = 0; n < kNodes; n++ )
    {
    CObjectPoint * pOP = ppNodes[n];
    pOP->Type = eГидравлика;
    int N;
    if ( ToplIn( "газ", pOP->PntName, N ) || ToplIn( "in_topgas", pOP->PntName, N ))
      {
      if ( N < 1 || N > kFors )
        {
        ModMsg ( "В печи '%s' неправильный номер форсунки %s", ObjName, pOP->PntName );
        Err = __LINE__;
        continue;
        }
      Use[N-1] |= GAS;
      continue;
      }
    if ( ToplIn( "пилот", pOP->PntName, N )||ToplIn( "in_pilot", pOP->PntName, N ) )
      {
      if ( N < 1 || N > kFors )
        {
        ModMsg ( "В печи '%s' неправильный номер форсунки %s", ObjName, pOP->PntName );
        Err = __LINE__;
        continue;
        }
      Use[N-1] |= PILOT;
      continue;
      }
    if ( ToplIn( "мазут", pOP->PntName, N )||ToplIn( "in_mazut", pOP->PntName, N ))
      {
      if ( N < 1 || N > kFors )
        {
        ModMsg ( "В печи '%s' неправильный номер форсунки %s", ObjName, pOP->PntName );
        Err = __LINE__;
        continue;
        }
      Use[N-1] |= MAZUT;
      continue;
      }
    if ( ToplIn( "in_add", pOP->PntName, N ))
      {
      if ( N < 1 || N > kFors )
        {
        ModMsg ( "В печи '%s' неправильный номер форсунки %s", ObjName, pOP->PntName );
        Err = __LINE__;
        continue;
        }
      Use[N-1] |= ADD;
      continue;
      }
    if ( ToplIn( "пар", pOP->PntName, N )||ToplIn( "in_steam", pOP->PntName, N ))
      {
      if ( N < 1 || N > kFors )
        {
        ModMsg ( "В печи '%s' неправильный номер форсунки %s", ObjName, pOP->PntName );
        Err = __LINE__;
        continue;
        }
      Use[N-1] |= STEAM;
      continue;
      }
    if ( ToplIn( "in_smoke", pOP->PntName, N ))
      {
      if ( N < 1 || N > kFors )
        {
        ModMsg ( "В печи '%s' неправильный номер форсунки %s", ObjName, pOP->PntName );
        Err = __LINE__;
        continue;
        }
      Use[N-1] |= SMOKE_IN;
      continue;
      }
    if ( ToplIn( "воздух", pOP->PntName, N )||ToplIn( "in_air", pOP->PntName, N ))
      {
      if ( N < 1 || N > kFors )
        {
        ModMsg ( "В печи '%s' неправильный номер форсунки %s", ObjName, pOP->PntName );
        Err = __LINE__;
        continue;
        }
      Use[N-1] |= AIR;
      continue;
      }
    }
  if ( Err )
    return Err;
//////////////////////////////////////////////////////////////////
// Сочетания
  DWORD T_AIR = 0;
  if ( k_Air > 0 )
    T_AIR = AIR;
  for ( int n = 0; n < kFors; n++ )
    {
    DWORD ST = Use[n];
    DWORD T = GAS|PILOT;
    //if (( ST & T ) != 0 && ( ST & T ) != T )
    //  {
    //  ModMsg ( "В печи '%s' неправильная подача газа на форсунку %d", ObjName, n + 1 );
    //  Err = __LINE__;
    //  }
    //T = MAZUT|STEAM;
    //if (( ST & T ) != 0 && ( ST & T ) != T )
    //  {
    //  ModMsg ( "В печи '%s' неправильная подача мазута и пара на форсунку %d", ObjName, n + 1 );
    //  Err = __LINE__;
    //  }
    if (( ST & T_AIR ) != T_AIR )
      {
      ModMsg ( "В печи '%s' не подается воздух на форсунку %d", ObjName, n + 1 );
      Err = __LINE__;
      }
    }
  if ( Err )
    return Err;
//////////////////////////////////////////////////////////////////
  MatrixModel = false;
/*
  if ( k_Smoke > 0 )
    {
    MatrixModel = true;
    p_Smoke = -1;
    p_Gas = -1;
    p_Pilot = -1;
    p_Air = -1;
    p_Mazut = -1;
    p_Steam = -1;
    p_Smoke_in = -1;
    int p = 0;
    NumbFlow[p] = N_SMOKE;
    p_Smoke = p++;
    if ( k_Air > 0)
      {
      NumbFlow[p] = N_AIR;
      p_Air = p++;
      }
    if ( k_Gas > 0)
      {
      NumbFlow[p] = N_GAS;
      p_Gas = p++;
      }
    if ( k_Pilot > 0)
      {
      NumbFlow[p] = N_PILOT;
      p_Pilot = p++;
      }
    if ( k_Mazut > 0)
      {
      NumbFlow[p] = N_MAZUT;
      p_Mazut = p++;
      }
    if ( k_Steam > 0)
      {
      NumbFlow[p] = N_STEAM;
      p_Steam = p++;
      }
    if ( k_Smoke_in > 0)
      {
      NumbFlow[p] = N_SMOKE_IN;
      p_Smoke_in = p++;
      }
    N_Matrix = p;
    }
  */
  for ( int n = 0; n < kNodes; n++ )
    {
    CObjectPoint * pOP = ppNodes[n];
    pOP->Type = eГидравлика;
    if ( CompNodeM ( pOP->PntName, "in_product" ))
      {
      pOP->PntGroup = nIn_Product;  
      pOP->NumbInGroup = 0;
      pOP->IO = '+';
      pOP->Type = eГидравлика;
      pNodeTube[nIn_Product].In = pOP->PntName;
      lstrcpy( Out_Name + 4, pOP->PntName + 3 );
      bool OK = false;
      for ( int j = 0; j < kNodes; j++ )
        {
        CObjectPoint * pOP_Out = ppNodes[j];
        pOP->Type = eГидравлика;
        if ( lstrcmp ( pOP_Out->PntName, Out_Name ) == 0 )
          {
          OK = true;
          pNodeTube[nIn_Product].Out = pOP_Out->PntName;
          pOP_Out->PntGroup = nIn_Product;  
          pOP_Out->NumbInGroup = 1;
          pOP_Out->IO = '-';
          pOP_Out->Type = eГидравлика;
          break;
          }
        }
      nIn_Product++;
      if ( !OK )
        {
        ModMsg ( "Не найден выход, соответствующий входу '%s'", pOP->PntName );
        Err = 1;
        }
      continue;
      }
    if ( CompNodeM ( pOP->PntName, "out_product" ))
      {
      continue;
      }
    if ( CompNodeM ( pOP->PntName, "in_par_topka" ))
      {
      pOP->PntGroup = 800;  
      pOP->NumbInGroup = 0;
      pOP->IO = '+';
      pOP->Type = eГидравлика;
      continue;
      }
    if ( lstrcmp ( pOP->PntName, "out_smoke" ) == 0 )
      {
      ASS( !MatrixModel )
      pOP->PntGroup = 900;  
      pOP->NumbInGroup = 0;
      pOP->IO = '-';
      pOP->Type = eГидравлика;
      continue;
      }
    //if ( lstrcmp ( NodeNoNumb ( pOP->PntName ), "in_smoke" ) == 0 )
    //  {
    //  ASS( !MatrixModel )
    //  pOP->PntGroup = 900;  
    //  pOP->NumbInGroup = 0;
    //  pOP->IO = '+';
    //  pOP->Type = eГидравлика;
    //  continue;
    //  }
    int N;
    //if ( ToplIn( "out_smoke", pOP->PntName, N ))
    //  {
    //  ASS( MatrixModel )
    //  pOP->PntGroup = 999+N;  
    //  pOP->NumbInGroup = p_Smoke;
    //  pOP->IO = '-';
    //  pOP->Type = eГидравлика;
    //  continue;
    //  }
    if ( ToplIn( "газ", pOP->PntName, N ) || ToplIn( "in_topgas", pOP->PntName, N ))
      {
      if ( MatrixModel )
        {
        pOP->PntGroup = 999+N;  
        pOP->NumbInGroup = p_Gas;
        pOP->IO = '+';
        pOP->Type = eГидравлика;
        }
      else
        {
        pOP->PntGroup = 99+N;  
        pOP->NumbInGroup = 0;
        pOP->IO = '+';
        pOP->Type = eГидравлика;
        }
      continue;
      }
    if ( ToplIn( "пилот", pOP->PntName, N )||ToplIn( "in_pilot", pOP->PntName, N ) )
      {
      if ( MatrixModel )
        {
        pOP->PntGroup = 999+N;  
        pOP->NumbInGroup = p_Pilot;
        pOP->IO = '+';
        pOP->Type = eГидравлика;
        }
      else
        {
        pOP->PntGroup = 199+N;  
        pOP->NumbInGroup = 0;
        pOP->IO = '+';
        pOP->Type = eГидравлика;
        }
      continue;
      }
    if ( ToplIn( "мазут", pOP->PntName, N )||ToplIn( "in_mazut", pOP->PntName, N ))
      {
      if ( MatrixModel )
        {
        pOP->PntGroup = 999+N;  
        pOP->NumbInGroup = p_Mazut;
        pOP->IO = '+';
        pOP->Type = eГидравлика;
        }
      else
        {
        pOP->PntGroup = 299+N;  
        pOP->NumbInGroup = 0;
        pOP->IO = '+';
        pOP->Type = eГидравлика;
        }
      continue;
      }
    if ( ToplIn( "in_add", pOP->PntName, N ))
      {
      pOP->PntGroup = 599+N;  
      pOP->NumbInGroup = 0;
      pOP->IO = '+';
      pOP->Type = eГидравлика;
      continue;
      }
    if ( ToplIn( "пар", pOP->PntName, N )||ToplIn( "in_steam", pOP->PntName, N ))
      {
      if ( MatrixModel )
        {
        pOP->PntGroup = 999+N;  
        pOP->NumbInGroup = p_Steam;
        pOP->IO = '+';
        pOP->Type = eГидравлика;
        }
      else
        {
        pOP->PntGroup = 399+N;  
        pOP->NumbInGroup = 0;
        pOP->IO = '+';
        pOP->Type = eГидравлика;
        }
      continue;
      }
    if ( ToplIn( "воздух", pOP->PntName, N )||ToplIn( "in_air", pOP->PntName, N ))
      {
      if ( MatrixModel )
        {
        pOP->PntGroup = 999+N;  
        pOP->NumbInGroup = p_Air;
        pOP->IO = '+';
        pOP->Type = eГидравлика;
        }
      else
        {
        pOP->PntGroup = 499+N;  
        pOP->NumbInGroup = 0;
        pOP->IO = '+';
        pOP->Type = eГидравлика;
        }
      continue;
      }
    if ( ToplIn( "in_smoke", pOP->PntName, N ))
      {
      if ( MatrixModel )
        {
        pOP->PntGroup = 999+N;  
        pOP->NumbInGroup = p_Smoke_in;
        pOP->IO = '+';
        pOP->Type = eГидравлика;
        }
      else
        {
        pOP->PntGroup = 699+N;  
        pOP->NumbInGroup = 0;
        pOP->IO = '+';
        pOP->Type = eГидравлика;
        }
      continue;
      }
    ModMsg ( "Ошибочная точка '%s' объекта '%s'", pOP->PntName, ObjName );
    Err = 2;
    }
  return Err;
  }

#include "SetMatr.h"

#define DIM 5
double sA[DIM][2*DIM];

int CFurnace::SetMatr( void * pExternals, int PntGroup,  double dt, 
                      double A[/* kExkV */], double B[ /*kE*/ ] )
  {
  SET_BP BreakPoint;
  if ( PntGroup < 100 )
    {
    int n = PntGroup;
    double * Eq = A;
    Eq[P_0] = Omega_Product;
    Eq[P_1] = -Omega_Product;
    Eq[F_0] = -1.;
    //
    Eq += 4;
    if ( pTube && pTube[n] )
      Eq[F_0] = pTube[n]->kFlow;
    else
      Eq[F_0] = 1.;
    Eq[F_1] = -1.;
//
    if ( pTube && pTube[n] )
      {
      Eq[P_0] = pTube[n]->O_Прогар;
      B[1] = pTube[n]->O_Прогар;
      }
    return 0;
    }
  double Pb = 1. + P_Под;
  double Pt = 1. + P_Верх;
  if ( PntGroup < 200 )
    {
    // in_topgas
    MATR_IN(Pb,(Omega_Topl_Gas))
    return 0;
    }
  if ( PntGroup < 300 )
    {
    // in_pilot
    MATR_IN(Pb,(Omega_Pilot))
    return 0;
    }
  if ( PntGroup < 400 )
    {
    //in_mazut
    MATR_IN(Pb,(Omega_Mazut))
    return 0;
    }
  if ( PntGroup < 500 )
    {
    //in_steam
    MATR_IN(Pb,(Omega_Mazut_Steam))
    return 0;
    }
  if ( PntGroup < 600 )
    {
    //in_air
    int n = PntGroup - 500;
    double P = 1. - P_Air[n] * 1e-4 + k_P_Под * P_Под;
    MATR_IN(P,(Omega_Air))
    return 0;
    }
  if ( PntGroup < 700 )
    {
    // in_add
    int n = PntGroup - 600;
    MATR_IN(Pb,(Omega_Add))
    return 0;
    }
  if ( PntGroup < 800 )
    {
    // in_smoke
    MATR_IN(Pb,Omega_Smoke_in)
    return 0;
    }
 //
  if ( PntGroup == 800 )
    {
    // in_par_topka
    MATR_IN(Pb,Omega_Steam)
    return 0;
    }
  if ( PntGroup == 900 )
    {
    // out_smoke
    ASS(!MatrixModel)
    if ( strstr ( ObjName, "1_левая" ))
      KKK();
//    MATR_OUT(Pt,Omega_Smoke)
//    k_P = 0.01;
    if ( SmokeAsFlow )
      {
      A[F_0] = 1;
      B[0] = Flow_Smoke;
      }
    else
      {
      A[P_0] = 1.;
      A[F_0] = dt * k_P;
      B[0] = Pt + dt * k_P * Flow_Smoke;
      }
    return 0;
    }
  if ( PntGroup >= 1000 )
    {
#define MP(i,j) A[(i)*N_Matrix+(j)*2]
#define MF(i,j) A[(i)*N_Matrix+(j)*2+1]
    int n = PntGroup - 900;
// Уравнения для дыма
	 for ( int n = 0; n < 6; n++ )
		 {
		 if ( Mu[n] < 16 )
			 Mu[n] = 28.;
	   }
   MF(0,0) = Mu[N_SMOKE];
   //int j_GAS = -1;
   //int j_MAZUT = -1;
   for ( int j = 1; j < N_Matrix; j++ )
     {
     int P = NumbFlow[j];
     if ( P == N_SMOKE_IN )
       MF(0,j) = -Mu_Smoke_in;
     else
      MF(0,j) = -Mu[P];
     if ( P == N_GAS )
       {
//       j_GAS = j;
       MF(0,j) -= Mu[N_AIR] * k_Air_Gas;
       }
     if ( P == N_MAZUT )
       {
//       j_MAZUT = j;
       MF(0,j) -= Mu[N_AIR] * k_Air_Mazut;
       }
     }
   if ( k_Air <= 0 )
     B[0] = Mu[N_AIR] * Omega[N_AIR];
// Обычные уравнения
   for ( int i = 1; i < N_Matrix; i++ )
     {
     MF(i,i) = -1.;
     int P = NumbFlow[i];
     MP(i,i) = Omega[P];
     MP(i,0) = -Omega[P];
     MF(i,0) = -Omega[P] / Omega[N_SMOKE];
     //if ( P == N_AIR )
     //  {
     //  if ( k_Gas > 0 )
     //    MF(i,j_GAS) = k_Air_Gas;
     //  if ( k_Mazut > 0 )
     //    MF(i,j_MAZUT) = k_Air_Mazut;
     //  }
     }
    memcpy( sA, A, sizeof ( sA ));
    return 0;
    }
  ASS(0)
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
    return 0;
    }
  if ( PntGroup < 200 )
    {
    // in_topgas
    int n = PntGroup - 100;
    pTopl_Gas[n] = (CFlow*)pVar[0];
    return 0;
    }
  if ( PntGroup < 300 )
    {
    // in_pilot
    int n = PntGroup - 200;
    pPilot[n] = (CFlow*)pVar[0];
    return 0;
    }
  if ( PntGroup < 400 )
    {
    // in_mazut
    int n = PntGroup - 300;
    pMazut[n] = (CFlow*)pVar[0];
    return 0;
    }
  if ( PntGroup < 500 )
    {
    // in_steam
    int n = PntGroup - 400;
    pMazutSteam[n] = (CFlow*)pVar[0];
    return 0;
    }
  if ( PntGroup < 600 )
    {
    // in_air
    int n = PntGroup - 500;
    pAir[n] = (CFlow*)pVar[0];
    return 0;
    }
  if ( PntGroup < 700 )
    {
    // in_add
    int n = PntGroup - 600;
    pAdd[n] = (CFlow*)pVar[0];
    return 0;
    }
  if ( PntGroup < 800 )
    {
    // in_smoke
    int n = PntGroup - 700;
    pFlow_Smoke_in[n] = (CFlow*)pVar[0];
    return 0;
    }
  if ( PntGroup == 800 )
    {
    // in_par_topka
    pSteam = (CFlow*)pVar[0];
    return 0;
    }
  if ( PntGroup == 900 )
    {
    // out_smoke
    pSmoke[0] = (CFlow*)pVar[0];
    double Pt = Var[P_0];
    P_Верх = Pt - 1.;
    Flow_Smoke_Real = Var[F_0];
    return 0;
    }
  if ( PntGroup >= 1000 )
    {
    int n = PntGroup - 900;
//
    double T[DIM];
    for ( int i = 0; i < DIM; i++ )
      {
      T[i] = 0;
      for ( int j = 0; j < DIM; j++ )
        T[i] += sA[i][j*2] * Var[j*2] + sA[i][j*2+1] * Var[j*2+1];
      }
    double P = Var[P_0];
    P_Out[n] = P;
    P_Smoke[n] = ( 1. - P ) * 1e4;
    double F = Var[F_0];
    double Pm = P + F / Omega[N_SMOKE];
    Pz[n] = Pm;
    Pz_mm[n] = ( 1. - Pm ) * 1e4;
    pSmoke[n] = (CFlow*)pVar[0];
    F_Smoke_m3[n] = pSmoke[n]->Flow_mol * M3;
 //
    for ( int j = 1; j < N_Matrix; j++ )
      {
      CFlow * pF = (CFlow*)pVar[j];
      int P = NumbFlow[j];
      if ( P == N_GAS )
        {
        pTopl_Gas[n] = pF;
        continue;
        }
      if ( P == N_PILOT )
        {
        pPilot[n] = pF;
        continue;
        }
      if ( P == N_MAZUT )
        {
        pMazut[n] = pF;
        continue;
        }
      if ( P == N_STEAM )
        {
        pMazutSteam[n] = pF;
        continue;
        }
      if ( P == N_AIR )
        {
        pAir[n] = pF;
        Flow_Air[n] = pF->Flow_mol;
        continue;
        }
      if ( P == N_SMOKE_IN )
        {
        pFlow_Smoke_in[n] = pF;
        continue;
        }
      }
    if ( k_Air <= 0 )
      Flow_Air[n] = Omega[N_AIR] * ( 1. - Pz[n] );
    double F_Podsos = 0.;
    if ( k_Gas > 0 )
      F_Podsos += k_Air_Gas * pTopl_Gas[n]->Flow_mol;
    if ( k_Mazut > 0 )
      F_Podsos += k_Air_Mazut * pMazut[n]->Flow_mol;
    Flow_Air[n] += F_Podsos;
    P_Gas[n] = pTopl_Gas[n]->P - 1.;
    F_Gas_m3[n] = pTopl_Gas[n]->Flow_mol * M3;
    if ( pMazut )
      F_Mazut_kg[n] = pMazut[n]->Flow_mol * Mw ( pMazut[n]->Get_Comp());
    else
      F_Mazut_kg[n] = 0.;
    if ( pMazutSteam )
      F_Steam_kg[n] = pMazutSteam[n]->Flow_mol * Mw ( pMazutSteam[n]->Get_Comp());
    else
      F_Steam_kg[n] = 0.;
    F_Pilot_m3[n] = pPilot[n]->Flow_mol * M3;
    P_Air[n] = pAir[n]->P - 1.;
    F_Air_m3[n] = pAir[n]->Flow_mol * M3;
    F_Air_podsos_m3[n] = F_Podsos * M3;
// Проверка
    double T_GAS = 0;
    if ( k_Gas > 0 )
      T_GAS = Omega[N_GAS] * ( pTopl_Gas[n]->P - Pm ) - pTopl_Gas[n]->Flow_mol;
    double T_SMOKE_IN = 0;
    if ( k_Smoke_in > 0 )
      T_SMOKE_IN = Omega[N_SMOKE_IN] * ( pFlow_Smoke_in[n]->P - Pm ) - pFlow_Smoke_in[n]->Flow_mol;
    double T_PILOT = 0;
    if ( k_Pilot > 0 )
      T_PILOT = Omega[N_PILOT] * ( pPilot[n]->P - Pm ) - pPilot[n]->Flow_mol;
    double T_MAZUT = 0;
    if ( k_Mazut > 0 )
      T_MAZUT = Omega[N_MAZUT] * ( pMazut[n]->P - Pm ) - pMazut[n]->Flow_mol;
    double T_STEAM = 0;
    if ( k_Steam > 0 )
      T_STEAM = Omega[N_STEAM] * ( pMazutSteam[n]->P - Pm ) - pMazutSteam[n]->Flow_mol;
    double T_AIR = 0;
    if ( k_Air > 0 )
      T_AIR = Omega[N_AIR] * ( pAir[n]->P - Pm );
    else
      T_AIR = Omega[N_AIR] * ( 1. - Pm );
    if ( k_Gas > 0 )
      T_AIR += k_Air_Gas * pTopl_Gas[n]->Flow_mol;
    if ( k_Mazut > 0 )
      T_AIR += k_Air_Mazut * pMazut[n]->Flow_mol;
    T_AIR -= Flow_Air[n];
//
    double T_SMOKE = Mu[N_AIR] * Flow_Air[n];
    if ( k_Gas > 0 )
      T_SMOKE += Mu[N_GAS] * pTopl_Gas[n]->Flow_mol;
    if ( k_Pilot > 0 )
      T_SMOKE += Mu[N_PILOT] * pPilot[n]->Flow_mol;
    if ( k_Mazut > 0 )
      T_SMOKE += Mu[N_MAZUT] * pMazut[n]->Flow_mol;
    if ( k_Steam > 0 )
      T_SMOKE += Mu[N_STEAM] * pMazutSteam[n]->Flow_mol;
    if ( k_Smoke_in > 0 )
      T_SMOKE += Mu_Smoke_in * pFlow_Smoke_in[n]->Flow_mol;
//
    T_SMOKE -= Mu[N_SMOKE] * pSmoke[n]->Flow_mol;
    KKK();
    return 0;
    }
  ASS(0)
  return 0;
  }

