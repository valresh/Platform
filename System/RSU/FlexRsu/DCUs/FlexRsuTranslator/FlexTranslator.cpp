#include <crossplatform.h>
#include "FlexTranslator.h"
#include <rsuErr.h>
#include <macros/csvrow.h>

using namespace std;
using namespace ns_translator;

KFlexTranslator::KFlexTranslator( ITranslatorOwner *pOwner )
: m_pFullStream( NULL )
, m_pLineStream( NULL )
, m_pOwner( pOwner )
, m_bIsLogicOperation( false )
{
  memset( Stack, 0xFF, sizeof( Stack ) );
  PosSt = 0; 
  memset( OP, 0xFF, sizeof( OP ) );
  kOP = 0;
  memset( nFunction, 0xFF, sizeof( nFunction ) );
  kFunction = 0; 
  memset( StackInversVars, 0xFF, sizeof( StackInversVars ) );
  kInvers = 0; 
}

KFlexTranslator::~KFlexTranslator(void)
{
}

void KFlexTranslator::Translate( LPCSTR pszExpr )
{
  m_input << pszExpr;
}

void KFlexTranslator::Translate( istream &str )
{
  m_pFullStream = &str;

  NextTokens();

  char ch = 1;
  if( m_pLineStream )
    ASS( !m_pLineStream->get(ch) );
  ASS( !m_pFullStream->get(ch) );
  m_pFullStream = NULL;
  m_pLineStream = NULL;
}

eYToken_value KFlexTranslator::GetTokenFromLine()
{
  if( !Parm.skipShiftTokHistory )
  {
    memmove( &Parm.prevTok[1], &Parm.prevTok[0], (countof(Parm.prevTok)-1)*sizeof(Parm.prevTok[0]) );
    Parm.prevTok[0] = Parm.curTok;
  }
  Parm.curTok = GetToken( *m_pLineStream );
  Parm.skipShiftTokHistory = 0;
  return Parm.curTok;
}

void KFlexTranslator::NextTokens()
{
  KTHROW( m_pFullStream );

  if( m_pLineStream )
  {
    while( *m_pLineStream )
    {
      eYToken_value tok = GetTokenFromLine();
      if( !AnalizeTok( tok, m_szValue ) )
      {
        Parm.tokAfterAnalize = unknown;
        return;
      }
    }
  }

  string szTmp;
  while( GetLine( *m_pFullStream, szTmp) )
  {    
    TrimLeft( szTmp, ' ' );
    TrimLeft( szTmp, '\t' );
    AnalizeLine( szTmp );
  }
}

std::istream &KFlexTranslator::GetLine( std::istream &_ist, std::string &str )
{
  string szTmp;
  str.clear();
  while( getline( _ist, szTmp) )
  {
    bool cont = false;
    string::size_type pos = szTmp.rfind( "//" );
    if( string::npos!=pos )
    {
      string::size_type l = szTmp.length();
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

void KFlexTranslator::AnalizeLine( std::string &szLine )
{
  Trim( szLine, ' ');
  if( szLine.empty() )
    return;

  if( '\''==szLine.at(0) )
    return;

  istream *pPrevLineStream = m_pLineStream;
  stringstream lineStr;
  m_pLineStream = &lineStr;
  lineStr << szLine;
  while( lineStr )
  {
    eYToken_value tok = GetTokenFromLine();
    if( !AnalizeTok( tok, m_szValue ) )
    {
      m_pLineStream = pPrevLineStream;
      Parm.tokAfterAnalize = unknown;
      return;
    }
    Parm.tokAfterAnalize = tok;
  }
  if( !Parm.skipEndLine )
  {
    while( PosSt > 0 )
    {
      KMethodOnDestroy<STokenElement> C(Stack[PosSt-1], &STokenElement::Reset );
      PosSt--;
      AddToOP( OP_Operation, (STokenElement)C );
      Parm.Init();
    }
  }
  m_pLineStream = pPrevLineStream;
}

void KFlexTranslator::GetResult( DWORD &kVar, DWORD &kop )
{
  Translate( m_input );
  kVar = (DWORD)Var.size();
  kop = kOP;
}

void KFlexTranslator::GetResult( KVar *var, SOPElement *op )
{
  if( !Var.empty() && var )
    memmove( var, &Var[0], sizeof(*var)*Var.size() );
  if( kOP && op )
    memmove( op, OP, sizeof(*op)*kOP );
}
