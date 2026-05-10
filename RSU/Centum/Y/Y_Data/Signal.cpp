#include <Y_Data.h>
#include "StdDef.h"

int CSignal::SaveState( )
{
	SAVE_CLASS( Name, CIO_W )
  return 0;
}

int CSignal::RestoreState ( char * StrName )
	{
	REST_CLASS( Name, CIO_W )
	return 1;
	}

int CSignal::StateSave( IYokoStateSer* pStateSer )
{
  CIO_W *p = this;
  WriteFullStruct( pStateSer, eKeySignal, Name, *p );
  return 0;
}

int CSignal::StateRestore( IYokoStateSer* pStateSer )
{
  CIO_W *p = this;
  ReadNamedFullStruct( pStateSer, eKeySignal, Name, *p );
  return 0;
}

void CSignal::Set( double & Var )
	{
	pDbl = &Var;
	Type = Double;
	}

void CSignal::Set( int & Var )
	{
	pInt = &Var;
	Type = Int;
	}

void CSignal::Set( bool & Var )
	{
	pBool = &Var;
	Type = Bool;
	}

/*void CSignal::Set( char * Var )
	{
	pChar = Var;
	Type = String;
	}*/

void CSignal::Null( )
	{
	Type = NoType;
	pDbl = NULL; 
	}

void CSignal::operator <<( double & Var )
	{
	pDbl = &Var;
	Type = Double;
	}

void CSignal::operator <<( int & Var )
	{
	pInt = &Var;
	Type = Int;
	}

void CSignal::operator <<( bool & Var )
	{
	pBool = &Var;
	Type = Bool;
	}

/*void CSignal::operator <<( char * Var )
	{
	pChar = Var;
	Type = String;
	}*/

void CSignal::operator <<( char & Var )
	{
	pChar = &Var;
	Type = Char;
	}


void CSignal::operator << ( BYTE & Var )
	{
	pByte = &Var;
	Type = Byte;
	}
