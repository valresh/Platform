#include "stdafx.h"
#define DLL_Chem
#include "Chem.h"


//struct IN_DLL CСompList : public IFreeParam
//  {
//  };
#if 0

int IN_DLL FindGasComp ( char * Name );
int IN_DLL AltFindGasComp ( char * Name );
int IN_DLL GetGasID( char * Name );
int IN_DLL GetSolidID( char * Name );
int IN_DLL GetVirtID( char * Name );

CСompList::CСompList( )//IBaseModel * _pMain )
  {
  N = -1;
  ID = -1;
//  pMain = _pMain;
  }

void CСompList::operator = ( int _ID ) 
  {
  N = -1;
  ID = -1;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( ID_GasComp[n] == _ID )
      {
      N = n;
      ID = _ID;
      return;
      }
    }
  }

bool CompName ( int Level, int lN, const char * Name, int lS, char * S )
  {
	if ( Level > 50 )
		return true;
	if ( lN == 0 && lS == 0 )
		return true;
	if ( lS == 1 && S[0] == '*' )
		return true;
	if ( lN > 0 && lS == 0 )
		return false;
	if ( S[0] == '*' )
	  {
		for ( int n = 0; n < lN; n++ )
		  {
			if ( CompName ( Level + 1, lN - n, Name + n, lS - 1, S + 1 ))
				return true;
		  }
		return false;
	  }
	if ( S[0] == '?' )
	  {
		return CompName ( Level + 1, lN - 1, Name + 1, lS - 1, S + 1 );
	  }
  char SS[2];
  SS[0] = S[0];
  SS[1] = 0;
  char NN[2];
  NN[0] = Name[0];
  NN[1] = 0;
	if ( strcmpi ( SS, NN ))
		return false;
	return CompName ( Level + 1, lN - 1, Name + 1, lS - 1, S + 1 );
  }

bool CСompList::SetValue ( const char * Value )
  {
  bool Numb = true;
  const char * P = Value;
  while ( *P )
    {
    if ( *P < '0' || *P > '9' )
      {
      Numb = false;
      break;
      }
    P++;
    }
  if ( Numb )
    {
    ID = atoi ( Value );
    if ( ID < 0 )
      ID = 0;
    if ( ID >= LAST_GAS )
      ID = LAST_GAS - 1;
    N = N_GasComp[ID];
    return true;
    }
  int lV = lstrlen ( Value );
  int Level = 0;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( CompName ( Level, lstrlen ( NameGasComp[n] ), NameGasComp[n], lstrlen ( Value ), (char*)Value ))
      {
      ID = n;
      N = N_GasComp[ID];
      return true;
      }
    }
  return false;
  }

void CСompList::ChangeVal( double Delta )
  {
  double V = Delta - 1.;
  if ( V < -0.05 )
    {
    N -= 10;
    if ( N < 0 )
      N = 0;
    ID = ID_GasComp[N];
    return;
    }
  if ( V < 0. )
    {
    N--;
    if ( N < 0 )
      N = 0;
    ID = ID_GasComp[N];
    return;
    }
  if ( V > 0.05 )
    {
    N += 10;
    if ( N >= LAST_GAS )
      N = LAST_GAS - 1;
    ID = ID_GasComp[N];
    return;
    }
  if ( V > 0. )
    {
    N++;
    if ( N >= LAST_GAS )
      N = LAST_GAS - 1;
    ID = ID_GasComp[N];
    return;
    }
  }

void CСompList::Out ( char Txt[256] )
  {
  if ( ID < 0 || ID >= LAST_GAS )
    {
    Txt[0] = 0;
    return;
    }
  lstrcpy ( Txt, NameGasComp[ID] );
  }

bool CСompList::IsWrite()
  {
  return true;
  }

int CСompList::Len()
  {
  if ( ID < 0 || ID >= LAST_GAS )
    return 0;
  return lstrlen ( NameGasComp[ID] ) + 1;
  }
void * CСompList::Data()
  {
  if ( ID < 0 || ID >= LAST_GAS )
    return NULL;
  return NameGasComp[ID];
  }
bool CСompList::SetData( int Len, const void * pData )
  {
  N = -1;
  ID = -1;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( lstrcmp ( NameGasComp[c], (char*)pData ) == 0 )
      {
      ID = c;
      N = N_GasComp[ID];
      return true;
      }
    }
  return true;
  }
#endif