#include "stdafx.h"
#if 0
#define ALL_DATA 
#include "Data.h"
#include "CommProc.h"
#include "Solid.h"
#include "Err.h"
#include "Chem.h"
#include "ChemPET.h"
#include <stdlib.h>


CSolidComp * CSolid::pComp[CSolid_W::kSolidComp];

int FindSolidComp( const char * Name )
  {
  for ( int c = 0; c < CSolid::_LAST; c++ )
    {
    if ( lstrcmp ( Name, CSolid::pComp[c]->Name ) == 0 )
      return c;  
    if ( CSolid::pComp[c]->Alt_Name && lstrcmp ( Name, CSolid::pComp[c]->Alt_Name ) == 0 )
      return c;  
    }
  return -1;
  }

CSolid::CSolid() : CFlow_Ext()
  {
  Type = Solid;
  C = 0.;
  T = 15.;
  CLEAR(Cmol)
  }

int CSolid::SaveData( )
  {
  CSolid_W * pW = static_cast<CSolid_W*>( this );
  ASS(pSRFile->WD( pW, sizeof ( CSolid_W )));
  return 0;
  }

void CSolid::SaveData( const  char * SolidName, CSolid * pSolid )
  {
  if ( pSolid == NULL )
    return;
  ASS(pSRFile->WStruct ( (char*)SolidName, sizeof ( CSolid_W ), static_cast<CSolid_W*>(pSolid)));
  }

bool CSolid::RestData( const  char * SolidName, const  char * StrName, CSolid ** pSolid )
  {
  if ( lstrcmp ( SolidName, StrName ) != 0 )
    return false;
  if ( *pSolid == NULL )
    *pSolid = new CSolid;
  ASS( pSRFile->RStruct( sizeof ( CSolid_W ), static_cast<CSolid_W*>(*pSolid)));
  return true;
  }

int CSolid::RestData( )
  {
  CSolid_W * pW = static_cast<CSolid_W*>( this );
  ASS(pSRFile->RD( pW, sizeof ( CSolid_W )));
  return 0;
  }

bool CSolid::Set_Cmol( double * _Cmol )
  {
  memmove ( Cmol, _Cmol, sizeof ( Cmol ));
  Mw = To_kg();
  if ( Mw < 50. )
    return false;
  return true;
  }

int CSolid::Copy( CFlow_Ext * pExt )
  {
  if ( pExt == NULL )
    {
    C = 0.;
    T = T_air;
    Mw = 0.;
    memset( Cmol, 0, kSolidComp * 8 );
    return -1;
    }
  ASS(pExt->Type == Solid )
  CSolid * pS = (CSolid *)pExt;
  C = pS->C;
  T = pS->T;
  Mw = pS->Mw;
  memmove ( Cmol, pS->Cmol, sizeof ( Cmol ));
  return 0;
  }

CFlow_Ext * CreateExt_Solid ( )
  {
  CSolid * pSolid = new CSolid;
  ASS(pSolid)
  return pSolid;
  }


int SizeExt_Solid ( )
  {
  return sizeof ( CSolid_W ) + 4;
  }

double CSolid::Ro( double TC )
  {
  return To_kg( )/To_m3( TC );
  }

double CSolid::To_kg( )
  {
  double S = 0.;
  double M = 0.;
  for ( int c = 0; c < _LAST; c++ )
    {
    S += Cmol[c];
    M += Cmol[c] * pComp[c]->Mw;
    }
  if ( S < 1e-5 )
    return 0.;
  Mw = M / S;
  return Mw;
  }

double CSolid::Cp( double TC )
  {
  double S = 0.;
  double C = 0.;
  for ( int c = 0; c < _LAST; c++ )
    {
    S += Cmol[c];
    C += Cmol[c] * pComp[c]->Cp( TC );
    }
  if ( S < 1e-5 )
    return 0.;
  return C / S;
  }

double CSolid::To_m3( double TC )
  {
  double S = 0.;
  double M = 0.;
  double To = 0.;
  for ( int c = 0; c < _LAST; c++ )
    {
    if ( Cmol[c] <= 0. )
      continue;
    To = pComp[c]->To_m3( TC );
    S += Cmol[c];
    M += Cmol[c] * To;
    }
  if ( S < 1e-5 )
    return 0.;
  return M / S;
  }

double CSolid::Norm( )
  {
  double S = 0.;
  for ( int c = 0; c < _LAST; c++ )
    S += Cmol[c];
  if ( S < 1e-5 )
    return 0.;
  if ( fabs ( S - 1. ) < 1e-6 )
    return 1.;
  double N = 1. / S;
  for ( int c = 0; c < _LAST; c++ )
    Cmol[c] *= N;
  return S;
  }

bool CSolid::Read ( char * CompName )
  {
  memset ( Cmol, 0, sizeof ( Cmol ));
  char Path[1024];
  sprintf_s ( Path, 1024, "%sDATA\\СОСТАВЫ\\%s.txt", PROJECT_ROOT, CompName );
  FILE * F;
	if ( fopen_s ( &F, Path, "rt" ) != 0 )
		{
		MsgErr ( NULL, "Ошибка открытия файла составов %s.txt", CompName );
		return false;
		}
  char Str[256];
	bool Mass = false;
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
				       "Ошибка в строке '%s'", CompName, Str );
		  fclose ( F );
			return false;
			}
    *P0 = 0;
// Компонента
		double C = atof ( P0 + 1 );
    int nC = FindSolidComp( Str );
		if ( nC < 0 )
			{
			if ( C > 1e-8 )
				{
				MsgErr ( NULL, "Не найдена компонента в файле %s.txt. \n"
				       "Компонента '%s',C=%12.5lf", CompName, Str, C );
				}
			continue;
			}
		Cmol[nC] = C;
		}
  fclose ( F );
	if ( Mass )
		{
		for ( int n = 0; n < _LAST; n++ )
			{
		  Cmol[n] /= pComp[n]->Mw;
			}
		}
  double N = 0.;
  for ( int n = 0; n < _LAST; n++ )
    N += Cmol[n];
	if ( N < 1e-8 )
		{
		MsgErr ( NULL, "Отсутствуют данные в файле составов %s.txt.", CompName );
		return false;
		}
  N = 1. / N;
  Mw = 0.;
  for ( int n = 0; n < _LAST; n++ )
    {
    Cmol[n] *= N;
    Mw += Cmol[n] * pComp[n]->Mw;
  }
	return true;
  }
#endif
