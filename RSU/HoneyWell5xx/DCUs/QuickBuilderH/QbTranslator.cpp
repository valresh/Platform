#include "QbTranslator.h"
#include <macros/csvrow.h>

KQbTranslator::KQbTranslator()
: m_bIsLogicOperation( FALSE )
, m_pLineStream( NULL )
, m_pFullStream( NULL)
, m_bUnderComment( false )
, m_nLine( 0 )
{
  m_OPs.reserve( 1000 );
}

void KQbTranslator::GetCodeOPs( tOPCont_ &Ops, tVarCont_ &Vars )
{
  Ops = m_OPs;
  Vars = m_Vars;
}

void KQbTranslator::ParseText( LPCSTR pszOneLine, bool textAsName /*= false*/ )
{
  std::stringstream input;
  input << pszOneLine;
  m_pFullStream = &input;

  FillTokenTrain( textAsName );
  m_pLineStream = NULL;
  m_pFullStream = NULL;

  while( !m_tokenTrain.empty() )
  {
    NextTokens();
  }
  ASSD( Parm.LevIF < 1 );

}


bool KQbTranslator::FillTokenTrain( bool textAsName, LPCSTR pszLine /*= NULL*/ )
{
  if( m_pLineStream )
  {
    while( *m_pLineStream )
    {
      eToken_value tok = GetToken( *m_pLineStream );
      if( IsNotSupported(tok, m_Value) )
      {
        m_tokenTrain.clear();
        m_pLineStream = NULL;
        return false;
      }
      if( etTEXT==tok )
      {
        int N = IsFunc( m_Value.c_str() );
        if( N>=0 )
          tok = etFUNCTION_HardCoded;
        else if( textAsName )
          tok = etRealVarField;
        else if( m_Value=="A" )
          tok = etVarFieldAsTxt;
        else if( m_Value=="Dim" )
          return false;
	    else if( m_Value=="On" )
          return false;
        else
        {
          ASSD( 0 );
        }
        KKK();
      }
      SSepToken st = { tok, m_Value };
      st.nLine = m_nLine;
      m_tokenTrain.push_back( st );
    }
  }

  std::string szTmp;
  while( GetLine( *m_pFullStream, szTmp) )
  {    
    ++m_nLine;
    TrimLeft( szTmp, ' ' );
    TrimLeft( szTmp, '\t' );
    TrimRight( szTmp, '\r' );
    TrimRight( szTmp, '\t' );
    TrimRight( szTmp, ' ' );
    if( szTmp.empty() )
      continue;
    std::string::size_type posS = szTmp.find("(*");
    if( m_bUnderComment )
    {
      std::string::size_type pos = szTmp.find("*)");
      if( std::string::npos!=pos )
        szTmp.erase( 0, pos+2), m_bUnderComment = false;
    }
    if( std::string::npos!=posS )
    {
      ASS( !m_bUnderComment );
      m_bUnderComment = true;
    }
    if( m_bUnderComment )
    {
      std::string::size_type pos = szTmp.find("*)");
      if( std::string::npos!=pos )
        szTmp.erase( posS, pos+2), m_bUnderComment = false;
    }
    if( !m_bUnderComment )
    {
      std::stringstream lineStr;
      m_pLineStream = &lineStr;
      lineStr << szTmp;
      if( !FillTokenTrain( textAsName, szTmp.c_str() ) )
        return false;
    }
  }
  return true;
}

void KQbTranslator::NextTokens()
{
  while( !m_tokenTrain.empty() )
  {
    SSepToken st = m_tokenTrain.front();
    m_tokenTrain.erase( m_tokenTrain.begin() );
    eToken_value tok = st.tok;
    m_Value = st.szValue;
    if( !AnalizeTok( tok, m_Value ) )
    {
      Parm.tokAfterAnalize = tok;
      break;
    }
    Parm.tokAfterAnalize = tok;
  }
}

std::istream &KQbTranslator::GetLine( std::istream &_ist, std::string &str )
{
  std::string szTmp;
  str.clear();
  while( getline( _ist, szTmp) )
  {
    bool cont = false;
    std::string::size_type pos = szTmp.find( "//" );
    if( std::string::npos!=pos )
    {
      std::string::size_type l = szTmp.length();
      if( (l-pos)>=2 )
      {
        szTmp.erase( pos );
        cont = true;
      }
    }
    str.append( szTmp );
    if( !cont )
      break;
  }
  return _ist;
}
