#pragma once
#include <crossplatform.h>
#include <istream>
#include <sstream>
#include <string>
#include "TransTypes.h"
#include <vector>
#include "ITranslatorOwner.h"
#include <macros/MethodOnDestroy.h>

#ifdef FLEXRSUTRANSLATOR_EXPORTS
#define FLEXTRANSLATOR_API _EXP
#else
#define FLEXTRANSLATOR_API _IMP
#endif

#pragma warning( push )
#pragma warning( disable : 4251 )

namespace ns_translator
{
  struct KTransParm
  {
    enum
    {
      MAX_IF = 2, // Макс уровень вложенности IF
      MAX_PREV_TOK = 10,
    };
    bool StartOperator;
    bool TestSpecIF;
    bool SpecIF;
    int AddrIfElseIfOnLevel[MAX_IF][MAX_IF];
    int CountIfElseIf[MAX_IF];
    int LevIF;

    int ifOpenThenClose;

    int skipEndLine;
    int skipShiftTokHistory;
    ns_translator::eYToken_value prevTok[MAX_PREV_TOK], curTok, tokAfterAnalize;

    KTransParm()
    {
      InitAll();
    }
    void Init()
    {
      StartOperator = true;
      TestSpecIF = true;
      SpecIF = false;
      skipEndLine = 0;
    }
    void InitAll()
    {
      Init();
      ifOpenThenClose = 0;
      memset( AddrIfElseIfOnLevel, 0, sizeof( AddrIfElseIfOnLevel ) );
      memset( CountIfElseIf, 0, sizeof(CountIfElseIf) );
      LevIF = -1;
      tokAfterAnalize = curTok = unknown;
      for( int i=0; i<countof(prevTok); i++ )
        prevTok[i] = unknown;
      skipShiftTokHistory = 0;
    }
  };
}

class FLEXTRANSLATOR_API KFlexTranslator
{
protected:
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

protected:
  typedef std::vector<ns_translator::KVar> tVarCont_;

  STokenElement Stack[1000];
  int PosSt;
  ns_translator::SOPElement OP[1000];
  int kOP;
  ns_translator::eYFunctions nFunction[10];
  int kFunction;
  int StackInversVars[100];
  int kInvers;

  BOOL m_bIsLogicOperation;

  tVarCont_ Var;

  std::istream *m_pFullStream;
  std::istream *m_pLineStream;
  std::string m_szValue;
  ns_translator::KTransParm Parm;
  ns_translator::ITranslatorOwner *m_pOwner;
  std::stringstream m_input;
public:
  KFlexTranslator( ns_translator::ITranslatorOwner *pOwner );
  ~KFlexTranslator(void);
  void Translate( LPCSTR pszExpr );
  void GetResult( DWORD &kVar, DWORD &kop );
  void GetResult( ns_translator::KVar *var, ns_translator::SOPElement *op );
protected:
  void AddToOP( ns_translator::eStOP st )
  {
    KTHROW( kOP<(countof(OP)-1) );
    OP[kOP++] = st;
  }

  template<typename T>
  void AddToOP( ns_translator::eStOP st, T n )
  {
    KTHROW( kOP<(countof(OP)-1) );
    OP[kOP++] = st;
    OP[kOP++] = n;
  }

  template<typename T>
  void AddToOP( ns_translator::eStOP st, T n1, T n2 )
  {
    KTHROW( kOP<(countof(OP)-2) );
    OP[kOP++] = st;
    OP[kOP++] = n1;
    OP[kOP++] = n2;
  }
protected:
  void Translate( std::istream &str );
  void NextTokens();
  std::istream &GetLine( std::istream &_ist, std::string &str );
  void AnalizeLine( std::string &szLine );
  ns_translator::eYToken_value GetTokenFromLine();
  ns_translator::eYToken_value GetToken( std::istream &input );
  ns_translator::eYToken_value GetTokenNoReset( std::istream &input );
  bool AnalizeTok( ns_translator::eYToken_value &tok, const std::string &szVal );
  int AddVar( LPCSTR pszName, ns_translator::KVar::eType Type );
  int ArithOP( ns_translator::eYToken_value AOP );
  bool IsSys( ns_translator::eYToken_value &tok, const std::string &szVal );
  int IsFunc( LPCSTR pszName );
  bool IsVar( LPCSTR pszName );
  int Prior( ns_translator::eYToken_value AOP );
  bool IsArith( ns_translator::eYToken_value AOP );
  void HandleInvers( STokenElement C );
};

#pragma warning( pop )
