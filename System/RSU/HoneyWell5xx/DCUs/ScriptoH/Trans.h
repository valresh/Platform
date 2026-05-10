#pragma once
#include "./export.h"
#include "transtypes.h"
#include <istream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <macros/MethodOnDestroy.h>
#include <rsuErr.h>


#pragma warning( push )
#pragma warning( disable : 4251 )

namespace ns_translator
{
  struct SCRIPTOH5XX_API KTransParm
  {
  private:
     bool m_bAction;
  public:
    enum
    {
      MAX_IF = 256, // Макс уровень вложенности IF
      MAX_SWITCH = 1,// -//- switch
      MAX_CASE = 10,
      MAX_PREV_TOK = 10,
      MAX_LPRP = 5,//уровень вложенности открывающих скобок 
    };
    bool StartOperator;
    bool TestSpecIF;
    bool SpecIF;
    int AddrIfElseIfOnLevel[MAX_IF][MAX_IF];
    int CountIfElseIf[MAX_IF];
    int ifPoses[MAX_IF];
    int wasEndIf[MAX_IF];
    int wasElse[MAX_IF];
    int LevIF;
    int lastLevelOnElse;

    int ifOpenThenClose;

    int skipEndLine;
    int skipShiftTokHistory;
    ns_translator::eYToken_value prevTok[MAX_PREV_TOK], curTok, tokAfterAnalize;

    int AddrSwitch[MAX_SWITCH];
    int LevSwitch;
    int AddrCase[MAX_SWITCH][MAX_CASE];
    int AddrBeforCase[MAX_SWITCH][MAX_CASE];
    int LastCase[MAX_SWITCH];

    int LPRP[MAX_LPRP];
    int levelLPRP;

    KTransParm()
    {
      InitAll( true );
    }
    void Init()
    {
      if( m_bAction )
        StartOperator = true;
      else
        StartOperator = false;
      TestSpecIF = true;
      SpecIF = false;
      skipEndLine = 0;
    }
    void InitAll( bool bAction )//действие (true) либо выражение (false), возвращающее результат
    {
      m_bAction = bAction;
      Init();
      ifOpenThenClose = 0;
      ZeroMemory( AddrIfElseIfOnLevel, sizeof( AddrIfElseIfOnLevel ) );
      ZeroMemory( CountIfElseIf, sizeof(CountIfElseIf) );
      ZeroMemory( ifPoses, sizeof(ifPoses) );
      ZeroMemory( wasEndIf, sizeof(wasEndIf) );
      ZeroMemory( wasElse, sizeof(wasElse) );
      LevIF = -1;
      lastLevelOnElse = -1;
      memset( AddrSwitch, 0, sizeof( AddrSwitch ) );
      LevSwitch = -1;
      memset( AddrCase, 0, sizeof( AddrCase ) );
      memset( AddrBeforCase, 0, sizeof( AddrBeforCase ) );
      memset( LastCase, 0, sizeof( LastCase ) );
      tokAfterAnalize = curTok = unknown;
      for( int i=0; i<_countof(prevTok); i++ )
        prevTok[i] = unknown;
      skipShiftTokHistory = 0;
      ZeroMemory( LPRP, sizeof(LPRP) );
      levelLPRP = -1;
    }
  };
}

class SCRIPTOH5XX_API KAaTranslator
{
  struct STokenElement
  {
    ns_translator::eYToken_value value;

    STokenElement& operator = ( ns_translator::eYToken_value &thr )
    {
      value = thr;
      return *this;
    }
    operator ns_translator::eYToken_value()
    {
      return value;
    }
    void Reset()
    {
      value = ns_translator::unknown;
    }
  };
  std::istream &GetLine( std::istream &_ist, std::string &str );
  bool m_bUnderComment;
public:
  KAaTranslator( class KBmBase *pOwner );
  ~KAaTranslator(void);

  void Translate( std::istream &str, bool bAction );//действие (true) либо выражение (false), возвращающее результат
  void Reset();

  void GetResult( int &kVar, int &kop );
  void GetResult( ns_translator::KVar *var, ns_translator::SOPElement *op );

protected:

  ns_translator::eYToken_value GetToken( std::istream &input );
  ns_translator::eYToken_value GetTokenNoReset( std::istream &input );
  ns_translator::eYToken_value GetTokenFromLine();
  void NextTokens( bool bOne = false );
  void AnalizeLine( std::string &szLine );
  bool AnalizeTok( ns_translator::eYToken_value &tok, std::string &szVal );
  bool IsSys( ns_translator::eYToken_value &tok, std::string &szVal );
  bool IsConst( ns_translator::eYToken_value &tok, std::string &szVal );
  LPCSTR IsDefine( std::string &szVal );

  int Prior( ns_translator::eYToken_value AOP );
  bool IsArith( ns_translator::eYToken_value AOP );

  void ArithOP( ns_translator::eYToken_value AOP );

  int IsFunc( LPCSTR pszName );
  int AddVar( LPCSTR pszName, ns_translator::KVar::eType Type );
  bool IsVar( LPCSTR pszName );

  void AddToOP( ns_translator::eStOP st )
  {
    KTHROW( kOP<(_countof(OP)-1) );
    OP[kOP++] = st;
  }

  template<typename T>
  void AddToOP( ns_translator::eStOP st, T n )
  {
    KTHROW( kOP<(_countof(OP)-1) );
    OP[kOP++] = st;
    OP[kOP++] = n;
  }

  template<typename T>
  void AddToOP( ns_translator::eStOP st, T n1, T n2 )
  {
    KTHROW( kOP<(_countof(OP)-2) );
    OP[kOP++] = st;
    OP[kOP++] = n1;
    OP[kOP++] = n2;
  }

protected:
  class KBmBase *m_pOwner;
  ns_translator::KTransParm Parm;

  struct SAliasInfo
  {
    std::string refName;
    ns_translator::KVar::eType type;
  };

  typedef std::vector<ns_translator::KVar> tVarCont_;
  typedef std::map<std::string, std::string> tStrByStrCont_;
  typedef std::map<std::string, SAliasInfo> tAliasByStrCont_;
  typedef std::map<std::string, int> tIntByStrCont_;

  STokenElement Stack[100];
  int PosSt;
  ns_translator::SOPElement OP[15000];
  int kOP;
  ns_translator::eYFunctions nFunction[10];
  int kFunction;

  tVarCont_ Var;
  tAliasByStrCont_ m_Aliases;
  tStrByStrCont_ m_Defines;

  std::string m_szValue;
  std::istream *m_pLineStream;
  std::istream *m_pFullStream;
  BOOL m_bIsLogicOperation;
};

#pragma warning( pop )
