#ifndef VBSCRIPT_H
#define VBSCRIPT_H
//
//#include <UniBuffer.h>
#include "TagSPAN.h"
#include "TagTextArea.h"
#include "TagDIV.h"
#include <vector>
#include <map>
#include <set>
//
struct SToken
  {
  DWORD dw;
  DWORD id;
  char* txt;
  union
    {
    CTag* mTag;
    cross::CWnd* mWnd;
    int   nVar;
    };
  };
//
struct SValueValue
  {
  EValueType eVal;
  void* Void;
  CTag* mTag;
  AcceptCallback Accept;
  SValueValue() { memset(this, 0, sizeof(SValueValue)); }
  };
//
enum EValueTypeScript : unsigned char
  {
  _enumValueUnk = enumValueUnk,
  _enumValueDbl = enumValueDbl,
  _enumValueInt = enumValueInt,
  _enumValueChr = enumValueChr,
  _enumValueBol = enumValueBol,
  _enumValueFlt = enumValueFlt,
  _enumValueStr = enumValueStr,
  _enumValueI64 = enumValueI64,
  _enumValueTag = 10,
  _enumValueCastomProperty = 11,
  _enumValueObjects = 12,
  _enumValueVariant = 13,
  };
//

enum OBJTYPE 
{
    otTAG = 0,
    otWindowEvent, 
    otARRAY,
};

struct SCTag   /* должна быть меньше 512байт - умалчиваемого размера для неопределеннных тпеременных */
  {
  char m_name[256];
  EValueTypeScript eVal;
  CTag *m_pTag;
  UINT  nEndPipeCount;
  OBJTYPE objectType;
  SCTag() { memset(this, 0, sizeof(SCTag)); }
  };

struct DATA_CHANGED : cross::SParamValueH
  {
  EValueTypeScript eVal;
  union
  {
      bool bPrevVal;
      int  nPrevVal;
      double dPrevVal;
      char sPrevVal[512];
  };
  DWORD    dwChangeTime;
  bool     bChanged;
  DATA_CHANGED() {eVal=_enumValueUnk; dwChangeTime=0; bChanged=false; memset(sPrevVal, 0, sizeof sPrevVal);}
  };
//
#define MAX_LOCAL 512
//
enum _types
 {
 TYPE_UNKNOWN = 0,
 TYPE_BOOL  = 1,   // 1
 TYPE_CHAR  = 2,
 TYPE_INT   = 3,   // 4
 TYPE_FLOAT = 4,   // 8
 TYPE_TIME  = 5,   // 8
 TYPE_STRING = 6,   // Переменная
 TYPE_PARAM = 7,   // sizeof(SParamValueH)
 TYPE_VALUE = 8,   // sizeof(SValueValue)
 TYPE_TAG   = 9,
 TYPE_CUSTOMPROPERTY = 0xA,
 TYPE_OBJECT  = 0xB,
 TYPE_LOCAL = 0x10,// локальная переменная
 };
//
enum _keys
{
 KEY_IF        = 1,
 KEY_THEN      = 2,
 KEY_ELSE      = 3,
 KEY_ELSEIF    = 4,
 KEY_END       = 5,
 KEY_DIM       = 6,
 KEY_SUB       = 7,
 KEY_THENIF    = 8, // Простой If Then без Else
 KEY_CALL      = 9,
 KEY_FOR       = 10,
 KEY_TO        = 11,
 KEY_NEXT      = 12,
 KEY_EXIT      = 13,
 KEY_EXITSUB   = 14,
 KEY_ARRAY     = 15,
 KEY_ARRAYELEMENT = 16,
};
//
#define _KEY_BEGIN     1
#define _KEY_END       16
//
inline bool IsKey( SToken& n )
  {
  return _KEY_BEGIN <= n.dw && n.dw <= _KEY_END;
  }
//
enum _functions
{
 FUNC_MSGBOX      = 50,
 FUNC_ABS         = 51,
 FUNC_CDBL        = 52,
 FUNC_CINT        = 53,
 FUNC_CSTR        = 54,
 FUNC_ISNUMERIC   = 55,
 FUNC_CHR         = 56,
 FUNC_BRACE       = 57,
 FUNC_FORM_N      = 58,
 FUNC_CREATEPOPUOWINDOW = 59,
 FUNC_TYPENAME    = 60,
 FUNC_TRIM        = 61,
 FUNC_LOGMESSAGE  = 62,
 FUNC_LTRIM       = 63,
 FUNC_RTRIM       = 64,
 FUNC_INSTR       = 65,
 FUNC_MID         = 66,
 FUNC_LEFT        = 67,
 FUNC_RIGHT       = 68,
 FUNC_GETCUSTOMPROPERTY = 69,
 FUNC_VARTYPE     = 70,
 FUNC_GETELEMENTBYID = 71,
 FUNC_CloseAllWindows = 72,
 FUNC_DATAVALUE   = 73,  /* когда в PARAM_DATAVALUE выражение */
 FUNC_CSNG        = 74,
 FUNC_LCASE       = 75,
 FUNC_UCASE       = 76,
 FUNC_ROUND       = 77,
 FUNC_page_GetStyleClassProperty = 78,
 FUNC_FindPopup   = 79,
 FUNC_CLOSE_POPUP = 80,
 FUNC_page_stylesheetSrc = 81,
 FUNC_EVAL        = 82,
 FUNC_DisplayDataRepositoryGetValue = 83,
 FUNC_REPLACE     = 84,
 FUNC_ISNAN       = 85,
 FUNC_DisplayDataRepositoryPutValue = 86,
 FUNC_STRCOMP     = 87,
 FUNC_MR_Comment   = 88,
 FUNC_LEN         = 89,
 FUNC_PAGE_GETATTRIBUTE = 90,
 FUNC_PAGE_SETATTRIBUTE = 91,
 FUNC_ISNULL      = 92, 
 FUNC_AlarmStateIconfactory = 93,
 FUNC_GETATTRIBUTE = 94,
 FUNC_SETATTRIBUTE = 95,
 FUNC_SecurityLevel = 96,
 FUNC_SET_DATAVALUE = 97,
};
#define _FUNC_BEGIN      50
#define _FUNC_END        97
//
inline bool IsFunc( SToken& n )
  {
  return _FUNC_BEGIN <= n.dw && n.dw <= _FUNC_END;
  }
//
enum _operates
{
 OPER_OR        =  150,
 OPER_MOD       =  151,
 OPER_SUB       =  152,// -
 OPER_ADD       =  153,// +
 OPER_DIV       =  154,// /
 OPER_DIV_INT   =  155,// '\'
 OPER_EQ        =  156,// =
 OPER_BEQ       =  157,// >=
 OPER_MEQ       =  158,// <=
 OPER_NEQ       =  159,// <>
 OPER_AND       =  160,// &
 OPER_MUL       =  161,// *
 OPER_MEN       =  162,// <
 OPER_BOL       =  163,// >
 OPER_NOT       =  164, // not
 OPER_IS        =  165, //
 OPER_EQ_EQ     =  166,// == Для логических выражений
};
//
#define _OPER_BEGIN      150
#define _OPER_END        165
//
#define WINDOW_CLOSE     167
#define _MAX_FUNCTION    (WINDOW_CLOSE+1)
//
inline bool IsOper( SToken& n )
  {
  return _OPER_BEGIN <= n.dw && n.dw <= _OPER_END;
  }
inline bool IsCompareOper( SToken& token )
{
  return(token.dw==OPER_EQ ||token.dw==OPER_MEN ||token.dw==OPER_BOL ||token.dw==OPER_BOL ||token.dw==OPER_NEQ ||token.dw==OPER_BEQ ||token.dw==OPER_MEQ);
}
//
enum _consts
{
 CONST_FALSE   =   200,
 CONST_TRUE    =   201,
// Цвета
 CONST_VBBLACK =   202,
 CONST_VBRED   =   203,
 CONST_VBGREEN =   204,
 CONST_VBYELLOW =  205,
 CONST_VBBLUE   =  206,
 CONST_VBMAGENTA = 207,
 CONST_VBCYAN    = 208,
 CONST_VBWHITE   = 209,
// Кнопки у MessageBox-а,
 CONST_VBOKONLY             = 210,
 CONST_VBOKCANCEL           = 211,
 CONST_VBABORTRETRYIGNORE   = 212,
 CONST_VBYESNOCANCEL        = 213,
 CONST_VBYESNO              = 214,
 CONST_VBRETRYCANCEL        = 215,
 CONST_VBCRITICAL           = 216,
 CONST_VBQUESTION           = 217,
 CONST_VBEXCLAMATION        = 218,
 CONST_VBINFORMATION        = 219,
 CONST_VBDEFAULTBUTTON1     = 220,
 CONST_VBDEFAULTBUTTON2     = 221,
 CONST_VBDEFAULTBUTTON3     = 222,
 CONST_VBDEFAULTBUTTON4     = 223,
 CONST_VBAPPLICATIONMODAL   = 224,
 CONST_VBSYSTEMMODAL        = 225,
 CONST_VBTASKMODAL          = 226,
// Возвращаемые значения MessageBox-а
 CONST_VBOK                 = 227,
 CONST_VBCANCEL             = 228,
 CONST_VBABORT              = 229,
 CONST_VBRETRY              = 230,
 CONST_VBIGNORE             = 231,
 CONST_VBYES                = 232,
 CONST_VBNO                 = 233,
//
 CONST_HIDDEN    = 234,
 CONST_VISIBLE   = 235,
 CONST_BAD       = 236,
//
 CONST_RGB       = 237,
 CONST_ERR_NUMBER = 238, 
 CONST_ERR_DESCRIPTION = 239,
 CONST_WINDOW_EXTERNAL_ENVIRONMENT = 240,
 CONST_TRANSPARENT = 241,
 CONST_NOTHING = 242,
 CONST_SPDEBUG = 243,
 CONST_WINDOW_APPLICATION_CURRENT_PAGE = 244,
 CONST_VBCR   = 245,
 CONST_VBLF  = 246,
 CONST_VBCRLF  = 247,
 CONST_WINDOW_EXTERNAL_OPERATOR_ID = 248,
};
//
#define _CONST_BEGIN     200
#define _CONST_END       248
//
inline bool IsConst( SToken& n )
  {
  return _CONST_BEGIN <= n.dw && n.dw <= _CONST_END;
  }
//
#define VAR_INTEGER      400
#define VAR_DOUBLE       401
#define VAR_STRING       402
//
inline bool IsVar( SToken& n )
  {
  return VAR_INTEGER <= n.dw && n.dw <= VAR_STRING;
  }
//
enum _params
{
 PARAM_QUALITYBAD = 500,
 PARAM_DATAVALUE  = 501,
 PARAM_DATACHANGED = 502,
 PARAM_ISNAN      = 503,
 PARAM_UnacknowledgedAlarm = 504,
 PARAM_ALARMSEVERITY = 505,
 PARAM_QUALITYGOOD = 506,
 PARAM_DATAQUALITY =   507,
 PARAM_DATAALARM = 508,
 PARAM_DATAEXIST = 509
};
//
#define _PARAM_BEGIN     500
#define _PARAM_END       509
//
inline bool IsParam( SToken& n )
  {
  return _PARAM_BEGIN <= n.dw && n.dw <= _PARAM_END;
  }
//
enum _values
{
 VALUE_TEXTCOLOR         = 600,
 VALUE_STYLE_VISIBILITY  = 601,
 VALUE_VALUE             = 602,
 VALUE_VISIBLERANGEHI    = 603,
 VALUE_VISIBLERANGELO    = 604,
 VALUE_LEVELFILLCOLOR    = 605,
 VALUE_ROTATION          = 606,
 VALUE_FILLCOLOR         = 607,
 VALUE_HOSTPOPULATION    = 608,
 VALUE_LINECOLOR         = 609,
 VALUE_STYLE_PIXELHEIGHT = 610,
 VALUE_STYLE_PIXELWIDTH  = 611,
 VALUE_INNERTEXT         = 612,
 VALUE_NUMERICDISPLAYFORMAT  = 613,
 VALUE_LINECOLORBLINK    = 614,
 VALUE_GRADIENTFILLCOLOR = 615,
 VALUE_LINEWIDTH         = 616,
 VALUE_TEXTCOLORBLINK    = 617,
 VALUE_CANCELCHANGE      = 618,
 VALUE_TAG               = 619, 
 VALUE_CUSTOMPROPERTY    = 620,
 VALUE_TYPENAME          = 621,
 VALUE_OBJECT            = 622,
 VALUE_STYLECLASS        = 623,
 VALUE_ALARM             = 624,
 VALUE_TITLE             = 625,
 VALUE_DOCUMENTTITLE     = 626,
 VALUE_VISIBLERANGEORIGIN = 627,
 VALUE_STYLE_FONTWEIGHT  = 628,
 VALUE_STRARTARROW       = 629,
 VALUE_ENDARROW          = 630,
 VALUE_LINESTYLE         = 631,
 VALUE_FILLCOLORBLINK    = 632,
 VALUE_WINDOWEVENT       = 633,
 VALUE_WINDOWS_EXTERNAL_CURRENTPAGE = 634,
 VALUE_SRC               = 635,
 VALUE_STYLE_PIXELLEFT   = 636,
 VALUE_SHELVED           = 637,
 VALUE_RTN               = 638,
 VALUE_ACKED             = 639,
 VALUE_PRIORITY          = 640
};
#define _VALUE_BEGIN            600
#define _VALUE_END              640

#define WINDOW_EXT_CLOSE        700
#define WINDOW_EXT_CURRPAGE     701
#define WINDOW_EXT_S_LEVEL      702
#define WINDOW_EXT_TMZONE_T     703
#define WINDOW_EVENT            704

#define _WINDOW_BEGIN           700
#define _WINDOW_END             704
//
inline bool IsWnd( SToken& n )
  {
  return _WINDOW_BEGIN <= n.dw && n.dw <= _WINDOW_END;
  }
//
inline bool IsValue( SToken& n )
  {
  return _VALUE_BEGIN <= n.dw && n.dw <= _VALUE_END;
  }
//
inline bool IsV( char a )
  {
  BYTE b = (BYTE)toupper(a);
  if ( '0' <= b && b <= '9' ) return true;
  if ( 'A' <= b && b <= 'Z' ) return true;
  if ( L'А' <= b && b <= L'Я' ) return true;
  if ( b == '_' ) return true;
  return false;
  }
//
inline bool IsN( char a )
  {
  BYTE b = (BYTE)toupper(a);
  if ( '0' <= b && b <= '9' ) return true;
  return false;
  }
//
inline bool IsSpace( char a )
  {
  return a == ' ' || a == 0xd || a == 0xa || a == '\t';
  }
//
inline bool IsSub( SToken& n ) 
  {//не возващают результат
  return n.dw == FUNC_CREATEPOPUOWINDOW || n.dw == FUNC_LOGMESSAGE || n.dw==FUNC_CloseAllWindows || n.dw==FUNC_MR_Comment || n.dw==FUNC_DisplayDataRepositoryPutValue
                 ||  n.dw == FUNC_PAGE_SETATTRIBUTE ||  n.dw == FUNC_SETATTRIBUTE;
  }
//
struct SUniToken : public SUniBuffer
  {
  SUniToken()
    : SUniBuffer( sizeof(SToken) )
    {
    };
  //
  SToken& Obj(int n)
    {
    SToken* lst = (SToken*)m_szBuffer;
    return lst[n];
    }
  SToken& Add()
    {
    Realloc(1);
    int n = AddCurr();
    return Obj(n);
    }
  };
//
// Глобальные, локальные и временные переменные
struct GVAR
  {
  UINT  nType;// Тип переменной
  UINT  nSize;// Размер переменной
  UINT  nPlac;// Местоположение (mBuff)
  HCHAR nName;// Имя объекта (mName)
  UINT  nOrd ;// Для сортировки по именам
  UINT  nDef ;// Тип объекта
  GVAR() {memset(this, 0, sizeof *this);}
  };
//
// Простейшие операции
struct SCommand
  {
  UINT nOper;// Операция
  UINT nRes ;// Результат в GVAR
  UINT nArg1;// Аргумент1 в GVAR
  UINT nArg2;// Аргумент2 в GVAR
  UINT nArg3;// Аргумент3 в GVAR
  UINT nArg4;// Аргумент4 в GVAR
  UINT nArg5;// Аргумент5 в GVAR
  UINT nArg6;// Аргумент6 в GVAR
  SCommand() : nOper(-1), nRes (-1), nArg1(-1), nArg2(-1), nArg3(-1), nArg4(-1), nArg5(-1), nArg6(-1){}
  };
//
struct SUniGVAR : public SUniBuffer
  {
  SUniGVAR(SUniChar& rChar,SUniChar& rBuff)
  : SUniBuffer( sizeof(GVAR) )
  , m_nSort(1)
  , name(rChar)
  , buff(rBuff)
    {
    };
  //
  private:
  SUniChar& name;// Буфер для имён
  SUniChar& buff;// Буфер для переменных
  int m_nSort;//Тип сортировки
  public:
  GVAR& Obj( UINT n );
  //
  const char* Name(unsigned int n)
    {
    if ( n > AddCurr(0) ) return name.m_szBuffer;
    GVAR& obj = Obj(n);
    return name.Name(obj.nName);
    }
  //
  //
  UINT AddN( const char* d )
    {
    int nb;
    UINT n = Find( d, &nb );
    if ( n != -1 ) return n;
    // Новое имя сразу за nb
    int K = AddCurr(0);
    if ( !Realloc(1) )
      return -1;
    n = AddCurr();
    GVAR* a = (GVAR*)m_szBuffer;
    a[n].nName = name.AddBuff(d);
    if ( nb < K - 1 )
    for ( int m = K - 1; m > nb; m-- )
    a[m+1].nOrd = a[m].nOrd;
    //
    a[nb+1].nOrd = K;
    ASS( a[n].nName >=  (HCHAR)0 )
    return n;
    }
  //
  UINT Find( const char* d, int* pNB = NULL )
    {
    GVAR* a = (GVAR*)m_szBuffer;
    int K = AddCurr(0);
    int nb = -1, ne = K, no, n, Res;
    while ( ne - nb > 1 )
      {
      n = ( nb + ne ) >> 1;
      no = a[n].nOrd;
      //
      const char* dll = name.Name(a[no].nName);
      switch ( m_nSort )
        {
        case 0: Res = lstrcmp ( dll, d ); break;
        case 1: Res = lstrcmpi( dll, d ); break;
        case 2: Res = Lstrcmp ( dll, d ); break;
        };
      if ( Res == 0 )
        return no;
      if ( Res < 0 ) ne = n;
      else           nb = n;
      }
    if ( pNB ) *pNB = nb;
    return -1;
    }
  //
  UINT  AddB( UINT n )
    {
    if ( !buff.Realloc( n ) )
      return -1;
    return buff.AddCurr(n);
    }
  //
  char* Chars( UINT n )
    {
    GVAR& v = Obj(n);
    return buff.m_szBuffer+v.nPlac;
    }
  //
  cross::SParamValueH* Param( UINT n )
    {
    GVAR& v = Obj(n);
    return (cross::SParamValueH*)(buff.m_szBuffer+v.nPlac);
    }
  //
  SValueValue* Value( UINT n )
    {
    GVAR& v = Obj(n);
    return (SValueValue*)(buff.m_szBuffer+v.nPlac);
    }
  //
  void* Var( CHTML& html, UINT n, EValueTypeScript& e, bool bRightValue = false );
  //
  };
//
struct SUniComm : public SUniBuffer
  {
  SUniComm()
  : SUniBuffer( sizeof(SCommand) )
    {
    };
  SCommand& Obj( int n ){SCommand* lst = (SCommand*)m_szBuffer;return lst[n];}
  };
//
class CVBScript;
#undef  XAXA
#define XAXA CHTML& html, SUniGVAR& gVar, SCommand& comm, int& nParam, CVBScript *script /*std::map<std::string, CTag*> &vTags*/
typedef int (*LPScript)(XAXA);
//
class CVBScript
  {
  static LPScript script[_MAX_FUNCTION];
  //
  LPCTSTR mId;
  CHTML* html;
  CTag* mLink;
  std::vector<DATA_CHANGED> m_vParams; //список параметров работающих с pipe и имеющих несколько вариантов связывания: в SParamValueH pMulti!=NULL
 
  //std::vector<const std::string> m_TempStrings; //для дополнительных строк скрита
  std::vector<std::string> m_TempStrings;
  std::string lastForParam;    //имя переменной цикла
  int m_nTempCount; 
  // Сканирование кода VBScript
  bool VBScriptScan( CHTML& html, SUniToken& uni, CTag* pLink, char* txt, bool bInit );
  bool IsStdK( char*& ptr, SUniToken& uni );
  bool IsStdV( char*& ptr, SUniToken& uni, CTag* pThis, CHTML& html );
  bool IsSubroutine( char*& ptr, SUniToken& uni, CTag* pThis, CHTML& html );
  bool IsDisplayDataRepository( char*& ptr, SUniToken& uni, CTag* pThis, CHTML& html  );
  public:
    CVBScript()
    : html(NULL)
    , mLink(NULL)
    , mGVAR(mChar,mBuff)
    , m_dwCurr(0)
    , m_nTempCount(0)
      {
          m_TempStrings.reserve(1024);
      };
    //
    void Init();
    void Post();
    int Run();
    //
    std::map<std::string, CTag*> m_vTags;
    void SetVars(CHTML* _html,CTag* pLink,LPCTSTR id)
      {
      mId   =  id;
      html  = _html;
      mLink = pLink;
      Init();
      }
    DWORD m_dwCurr;
    SUniGVAR mGVAR;// Список глобальных переменных
    SUniChar mChar;// Буфер для имён переменных
    SUniChar mBuff;// Буфер для значений переменных
    SUniComm mComm;// Список команд для выполнения
    //
    bool BuildTable( SUniToken& uni );
    bool Add       ( SToken& token, const char* name );
    bool ParamValue( cross::SParamValueH& val, SToken& token );
    bool ParamValue( cross::SParamValueH& val, char* value, CTag* pTag );
    UINT AddTemp   ( UINT nType );
    UINT AddMeas   ( SCommand& comm, bool bLogic );
    UINT AddComm   ( SCommand& comm );
    int GetArgList( char* ptr, char** arg, int nMax );
    int FillArgList( const char* text, UINT* arg, int nMax );
    UINT CommandFunc( SToken& token, int nMax, UINT nReturn );
    SCommand& Comm( int n ) { return mComm.Obj(n); }
    SCommand& CommLast() { return mComm.Obj(mComm.Count()-1);}
    UINT AddCommand( UINT nOper, UINT nArg1, SToken& token, UINT* pRes, bool bLogic );
    UINT AddArgument( SToken& token, bool bLogic );
    //
    bool EqType( UINT a1, UINT a2 );
    void TestPrint( char* id );
    //
    bool Compile( char* txt, UINT* pRes, bool bLogic );
    UINT VBScriptList ( SToken* token, int nCount, bool bLogic );
    UINT VBRecursiveIf( SToken* token, int nCount );
    bool VBArgumentis( SToken* token, int nCount );
    bool Arguments( char* txt, UINT& nRes, bool bLogic );
    //
    int KeyIF   ( UINT n );
    int KeyEND  ( SToken* token, int nCount );
    int KeyLogic( SToken* token, int nCount, DWORD dwIF );
    int KeyOther( SToken* token, int nCount );
    int KeyArgus( SToken* token, int nCount );
	int KeyArgusLog( SToken* token, int nCount );
	UINT KeyArgusRLog( SToken* token, int nCount, UINT nArg1 );
    //
    int FindEnd( SToken* token, int nCount );

    UINT FillMSGBOX   ( SToken& token );
    UINT FillABS      ( SToken& token );
    UINT FillFunction ( SToken& token, UINT nType  );
    UINT FillFunctionL( SToken& token, int nMax, UINT nType  );
    UINT FillBRACE    ( SToken& token, bool bLogic );
    UINT FillFormatNum( SToken& token );
    UINT FillCreatePopupWindow( SToken& token );
    UINT FillAlarmStateIconfactory( SToken& token); 
    //UINT FillLogMessage( SToken& token );
    UINT FillGetcustomproperty ( SToken& token );
    UINT FillForTo(SToken *token, int nCount);
    UINT FillFuncWithEmptyParam(SToken& token, UINT nType );
    UINT FillFunDataValue( SToken& token, UINT nType );
    UINT FillGetAttribute( SToken& token );
    UINT FillSetAttribute( SToken& token );
    //
    bool ValueWindow( UINT dw, SValueValue& val );
  };
//
//
struct SLocalStd
  {
  char* name;
  DWORD dw;
  DWORD var;
  int dl;
  };
 std::vector<SLocalStd> stdV;
//  SLocalStd stdV[];
//
char *ReplaceOperatorSelect(const char* ptr);
#endif
