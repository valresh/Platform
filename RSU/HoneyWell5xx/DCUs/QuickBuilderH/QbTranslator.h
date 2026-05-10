#pragma once
#include "Trans.h"
#include <sstream>
#include <stack>
#include <QbDataTypes_.h>

struct KTransParm
{
  enum
  {
    MAX_IF = 10, // Макс уровень вложенности IF
    MAX_PREV_TOK = 10,
  };
  struct SIfElseBlocks
  {
    eStOP activeOper;
    eToken_value ifBlock, elseBlock;
    SIfElseBlocks()
    {
      Reset();
    }
    void operator = ( eToken_value tok )
    {
      switch( activeOper )
      {
      case OP_IF:
        ifBlock = tok;
        break;
      case OP_ELSE:
        elseBlock = tok;
        break;
      default:
        ASSD(0);
      }
    }
    operator eToken_value()
    {
      switch( activeOper )
      {
      case OP_IF:
        return ifBlock;
      case OP_ELSE:
        return elseBlock;
      }
      ASSD(0);
      return etUnknown;
    }
    operator eStOP()
    {
      return activeOper;
    }
    void operator = ( eStOP op )
    {
      activeOper = op;
    }
    void Reset()
    {
      activeOper = OP_NOP;
      ifBlock = elseBlock = etUnknown;
    }
  };
  bool StartOperator;
  bool TestSpecIF;
  bool SpecIF;
  int AddrIfElseIfOnLevel[MAX_IF][MAX_IF];
  int CountIfElseIf[MAX_IF];
  int LevIF;

  int ifOpenThenClose;

  int skipShiftTokHistory;
  eToken_value prevTok[MAX_PREV_TOK], curTok, tokAfterAnalize;

  KTransParm()
  {
    InitAll();
  }
  void Init()
  {
    StartOperator = true;
    TestSpecIF = true;
    SpecIF = false;
  }
  void InitAll()
  {
    Init();
    ifOpenThenClose = 0;
    ZeroMemory( AddrIfElseIfOnLevel, sizeof( AddrIfElseIfOnLevel ) );
    ZeroMemory( CountIfElseIf, sizeof(CountIfElseIf) );
    LevIF = -1;
    tokAfterAnalize = curTok = etUnknown;
    for( int i=0; i<_countof(prevTok); i++ )
      prevTok[i] = etUnknown;
  }
};

class KQbTranslator 
{

  typedef std::stack<eToken_value> tTokensStack_;
  typedef std::stack<eFunctions> tFunctionsStack_;

  tVarCont_ m_Vars;
  tOPCont_ m_OPs;

  tTokensStack_ m_Stack;
  tFunctionsStack_ m_Functions;

  BOOL m_bIsLogicOperation;
  KTransParm Parm;

  std::string m_Value;

  bool m_bUnderComment;

  std::istream *m_pLineStream;
  std::istream *m_pFullStream;
  int m_nLine;

  struct SSepToken
  {
    eToken_value tok;
    std::string szValue;
    int nLine;
  };
  typedef std::vector<SSepToken> tTokensCont_;
  tTokensCont_ m_tokenTrain;

public:
  KQbTranslator();
public:
  void ParseText( LPCSTR pszOneLine, bool textAsName = false );
  void GetCodeOPs( tOPCont_ &Ops, tVarCont_ &Vars );
private:
  bool FillTokenTrain( bool textAsName, LPCSTR pszLine = NULL );
protected:
  void NextTokens();
  std::istream &GetLine( std::istream &_ist, std::string &str );
  template<typename T>
  void AddToOP( T st )
  {
    SOPElement el(st);
    m_OPs.push_back( el );
  }
  template<typename T>
  void AddToOP( eStOP st, T n )
  {
    AddToOP( st );
    AddToOP( n );
  }

  template<typename T>
  void AddToOP( eStOP st, T n1, T n2 )
  {
    AddToOP( st );
    AddToOP( n1 );
    AddToOP( n2 );
  }
protected:
  bool AnalizeTok( eToken_value &tok, std::string &szVal );
  void CloseIf( eToken_value tok );
  eToken_value GetToken( std::istream &input );
  bool IsSys( eToken_value &tok, std::string &szVal );
  bool IsConst( eToken_value &tok, std::string &szVal );
  bool IsNotSupported( eToken_value &tok, std::string &szVal );
  int IsFunc( LPCSTR pszName );
  int AddVar( LPCSTR pszName, KVar::eType Type );
  int Prior( eToken_value AOP );
  bool IsArith( eToken_value AOP );
};