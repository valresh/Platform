#pragma once

struct CYParmInfo // Переменные из P
{
  DWORD sVar_1;
  DWORD sVar_2;
  DWORD sVar_3;
  DWORD sVar_4;
  void * Name_1;
  void * Name_2;
  void * Name_3;
  void * Name_4;
  char * Type;
  char * ParmName;
  char * ParmFmt;
  int Read;
  USHORT szVarSize;
  CYParmInfo()
  {
    memset ( this, 0, sizeof ( CYParmInfo ));
  }
};

struct CYVarInfo // Все переменные
{
  DWORD sVar;
  DWORD sVarTune;
  LPCSTR VarName;
  char Type;
  BYTE Work;//2 - относится к сигнализациям
  bool bEnum;
  LPCSTR pszEnumVals;
  USHORT szVarSize;
  CYVarInfo()
  {
    memset ( this, 0, sizeof ( CYVarInfo ));
  }
};

int CompParmInfo ( const void * pP1, const void * pP2 );
int CompVarInfo ( const void * pP1, const void * pP2 );
CYParmInfo * GetParm ( int kParmInfo, CYParmInfo ParmInfo[],  char * ParmName );
CYVarInfo * GetParmRef ( CY_Base * pObj,  char * VarName );
bool StdGet ( void * pBase, CYParmInfo * pP, char * ParmValue );
bool Parm ( char * Value, char * Fmt, ... );
bool GetTPFX ( CY_Base * pY, void * pBase, int kVarInfo, CYVarInfo VarInfo[],  char * TPFX );
void TestParm ( char * ParmName, double & Parm, double Default );


template<size_t F, size_t S>
int SetFld_X( LPCSTR Str, const char Devider, char (&Fld)[F][S], bool plantText = true, bool skipBlank = true, bool splashComma = true )
{
  ZeroMemory( Fld, sizeof(Fld) );
  if ( Str[0] == 0 )
    return 0;
  int kFld = 0;
  int p = 0;
  int L = (int)strlen ( Str );
  int l = 0;
  char s;
  bool Test = true;
  while ( p < L )
  {
    s = Str[p++];
    if( s == ' ' && skipBlank )
      continue;
    bool asis = false;
    if( s == '\\' )
    {
      if( plantText )
        continue;
      if( ','==Str[p] )
      {
        //ASSD( ','!=Devider );
        if( splashComma )
        {
          s = Str[p++];
          asis = true;
        }
      }
      else
      {
        s = Str[p++];
        asis = true;
      }
    }

    if ( s == '[' )
      Test = false;
    if ( Test && Devider==s && !asis )
    {
      ASS( l < S );
      ASS( kFld < F );
      Fld[kFld][l] = 0;
      kFld++;
      l = 0;
      continue;
    }
    ASS( l < S );
    ASS( kFld < F );
    if ( kFld >= F || l >= S	)
      break;
    if ( s == ']' )
      Test = true;
    Fld[kFld][l++] = s;
  }
  if ( l > 0 )
  {
    Fld[kFld][l] = 0;
    kFld++;
  }
  return kFld;
}

template<size_t F, size_t S>
int SetFld_X_FromTunning( LPCSTR Str, const char Devider, char (&Fld)[F][S] )
{
  ZeroMemory( Fld, sizeof(Fld) );
  if ( Str[0] == 0 )
    return 0;
  int kFld = 0;
  int p = 0;
  int L = (int)strlen ( Str );
  int l = 0;
  char s = 0, prevS = 0;
  bool Test = true;
  while ( p < L )
  {
    prevS = s;
    s = Str[p++];
    if ( s == '\\' )
      continue;
    if ( s == '[' )
      Test = false;
    if ( Test && Devider==s && '\\'!=prevS)
    {
      ASS( l < S );
      ASS( kFld < F );
      Fld[kFld][l] = 0;
      kFld++;
      l = 0;
      continue;
    }
    ASS( l < S );
    ASS( kFld < F );
    if ( kFld >= F || l >= S	)
      break;
    if ( s == ']' )
      Test = true;
    Fld[kFld][l++] = s;
  }
  if ( l > 0 )
  {
    Fld[kFld][l] = 0;
    kFld++;
  }
  return kFld;
}
