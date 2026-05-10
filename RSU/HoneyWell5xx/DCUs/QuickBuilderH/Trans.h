#pragma once
#include <rsuErr.h>
#include <vector>
#include <macros/FixString.h>

enum eToken_value
{
  etUnknown = 0,
  etEndInData,
  etEndLine,
  //etEndExpr = ';',
  etCOMMA = ',',
  etDblPoint = ':',
  etGT='>', etLT='<', etASSIGN='=', etNE='!',
  etPLUS='+',	etMINUS='-', etMUL='*',	etDIV='/',
  etLP='(',	etRP=')',
  etBITOR='|', etBITAND='&',
  etTEXT = 1000,
  etNUMBER,
  etVarFieldAsTxt,
  etRealVarField,
  etEQ,
  etGE, etLE,
  etOR, etAND, etNOT,
  etNEGATIVE,//отрицание следующего за ним
  etIF,
  etTHEN, etELSE, etEND_IF,
  etON,
  etOFF,
  etFUNCTION_HardCoded,
  etPOINTID,
  etCALL,
  etPmmCmdToCM,
  etPmmCmdToSM,
  etPmmCmdToSMPmp,
  etSelect,
  etCase,
  etEnd_Select,
  etSet,
};

enum eStOP { _no_op = -1, OP_NOP = 100, OP_LoadVar, OP_AddrVar, OP_Operation, OP_Function,
              OP_IF, OP_ELSE };

enum eFunctions
{
  efHZ,
  efPARAMVALUE,
};

struct SOPElement
{
  enum
  {
    eNoAssigned = -1,
    eN,
    eOP,
    eTOK,
  }Type;
  union
  {
    eStOP OP;
    eToken_value tok;
    union
    {
      int N;
      eFunctions F;
    };
  };
  SOPElement()
  {
    Type = eNoAssigned;
    N = 0;
  }
  SOPElement(eStOP thr)
  {
    Type = eOP;
    OP = thr;
  }
  SOPElement(eToken_value thr)
  {
    Type = eTOK;
    tok = thr;
  }
  SOPElement(int thr)
  {
    Type = eN;
    N = thr;
  }
  bool operator ==( int n )
  {
    ASSD( eN==Type );
    return N==n;
  }
  operator int()
  {
    ASSD( eN==Type );
    return N;
  }
  operator eToken_value()
  {
    ASSD( Type == eTOK );
    return tok;
  }
  operator eStOP()
  {
    ASSD( Type == eOP );
    return OP;
  }
};

struct KVar
{
  enum eType 
  { 
    None,
    VarFieldText,
    ConstInteger,//переменная
    ConstFloat,//переменная
    ConstBoolTF,
    Ref2FieldVar,
  }locType;

  KVar()
  {
    locType = None;
  }

  KVar( LPCSTR pszName, eType type )
  {
    szName = pszName;
    locType = type;
  }

  bool operator==( LPCSTR pOther )
  {
    return _strcmpi( szName.c_str(), pOther )==0 ? true : false;
  }

  SFixString<64> szName;
};

typedef std::vector<KVar> tVarCont_;
typedef std::vector<SOPElement> tOPCont_;
