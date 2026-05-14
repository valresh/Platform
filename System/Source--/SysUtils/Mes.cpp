#include "CommProc.h"

static char * AllMes = NULL;
static double * MesCoef = NULL;
static char ** MesTxt = NULL;
static int  * MesLen = NULL;
static int  * MesMes = NULL;
static int kMes = 0;
static int lMes = 0;

int InitMes( )
  {
  char * Loc = setlocale( LC_CTYPE, "en_US" );
  Loc = setlocale( LC_ALL, NULL );
  char Path[1024];
  sprintf_s ( Path, 1024, "%sINI/Размерности.csv", SYSTEM_PATH );
  FILE * F = fopen ( Path, "rt" );
  if ( F == NULL )
    return 1;
  int L = 0;
  int K = 0;
  char Str[256];
  fgets ( Str, 255, F, false );
  while ( fgets ( Str, 255, F, false ))
    {
    char * P = strchr( Str, ';' );
    if ( P == NULL )
      break;
    *P = 0;
    L += lstrlen ( Str ) + 1;
    K++;
    }
  fclose ( F );
/////////////////////////////////////////////
  AllMes = NewArr(char,(L+64));
  MesCoef = NewArr(double,(K+16));
  MesTxt = NewArr(char *,(K+16));
  MesLen = NewArr(int,(K+16));
  MesMes = NewArr(int,(K+16));
  L = 0;
  lstrcpy ( AllMes + L, ";" );
  L++;
  kMes = 0;
  MesTxt[kMes] = "";
  MesMes[kMes] = 0;
  MesCoef[kMes++] = 1.;
  F = fopen ( Path, "rt" );
  fgets ( Str, 255, F, false );
  while ( fgets ( Str, 255, F, false ))
    {
    char * Name = Str;
    char * P =  strchr( Name, ';' );
    if ( P == NULL )
      continue;
    *P = 0;
    if ( Name[0] == 0 )
      continue;
//
    char * Mes = P + 1;
    P =  strchr( Mes, ';' );
    if ( P == NULL )
      continue;
    *P = 0;
    ASS(Mes[0])
    if ( Mes[0] == 0 )
      continue;
    char * Scale = P + 1;
    P =  strchr( Scale, ';' );
    if ( P )
      *P = 0;
    ASS(Scale[0])
    if ( Scale[0] == 0 )
      continue;
//
    lstrcpy ( AllMes + L, Name );
    MesTxt[kMes] = AllMes + L;
    MesLen[kMes] = lstrlen ( Name );
    L += MesLen[kMes];
    lstrcpy ( AllMes + L, ";" );
    L++;
    MesMes[kMes] = atoi( Mes );
    MesCoef[kMes] = atof ( Scale );
    kMes++;
    }
  lMes = L;
  AllMes[L] = 0;
  fclose ( F );
  return 0;
  }

int k_Mes( )
  {
  return kMes;
  }

char * MesNames( )
  {
  return AllMes;
  }

double Scale( int Mes )
  {
  for ( int n = 1; n < kMes; n++ )
    {
    if ( MesMes[n] == Mes )
      return MesCoef[n];
    }
  return 1.;
  }

char * MesName( int Mes )
  {
  for ( int n = 1; n < kMes; n++ )
    {
    if ( MesMes[n] == Mes )
      return MesTxt[n];
    }
  return "";
  }

int GetEngUnits( char * Mes )
  {
  int L = strlen( Mes );
  for ( int n = 1; n < kMes; n++ )
    {
    if ( MesLen[n] == L && strncmp ( Mes, MesTxt[n], L ) == 0 )
      return MesMes[n];
    }
//  Fsend( "ErrMes.txt", "%s\n", Mes );
  return -1;
  }

