#pragma once
#include <TINYXML/tinyxml2.h>
#include <string>
#include <rsuErr.h>
#include <vector>
#include <GuidConverter.h>
#include <rsuStr.h>

namespace TINYXMLHELPER
{
  template<typename T>
  struct variant_cast_t
  {
    T operator()( LPCSTR pszText )
    {
      ASS(0);
      return (T)0;
    }
  };

  template<>
  struct variant_cast_t<std::string>
  {
    std::string operator()( LPCSTR pszText )
    {      
      return pszText;
    }
  };

  /*
  template<>
  struct variant_cast_t<std::wstring>
  {
    std::wstring operator()( LPCSTR pszText )
    {      
      std::wstring w;
      int l = MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, pszText, -1, NULL, 0 );
      w.resize( l + 1 );
      MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, pszText, -1, &w.at(0), (int)w.size() );
      return w;
    }
  };
  */
  template<>
  struct variant_cast_t<int>
  {
    int operator()( LPCSTR pszText )
    {      
      return atoi(pszText);
    }
  };
  
#ifdef _WIN32
  template<>
  struct variant_cast_t<LONG>
  {
    LONG operator()( LPCSTR pszText )
    {      
      return atol(pszText);
    }
  };
#endif

  template<>
  struct variant_cast_t<double>
  {
    double operator()( LPCSTR pszText )
    {      
      return atof(pszText);
    }
  };

  template<>
  struct variant_cast_t<float>
  {
    float operator()( LPCSTR pszText )
    {      
      return (float)atof(pszText);
    }
  };

  template<>
  struct variant_cast_t<GUID>
  {
    GUID operator()( LPCSTR pszText )
    {   
      GUID guid;
      memset( &guid, 0, sizeof(guid) );
      if( '{'==pszText[0] )
        guid = StringToGuid(pszText);
      else
        guid = StringToGuidWithoutBrackets(pszText);

      return guid;
    }
  };

  template<>
  struct variant_cast_t<bool>
  {
    bool operator()( LPCSTR pszText )
    {
      bool r = false;
      size_t len = strlen(pszText);
      if( 1==len )
      {
        switch( *pszText )
        {
        case 't':
        case 'T':
          r = true;
          break;
        case 'f':
        case 'F':
          r = false;
          break;
		default:
          if( isdigit(*pszText) )
            r = atoi(pszText) ? true : false;
          else
          {
            ASS(0);
          }
          break;  
        }
      }
      else
      {
        if( !_Kstricmp(pszText,"true") )
          r = true;
        else if( !_Kstricmp(pszText,"false") )
          r = false;
        else
        {
          ASS(0);
        }
      }
      return r;
    }
  };

  template<typename T>
  inline T variant_cast( LPCSTR pszText )
  {
    return variant_cast_t<T>()(pszText);
  }

  template<typename Arg>
  bool GetXMLData( const tinyxml2::XMLElementPtr& xml, Arg& arg )
  {
    LPCSTR var = xml->GetText();
    if( !var || !*var )
      return false;
    arg = variant_cast<Arg>(var);
    return true;
  }

  template<size_t _S>
  bool GetXMLData( const tinyxml2::XMLElementPtr& xml, char (&arg)[_S] )
  {
    LPCSTR var = xml->GetText();
    if( !var || !*var )
      return false;
    strncpy( arg, var, _S );
    return true;
  }

  template<typename Arg>
  bool GetXMLAttribute( const tinyxml2::XMLElementPtr& xml, LPCSTR szAttrName, Arg& arg )
  {
    LPCSTR val = xml->Attribute( szAttrName );
    if( !val || !*val )
      return false;
    arg = variant_cast<Arg>(val);
    return true;
  }

  template<size_t _S>
  bool GetXMLAttribute( const tinyxml2::XMLElementPtr& xml, LPCSTR szAttrName, char (&arg)[_S] )
  {
    arg[0] = 0;
    LPCSTR val = xml->Attribute( szAttrName );
    if( !val || !*val )
      return false;
    strcpy_s( arg, val );
    return true;
  }

  template<size_t _S>
  tinyxml2::XMLElementPtr SelectSingleElement( tinyxml2::XMLElementPtr &root, LPCSTR pszQuery, char (&szBuf)[_S] )
  {
    LPCSTR pT = strchr( pszQuery, '/');

    if( pT )
    {
      size_t l = pT-pszQuery;
      ASS( l < _countof(szBuf) );
      strncpy_s( szBuf, _S, pszQuery, l );
      ++pT;
    }

    tinyxml2::XMLElementPtr item = root->FirstChildElement( pT ? szBuf : pszQuery );
    if( !item )
      return NULL;

    if( pT )
      return SelectSingleElement( item, pT, szBuf );  
    return item;
  }

  inline std::string GetXMLDataStr(const tinyxml2::XMLElementPtr& xml)
  {
      LPCSTR var = xml->GetText();
      if (!var || !*var)
          return "";
      return var;
  }
}
