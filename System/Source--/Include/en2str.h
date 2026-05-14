#pragma once
#include <string>
#include <map>
#include <sstream>
#include <crossplatform.h>

static std::string TrimEnumString(const std::string &s)
{
  using namespace std;
  string::const_iterator it = s.begin();
  while (it != s.end() && ' '==(*it)) { it++; }
  string::const_reverse_iterator rit = s.rbegin();
  while (rit.base() != it && ' '==(*rit)) { rit++; }
  return string(it, rit.base());
}

typedef std::map<int,std::string> tValEnums_;

static void SplitEnumArgs(const char* szArgs, tValEnums_ &Array)
{
  using namespace std;
  stringstream ss(szArgs);
  string strSub;
  int nIdx = 0;
  while( ss.good() ) 
  {
    getline(ss, strSub, ',');
    string::size_type pos = strSub.find('=');
    if( string::npos != pos )
    {
      string n = strSub.substr( 0, pos );
      string i = strSub.substr( pos+1 );
      n = TrimEnumString( n );
      i = TrimEnumString( i );
      int v = 0;
      string::size_type pos = i.find_first_not_of("0123456789-");
      if( string::npos==pos )
        v = atoi( i.c_str() );
      else if( 'x'==i.at(pos) )
        v = strtoul( i.c_str(), NULL, 16 );
      else
      {
        const char* pI = i.c_str();
        for( map<int,string>::iterator it=Array.begin(), end(Array.end()); it!=end; ++it )
        {
          const char* pU = it->second.c_str();
          if( !_strcmpi( pU, pI ) )
          {
            v = it->first;
            break;
          }
        }
      }
      nIdx = v;
      strSub = n;
    }
    Array.insert( map<int,string>::value_type(nIdx, TrimEnumString(strSub)) );
    nIdx++;
  }
}

typedef std::map<std::string,int> tEnumVals_;

static void SplitEnumArgs(const char* szArgs, tEnumVals_ &Array)
{
  using namespace std;
  stringstream ss(szArgs);
  string strSub;
  int nIdx = 0;
  while( ss.good() ) 
  {
    getline(ss, strSub, ',');
    string::size_type pos = strSub.find('=');
    if( string::npos != pos )
    {
      string n = strSub.substr( 0, pos );
      string i = strSub.substr( pos+1 );
      n = TrimEnumString( n );
      i = TrimEnumString( i );
      int v = 0;
      string::size_type pos = i.find_first_not_of("0123456789-");
      if( string::npos==pos )
        v = atoi( i.c_str() );
      else if( 'x'==i.at(pos) )
        v = strtoul( i.c_str(), NULL, 16 );
      else
      {
        const char* pI = i.c_str();
        for( std::map<std::string,int>::iterator it=Array.begin(), end(Array.end()); it!=end; ++it )
        {
          const char* pU = it->first.c_str();
          if( !_strcmpi( pU, pI ) )
          {
            v = it->second;
            break;
          }
        }
      }
      nIdx = v;
      strSub = n;
    }
    Array[ TrimEnumString(strSub) ] = nIdx;
    nIdx++;
  }
}

static int GetIndex( std::map<int,std::string> &cont, const char* pszTest, int errorIndex )
{
  typedef std::map<int,std::string> t_;
  for( t_::iterator it=cont.begin(), end(cont.end()); it!=end; ++it )
  {
    if( it->second==pszTest )
      return it->first;
  }
  return errorIndex;
}


#define DECLARE_ENUM(ename, ...) \
  enum ename { __VA_ARGS__, MAX_NUMBER_OF_##ename }; \
  static tValEnums_ ename##Strings; \
  static bool ename##StringsbInited = false; \
  static const char* ename##ToString(ename e) { \
  if ( !ename##StringsbInited ) { ename##StringsbInited = true; SplitEnumArgs(#__VA_ARGS__, ename##Strings); } \
  return ename##Strings[e].c_str(); \
  } \
  static ename StringTo##ename(const char* szEnum) { \
  if ( !ename##StringsbInited ) { ename##StringsbInited = true; SplitEnumArgs(#__VA_ARGS__, ename##Strings); } \
    return (ename)GetIndex( ename##Strings, szEnum, MAX_NUMBER_OF_##ename ); \
  }
