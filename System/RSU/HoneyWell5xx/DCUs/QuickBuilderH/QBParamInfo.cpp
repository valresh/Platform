#include "QBParamInfo.h"
#include <macros/en2str.h>
#include <math.h>
#include <algorithm>
#include <rsuErr.h>
#include <rsuIsType.h>
#include <crosslocale.h>

void FillVal( char *pszBuf, size_t bufSize, eVarType Type, BYTE *Addr, USHORT stringVarSize, LPCSTR pszEnumVals )
{
  switch( Type )
  {
  case evtBool:
    {
      ASSD( !pszEnumVals );
      BYTE v = *(BYTE*)Addr;
      if( 1==v )
        strcpy_s( pszBuf, bufSize, "TRUE");
      else if( 0==v )
        strcpy_s( pszBuf, bufSize, "FALSE");
      else
        strcpy_s( pszBuf, bufSize, "TRUE (???)");
    }
    break;
  case evtByte:
    {
      BYTE v = *(BYTE*)Addr;
      if( pszEnumVals )
      {
        tValEnums_ Array;
        SplitEnumArgs( pszEnumVals, Array );
        tValEnums_::iterator it = Array.find( v );
        if( Array.end()!=it )
          sprintf_s( pszBuf, bufSize, "%s", it->second.c_str());
        else
          sprintf_s( pszBuf, bufSize, "??? %d", (int)v);
      }
      else
        sprintf_s( pszBuf, bufSize, "%d", (int)v);
    }
    break;
  case evtUShort:
    {
      USHORT v = *(USHORT*)Addr;
      if( pszEnumVals )
      {
        tValEnums_ Array;
        SplitEnumArgs( pszEnumVals, Array );
        tValEnums_::iterator it = Array.find( v );
        if( Array.end()!=it )
          sprintf_s( pszBuf, bufSize, "%s", it->second.c_str());
        else
          sprintf_s( pszBuf, bufSize, "??? %d", (int)v);
      }
      else
      {
        ASSD(0);
        sprintf_s( pszBuf, bufSize, "%d", (int)v);
      }
    }
    break;
  case evtInt:
    {
      int v = *(int*)Addr;
      sprintf_s( pszBuf, bufSize, "%d", v);
    }
    break;
  case evtDouble:
    {
      double V = *(double*)Addr;
      if ( V < -1e10 )
        V = -1e10;
      if ( V > 1e10 )
        V = 1e10;
		  if ( V == 0. )
			  strcpy_s( pszBuf, bufSize, "0.0" );
      else if ( fabs ( V ) < 1e-5 )
			  sprintf_s ( pszBuf, bufSize, "%13.5le", V );
      else if ( fabs ( V ) < 1e-4 )
			  sprintf_s ( pszBuf, bufSize, "%11.9lf", V );
      else if ( fabs ( V ) < 1e-3 )
			  sprintf_s ( pszBuf, bufSize, "%10.8lf", V );
      else if ( fabs ( V ) < 1e-2 )
			  sprintf_s ( pszBuf, bufSize, "%9.7lf", V );
      else if ( fabs ( V ) < 1e-1 )
			  sprintf_s ( pszBuf, bufSize, "%8.6lf", V );
      else if ( fabs ( V ) < 1. )
			  sprintf_s ( pszBuf, bufSize, "%7.5lf", V );
      else if ( fabs ( V ) < 10. )
			  sprintf_s ( pszBuf, bufSize, "%6.4lf", V );
      else if ( fabs ( V ) < 100. )
			  sprintf_s ( pszBuf, bufSize, "%5.3lf", V );
      else if ( fabs ( V ) < 1000. )
			  sprintf_s ( pszBuf, bufSize, "%5.2lf", V );
      else if ( fabs ( V ) < 10000. )
			  sprintf_s ( pszBuf, bufSize, "%5.1lf", V );
      else if ( fabs ( V ) > 10000000. )
			  sprintf_s ( pszBuf, bufSize, "%e", V );
      else if( IsNaN(V) )
        strcpy_s( pszBuf, bufSize, "NaN");
      else
			  sprintf_s ( pszBuf, bufSize, "%.0lf", V );
    }
    break;
  case evtString:
    {
      LPCSTR pv = (LPCSTR)Addr;
      pszBuf[0] = '"';
      strncpy_s( pszBuf+1, bufSize-1, pv, stringVarSize );
      strcat_s( pszBuf, bufSize, "\"");
    }
    break;
  case evtTime:
    {
      time_t v = *(time_t*)Addr;
      sprintf_s( pszBuf, bufSize, "%I64d", v);
    }
    break;
  }
}

void FillValues( tValEnums_ &vals, eVarType Type, BYTE *Addr, USHORT stringVarSize, LPCSTR pszEnumVals )
{
  vals.clear();
  switch( Type )
  {
  case evtBool:
    {
      ASSD( !pszEnumVals );
      vals.insert( tValEnums_::value_type(0,"FALSE") );
      vals.insert( tValEnums_::value_type(1,"TRUE") );
    }
    break;
  case evtByte:
  case evtUShort:
    if( pszEnumVals )
      SplitEnumArgs( pszEnumVals, vals );
    break;
  }
}

void SetValue( LPCSTR pszVal, eVarType Type, BYTE *Addr, USHORT stringVarSize, LPCSTR pszEnumVals )
{
  int tmpForEnum = -100;
  if( pszEnumVals )
  {
    tEnumVals_ Array;
    SplitEnumArgs( pszEnumVals, Array );
    std::string trimmed( pszVal );
    std::string::iterator l = std::remove( trimmed.begin(), trimmed.end(), ' ');
    if( l!=trimmed.end() )
      *l = '\0';
    l = std::remove( trimmed.begin(), trimmed.end(), '[');
    if( l!=trimmed.end() )
      *l = '\0';
    l = std::remove( trimmed.begin(), trimmed.end(), ']');
    if( l!=trimmed.end() )
      *l = '\0';
    pszVal = trimmed.c_str();
    if( isdigit(pszVal[0]) )
      tmpForEnum = atoi(pszVal);
    else
    {
      for( tEnumVals_::iterator it=Array.begin(), end(Array.end()); it!=end; ++it )
      {
        LPCSTR pU = it->first.c_str();
        if( !_strcmpi( pU, pszVal ) )
        {
          tmpForEnum = it->second;
          break;
        }
      }
    }
    if( -100!=tmpForEnum )
    {
      switch( Type )
      {
      case evtByte:
        *(BYTE*)Addr = tmpForEnum;
        break;
      case evtUShort:
        *(USHORT*)Addr = tmpForEnum;
        break;
      default:
        ASSD(0);
        KKK();
        break;
      }
    }
    else
    {
      ASSD(0);
#pragma message("!!!!!! возможно, обработать как связь\n")
    }
  }
  else
  {
    switch( Type )
    {
    case evtByte:
      *(BYTE*)Addr = (BYTE)atoi(pszVal);
      break;
    case evtBool:
      if( isdigit(*pszVal) )
        *(BYTE*)Addr = (BYTE)atoi(pszVal);
      else
      {
        if( !_strcmpi(pszVal, "ON") )
          *(BYTE*)Addr = true;
        else if( !_strcmpi(pszVal, "OFF") )
          *(BYTE*)Addr = false;
        else if( !_strcmpi(pszVal, "DISABLE") )
          *(BYTE*)Addr = false;
        else if( !_strcmpi(pszVal, "ENABLE") )
          *(BYTE*)Addr = true;
        else if( !_strcmpi(pszVal, "YES") )
          *(BYTE*)Addr = true;
        else if( !_strcmpi(pszVal, "TRUE") )
          *(BYTE*)Addr = true;
        else if( !_strcmpi(pszVal, "FALSE") )
          *(BYTE*)Addr = false;
        else if( !_strcmpi(pszVal, "N") )
          *(BYTE*)Addr = false;
        else if( !_strcmpi(pszVal, "Y") )
          *(BYTE*)Addr = true;
        else
        {
          ASS(0);
        }
      }
      break;
    case evtInt4:
      *(int*)Addr = atoi(pszVal);
      break;
    case evtInt2:
      *(short*)Addr = atoi(pszVal);
      break;
    case evtDouble:
      if( IsFlt(pszVal) )
        *(double*)Addr = cross::locale::safe_atof(pszVal);
      else if( !strcmp(pszVal,"NaN") )
        *(double*)Addr = NaN;
      else
      {
        ASSD(0);
      }
      break;
    case evtTime:
      *(time_t*)Addr = std::stoll(pszVal);
      break;
    case evtString:
      {
#ifdef _DEBUG
      size_t l = strlen(pszVal);
      strcpy_s( (char*)Addr, stringVarSize, pszVal );
      KKK();
#else
      strncpy( (char*)Addr, pszVal, stringVarSize-1 );
#endif
      }
      break;
    default:
      ASS(0);
    }
  }
}
