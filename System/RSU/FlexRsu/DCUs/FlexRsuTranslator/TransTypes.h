#pragma once
#include <exception>
#include <rsuStr.h>

#define KTHROW(V) if( !(V) ) throw ns_translator::KThrow(#V);
#define KTHROWm(V,M) if( !(V) ) throw ns_translator::KThrow(#M);
#define KTHROW_TRUE(V) if( (V) ) throw ns_translator::KThrow(#V);

namespace ns_translator
{
  class KThrow 
    : public std::exception
  {
  public:
    KThrow( LPCSTR msg ) 
    {
      /*
      OutputDebugString( "exception \"" );
      OutputDebugString( msg );
      OutputDebugString( "\"\n" );
      */
    }
  };

  enum eYToken_value
  {
    eytSKIP = -4,
    eytTAB = -3, endLine = -2, unknown = -1,
    eytGT='>', eytLT='<', eytASSIGN='=',
    eytPLUS='+',	eytMINUS='-',	eytMUL='*',	eytDIV='/', eytPOW='^',
    eytLP='(',	eytRP=')', /*eytPERCENT = '%',*/
    eytCOMMA = ',', eytExecCase = ':', eytEndExpr = ';',

    eytTEXT = 1000,
    eytInverseByVar,
    eytNUMBER, eytDOUBLEkeyword, eytINTEGERkeyword,
    eytIF, eytTHEN, eytELSE, eytELSEIF, eytEND_IF, eytFUNCTION, eytEND, 
    eytOR, eytXOR, eytAND,
    eytEQUAL, eytNE, eytGE, eytLE,
    eytLoadAddr,
    eytNegation,
    eytBITOR, eytBITAND,
  };

  enum eYFunctions
  {
    eyfNoFunc = -1,
    eyfSR = 33,
    eyfRS,
    eyfSQRT,
    eyfABS,
    eyfRAND,
  };

  struct KVar
  {
    enum eType 
    { 
      None,
      Const,
      Reference,
      Integer,//переменная
      Double,//переменная
      UnknownTypeReference,
      Const_String,
      String,//переменная
    }Type;

    enum
    {
      MAX_TEXT_LEN = 32,
    };

    KVar()
    {
      memset( szName, 0, sizeof(szName) );
      Type = None;
    }

    KVar( LPCSTR pszName, eType type )
    {
      SetName( pszName );
      Type = type;
    }

    bool operator==( LPCSTR pOther )
    {
      return _Kstricmp( szName, pOther )==0 ? true : false;
    }

    char szName[MAX_TEXT_LEN*4];
    void SetName( LPCSTR pszName )
    {
      strcpy( szName, pszName );      
    }
  };

  enum eStOP { _no_op = -1, OP_NOP = 100, OP_LoadVar, OP_AddrVar, OP_Operation, OP_Function, OP_IF, OP_GOTO_OTHER_ELSE };

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
      eYToken_value tok;
      union
      {
        int N;
        eYFunctions F;
      };
    };
    SOPElement& operator = (eStOP thr)
    {
      Type = eOP;
      OP = thr;
      return *this;
    }
    SOPElement& operator = (DWORD thr)
    {
      Type = eN;
      N = thr;
      return *this;
    }
    SOPElement& operator = (eYToken_value thr)
    {
      Type = eTOK;
      tok = thr;
      return *this;
    }
    bool operator ==( int n )
    {
      KTHROW( eN==Type );
      return N==n;
    }
    operator int()
    {
      KTHROW( eN==Type );
      return N;
    }
    operator eYToken_value()
    {
      KTHROW( Type == eTOK );
      return tok;
    }
    operator eStOP()
    {
      KTHROW( Type == eOP );
      return OP;
    }
  };
}
