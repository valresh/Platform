#include "Trans.h"

using namespace std;
using namespace ns_translator;

eYToken_value KAaTranslator::GetToken( std::istream &input )
{
  m_szValue.clear();
  return GetTokenNoReset( input );
}

eYToken_value KAaTranslator::GetTokenNoReset( istream &input )
{
  eYToken_value tok( unknown );

  char ch = 0;
  do 
  {
    if(!input.get(ch)) 
      return unknown;
  }while( ch!='\n' && (ch==' ' || ch=='\t' ));

  switch( ch )
  {
  case '?':
    return eytIF;
  case ':':
    {
      m_szValue = ch;
      tok = eYToken_value(ch);
      input.get(ch);
      if( ch!='=' )
      {
        input.putback(ch);
        return eytELSE;
      }
      m_szValue += ch;
      return eytASSIGN;
    }
  case '\r':
  case '\n':
    return endLine;
  case '\t':
    return eytTAB;
    // Operators
  case eytEndExpr:
  case eytCOMMA:
  case '(':
  case ')':
  case '^':
    m_szValue = ch;
    tok = eYToken_value(ch);
    break;
  case '*':
  case '/':
  case '+':
  case '-':
    m_szValue = ch;
    tok = eYToken_value(ch);
    input.get(ch);
    if( ch!='=' )
      input.putback(ch);
    else
    {
      KTHROW(!"еще не умеем обрабатывать");
    }
    if( eytMINUS==tok || eytPLUS==tok )
    {
      switch( Parm.prevTok[0] )
      {
      case unknown:
        if( eytPLUS==tok )
          break;
      case eytIF:
      case eytELSE:
      case eytLP:
      case eytPLUS:
      case eytMUL:
      case eytMINUS:
      case eytDIV:
      case eytBIG:
      case eytLIT:
      case eytEQUAL:
      case eytNE:
      case eytGE:
      case eytLE:
      case eytPERCENT:
      case eytMOD:
      case eytCOMMA:
        {
          eYToken_value nextToken = GetTokenNoReset(input);
          if( nextToken==eytNUMBER )
            tok = nextToken;
          else
          {
            ASS( nextToken==eytTEXT );
            if( m_szValue=="-INF" )
              tok = nextToken;
            else if( '-'==m_szValue.at(0) )
            {
              tok = eytNEGATIVE;
              for( int i=(int)m_szValue.size()-1; i>0; i-- )
                m_pLineStream->putback( m_szValue.at(i) );
            }
            else
            {
              ASSD( m_szValue=="+INF" );
              tok = nextToken;
            }
          }
        }
        break;
      }
    }
    else if( eytPLUS==tok )
    {
      switch( Parm.prevTok[0] )
      {
      case eytPLUS:
        {//ошибочно написали ++
          eYToken_value nextToken = GetTokenNoReset(input);
          KTHROW( nextToken==eytTEXT );
          KTHROW( Parm.prevTok[1]==eytTEXT );
          input.get(ch);
          OutputDebugString( "Встретилась последовательность ++ между 2-я операндами, превращаю в простой +" );
          tok = eytSKIP;
        }
        break;
      }
    }
    break;
  case '&':
    tok = eytBITAND;
    break;
  case '#':
    m_szValue += ch;
    while( input.get(ch) )
    {
      if( isalpha(ch) )
      {
        m_szValue += ch;
        continue;
      }
      input.putback(ch);
      break;
    }
    tok = eytTEXT;
    break;
  case '$':
    tok = eytNUMBER;
    m_szValue += "0x";
    while( input.get(ch) )
    {
      if( isdigit(ch) )
      {
        m_szValue += ch;
        continue;
      }
      input.putback(ch);
      break;
    }
    break;
  case '0': case '1': case '2': case '3': case '4':
  case '5': case '6': case '7': case '8': case '9':
  case '.':
    {
      BOOL bAlpha = FALSE;
      BOOL b_ = FALSE;
      m_szValue += ch;
      tok = eytNUMBER;
      BOOL bE = FALSE;
      while( input.get(ch) )
      {
        if( isdigit(ch) )
        {
          m_szValue += ch;
          continue;
        }
        if( isalpha(ch) )
        {
          if( 'E'==ch )
            bE += TRUE;
          bAlpha += TRUE;
          m_szValue += ch;
          tok = eytTEXT;
          if( bE==1 && bAlpha==1 )
            tok = eytNUMBER;
          continue;
        }
        if( ch=='.' )
        {
          m_szValue += ch;
          continue;
        }
        if( (bAlpha || b_) && (ch=='[' || ch==']') )
        {
          m_szValue += ch;
          continue;
        }
        if( '_'==ch )
        {
          m_szValue += ch;
          b_ += TRUE;
          continue;
        }
        if( '-'==ch && 1==bE )
        {
          if( 1==bAlpha )
          {
            m_szValue += ch;
            bE += TRUE;
            continue;
          }
        }
        break;
      }
      input.putback(ch);
    }
    break;
  case '=':
  case '>':
  case '<':
    tok = eYToken_value(ch);
    if( '='==ch )
      tok = eytEQUAL;
    input.get(ch);
    if( '>'==ch )
    {
      ASS( tok==eytLIT );
      tok = eytNE;
    }
    else if( '<'==ch )
    {
      ASS( tok==eytEQUAL );
      tok = eytLE;
      break;
    }
    else if( '='==ch )
    {
      switch(tok)
      {
      case '=':
        ASS(0);
        break;
      case eytBIG:
        tok = eytGE;
        break;
      case eytLIT:
        tok = eytLE;
        break;
      default:
        input.unget();
      }
    }
    else if( '@'==ch )
    {
      switch(tok)
      {
      case eytLIT:
        tok = eytSHIFT_LEFT;
        break;
      case eytBIG:
        tok = eytSHIFT_RIGHT;
        break;
      default:
        KTHROW(!"уж совсем хитро");
      }
    }
    else
      input.unget();
    break;
  case '{':
    m_bUnderComment = true;
    while( input.get(ch) && '}'!=ch )
    {
    }
    if( '}'==ch )
      m_bUnderComment = false;
    tok = eytCOMMENT;
    break;
  case '"':
    while( input.get(ch) )
    {
      if( '"'==ch )
        break;
      m_szValue += ch;
    }
    tok = eytSTRING;
    break;
  case ']':
    tok = eytINDEX_CLOSE;
    break;
  case '\'':
    KKK();
  case '%':
  default:
    if( !isalpha(ch) && '%'!=ch )
    {
      if( '\''!=ch )
        break;
    }
    m_szValue += ch;
    if( '\''==ch )
    {
      while( input.get(ch) )
      {
        m_szValue += ch;
        if( '\''==ch )
          break;
      }
    }
    {
      BOOL bCanMinus = TRUE;
      while( input.get(ch) )
      {
        if( isalpha(ch) || isdigit(ch) || '_'==ch )
        {}
        else if( ch=='.' )
          bCanMinus = FALSE;
        else if( bCanMinus && ch=='-' )
        {
          if( m_Aliases.end() != m_Aliases.find( m_szValue ) )
            break;
          if( m_Defines.end()!=m_Defines.find(m_szValue) )
            break;
          if( IsVar( m_szValue.c_str() ) )
            break;
        }
        else if( '['==ch )
        {
          string testNext;
          bool bOnlyDig = false;
          testNext += ch;
          while( input.get(ch) )
          {
            testNext += ch;
            if( isdigit(ch) )
            {
              bOnlyDig = true;
              continue;
            }
            else if( ']'==ch )
              break;
            else
              break;
          }
          if( bOnlyDig )
            m_szValue += testNext;
          else
          {
            for( int i=(int)testNext.size()-1; i>0; i-- )
              m_pLineStream->putback( testNext.at(i) );
            tok = eytTEXT_INDEX_OPEN;
            return tok;
          }
          tok = eytTEXT;
          return tok;
        }
        else if( ']'==ch )
        {
          break;
        }
        else
          break;
        m_szValue += ch;
      }
      KKK();
    }
    if( m_szValue=="END" && ' '==ch )
    {
      char testend[8] = { 0 };
      int n = 0;
      while( input.get(testend[n++]) )
      {
        if( !_strcmpi(testend, "IF") )
        {
          m_szValue += " IF";
          break;
        }
        else if( !_strcmpi(testend, "SWITCH") )
        {
          m_szValue += " SWITCH";
          break;
        }
        KTHROW( n<_countof(testend) );
      }
    }
    else
      input.putback( ch );
    if( tok==unknown )
      tok = eytTEXT;
    break;
  }

  if( tok==unknown ) 
   KTHROW_TRUE( tok==unknown);

  return tok;
}
