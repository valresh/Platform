#pragma once
#include "FlexBlockBase.h"
#include "../FlexRsuTranslator/FlexTranslator.h"

namespace ns_Script
{
  struct KCalcVar : ns_translator::KVar
  {
    KCalcVar() 
      : type( evtHZ )
    {}
    DWORD type;
    BYTE *pMem;
    bool Numb();
  protected:
    union
    {
      tDigital32 intValue;
      tAnalog32 fltValue;
    };
  };
}

class KScriptableBlock
  : public KFlexBlockBase
  , protected ns_translator::ITranslatorOwner
{
  struct SScriptParams
  {
    DWORD cOPs, cVars;
  }m_scriptParams;
  BYTE *m_pRawScript;
  DWORD m_rawScriptSize;

  ns_Script::KCalcVar *m_pVars;
  ns_translator::SOPElement *m_pOPs;
  int m_nInited;
public:
protected:
  //ITranslatorOwner
  virtual bool IsVar( LPCSTR pszVarName );
protected:
  void AllocScriptCode( KFlexTranslator &trans );
  bool ExecExpression( SCallParams &parm );
  void InitSync();
private:
  void InitVars();
  void Operation( ns_translator::eYToken_value cmd );
  void Function( ns_translator::eYFunctions nF );
};
