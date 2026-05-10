#include "FlexTranslator.h"
#include <rsuErr.h>

using namespace ns_translator;
using namespace std;

eYToken_value KFlexTranslator::GetToken( istream &input )
{
  m_szValue.clear();
  return GetTokenNoReset( input );
}

eYToken_value KFlexTranslator::GetTokenNoReset( istream &input )
{
  eYToken_value tok( unknown );

  char ch = 0;
  do 
  {
    if(!input.get(ch)) 
      return unknown;
  }while( ch!='\n' && (ch==' ' || ch=='\t') );

  switch( ch )
  {
  case '\r':
  case '\n':
    return endLine;
  case '\t':
    return eytTAB;
    // Operators
  case eytEndExpr:
  case eytExecCase:
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
    if( eytMINUS==tok )
    {
      switch( Parm.prevTok[0] )
      {
      case eytASSIGN:
      case eytLP:
      case eytPLUS:
      case eytMUL:
      case eytMINUS:
      case eytDIV:
      case eytPOW:
      case eytGT:
      case eytLT:
      case eytEQUAL:
      case eytNE:
      case eytGE:
      case eytLE:
        tok = eytNegation;
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
          //OutputDebugString( "Встретилась последовательность ++ между 2-я операндами, превращаю в простой +" );
          tok = eytSKIP;
        }
        break;
      }
    }
    break;
  case '0': case '1': case '2': case '3': case '4':
  case '5': case '6': case '7': case '8': case '9':
  case '.':
    {
      BOOL bAlpha = false;
      m_szValue += ch;
      tok = eytNUMBER;
      BOOL bE = false;
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
            bE += true;
          bAlpha += true;
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
        if( bAlpha )
        {
          if( '-'==ch || '_'==ch )
          {
            m_szValue += ch;
            continue;
          }
        }
        break;
      }
      input.putback(ch);
    }
    break;
  case '!':
    KKK();
  case '=':
  case '>':
  case '<':
    tok = eYToken_value(ch);
    input.get(ch);
    if( '>'==ch )
    {
      ASS( tok==eytLT );
      tok = eytNE;
    }
    else if( '<'==ch )
    {
      ASS( tok==eytASSIGN );
      tok = eytLE;
      break;
    }
    else if( '='==ch )
    {
      switch(tok)
      {
      case '=':
        tok = eytEQUAL;
        break;
      case eytGT:
        tok = eytGE;
        break;
      case eytLT:
        tok = eytLE;
        break;
      case '!':
        tok = eytNE;
        break;
      default:
        input.unget();
      }
    }
    else
      input.unget();
    break;
  case '&':
    tok = eytInverseByVar;
  default:
    if( !isalpha(ch) && !isdigit(ch) && '&'!=ch )
      break;
    m_szValue += ch;
    while( input.get(ch) )
    {
      if( isalpha(ch) || isdigit(ch) )
      {}
      else
        break;
      m_szValue += ch;
    }
    if( tok==unknown )
      tok = eytTEXT;
    else
    {
      ASSD(eytInverseByVar==tok);
      m_szValue.erase( 0, 1 );
      if( !strncmp(m_szValue.c_str(), "NOT", 3) )
      {}
      else if( !strncmp(m_szValue.c_str(), "ST", 2) )
      {
        tok = eytLoadAddr;
      }
      else
      {
        ASSD(0);
      }
    }
    input.putback( ch );
    break;
  }

  if( tok==unknown ) 
    KTHROW_TRUE( tok==unknown);

  return tok;
}
