#include "stdafx.h"
#include "UniHydro.h"
#include "Err.h"
//#define DLL_HYDRO
#include "Connect.h"
#include "HydroStruct.h"
#include "BaseInt.h"
#ifndef LINUX
//#include "Spider.h"
#endif

#define MAX_MATR 300
#define MAX_NODE 300
bool GetMatr = false;

void CGroupRef::There_and_Back_np( double dt )
{
#pragma omp single 
	{
		ss_test->solve(flag, nnzv, Arow, Acol, Aval, b_test, X);
	}
}

void CGroupRef::There_and_Back( double dt )
{

	if (flag)
	{
		ss_test->solve_blk( nnzv, Arow, Acol, Aval, b_test, X);
	}
	else
	{
#pragma omp single nowait 
		{
			ss_test->updateValues(Aval, ss_test->matrA);
		}
#pragma omp single 
		{
			ss_test->ipvec(ss_test->DM->pinv, b_test, X, ss_test->nInt);
		}

		ss_test->light_solve_blk(Aval, b_test, X);	
	}
}


void TestMatr ( char * ObjName, int kScalarVariables, int kEq, double * A, double * B );

int CGroupRef::HydroCalc( double dt, bool SetVars )
{
//	ASS(!GetMatr)
  if ( Numb )
  {
    //    GetObjMatr( dt );
#if (OLD_CALC_MODE >> 0 & 1)
    if (!Fwd( dt ))
      return 1;
    if ( !Back( dt ))
      return 2;
    if ( SetVars )
    {
      if ( !SetVar( dt ))
        return 3;
    }

#else
    //#pragma omp parallel num_threads(20)
    {
      There_and_Back( dt );
      SetVar( dt );
    }
#endif
  }
  else
  {
    if ( SetVars )
    {
      if ( !SetVar( dt ))
        return 4;
    }
  }
  return 0;
}


int CGroupRef::SetAllVars( double dt )
  {
  if ( !SetVar( dt ))
    return 3;
  return 0;
  }

void CGroupRef::GetObjMatr( double dt )
{
	GetMatr = true;
#if (!(OLD_CALC_MODE >> 0 & 1))
	int kBad = 0; 
	int Pos_Beg = 0;
	int Pos_End = 0;
	kCurrVars = 0;
	temp_eq = 0;
	int NNZ = nnzv;
	nnzv = -1;
	renum = true;

	int temp_eq = 0;
	#pragma omp parallel for num_threads(20)
#endif
	for ( int nObj = 0; nObj < kObjects; nObj++ )
	{
	#if (!(OLD_CALC_MODE >> 0 & 1))
		int nOpt = OptOrd ? (OptOrd[nObj])  : nObj;   
		CGroupObject & Obj = Objects[nOpt];
	#else
		CGroupObject & Obj = Objects[nObj];
	#endif
		int kScalarVariables = Obj.kLinearVar;
		int kEq =  Obj.kLinearEq;
		if ( kScalarVariables <= 0 )
			continue;
		int N8 = kEq << 3;		//Тоже самое N8 = N * pow(2, 3)
		memset( Obj.A, 0, N8 * kScalarVariables  );
		memset( Obj.B, 0, N8 );
		IBaseModel * pModel = Obj.pObject;
		//
		pModel->pUniModel->SetMatr( &Obj, Obj.PntGroup,  dt, Obj.A, Obj.B );   // A * X = B
		int L = kEq * kScalarVariables;
		for ( int n = 0; n < L; n++ )
		{
			if ( !_finite( Obj.A[n] ))
			{
				SysMSG( "Ошибочное значение матрицы объекта '%s'", Obj.ObjName.Str );
				break;
			}
		}
		L = kEq;
		for ( int n = 0; n < L; n++ )
		{
			if ( !_finite( Obj.B[n] ))
			{
				SysMSG( "Ошибочное значение вектора объекта '%s'", Obj.ObjName.Str );
				break;
			}
		}
		//
		TestMatr ( pModel->ObjName, kScalarVariables, kEq, Obj.A, Obj.B );
	}
#if (!(OLD_CALC_MODE >> 0 & 1))
	if (this->kVar != 0)
	{
		//#pragma omp parallel for num_threads(20) 
		flag = false;
		for ( int nObj = 0; nObj < kObjects; nObj++ )
		{
			int nOpt = OptOrd ? (OptOrd[nObj])  : nObj;   
			CGroupObject & Obj = Objects[nOpt];
			int kScalarVariables = Obj.kLinearVar;
			int kEq =  Obj.kLinearEq;
			int ind = t_eq[nObj];	
			int N_obj = Obj.kLinearEq;
			int kV_obj = Obj.kLinearVar;
			double * A = Obj.A;
			double * B = Obj.B;
			for ( int r = 0; r < N_obj; r++ )//for ( int n = 0; n < kV_obj; n++ )
			{
				for ( int n = 0; n < kV_obj; n++ )
				{
					int Int_N = n;
					int Ext_N = Obj.NumbVarInGroup[n];

					double v = A[(r)*kV_obj+(Int_N)];
					if (v != 0)
					{		
						++nnzv;
						if (((ind + r)  != Arow[nnzv]) || (Ext_N  != Acol[nnzv]))
						{
							flag = true;
							renum = true;
							Arow[nnzv] = (ind + r) ;
							Acol[nnzv] = Ext_N;
						}
						Aval[nnzv] = v;						
					}
				}
				for ( int r = 0; r < N_obj; r++ )							
					b_test[ind + r] = B[r];	
			}
		}
	}
	++nnzv;
#endif
	GetMatr = false;
}


void CGroupRef::Dump( int nErr )
  {
  // char Path[1025];
  // lstrcpy ( Path, GetDumpPath());
  // lstrcat ( Path, "Dump.txt" );
  // void * F = Fopen( Path, "wt" );
  // Fprintf ( F, "Группа: %d\n", Numb );
  // for ( int nObj = 0; nObj < kObjects; nObj++ )
  //   {
  //   int nOpt = OptOrd[nObj];
  //   CGroupObject & O = Objects[nOpt];
  //   Fprintf ( F, "Объект:%s(%s);kV=%d\n", (char*)O.pObject->ObjName, (char*)O.pObject->Model, O.kLinearVar );
  //   if ( nObj == nErr )
  //     Fprintf ( F, "######################################\n");
  //   }
  // Fclose ( F );
  }

bool CGroupRef::Fwd( double dt )
  {
  if ( kScalarVariables <= 0 )
    return true;
  kEQ = 0;
  int MaxkEQ = 0;
  Step = 1;
  StepSave = 0;
  PosInSt = 0;
  memset ( ActiveSet, 0, kScalarVariables );
  memset ( A, 0, kScalarVariables * MaxEQ * 8 );
  memset ( B, 0, MaxkEQ * 8 );
  memset ( Pos_in_CurrVars, 0xFF, kScalarVariables * 4 );
  B[0] = 0.;
  int kBad = 0; 
  int Pos_Beg = 0;
  int Pos_End = 0;
  kCurrVars = 0;
  for ( int nObj = 0; nObj < kObjects; nObj++ )
    {
    int nOpt = OptOrd[nObj];    
    CGroupObject & Obj = Objects[nOpt];
    int kAdd = FormMatr ( dt, nOpt, A + kEQ * kScalarVariables, B + kEQ );
    //////////////////////////////////////////////////////
    kEQ += kAdd;
    if ( kEQ > MaxkEQ )
      MaxkEQ = kEQ;
    while ( Pos_Beg < kScalarVariables && StepVar_Beg[Pos_Beg].Step <= Step )
      {
      if ( StepVar_Beg[Pos_Beg].Step == 0 )
        {
        Pos_Beg++;
        continue;
        }
      int nVar = StepVar_Beg[Pos_Beg++].Var;
      ActiveSet[nVar] = true;
      int Pos = Pos_in_CurrVars[nVar];
      ASS( Pos < 0 )
      Pos_in_CurrVars[nVar] = kCurrVars;
      CurrVars[kCurrVars++] = nVar;
      }
//    PrtA( pObj, true );
// Исключение переменной
      bool WasExclude = false;
      while ( Pos_End < kScalarVariables && StepVar_End[Pos_End].Step <= Step )
        {
        if ( StepVar_End[Pos_End].Step == 0 )
          {
          Pos_End++;
          continue;
          }
        int nVar = StepVar_End[Pos_End++].Var;
        ActiveSet[nVar] = false;
        int n = Pos_in_CurrVars[nVar];
        ASS( CurrVars[n] == nVar )
        CurrVars[n] = -1;
        WasExclude = true;
        if ( !Exclude( nVar ))
          {
          ShowFailed( nVar );
          kBad++;
          }
        }
  // Сжатие списка переменных
    if ( WasExclude )
      {
      int p = 0;
      for ( int n = 0; n < kCurrVars; n++ )
        {
        if ( CurrVars[n] < 0 )
          continue;
        int nVar = CurrVars[n]; 
        Pos_in_CurrVars[nVar] = p;
        CurrVars[p++] = nVar;
        }
      kCurrVars = p;
      }
    Step++;
    }
  if ( kBad )
    ShowErrors = false;
  ASS( kCurrVars == 0 && kEQ == 0 )
  MaxkEQ = 0;
  BegSt[StepSave] = PosInSt;
  return true;
  }

inline void CGroupRef::SetX ( double dt, int x, double Value )
  {
  //if ( pFirstFiltr )
  //  {
  //  CFiltrFlow * pF = pFirstFiltr;
  //  while ( pF )
  //    {
  //    if ( pF->x == x )
  //      {
  //      if ( pF->ActiveSet )
  //        X[x] += dt * pF->K * ( Value - X[x] );
  //      else
  //        X[x] = Value;
  //      return;
  //      }
  //    pF = pF->pNext;
  //    }
  //  }
  X[x] = Value;
  }

bool CGroupRef::Back( double dt )
  {
  if ( StepSave <= 0 )
    return true;
  int Pos = BegSt[StepSave-1];
  int x = pNumb[Pos];
	SetX( dt, x, pVar[Pos] );
  for ( int i = StepSave - 2; i >= 0; i-- )
	  {
	  int Pos = BegSt[i];
	  int x = pNumb[Pos];
//		  X[x] = pVar[Pos];
	  double Xx = pVar[Pos];
//      if ( Trace && kTrace-- > 0 )
//        Fprintf ( Trace, "%4d) %d, %d, B=%7.5lf; ", kTrace, Pos, x, Xx );
	  Pos++;
	  while ( Pos < BegSt[i+1] )
		  {
		  int v = pNumb[Pos];
			Xx -= X[v] * pVar[Pos];
		  Pos++;
		  }
		SetX( dt, x, Xx );
    }
/*
  int nB = 0;
  int nE = kP;
  double MaxPress = 500.;//pHydro->MaxPress;
  for ( int n = nB; n < nE; n++ )
    {
    double P = X[n];
    if ( P > MaxPress )
      {
//      PrtMinMax( n );
      X[n] = MaxPress;
      }
    if ( P <= 0. )
      {
//      PrtMinMax( n );
      X[n] = 1e-4;
      }
    }
*/
/*
// Проверка потоков на Max
  double MaxFlow = 500000.;//pHydro->MaxFlow;
  nB = nE;
  nE = kScalarVariables;
  for ( int n = nB; n < nE; n++ )
    {
    double F = X[n];
    if ( F > MaxFlow )
      {
//      PrtMinMax( n );
      X[n] = MaxFlow;
      }
    if ( F < -MaxFlow )
      {
//      PrtMinMax( n );
      X[n] = -MaxFlow;
      }
    }
*/
	return true;
  }

void * TraceFile = NULL;
int kTrace = 0;

// Исключение переменной nV
bool CGroupRef::Exclude( int nVar_excl )
  {
	if ( kEQ <= 0 )
		return false;
// Максимальные элемент
	double Max = 0.;
	int nEQ = -1;
  double * Aa = A + nVar_excl;
  double * Aa_max = Aa + kEQ * kScalarVariables;
  int i = 0;
  while ( 1 )
		{
    double a = fabs ( *Aa );
		if ( a > Max )
			{
			Max = a;
			nEQ = i;
			}
    i++;
    Aa += kScalarVariables;
    if ( Aa == Aa_max )
      break;
		}
//
	if ( nEQ < 0 || Max < 1e-50 )
    {
	  BegSt[StepSave++] = PosInSt;
//	ASS(PosInSt<MemSave);
    pVar[PosInSt] = 0.011111;
	  pNumb[PosInSt] = nVar_excl;
	  PosInSt++;
	  kEQ--;
		return false;
    }
  int nEQ_N = nEQ * kScalarVariables;
  double * A_nEQ_N = A + nEQ_N;
  double C = *( A_nEQ_N + nVar_excl );
  double * Ae = A_nEQ_N;
  if ( C != 1. )
    {
    C = 1. / C;
    for ( int n = 0; n < kCurrVars; n++ )
      {
      int nVar = CurrVars[n];
      if ( nVar >= 0 )
        Ae[nVar] *= C;
      }
    B[nEQ] *= C; 
    }
// Исключаем из остальных
  double B_eq = B[nEQ];
  double * Ai = A;
  i = 0; 
  while ( i < kEQ )
    {
    if ( i == nEQ )
      {
      i++;
      Ai += kScalarVariables;
	    continue;
      }
    double D = *( Ai + nVar_excl );
    if ( D == 0. )
      {
      i++;
      Ai += kScalarVariables;
	    continue;
      }
    for ( int n = 0; n < kCurrVars; n++ )
      {
      int nVar = CurrVars[n];
      if ( nVar >= 0 )
        Ai[nVar] -= Ae[nVar] * D;
	    }
    B[i] -= B_eq * D;
    i++;
    Ai += kScalarVariables;
    }
  // Выписываем строку
  BegSt[StepSave++] = PosInSt;
//	ASS(PosInSt<MemSave);
  pVar[PosInSt] = B[nEQ];
  pNumb[PosInSt] = nVar_excl;
  PosInSt++;
  Ae = A_nEQ_N;
  for ( int n = 0; n < kCurrVars; n++ )
    {
    int nVar = CurrVars[n];
    if ( nVar < 0 )
      continue;
    double D = Ae[nVar];
    if ( D == 0. )
      continue;
	  pVar[PosInSt] = D;
	  pNumb[PosInSt] = nVar;
	  PosInSt++;
    ASS( PosInSt < MemSave)
    }
// Перепись последней строки на место данной
  int N8 = kScalarVariables * 8;
	if ( kEQ <= 1 )
		{
		kEQ--;
		memset ( A + kEQ * kScalarVariables, 0, N8 );
		B[kEQ] = 0.;
		return true;
		}
	kEQ--;
	if ( nEQ == kEQ )
		{
    memset ( A + kEQ * kScalarVariables, 0, N8 );
		B[kEQ] = 0.;
		return true;
		}
	memcpy ( A + nEQ * kScalarVariables, A + kEQ * kScalarVariables, N8 );
	B[nEQ] = B[kEQ];
//  FINITE(B[nEQ])
	memset ( A + kEQ * kScalarVariables, 0, N8 );
	B[kEQ] = 0.;
	return true;
	}

void CGroupRef::PrtFailed( int nVar )
  {
  char Path[1025];
  lstrcpy ( Path, PROJECT_ROOT);
  lstrcat ( Path, "TEMP/HydroErr.txt" );
  FILE * F;
  if ( fopen_s ( &F, Path, "at" ) != 0 )
    return ;
  fprintf ( F, "######### Группа %d #######################\n", Numb );
  fprintf ( F, "Объектов %d, переменных %d\n",
    kObjects, kScalarVariables );
//
  fprintf ( F, "Ошибка при работе с переменной %d\n", nVar );
  for ( int n = 0; n < kObjects; n++ )
    {
    CGroupObject & Obj = Objects[n];
    for ( int v = 0; v < Obj.kLinearVar; v++ )
      {
      int nObjVar = Obj.NumbVarInGroup[v];
      if ( nObjVar == nVar )
        {
        fprintf ( F, "Объект '%s'\n", Obj.pObject->ObjName.Str );
        break;
        }
      }
    }
  fclose ( F );
  }

void CGroupRef::ShowFailed( int nVar )
  {
  if ( !ShowErrors )
    return;
  SysMSG ( "Ошибка при работе с переменной %d\n", nVar );
  for ( int n = 0; n < kObjects; n++ )
    {
    CGroupObject & Obj = Objects[n];
    for ( int v = 0; v < Obj.kLinearVar; v++ )
      {
      int nObjVar = Obj.NumbVarInGroup[v];
      if ( nObjVar == nVar )
        {
        SysMSG ( "     Объект '%s'\n", Obj.pObject->ObjName.Str );
        break;
        }
      }
    }
  }

void CGroupRef::ObjVars( CGroupObject * pObj )
  {
  int kV_obj = pObj -> kLinearVar;
  int N_obj = pObj -> kLinearEq;// = Числу добавляемых уравнений
  kEQ += N_obj;
  ///////////////////////////////////////
  for ( int n = 0; n < kV_obj; n++ )
    {
//    int Int_N = pObj -> NumbVarInGroup[n];
    int Ext_N = pObj -> NumbVarInGroup[n];
    if ( State_Var[Ext_N] == 0 )
      {
      State_Var[Ext_N] = 1;
      Numb_Var[kScalarVariables++] = Ext_N;
      continue;
      }
    if ( State_Var[Ext_N] == 3 )
      {
      State_Var[Ext_N] = 4;
      Numb_Var[kScalarVariables++] = Ext_N;
      continue;
      }
    }
  }

#define A_obj(r,c) A[(r)*kV_obj+(c)]
#define M_sys(r,c) M[(r)*kScalarVariables+(c)]
#define ind_A_obj(r,c) ((r)*kV_obj+(c))
#define ind_M_sys(r,c) ((r)*kScalarVariables+(c))

int CGroupRef::FormMatr( double dt, int nObj, double * M, double * V )
  {
//  ASS( kObj == kObjects );
  CGroupObject  Obj = Objects[nObj];
  int N_obj = Obj.kLinearEq;
  if ( N_obj <= 0 )
    return 0;
  memset ( M, 0, N_obj * kScalarVariables * 8 );
  Obj.IsMatrInd = false; //!!!!!!!!!!!
  if ( Obj.IsMatrInd )
    {
    int l_A = Obj.l_A;
    IBaseModel * pModel = Obj.pObject;
    pModel -> ModelFlags |= MODEL_IN_USE;
    double * A = Obj.A;
    double * B = Obj.B;
    bool All_0 = true;
    for ( int n = 0; n < l_A; n++ )
      {
      double v = A[n];
      if ( v != 0. )
        All_0 = false;
      if ( !_finite(v))
        {
        LogMsg( "Ошибка в матрице P модели %s\n", Obj.pObject->ObjName.Str );
        v = 0.;
        }
      int N = Obj.Ind_A[n];
      M[N] += v; 
      }
    for ( int r = 0; r < N_obj; r++ )
      {
      double v = B[r];
      if ( !_finite(v))
        {
        LogMsg( "Ошибка в векторе V[%d] модели %s\n", r, Obj.pObject->ObjName.Str );
        v = 0.;
        }
      V[r] = v;
      }
    return N_obj;
    }
  //
  int kV_obj = Obj.kLinearVar;
  //
  ///////////////////////////////////////
//  pObj -> pObject->ModelFlags |= MODEL_IN_USE;
  // Вставка матрицу
  double * A = Obj.A;
  double * B = Obj.B;
  bool All_0 = true;
  int l_A = 0;
  for ( int n = 0; n < kV_obj; n++ )
    {
    int Int_N = n;//Obj.InternalNumbVar[n];
    int Ext_N = Obj.NumbVarInGroup[n];
    for ( int r = 0; r < N_obj; r++ )
      {
      double v = A_obj(r,Int_N);
      if ( v != 0. )
        All_0 = false;
      if ( !_finite(v))
        {
        LogMsg( "Ошибка в матрице P[%d,%d] модели %s\n", r, n, Obj.pObject->ObjName.Str );
        v = 0.;
        }
      M_sys(r,Ext_N) += v;
      Obj.Ind_A[ind_A_obj(r,Int_N)] = ind_M_sys(r,Ext_N);
      l_A++; 
      }
    }
  ASS ( l_A <= MAX_MATR )
  Obj.l_A = l_A;
  if ( All_0 )
    {
    LogMsg( "Ошибка в матрицах модели %s - все 0\n", Obj.pObject->ObjName.Str );
    ASS(FALSE)
    }
  for ( int r = 0; r < N_obj; r++ )
    {
    All_0 = true;
    for ( int n = 0; n < kV_obj; n++ )
      {
      int Int_N = n;//Obj.InternalNumbVar[n];
      if ( A_obj(r,Int_N) != 0. )
        {
        All_0 = false;
        break;
        }
      }
    if ( All_0 )
      {
      LogMsg( "Ошибка в матрицах модели %s(%s) - нулевая строка\n", 
              Obj.pObject->ObjName.Str, Obj.pObject->Model.Str );
      ASS(FALSE)
      }
    }
  for ( int r = 0; r < N_obj; r++ )
    {
    double v = B[r];
    if ( !_finite(v))
      {
      LogMsg( "Ошибка в векторе V[%d] модели %s\n", r, Obj.pObject->ObjName.Str );
      v = 0.;
      }
    V[r] = v;
    }
  Obj.IsMatrInd = true;
  return N_obj;
  }


bool CGroupRef::SetVar( double dt )
{

#if (!(OLD_CALC_MODE >> 0 & 1))
	  if (this->kVar != 0)
	  {
#pragma omp for nowait
		  for ( int nObj = 0; nObj < kObjects; nObj++ )
		  {
			  SetVar( dt, nObj );
		  }
	  }
	  else
#endif
		  for ( int nObj = 0; nObj < kObjects; nObj++ )
			  SetVar( dt, nObj );


	  return true;
}

int CGroupRef::SetVar( double dt, int nObj )
  {
  CGroupObject & Obj = Objects[nObj];
// Результат в X
  double V_obj[MAX_NODE*2];
  CBaseStruct * Var_obj[MAX_NODE];
  CLEAR(Var_obj)
  ASS( Obj.kLinearVar <= MAX_NODE )
// Скалярные переменные
  for ( int n = 0; n < Obj.kLinearVar; n++ )
    {
    int N = Obj.NumbVarInGroup[n];
    *Obj.AddrVar[n] = V_obj[n] = X[N];
    }
  for ( int n = 0; n < Obj.kPoints; n++ )
    {
    if ( Obj.MainPool )
      {
      Var_obj[n] = Obj.pPoints[n]->pVar;
      }
    else
      {
      int N = Obj.pPoints[n]->NumbInGroup;
      Var_obj[N] = Obj.pPoints[n]->pVar;
      }
    }
  //int p = 0;
  //for ( int n = 0; n < Obj.kPoints; n++ )
  //  {
  //  p += Var_obj[n]->SetVarValue( V_obj + p );
  //  }
  IBaseModel * pModel = Obj.pObject;
  Obj.pGroup = this;
  // if ( Obj.PntGroup >= 40000 )
  //   {
  //   for ( int n = 0; n < Obj.kPoints; n++ )
  //     {
  //     CSignalData * pSig = (CSignalData*)Obj.pPoints[n]->pVar;
  //     if ( pSig->pSignal )
  //       {
  //       if ( Obj.pPoints[n]->IO == '-' )
  //         pSig->Signal = *(pSig->pSignal);
  //       else
  //         *(pSig->pSignal) = pSig->Signal;
  //       }
  //     }
  //  return 0;
  //  }
  if ( pModel->pUniModel == NULL )
    return 0;
  pModel->pUniModel->SetVar( &Obj, Obj.PntGroup, dt, V_obj,	Var_obj );
//  if ( pModel ->pSpider )
//    pModel ->pSpider->SetVar( &Obj, Obj.PntGroup, dt, V_obj,	Var_obj );
  return 0;
  }

#define MA(i,j) A[(i)*kVar+(j)]


void TestMatr ( char * ObjName, int kVar, int kEq, double * A, double * B )
  {
/*
  for ( int r = 0; r < kEq; r++ )
    {
    for ( int c = 0; c < kVar; c++ )
      {
			double M = MA(r,c);
      FINITE(M)
      }
    }
*/
  return;
  }
