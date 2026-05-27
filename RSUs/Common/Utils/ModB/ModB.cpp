#include "stdafx.h"
#include "ModB.h"
#include <macros/csvrow.h>
#include <fstream>
#include <sstream>

using namespace std;

KModB::KModB(  char * _ObjName )
:	IBaseModel ( _ObjName )
, m_ExchangeCount( 0 )
{
  ObjName = _ObjName;
  TypeObj = RSU_Obj;
}

KModB::~KModB(void)
{
}

int KModB::Step1()
{
  char* Q = setlocale( LC_ALL, "Russian" );
  char Path[512];
  sprintf_s( Path, _countof(Path), "%sData\\Acy\\MODB.csv", GetBasePath());
  ifstream file( Path );
  aCSVRow_  row(';');
  int lineNum = 0;

  void *Ferr = NULL;
  BOOL bWasErr = FALSE;

  for( ;file >> row; lineNum++ )
  {
    if( row.size()<4 )
      continue;

    if( row[0].size()>1 && row[0].at(0)=='#' )
      continue;

    tPairs_ pairs;
    SplitToTags( row[0], row[2], pairs );
    for ( tPairs_::iterator it=pairs.begin(), end( pairs.end()); it!=end; ++it )
    {
      tPairs_::value_type &info = *it;
      int err[2] = {0};

      ASS( m_ExchangeCount < _countof(m_Exchange) );
      if( m_ExchangeCount >= _countof(m_Exchange) )
        break;

      bool bDirect = true;
      if( row.size()>=6 )
      {
        if( !row[5].empty() && '<'==row[5].at(0) )
          bDirect = false;
      }

      if( bDirect )
        err[0] = Connect( info.from, row[1], info.to, row[3], m_Exchange[m_ExchangeCount] );
      else
        err[0] = Connect( info.to, row[3], info.from, row[1], m_Exchange[m_ExchangeCount] );

      if( err[0]==0 )
      {
        SExchange &se = m_Exchange[m_ExchangeCount];
        se.from = info.from;
        se.to = info.to;
        if( row.size()>4 && !row[4].empty() )
        {
          se.bMul = true;
          se.mul = atof( row[4].c_str() );
        }
        m_ExchangeCount++;
      }
      else
      {
        int errMin = __min(err[0], err[1]);
        if( -1 != errMin)
        {
          if( !Ferr )
            Ferr = Fopen( "Ошибки в MODBUS.csv", "wt" );
          int errMax = __max(err[0], err[1]);
          if( errMax == 1 )
            Fprintf( Ferr, "Ошибка в строке %d, не соедениться с rsLogix \"%s\", \"%s\"\n", lineNum, info.from.c_str(), info.to.c_str() );
          else if( errMax <= 3)
            Fprintf( Ferr, "Ошибка в строке %d, не соедениться с DV \"%s\", \"%s\"\n", lineNum, info.from.c_str(), info.to.c_str() );
          else
            Fprintf( Ferr, "Ошибка в строке %d, проблема с точкой DV \"%s\", \"%s\"\n", lineNum, info.from.c_str(), info.to.c_str() );
          bWasErr = TRUE;
        }
      }
    }
  }

  if( Ferr )
    Fclose( Ferr );

  return 0;
}

int KModB::Connect( const std::string &szFrom,  const std::string &szFromType, const std::string &szTo, const std::string &szToType, SExchange &pair )
{
  if( 0!=pair.rsuFrom.LinkTo( szFrom.c_str(), NULL, szFromType.c_str() ) )
    return 1;

  if( 0!=pair.rsuTo.LinkTo( szTo.c_str(), NULL, szToType.c_str() ) )
    return 2;

  return 0;
}

int KModB::StepT(double dt)
{
  for ( int i=0;i<m_ExchangeCount; i++ )
  {
    SExchange &RC = m_Exchange[i];
    RC.rsuTo << RC.rsuFrom;
    if( !RC.bMul )
      continue;
    RC.rsuTo *= RC.mul;
  }
  return 0;
}

int KModB::SplitToTags( const std::string &left, const std::string &right, tPairs_ &pairs )
{
  tVectString_ vl, vr;
  SplitLine( left, vl );
  SplitLine( right, vr );
  if( vl.size()<vr.size() )
    Dilator( vl, vr );
  else if( vr.size()<vl.size() )
    Dilator( vr, vl );

  if( vl.size()!=vr.size() )
    return 1;
  for( tVectString_::size_type i=0; i<vl.size(); i++ )
  {
    tPairs_::value_type pair = { vl.at(i), vr.at(i)};
    pairs.push_back( pair );
  }
  return 0;
}

int KModB::Dilator( tVectString_ &minSize, tVectString_ &bigSize )
{
  if( 1!=minSize.size() )
    return 1;
  int index = 0;
  char buf[64];
  string pref = minSize.at(0);
  minSize.clear();
  for( tVectString_::size_type i=0; i<bigSize.size(); i++, index++ )
  {
    _snprintf_s( buf, _countof(buf), _countof(buf), "%s.%d", pref.c_str(), index );
    minSize.push_back( buf );
  }
  return 0;
}

void KModB::SplitLine( const std::string &str, tVectString_ &lines )
{
  string tok;
  stringstream ss( str );
  while( getline( ss, tok, ',') )
  {
    SplitLineComma( tok, lines );
  }
}

void KModB::SplitLineComma( const std::string &str, tVectString_ &lines )
{
  string tok, mas[2];
  stringstream ss( str );
  int c = 0;
  while( getline( ss, tok, '~') )
  {
    ASS( c<_countof(mas) );
    mas[c++] = tok;
  }
  ASS(c);
  if( 1==c )
    lines.push_back( mas[0] );
  else if( 2==c )
  {
    string::size_type pos = mas[0].find_first_of("0123456789");
    ASS( string::npos!=pos );
    string pref = mas[0].substr( 0, pos );
    string post = mas[0].substr( pos );
    char mask[16], buf[16];
    _snprintf_s( mask, _countof(mask), _countof(mask), "%%0%dd", post.size() );
    int from = atoi( post.c_str() );
    post = mas[1].substr( pos );
    int to = atoi( post.c_str() );
    char *pBuf = buf;
    strcpy_s( buf, _countof(buf), pref.c_str() );
    pBuf += pref.size();
    for ( int i=from; i<=to; i++ )
    {
      _snprintf_s( pBuf, _countof(buf)-pref.size(), _countof(buf)-pref.size(), mask, i );
      lines.push_back( buf );
    }
  }
}
