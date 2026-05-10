#include "stdafx.h"
#include "CommProc.h"
#include "Err.h"
#include <stdlib.h>
#define ALL_DATA 
#include "Data.h"

const int MAX_BASE_STRUCT = 65536;

void LoadRes();

CBaseStruct * BaseStruct[MAX_BASE_STRUCT]; 
int NumbStruct[MAX_BASE_STRUCT]; 
int kBaseStruct = 0;

void GetAllStruct( int * kStruct, CBaseStruct *** ppAllStruct )
  {
  *kStruct = kBaseStruct;
  *ppAllStruct = BaseStruct;
  }

struct CBaseStructKey
  {
  int Type;
  char * Name;
  int Numb; 
  };
CBaseStructKey Key;

void PrtVars( void * F ) 
  {
  // Fprintf ( F, "############### Структуры \n");
  // for ( int n = 1; n < kBaseStruct; n++ )
  //   {
  //   int N = NumbStruct[n];
  //   CBaseStruct * pS = BaseStruct[N];
  //   Fprintf ( F, "%s;%d;%d\n", pS->Name.Str, pS->Type, pS->ID );
  //   }
  }

int CompStruct( int n )
  {
  int N = NumbStruct[n];
  CBaseStruct * pS = BaseStruct[N];
  if ( Key.Type < pS -> Type )
    return -1;
  if ( Key.Type > pS -> Type )
    return 1;
  int Res = lstrcmp ( Key.Name, pS->Name );
  if ( Res )
    return Res;
  if ( Key.Numb < pS -> Numb )
    return -1;
  if ( Key.Numb > pS -> Numb )
    return 1;
  return 0;
  }

int GetPos( int & Pos )  // 0 = найден, -1 - перед Pos, 1 - за Pos
  {
  if ( kBaseStruct <= 1 )
    {
    kBaseStruct = 1;
    Pos = 0;  // Вставить в 1 ( за 0 )
    return 1; 
    }
  if ( kBaseStruct == 2 )
    {
    int Res = CompStruct( 1 );
    Pos = 1;
    return Res;
    }
  if ( kBaseStruct == 3 )
    {
    int Res = CompStruct( 1 );
    Pos = 1;
    if ( Res < 0 )
      return -1;
    Res = CompStruct( 2 );
    Pos = 2;
    return Res;
    }
/////////////////////////////////////////
  int nb = 1;
  int ne = kBaseStruct - 1;
  if ( kBaseStruct >= 7 )
    KKK();
  while ( ne - nb > 1 )
    {
    int n = ( nb + ne ) >> 1;
    int Res = CompStruct( n );
    if ( Res == 0 )
      {
      Pos = n;
      return 0;
      }
    if ( Res < 0 )
      {
      if ( n - nb <= 1 )
        {
        Pos = n - 1;
        Res = CompStruct( Pos );
        return Res;
        }
      ne = n;
      }
    else
      {
      if ( ne - n <= 1 )
        {
        Pos = n + 1;
        Res = CompStruct( Pos );
        return Res;
        }
      nb = n;
      }
    }
  ASS(0)
  return 0;
  }


// Получение ID и подготовка массивов 
int Get_Pos_Struct ( char * Name, int Type, int Numb ) // Pos > 0 найден, < 0 новый
  {
  ASS( Name[0]!=0 )
  Key.Name = Name;
  Key.Type = Type;
  Key.Numb = Numb;
  int Pos;
  int Res = GetPos( Pos );
  if ( Res == 0 )
    return Pos;
  ASS ( kBaseStruct < MAX_BASE_STRUCT );
  if ( Res < 0 )
    {
    // Сдвиг с Pos включительно
    int L = kBaseStruct - Pos;
    memmove( NumbStruct + Pos + 1,  NumbStruct + Pos, L << 2 );
    NumbStruct[Pos] = kBaseStruct;
    return -Pos; 
    }
  else
    {
    // Сдвиг за Pos включительно
    int Pos1 = Pos + 1;
    int L = kBaseStruct - Pos1;
    if ( L > 0 )
      memmove( NumbStruct + Pos1 + 1,  NumbStruct + Pos1, L << 2 );
    NumbStruct[Pos1] = kBaseStruct;
    return -Pos1; 
    }
  return 0;
  }

CBaseStruct * Create ( char * Name, int Type, int Numb )
  {
  int Pos = Get_Pos_Struct ( Name, Type, Numb ); // Pos > 0 найден, < 0 новый
  if ( Pos > 0 )
    {
    int N = NumbStruct[Pos];
    return BaseStruct[N];
    }
  //  Pos < 0
  int N = -Pos;
  ASS( NumbStruct[N] == kBaseStruct )
  if ( kBaseStruct >= 13228 )
    KKK();
  switch ( Type )
    {
    case CComp::TypeID:
      {
      CComp * pComp = new CComp( Name, Numb );
      pComp->ID = kBaseStruct;
      BaseStruct[kBaseStruct++] = pComp;
      return pComp;
      }
    case CFlow::TypeID:
      {
      CFlow * pFlow = new CFlow( Name, Numb );
      pFlow->ID = kBaseStruct;
      BaseStruct[kBaseStruct++] = pFlow;
      return pFlow;
      }
    case CPortion::TypeID:
      {
      CPortion * pP = new CPortion( Name, Numb );
      pP->ID = kBaseStruct;
      BaseStruct[kBaseStruct++] = pP;
      return pP;
      }
    case CSignalData::TypeID:
      {
      CSignalData * pSignal = new CSignalData( Name, Numb );
      pSignal->ID = kBaseStruct;
      BaseStruct[kBaseStruct++] = pSignal;
      return pSignal;
      }
    case CContact::TypeID:
      {
      CContact * pContact = new CContact( Name, Numb );
      pContact->ID = kBaseStruct;
      BaseStruct[kBaseStruct++] = pContact;
      return pContact;
      }
    }
  ASS(0)
  return NULL;
  }

int Get_ID_Data ( char * Name, int Type )
  {
// Безличные Numb = 0
  CBaseStruct * pBase = Create ( Name, Type, 0 );
  return pBase->ID;
  }

/////////////////////////////////////////////////////////////////////////////////////////////////
CComp * Get_Comp ( int ID )
  {
//  ASS(ID >= 1 && ID < kBaseStruct )
  if (ID < 1 || ID >= kBaseStruct )
    return NULL;
  CBaseStruct * pS = BaseStruct[ID];
  ASS(pS->Type == CComp::TypeID);
  return (CComp*)pS;
  }

CFlow * Get_Flow ( int ID )
  {
//  ASS(ID >= 1 && ID < kBaseStruct )
  if (ID < 1 || ID >= kBaseStruct )
    return NULL;
  CBaseStruct * pS = BaseStruct[ID];
  ASS(pS->Type == CFlow::TypeID);
  return (CFlow*)pS;
  }

///////////////////////////////////////////////////////////////////
CComp * Get_Comp ( char * Name )
	{
	ASS(Name[0]!=0)
  Key.Name = Name;
  Key.Type = CComp::TypeID;
  Key.Numb = 0;
  int Pos;
  int Res = GetPos( Pos );
  if ( Res == 0 )
    {
    int N = NumbStruct[Pos];
    return (CComp*)BaseStruct[N];
    }
	return NULL;
	}

double Cmin = 1e-9;
char Name[256];
int Comp;
bool Is[K_GAS];
double C[K_GAS];
void NormComp()
	{
  for ( int n = 1; n < kBaseStruct; n++ )
    {
    if ( BaseStruct[n] ->Type != CComp::TypeID )
      continue;
    CComp * pComp = (CComp*)BaseStruct[n];
		bool * IsComp = pComp->IsComp;
		double * Cmol = pComp->Cmol;
		double S = 0.;
		for ( int c = 0; c < LAST_GAS; c++ )
			{
			if ( !_finite(Cmol[c]))
				{
				strcpy_s( Name, pComp->Name );
				Comp = c;
				memcpy ( Is, IsComp, sizeof ( Is ));
				memcpy ( C, Cmol, sizeof ( C ));
				Cmol[c] = 0.;
//				ASS(FALSE)
				}
			if ( !IsComp[c] )
				{
				Cmol[c] = 0.;
				continue;
				}
			if ( Cmol[c] < Cmin )
				{
				Cmol[c] = 0.;
				IsComp[c] = false;
				continue;
				}
			S += Cmol[c];
			}
		if ( S < 1e-3 )
			{
			memset( IsComp, 0, sizeof ( pComp->IsComp ));
			memset( Cmol, 0, sizeof ( pComp->Cmol ));
			IsComp[G_N2] = true;
			Cmol[G_N2] = 1.;
			S = 1.;
			}
		if ( fabs ( S - 1. ) < 1e-6 )
			continue;
		double N = 1. / S;
		for ( int c = 0; c < LAST_GAS; c++ )
			{
			if ( IsComp[c] )
				Cmol[c] *= N;
			}
		}
	}

void CComp::Summ ( double W_1, CComp * pComp_1, double W_2, CComp * pComp_2 )
	{
	memset ( Cmol, 0, sizeof ( Cmol ));
	memset ( IsComp, 0, sizeof ( IsComp ));
	bool * IsComp_1 = pComp_1 -> IsComp;
	double * Cmol_1 = pComp_1 -> Cmol;
	bool * IsComp_2 = pComp_2 -> IsComp;
	double * Cmol_2 = pComp_2 -> Cmol;
	for ( int c = 0; c < LAST_GAS; c++ )
		{
		if ( IsComp_1[c] )
			{
			IsComp[c] = true;
			Cmol[c] = W_1 * Cmol_1[c];
			}
		if ( IsComp_2[c] )
			{
			IsComp[c] = true;
			Cmol[c] += W_2 * Cmol_2[c];
			}
		}
	double S = 0.;
	for ( int c = 0; c < LAST_GAS; c++ )
		{
		if ( IsComp[c] )
			S += Cmol[c];
		}
  if ( S > 1e-9 )
    {
	  double N = 1. / S;
	  for ( int c = 0; c < LAST_GAS; c++ )
		  {
		  if ( IsComp[c] )
			  Cmol[c] *= N;
		  }
    }
	}

double CComp::Norm ( double Eps )
	{
  double S = 0.;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      {
      if( Cmol[n] < 0.)
        Cmol[n] = 0.;
      S += Cmol[n];
      }
    }
  if ( S <= Eps ) 
    {
    return S;
    }
  if ( fabs ( S - 1. ) < 1e-8 )
    {
    return S;
    }
  double Si = 1. / S;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      {
      Cmol[n] *= Si;
      }
    }
  return S; 
	}

void NewComp( int * ID, CComp ** pComp, char * Name, ... )
	{
	va_list Parm;
	char CompName[256];
	va_start ( Parm, Name );
    vsprintf(  CompName, Name, Parm );
	*ID = Get_ID_Data ( CompName, CComp::TypeID );
	*pComp = Get_Comp ( *ID );
	}

CComp * NewComp( char * Name, int Numb )
  {
  CBaseStruct * pBase = Create ( Name, CComp::TypeID, Numb );
  return (CComp*)pBase;
  }


void SetStdComp()
	{
	ID_Air = Get_ID_Data ( "Воздух", CComp::TypeID );
	pAir = Get_Comp ( ID_Air );
	CLEAR(pAir ->IsComp)
	CLEAR(pAir ->Cmol)
//
  if ( G_N2 >= 0 )
    {
    pAir ->IsComp[G_N2] = true;
    pAir ->Cmol[G_N2] = 0.79;
    }
  if ( G_O2 >= 0 )
    {
	  pAir ->IsComp[G_O2] = true;
	  pAir ->Cmol[G_O2] = 0.21;
    }
//
//
  if ( G_N2 >= 0 )
    {
  	ID_N2 = Get_ID_Data ( "Азот", CComp::TypeID );
	  pN2 = Get_Comp ( ID_N2 );
	  CLEAR(pN2 ->IsComp)
	  CLEAR(pN2 ->Cmol)
	  pN2 ->IsComp[G_N2] = true;
	  pN2 ->Cmol[G_N2] = 1.;
    }
//
	ID_Water = Get_ID_Data ( "Вода", CComp::TypeID );
	pWater = Get_Comp ( ID_Water );
	CLEAR(pWater ->IsComp)
	CLEAR(pWater ->Cmol)
  if ( G_H2O >= 0 )
    {
    pWater->IsComp[G_H2O] = true;
    pWater->Cmol[G_H2O] = 1.;
    }
//
  pAirFlow->Flow_mol = 0.;
  pAirFlow->P = 1.;
  pAirFlow->T = T_air;
  pAirFlow->To_m3 = M3;
  pAirFlow->To_kg = Mw ( pAir );
  pAirFlow->h_Flow = ::H_IG ( T_air, pAir );
  pAirFlow->ID_COMP = ID_Air;
  pAirFlow->pComp = pAir;
  //			pAirFlow->TYPE_COMP = MASK_N2 | MASK_O2;
  pN2Flow->Flow_mol = 0.;
  pN2Flow->P = 1.;
  pN2Flow->T = T_air;
  pN2Flow->To_m3 = M3;
  pN2Flow->To_kg = Mw ( pN2 );
  pN2Flow->h_Flow = ::H_IG ( T_air, pN2 );
  pN2Flow->ID_COMP = ID_N2;
  pN2Flow->pComp = pN2;
  //			pN2Flow->TYPE_COMP = MASK_N2;
  //
  pWaterFlow->Flow_mol = 0.;
  pWaterFlow->P = 5.;
  pWaterFlow->T = 20.;
  pWaterFlow->To_m3 = Mw_H2O * 1e-3;
  pWaterFlow->To_kg = Mw_H2O;
  pWaterFlow->h_Flow = pGas[G_H2O] -> H_l ( 20. );
  pWaterFlow->ID_COMP = ID_Water;
  pWaterFlow->pComp = pWater;
	}


bool CComp::Read ( char * Name )
	{
  memset ( IsComp, 0, sizeof ( IsComp ));
  memset ( Cmol, 0, sizeof ( Cmol ));
  char Path[1024];
  sprintf ( Path, "%sDATA\\СОСТАВЫ\\%s.txt", PROJECT_ROOT, Name );
  FILE * F;
	if ( fopen_s ( &F, Path, "rt" ) != 0 )
		{
		MsgErr ( NULL, "Ошибка открытия файла составов %s.txt", Name );
		return false;
		}
  char Str[256];
	bool Mass = false;
  pSpecial = NULL;
	while ( fgets ( Str, 256, F ))
		{
    if ( Str[0] == '%' )
      continue;
		if ( Str[0] == '#' )
			{
			if ( strncmp ( Str + 1, "моль", 4 ) == 0 )
				{
				Mass = false;
				continue;
				}
			if ( strncmp ( Str + 1, "масс", 4 ) == 0 )
				{
				Mass = true;
				continue;
				}
			ASS(FALSE)
      continue;
			}
    int L = lstrlen ( Str );
    if ( L < 3 )
      continue;
		char * P0 = strchr ( Str, ';' );
		if ( P0 == NULL )
			{
			MsgErr ( NULL, "Ошибочная структура файла составов %s.txt. \n"
				       "Ошибка в строке '%s'", Name, Str );
		  fclose ( F );
			return false;
			}
    *P0 = 0;
// Компонента
		double C = atof ( P0 + 1 );
    int nC = FindGasComp ( Str );
    if ( nC < 0 )
      nC = AltFindGasComp ( Str );
		if ( nC < 0 )
			{
			if ( C > 1e-8 )
				{
				MsgErr ( NULL, "Не найдена компонента в файле %s.txt. \n"
				       "Компонента '%s',C=%12.5lf", Name, Str, C );
				}
			continue;
			}
		IsComp[nC] = true;
		Cmol[nC] = C;
//
    char * Parm = strchr ( P0 + 1, ';' );
    if ( Parm )
      {
      CSpecialData * pSp = pGas[nC]->GetSpecialData ( Parm + 1 );
      if ( pSp )
        {
        pSp->Name = Name;
        pSpecial = pSp;
        }
      }
		}
  fclose ( F );
	if ( Mass )
		{
		for ( int n = 0; n < LAST_GAS; n++ )
			{
			if ( IsComp[n] )
				Cmol[n] /= MwGasComp[n];
			}
		}
	double N = Norm( 0. );
	if ( N < 1e-8 )
		{
		MsgErr ( NULL, "Отсутствуют данные в файле составов %s.txt.", Name );
		return false;
		}
	return true;
	}

bool CComp::Write ( char * Name )
  {
  char Path[1024];
  sprintf ( Path, "%sDATA\\СОСТАВЫ\\%s.txt", PROJECT_ROOT, Name );
//  TakeOffReadOnly( Path );
  FILE * F;
  if ( fopen_s ( &F, Path, "wt" ) != 0 )
    {
    MsgErr ( NULL, "Ошибка открытия файла составов %s.txt", Name );
    return false;
    }
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp[c] )
      fprintf ( F, "%s;%23.16le;%.0lf\n",	pGas[c]->Name, Cmol[c], pGas[c]->Tb );
    }
  fclose( F );
  return true;
  }

void GetCompData( CComp * pComp, double & C, double & Ckg )
  {
  double S = 0.;
  double S_kg = 0.;
  bool * IsComp = pComp->IsComp;
  double * Cmol = pComp->Cmol;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( !IsComp[c] )
      continue;
    S += Cmol[c];
    S_kg += Cmol[c] * pGas[c]->Mw;
    } 
  C = S;
  Ckg = S_kg;
  }

