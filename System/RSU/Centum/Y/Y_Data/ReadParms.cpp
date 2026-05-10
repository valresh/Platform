#include "Y_Data.h"
#include "math.h"
#include <rsuErr.h>
#include "ReadParms.h"
#include <macros/StrHelps.h>


int CompParmInfo ( const void * pP1, const void * pP2 )
  {
  CYParmInfo * pI1 = (CYParmInfo*)pP1;
  CYParmInfo * pI2 = (CYParmInfo*)pP2;
  return strcmp ( pI1->ParmName, pI2->ParmName );
  }
int CompVarInfo ( const void * pP1, const void * pP2 )
  {
  CYVarInfo * pI1 = (CYVarInfo*)pP1;
  CYVarInfo * pI2 = (CYVarInfo*)pP2;
  return strcmp ( pI1->VarName, pI2->VarName );
  }

CYParmInfo Key;
CYParmInfo * GetParm ( int kParmInfo, CYParmInfo ParmInfo[],  char * ParmName )
  {
  Key.ParmName = ParmName;
  void * pRes = bsearch ( &Key, ParmInfo, kParmInfo, sizeof ( CYParmInfo ), CompParmInfo );
  return (CYParmInfo*)pRes;
  }

CYVarInfo * GetParmRef ( CY_Base * pObj,  char * VarName )
  {
  CYVarInfo Key;
  Key.VarName = VarName;
  void * pRes = bsearch ( &Key, pObj->ClassVarInfo, pObj->kClassVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
  return (CYVarInfo*)pRes;
  }

bool SetVar( LPCSTR ParmName, void * pVar, char Type, char * Value, USHORT szVarSize )
{
  switch ( Type )
    {
    case 'I':
      {
        LPSTR pEnd = NULL;
        DWORD v = strtoul(Value, &pEnd, 10 );
        DWORD MD = 0;
        if( v & 0x80000000 )
        {
          MD = v & (SMode::MD_IMAN|SMode::MD_TRK);
          if( ParmName && !strcmp(ParmName,"MODE") )
          {
            v = v & SMode::_MODE_MASK;
            if( v&(~SMode::OS) )
              v &= ~SMode::OS;
          }
          ASSD( szVarSize==sizeof(v) );
          memmove ( pVar, &v, szVarSize );
        }
        else
        {
          int I = atoi ( Value ); 
          MD = I & (SMode::MD_IMAN|SMode::MD_TRK);
          if( ParmName && !strcmp(ParmName,"MODE") )
          {
            I = I & SMode::_MODE_MASK;
          }
          ASSD( szVarSize==sizeof(I) );
          memmove ( pVar, &I, szVarSize );
        }
#pragma message("!!!!! улучшение\n")
        /*if( MD )
        {
          SMode *pMode = (SMode*)pVar;
          if( MD & SMode::MD_IMAN )
            pMode->IMAN = true;
          if( MD & SMode::MD_TRK )
            pMode->TRK = true;
        }*/
      }
    break;
    case 'D':
      {
      double D = atof ( Value ); 
      ASSD( szVarSize==sizeof(D) );
      if( fabs(D) < FLT_MAX )
        memmove ( pVar, &D, szVarSize );
      else
        return false;
      }
    break;
    case 'S':
      {
        LPSTR pNoComma = strchr( Value, '"');
        if( !pNoComma )
        {
          size_t len = strlen(Value);
          if( len<szVarSize)
            strcpy_s( (char*)pVar, szVarSize, Value );
          else
            memcpy( pVar, Value, szVarSize );
        }
        else
        {
          ++pNoComma;
          strcpy_s( (char*)pVar, szVarSize, pNoComma );
          pNoComma = strrchr( (char*)pVar, '"');
          if( pNoComma )
            *pNoComma = 0;
        }
      }
    break;
    case 'B':
      {
      bool B;
      ASS ( Value[0] == 'Y' || Value[0] == 'N' );
      if ( Value[0] == 'Y' )
        B = true;
      else
        B = false;
      ASSD( szVarSize==sizeof(B) );
      memmove ( pVar, &B, szVarSize );
      }
    break;
    case 'H':
      {
      char * Pr = strchr ( Value, '%' );
      double D;
      if ( Pr )
        D = atof ( Value ) * 0.01; // %
      else
        D = -atof ( Value ); // абс величины
      ASSD( szVarSize==sizeof(D) );
      memmove ( pVar, &D, szVarSize );
      }
    break;
    case 'W':
      {
        DWORD t = atoi ( Value ); 
        if( t>_UI8_MAX || szVarSize!=sizeof(t) )
          return false;
        BYTE W = (BYTE)t; 
        memmove( pVar, &W, szVarSize );
      }
    break;
    default:
      ASSD(FALSE)
    return false;
    }
  return true;
}

static LPCSTR s_szSkipTunningParams[] = { "ALRM", "AFLS", "AF", "AOFS" };

void SetTunningValues( CY_Base * pY, char * Params )
{
  char * Numb = Params;
  char * Model = strchr ( Numb, ':' );
  if ( Model == NULL )
    return;
  char * Parm = strchr ( Model + 1, ':' );
  if ( Parm == NULL )
    return;
  TStringTerminator stModel(Model);
  TStringTerminator stParm(Parm);
  char Fld[3072][88];
  int K = SetFld_X_FromTunning( Parm+1, ',',  Fld );
  ASS( K < _countof(Fld) );
  for ( int n = 0; n < K; n++ )
  {
    char * ParmName = Fld[n];
    char * P = strchr ( ParmName, '=' );
    *P = 0;
    P++;
    void * pVar = NULL;
    void * pTuneVar = NULL;
    char Type = 0;
    CY_Base::eVarType Work = CY_Base::evtP;
    USHORT szVarSize = 0;
    bool Res = pY->GetVar( ParmName, &pVar, &pTuneVar, &Type, &Work, NULL, &szVarSize );
    if ( !Res )
      continue;
    if( !CY_Base::s_flagReadAllTunningValues && (pY->Y_Type>=id__SFCSW && pY->Y_Type<=id__UTSW) )
      Work = CY_Base::evtP;
    if( CY_Base::evtP!=Work )
    {
      if ( pTuneVar )
        pVar = pTuneVar;
      else
      {
        if( !CY_Base::s_flagReadAllTunningValues )
          continue;
        bool bSkip = false;
        for( int i=0; i<_countof(s_szSkipTunningParams); ++i )
        {
          if( !strcmp(ParmName, s_szSkipTunningParams[i]) )
          {
            bSkip = true;
            break;
          }
        }
        if( bSkip )
          continue;
      }
    }
    if( !SetVar( ParmName, pVar, Type, P, szVarSize ) )
      pY->OnFailedTunningAssign( ParmName, P );
    KKK();
  }
}

bool GetTPFX ( CY_Base * pY, void * pBase, int kVarInfo, CYVarInfo VarInfo[],  char * TPFX )
{
  if ( TPFX[0] == 0 )
    return false;
  char Fld[50][64];
  int K = SetFld_X( TPFX, ',', Fld );
  ASS( K < 50 );
  CYVarInfo  Key;
  for ( int n = 0; n < K; n++ )
  {
    char * P = strchr ( Fld[n], '=' );
    if ( P == NULL )
      continue;
    TStringTerminator st( P );
    P++;
    Key.VarName = Fld[n];
    for( LPSTR pSz = Fld[n]; *pSz; ++pSz )
    {
      *pSz = toupper( *pSz );
    }
    CYVarInfo * pRes = (CYVarInfo*)bsearch ( &Key, VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
    if( !pRes )
    {
      char buf[8] = { 0 };
      LPCSTR pNul = strchr( Fld[n], '0' );
      if( pNul )
      {
        strncpy_s( buf, Fld[n], pNul-Fld[n] );
        strcat_s( buf, pNul+1 );
        Key.VarName = buf;
        _strupr_s( buf );
        pRes = (CYVarInfo*)bsearch ( &Key, VarInfo, kVarInfo, sizeof ( CYVarInfo ), CompVarInfo );
      }
    }
    if( !pRes )
    {
      void *pVar = NULL;
      char type = ' ';
      USHORT szVarSize = 0;
      bool y = pY->GetVar( Fld[n], &pVar, NULL, &type, NULL, NULL, &szVarSize );
      if( y )
      {
        SetVar( NULL, pVar, type, P, szVarSize );
        continue;
      }
    }
    if ( pRes == NULL )
    {
      //Fsend ( "TPFX.csv", "%s;%s;\n", pY->Model, Fld[n] );
      continue;
    }
    BYTE * pVar = (BYTE*)pBase + pRes->sVar;
    SetVar( NULL, pVar, pRes->Type, P, pRes->szVarSize );
  }
  return true;
}

bool Parm ( char * Value, char * Fmt, ... )
	{
  int nP = -1;
	va_list arg;
	va_start ( arg, Fmt );
	char * Mod = NULL;	
	if ( Fmt[1]	== 'T' ||	Fmt[1]	== 't' )
		{
		Mod = va_arg ( arg, char * );
		}
	char * Str = Value;
//
	int p = 0;
	int ps = 0;
	while ( 1 )
		{
		char s = Fmt[p];
		if ( s == 0 )
			break;
		char next = s;
		if ( s == '%' )
			{
// Начало фрмата
			p++;
			char F = Fmt[p];
			p++;
			char List[256];
			int L = 0;
			if ( F == '{' )
				{
				while ( L < 256 )
					{
					s = Fmt[p++];
					if ( s == '}' )
						{
						p++;
						break;
						}
					List[L++] = s;
					}
        if ( List[L-1] != ';' )
          {
					List[L++] = ';';
          }
				List[L] = 0;
				}
			next = Fmt[p];// След. символ
			char Numb[256];
			int pn = 0;
// Сканируем строку до след. символа формата
			while ( pn < 256 )
				{
				if ( Str[ps] == next )
					break;
				Numb[pn++] = Str[ps++];
				}
			ASS( pn < 256 );
			Numb[pn] = 0;
			switch ( F )
				{
				case 'N':
				case 'n':
					{
          ASS(Numb[0] == 0 )// Данных не должно быть
	        return true;
					}
				break;
				case 'D':
				case 'd':
					{
					double * D = 	va_arg ( arg, double * );
					*D = atof ( Numb );
					}
				break;
				case 'H'://HYS %(>0 * 0.01) или абс величины(<0)
				case 'h':
					{
					double * D = 	va_arg ( arg, double * );
					*D = atof ( Numb );
					if ( Numb[pn-1] == '%' )
						*D *= 0.01;
					else
						*D = - *D;
					}
				break;
				case 'I':
				case 'i':
					{
					int * I = 	va_arg ( arg, int * );
					*I = atoi ( Numb );
					}
				break;
				case 'B':
				case 'b':
					{
					bool * B = 	va_arg ( arg, bool * );
					ASS( Numb[0] == 'Y' || Numb[0] == 'N' );
					if ( Numb[0] == 'Y' )
						*B = true;
					else
						*B = false;
					}
				break;
				case 'Y':
				case 'y':
					{
					char * s = 	va_arg ( arg, char * );
					bool * B = 	va_arg ( arg, bool * );
          int L = (int)strlen ( s );
					if ( strncmp ( s, Numb, L ) == 0 )
						*B = true;
					else
						*B = false;         
					}
				break;
				case 'S':
				case 's':
					{
					char * s = 	va_arg ( arg, char * );
					strcpy_s ( s, 64, Numb );
					}
				break;
				case 'T':
				case 't':
					{
					char * s = NULL;
					if ( Mod == NULL )
						s =	va_arg ( arg, char * );
					else
						s = Mod;
					Mod = NULL;
					if ( strcmp ( s, Numb ) != 0 )
            {
            return false;
            }
					}
				break;
				case 'M':
				case 'm':
					{
//					int * I = va_arg ( arg, int * );
//					*I = Get_Mes ( Numb );
//					char * s = 	va_arg ( arg, char * );
//					lstrcpy ( s, Numb );
          ASS(FALSE)
					}
				break;
				case '{':
					{
					int N = 0;
					int pt = 0;
					bool Prop = false;
					for ( int n = 0; n < L; n++ )
						{
						char s = List[n];
						if ( Prop )
							{
							if ( s == ';' )
								Prop = false;
							continue;
							}
						if ( s == Numb[pt] )
							{
							pt++;
							continue;
							}
						if ( s == '|' )
							{
							int * I = va_arg ( arg, int * );
							*I = N;
							break;
							}
						if ( s == '*' )
							{
							int * I = va_arg ( arg, int * );
							*I = N;
							break;
							}
						if ( s == ';' && Numb[pt] == 0 )
							{
							int * I = va_arg ( arg, int * );
							*I = N;
							break;
							}
// отличие
						N++;
						pt = 0;
            if ( s != ';' )
						  Prop = true;
						}
					if ( Prop )
					  ASS ( false ); // не найден в списке
					}
				break;
				}
			}
		ASS( Str[ps] == next );
		if ( next == 0 )
			break;
		ps++;
		p++;
		}
	return true;
	}

int GetList( char * ValList, char * ParmValue )
{
  char List[256];
  strcpy_s( List, ValList );
  char * P = List;
  int n = 0; 
  while ( 1 )
  {
    char * Pn = strchr ( P, ';' );
    if ( Pn )
    {
      *Pn = 0;
      if ( _stricmp ( ParmValue, P ) == 0 )
        return n;
      n++;
      P = Pn + 1;
      continue;
    }
    Pn = strchr ( P, '}' );
    if ( Pn )
    {
      *Pn = 0;
      if ( _stricmp ( ParmValue, P ) == 0 )
        return n;
    }
    break;
  }
  size_t lenParmName = strlen( ParmValue );
  strcpy_s( List, ValList );
  P = List;
  n = 0; 
  while( 1 )
  {
    char * Pn = strchr ( P, ';' );
    if( Pn )
    {
      *Pn = 0;
      if( _strnicmp( ParmValue, P, lenParmName ) == 0 )
        return n;
      n++;
      P = Pn + 1;
      continue;
    }
    Pn = strchr ( P, '}' );
    if( Pn )
    {
      *Pn = 0;
      if( _strnicmp ( ParmValue, P, lenParmName ) == 0 )
        return n;
    }
    break;
  }
  return -1;
}

int SplitValue( int K_max, char Str[256], char Fld[][32] )
	{
	if ( Str[0] == 0 )
		return 0;
	int kFld = 0;
	int p = 0;
	int L = (int)strlen ( Str );
	int l = 0;
	char s;
  char so = 0;
	while ( p < L )
		{
		s = Str[p++];
		if ( s == ':' && so != '\\' )
			{
      ASS(kFld < K_max)
      if ( kFld >= K_max )
        return kFld;
			Fld[kFld][l] = 0;
			kFld++;
			l = 0;
			continue;
			}
    ASS( l < 32 )
		if ( kFld >= K_max || l >= 32	)
			break;
		Fld[kFld][l++] = s;
    so = s;
		}
	if ( l > 0 )
		{
		Fld[kFld][l] = 0;
		kFld++;
		}
	return kFld;
	}

bool StdGet ( void * pBase, CYParmInfo * pI, char * ParmValue )
{
  char * Fmt = pI->ParmFmt;
  ASS(Fmt[0] == '%' );
  int pFmt = 1;
  char Fld[4][32];
  int K = SplitValue( 4, ParmValue, Fld );
  DWORD pAllVar[4] = { pI->sVar_1, pI->sVar_2, pI->sVar_3, pI->sVar_4 };
  for ( int nV = 0; nV < K; nV++ )
    {
    BYTE * pVar = (BYTE*)(pBase) + pAllVar[nV];
    char * P = Fld[nV];
    ASS( pVar ); // Несоответствие формата и числа переменных
    ASS( pI->Type[nV] != 0 );
    switch ( Fmt[pFmt] )
      {
      case 'I':
        {
        int I = atoi ( P ); 
        if( 'I'==pI->Type[nV] )
          memmove( pVar, &I, 4 );
        else if( 'W'==pI->Type[nV] )
        {
          BYTE b = I;
          memmove( pVar, &b, 1 );
        }
        else
        {
          ASSD(!"?");
        }
        pFmt++;
        }
      break;
      case 'D':
        {
        double D = atof ( P ); 
        ASS( pI->Type[nV] == 'D' );
        memmove ( pVar, &D, 8 );
        pFmt++;
        }
      break;
      case 'S':
        {
        ASS((DWORD)lstrlen(P)<pI->szVarSize)
        strcpy_s( (char*)pVar, pI->szVarSize, P );
        pFmt++;
        }
      break;
      case 'B':
        {
        bool B;
        ASS ( P[0] == 'Y' || P[0] == 'N' );
        if ( P[0] == 'Y' )
          B = true;
        else
          B = false;
        ASS( pI->Type[nV] == 'B' );
        memmove ( pVar, &B, 1 );
        pFmt++;
        }
      break;
      case 'H':
        {
        char * Pr = strchr ( P, '%' );
        double D;
        if ( Pr )
          D = atof ( P ) * 0.01; // %
        else
          D = -atof ( P ); // абс величины
        ASS( pI->Type[nV] == 'D' );
        memmove ( pVar, &D, 8 );
        pFmt++;
        }
      break;
      case '{':
        {
        int N = GetList ( Fmt + 2, P );
        ASS( pI->Type[nV] == 'I');
        memmove ( pVar, &N, 4 );
        pFmt += 2;
        while ( 1 )
          {
          if ( Fmt[pFmt] == 0 )
            break;
          if ( Fmt[pFmt] == '}' )
            {
            pFmt++;
            break;
            }
          pFmt++;
          }
        }
      break;
      default:
        ASS(FALSE)
      return false;
      }
    if ( Fmt[pFmt] == 0 )
      {
      ASSD(nV <= (K - 1) );
      ASS( pI->Type[nV+1] == 0 )// Несоответствие формата и числа вх. переменных
      break;
      }
    ASS ( Fmt[pFmt] == ':' )
    pFmt++;
    ASS( Fmt[pFmt] == '%')
    pFmt++;
    }
  return true;
  }

void TestParm ( char * ParmName, double & Parm, double Default )
{
  if( Parm != DBL_MAX )
    return;
  if( Default != DBL_MAX )// Параметр должен быть обязательно задан 
    Parm = Default;
}
