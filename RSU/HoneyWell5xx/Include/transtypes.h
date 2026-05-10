#pragma once
#include <exception>
#include <crossplatform.h>
#include <string>


#include <QtCore/QtGlobal>
#ifdef SCRIPTOH5XX_EXPORTS
#define SCRIPTOH5XX_API Q_DECL_EXPORT
#else
#define SCRIPTOH5XX_API Q_DECL_IMPORT
#endif

#include <QDebug>

#define KTHROW(V) if( !(V) ) throw ns_translator::KThrow(#V);
#define KTHROWm(V,M) if( !(V) ) throw ns_translator::KThrow(#M);
#define KTHROW_TRUE(V) if( (V) ) throw ns_translator::KThrow(#V);

namespace ns_translator
{
  class KThrow 
    : public std::exception
  {
  public:
    KThrow( LPCSTR msg ) : msg_( msg )
    {
      qDebug( "exception \"" );
      qDebug( msg );
      qDebug( "\"\n" );
    }
  private:
      std::string msg_;
  };

  struct SCRIPTOH5XX_API KVar
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
      MAX_TEXT_LEN = 128,
    };

    KVar()
    {
      ZeroMemory( szName, sizeof(szName) );
      Type = None;
    }

    KVar( LPCSTR pszName, eType type )
    {
      SetName( pszName );
      Type = type;
    }

    bool operator==( LPCSTR pOther )
    {
      return _strcmpi( szName, pOther )==0 ? true : false;
    }

    char szName[MAX_TEXT_LEN];
    void SetName( LPCSTR pszName )
    {
      strcpy_s( szName, pszName );
    }
  };

  enum eYToken_value
  {
    eytSKIP = -4,
    eytTAB = -3, endLine = -2, unknown = -1,
    eytBIG='>', eytLIT='<',
    eytPLUS='+',	eytMINUS='-',	eytMUL='*',	eytDIV='/', eytPOW='^',
    eytLP='(',	eytRP=')', eytPERCENT = '%',
    eytCOMMA = ',', eytExecCase = ':', eytEndExpr = ';',

    eytCOMMENT = 1000,
    eytTEXT, //eytCONDITION,
    eytNUMBER, eytDOUBLEkeyword, eytINTEGERkeyword,
    eytBITAND/*='&'*/, eytAND/*='$'*/, eytNOT/*='@'*/,
    eytPROGRAM, eytDEFINE,// eytNO_SYS_ID,
    eytALIAS, eytIF, eytTHEN, eytELSE, eytELSEIF, eytEND_IF/*, eytFLT*/, eytFUNCTION, eytEND, eytOR, eytBITOR,
    eytEQUAL, eytNE, eytGE, eytLE,
    eytSWITCH, eytCASE, eytOTHERWISE, eytEND_SWITCH,
    eytSHIFT_RIGHT, eytSHIFT_LEFT,
    eytEXIT, eytLABEL, eytDIM, eytSTRING,
    eytNEGATIVE,//отрицание следующего за ним
    eytMOD,
    eytASSIGN,
    eytTEXT_INDEX_OPEN,
    eytINDEX_CLOSE,
    eytIF_Q, eytELSE_Q,
  };

  enum eYFunctions
  {
    eyfNoFunc = -1,
    eyfABS = 0,
    eyfSQRT,
    eyfPOWER,
    eyfINT,
    eyfLOG,
    eyfDLIMIT,
    eyfTRUNC,
    eyfSTRINGREPLACE,
    eyfSTRINGINSTRING,
    eyfSTRINGMID,
    eyfSTRINGTOINTG,
    eyfSTRINGLEN,
    eyfSTRINGLEFT,
    eyfSTRINGRIGHT,
    eyfLOGDATACHANGEEVENT,
    eyfSTRINGFROMREAL,
    eyfTEXT,
    eyfNOW,
    eyfMAX,
    eyfISNAN,
    eyfEXP,
    eyfATN,
    eyfSIN,
    eyfLN,
    eyfMIN,
    eyfISFIN,
    eyfAVG,
  };

  enum eStOP { _no_op = -1, OP_NOP = 100, OP_LoadVar, OP_AddrVar, OP_Operation, OP_Function, OP_IF, OP_GOTO_OTHER_ELSE, OP_SWITCH, OP_CASE, OP_OTHERWISE, OP_BREAKTO,
              OP_EXIT, OP_LABEL, OP_LoadVarIndex, OP_AddrVarIndex };

  struct SCRIPTOH5XX_API SOPElement
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
    SOPElement& operator = (int thr)
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
