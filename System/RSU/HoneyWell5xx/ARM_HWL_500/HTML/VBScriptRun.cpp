#include "StdAfx.h"
#include "../ARM.h"
#include "../AplicFrm.h"
#include <math.h>
#include "HTML.h"
#include "VBScript.h"

#include "Style.h"
#include <crosswindows.h>
#include <Profile.h>
#include <map>

//
SValueValue GetTagParam(CHTML& html, CVBScript *pScript, SUniGVAR& gVar, CTag *pTag, char *pparam, char buf[256], UINT *pnEndPipeCount = NULL);

inline double Dbl( EValueTypeScript e, void* v )
  {
  int* i; bool* b; char* c;
  switch ( e )
    {
    case enumValueDbl:
    return *(double*)v;
    case enumValueChr:
      c = (char*)v;
    return int(*c);
    case enumValueInt:
      i = (int*)v;
    return double(*i);
    case enumValueBol:
      b = (bool*)v;
    return double(*b);
    case enumValueStr:
    if(strcmp((char*)v, "1.#QNAN")==0)
      return NaN;
    return atof((char*)v);
    };
  ASSERT(0);
  return 0;
  }
//
inline int Int( EValueTypeScript e, void* v, bool bColor = false )
  {
  double* d; bool* b; char* c;
  switch ( e )
    {
    case enumValueDbl:
      d = (double*)v;
    return int(*d);
    case enumValueChr:
      c = (char*)v;
    return int(*c);
    case enumValueInt:
    return *(int*)v;
    case enumValueBol:
      b = (bool*)v;
    return int(*b);
    case enumValueStr:
      c = (char*)v;
      if ( lstrcmpi( c, "HIDDEN" ) == 0 )
        return int(enumHidden);
      if ( lstrcmpi( c, "VISIBLE" ) == 0 )
        return int(enumVisible);
      if ( lstrcmpi( c, "ON" ) == 0 )
        return int(1);
      if ( lstrcmpi( c, "STOP" ) == 0 )
        return int(1);
      if ( lstrcmpi( c, "OFF" ) == 0 )
        return int(0);
      if ( lstrcmpi( c, "#008000" ) == 0 )
        return 0x008000;
      if( c[0] == '#')
         return ColorDiez(c); //return strtol(c+1, NULL, 16);
      if ( lstrcmpi( c, "inherit" ) == 0 ) 
         return enumInherit;
      if ( lstrcmpi( c, "visible" ) == 0 ) 
          return enumVisible;
      if ( lstrcmpi( c, "hidden"  ) == 0 ) 
          return enumHidden;
      if ( lstrcmpi( c, "auto"    ) == 0 ) 
          return enumAuto;
      if ( lstrcmpi( c, "Transparent" ) == 0 )
         return 0xff000000;
       if (bColor)
       {
       if( isxdigit(c[0]) && isxdigit(c[1]) && isxdigit(c[2]) )
         {
         if( IsInt(c) && (Version == T1163_ONPZ || Version == T1165P_ONPZ))
           return  atoi(c);
         char color[16] = "#";
         strcat_s(&color[1], sizeof(color)-2, c);
         return ColorDiez(color);
         }
        _strlwr(c);
        return ColorDiez(c);
       }
     return atoi(c);
     break;
    };
  c = (char*)v;
  ASSERT(0);
  return 0;
  }
//
inline bool Bol( EValueTypeScript e, void* v )
  {
  double* d; int* i; char* c;
  switch ( e )
    {
    case enumValueDbl:
      d = (double*)v;
    return (*d) != 0;
    case enumValueChr:
      c = (char*)v;
    return (*c) != 0;
    case enumValueInt:
      i = (int*)v;
    return (*i) != 0;
    case enumValueBol:
    return *(bool*)v;
    };
  ASSERT(0);
  return 0;
  }
//
LPScript CVBScript::script[_MAX_FUNCTION];
CTag *HtmlGetObject(CHTML& html, std::map<std::string, CTag*> &vTags, SCTag *pProp, char* &ppar);
//
void CVBScript::Init()
  {
  _static bool bInit = false;
  if ( bInit ) return;
  bInit = true;
  //
  memset( script, 0, sizeof(script) );
  //
  int FuncIF      (XAXA);script[KEY_IF        ] = &FuncIF     ;
  int FuncTHEN    (XAXA);script[KEY_THEN      ] = &FuncTHEN   ;
  int FuncEmpty   (XAXA);script[KEY_ELSE      ] = &FuncEmpty  ;
                         script[KEY_END       ] = &FuncEmpty  ;
                         script[KEY_THENIF    ] = &FuncEmpty  ;
                         script[KEY_ELSEIF    ] = &FuncIF     ;
                         script[FUNC_CLOSE_POPUP] = &FuncEmpty;
  //
  int Func_FORTO  (XAXA);script[KEY_TO] = &Func_FORTO;
  int Func_NEXT  (XAXA);script[KEY_NEXT] = &Func_NEXT;
  int FuncExitSub(XAXA);script[KEY_EXITSUB] = &FuncExitSub;
  //
  int Func_MSGBOX (XAXA);script[FUNC_MSGBOX   ] = &Func_MSGBOX;
  int Func_ABS    (XAXA);script[FUNC_ABS      ] = &Func_ABS   ;
  int Func_CDBL   (XAXA);script[FUNC_CDBL     ] = &Func_CDBL  ;
  int Func_CINT   (XAXA);script[FUNC_CINT     ] = &Func_CINT  ;
  int Func_CSTR   (XAXA);script[FUNC_CSTR     ] = &Func_CSTR  ;
  int Func_ISNUME (XAXA);script[FUNC_ISNUMERIC] = &Func_ISNUME;
  int Func_CHR    (XAXA);script[FUNC_CHR      ] = &Func_CHR   ;
  int Func_ISNAN(XAXA); script[FUNC_ISNAN]      = &Func_ISNAN;

  int Func_CSNG(XAXA);script[FUNC_CSNG      ] = &Func_CSNG   ;
  int Func_LCASE(XAXA);script[FUNC_LCASE    ] = &Func_LCASE  ;
  int Func_UCASE(XAXA);script[FUNC_UCASE    ] = &Func_UCASE  ;
  int Func_ROUND(XAXA);script[FUNC_ROUND    ] = &Func_ROUND  ;
  int Func_EVAL(XAXA);script[FUNC_EVAL      ] = &Func_EVAL   ;
  int Func_REPLACE(XAXA);script[FUNC_REPLACE] = &Func_REPLACE;
  int Func_STRCOMP(XAXA);script[FUNC_STRCOMP] = &Func_STRCOMP;
  int Func_MR_Comment(XAXA);script[FUNC_MR_Comment] = &Func_MR_Comment;
  int Func_LEN(XAXA);script[FUNC_LEN]         = &Func_LEN;

  int Func_BRACE  (XAXA);script[FUNC_BRACE    ] = &Func_BRACE ;
  int Func_FORM_N (XAXA);script[FUNC_FORM_N   ] = &Func_FORM_N;
  int Func_CreatePopupWindow(XAXA); script[FUNC_CREATEPOPUOWINDOW] = &Func_CreatePopupWindow;
  int Func_Typename(XAXA); script[FUNC_TYPENAME] = &Func_Typename;
  int Func_IsNull(XAXA); script[FUNC_ISNULL] = &Func_IsNull;
  int Func_Vartype(XAXA); script[FUNC_VARTYPE] = &Func_Vartype;
  int Func_getElementById(XAXA); script[FUNC_GETELEMENTBYID] = &Func_getElementById;
  int Func_LogMessage(XAXA); script[FUNC_LOGMESSAGE] = &Func_LogMessage;
  int Func_Getcustomproperty(XAXA); script[FUNC_GETCUSTOMPROPERTY] = &Func_Getcustomproperty;
  int Func_CloseAllWindows(XAXA); script[FUNC_CloseAllWindows] = &Func_CloseAllWindows;
  int Func_DataValue(XAXA); script[FUNC_DATAVALUE] = &Func_DataValue;
  int Func_page_GetStyleClassProperty(XAXA); script[FUNC_page_GetStyleClassProperty] = &Func_page_GetStyleClassProperty;
  int Func_FindPopup(XAXA);  script[FUNC_FindPopup] = &Func_FindPopup;
  int Func_page_stylesheetSrc(XAXA); script[FUNC_page_stylesheetSrc] = &Func_page_stylesheetSrc;
  int Func_DisplayDataRepositoryGetValue(XAXA); script[FUNC_DisplayDataRepositoryGetValue] = &Func_DisplayDataRepositoryGetValue;
  int Func_DisplayDataRepositoryPutValue(XAXA); script[FUNC_DisplayDataRepositoryPutValue] = &Func_DisplayDataRepositoryPutValue;
  int Func_PAGEGETATTRIBUTE(XAXA); script[FUNC_PAGE_GETATTRIBUTE] = &Func_PAGEGETATTRIBUTE;
  int Func_PAGESETATTRIBUTE(XAXA); script[FUNC_PAGE_SETATTRIBUTE] = &Func_PAGESETATTRIBUTE;
  int Func_AlarmStateIconfactory(XAXA); script[FUNC_AlarmStateIconfactory] = &Func_AlarmStateIconfactory;
  int Func_GetAttribute(XAXA); script[FUNC_GETATTRIBUTE] = &Func_GetAttribute;
  int Func_SetAttribute(XAXA); script[FUNC_SETATTRIBUTE] = &Func_SetAttribute;
  int Func_SecurityLevel(XAXA); script[FUNC_SecurityLevel] = &Func_SecurityLevel;
  int Func_SetDataValue(XAXA); script[FUNC_SET_DATAVALUE] = &Func_SetDataValue;

  int Func_TRIM   (XAXA);script[FUNC_TRIM     ] = &Func_TRIM  ;
  int Func_LTRIM  (XAXA);script[FUNC_LTRIM    ] = &Func_LTRIM ;
  int Func_RTRIM  (XAXA);script[FUNC_RTRIM    ] = &Func_RTRIM ;
  int Func_INSTR  (XAXA);script[FUNC_INSTR    ] = &Func_INSTR ;
  int Func_MID    (XAXA);script[FUNC_MID      ] = &Func_MID   ;
  int Func_LEFT   (XAXA);script[FUNC_LEFT     ] = &Func_LEFT  ;
  int Func_RIGHT  (XAXA);script[FUNC_RIGHT    ] = &Func_RIGHT ;
  //
  int Func_OR     (XAXA);script[OPER_OR       ] = &Func_OR    ;
  int Func_MOD    (XAXA);script[OPER_MOD      ] = &Func_MOD   ;
  int Func_SUB    (XAXA);script[OPER_SUB      ] = &Func_SUB   ;
  int Func_ADD    (XAXA);script[OPER_ADD      ] = &Func_ADD   ;
  int Func_DIV    (XAXA);script[OPER_DIV      ] = &Func_DIV   ;
  int Func_DIVINT (XAXA);script[OPER_DIV_INT  ] = &Func_DIVINT;
  int Func_EQ     (XAXA);script[OPER_EQ       ] = &Func_EQ    ;
  int Func_BEQ    (XAXA);script[OPER_BEQ      ] = &Func_BEQ   ;
  int Func_MEQ    (XAXA);script[OPER_MEQ      ] = &Func_MEQ   ;
  int Func_NEQ    (XAXA);script[OPER_NEQ      ] = &Func_NEQ   ;
  int Func_AND    (XAXA);script[OPER_AND      ] = &Func_AND   ;
  int Func_MUL    (XAXA);script[OPER_MUL      ] = &Func_MUL   ;
  int Func_MEN    (XAXA);script[OPER_MEN      ] = &Func_MEN   ;
  int Func_BOL    (XAXA);script[OPER_BOL      ] = &Func_BOL   ;
  int Func_EQ_EQ  (XAXA);script[OPER_EQ_EQ    ] = &Func_EQ_EQ ;
  int Func_NOT    (XAXA);script[OPER_NOT] = &Func_NOT;
  int Func_IS     (XAXA);script[OPER_IS] = &Func_IS;
  }
//
int CVBScript::Run()
  {
  //if ( lstrcmpi( mId, "line015" ) == 0 )
  //  {
  //  int yy = 90;
  //  }
  _static int nGlobalA = 1;
  UINT nA = mGVAR.Find( "LOCAL_A" );
  EValueTypeScript eA;
  int* vA = (int*)mGVAR.Var( *html, nA, eA );
  if ( vA ) //&& eA == enumValueInt )
    *vA = nGlobalA;
  //
  int nParam;
  SCommand* comm = (SCommand*)mComm.m_szBuffer;
  GVAR* list = (GVAR*)mGVAR.m_szBuffer;
  int nCount = mComm.Count();
  int N = 0;
  int n = 0;
  while ( n < nCount )
    {
    if ( AS( comm[n].nOper < _MAX_FUNCTION ) ) return 0;
    if ( AS( script[comm[n].nOper] != NULL ) ) return 0;
    //
#ifdef _DEBUG
    const char* r = mGVAR.Name( comm[n].nRes  );
    const char* a = mGVAR.Name( comm[n].nArg1 );
    const char* b = mGVAR.Name( comm[n].nArg2 );
    const char* c = mGVAR.Name( comm[n].nArg3 );
#endif 
    //
    nParam = 0;
    N = (*script[comm[n].nOper])( *html, mGVAR, comm[n], nParam, this );
    //
    if ( N < 0 )
      {
//      ASSERT(0);
      // Для повторного вызова
//      N = (*script[comm[n].nOper])( *html, mGVAR, comm[n], nParam );
      return -1;
      }
    //
    if ( nParam == 0 )
    if ( comm[n].nRes != -1 && list[comm[n].nRes].nDef == VALUE_VALUE )
      {
      SValueValue* s = mGVAR.Value(comm[n].nRes);
      if ( AS( s->mTag != NULL ) ) return 0;
      s->mTag->Values(*html);
      }
    //
    if ( N == 0 ) n++;  // Простая команда
    else          n = N;// Переход
    }
  //
  if ( vA ) //&& eA == enumValueInt )
    nGlobalA = *vA;
  return 0;
  }
//
GVAR& SUniGVAR::Obj( UINT n )
  {
  _static GVAR v;
  if ( n >= Count() )
    return v;
  GVAR* lst = (GVAR*)m_szBuffer;
  return lst[n];
  }
//
void* SUniGVAR::Var( CHTML& html, UINT n, EValueTypeScript& e,  bool bRightValue )
  {
  e = _enumValueUnk;
  if ( n == -1 )
    return NULL;
  GVAR& v = Obj( n );
  if(v.nSize == 0)
    return NULL;
  switch ( v.nType&0xf )
    {
    case TYPE_FLOAT : e = _enumValueDbl;break;
    case TYPE_INT   : e = _enumValueInt;break;
    case TYPE_BOOL  : e = _enumValueBol;break;
    case TYPE_STRING: e = _enumValueStr;break;
    case TYPE_CHAR  : e = _enumValueChr;break;
    case TYPE_UNKNOWN: e = _enumValueVariant;break;
    case TYPE_PARAM :
      {
      static const int b01[] = {0,1};
      cross::SParamValueH* s = Param(n);
      if ( s->def == NULL )
      {
        if( v.nDef == PARAM_DATAEXIST)
        {
           e = _enumValueBol;
           return (void*)&b01[0];
        }
        else return NULL;
      }
#ifdef _DEBUG
      LPCTSTR name = html.pipe.GetMnemoName(s->nNumber);
#endif 
      if ( v.nDef == PARAM_QUALITYBAD )
        {
        ASSERT( s->def->eVal == _enumValueDbl );
        e = _enumValueBol;
        //double d = html.pipe.ValueF_H( *s );
        //bool* b = (bool*)Chars(n);
        //if ( b ) *b = IsNaN(d);
        //return b;
        return (void*)&b01[0];         
        }
      else
      if ( v.nDef == PARAM_QUALITYGOOD )
        {
        void* val = html.pipe.ParamValue_H( *s );
        e = _enumValueBol;
        return val? (void*)&b01[1] : (void*)&b01[0];         
        }
      else
      if ( v.nDef == PARAM_ISNAN )
        {
        if( s->def->eVal == _enumValueDbl )
          {
          e = _enumValueBol;
          double d = html.pipe.ValueF_H( *s );
          if(IsNaN(d))
            return (void*)&b01[1];
          return (void*)&b01[0];
          }
        }
      else
      if ( v.nDef == PARAM_DATACHANGED )
        {
        e = _enumValueBol;
        void* Void = html.pipe.ParamValue_H( *s );
        if(Void && v.nSize == sizeof(DATA_CHANGED) && s->def)
        {
            DATA_CHANGED* pdc = (DATA_CHANGED*)s;
            //DWORD time = ::GetTickCount();
            //if((time- pdc->dwChangeTime) > 900)
            if(html.m_nTimerCount > 0)
            {
              //pdc->dwChangeTime = time; 
              EValueTypeScript ce = (EValueTypeScript)s->def->eVal;
              switch(ce)
              {
               case _enumValueDbl: 
                   {                   
                   double rr = *(double*)Void;
                   pdc->bChanged = rr != pdc->dPrevVal || pdc->eVal != ce;
                   pdc->dPrevVal = rr;
                   pdc->eVal = ce;
                   return (void*)&pdc->bChanged;
                   }
               case _enumValueInt: 
                   {
                   int rr = *(int*)Void;
                   pdc->bChanged = rr != pdc->nPrevVal || pdc->eVal != ce;
                   pdc->nPrevVal = rr; 
                   pdc->eVal = ce;
                   return (void*)&pdc->bChanged;
                   }
               case _enumValueChr: 
                   {
                   int rr = *(BYTE*)Void;
                   pdc->bChanged = rr != pdc->nPrevVal || pdc->eVal != ce;
                   pdc->nPrevVal = rr;
                   pdc->eVal = ce;
                   return (void*)&pdc->bChanged;
                   }
               case _enumValueBol: 
                   {
                   bool rr = *(bool*)Void;
                   pdc->bChanged = rr != pdc->bPrevVal || pdc->eVal != ce;
                   pdc->bPrevVal = rr;
                   pdc->eVal = ce;
                   return (void*)&pdc->bChanged;
                   }
               default: AAA();
                   break;
              }
            }
            else
            {
                //pdc->bChanged = false;
                pdc->bChanged = true;
                return (void*)&pdc->bChanged;
            }
        }

        static bool bChacged = true;
        return &bChacged;
        }
      else
      if ( v.nDef == PARAM_UnacknowledgedAlarm )
        {
        e = _enumValueBol;
        static bool notAck = false;
        CAlarmBase* al = html.pipe.Alarm( s->nNumber );
        if(al)
          return &al->notACK;
        else &notAck;
        }
      else
      if ( v.nDef == PARAM_ALARMSEVERITY)
        {
        e = _enumValueInt;
        static int AlarmSeverity;
        AlarmSeverity  = 0;
        CAlarmBase* al = html.pipe.Alarm( s->nNumber );
        if(al && al->btEHLN >= 2 && al->btEHLN <= 4)         
           AlarmSeverity = al->btEHLN - 1;
        return &AlarmSeverity;
        }
      else
      if ( v.nDef == PARAM_DATAALARM)
        {//что то тут другое
        e = _enumValueInt;
        static int AlarmSeverity;
        AlarmSeverity  = 0;
        CAlarmBase* al = html.pipe.Alarm( s->nNumber );
        if(al && al->btEHLN >= 2 && al->btEHLN <= 4)         
          AlarmSeverity = 201 + 200*(al->btEHLN - 1);                                                                   
        return &AlarmSeverity;
        }
      else
      if ( v.nDef == PARAM_DATAQUALITY)
        {//что то тут другое
        e = _enumValueInt;
        static int OALITY_GOOD = 198;
        return &OALITY_GOOD;
        }
      else
        if( v.nDef == PARAM_DATAEXIST)
        {
           e = _enumValueBol;
           return (void*)&b01[1];
        }
      else
        {
        e = (EValueTypeScript)s->def->eVal;
        void* Void = html.pipe.ParamValue_H( *s );
        if ( Void == NULL || s->def->dwShiftSh == 0 )
          {
          _static char q[MAX_LOCAL];
          memset( q, 0, MAX_LOCAL );
          Void = q;
          }
        return Void;
        }
      }
    break;
    case TYPE_VALUE:
      {
      SValueValue* s = Value(n);
      if ( s->Void == NULL )
        return NULL;
      e = (EValueTypeScript)s->eVal;
#ifdef _DEBUG
      if(s->mTag)
      {
      CStyle& style = html.Style(s->mTag->m_nStyle);
      const char* id = style.NameID(html);
      AAA();
      }
#endif
      if(e==_enumValueDbl && s->mTag)
        {
        CStyle& style = html.Style(s->mTag->m_nStyle);
        const type_info& ti = typeid(*s->mTag);
        if (ti == typeid(CTagTextArea) ||
            ti == typeid(CTagSPAN) || style.IsShape() ||
            ( Version >= AM2_NAK && bRightValue && ti == typeid(CTagDIV) && TYPE_PNT == TYPE_TEXT)
           )
         e = _enumValueStr;
        }
      return s->Void;
      }    
    case TYPE_TAG:
        e = _enumValueTag;
    break; 

    case TYPE_CUSTOMPROPERTY:
        e = _enumValueCastomProperty;
    break;

    case TYPE_OBJECT:
        e = _enumValueObjects;
    break;

    default:
    if ( v.nType != TYPE_LOCAL )
      {
      ASSUNO
      return NULL;
      }
    else
      e = _enumValueFlt;//Временно
    };
  return Chars(n);
  }
//
//////////////////////////////////////////////////////////////////////////
bool SetTypeAndVal(UINT nRes, EValueType eA, void *A, void *R, SUniGVAR& gVar)
{
     GVAR& res = gVar.Obj(nRes);
     int nType = -1;
     switch(eA)
       {
     case enumValueDbl: if(res.nSize >= sizeof(double)) nType = TYPE_FLOAT;
          break;
     case enumValueInt: if(res.nSize >= sizeof(int)) nType = TYPE_INT;
         break;
     case enumValueBol: nType = TYPE_BOOL;
         break;
     case enumValueStr: if(res.nSize >= 256) nType = TYPE_STRING;
         break;
     case enumValueChr: nType = TYPE_CHAR;
         break;
       }

     if(nType != -1)
       {
       res.nType = nType | res.nType & 0xFFFFFFF0;  

       switch ( eA )
        {
          case enumValueDbl:
            *((double*)R) = *(double*)A;
            break;
          case enumValueChr:
            *((char*)R) = *(char*)A;
            break;
          case enumValueInt:
            *((int*)R) = *(int*)A;
            break;
          case enumValueBol:
            *((bool*)R) = *(bool*)A;
            break;
          case enumValueStr:
            lstrcpy((char*)R, (char*)A);
            break;
        }

       return true;
       }
     return false;
}
//
const char* GetCustomProperty(CHTML& html, SUniGVAR& gVar, std::map<std::string, CTag*> &vTags, UINT nRes, UINT nA)
{
     GVAR& r = gVar.Obj( nRes );
     EValueTypeScript eR, eA;
     void* R = gVar.Var( html, nRes , eR );
     void* A = gVar.Var( html, nA , eA );
     SCTag *pProp = (SCTag *)A;
     if(pProp)
       {
        char *pp = strchr(pProp->m_name, '.');
        if(pp)
        {
            char objname[128]="";
            strncpy_s(objname, pProp->m_name, pp - pProp->m_name);
            _strupr_s(objname);
            std::map<std::string, CTag*>::iterator it = vTags.find(objname);
            if(it!= vTags.end())
              {
              char *variant = strchr(pp, '"');
              if(variant)
               {
                ++variant;
                char param[256];
                char *pp1 = strchr(variant, '"'); //конец варианта
                if(pp1)
                  pp1 = strchr(pp1+1, '"');  //начало параметра
                if(pp1)
                {
                char *pp2 = strchr(pp1+1, '"');
                strncpy_s(param, pp1+1, pp2-pp1-1);
                const char *prop = Alias(html, it->second, param);
                if(prop)
                {
                if(_strnicmp(variant, "Text", sizeof("Text")-1)==0)
                 {                    
                  return prop;
                 }
                else
                if(_strnicmp(variant, "Style", sizeof("Style")-1)==0)
                 {
                     AAA();
                 }
                else
                if(_strnicmp(variant, "Num", sizeof("Num")-1)==0)
                 {
                     AAA();
                 }
                }
                }
               }
              }
            }
        }
 return NULL;
}
//
bool ovalfromsval(O_VAL* poval, SValueValue* psval)
{
    poval->eType = psval->eVal;
    if(psval->Void)
    {
      switch(psval->eVal)
      {
      case enumValueDbl:
          poval->odouble = *(double*)psval->Void;
          break;
      case enumValueInt:
          poval->oint = *(int*)psval->Void;
          break;
      case _enumValueChr:
          poval->oint = *(BYTE*)psval->Void;
          break;
      case enumValueBol:
          poval->obool = *(bool*)psval->Void;
          break;
      case enumValueStr:
          lstrcpy(poval->ostr, (char*)psval->Void);
          break;
      default: return false;
      }
      return true;
    }
    return false;
}

bool GetDataFromObject(O_VAL* oval, void* A, EValueTypeScript& eA, CHTML& html, SUniGVAR& gVar, CVBScript *pScript)
{
    SCTag *pPropA = (SCTag *)A;
    char *pparam = NULL;
    bool bb = false;
    if(pPropA->objectType==otWindowEvent)
    {
      if(_strnicmp(pPropA->m_name, "event.", _countof("event.")-1) == 0)
      {
        char *pp = pPropA->m_name + _countof("event.")-1;
        if(_strnicmp(pp, "srcelement", _countof("srcelement")-1) == 0)
        {
          pp += _countof("srcelement")-1;
          if(html.m_psrcelement)
          {
            CStyle& style = html.Style(html.m_psrcelement->m_nStyle);
            if(stricmp(pp, ".id") == 0)
            {
                const char* id = style.NameID(html);
                lstrcpy(oval->ostr, id); 
                oval->eType = enumValueStr;
                return true;
            }
            if(strnicmp(pp, ".title", _countof(".title")-1) == 0)
            {                
                lstrcpy(oval->ostr, style.m_szTitle); 
                oval->eType = enumValueStr;
                return true;
            }

          }
        }
        else
        if(stricmp(pp, "y") == 0)
        {
           oval->oint = html.m_eventpoint.y; 
           oval->eType = enumValueInt;
           return true;
        }
        AAA();
      }
    }
    else
    {
       char objname[128]="";
       char *pp = strchr(pPropA->m_name, '.');
       if(pp)
       {
         strncpy_s(objname, pPropA->m_name, pp - pPropA->m_name);
         _strupr_s(objname);
         std::map<std::string, CTag*>::iterator it = pScript->m_vTags.find(objname);
         if(it!= pScript->m_vTags.end())
         {
           char buf[256];
           SValueValue sval = GetTagParam(html, pScript, gVar, it->second, pp+1, buf, &pPropA->nEndPipeCount);
           return ovalfromsval(oval, &sval);           
         }
       }
    }

    return bb;
}
//
int FuncIF        (XAXA)
  {
  EValueTypeScript eR;
  void* R = gVar.Var( html, comm.nRes , eR );
  if ( R == NULL ) return -1;
  bool* Z = (bool*)R;
  if ( eR != enumValueBol )
    {
    ASSERT(0);
    return -1;
    }
  if ( !*Z )
    return comm.nArg1;
  return 0;
  }
//
int FuncTHEN      (XAXA)
  {
  return comm.nArg1;
  }
//
int FuncEmpty(XAXA)
  {
  return 0;
  }
//
int Func_SUB      (XAXA)
  {
  EValueTypeScript eR,eA,eB;
  void* R = gVar.Var( html, comm.nRes , eR );
  void* A = gVar.Var( html, comm.nArg1, eA );
  void* B = gVar.Var( html, comm.nArg2, eB );
  if ( R == NULL || A == NULL  || B == NULL ) return -1;
  //
  switch ( eR )
    {
    case enumValueDbl: {double* Z = (double*)R; *Z = Dbl( eA, A ) - Dbl( eB, B );} break;
    case enumValueInt: {int   * Z = (int   *)R; *Z = Int( eA, A ) - Int( eB, B );} break;
    case enumValueBol:// {bool  * Z = (bool  *)R; *Z = Bol( eA, A ) - Bol( eB, B );} break;
       {
       bool* Z = (bool*)R;
       int z = int(Bol( eA, A )) - int(Bol( eB, B ));
       *Z = z != 0;
       }
       break;
    case enumValueStr:
      {
      char* r = (char*)R;
      char* a = (char*)A;     
      if(eA == _enumValueStr && eB == _enumValueInt)
        {
        char b[128];
        int na = atoi(a);
        sprintf_s(b, "%d", na - *(int*)B );
        lstrcpy( r, b );
        }
      else
      if(eA == _enumValueStr && eB == _enumValueDbl)
        {
        char b[128];
        double na = atof(a);
        sprintf_s(b, "%f", na - *(double*)B );
        lstrcpy( r, b );
        }
      else
        {
        ASSERT(0);
        }
      }
    case _enumValueObjects:
        if( eA == _enumValueStr && eB == _enumValueStr)
        {
            //char b[128];
            double rr = atof((char*)A) - atof((char*)B);
            GVAR& r = gVar.Obj( comm.nRes );
            r.nType = TYPE_FLOAT | (r.nType&0xFFF0);
            *(double*)R = rr;
        }
    default:
      ASSERT(0);
    break;
    }
  return 0;
  }
//
int Func_MUL      (XAXA)
  {
  EValueTypeScript eR,eA,eB;
  void* R = gVar.Var( html, comm.nRes , eR );
  void* A = gVar.Var( html, comm.nArg1, eA );
  void* B = gVar.Var( html, comm.nArg2, eB );
  if ( R == NULL || A == NULL  || B == NULL ) return -1;
  //
  switch ( eR )
    {
    case enumValueDbl: {double* Z = (double*)R; *Z = Dbl( eA, A ) * Dbl( eB, B );} break;
    case enumValueInt: {int   * Z = (int   *)R; *Z = Int( eA, A ) * Int( eB, B );} break;
    case enumValueBol://{bool  * Z = (bool  *)R; *Z = Bol( eA, A ) * Bol( eB, B );} break;
       {
       bool* Z = (bool*)R;
       int z = int(Bol( eA, A )) * int(Bol( eB, B ));
       *Z = z != 0;
       }
    case enumValueStr:
        if( eA ==  enumValueDbl || eB ==  enumValueDbl)
        {
          char tr[128];
          double dd = Dbl( eA, A ) * Dbl( eB, B );
          sprintf_s(tr, "%0.4f", dd);
          lstrcpy((char*)R, tr); 
          break;
        }
    case _enumValueObjects:
        if( (eA == _enumValueDbl || eA == _enumValueInt) && (eB == _enumValueDbl || eB == _enumValueInt) )
        {
            *(double*)R = Dbl( eA, A ) * Dbl( eB, B );
            GVAR& r = gVar.Obj( comm.nRes );
            r.nType = TYPE_FLOAT | (r.nType&0xFFF0);
        }
    default:
      ASSERT(0);
    break;
    }
  return 0;
  }
//
int Func_ADD      (XAXA)
  {
  EValueTypeScript eR,eA,eB;
  void* R = gVar.Var( html, comm.nRes , eR );
  void* A = gVar.Var( html, comm.nArg1, eA );
  void* B = gVar.Var( html, comm.nArg2, eB );
  if ( R == NULL || A == NULL  || B == NULL ) return -1;
  //
  switch ( eR )
    {
    case enumValueDbl: {double* Z = (double*)R; *Z = Dbl( eA, A ) + Dbl( eB, B );} break;
    case enumValueInt: {int   * Z = (int   *)R; *Z = Int( eA, A ) + Int( eB, B );} break;
    case enumValueBol://{bool  * Z = (bool  *)R; *Z = Bol( eA, A ) + Bol( eB, B );} break;
       {
       bool* Z = (bool*)R;
       int z = int(Bol( eA, A )) + int(Bol( eB, B ));
       *Z = z != 0;
       }
    case enumValueStr:
      {
      //ASSERT( eA == eR && eB == eR );
      char* r = (char*)R;
      char* a = (char*)A;     
      if(eA == _enumValueStr && eB == _enumValueInt)
        {
        char b[128];
        int na = atoi(a);
        sprintf_s(b, "%d", na + *(int*)B );
        lstrcpy( r, b );
        }
      else
      if(eA == _enumValueStr && eB == _enumValueDbl)
        {
        char b[256];
        if(::IsFlt((char*)A))
          {
        double na = atof(a);
        sprintf_s(b, "%f", na + *(double*)B );
          }
        else 
          {
            strcpy_s(b, a);
            GVAR& v = gVar.Obj( comm.nArg2 );
            if( (v.nType&0xf) == TYPE_VALUE)
            {
                strcat_s(b, (char*)B);
            }
          }
        lstrcpy( r, b );
        }
      else
        {
      char* b = (char*)B;
      lstrcpy( r, a );
      lstrcat( r, b );
        }
      }
    break;
    case _enumValueObjects:
      {
          _types nType = TYPE_UNKNOWN;
          if(eA == _enumValueInt && eB == _enumValueInt)
            {int   * Z = (int   *)R; *Z = Int( eA, A ) + Int( eB, B );
             nType = TYPE_INT;
            }
          else 
          if(eA == _enumValueDbl || eB == _enumValueDbl)
             {double* Z = (double*)R; *Z = Dbl( eA, A ) + Dbl( eB, B );
               nType = TYPE_FLOAT;
             }
          if(nType != TYPE_UNKNOWN)
          {
             GVAR& res = gVar.Obj(comm.nRes);  
             res.nType = nType | res.nType & 0xFFFFFFF0; 
          }
      }
      break;
    default:
      ASSERT(0);
    break;
    }
  return 0;
  }
// Челочисленное деление
int Func_DIVINT(XAXA)
  {
  EValueTypeScript eR,eA,eB;
  void* R = gVar.Var( html, comm.nRes , eR );
  void* A = gVar.Var( html, comm.nArg1, eA );
  void* B = gVar.Var( html, comm.nArg2, eB );
  if ( R == NULL || A == NULL  || B == NULL ) return -1;
  ASSERT( eR == enumValueInt );
  int* r = (int*)R;
  int a = Int( eA, A ); 
  int b = Int( eB, B ); 
  if ( b <= 0 )
    {
    ASSERT(0);
    *r = 0;
    }
  else
    {
    *r = a / b;
    }
  return 0;
  }
//
int Func_DIV(XAXA)
  {
  EValueTypeScript eR,eA,eB;
  void* R = gVar.Var( html, comm.nRes , eR );
  void* A = gVar.Var( html, comm.nArg1, eA );
  void* B = gVar.Var( html, comm.nArg2, eB );
  if ( R == NULL || A == NULL  || B == NULL ) return -1;
  double b = Dbl( eB, B );
  if ( b == 0 )
    {
//  ASSERT(0);
    return 0;
    }
  //
  switch ( eR )
    {
    case enumValueDbl: {double* Z = (double*)R; *Z = Dbl( eA, A ) / b;} break;

    case _enumValueObjects:
    {
    *(double*)R = Dbl( eA, A ) / b;
    GVAR& r = gVar.Obj( comm.nRes );
    r.nType = TYPE_FLOAT | (r.nType&0xFFF0);
    break;
    }
    default:
      ASSERT(0);
    break;
    }
  return 0;
  }
//
int Func_MOD(XAXA)
  {
  EValueTypeScript eR,eA,eB;
  void* R = gVar.Var( html, comm.nRes , eR );
  void* A = gVar.Var( html, comm.nArg1, eA );
  void* B = gVar.Var( html, comm.nArg2, eB );
  if ( R == NULL || A == NULL  || B == NULL ) return -1;
  ASSERT( eR == enumValueInt );
  int* r = (int*)R;
  int a = Int( eA, A ); 
  int b = int(Dbl( eB, B )+0.5);
  if ( b <= 0 )
    {
    ASSERT(0);
    *r = 0;
    }
  else
    {
    *r = a % b;
    }
  return 0;
  }
//
int Func_BEQ      (XAXA)
  {
  EValueTypeScript eR,eA,eB;
  void* R = gVar.Var( html, comm.nRes , eR );
  void* A = gVar.Var( html, comm.nArg1, eA );
  void* B = gVar.Var( html, comm.nArg2, eB );
  if ( R == NULL || A == NULL || B == NULL ) return -1;
  //
  bool* Z = (bool*)R;
  if ( eR != enumValueBol )
    {
    ASSERT(0);
    return -1;
    }
  //
  switch ( eA )
    {
    case enumValueDbl: {*Z = Dbl( eA, A ) >= Dbl( eB, B );} break;
    case enumValueInt: {*Z = Int( eA, A ) >= Int( eB, B );} break;
    case enumValueBol: {*Z = Bol( eA, A ) >= Bol( eB, B );} break;
    case enumValueStr: 
      if(eB == _enumValueInt)
       {
        int a = atoi((char*)A);
        *Z = a >= *((int*)B);
       }
      else
      if(eB == _enumValueStr)
      {
          double a = atof((char*)A);
          double b = atof((char*)B);
          *Z = a >= b;
      }
      break;
    default:
      ASSERT(0);
    break;
    }
  return 0;
  }
//
int Func_MEQ      (XAXA)
  {
  EValueTypeScript eR,eA,eB;
  void* R = gVar.Var( html, comm.nRes , eR );
  void* A = gVar.Var( html, comm.nArg1, eA );
  void* B = gVar.Var( html, comm.nArg2, eB );
  if ( R == NULL || A == NULL || B == NULL ) return -1;
  //
  bool* Z = (bool*)R;
  if ( eR != enumValueBol )
    {
    ASSERT(0);
    return -1;
    }
  //
  O_VAL oval;
  if(eA == _enumValueObjects)
  {   
      if(!GetDataFromObject(&oval, A, eA, html, gVar, script))
         return -1; 
      eA = (EValueTypeScript)oval.eType;
      if(oval.eType ==  enumValueInt) 
        A = &oval.oint;
  }
  //
  switch ( eA )
    {
    case enumValueDbl: {*Z = Dbl( eA, A ) <= Dbl( eB, B );} break;
    case enumValueInt: {*Z = Int( eA, A ) <= Int( eB, B );} break;
    case enumValueBol: {*Z = Bol( eA, A ) <= Bol( eB, B );} break;
    case enumValueStr:
        switch(eB)
        {
        case _enumValueInt:
          *Z = Int( eA, A ) <= Int( eB, B );
          break;
        case _enumValueStr:
          *Z = Dbl( eA, A ) <= Dbl( eB, B );
          break;
        }
        break;
    default:
      ASSERT(0);
    break;
    }
  return 0;
  }
//
int Func_NEQ      (XAXA)
  {
  EValueTypeScript eR,eA,eB;
  void* R = gVar.Var( html, comm.nRes , eR );
  void* A = gVar.Var( html, comm.nArg1, eA );
  void* B = gVar.Var( html, comm.nArg2, eB );
  if ( R == NULL || A == NULL || B == NULL ) return -1;
  //
  bool* Z = (bool*)R;
  if ( eR != enumValueBol )
    {
    ASSERT(0);
    return -1;
    }
  //
  O_VAL oval;
  if(eA == _enumValueObjects)
  {   
      if(!GetDataFromObject(&oval, A, eA, html, gVar, script))
         return -1; 
      eA = (EValueTypeScript)oval.eType;
      if(oval.eType ==  enumValueStr) 
        A = (void*)oval.ostr;
      else return -1;
  }
  //
  switch ( eA )
    {
    case enumValueDbl: {*Z = Dbl( eA, A ) != Dbl( eB, B );} break;
    case enumValueInt: {*Z = Int( eA, A ) != Int( eB, B );} break;
    case enumValueBol: {*Z = Bol( eA, A ) != Bol( eB, B );} break;
    case enumValueStr: {*Z = strcmp((char*)A, (char*)B) != 0;} break;
    default:
      ASSERT(0);
    break;
    }
  return 0;
  }
//
int Func_OR       (XAXA)
  {
  EValueTypeScript eR,eA,eB;
  void* R = gVar.Var( html, comm.nRes , eR );
  void* A = gVar.Var( html, comm.nArg1, eA );
  void* B = gVar.Var( html, comm.nArg2, eB );
  if ( R == NULL || A == NULL  || B == NULL ) return -1;
  ASSERT( eA == eB || eB == enumValueChr );
  //
  switch ( eR )
    {
    //case enumValueDbl: {double* Z = (double*)R; *Z = Dbl( eA, A ) | Dbl( eB, B );} break;
    case enumValueInt: {int   * Z = (int   *)R; *Z = Int( eA, A ) | Int( eB, B );} break;
    case enumValueBol: {bool  * Z = (bool  *)R; *Z = Bol( eA, A ) | Bol( eB, B );} break;
    default:
      ASSERT(0);
    break;
    }
  return 0;
  }
//
int Func_AND      (XAXA)
  {
  EValueTypeScript eR,eA,eB;
  void* R = gVar.Var( html, comm.nRes , eR );
  void* A = gVar.Var( html, comm.nArg1, eA );
  void* B = gVar.Var( html, comm.nArg2, eB );
  if ( R == NULL || A == NULL  || B == NULL ) return -1;
  //
  O_VAL oval;
  if(eA == _enumValueObjects)
  {   
      if(!GetDataFromObject(&oval, A, eA, html, gVar, script))
         return -1; 
      eA = (EValueTypeScript)oval.eType;
      if(oval.eType ==  enumValueStr) 
        A = (void*)oval.ostr;
      else
      if(oval.eType ==  enumValueInt) 
        A = (void*)&oval.oint;
      else
      if(oval.eType ==  enumValueDbl) 
        A = (void*)&oval.odouble;
      else
      if(oval.eType ==  enumValueBol) 
        A = (void*)&oval.obool;
      else return -1;
  }
  //
  if(eB == _enumValueCastomProperty)
    {
       const char* prop = GetCustomProperty(html, gVar, script->m_vTags, comm.nRes, comm.nArg2);
       GVAR& r = gVar.Obj( comm.nRes );
       if(prop)
        {
          UINT nType = r.nType&0x0F;
          if( (nType==0) || (nType==TYPE_CUSTOMPROPERTY))
            nType = TYPE_STRING;
          if(nType == TYPE_STRING)
          {
            r.nType = nType | (r.nType&0xFFF0);
          }
        }
    }
  //
  ASSERT( eA == eB );
  //
  switch ( eR )
    {
    case enumValueStr:
      {
      if(eA != _enumValueObjects && eB != _enumValueObjects )
      {
      ASSERT( eA == eR && eB == eR );
      char* r = (char*)R;
      char* a = (char*)A;
      if(eB == _enumValueInt) 
       {
       char b[128];
       sprintf_s(b, "%d", *(int*)B);
       lstrcpy( r, a );
       lstrcat( r, b );
       }
      else
      if(eB == _enumValueChr) 
       {
       char b[128];
       sprintf_s(b, "%d", (int)*(char*)B);
       lstrcpy( r, a );
       lstrcat( r, b );
       }
      else
      {
      switch(eA)
      {
      case _enumValueStr:
        lstrcpy( r, (char*)a );
        lstrcat( r, (char*)B );
        break;
      case _enumValueDbl:
        {
           char aa[256];
           sprintf_s(aa, "%0.1f", *(double*)A);
           lstrcpy( r, aa );
           lstrcat( r, (char*)B );
        }
        break;
      case _enumValueChr: 
        {
           char aa[128];
           sprintf_s(aa, "%d", (int)*(char*)A);
           lstrcpy( r, a );
           lstrcat( r, (char*)B  );
        }
       break;
      case _enumValueInt: 
        {
           char aa[128];
           sprintf_s(aa, "%d", *(int*)A);
           lstrcpy( r, a );
           lstrcat( r, (char*)B  );
        }
       break;
      }
      }
      }
      else
        {
         char* r = (char*)R;
         char* a = "";
         char* b = "";
         if(eA == _enumValueObjects)
           {
           SCTag *pPropA = (SCTag *)A;
           char *pparam = NULL;
           bool bb = false;
           CTag *pTagA = HtmlGetObject(html, script->m_vTags, pPropA, pparam);
           if(pTagA && pparam)
           if(_stricmp(pparam, "id")==0)
             {
              CStyle& style = pTagA->Style(html);
              LPCSTR id = style.NameID(html);
              a= (char*)id;
             }
           }
         else a = (char*)A;
         if(eB == _enumValueObjects)
           {
           SCTag *pPropB = (SCTag *)B;
           char *pparam = NULL;
           bool bb = false;
           CTag *pTagB = HtmlGetObject(html, script->m_vTags, pPropB, pparam);
           if(pTagB && pparam)
           {
           if(_stricmp(pparam, "id")==0)
             {
              CStyle& style = pTagB->Style(html);
              LPCSTR id = style.NameID(html);
              b= (char*)id;
             }
           else
           if(_stricmp(pparam, "value")==0)
             {
              b= (char*)(pTagB->m_szScriptValue);
             }
           }
           }
         else b = (char*)B;
         lstrcpy( r, a );
         lstrcat( r, b );
         AAA();
        }
      }
    break;
    //case enumValueDbl: {double* Z = (double*)R; *Z = Dbl( eA, A ) & Dbl( eB, B );} break;
    case enumValueInt: {int   * Z = (int   *)R; *Z = Int( eA, A ) & Int( eB, B );} break;
    case enumValueBol: {bool  * Z = (bool  *)R; *Z = Bol( eA, A ) & Bol( eB, B );} break;
    case _enumValueObjects:
        {   
            SCTag *pProp = (SCTag *)R;
            if(pProp->m_pTag == NULL && pProp->objectType==otTAG)
            {
              GVAR& r = gVar.Obj( comm.nRes );
              if(eA == _enumValueInt && eB == _enumValueInt)
              {      
              r.nType = TYPE_INT | (r.nType&0xFFF0);
              {int   * Z = (int*)R; *Z = Int( eA, A ) & Int( eB, B );}
              }
            }
        }
    break;
    default:
      ASSERT(0);
    break;
    }
  return 0;
  }
//
int Func_MEN      (XAXA) 
  {
  EValueTypeScript eR,eA,eB;
  void* R = gVar.Var( html, comm.nRes , eR );
  void* A = gVar.Var( html, comm.nArg1, eA );
  void* B = gVar.Var( html, comm.nArg2, eB );
  if ( R == NULL || A == NULL || B == NULL ) return -1;
  //
  bool* Z = (bool*)R;
  if ( eR != enumValueBol )
    {
    ASSERT(0);
    return -1;
    }
  //
  switch ( eA )
    {
    case enumValueDbl: {*Z = Dbl( eA, A ) < Dbl( eB, B );} break;
    case enumValueInt: {*Z = Int( eA, A ) < Int( eB, B );} break;
    case enumValueBol: {*Z = Bol( eA, A ) < Bol( eB, B );} break;
    case enumValueStr:
        switch(eB)
        {
        case _enumValueInt:
          *Z = Int( eA, A ) < Int( eB, B );
          break;
        case _enumValueDbl:
          *Z = Dbl( eA, A ) < Dbl( eB, B );
          break;
        case _enumValueStr:
          *Z = Dbl( eA, A ) < Dbl( eB, B );
          break;
        }
        break;
    default:
      ASSERT(0);
    break;
    }
  return 0;
  }
//
int Func_BOL      (XAXA)
  {
  EValueTypeScript eR,eA,eB;
  void* R = gVar.Var( html, comm.nRes , eR );
  void* A = gVar.Var( html, comm.nArg1, eA );
  void* B = gVar.Var( html, comm.nArg2, eB );
  if ( R == NULL || A == NULL || B == NULL ) return -1;
  //
  bool* Z = (bool*)R;
  if ( eR != enumValueBol )
    {
    ASSERT(0);
    return -1;
    }
  //
  switch ( eA )
    {
    case enumValueDbl: {*Z = Dbl( eA, A ) > Dbl( eB, B );} break;
    case enumValueInt: {*Z = Int( eA, A ) > Int( eB, B );} break;
    case enumValueBol: {*Z = Bol( eA, A ) > Bol( eB, B );} break;
    case enumValueStr:
        switch(eB)
        {
        case _enumValueInt:
          *Z = Int( eA, A ) > Int( eB, B );
          break;
        case _enumValueDbl:
          *Z = Dbl( eA, A ) > Dbl( eB, B );
          break;
        case _enumValueStr:
          *Z = Dbl( eA, A ) > Dbl( eB, B );
          break;
        }
        break;
    default:
      ASSERT(0);
    break;
    }
  return 0;
  }
//
CTag *HtmlGetObject(CHTML& html, std::map<std::string, CTag*> &vTags, SCTag *pProp, char* &ppar)
{
    if(pProp)
    {
    char *pp = strchr(pProp->m_name, '.');
    if(pp)
      {
        char objname[128]="";
        strncpy_s(objname, pProp->m_name, pp - pProp->m_name);
        _strupr_s(objname);
        std::map<std::string, CTag*>::iterator it = vTags.find(objname);
        if(it!= vTags.end())
          {
          char *pparam = strchr(pp, '"');
          if(pparam)
           {
            pparam++;
            char param[256];
            char *pp1 = strchr(pparam, '"'); //конец параметра
            if(pp1)
            {
              strncpy_s(param, pparam, pp1-pparam);
              char fullname[256];
              CStyle& style = it->second->Style(html);
              LPCSTR id = style.NameID(html);
              bool bShape = style.IsShape();
              sprintf_s(fullname, "%s_%s", id, param);
              CTag *pTag = it->second->FindTagById(html, fullname);
              if(pTag)
               {
                  char *pp2 = strchr(pp1+1, '.');
                  if(pp2)
                  {
                    pp2++;
                    ppar = pp2;
                  }
               }
              return pTag;
            }
          }
          else
          {
             ppar = ++pp; 
             return it->second;
          }
        }
      }
    }
    return NULL;
}
//
bool GetTagParamSetRes(CHTML& html, CVBScript *pScript, SUniGVAR& gVar, UINT nRes, CTag *pTag,  char *pparam, bool bColor, UINT *pnEndPipeCount = NULL)
{
   bool bSet = false;
#ifdef _DEBUG
      CStyle& style = html.Style(pTag->m_nStyle);
      const char* id = style.NameID(html);
#endif
   if(pparam)
     {// допустимый париметр - заполняю
      size_t len = strlen(pparam);
      if(len && pparam[len-1]==':')
        pparam[len-1] = 0;
      for(int ii=0; stdV[ii].name; ii++)
        {
          if(_stricmp(stdV[ii].name, pparam)==0)
            {
               SValueValue val;
               if ( pTag->ValueValue( html, stdV[ii].dw, val ) )
               {
                  PVOID A = val.Void;  
                  EValueTypeScript eA = (EValueTypeScript)val.eVal;
                  if(stdV[ii].dw == VALUE_VALUE)
                     eA = _enumValueStr; 
                  EValueTypeScript eR;
                  void* R = gVar.Var( html, nRes , eR );
                  GVAR& res = gVar.Obj(nRes);
                  UINT nType = res.nType & 0x0F;
                  if(nType == TYPE_OBJECT)
                    {
                    if(Version == BERTH_ONPZ) //тут попробую
                      {
                      const char* r = gVar.Name( nRes  );
                      if(strncmp(r, "LOCAL_I", _countof("LOCAL_I")-1) == 0)
                        {                          
                          if(stdV[ii].dw == VALUE_VALUE)
                          {
                              nType = TYPE_INT;
                              res.nType = nType | res.nType & 0xFFFFFFF0; 
                              *(int*)R = atoi((char*)A);
                              bSet = true;
                              return bSet;
                          }
                        }
                      }

                    _types nType = TYPE_UNKNOWN;
                    switch(eA)
                      {
                    case _enumValueDbl: nType = TYPE_FLOAT ;break;
                    case _enumValueInt: nType = TYPE_INT   ;break;
                    case _enumValueBol: nType = TYPE_BOOL  ;break;
                    case _enumValueStr: nType = TYPE_STRING;break;
                      }
                    if(nType != TYPE_UNKNOWN)
                      {
                      res.nType = nType | res.nType & 0xFFFFFFF0; //тот же тип что полученный от объекта 
                      R = gVar.Var( html, nRes , eR );
                      }
                    }
                  switch ( eR )
                    {
                    case enumValueDbl: {double* Z = (double*)R; *Z = Dbl( eA, A );} 
                      bSet= true; break;
                    case enumValueChr:
                    case enumValueInt: {int   * Z = (int   *)R; *Z = Int( eA, A, bColor );} 
                      bSet= true; break;
                    case enumValueBol: {bool  * Z = (bool  *)R; *Z = Bol( eA, A );} 
                      bSet= true; break;
                    case enumValueStr:
                      {
                      switch(eA)
                       {
                       case enumValueInt:
                       *((double*)R) = (double)*((int*)A); 
                       bSet= true; break;
                       case enumValueDbl:
                       *((double*)R) = *(double*)A;
                       bSet= true; break;
                       case enumValueStr:
                       default:
                         char* r = (char*)R;
                         char* a = (char*)A;
                         lstrcpy( r, a );
                         bSet= true; break;
                       }
                      }
                     }
                  }
               break;
            }
          else
          if(_strnicmp(stdV[ii].name, pparam, stdV[ii].dl)==0)
          {
              if(stdV[ii].dw == PARAM_DATAVALUE || stdV[ii].dw == PARAM_DATAALARM)
              {
                  char *pc = strchr(pparam, '(');
                  if(pc)
                  {
                    char *p1 =  strchr(pc+1, '"');
                    if(p1)
                    {
                       if(stdV[ii].dw == PARAM_DATAALARM)
                       {
                         EValueTypeScript eR;
                         void* R = gVar.Var( html, nRes , eR );
                         GVAR& res = gVar.Obj(nRes);
                         UINT nType = res.nType & 0x0F;
                         if(nType == TYPE_OBJECT)
                          {
                          if(nType != TYPE_UNKNOWN)
                          {
                           res.nType = TYPE_INT | res.nType & 0xFFFFFFF0; //тот же тип что полученный от объекта 
                          }  
                         }
                       }

                       cross::SParamValueH tval;
                       if ( pScript->ParamValue( tval, p1, pTag ) )
                       {
                           if(tval.nNumber >=0 && tval.def)
                           {
#ifdef _DEBUG
                             const char* name = html.pipe.GetMnemoName( tval.nNumber );
#endif
                             if(stdV[ii].dw == PARAM_DATAALARM)
                             {
                               int AlarmSeverity;
                               AlarmSeverity  = 0;
                               CAlarmBase* al = html.pipe.Alarm( tval.nNumber );
                               if(al && al->btEHLN >= 2 && al->btEHLN <= 4)         
                                 AlarmSeverity = 201 + 200*(al->btEHLN - 1);
                               EValueTypeScript eR;
                               int* R = (int*)gVar.Var( html, nRes , eR );
                               *R = AlarmSeverity;
                               bSet = true;
                               break;
                             }   

                             bool bStrH = false;
                             void* val = html.pipe.ParamValue_H( tval );
                             if(val)
                             {
                               EValueTypeScript eR;
                               void* R = gVar.Var( html, nRes , eR );
                               GVAR& res = gVar.Obj(nRes);
                               UINT nType = res.nType & 0x0F;
                               bool bResetRType = (nType == TYPE_OBJECT)? true : false;
                                {
                                _types nType = TYPE_UNKNOWN;
                                switch(tval.def->eVal)
                                  {
                                case _enumValueDbl: nType = TYPE_FLOAT ;break;
                                case _enumValueChr: if(strcmp(tval.def->name, "PVFORMAT")==0)
                                                    {
                                                     nType = TYPE_STRING;
                                                     break;
                                                    }
                                                    else 
                                                    if( (id_PID == tval.def->dwLog || id_REGCALC == tval.def->dwLog || id_AUTOMAN == tval.def->dwLog || id_OVRDSEL) 
                                                        && (stricmp(tval.def->name, "MODE")==0 || stricmp(tval.def->name, "NORMMODE")==0)
                                                      )
                                                    {
                                                     bStrH = true;
                                                     nType = TYPE_STRING;
                                                     break;
                                                    }
                                                    nType = TYPE_INT;
                                                    break;
                                case _enumValueInt: nType = TYPE_INT  ;break;
                                case _enumValueBol: nType = TYPE_INT  ;break;
                                case _enumValueStr: nType = TYPE_STRING;break;
                                   }
                                if(bResetRType && nType != TYPE_UNKNOWN)
                                  {
                                  res.nType = nType | res.nType & 0xFFFFFFF0; //тот же тип что полученный от объекта 
                                  R = gVar.Var( html, nRes , eR );
                                  }
                                }

                               switch(eR)
                               {
                               case _enumValueInt:  
                                     if(tval.def->eVal == _enumValueChr || tval.def->eVal == _enumValueBol)
                                       *(int*)R = *(BYTE*)val;
                                     else *(int*)R = *(int*)val;
                                     bSet= true; 
                                     break;
                               case _enumValueDbl:  *(double*)R = *(double*)val;
                                     bSet= true; 
                                     break;
                               case _enumValueStr:
                                     if(tval.def->eVal == _enumValueChr)
                                     {
                                      if(!bStrH)
                                      {
                                          char tmp[128];
                                          sprintf_s(tmp, "%d", (int)*(BYTE*)val);
                                          lstrcpy((char*)R, tmp);
                                      } 
                                      else
                                      {
                                          lstrcpy((char*)R, EnumStrH( *tval.def, (int)*(BYTE*)val));
                                      }
                                     }
                                     else lstrcpy((char*)R, (char*)val);
                                      bSet= true; 
                                     break;
                               default: AAA();
                                     break;
                               }                               
                             }
                             else if(pnEndPipeCount != NULL && ((*pnEndPipeCount) < 2))
                                  {
                                     (*pnEndPipeCount)++;
                                     html.m_bNeedPipeEnd = true;
                                  }
                           }
                          
                       }
                    }
                  }
              break;
              }             
          }
        }
     }
   return bSet;
}
//
SValueValue GetTagParam(CHTML& html, CVBScript *pScript, SUniGVAR& gVar, CTag *pTag, char *pparam, char buf[256], UINT *pnEndPipeCount) 
{//buf 256 байт для размещения 
#ifdef _DEBUG
      const char* id = NULL; 
      if(pTag)
      {
        CStyle& style = html.Style(pTag->m_nStyle);
        id = style.NameID(html);
      }
#endif

   SValueValue sval;
   if(pparam)
     {// допустимый париметр - заполняю
      for(int ii=0; stdV[ii].name; ii++)
        {
          if(_stricmp(stdV[ii].name, pparam)==0)
            {              
               if ( pTag->ValueValue( html, stdV[ii].dw, sval ) )
               {
                  if(stdV[ii].dw == VALUE_VALUE)
                    sval.eVal = enumValueStr; 
               }
               break;
            }
          else
          if(_strnicmp(stdV[ii].name, pparam, stdV[ii].dl)==0)
          {
              if(stdV[ii].dw == PARAM_DATAVALUE || stdV[ii].dw == PARAM_DATAALARM)
              {
                  char *pc = strchr(pparam, '(');
                  if(pc)
                  {
                    char *p1 =  strchr(pc+1, '"');
                    if(p1)
                    {
                       cross::SParamValueH tval;
                       if ( pScript->ParamValue( tval, p1, pTag ) )
                       {
                           if(tval.nNumber >=0 && tval.def)
                           {
#ifdef _DEBUG
                             const char* name = html.pipe.GetMnemoName( tval.nNumber );
#endif
                             if(stdV[ii].dw == PARAM_DATAALARM)
                             {
                               static int AlarmSeverity;
                               AlarmSeverity  = 0;
                               CAlarmBase* al = html.pipe.Alarm( tval.nNumber );
                               if(al && al->btEHLN >= 2 && al->btEHLN <= 4)         
                                 AlarmSeverity = 201 + 200*(al->btEHLN - 1);
                               sval.Void = &AlarmSeverity;
                               sval.eVal = enumValueInt; 
                               break;
                             }                        

                             void* val = html.pipe.ParamValue_H( tval );
                             if(val)
                             {
                               sval.Void = val;
                               sval.eVal = tval.def->eVal;                            
                             }
                             else if(pnEndPipeCount != NULL && ((*pnEndPipeCount) < 2))
                                  {
                                     (*pnEndPipeCount)++;
                                     html.m_bNeedPipeEnd = true;
                                  }
                           }
                       }
                       else
                       if(BERTH_ONPZ == Version)
                       {
                          if(tval.nNumber == -2 && strcmp(p1, "\"tagname.cp_execstate\"") ==0)
                          {
                             static const bool b1 = true;
                             sval.Void = (void*)&b1;
                             sval.eVal = enumValueBol;
                          }
                       }
                    }
                  }
              break;
              } 
              else
              if(stdV[ii].dw == PARAM_DATAQUALITY)
              {
                   static const int i192 = 192;
                   sval.Void = (void*)&i192;
                   sval.eVal = enumValueInt;
                   break;
              }
          }
        }
     }
   return sval;
}
//
int Func_EQ       (XAXA)
  {
  EValueTypeScript eR,eA;
  void* R = gVar.Var( html, comm.nRes , eR );
  void* A = gVar.Var( html, comm.nArg1, eA, true );

  if ( R == NULL)
    {//проверка автоматически созданных переменных и определение их типа (пример: DBL = me.DataValue("PZIT2102A.MOSR.PVFL") )
      if(A && eA != _enumValueUnk)
        {
          GVAR& v = gVar.Obj( comm.nRes );
          _types t = TYPE_UNKNOWN;
          switch ( v.nType&0xf )
            {
          case TYPE_VALUE: 
             AAA();
          case TYPE_PARAM:
             switch(eA)
              {
              case _enumValueDbl: t = TYPE_FLOAT; break;
              case _enumValueChr:
              case _enumValueInt: t = TYPE_INT; break;
              case _enumValueBol: t = TYPE_BOOL; break;
              case _enumValueStr: t = TYPE_STRING; break;
              }
            break;
            }
          if(t != TYPE_UNKNOWN)
            {
            v.nType = t | (v.nType & 0xFFF0); 
            R = gVar.Var( html, comm.nRes , eR );
            }
        }
    }
  if ( R == NULL || A == NULL ) return -1;
  ASSERT( comm.nArg2 == -1 );
  //
  char *Old,New[8];
  GVAR& r = gVar.Obj( comm.nRes );

  bool bColor = false;
  if( (r.nDef == VALUE_TEXTCOLOR) || (r.nDef == VALUE_FILLCOLOR) || (r.nDef == VALUE_LINECOLOR) 
       || (r.nDef == VALUE_LEVELFILLCOLOR) || (r.nDef == VALUE_GRADIENTFILLCOLOR) )
    bColor = true;

  if ( r.nDef == PARAM_DATAVALUE )
    {
    // Изменения надо послать в модель
    // Вообще-то это лишний ASSERT
    if ( AS( r.nType == TYPE_PARAM ) ) return -1;
    Old = (char*)R;
    R = New;
    }
  else
  if ( r.nDef == VALUE_ROTATION )
    {
    if ( AS(eR == enumValueDbl) ) 
        return -1;
    if(eA == enumValueDbl)
       *(double*)R = *(double*)R;
    else
    if(eA == enumValueStr)
      *(double*)R = atof((char*)A);    
    memcpy( New, R, 8 );
    }
  else
  if (r.nDef == VALUE_INNERTEXT)
  {
	  char* innerText = (char*) R;
      if(*(char*)A==0)
         lstrcpy(innerText, " "); //иначе при пустом рисуется m_constValue
      else
	  lstrcpy(innerText, (char*)A);
	  return 0;
  }
  else
  if (r.nDef == VALUE_NUMERICDISPLAYFORMAT)
  {
	  char* numericdisplayformat = (char*) R;
	  lstrcpy(numericdisplayformat, (char*)A);
	  return 0;
  }
  else
  if(r.nDef == VALUE_VALUE)
  {
	  SValueValue* svalue = gVar.Value(comm.nRes);
	  AcceptCallback accept = svalue->Accept;
	  CTag* tag = svalue->mTag;
      if(tag)
      {
#ifdef _DEBUG
      CStyle& style = html.Style(tag->m_nStyle);
      const char* id = style.NameID(html);
#endif
	  if (accept && tag && tag->mVal.def && A)
      {
         if(eA == _enumValueStr)
         {
          if(tag->mVal.def->eVal == _enumValueDbl)
          {
              double da = atof((char*)A);
              if(accept(html, tag, (void*)&da)) 
                return 0;
          }
          else
          if(accept(html, tag, A)) 
             return 0;
         }
         else
         {
            if(accept(html, tag, A)) 
              return 0;
         }
      }
      }
  }
  else
  if(r.nDef == WINDOW_EXT_CURRPAGE)
  {
      if(eA == _enumValueStr)
      {
      if(eR != _enumValueStr)
      {
      // SValueValue* svalue = gVar.Value(comm.nRes);
   //    if((CHTML*) svalue->Void)
      //   GetAppWnd()->ButtonOk(barPAGE, (char*)A, ((CHTML*) svalue->Void)->mWnd->m_hWnd);
      }
      else lstrcpy((char*)R, (char*)A);
	  return -1;
      }
  }
  //
  if(eA == _enumValueCastomProperty)
     {
        const char* prop = GetCustomProperty(html, gVar, script->m_vTags, comm.nRes, comm.nArg1);
        if(prop)
        {
          UINT nType = r.nType&0x0F;
          if( (nType==0) || (nType==TYPE_CUSTOMPROPERTY))
            nType = TYPE_STRING;
          if(nType == TYPE_STRING)
          {
            strncpy( (char*)R, prop, r.nSize-1 );
            r.nType = nType | (r.nType&0xFFF0);
          }
        }
     }
  else
  if(eR == _enumValueObjects)
     {
        SCTag *pPropR = (SCTag *)R;
        if(pPropR->objectType == otARRAY)
        {
          char* pp = strchr(pPropR->m_name, '(');
          if(pp==NULL)
            return false;  
          char arname0[256];
          strncpy_s(arname0, pPropR->m_name, pp - pPropR->m_name);
          ::CharUpper( arname0 );
          if(!IsInt(pp+1))
          {
              char indname[256] = "LOCAL_";
              strcat(indname, pp+1);
              ::CharUpper( indname );
              UINT mm = gVar.Find( indname );
              if(mm == -1)
                 return 0;
              EValueTypeScript eIND;
              void* pIND = gVar.Var( html, mm, eIND );
              if(pIND == NULL)
                return 0;
              int ind = Int( eIND, pIND);
              char arname[256];
              sprintf_s(arname, "LOCALARRAY_%s(%d", arname0, ind);
              UINT mmv = gVar.Find( arname );
              if(mmv == -1)
                 return 0;
              EValueTypeScript eRR;
              void* pRR = gVar.Var( html, mmv, eRR );
              if(pRR == NULL)
                return 0;
              if(eA != _enumValueUnk)
              {
                SetTypeAndVal(mmv, (EValueType)eA, A, pRR, gVar);
              }              
          }
          return 0;
        }
        char *pparam = NULL;
        CTag *pTagR = HtmlGetObject(html, script->m_vTags, pPropR, pparam);
        if(pTagR)
        {
#ifdef _DEBUG
        CStyle& style = html.Style(pTagR->m_nStyle);
        const char* id = style.NameID(html);
#endif
          if(pparam)
          {// допустимай париметр - заполняю
           for(int ii=0; stdV[ii].name; ii++)
             {
               if(_stricmp(stdV[ii].name, pparam)==0)
                 {
                    SValueValue val;
                    if ( pTagR->ValueValue( html, stdV[ii].dw, val ) )
                    {                         
                        PVOID RR = val.Void;  
                        bool bColor = false;
                        if( (stdV[ii].dw == VALUE_TEXTCOLOR) || (stdV[ii].dw == VALUE_FILLCOLOR) || (stdV[ii].dw == VALUE_LINECOLOR) 
                           || (stdV[ii].dw == VALUE_LEVELFILLCOLOR) || (stdV[ii].dw == VALUE_GRADIENTFILLCOLOR) )
                          bColor = true;
                        switch ( val.eVal )
                         {
                         case enumValueDbl: if ( stdV[ii].dw != VALUE_VALUE )
                                              {double* Z = (double*)RR; *Z = Dbl( eA, A );} 
                                            else
                                            switch ( eA )
                                            {
                                            case enumValueDbl:
                                               sprintf_s((char*)RR, 128, "%f", double(*(double*)A));
                                               break;
                                            case enumValueInt:
                                               sprintf_s((char*)RR, 64, "%d", int(*(int*)A));
                                               break;                                           
                                            case enumValueStr:
                                               {
                                               char* r = (char*)RR;
                                               char* a = (char*)A;
                                               lstrcpy( r, a );
                                               }
                                               break;
                                            case _enumValueObjects:
                                               {
                                                 char* r = (char*)RR;
                                                 SCTag *pPropA = (SCTag *)A;
                                                 O_VAL oval;
                                                 if(GetDataFromObject(&oval, A, eA, html, gVar, script))
                                                 {
                                                 if(oval.eType == _enumValueStr)
                                                    lstrcpy( r, oval.ostr );
                                                 AAA();
                                                 }
                                               }
                                               break;
                                            }
                                            return 0;
                         case enumValueChr:
                         case enumValueInt: {int   * Z = (int   *)RR; *Z = Int( eA, A, bColor );} 
                                            return 0;
                         case enumValueBol: {bool  * Z = (bool  *)RR; *Z = Bol( eA, A );} 
                                            return 0;
                         case enumValueStr:
                           {
                           switch(eA)
                            {
                         case enumValueInt:
                             *((double*)RR) = (double)*((int*)A); 
                            return 0;
                         case enumValueDbl:
                             *((double*)RR) = *(double*)A;
                            return 0;
                         case enumValueStr:
                         default:
                           char* r = (char*)RR;
                           char* a = (char*)A;
                           lstrcpy( r, a );
                           return 0;
                            }
                         }
                        }
                    }
                 }
             }
           return 0; //не нашли параметр
          }
          else
          { //это CTag а не его параметр
              pPropR->m_pTag = pTagR;
          }
        }
        else
        if(eA == _enumValueObjects)
          {
           SCTag *pPropA = (SCTag *)A;
           char *pparam = NULL;
           bool bb = false;
           CTag *pTagA = HtmlGetObject(html, script->m_vTags, pPropA, pparam);
           if(pTagA)
            {
#ifdef _DEBUG
        CStyle& style = html.Style(pTagA->m_nStyle);
        const char* id = style.NameID(html);
#endif
            if(pparam==NULL)
              {
              const char* rr = gVar.Name( comm.nRes  );
              char *rr2= (char*)strchr(rr, '_');
              if(rr2==NULL)
                 rr2 = (char*)rr;
              else rr2++;
              script->m_vTags[rr2] = pTagA; 
              pPropR->m_pTag = pTagA;
              //memcpy(R, A, sizeof(SCTag)); 
              }
            else
              {
                 bb = GetTagParamSetRes(html, script, gVar, comm.nRes, pTagA, pparam, bColor);
              }
             AAA();
            } 
           else
            {
             if(pPropA)
              {
              char objname[128]="";
              char *pp = strchr(pPropA->m_name, '.');
              if(pp)
                {
                strncpy_s(objname, pPropA->m_name, pp - pPropA->m_name);
                _strupr_s(objname);
                std::map<std::string, CTag*>::iterator it = script->m_vTags.find(objname);
                if(it!= script->m_vTags.end())
                  {
                     bb = GetTagParamSetRes(html, script, gVar, comm.nRes, it->second, pp+1, bColor, &pPropA->nEndPipeCount);
                  }
                AAA();
                }
              else
                {
                    pPropR->m_pTag = pPropA->m_pTag;
                    if(pPropR->m_name[0])
                      script->m_vTags[pPropR->m_name] = pPropA->m_pTag;
                    else
                    {
                     const char* rr = gVar.Name( comm.nRes  );
                     char *rr2= (char*)strchr(rr, '_');
                     if(rr2==NULL)
                        rr2 = (char*)rr;
                     else rr2++;
                     script->m_vTags[rr2] = pPropA->m_pTag;
                    }
                }
              }
            }
          }
        else 
        if(r.nDef == KEY_DIM) 
        {
         if(eA == _enumValueStr)
          {
            r.nType = TYPE_STRING | (r.nType&0xFFF0);
            strncpy((char*)R, (char*)A, r.nSize-1);
          }
         else
         if(eA == _enumValueInt)
         {
            r.nType = TYPE_INT | (r.nType&0xFFF0);
            *(int*)R = *(int*)A;
         }
         else
         if(eA == _enumValueDbl)
         {
            r.nType = TYPE_FLOAT | (r.nType&0xFFF0);
            *(double*)R = *(double*)A;
         }
        }
        return 0;
     }
  else
      if(eA == _enumValueObjects)
      {
           SCTag *pPropA = (SCTag *)A;
           char *pparam = NULL;
           bool bb = false;
           CTag *pTagA = HtmlGetObject(html, script->m_vTags, pPropA, pparam);
           if(pPropA)
              {
              char objname[128]="";
              char *pp = strchr(pPropA->m_name, '.');
              if(pp)
              if(pPropA->objectType==otTAG)
                {
                strncpy_s(objname, pPropA->m_name, pp - pPropA->m_name);
                _strupr_s(objname);
                std::map<std::string, CTag*>::iterator it = script->m_vTags.find(objname);
                if(it!= script->m_vTags.end())
                  {
                   bb = GetTagParamSetRes(html, script, gVar, comm.nRes, it->second, pp+1, bColor, &pPropA->nEndPipeCount);
                  }
                return 0;
                }
              else
              {
                 O_VAL oval;
                 if(GetDataFromObject(&oval, A, eA, html, gVar, script))
                 {
                     eA = (EValueTypeScript)oval.eType;      
                     A = &oval.ostr;
                     AAA();
                 }                     
              }
             }
      }

  if ( eR == enumValueDbl && eA == enumValueStr )
    {
    GVAR& v = gVar.Obj(comm.nRes);
    if ( v.nDef == VALUE_VALUE )
      {
      nParam = 1;
      char* r = (char*)R;
      char* a = (char*)A;
      lstrcpy( r, a );
      }
    else
    if ( v.nDef == KEY_DIM)
      {
      char* r = (char*)R;
      char* a = (char*)A;
      v.nType = TYPE_STRING;
      lstrcpy( r, a );
      }
    else
      {
      ASSERT(0);
      }
    }
  else
  switch ( eR )
    {
    case enumValueDbl: {double* Z = (double*)R; *Z = Dbl( eA, A );} break;
    case enumValueChr:
    case enumValueInt: {int   * Z = (int   *)R; *Z = Int( eA, A, bColor );} break;
    case enumValueBol: {bool  * Z = (bool  *)R; *Z = Bol( eA, A );} break;
    case enumValueStr:
      {
      switch(eA)
       {
    case enumValueChr:
        {
        //*((double*)R) = (double)*((char*)A); 
        char buf[32];
       _itoa_s((int)*((char*)A), buf, sizeof(buf)-1, 10); 
        lstrcpy( (char*)R, buf );
        }
       break;
    case enumValueInt:
        if(r.nDef == VALUE_VALUE)
           *((double*)R) = (double)*((int*)A);
        else
         {
        char buf[128];
        sprintf_s(buf, "%d", *(int*)A); 
        lstrcpy( (char*)R, buf );
         }
       break;
    case enumValueDbl:
        {
         if(r.nDef == VALUE_VALUE)
           *((double*)R) = *(double*)A;
         else
         {
        char buf[128];
        sprintf_s(buf, "%.3f", *(double*)A); 
        lstrcpy( (char*)R, buf );
         }
        }
       break;
    case _enumValueObjects:
       AAA();  //неправильно что-то
       break;
    default:
      ASSERT( eA == eR );    
      char* rr = (char*)R;
      char* aa = (char*)A;
      if(r.nSize > strlen(rr))
        lstrcpy( rr, aa );
      else AAA();
      break;
       }
      }
    break;
    case _enumValueTag:
     if(eA==_enumValueTag)
     {
      SCTag *pSctagA = (SCTag *)A;
      SCTag *pSctagR = (SCTag *)R;
      if(pSctagA)
      {
      CTag *pTag = pSctagA->m_pTag;
      if(pTag)
        {
#ifdef _DEBUG
        CStyle& style = html.Style(pTag->m_nStyle);
        const char* id = style.NameID(html);
#endif
        const char* rr = gVar.Name( comm.nRes  );
        char *rr2= (char*)strchr(rr, '_');
        if(rr2==NULL)
           rr2 = (char*)rr;
        else rr2++;
        script->m_vTags[rr2] = pTag;
        memcpy(R, A, sizeof(SCTag));       
        }
      else
        {
           char *pp = strchr(pSctagA->m_name, '.');
           if(pp)
           {
             char objname[32]="";
             strncpy_s(objname, pSctagA->m_name, pp-pSctagA->m_name);
             strupr(objname);
             std::map<std::string, CTag*>::iterator it = script->m_vTags.find(objname);
             if(it!= script->m_vTags.end())
             {
               CTag *pTag = it->second; 
               while(_strnicmp(pp, ".parentnode", _countof(".parentnode")-1)==0)
                 {
                    if(pTag->m_pPar)
                      pTag = pTag->m_pPar;
                    else return -1; //нет такого родителя                  
#ifdef _DEBUG
      CStyle& style = html.Style(pTag->m_nStyle);
      const char* id = style.NameID(html);
#endif
                 pp += _countof(".parentnode")-1;
                 }
               const char* rr = gVar.Name( comm.nRes  );
               char *rr2= (char*)strchr(rr, '_');
               if(rr2==NULL)
                  rr2 = (char*)rr;
               else rr2++;
               script->m_vTags[rr2] = pTag;
               //pSctag->m_pTag = pTag;
               //memcpy(R, A, sizeof(SCTag)); 
               pSctagR->m_pTag = pTag;
             }
           }
        }
      }
     }
    break;
    case _enumValueVariant:
      SetTypeAndVal(comm.nRes, (EValueType)eA, A, R, gVar);
    break;
    default:
      ASSERT(0);
    break;
    }
  //
  if ( r.nDef == PARAM_DATAVALUE )
    {
    cross::SParamValueH* v = gVar.Param( comm.nRes );
    if ( AS (v->def != NULL) ) return -1;
    SSendToModel send;
    lstrcpy( send.szValue, v->def->name );// Имя переменной
    send.eType = v->def->eVal;// Тип переменной
    send.Set( Old, New );
    html.pipe.SendDataEx( v->nNumber, send );
    }
  //
  return 0;
  }
//
int Func_EQ_EQ    (XAXA)
  {
  EValueTypeScript eR,eA,eB;
  void* R = gVar.Var( html, comm.nRes , eR );
  void* A = gVar.Var( html, comm.nArg1, eA, true );
  void* B = gVar.Var( html, comm.nArg2, eB );
  if ( R == NULL || A == NULL || B == NULL ) return -1;
  if ( eA == (EValueTypeScript)enumValueFlt ) eA = eB; //Локальные переменные

  bool* Z = (bool*)R;
  if ( eR != (EValueTypeScript)enumValueBol )
    {
    ASSERT(0);
    return -1;
    }
  //
  switch ( eA )
    {
    case enumValueDbl: {*Z = Dbl( eA, A ) == Dbl( eB, B );} break;
    case enumValueChr: {*Z = Int( eA, A ) == Int( eB, B );} break;
    case enumValueInt: {*Z = Int( eA, A ) == Int( eB, B );} break;
    case enumValueBol: {*Z = Bol( eA, A ) == Bol( eB, B );} break;
    case enumValueStr:
         switch(eB)
         {
         case enumValueInt:
             *Z = int(atof((char*)A)) == Int( eB, B );
             break;
         case enumValueStr:
             *Z = strcmp((char*)A, (char*)B) == 0;
             break;
         case enumValueDbl:
             *Z = atof((char*)A) == Dbl( eB, B );
             break;
         }
         break;
    case _enumValueObjects:                
          {
           SCTag *pPropA = (SCTag *)A;
           char *pparam = NULL;
           bool bb = false;
           CTag *pTagA = HtmlGetObject(html, script->m_vTags, pPropA, pparam);
           if(pTagA)
            {
#ifdef _DEBUG
        CStyle& style = html.Style(pTagA->m_nStyle);
        const char* id = style.NameID(html);
#endif
            if(pparam!=NULL)
              {
              for(int ii=0; stdV[ii].name; ii++)
                {
                  if(_stricmp(stdV[ii].name, pparam)==0)
                    {
                       SValueValue val;
                       if ( pTagA->ValueValue( html, stdV[ii].dw, val ) )
                       {
                          PVOID A = val.Void;  
                          EValueTypeScript eA = (EValueTypeScript)val.eVal;
                          if(stdV[ii].dw == VALUE_VALUE)
                             eA = _enumValueStr; 
                          switch ( eA )
                            {
                            case enumValueDbl: {*Z = Dbl( eA, A ) == Dbl( eB, B );} break;
                            case enumValueChr: {*Z = Int( eA, A ) == Int( eB, B );} break;
                            case enumValueInt: {*Z = Int( eA, A ) == Int( eB, B );} break;
                            case enumValueBol: {*Z = Bol( eA, A ) == Bol( eB, B );} break;
                            case enumValueStr:
                                 switch(eB)
                                 {
                                 case enumValueInt:
                                     *Z = int(atof((char*)A)) == Int( eB, B );
                                     break;
                                 }
                                 break;
                            }
                          break;
                      }
                    }
                }
              }
             AAA();
            }
           else
           if(eA == _enumValueObjects)
           {
           if(pPropA)
              {
              char objname[128]="";
              char *pp = strchr(pPropA->m_name, '.');
              if(pp)
                {
                strncpy_s(objname, pPropA->m_name, pp - pPropA->m_name);
                _strupr_s(objname);
                std::map<std::string, CTag*>::iterator it = script->m_vTags.find(objname);
                if(it!= script->m_vTags.end())
                  {
                   char buf[256];
                   SValueValue sval = GetTagParam(html, script, gVar, it->second, pp+1, buf, &pPropA->nEndPipeCount); 
                   if(sval.Void && sval.eVal != enumValueUnk)
                   {
                      switch ( sval.eVal )
                            {
                            case enumValueDbl: {*Z = Dbl( (EValueTypeScript)sval.eVal, sval.Void ) == Dbl( eB, B );} break;
                            case enumValueChr: {*Z = Int( (EValueTypeScript)sval.eVal, sval.Void ) == Int( eB, B );} break;
                            case enumValueInt: {*Z = Int( (EValueTypeScript)sval.eVal, sval.Void ) == Int( eB, B );} break;
                            case enumValueBol: {*Z = Bol( (EValueTypeScript)sval.eVal, sval.Void ) == Bol( eB, B );} break;
                            case enumValueStr:
                                 switch(eB)
                                 {
                                 case enumValueInt:
                                     *Z = int(atof((char*)sval.Void)) == Int( eB, B );
                                     break;
                                 }
                                 break;
                            }
                   AAA();
                   }
                  }
                return 0;
                }
             }
           }
          }
        break;
    default:
      ASSERT(0);
    break;
    }
  return 0;
  }
//
int Func_NOT(XAXA)
  {
  EValueTypeScript eR,eA;
  bool* R = (bool*)gVar.Var( html, comm.nRes , eR );
  void* A =        gVar.Var( html, comm.nArg1, eA );
  if ( R == NULL || A == NULL ) return -1;
  if ( As(eR == enumValueBol) ) return -1;
  switch ( eA )
    {
    case enumValueBol: {*R = !Bol( eA, A );} break;
    default:ASSERT(0);break;
    }
  return 0;
  }
//
int Func_MSGBOX   (XAXA)
  {
  EValueTypeScript eR,eA,eB,eC;
  void* R = gVar.Var( html, comm.nRes , eR );
  void* A = gVar.Var( html, comm.nArg1, eA );
  void* B = gVar.Var( html, comm.nArg2, eB );
  void* C = gVar.Var( html, comm.nArg3, eC );
  if ( R == NULL || A == NULL || B == NULL || C == NULL ) return -1;
  ASSERT( eR == enumValueInt || eB == enumValueInt );
  ASSERT( eA == enumValueStr || eC == enumValueStr );
  //
  int * r = (int *)R;int * b = (int *)B;
  char* a = (char*)A;char* c = (char*)C;
  *r = GetAppWnd()->MessageBox( a, c, *b |MB_TOPMOST );
  return 0;
  }
//
int Func_ABS      (XAXA)
  {
  EValueTypeScript eR,eA;
  void* R = gVar.Var( html, comm.nRes , eR );
  void* A = gVar.Var( html, comm.nArg1, eA );
  if ( R == NULL || A == NULL ) return -1;

  if ( eR == enumValueDbl && eA == enumValueDbl )
    {
    double* r = (double*)R;
    double* a = (double*)A;
    *r = fabs(*a);
    }
  else
  if ( eR == enumValueInt && eA == enumValueInt )
    {
    int* r = (int*)R;
    int* a = (int*)A;
    *r = abs(*a);
    }
  else
    {
    ASSERT(0);
    }
  return 0;
  }
//
int Func_CDBL     (XAXA)
  {
  EValueTypeScript eR,eA;
  double* R = (double*)gVar.Var( html, comm.nRes , eR );
  char  * A = (char  *)gVar.Var( html, comm.nArg1, eA );
  if ( R == NULL || A == NULL ) return -1;
  ASSERT( eR == enumValueDbl || eA == enumValueStr );
  //
  *R = atof(A);
  return 0;
  }
//
int Func_CINT     (XAXA)
  {
  EValueTypeScript eR,eA;
  int * R = (int *)gVar.Var( html, comm.nRes , eR );
  char* A = (char*)gVar.Var( html, comm.nArg1, eA );
  if ( R == NULL || A == NULL ) return -1;
  ASSERT( eR == enumValueInt || eA == enumValueStr );
  //
  *R = atoi(A);
  return 0;
  }

int Func_LEN(XAXA)
  {
  EValueTypeScript eR,eA;
  int * R = (int *)gVar.Var( html, comm.nRes , eR );
  char* A = (char*)gVar.Var( html, comm.nArg1, eA );
  if ( R == NULL || A == NULL ) return -1;
  ASSERT( eR == enumValueInt || eA == enumValueStr );
  //
  *R = (int)strlen(A);
  return 0;
  }
//
int Func_FORM_N(XAXA)
  {
  EValueTypeScript eR,eA,eB;
  void* R = gVar.Var( html, comm.nRes , eR );
  void* A = gVar.Var( html, comm.nArg1, eA );
  void* B = gVar.Var( html, comm.nArg2, eB );
  if ( R == NULL || A == NULL  || B == NULL ) return -1;
  ASSERT( eR == enumValueStr );
  ASSERT( eA == enumValueDbl );
  ASSERT( eB == enumValueInt );
  //GVAR& sR = gVar.Obj( comm.nRes  );
  //GVAR& sA = gVar.Obj( comm.nArg1 );
  //GVAR& sB = gVar.Obj( comm.nArg2 );
  char * r = (char*)R;
  double a = Dbl( eA, A );
  int    b = Int( eB, B );
  char c[MAX_LOCAL];
  sprintf_s( c, MAX_LOCAL, "%%.%df", b );
  sprintf_s( r, MAX_LOCAL, c, a );
  return 0;
  }
//
int Func_CSTR     (XAXA)
  {
  EValueTypeScript eR,eA;
  char* R = (char*)gVar.Var( html, comm.nRes , eR );
  void* A = (int *)gVar.Var( html, comm.nArg1, eA );
  if ( R == NULL || A == NULL ) return -1;
  ASSERT( eR == enumValueStr );
  //
  if ( eA == enumValueInt )
    {
    int* a = (int*)A;
    sprintf_s( R, MAX_LOCAL, "%d", *a );
    }
  else
  if ( eA == enumValueChr )
    {
    char* a = (char*)A;
    sprintf_s( R, MAX_LOCAL, "%d", int(*a) );
    }
  else
  if ( eA == enumValueDbl )
    {
    double* a = (double*)A;
    sprintf_s( R, MAX_LOCAL, "%.1f", *a );
    }
  if ( eA == enumValueStr )
    {
    lstrcpy( (char*)R, (char*)A );
    }
  else
    {
    ASSERT(0);
    }
  return 0;
  }
//
int Func_ISNUME(XAXA)
  {
  //Dim MyVar, MyCheck
  //MyVar = 53   ' Assign a value.
  //MyCheck = IsNumeric(MyVar)   ' Returns True.
  //MyVar = "459.95"   ' Assign a value.
  //MyCheck = IsNumeric(MyVar)   ' Returns True.
  //MyVar = "45 Help"   ' Assign a value.
  //MyCheck = IsNumeric(MyVar)   ' Returns False.
  EValueTypeScript eR,eA;
  bool* R = (bool*)gVar.Var( html, comm.nRes , eR );
  char* A = (char*)gVar.Var( html, comm.nArg1, eA );
  if ( R == NULL || A == NULL ) return -1;
  ASSERT( eR == enumValueBol || eA == enumValueStr );
  //
  *R = ::IsFlt(A);
  return 0;
  }
//
int Func_ISNAN(XAXA)
{
  EValueTypeScript eR,eA;
  bool* R = (bool*)gVar.Var( html, comm.nRes , eR );
  double* A = (double*)gVar.Var( html, comm.nArg1, eA );
  if ( R == NULL || A == NULL ) return -1;
  if( eR != enumValueBol || eA != enumValueDbl ) return -1;
  //
  *R = IsNaN(*A);
  return 0;
}

int Func_CHR      (XAXA)
  {
  EValueTypeScript eR,eA;
  char* R = (char*)gVar.Var( html, comm.nRes , eR );
  int * A = (int *)gVar.Var( html, comm.nArg1, eA );
  if ( R == NULL || A == NULL ) return -1;
  ASSERT( eR == enumValueStr || eA == enumValueInt );
  R[0] = char(*A);
  R[1] = 0;
  return 0;
  }
//
int Func_BRACE(XAXA)
  {
  EValueTypeScript eR,eA;
  void* R = gVar.Var( html, comm.nRes , eR );
  void* A = gVar.Var( html, comm.nArg1, eA );
  if ( R == NULL || A == NULL ) return -1;

  if ( eR == enumValueDbl && eA == enumValueDbl )
    {
    double* r = (double*)R;
    double* a = (double*)A;
    *r = *a;
    }
  else
  if ( eR == enumValueInt && eA == enumValueInt )
    {
    int* r = (int*)R;
    int* a = (int*)A;
    *r = *a;
    }
  else
  if ( eR == enumValueBol && eA == enumValueBol )
    {
    bool* r = (bool*)R;
    bool* a = (bool*)A;
    *r = *a;
    }
  else
  if ( eR == _enumValueObjects && eA == enumValueDbl )
    {
    *(double*)R = *(double*)A;
    GVAR& r = gVar.Obj( comm.nRes );
    r.nType = TYPE_FLOAT | (r.nType&0xFFF0);
    }
  else
  if ( eR == _enumValueObjects && eA == enumValueInt )
    {
    *(int*)R = *(int*)A;
    GVAR& r = gVar.Obj( comm.nRes );
    r.nType = TYPE_INT | (r.nType&0xFFF0);
    }
  else
    {
    ASSERT(0);
    }
  return 0;
  }
//
int Func_TRIM(XAXA)
  {
  EValueTypeScript eR,eA;
  char* R = (char*)gVar.Var( html, comm.nRes , eR );
  char* A = (char*)gVar.Var( html, comm.nArg1, eA );
  if ( R == NULL || A == NULL ) return -1;
  ASSERT( eR == enumValueStr || eA == enumValueStr );
  //
  while ( *A == ' ' ) A++;
  lstrcpy( R, A );
  int dl = lstrlen(R)-1;
  if ( dl >= 0 ) while ( R[dl] == ' ' ) R[dl] = 0, dl--;
  return 0;
  }
//
int Func_LTRIM(XAXA)
  {
  EValueTypeScript eR,eA;
  char* R = (char*)gVar.Var( html, comm.nRes , eR );
  char* A = (char*)gVar.Var( html, comm.nArg1, eA );
  if ( R == NULL || A == NULL ) return -1;
  ASSERT( eR == enumValueInt || eA == enumValueStr );
  //
  while ( *A == ' ' ) A++;
  lstrcpy( R, A );
  return 0;
  }
//
int Func_RTRIM(XAXA)
  {
  EValueTypeScript eR,eA;
  char* R = (char*)gVar.Var( html, comm.nRes , eR );
  char* A = (char*)gVar.Var( html, comm.nArg1, eA );
  if ( R == NULL || A == NULL ) return -1;
  ASSERT( eR == enumValueInt || eA == enumValueStr );
  //
  lstrcpy( R, A );
  int dl = lstrlen(R)-1;
  if ( dl >= 0 ) while ( R[dl] == ' ' ) R[dl] = 0, dl--;
  return 0;
  }
//
// Function InStr([Start], [String1], [String2], [Compare As VbCompareMethod = vbBinaryCompare])
int Func_INSTR(XAXA)
  {
  EValueTypeScript eR,eA,eB,eC;
  int * R = (int *)gVar.Var( html, comm.nRes , eR );
  int * A = (int *)gVar.Var( html, comm.nArg1, eA );
  char* B = (char*)gVar.Var( html, comm.nArg2, eB );
  char* C = (char*)gVar.Var( html, comm.nArg3, eC );
  if ( R == NULL || A == NULL ) return -1;
  if ( B == NULL) return -1;
  if ( eR != enumValueInt ) 
     return -1;
  if ( eB != enumValueStr )
     return -1;

  O_VAL oval;
  if(eA == _enumValueObjects)
  {   
      if(!GetDataFromObject(&oval, A, eA, html, gVar, script))
         return -1; 
      eA = (EValueTypeScript)oval.eType;
      if(oval.eType ==  enumValueStr) 
        A = (int*)oval.ostr;
      else return -1;
  }

  if(C==NULL)
  {//только два параметра - где и что
      if ( eA != enumValueStr ) 
          return -1;
      char *str = (char*)A;
      char *rr = strstr(str, B);
      if(rr==NULL)
        *R = 0;
      else *R = int(rr-str)+1;
      return 0;
  }

  if ( eA != enumValueInt ) return -1;
  if ( eC != enumValueStr ) return -1;

  //
  //If                              InStr returns
  //string1 is zero-length          0
  if ( lstrlen( B ) == 0 ) { *R =  0; return 0; }
  //string1 is Null                 Null Этого не может быть
  //string2 is zero-length          start
  if ( lstrlen( C ) == 0 ) { *R = *A; return 0; }
  //string2 is Null                 Null Этого не может быть
  //string2 is not found            0
  char* a = strchr( B+*A-1, *C );
  if ( a == NULL ) { *R = 0; return 0; }
  //string2 is found within string1 Position at which match is found
  DWORD d = DWORD(a)-DWORD(B)+1;
  *R = d;
  //start > Len(string2)            0
  //
  return 0;
  }
// Function Mid(String, Start As Long, [Length])
int Func_MID(XAXA)
  {
  EValueTypeScript eR,eA,eB,eC;
  char* R = (char*)gVar.Var( html, comm.nRes , eR );
  char* A = (char*)gVar.Var( html, comm.nArg1, eA );
  int * B = (int *)gVar.Var( html, comm.nArg2, eB );
  int * C = (int *)gVar.Var( html, comm.nArg3, eC );
  if ( R == NULL || A == NULL || B == NULL ) return -1;
  if ( eR != enumValueStr ) return -1;
  if ( eA != enumValueStr ) return -1;
  if ( eB != enumValueInt ) return -1;
  if ( C != NULL && eC != enumValueInt ) return -1;
  //
  int dl = lstrlen(A);
  if ( dl <= *B ) { *R = 0; return 0; }
  lstrcpy( R, A+*B-1 );
  //
  if ( C != NULL )
    {
    dl = lstrlen(R);
    if ( dl > *C ) R[*C] = 0;
    }
  return 0;
  }
// Function Left(String, Length As Long)
int Func_LEFT(XAXA)
  {
  EValueTypeScript eR,eA,eB;
  char* R = (char*)gVar.Var( html, comm.nRes , eR );
  char* A = (char*)gVar.Var( html, comm.nArg1, eA );
  int * B = (int *)gVar.Var( html, comm.nArg2, eB );
  if ( R == NULL || A == NULL || B == NULL ) return -1;
  if ( eR != enumValueStr ) return -1;
  if ( eA != enumValueStr ) return -1;
  if ( eB != enumValueInt ) return -1;
  lstrcpy( R, A );
  int dl = lstrlen(R);
  if ( dl > *B ) R[*B] = 0;
  return 0;
  }
// Function Right(String, Length As Long)
int Func_RIGHT(XAXA)
  {
  EValueTypeScript eR,eA,eB;
  char* R = (char*)gVar.Var( html, comm.nRes , eR );
  char* A = (char*)gVar.Var( html, comm.nArg1, eA );
  int * B = (int *)gVar.Var( html, comm.nArg2, eB );
  if ( R == NULL || A == NULL || B == NULL ) return -1;
  if ( eR != enumValueStr ) return -1;
  if ( eA != enumValueStr ) return -1;
  if ( eB != enumValueInt ) return -1;
  lstrcpy( R, A );
  int dl = lstrlen(A);
  if ( dl > *B )
    lstrcpy( R, A+dl-*B );
  else
    lstrcpy( R, A );
  return 0;
  }
//
int Func_CreatePopupWindow(XAXA)
  {
  EValueTypeScript eR,eA;
  bool* R = (bool*)gVar.Var( html, comm.nRes , eR );
  char* A = (char*)gVar.Var( html, comm.nArg1, eA );
  if ( R == NULL || A == NULL ) return -1;
  if ( eR != enumValueBol) return -1;
  *R = true;
  if ( eA != enumValueStr ) return -1;
  char *pp1 = A;
  while(char *pp2 = strchr(pp1, '\\'))
       pp1 = pp2+1;
  char temp[512];
  strcpy_s(temp, pp1);
  _strlwr(temp);
  if(char *pcur = strstr(temp, "?currentpoint="))
    lstrcpy(pp1 + (pcur-temp) + 1, pp1 + (pcur-temp) + _countof("?currentpoint=")-1); 

//  HWND hChild = (HWND)GetAppWnd()->SendMessage( nPressQueue, QUEUE_POPUP, (LPARAM)pp1 );
  return 0;
  }
//
int Func_Typename(XAXA)
  {
  EValueTypeScript eR,eA;
  char* R = (char*)gVar.Var( html, comm.nRes , eR );
  void* A = (char*)gVar.Var( html, comm.nArg1, eA );
  if ( R == NULL || A == NULL ) return -1;
  if( eR == enumValueStr || eA == _enumValueObjects )
  {
        SCTag *pProp = (SCTag *)A;
        char *pparam = NULL;
        CTag *pTag = pProp->m_pTag;
        if(pTag==NULL)
           pTag = HtmlGetObject(html, script->m_vTags, pProp, pparam);
        if(pTag)
        {
            lstrcpy(R, typeid(*pTag).name());  
            KKK();
        }
        else strcpy(R, "Nothing");  //что должна возвращать не знаю, но если объекта нет - нужно возвращать "Nothing"
  }

  return 0;
  }
//
int Func_IsNull(XAXA)
{
  EValueTypeScript eR,eA;
  bool* R = (bool*)gVar.Var( html, comm.nRes , eR );
  void* A = (void*)gVar.Var( html, comm.nArg1, eA );
  if ( R == NULL || A == NULL ) return -1;

  if( eR == enumValueBol )
    if(eA == _enumValueObjects)
    {
        SCTag *pProp = (SCTag *)A;
        char *pparam = NULL;
        CTag *pTag = pProp->m_pTag;
        if(pTag==NULL)
           pTag = HtmlGetObject(html, script->m_vTags, pProp, pparam);
        if(pTag)
           *R = false;  
        else *R = true;
    }
    else
    if(eA == _enumValueStr)
    {
        if(*(char*)A)
          *R = false;
        else *R = true;  
    }

  return 0;
}
//
int Func_Vartype(XAXA)
  {
  EValueTypeScript eR,eA;
  int* R = (int*)gVar.Var( html, comm.nRes , eR );
  void* A = (char*)gVar.Var( html, comm.nArg1, eA );
  if ( R == NULL || A == NULL ) return -1;
  if( eR == enumValueInt)
  {
      switch(eA)
      {
      case enumValueDbl: *R = 4; break;
      case enumValueInt: *R = 2; break;
      case enumValueStr: *R = 8; break;
      case enumValueBol: *R = 11; break; 
      case _enumValueObjects: *R = 9; break;
      default: *R = 1; break;
      }
  }

  return 0;
  }

int Func_LogMessage(XAXA)
{
  EValueTypeScript eR,eA;
  char* R = (char*)gVar.Var( html, comm.nRes , eR );
  char* A = (char*)gVar.Var( html, comm.nArg1, eA );
  return 0;
}

int Func_Getcustomproperty(XAXA)
{
  EValueTypeScript eR,eA1,eA2,eA3;
  char* R = (char*)gVar.Var( html, comm.nRes , eR );
  SCTag* A1 = (SCTag*)gVar.Var( html, comm.nArg1, eA1 );
  char* A2 = (char*)gVar.Var( html, comm.nArg2, eA2 );
  char* A3 = (char*)gVar.Var( html, comm.nArg3, eA3 );
  if( R==NULL || A1==NULL || A2==NULL || A3==NULL )
     return -1;
  if(eA1 == _enumValueTag && eA2 == enumValueStr && eA3 == enumValueStr)
    {
      if(A1->m_pTag)
       {
        char *variant = A2;
        char *param = A3;
        const char *prop = Alias(html, A1->m_pTag, param);
        if(prop)
          {
           if(strcmp(prop, param)==0)
           {
              *R = '\0';  //нет такого
              return 0;
           }
          if(_stricmp(variant, "Parameter")==0)
              lstrcpy(R, prop);          
          else
          if(_stricmp(variant, "Text")==0)               
              lstrcpy(R, prop);
          else
          if(_stricmp(variant, "Style")==0)
              lstrcpy(R, prop);
          else
          if(_stricmp(variant, "Num")==0)
             lstrcpy(R, prop);
          else 
          if(_stricmp(variant, "Point")==0)
             lstrcpy(R, prop);
          else 
          if(_stricmp(variant, "Real")==0)
             lstrcpy(R, prop);
          else AAA();

          return 0;
          }
       }
    }
  return -1;
}

int Func_PAGEGETATTRIBUTE(XAXA)
{
  EValueTypeScript eR,eA;
  char* R = (char*)gVar.Var( html, comm.nRes , eR );
  char* A = (char*)gVar.Var( html, comm.nArg1, eA );
  if( R==NULL || A==NULL  )
     return -1;

  if(stricmp(A, "lastselection")==0)
  {
      if(eR == _enumValueObjects)
      {
         SCTag *pObj = (SCTag *)R;
         pObj->m_pTag = html.m_psrcelement;
         return 0;
      }
  }

  return -1;
}

int Func_PAGESETATTRIBUTE(XAXA)
{
  EValueTypeScript eR,eA,eB;
  bool* R = (bool*)gVar.Var( html, comm.nRes , eR );
  char* A = (char*)gVar.Var( html, comm.nArg1, eA );
  char* B = (char*)gVar.Var( html, comm.nArg1, eB );
  if( R==NULL || A==NULL  )
     return -1;

  return 0;
}

int Func_AlarmStateIconfactory(XAXA)
{
  EValueTypeScript eR,eA1;
  char* R = (char*)gVar.Var( html, comm.nRes , eR );
  char* A1 = (char*)gVar.Var( html, comm.nArg1, eA1 );
  if( R==NULL || A1==NULL  )
     return -1;

  return -1;
}

int Func_getElementById(XAXA)
{
  EValueTypeScript eR,eA;
  void* R = (int*)gVar.Var( html, comm.nRes , eR );
  const char* A = (char*)gVar.Var( html, comm.nArg1, eA );
  if ( R == NULL || A == NULL ) return -1;
  if ( eR != _enumValueObjects)  return -1;

  if ( eA == _enumValueObjects) 
  {
      SCTag *pPropA = (SCTag *)A;
      if(pPropA->m_pTag)
      {
        CStyle& style = pPropA->m_pTag->Style(html);
        A = style.NameID(html);
        eA = _enumValueStr;
      }
  }

  if ( eA != enumValueStr) 
      return -1;

#ifdef _DEBUG
   if(_strnicmp(A, "alpha", _countof("alpha")-1)==0)
      AAA();
   if(_strnicmp(A, "Line", _countof("Line")-1)==0)
      AAA();
#endif

  CTag* pTagA = html.FindTagInBodyById(html, A );
  if (pTagA)
    {
       SCTag *pPropR = (SCTag *)R;
       pPropR->m_pTag = pTagA;
    }

  return 0;
}

int Func_FORTO(XAXA)
{
  EValueTypeScript eR,eA,eB,eC;
  int* R = (int*)gVar.Var( html, comm.nRes , eR );
  int* A = (int*)gVar.Var( html, comm.nArg1, eA );
  int* B = (int*)gVar.Var( html, comm.nArg2, eB );
  int* C = (int*)gVar.Var( html, comm.nArg3, eC );

  if( A && B && C && eA==enumValueInt &&  eB==enumValueInt &&  eC==enumValueInt)
    {
    if(*B <= *C)
      {
      return 0;
      }

    return *A;
    }
  return -1;
}

int Func_NEXT(XAXA)
{
  EValueTypeScript eR, eA;
  int* R = (int*)gVar.Var( html, comm.nRes , eR );
  int* A = (int*)gVar.Var( html, comm.nArg1 , eA );

  if (R && eR==enumValueInt && eA && eA==enumValueInt)
   {
     (*A)++;
     return *R;
   }

  return -1;
}

int Func_CloseAllWindows(XAXA)
{
  if(html.mWnd)
    {
    cross::CWnd* pParent = html.mWnd->GetParent();
    // if(pParent)
    //   pParent->PostMessage(WM_CLOSE, 0, 0);
    }
  return 0;
}

//me.value=me.datavalue("Point.firstouta.indesc" & me.datavalue("Point.firstouta.firstoutinput"))	 
int Func_DataValue(XAXA)
{
     EValueTypeScript eR, eA, eB;
     int* R = (int*)gVar.Var( html, comm.nRes , eR );
     char* A = (char*)gVar.Var( html, comm.nArg1 , eA );
     if( R && A && eA==_enumValueStr)
     {
         char temp[256];
         strcpy_s(temp, A);
         char *ref = strrchr(temp, '.');
         cross::SParamValueH val;
         if(ref)
           {
            *ref ='\0';
            SCTag *tg = ( SCTag*)gVar.Var( html, comm.nArg2, eB );
            if( tg && eB == _enumValueObjects && tg->m_pTag)
            {
            if(_strnicmp( A, "Point.", sizeof("Point.")-1 )==0)
               {           
                cross::CString strTemp=temp;
                char fname[32]="";
                strncpy_s(fname, A, sizeof("Point")-1);
                fname[sizeof("Point")-1]=0;
                const char *tag = Alias( html, tg->m_pTag, fname);
                strTemp.Replace(fname, tag); 
 //               val = ::FindExperion( html.pipe, strTemp, ref+1 );
              }
            else //val = ::FindExperion( html.pipe, temp, ref+1 );
              {
                   sprintf_s(temp, "\"%s\"", A);
                   script->ParamValue( val, temp, tg->m_pTag ); 
              }
            }
            void* pval = NULL;
            if(val.def)
            { 
              EValueType eA = enumValueUnk;
              pval = html.pipe.ParamValue_H( val );
#ifdef _DEBUG
              const char* name = html.pipe.GetMnemoName( val.nNumber );
#endif
              if(pval == NULL)
              {
                  if(tg->nEndPipeCount < 2)
                    {
                     tg->nEndPipeCount++;
                     html.m_bNeedPipeEnd = true;
                    }
              }
              else
              switch(val.def->eVal)
              {
              case enumValueStr:
                {
                char *str = html.pipe.ValueS_H(val);
                if(str)
                  {
                  GVAR& v = gVar.Obj( comm.nRes );
                  if(v.nSize >= 256)
                  {
                  if((v.nType&0x0f) != TYPE_STRING)
                    v.nType = TYPE_STRING | v.nType & 0xFFFFFFF0;
                  lstrcpy((char*)R, str);
                  }
                  return 0;
                  }
                }
                break;

               case enumValueDbl:
               case enumValueInt:
               case enumValueChr:
               case enumValueBol:
                 eA = val.def->eVal;
                break;
              }
              if(eA != enumValueUnk)
                SetTypeAndVal(comm.nRes, eA, pval, R, gVar);
             }
           }
         return 0;
     }
     return -1;
}
//
int Func_SetDataValue(XAXA)
{
     EValueTypeScript eR, eA, eB;
     int* R = (int*)gVar.Var( html, comm.nRes , eR );
     char* A = (char*)gVar.Var( html, comm.nArg1 , eA );
     int* B = (int*)gVar.Var( html, comm.nArg2 , eB );
     if( R && A && eA==_enumValueStr && eB!=enumValueUnk)
     {
         char temp[256];
         strcpy_s(temp, A);
         char *ref = strrchr(temp, '.');
         cross::SParamValueH val;
         if(ref)
         {
          *ref ='\0';
          val = ::FindExperion( html.pipe, temp, ref+1 );
          if(val.def)
          {
            void* pval = NULL;
            if(val.def)
            { 
              EValueType eA = enumValueUnk;
              pval = html.pipe.ParamValue_H( val );
#ifdef _DEBUG
              const char* name = html.pipe.GetMnemoName( val.nNumber );
#endif
              if(pval == NULL)
              {
                   html.m_bNeedPipeEnd = true;
              }
              else
              {
               SSendToModel send;
               strcpy_s( send.szValue, val.def->name );
               switch(val.def->eVal)
               {
               //case enumValueStr:
               //char *str = html.pipe.ValueS_H(val);
               //break;
               case enumValueDbl:
                 switch(eB)
                 {
               case enumValueDbl:
                 send.Set(val.def->eVal, pval, B); break;
               case enumValueInt:
                  {
                 double dd = (double)*(int*)B;
                 send.Set(val.def->eVal, pval, &dd); 
                  }
                  break;
               case enumValueChr:
               case enumValueBol:
                  {
                 double bb = (double)*(BYTE*)B;
                 send.Set(val.def->eVal, pval, &bb); 
                  }
                  break;
                 }
                 break;
               case enumValueInt:
                 switch(eB)
                 {
               case enumValueDbl:
                  {
                 int nn = (int)*(double*)B;
                 send.Set(val.def->eVal, pval, &nn); 
                  }
                  break;
               case enumValueInt:
                  send.Set(val.def->eVal, pval, B); break;
               case enumValueChr:
               case enumValueBol:
                  {
                 int nn = (int)*(BYTE*)B;
                 send.Set(val.def->eVal, pval, &nn); 
                  }
                  break;
                 }
                 break;
               case enumValueChr:
               case enumValueBol:
                 switch(eB)
                 {
               case enumValueDbl:
                  {
                 BYTE bb = (BYTE)*(double*)B;
                 send.Set(val.def->eVal, pval, &bb); 
                  }
                  break;
               case enumValueInt:                  
               case enumValueChr:
               case enumValueBol:
                  send.Set(val.def->eVal, pval, B); break;
                  break;
                 }
               break;
              }
              if(val.def->eVal > enumValueUnk && val.def->eVal <= enumValueBol) 
                html.pipe.SendDataEx( val.nNumber, send, true );
              }
             }
           }
         }
         return 0;
     }
     return -1;
}
//
int FuncExitSub(XAXA)
{
  return -3;
}

int Func_CSNG(XAXA)
{
  EValueTypeScript eR,eA;
  double* R = (double*)gVar.Var( html, comm.nRes , eR );
  char* A = (char*)gVar.Var( html, comm.nArg1, eA );

  if( R && A )
    {
    if( eA == _enumValueStr)
     {
      *R = atof(A);
      return 0;
     }
    }

  return -1;
}

int Func_LCASE(XAXA)
{
  EValueTypeScript eR,eA;
  char* R = (char*)gVar.Var( html, comm.nRes , eR );
  char* A = (char*)gVar.Var( html, comm.nArg1, eA );

  if( R && A)
  if(eA == _enumValueStr)
    {
    lstrcpy(R, A);
    _strlwr(R);
    return 0;
    }
  else
  if(eA == _enumValueObjects)       
  {   
      O_VAL oval;
      if(!GetDataFromObject(&oval, A, eA, html, gVar, script))
         return -1; 
      eA = (EValueTypeScript)oval.eType;
      if(oval.eType ==  enumValueStr) 
      {
        A = (char*)oval.ostr;
        lstrcpy(R, A);
        _strlwr(R);
        return 0;
      }
      else return -1;
  }

  return -1;
}

int Func_UCASE(XAXA)
{
  EValueTypeScript eR,eA;
  char* R = (char*)gVar.Var( html, comm.nRes , eR );
  char* A = (char*)gVar.Var( html, comm.nArg1, eA );

  if( A && eA == _enumValueStr)
    {
    lstrcpy(R, A);
    strupr(R);
    return 0;
    }

  return -1;
}

int Func_ROUND(XAXA)
{
  EValueTypeScript eR,eA;
  int* R = (int*)gVar.Var( html, comm.nRes , eR );
  double* A = (double*)gVar.Var( html, comm.nArg1, eA );

  if( A && R)
  {
    int rez = 0;
    if(eA == _enumValueDbl) 
    {

    if(eA == _enumValueDbl)
      {
      if(*A>0)
        rez = int(*A + 0.5);
      else  rez = int(*A - 0.5);
      }
    }
    else
    if(eA == _enumValueInt)
      rez = *(int*)A;
    else
    if(eA == _enumValueStr)
    {
       double tmp = atof((char*)A);
       if(tmp>0)
        rez = int(tmp + 0.5);
      else  rez = int(tmp - 0.5);
    }

    if(eR ==  _enumValueInt)
      {
      *R = rez;
      return 0;
      }
    else
    if(eR ==  _enumValueStr)
      {
      char tmp[256];
      sprintf_s(tmp, "%d", rez);
      lstrcpy((char*)R, tmp); 
      return 0;
      }
  }

  return -1;
}

int Func_EVAL(XAXA)
{
  EValueTypeScript eR,eA;
  int* R = (int*)gVar.Var( html, comm.nRes , eR );
  char* A = (char*)gVar.Var( html, comm.nArg1, eA );

  if( A && R)
  {// здесь по названию (вроде такого AlarmStateIconFactory.PriorityHHColor ) нужно присвоить цвет
    int rez = 0;
    *R = 0;
  }

  return -1;
}

int Func_REPLACE(XAXA)
  {
  EValueTypeScript eR,eA,eB,eC;
  char* R = (char*)gVar.Var( html, comm.nRes , eR );
  char* A = (char*)gVar.Var( html, comm.nArg1, eA );
  char* B = (char*)gVar.Var( html, comm.nArg2, eB );
  char* C = (char*)gVar.Var( html, comm.nArg3, eC );
  if ( R == NULL || A == NULL || B == NULL || C == NULL ) return -1;
  if ( eR != enumValueStr ) return -1;
  if ( eA != enumValueStr ) return -1;
  if ( eB != enumValueStr ) return -1;
  if ( eC != enumValueStr ) return -1;

  cross::CString as(A);
  as.Replace(B, C);
  lstrcpy(R, as);
  return 0;
}

int Func_STRCOMP(XAXA)
  {
  EValueTypeScript eR,eA,eB,eC;
  int* R = (int*)gVar.Var( html, comm.nRes , eR );
  char* A = (char*)gVar.Var( html, comm.nArg1, eA );
  char* B = (char*)gVar.Var( html, comm.nArg2, eB );
  int* C = (int*)gVar.Var( html, comm.nArg3, eC );
  if ( R == NULL || A == NULL || B == NULL || C == NULL ) return -1;
  if ( eR != enumValueInt ) return -1;
  if ( eA != enumValueStr ) return -1;
  if ( eB != enumValueStr ) return -1;
  if ( eB != enumValueInt ) return -1;

  if(*C==0)
  {
     *R = _stricmp(A, B);
  }
  else
  {
     *R = strcmp(A, B);
  }

  return 0;
}

int Func_MR_Comment(XAXA)
  {
  EValueTypeScript eR,eA;
  int* R = (int*)gVar.Var( html, comm.nRes , eR );
  char* A = (char*)gVar.Var( html, comm.nArg1, eA );
  if ( R == NULL || A == NULL ) return -1;
  if ( eR != enumValueInt ) return -1;
  if ( eA != enumValueStr ) return -1;

#ifdef _DEBUG
  if(stricmp(A, "LPumpRoomLEDs") == 0)
     AAA();
  if(stricmp(A, "DockState") == 0)
     AAA();
#endif

  return 0;
}


int Func_page_GetStyleClassProperty(XAXA)
{
  EValueTypeScript eR,eA,eB;
  char* R = (char*)gVar.Var( html, comm.nRes , eR );
  char* A = (char*)gVar.Var( html, comm.nArg1, eA );
  char* B = (char*)gVar.Var( html, comm.nArg2, eB );

  if(R && A && B && eR == _enumValueStr &&  eA == _enumValueStr && eB == _enumValueStr)    
  {
      lstrcpy(R, CStyle::FindCSSSyileParam( A, B).c_str());
      return 0;
  }

  return -1;
}


int Func_FindPopup(XAXA)
{
  EValueTypeScript eR,eA;
  SCTag *pPropR = (SCTag*)gVar.Var( html, comm.nRes , eR );
  SCTag *pPropA = (SCTag*)gVar.Var( html, comm.nArg1, eA );

  if(pPropR && pPropA  && eR ==  _enumValueObjects &&  eA ==  _enumValueObjects)    
  {    
      //GetAppWnd().FindMnemoByTitle( pPropA );
      return 0;
  }

  return -1;
}

int Func_IS(XAXA)
{
  EValueTypeScript eR,eA,eB;
  bool* R = (bool*)gVar.Var( html, comm.nRes , eR );
  void* A = gVar.Var( html, comm.nArg1, eA );
  void* B = gVar.Var( html, comm.nArg2, eB );
  if ( R == NULL || A == NULL || B == NULL ) 
      return -1;

  const char* b = gVar.Name( comm.nArg2 );
  if(eA == _enumValueObjects && eR == _enumValueBol && strcmp(b, "CONST_NOTHING")==0)
  {
     SCTag *pPropA = (SCTag *)A;
     char *pparam = NULL;
     bool bb = false;
     CTag *pTagA = HtmlGetObject(html, script->m_vTags, pPropA, pparam);
     if(pTagA)
     {//есть такой               
          *R = 0;
          return 0;
     }
     else
     {//нет              
          *R = 1;
          return 0;
     }

  }

  return -1;
}

int Func_page_stylesheetSrc(XAXA)
{

  EValueTypeScript eR;
  char* R = (char*)gVar.Var( html, comm.nRes , eR );

  if ( R == NULL || eR !=  _enumValueStr)
      return -1;

  lstrcpy(R, "не пробел");

  return 0;
}

int Func_DisplayDataRepositoryGetValue(XAXA)
{

  EValueTypeScript eR,eA;
  char* R = (char*)gVar.Var( html, comm.nRes , eR );
  char* A = (char*)gVar.Var( html, comm.nArg1, eA );

  if ( R == NULL || eR !=  _enumValueStr)
      return -1;
  if ( A == NULL || eA !=  _enumValueStr)
      return -1;

  lstrcpy(R, html.FindInDisplayDataRepository(string_to_upper(A)).c_str());

  return 0;
}

int Func_DisplayDataRepositoryPutValue(XAXA)
{

  EValueTypeScript eR,eA,eB;
  bool* R = (bool*)gVar.Var( html, comm.nRes , eR );
  char* A = (char*)gVar.Var( html, comm.nArg1, eA );
  char* B = (char*)gVar.Var( html, comm.nArg1, eB );

  if(eA !=  _enumValueStr || eB !=  _enumValueStr)
    return -1;
  if( R && A && B)
    html.DisplayDataRepositorySetValue(A, B);

  return 0;
}


int Func_GetAttribute(XAXA)
{
  EValueTypeScript eR,eA1,eA2;
  char* R = (char*)gVar.Var( html, comm.nRes , eR );
  SCTag* A1 = (SCTag*)gVar.Var( html, comm.nArg1, eA1 );
  char* A2 = (char*)gVar.Var( html, comm.nArg2, eA2 );
  if( R==NULL || A1==NULL || A2==NULL)
     return -1;
  if(eR == _enumValueStr && eA1 == _enumValueTag && eA2 == _enumValueStr)
  {
      if(A1->m_pTag)
      {
         std::map<std::string, std::string>::iterator it = A1->m_pTag->m_Attributes.find(A2);
         if(it!=A1->m_pTag->m_Attributes.end())
         {
             lstrcpy(R, it->second.c_str());
         }
         else *R= '\0';
      } 
  }
  return 0;
}

int Func_SetAttribute(XAXA)
{
  EValueTypeScript eR,eA1,eA2,eA3;
  char* R = (char*)gVar.Var( html, comm.nRes , eR );
  SCTag* A1 = (SCTag*)gVar.Var( html, comm.nArg1, eA1 );
  char* A2 = (char*)gVar.Var( html, comm.nArg2, eA2 );
  char* A3 = (char*)gVar.Var( html, comm.nArg3, eA3 );
  if( R==NULL || A1==NULL || A2==NULL || A3==NULL )
     return -1;
  if(eA1 == _enumValueTag && eA2 == enumValueStr && eA3 == enumValueStr)
    {
      if(A1->m_pTag)
       {
         A1->m_pTag->m_Attributes[A2] = A3;
       }
    }
  return 0;
}

int Func_SecurityLevel(XAXA)
  {
  EValueTypeScript eR;
  int* R = (int*)gVar.Var( html, comm.nRes , eR );
  if ( R == NULL) return -1;
  if( eR != enumValueInt) return-1;
  
   switch(Version)
   {
  case T1163_ONPZ:
  case T1165P_ONPZ:
    *R = 4;
    break;
  default: *R = 5;
    break;
   }
  //
  return 0;
  }
