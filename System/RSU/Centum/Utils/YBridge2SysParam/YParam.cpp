#include <YBridge2SysParam.h>
#include <rsuErr.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <crossstring.h>

SYParam::SYParam() 
: pszName(NULL)
, pAddr(NULL)
, Size(0)
, Type(0)
, pextStaticVals(NULL)
, extCount(0)
, bFullEqValue( false )
//, pCallExternWindowHandler( NULL )
, pDefValue( NULL )
{
  ParamName[0] = 0;
}

void SYParam::Clear()
{
  ZeroMemory( this, sizeof(*this) );
}

bool SYParam::Out( LPSTR pszTxt, size_t size )
{
  switch( Type )
  {
  default:
    ASSD(0);
    return false;
  case 'T':
    strcpy_s( pszTxt, size, pszName );
    break;
  case 'I':
    {
      int V = *(int*)pAddr;
      if( !extCount )
        sprintf_s( pszTxt, size, "%d", V );
      else if( pextStaticVals )
      {
        if( !bFullEqValue )
        {
          for( int i=0; i<extCount; i++ )
          {
            if( V==(pextStaticVals[i].iValue&V) )
            {
              strcpy_s( pszTxt, size, pextStaticVals[i].szName );
              break;
            }
          }
        }
        else
        {
          for( int i=0; i<extCount; i++ )
          {
            if( V == pextStaticVals[i].iValue )
            {
              strcpy_s( pszTxt, size, pextStaticVals[i].szName );
              break;
            }
          }
        }
      }
    }
    break;
  case 'W':
    {
      BYTE V = *(BYTE*)pAddr;
      sprintf_s( pszTxt, size, "%d", V );
    }
    break;
  case 'B':
  case 'F':
    {
      BYTE V = *(BYTE*)pAddr;
      if( !extCount )
      {
        if( 1==V )
          strcpy_s( pszTxt, size, "Да" );
        else if( !V )
          strcpy_s( pszTxt, size, "Нет" );
        else
          sprintf_s( pszTxt, size, "%d", V );
      }
      else if( pextStaticVals )
      {
        for( int i=0; i<extCount; i++ )
        {
          if( V == pextStaticVals[i].eValue )
          {
            strcpy_s( pszTxt, size, pextStaticVals[i].szName );
            break;
          }
        }
      }
    }
    break;
  case 'S':
    {
      if( Size > 0 )
        strncpy_s( pszTxt, size, (char*)pAddr, Size );
      else
        strcpy_s( pszTxt, size, (char*)pAddr );
    }
    break;
  case 'C':
    {
      char V = *(char*)pAddr;
      if( !extCount )
      {
        pszTxt[0] = V;
        pszTxt[1] = 0;
      }
      else if( pextStaticVals )
      {
        for( int i=0; i<extCount; i++ )
        {
          if( V == pextStaticVals[i].eValue )
          {
            strcpy_s( pszTxt, size, pextStaticVals[i].szName );
            break;
          }
        }
      }
    }
    break;
  case 'D':
    {
      double V = *(double*)pAddr;
      if ( !_finite ( V ))
      {
        strcpy_s( pszTxt, size, "#" );
        break;
      }
      if ( V > 1e10 )
      {
        strcpy_s( pszTxt, size, ">" );
        break;
      }
      if ( V < -1e10 )
      {
        strcpy_s( pszTxt, size, "<" );
        break;
      }
      if ( ParamName[0] == '#' && ParamName[1] == '%'	)
        V *= 100.;
      if ( V == 0. )
        strcpy_s( pszTxt, size, "0" );
      else if ( fabs ( V ) < 1e-5 )
        sprintf_s( pszTxt, size, "%13.5le", V );
      else if ( fabs ( V ) < 1e-4 )
        sprintf_s( pszTxt, size, "%11.9lf", V );
      else if ( fabs ( V ) < 1e-3 )
        sprintf_s( pszTxt, size, "%10.8lf", V );
      else if ( fabs ( V ) < 1e-2 )
        sprintf_s( pszTxt, size, "%9.7lf", V );
      else if ( fabs ( V ) < 1e-1 )
        sprintf_s( pszTxt, size, "%8.6lf", V );
      else if ( fabs ( V ) < 1. )
        sprintf_s( pszTxt, size, "%7.5lf", V );
      else if ( fabs ( V ) < 10. )
        sprintf_s( pszTxt, size, "%6.4lf", V );
      else if ( fabs ( V ) < 100. )
        sprintf_s( pszTxt, size, "%5.3lf", V );
      else if ( fabs ( V ) < 1000. )
        sprintf_s( pszTxt, size, "%5.2lf", V );
      else if ( fabs ( V ) < 10000. )
        sprintf_s( pszTxt, size, "%5.1lf", V );
      else if ( fabs ( V ) > 10000000. )
        sprintf_s( pszTxt, size, "%e", V );
      else
        sprintf_s( pszTxt, size, "%5.0lf", V );
    }
    break;
  }
  return true;
}

void SYParam::SetValue( LPCSTR pszValue )
{
  if( !pAddr )
    return;
  switch( Type )
  {
  default:
    ASSD(0);
    return;
  case 'T':
    break;
  case 'I':
    if( !extCount )
    {
      int iVal = atoi ( pszValue );
      *(int*)pAddr = iVal;
    }
    else if( pextStaticVals )
    {
      for( int i=0; i<extCount; i++ )
      {
        if( strcmp( pszValue, pextStaticVals[i].szName) == 0 )
        {
          int iVal = pextStaticVals[i].iValue;
          *(int*)pAddr = iVal;
          break;
        }
      }
    }
    break;
  case 'W':
    {
      BYTE V = (BYTE)atoi(pszValue);
      *(BYTE*)pAddr = V;
    }
    break;
  case 'B':
  case 'F':
    {
      bool bVal = false;
      if (cross::string::starts_with(pszValue, U'Д') || cross::string::starts_with(pszValue, U'д') || pszValue[0] == 'Y' || pszValue[0] == 'y'  || pszValue[0] == '1' )
        bVal = true;

      if( !extCount )
      {
        *(bool*)pAddr = bVal;
      }
      else if(  pextStaticVals )
      {
        for( int i=0; i<extCount; i++ )
        {
          if( strcmp( pszValue, pextStaticVals[i].szName) == 0 )
          {
            BYTE bVal = pextStaticVals[i].eValue;
            *(BYTE*)pAddr = bVal;
            break;
          }
        }
      }
    }
    break;
  case 'S':
    {
      strcpy_s( (char*)pAddr, Size, pszValue );
    }
    break;
  case 'C':
    {
      char V = pszValue[0];
      if( !extCount )
      {
        *(char*)pAddr = V;
      }
      else if( pextStaticVals )
      {
        for( int i=0; i<extCount; i++ )
        {
          if( V == pextStaticVals[i].eValue )
          {
            BYTE bVal = pextStaticVals[i].eValue;
            *(BYTE*)pAddr = bVal;
            break;
          }
        }
      }
    }
  case 'D':
    {
      double dVal = atof( pszValue );
      *(double*)pAddr = dVal;
    }
    break;
  }
}

bool SYParam::ChangeVal( double Delta )
{
  if( !pAddr )
    return false;
  switch( Type )
  {
  default:
    ASSD(0);
    return false;
  case 'T':
    break;
  case 'I':
    if( !extCount )
    {
      int I = *(int*)(pAddr);
      Delta -= 1.;
      Delta *= 100.;
      I = (int)( I + Delta );
      *(int*)(pAddr) = I;
    }
    else
    {
      int V = *(int*)pAddr;
      int i=0;
      for( i=0; i<extCount; i++ )
      {
        if( pextStaticVals && V == pextStaticVals[i].iValue )
          break;
      }
      if ( Delta > 1. )
        i++;
      else
        i--;
      if( i<0 )
        i = 0;
      if( i>=extCount )
        i = extCount-1;
      if ( pextStaticVals )
        *(int*)(pAddr) = pextStaticVals[i].iValue;
    }
    break;
  case 'W':
    if( !extCount )
    {
      BYTE B = *(BYTE*)(pAddr);
      Delta -= 1.;
      Delta *= 100.;
      B = (BYTE)( B + Delta );
      *(BYTE*)(pAddr) = B;
      break;
    }
  case 'B':
  case 'F':
    if( !extCount )
    {
      bool b = *(bool*)(pAddr);
      *(bool*)(pAddr) = !b;
    }
    else
    {
      BYTE V = *(BYTE*)pAddr;
      int i=0;
      for( i=0; i<extCount; i++ )
      {
        if( pextStaticVals && V == pextStaticVals[i].eValue )
          break;
      }
      if ( Delta > 1. )
        i++;
      else
        i--;
      if( i<0 )
        i = 0;
      if( i>=extCount )
        i = extCount-1;
      if ( pextStaticVals )
        *(BYTE*)(pAddr) = pextStaticVals[i].eValue;
    }
    break;
  case 'S':
    break;
  case 'C':
    {
      char V = *(char*)pAddr;
      int i=0;
      for( i=0; i<extCount; i++ )
      {
        if( pextStaticVals && V == pextStaticVals[i].eValue )
          break;
      }
      if ( Delta > 1. )
        i++;
      else
        i--;
      if( i<0 )
        i = 0;
      if( i>=extCount )
        i = extCount-1;
      if ( pextStaticVals )
        *(char*)(pAddr) = pextStaticVals[i].eValue;
    }
  case 'D':
    {
      double V = *(double*)(pAddr);
      if ( V == 0. )
        V = Delta - 1.;
      else
        V *= Delta;
      *(double*)(pAddr) = V;
    }
    break;
  }
  return true;
}
