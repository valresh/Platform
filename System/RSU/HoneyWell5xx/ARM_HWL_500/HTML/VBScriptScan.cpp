#include "StdAfx.h"
#include "HTML.h"
#include "VBScript.h"
#include "TagBODY.h"
#include "Profile.h"

#include "Style.h"
#include "TagBODY.h"
#include <map>



bool IsComm( char*& ptr, SUniToken& uni );
bool IsStrV( char*& ptr, SUniToken& uni );
bool IsFltV( char*& ptr, SUniToken& uni );
bool IsIntV( char*& ptr, SUniToken& uni );
bool IsStdF( char*& ptr, SUniToken& uni );
bool IsVarV( char*& ptr, SUniToken& uni );
bool IsStdW( char*& ptr, SUniToken& uni, CTag* pThis, CHTML& html );
bool IsAutoVarV( char*& ptr, SUniToken& uni );
bool IsExit( char*& ptr, SUniToken& uni );
//
static SLocalStd stdK[] =
  {
  { "if"    , KEY_IF     },
  { "then"  , KEY_THEN   },
  { "then:" , KEY_THEN   },
  { "else"  , KEY_ELSE   },
  { "else:" , KEY_ELSE   },
  { "elseif", KEY_ELSEIF },
  { "end"   , KEY_END    },
  { "dim"   , KEY_DIM    },
  { "sub"   , KEY_SUB    },
  { "call"  , KEY_CALL   },
  { "for"   , KEY_FOR    },
  { "to"    , KEY_TO     },
  { "next"  , KEY_NEXT   },
  { "exit"  , KEY_EXIT   },
  { "exitsub", KEY_EXITSUB },
  //
  { "or"    , OPER_OR  },
  { "and"   , OPER_AND  },
  { "mod"   , OPER_MOD },
  // Константы
  { "false"   , CONST_FALSE   , 0          },
  { "true"    , CONST_TRUE    , 1          },
  { "hidden"  , CONST_HIDDEN  , enumHidden },
  { "visible" , CONST_VISIBLE , enumVisible},
  // Цвета
  { "vbBlack"  , CONST_VBBLACK  , 0x000000 },
  { "vbRed"    , CONST_VBRED    , 0x0000ff },
  { "vbGreen"  , CONST_VBGREEN  , 0x00ff00 },
  { "vbYellow" , CONST_VBYELLOW , 0x00ffff },
  { "vbBlue"   , CONST_VBBLUE   , 0xff0000 },
  { "vbMagenta", CONST_VBMAGENTA, 0xff00ff },
  { "vbCyan"   , CONST_VBCYAN   , 0xffff00 },
  { "vbWhite"  , CONST_VBWHITE  , 0xffffff },
  { "transparent", CONST_TRANSPARENT, 0xff000000 },
  // Кнопки у MessageBox-а
  { "vbOKOnly"          , CONST_VBOKONLY          , MB_OK               },
  { "vbOKCancel"        , CONST_VBOKCANCEL        , MB_OKCANCEL         },
  { "vbAbortRetryIgnore", CONST_VBABORTRETRYIGNORE, MB_ABORTRETRYIGNORE },
  { "vbYesNoCancel"     , CONST_VBYESNOCANCEL     , MB_YESNOCANCEL      },
  { "vbYesNo"           , CONST_VBYESNO           , MB_YESNO            },
  { "vbRetryCancel"     , CONST_VBRETRYCANCEL     , MB_RETRYCANCEL      },
  { "vbCritical"        , CONST_VBCRITICAL        , MB_ICONSTOP         },
  { "vbQuestion"        , CONST_VBQUESTION        , MB_ICONQUESTION     },
  { "vbExclamation"     , CONST_VBEXCLAMATION     , MB_ICONEXCLAMATION  },
  { "vbInformation"     , CONST_VBINFORMATION     , MB_ICONINFORMATION  },
  { "vbDefaultButton1"  , CONST_VBDEFAULTBUTTON1  , MB_DEFBUTTON1       },
  { "vbDefaultButton2"  , CONST_VBDEFAULTBUTTON2  , MB_DEFBUTTON2       },
  { "vbDefaultButton3"  , CONST_VBDEFAULTBUTTON3  , MB_DEFBUTTON3       },
  { "vbDefaultButton4"  , CONST_VBDEFAULTBUTTON4  , MB_DEFBUTTON4       },
  { "vbApplicationModal", CONST_VBAPPLICATIONMODAL, MB_APPLMODAL        },
  { "vbSystemModal"     , CONST_VBSYSTEMMODAL     , MB_SYSTEMMODAL      },
  { "vbTaskModal"       , CONST_VBTASKMODAL       , MB_TASKMODAL        },
  // Возвращаемые значения MessageBox-а
  { "vbOK"    , CONST_VBOK    , IDOK     },
  { "vbCancel", CONST_VBCANCEL, IDCANCEL },
  { "vbAbort" , CONST_VBABORT , IDABORT  },
  { "vbRetry" , CONST_VBRETRY , IDRETRY  },
  { "vbIgnore", CONST_VBIGNORE, IDIGNORE },
  { "vbYes"   , CONST_VBYES   , IDYES    },
  { "vbNo"    , CONST_VBNO    , IDNO     },
  { "vbCr"    , CONST_VBCR    , 0xD     },
  { "vbLf"    , CONST_VBLF    , 0xA     },
  { "vbCrLf"  , CONST_VBCRLF  , 0x0A0D   },
  //
  //{ "gbPageComplete"    , CONST_TRUE    , 1          },
  //
  { "bad"    , CONST_BAD     , 0 },// Надо уточнить
  //
  { "rgb("   , CONST_RGB  , 0xffffff },
  { "err.number", CONST_ERR_NUMBER, 0 },
  { "err.description", CONST_ERR_DESCRIPTION, 0 },
  { "window.external.environment", CONST_WINDOW_EXTERNAL_ENVIRONMENT, 0 },
  { "window.external.Application.environment", CONST_WINDOW_EXTERNAL_ENVIRONMENT, 0 },
  { "window.external.Application.currentpage", CONST_WINDOW_APPLICATION_CURRENT_PAGE, 0 },
  { "window.external.Parent.RuntimeStatus.OperatorID", CONST_WINDOW_EXTERNAL_OPERATOR_ID, 0 },
  { "nothing", CONST_NOTHING, 0},
  { "spDebug", CONST_SPDEBUG, 0},
  //
  {NULL,0},
  };
//
static SLocalStd stdO[] =
  {
  { "-"     , OPER_SUB     },
  { "+"     , OPER_ADD     },
  { "/"     , OPER_DIV     },
  { "\\"    , OPER_DIV_INT },
  { "="     , OPER_EQ      },
  { ">="    , OPER_BEQ     },
  { "<="    , OPER_MEQ     },
  { "<>"    , OPER_NEQ     },
  { "&"     , OPER_AND     },
  { "*"     , OPER_MUL     },
  { "<"     , OPER_MEN     },
  { ">"     , OPER_BOL     },
  { "not",    OPER_NOT     },
  { "is",     OPER_IS      },
  //
  {NULL,0},
  };
//
static SLocalStd stdF[] =
  {
  { "MsgBox"      , FUNC_MSGBOX , 0   },
  { "abs"         , FUNC_ABS    , 0  },
  { "cdbl"        , FUNC_CDBL   , 0  },
  { "cint"        , FUNC_CINT   , 0   },
  { "cstr"        , FUNC_CSTR   , 0   },
  { "isnumeric"   , FUNC_ISNUMERIC , 0 },
  { "isnan"       , FUNC_ISNAN  , 0 },
  { "chr"         , FUNC_CHR    , 0   },
  { "csng"        , FUNC_CSNG   , 0   },
  { "lcase"       , FUNC_LCASE  , 0   },
  { "ucase"       , FUNC_UCASE  , 0   },
  { "round"       , FUNC_ROUND  , 0   },
  { "eval"        , FUNC_EVAL   , 0   },
  { "formatnumber", FUNC_FORM_N , 0   },
  { "trim"        , FUNC_TRIM   , 0   },
  { "ltrim"       , FUNC_LTRIM  , 0   },
  { "rtrim"       , FUNC_RTRIM  , 0   },
  { "instr"       , FUNC_INSTR  , 0   },
  { "mid"         , FUNC_MID    , 0   },
  { "left"        , FUNC_LEFT   , 0   },
  { "right"       , FUNC_RIGHT  , 0   },
  { "replace"     , FUNC_REPLACE, 0   },
  { "strcomp"     , FUNC_STRCOMP, 0   },
  { "len"         , FUNC_LEN,     0   },
  { "("           , FUNC_BRACE  , 0   },
  { "window.external.ParentDocument.ParentWindow.external.InvokePopup", FUNC_CREATEPOPUOWINDOW , 1}, //1 = без скобок
  { "HDXPopupBehavior.CreatePopupWindow2", FUNC_CREATEPOPUOWINDOW , 1}, //1 = без скобок
  { "HDXPopupBehavior.CreatePopupWindow", FUNC_CREATEPOPUOWINDOW , 1}, //1 = без скобок
  { "window.external.Parent.MessageBox",  FUNC_MSGBOX  , 0  },
  { "window.external.Parent.RuntimeStatus.SecurityLevel", FUNC_SecurityLevel, 2}, //2- без параметров
  { "window.external.LogMessage", FUNC_LOGMESSAGE  , 1}, //1 = без скобок
  { "window.external.Application.LogMessage", FUNC_LOGMESSAGE  , 1}, //1 = без скобок
  { "typename"    , FUNC_TYPENAME  },
  { "vartype"     , FUNC_VARTYPE  },
  { "HDXPopupBehavior.CloseAllWindows", FUNC_CloseAllWindows, 2}, //без параметров
  { "page.GetStyleClassProperty" , FUNC_page_GetStyleClassProperty, 0   },    
  { "oShape.GetStyleClassProperty", FUNC_page_GetStyleClassProperty, 0  },
  { "HDXPopupBehavior.FindPopup",   FUNC_FindPopup, 0  },  
  { "page.stylesheetSrc",  FUNC_page_stylesheetSrc, 2  },
  { "DisplayDataRepository.GetValue", FUNC_DisplayDataRepositoryGetValue, 0},
  { "DisplayDataRepository.PutValue", FUNC_DisplayDataRepositoryPutValue, 1},
  { "MR_Comment", FUNC_MR_Comment, 0},  //искуственно - чтобы в отладке знать какая подпрограмма работает
  { "page.getattribute", FUNC_PAGE_GETATTRIBUTE, 0},
  { "page.setattribute", FUNC_PAGE_SETATTRIBUTE, 1},
  { "isnull", FUNC_ISNULL, 0},
  { "AlarmStateIconfactory", FUNC_AlarmStateIconfactory, 1},
  //
  {NULL,0},
  };
//
SLocalStd stdV[] =
  {
  //
  // Имеют параметры
  { "qualitybad"    , PARAM_QUALITYBAD },
  { "QualityGood"   , PARAM_QUALITYGOOD },
  { "datavalue"     , PARAM_DATAVALUE  },
  { "datachanged"   , PARAM_DATACHANGED  },
  { "isnan"         , PARAM_ISNAN  },
  { "AlarmSeverity" , PARAM_ALARMSEVERITY  },
  { "dataalarm"     , PARAM_DATAALARM  },
  { "dataquality"   , PARAM_DATAQUALITY  },
  { "UnacknowledgedAlarm", PARAM_UnacknowledgedAlarm  },
  { "DataExists"    , PARAM_DATAEXIST },
  // Без параметров
  { "textColorBlink"   , VALUE_TEXTCOLORBLINK   },
  { "textColor"        , VALUE_TEXTCOLOR        },
  { "style.visibility" , VALUE_STYLE_VISIBILITY },
  { "value"            , VALUE_VALUE            },
  { "visiblerangeHi"   , VALUE_VISIBLERANGEHI   },
  { "visiblerangeLo"   , VALUE_VISIBLERANGELO   },
  { "levelFillColor"   , VALUE_LEVELFILLCOLOR   },
  { "Rotation"         , VALUE_ROTATION         },
  { "fillColorBlink"   , VALUE_FILLCOLORBLINK   },
  { "fillColor"        , VALUE_FILLCOLOR        },
  { "hostpopulation"   , VALUE_HOSTPOPULATION   },
  { "lineColorBlink"   , VALUE_LINECOLORBLINK   },
  { "lineColor"        , VALUE_LINECOLOR        },
  { "gradientfillcolor", VALUE_GRADIENTFILLCOLOR},
  { "LineStyle"        , VALUE_LINESTYLE        }, 
  { "LineWidth"        , VALUE_LINEWIDTH        }, 
  { "style.pixelHeight", VALUE_STYLE_PIXELHEIGHT},
  { "style.pixelWidth" , VALUE_STYLE_PIXELWIDTH },
  { "style.pixelLeft",   VALUE_STYLE_PIXELLEFT  },
  { "style.fontWeight" , VALUE_STYLE_FONTWEIGHT },
  { "innerText"        , VALUE_INNERTEXT        },
  { "numericdisplayformat", VALUE_NUMERICDISPLAYFORMAT},
  { "styleclass"       , VALUE_STYLECLASS },
  { "alarm"            , VALUE_ALARM   },
  { "CurrentValue"     , VALUE_VALUE            },
  { "cancelchange"  , VALUE_CANCELCHANGE },
  { "title"         , VALUE_TITLE },
  { "objects"       , VALUE_OBJECT },
  { "rangeOrigin"   , VALUE_VISIBLERANGEORIGIN   },
  { "StartArrow"    , VALUE_STRARTARROW   },
  { "EndArrow"      , VALUE_ENDARROW   },
  { "src"           , VALUE_SRC   },
  { "shelved"       , VALUE_SHELVED },
  { "RTN"           , VALUE_RTN   },
  { "acknowledged"  , VALUE_ACKED   },
  { "priority"      , VALUE_PRIORITY  },
  {NULL,0},
  };
//
static SLocalStd stdW[] =
  {
  //
  // Имеют параметры
//{ "external.dictionary.item", PARAM_QUALITYBAD },
  { "external.MessageZoneText", WINDOW_EXT_TMZONE_T },
//{ "external.LogMessage"     , PARAM_DATAVALUE  },
  // Без параметров
  { "external.close"                    , WINDOW_EXT_CLOSE    },
  { "external.currentpage"              , WINDOW_EXT_CURRPAGE },
  { "external.SecurityLevel"            , WINDOW_EXT_S_LEVEL  },
  { "external.TemporaryMessageZoneText" , WINDOW_EXT_TMZONE_T },
  { "event"                             , WINDOW_EVENT },   
  {NULL,0},
  };
// Временно
#define _MAX_TEMP 1024
static int curT = 0;
static SLocalStd stdT[_MAX_TEMP] =
  {
  { "x", 0, 0, 1 },
  { "a", 0, 0, 1  },
  { "cp_int", VAR_INTEGER, 0, sizeof("cp_int")-1  },
  { "Er", VAR_INTEGER, 0, sizeof("Er")-1  },
  { "Op", VAR_INTEGER, 0, sizeof("Op")-1  },
  { "Cl", VAR_INTEGER, 0, sizeof("Cl")-1  },
  { "N", VAR_INTEGER, 0, sizeof("N")-1  },
  { "ConAlpResetval.value", VAR_INTEGER, 0, sizeof("ConAlpResetval.value")-1  }, //не понятно зачем эти переменные:
  { "flagreset.value", VAR_INTEGER, 0, sizeof("flagreset.value")-1  },           //в скриптах пересылаются
  { "resetval.value", VAR_INTEGER, 0, sizeof("resetval.value")-1  },             //одна в другую
  };
//
inline void Inits( SLocalStd* lst )
  {
  int n = 0;
  //while ( stdK[n].name ) lst[n].dl = lstrlen(lst[n].name),n++;
  while ( lst[n].name ) 
     lst[n].dl = lstrlen(lst[n].name),n++;
  }
//
void Inits()
  {
  _static bool bInit = false;
  curT = 3;  //число предварительно заполненых элементов stdT
  if(Version==DOTF_PM) 
    curT = 10;
  ::memset(&stdT[curT], 0, sizeof(stdT[0]) * (_countof(stdT) - curT));
  if ( bInit ) return;
  bInit = true;
  Inits( stdK );
  Inits( stdO );
  Inits( stdF );
  Inits( stdV );
  Inits( stdW );
  }
//
SLocalStd* FindVar(char *name)
{
    for(int ii=0; ii<curT; ii++)
     {
       if(_stricmp(stdT[ii].name, name)==0)
         return &stdT[ii];
     }
    return NULL;
}
//
void AddEndIfInSinglelineIf(char *ptr)
{
	// обрабатываем все однострочные версии условий if..then вставляя в конец условия end if
	char *str, *orig;//, *res;
	//res = new char[strlen(ptr)*2];
#define MAX_SCRIPT_LEN 8192
	if (strlen(ptr) > (MAX_SCRIPT_LEN-MAX_SCRIPT_LEN/4))
    {
		return;
    }
	char res[MAX_SCRIPT_LEN];
	str = res;
	orig = ptr;
	while(*ptr)
	{
		if (!strnicmp(ptr, "then", 4))
		{
			bool isinline = false;
			for(int i = 0; i< 4; i++){ *str = *ptr; str++; ptr++; }
			while(*ptr)
			{
				if (*ptr == ' ' || *ptr == '\t') {}
				else if (*ptr == '\r' || *ptr == '\n')
				{                       
					if (isinline) 
					{
						*str = ' '; str++;
						*str = 'e'; str++;
						*str = 'n'; str++;
						*str = 'd'; str++;
						*str = ' '; str++;
						*str = 'i'; str++;
						*str = 'f'; str++;
						isinline = false;
					}
					break;
				}
				else if (!strnicmp(ptr, "end if", 6)) break;
				else isinline = true;
				*str = *ptr; str++; ptr++;
			}
		}
		else { *str = *ptr; str++; ptr++; }
	}
	*str = 0; 
    if(strcmp(orig, res)!=0)
	strcpy(orig, res);
}

bool CVBScript::VBScriptScan( CHTML& html, SUniToken& uni, CTag* pThis, char* ptr, bool bInit )
  {
  if ( bInit ) Inits();
  SkipSpace( ptr );
  // Убираем комментарии
  char* p = ptr;
  while ( *p )
  {
    SkipSpace( p );
    if ( *p == '\'' )
      {
      char* s = strchr( p, '\xA' );
      if ( s == NULL )
		{ *p = 0; break; }
      else
        while ( p != s ) *p++ = ' ';
      }
    if(*p)
      p++;
  }
  AddEndIfInSinglelineIf(ptr);
  // Для упрощения, все переводы строк заменяем на пробелы
  //p = ptr; while ( *p ) { if ( BYTE(*p) < 0x20 ) *p = 0x20; p++;}
  //
  while ( *ptr )
    {    
    while(*ptr == '\xD' || *ptr == '\xA')
       ptr++;
    if(ptr[0]=='\x0')
      break;
    SkipSpace( ptr );
    if ( ptr[0] == '_' && (ptr[1] == 0xD || ptr[1] == 0xA || ptr[1] == ' ') )
      { 
        SkipSpace( ++ptr );
      }
    if ( *ptr == ':' )
    {
       *ptr = 0;
       SkipSpace( ++ptr );
    }
    while(*ptr == '\t')
       SkipSpace( ++ptr );
    while(*ptr == '\xD' || *ptr == '\xA')
       ptr++;
    SkipSpace( ptr );

#ifdef _DEBUG
   UINT nCount = uni.Count();
#endif 

    if ( *ptr )
    if ( !IsComm( ptr, uni ) )
    if ( !IsStrV( ptr, uni ) )
    if ( !IsIntV( ptr, uni ) )
    if ( !IsFltV( ptr, uni ) )
    if ( !IsStdK( ptr, uni ) )
    if ( !IsStdF( ptr, uni ) )
    if ( !IsVarV( ptr, uni ) )
    if ( !IsStdW( ptr, uni, pThis, html ) )
    if ( !IsStdV( ptr, uni, pThis, html ) )
      {
      _static char* on = "on error resume next";
      _static int   dl = lstrlen(on);
      if ( _strnicmp( ptr, on, dl ) == 0 )
        ptr += dl;
	  else if ( _strnicmp( ptr, "CancelClick", 11 ) == 0 ) ptr += 11;
      else
      if (_strnicmp( ptr, "set ", sizeof("set ")-1) == 0 ) 
         ptr += sizeof("set ")-1;
      else
      if((strnicmp( ptr, "me", _countofchar("me"))==0) && IsSpace(*(ptr+_countofchar("me"))) )
        {
        SToken& token = uni.Add();
        token.txt = ptr;
        token.dw = VALUE_TAG; 
        token.mTag = pThis;
        ptr+=_countofchar("me");
        *ptr++=0;
        }
      else
      if( strncmp(ptr, "VBScript:", _countof("VBScript:")-1) == 0)
      { 
          ptr += _countof("VBScript:")-1;
      }
      else
      if( strncmp(ptr, "window.external.", _countof("window.external.")-1) == 0)
      { 
          char* pp = strchr(ptr, '\xA');
          if(pp)
            ptr = pp;
          else  return false;
      }
      else
      if ( !IsDisplayDataRepository( ptr, uni, pThis, html ) )
      if ( !IsAutoVarV( ptr, uni ) )
      if ( !IsSubroutine( ptr, uni, pThis, html ) )
        {
        ASSUNO
#ifdef _DEBUG
       char buf[64];
       sprintf_s(buf, "ошибка скрипта: %-40.40s\n", ptr); 
       OutputDebugString(buf);
#endif
        return false;
        }
      }
    }
  return true;
  }
//
bool EndBrace( char*& ptr )
  {
  char* a = MyBraces( ptr, ')' );
  if ( AS(a != NULL) ) return false;
  //
  *a++ = 0;
  ptr = a;
  return true;
  }
//
bool AddDim( char*& ptr, SUniToken& uni )
  {
  //
  while ( 1 )
    {
    ASSERT( curT < _MAX_TEMP );
    if( curT >= (_MAX_TEMP-1))
      return false;
    SkipSpace( ptr );
    SToken& token = uni.Add();
    token.txt = ptr;
    token.dw  = KEY_DIM;
    while ( IsV( *ptr ) ) ptr++;
    //
    char* p = ptr;
    SkipSpace( ptr );
    bool b = *ptr == ',';
    if(*ptr == '(')
    {
      stdT[curT].var = KEY_ARRAY;
      char* pp = strchr(p, ')');
      if(pp==NULL)
         return false; 
      token.dw  = KEY_ARRAY;
      token.nVar = atoi(ptr+1);
      ptr = pp + 1;
      SkipSpace( ptr );
      b = *ptr == ',';
    }
    else stdT[curT].var = 0;
    *p = 0;
    stdT[curT].name = token.txt;
    stdT[curT].dl = lstrlen(stdT[curT].name);
    stdT[curT].dw = 0;    
    curT++;
    //
    if ( b )
      ptr++;
    else break;      
    }
  return true;
  }
//
bool CVBScript::IsStdK( char*& ptr, SUniToken& uni )
  {
  int n = 0;
  while ( stdK[n].name )
    {
    if ( _strnicmp( ptr, stdK[n].name, stdK[n].dl ) == 0 )
      {
      char* p = ptr+stdK[n].dl;
      if ( IsSpace(*p) || *p == 0 || ((*p == '=' || *p == '<' || *p == '+') && (_CONST_BEGIN <= stdK[n].dw && stdK[n].dw <= _CONST_END)) 
           || (*p == '(' && (stdK[n].dw == KEY_IF || _OPER_BEGIN <= stdK[n].dw && stdK[n].dw <= _OPER_END) ) 
         )
        {
        if ( stdK[n].dw == KEY_DIM )
          {
          ptr = p;
          if(!AddDim( ptr, uni ))
             return false;
          }
        else
        if ( stdK[n].dw == KEY_FOR )
          {
          if (Version==LG35_8_KF)
            return false; //не проверял
          // "for i = ConAlpnumberofinput.value+1 to intMaxInput"
          SkipSpace(p);
          char *pp = p;
          while(IsV(*p)) p++;
          char ch = *p;
          *p='\0'; 
          if(FindVar(pp)==NULL)
            {
            *p= ch;
            if ( !IsAutoVarV( pp, uni ) ) //добавил dim для i если его нет 
               return false;
            *p= '\0';
            }
          if(SLocalStd* ppi = FindVar(pp))
           {
            ppi->dw = 0; //переменная а не константа VAR_INTEGER;
            lastForParam = pp;
            *p= ch;
            //временная пременная для номера команды перехода за NEXT
            SToken& tokenJMP = uni.Add();
            char tempstr[256];
            sprintf_s(tempstr, "$$JMP%d$$", ++m_nTempCount);
            m_TempStrings.push_back(tempstr);
            tokenJMP.txt = (char*)m_TempStrings.at(m_TempStrings.size()-1).c_str();
            tokenJMP.dw = KEY_DIM;
            if( curT >= (_MAX_TEMP-1))
              return false;
            stdT[curT].name = tokenJMP.txt;
            stdT[curT].dl = lstrlen(stdT[curT].name);
            stdT[curT].dw = VAR_INTEGER;
            curT++;

            SToken& token = uni.Add();
            token.txt = stdK[n].name;
            token.dw  = stdK[n].dw;

            ptr = pp;
            return true;
           }
          return false;
          }
        else
        if ( stdK[n].dw == KEY_TO )
          {
          // "for i = ConAlpnumberofinput.value+1 to intMaxInput"
          if (Version==LG35_8_KF)
             return false; //не проверял
          if(lastForParam.size()==0)
             return false;
          SkipSpace(p);
          SToken& tokenTO = uni.Add();
          tokenTO.dw  = stdK[n].dw;
          char *pxA = strchr(p, '\xA');
          char *pxD = strchr(p, '\xD');
          if(pxA==NULL && pxD==NULL)
           ptr = p+strlen(p);
          else
          if(pxA==NULL)
            ptr = pxD;
          else      
          if(pxD==NULL)
            ptr = pxA;
          else ptr = min(pxA, pxD);
          *ptr++ = '\0';
          char tempstr[256];
          SkipSpace(p);
          int nTo = atoi(p);
          while(isdigit(*p)) p++;
          while(*p==' ') p++;
          if(_strnicmp(p, "step", _countof("step")-1) != 0)
            sprintf_s(tempstr, "$$JMP%d$$, %s, %d", m_nTempCount, lastForParam.c_str(), nTo);
          else
            sprintf_s(tempstr, "$$JMP%d$$, %s, %d, %s ", m_nTempCount, lastForParam.c_str(), nTo, p + _countof("step")-1);
          m_TempStrings.push_back(tempstr);
          tokenTO.txt = (char*)m_TempStrings.at(m_TempStrings.size()-1).c_str();
          lastForParam.clear();
          return true;
          }
        else if( stdK[n].dw == KEY_EXIT )
          {
          ptr = p;
          return IsExit( ptr, uni );
          }
        else
        if ( stdK[n].dw == KEY_CALL )
        {  //чтоб не мусорить
             ptr = p;
             return true;
        }
        else
        if(CONST_VBCR == stdK[n].dw)
        {
          SToken& token = uni.Add();
          token.txt = "\xD";
          token.dw  = VAR_STRING;
          ptr = p;
          return true;
        }       
        else
        if(CONST_VBLF == stdK[n].dw)
        {
          SToken& token = uni.Add();
          token.txt = "\xA";
          token.dw  = VAR_STRING;
          ptr = p;
          return true;
        }       
        else
        if(CONST_VBCRLF == stdK[n].dw)
        {
          SToken& token = uni.Add();
          token.txt = "\xD\xA";
          token.dw  = VAR_STRING;
          ptr = p;
          return true;
        }       
        else
          {
          SToken& token = uni.Add();
          token.txt = stdK[n].name;
          token.dw  = stdK[n].dw;
          if ( _CONST_BEGIN <= stdK[n].dw && stdK[n].dw <= _CONST_END )
            token.nVar = stdK[n].var;
          else
          if ( stdK[n].dw == KEY_THEN )
            {
            while ( *p == ' ' ) p++;
            if ( !(*p == '\r' || *p == '\n') )
            token.dw = KEY_THENIF;
            }
          ptr = p;
          }
        return true;
        }
      else
      if ( CONST_RGB == stdK[n].dw)
       {
        byte rr = atoi(p);
        char *pp = strchr(p,',');
        byte gg = 0;
        byte bb = 0;
        if(pp)
          {
            gg = atoi(++pp);
            pp = strchr(pp,',');
            if (pp)
             {
              bb = atoi(++pp);
              pp = strchr(pp, ')');
              if (pp)
                {
                 SToken& token = uni.Add();
                 token.txt = ptr;
                 token.dw  = stdK[n].dw;
                 token.nVar = RGB(rr, gg, bb);
                 *pp = 0;
                 ptr = pp+1;
                 return true;
                }
             }
          }
       }
      }
    n++;
    }
  //
  if ( *ptr == '-' || *ptr == '+' )
  if ( '0' <=ptr[1] && ptr[1] <= '9' )
    {
    int n = uni.Count();
    if ( n > 0 )
      {
      SToken& t = uni.Obj(n-1);
      if ( t.dw == OPER_EQ )
        {
        ptr++;
        if ( IsIntV( ptr, uni ) )
          {
          SToken& p = uni.Obj(n);
          p.txt--;
          return true;
          }
        else
        if ( IsFltV( ptr, uni ) )
          {
          SToken& p = uni.Obj(n);
          p.txt--;
          return true;
          }
        ptr--;
        }
      }
    }
  //
  n = 0;
  while ( stdO[n].name )
    {
    if ( _strnicmp( ptr, stdO[n].name, stdO[n].dl ) == 0 )
      {
      if ( _strnicmp( "is", stdO[n].name, stdO[n].dl ) == 0)
         if( ptr[stdO[n].dl] != ' ') 
         {  n++; continue; };
      char* p = ptr+stdO[n].dl;
      SToken& token = uni.Add();
      token.txt = stdO[n].name;
      token.dw  = stdO[n].dw;
      *ptr = 0;
      ptr = p;
      return true;
      }
    n++;
    }
  return false;
  }
//
bool IsVarV( char*& ptr, SUniToken& uni )
  {
  //
  for ( int n = 0; n < curT; n++ )
    {
    if ( _strnicmp( ptr, stdT[n].name, stdT[n].dl ) == 0 )
      {
      char* p = ptr+stdT[n].dl;
      if ( IsSpace(*p) || *p == '=' || *p == 0 || *p == '+' || *p == '-' || *p == '<' || *p == '>' )
        {
        SToken& token = uni.Add();
        token.txt = stdT[n].name;
        token.dw  = stdT[n].dw;
        if(*p==' ')
          *p++=0;
        ptr = p;
        return true;
        }
      else
      if(*p == '(')
      {
          if(stdT[n].var != KEY_ARRAY)
            return false;
          char* pp = strchr(p, ')');
          if(pp==NULL)
            return false;  
          *pp = 0;
          SToken& token = uni.Add();
          token.txt = ptr;
          token.dw  = KEY_ARRAYELEMENT;
          ptr = pp + 1;
          return true;
      }
      else
        {
         if(stdT[n].dw==0)
           {
           if(_strnicmp(p ,".getcustomproperty", _countof(".getcustomproperty")-1) == 0)
             {
               p +=_countofchar(".getcustomproperty");
               SkipSpace( p );
               if ( *p == '(' )
                {       
                 // Начало аргумента
                 SkipSpace( ++p );
                 SToken& token = uni.Add();
                 token.txt = ptr;
                 token.dw  = FUNC_GETCUSTOMPROPERTY;
                 // Надо дойти до завершающей скобки
                 EndBrace( p );
                 ptr = p;
                 return true;
                 }
               else return false;
/*
               SToken& token = uni.Add();         
               token.dw  = VALUE_CUSTOMPROPERTY;
               char *p1 = p+_countof(".getcustomproperty")-1;
               SkipSpace( p1 );
               if(*p1=='(')
                {
                 char *p2 = strchr(p1+1, ')');
                 if(p2)
                   {
                   token.txt = ptr;
                   p2[1] = 0;
                   ptr = p2+2;
                   return true;
                   }
                }
               return false;
*/
             }
           else
           if(_strnicmp(p ,".objects", _countof(".objects")-1) == 0)
             {
               SToken& token = uni.Add();         
               token.dw  = VALUE_OBJECT;
               char *p1 = p+_countof(".objects")-1;
               SkipSpace( p1 );
               if(*p1=='(')
                {
                 token.txt = ptr;
                 char *p2 = strchr(p1+1, ')');
                 if(p2)
                   {
                   SkipSpace( ++p2 );
                   if(p2[0]=='.')
                     {//параметр
                       while(*p2!=0 && *p2!='\n' && *p2!='=' && *p2!=' ')
                           p2++;
                       if(p2[0]==' ' || p2[0]=='\n')
                       {
                          p2[0] = 0;
                          p2++;
                       }
                     }
                   else
                   if(p2[-1]==' ' || p2[-1]=='\n')
                     p2[-1] = 0;
                   ptr = p2;
                   return true;
                   }                   
                }      
               return false;
             }
           else
           if(_strnicmp(p ,".getattribute", _countof(".getattribute")-1) == 0)
            {
               p +=_countofchar(".getattribute");
               SkipSpace( p );
               if ( *p == '(' )
                {       
                 // Начало аргумента
                 SkipSpace( ++p );
                 SToken& token = uni.Add();
                 token.txt = ptr;
                 token.dw  = FUNC_GETATTRIBUTE;
                 // Надо дойти до завершающей скобки
                 EndBrace( p );
                 ptr = p;
                 return true;
                 }
               else return false;
            }
           else
           if(_strnicmp(p ,".setattribute", _countof(".setattribute")-1) == 0)
            {
               p +=_countofchar(".setattribute");
               SkipSpace( p );
               if(*p)
                {       
                 // Начало аргумента
                 SkipSpace( ++p );
                 SToken& token = uni.Add();
                 token.txt = ptr;
                 token.dw  = FUNC_SETATTRIBUTE;
                 char *pxA = strchr(p, '\xA');
                 char *pxD = strchr(p, '\xD');
                 if(pxA==NULL && pxD==NULL)
                 {
                   ptr = p+strlen(p);
                   return true;
                 }
                 else
                 if(pxA==NULL)
                   ptr = pxD;
                 else      
                 if(pxD==NULL)
                   ptr = pxA;
                 else 
                   ptr = min(pxA, pxD);
                 //if(_strnicmp(ptr-_countofchar(" end if"), " end if", _countofchar(" end if"))==0)
                 //  ptr -=_countofchar(" end if");//подсунутый в предварительной обработке "end if"
                 if(*ptr)
                   *ptr++=0;
                 return true;
                 }
               else return false;
            }
           }
        }
      }
    }
  return false;
  }
//
bool IsStdF( char*& ptr, SUniToken& uni )
  {
  int n = 0;
  while ( stdF[n].name )
    {
    if ( _strnicmp( ptr, stdF[n].name, stdF[n].dl ) == 0 )
      {
      char* p = ptr+stdF[n].dl;
      SkipSpace( p );
      if ( stdF[n].dw == FUNC_BRACE )
        {
        SToken& token = uni.Add();
        token.txt = p;
        token.dw  = stdF[n].dw;
        // Надо дойти до завершающей скобки
        EndBrace( p );
        ptr = p;
        return true;
        }
      else
      if ( *p == '(' )
        {
        // Начало аргумента
        p++; SkipSpace( p );
        //
        SToken& token = uni.Add();
        token.txt = p;
        token.dw  = stdF[n].dw;
        // Надо дойти до завершающей скобки
        EndBrace( p );
        ptr = p;
        return true;
        }
      else
      if ( stdF[n].var == 1 )
       {// параметры без скобок
        SToken& token = uni.Add();
        token.txt = p;
        token.dw  = stdF[n].dw;
        char *pxA = strchr(p, '\xA');
        char *pxD = strchr(p, '\xD');
        if(pxA==NULL && pxD==NULL)
        {
          ptr = p+strlen(p);
          return true;
        }
        else
        if(pxA==NULL)
          ptr = pxD;
        else      
        if(pxD==NULL)
          ptr = pxA;
        else 
          ptr = min(pxA, pxD);
        if(_strnicmp(ptr-_countofchar(" end if"), " end if", _countofchar(" end if"))==0)
          ptr -=_countofchar(" end if");//подсунутый в предварительной обработке "end if"
        if(*ptr)
          *ptr++=0;
        return true;
       }
       else
       if ( stdF[n].var == 2 )
       {//без параметров
        SToken& token = uni.Add();
        token.txt = "";
        token.dw  = stdF[n].dw;
        ptr = p;
        return true;
       }
      }
    n++;
    }
  return false;
  }
//
// Устанавливает целочисленную переменную
bool IsIntV( char*& ptr, SUniToken& uni )
  {
  char* p = ptr;
  while ( '0' <= *p && *p <= '9' ) p++;
  if ( p == ptr ) return false;
  if (*p == '.' ) return false;
  if ( IsSpace(*p) ) *p++ = 0;
  //
  SToken& token = uni.Add();
  token.txt = ptr;
  token.dw  = VAR_INTEGER;
  ptr = p;
  return true;
  }
//
// Устанавливает переменную с плавающей запятой
bool IsFltV( char*& ptr, SUniToken& uni )
  {
  char* p = ptr;
  while ( ('0' <= *p && *p <= '9') || *p == '.' ) p++;
  if ( p == ptr ) return false;
  if ( IsSpace(*p) ) *p++ = 0;
  //
  SToken& token = uni.Add();
  token.txt = ptr;
  token.dw  = VAR_DOUBLE;
  ptr = p;
  return true;
  }
//
// Устанавливает строковую переменную
bool IsStrV( char*& ptr, SUniToken& uni )
  {
  if ( *ptr !=  '"' ) return false;
  char* p = strchr( ptr+1, '"' );
  if(p==NULL)
    return false;
  ASSERT(p); *p++ = 0;
  //
  SToken& token = uni.Add();
  token.dw  = VAR_STRING;
  token.txt = ptr+1;
  ptr = p;
  return true;
  }
//
// Пропускаем комментарий до конца строки
bool IsComm( char*& ptr, SUniToken& uni )
  {
  if ( *ptr !=  '\'' ) return false;
  char* p = strchr( ptr, 0xa );
  if ( p ) ptr = p+1;
  else
    {
    int dl = lstrlen( ptr );
    ptr += dl;
    }
  return true;
  }
//
bool IsStdV2( char*& ptr, SToken& token, CHTML& html )
  {
  int n = 0;
  while ( stdV[n].name )
    {
    if ( _strnicmp( ptr, stdV[n].name, stdV[n].dl ) == 0 )
      {
      // С параметрами
      char* p = ptr+stdV[n].dl;
      if ( _PARAM_BEGIN <= stdV[n].dw && stdV[n].dw <= _PARAM_END )
        {
        SkipSpace( p );
        if ( *p == '(' )
          {
          // Начало аргумента
          p++; SkipSpace( p );
          //
          token.txt = p;
          token.dw |= stdV[n].dw;
          // Надо дойти до завершающей скобки
          EndBrace( p );
          ptr = p;
          if( (stdV[n].dw == PARAM_DATAVALUE) && (FindVar(token.txt) || strchr(token.txt, '&') || strchr(token.txt, '+')) )
          {//если в параметре выражение
           token.dw = FUNC_DATAVALUE;
          }
          return true;
          }
        else
          {
          ASSERT(0);
          }
        }
      else
        {
        if(stdV[n].dw == VALUE_OBJECT)
        {
            char *pp1 = p;
            SkipSpace( pp1 );
            if(*pp1 == '(')
            {
              SkipSpace( ++pp1 );
              if(*pp1 == '"')
              {
                char *pp2 = strchr(++pp1, '"');
                if(pp2)
                {
                *pp2 = '\0';
                char *pp3 = strchr(token.txt, '.');

                if(pp3 != NULL)
                   {
                      *pp3 = '\0'; 
                      CTag* partag = html.mBody->LinkScript( html, token.txt );                   
                      if(partag)
                      {
#ifdef _DEBUG
      CStyle& pstyle = html.Style(partag->m_nStyle);
      const char* pid = pstyle.NameID(html);
#endif                  
                        CTag* tag = partag->Objects(html, pp1);
                        if ( tag )                 
                        {
#ifdef _DEBUG
      CStyle& style = html.Style(tag->m_nStyle);
      const char* id = style.NameID(html);
#endif
                        char *pp4 = pp2;
                        SkipSpace(++pp4);
                        if(*pp4==')' && pp4[1]=='.')
                          {
                          //if( /*Version!=AM2_NAK ||*/ strnicmp(pp2+2, "style.visibility", _countof("style.visibility")-1) != 0 ) 
                          { //прввильный  token.txt для уникальности имен
                            *pp2 = '"'; 
                            *pp3 = '.'; 
                            token.mTag = tag;
                          }
                          return IsStdV2( ptr = pp4+2, token, html );
                          }
                        }
                      }
                   }
                }
              }
            }
          return false;
        }
  
        token.dw |= stdV[n].dw;
        ptr = p;
        return true;
        }
      }
    n++;
    }
  //
  ASSUNO
  return false;
  }
//
bool IsStdW( char*& ptr, SToken& token )
  {
  int n = 0;
  while ( stdW[n].name )
    {
    if ( _strnicmp( ptr, stdW[n].name, stdW[n].dl ) == 0 )
      {
      // С параметрами
      char* p = ptr+stdW[n].dl;
      if ( _PARAM_BEGIN <= stdW[n].dw && stdW[n].dw <= _PARAM_END )
        {
        ASSERT(0);
        SkipSpace( p );
        if ( *p == '(' )
          {
          // Начало аргумента
          *p++ = 0; SkipSpace( p );
          //
          token.txt = p;
          token.dw |= stdW[n].dw;
          // Надо дойти до завершающей скобки
          EndBrace( p );
          ptr = p;
          return true;
          }
        else
          {
          ASSERT(0);
          }
        }
      else
        {
        token.dw |= stdW[n].dw;
        ptr = p;
        return true;
        }
      }
    n++;
    }
  //
  ASSUNO
  return false;
  }
//
bool CVBScript::IsStdV( char*& ptr, SUniToken& uni, CTag* pThis, CHTML& html )
  {
  char* p;
  // Обязательно должна быть точка
  p = ptr;
  while ( IsN(*p) ) p++;
  // Это число с плавающей запятой
  if ( *p == '.' )
    return false;
  if ( *p == ',' )
    return false;
  //
  p = ptr;
  while ( IsV(*p) ) p++;
  //Это какой-то параметр ?
  if ( *p != '.' )
  {
    if(Version==DOTF_PM || Version==AM2_NAK || Version==DMW_NAK) 
    { //используются имена элементов без парамертов - похоже эквивалентоно .value - так работает
    char c = *p;
    *p = 0;
    CTag* pTag = html.mBody->LinkScript( html, ptr );
    *p = c;
    if(pTag && typeid(*pTag) != typeid(CTagBODY))
      {
        while(!IsSpace(*p)) 
          p++;
        if(IsSpace(*p))
          *p++=0;
        SToken& token = uni.Add();
        token.mTag = pTag;
        token.txt = ptr;
        token.dw = VALUE_VALUE;
        ptr = p;
        return true;
      }
    }
    else
    {
      char c = *p;
      if(IsSpace(*p))
      {          
          *p= 0;
           CTag* pTag = html.mBody->LinkScript( html, ptr );
           if(pTag && typeid(*pTag) != typeid(CTagBODY))
           {
           SToken& token = uni.Add();
           token.txt = ptr;
           token.dw = VALUE_TAG; 
           token.mTag = pTag;
           ptr=p+1;
           return true;
           }
      }
      *p = c;
    }
    return false;
  }

  if ( _strnicmp( ptr, "me.focus", _countof("me.focus")-1 ) == 0 )
  {
      ptr += _countof("me.focus")-1;
      return true;
  }
  // Это какой-то параметр
  SToken& token = uni.Add();
  token.txt = ptr;
  // Ссылка на текущий CTag
  char* B = ptr;
  char* E = p;
  if ( _strnicmp( ptr, "me.", 3 ) == 0 )
    {
    CTag* pTag = pThis;
    char *pp = ptr+2;
    bool pParent=false;
    while(_strnicmp(pp, ".parentnode", _countof(".parentnode")-1)==0)
      {
        pParent=true;
        token.dw = VALUE_TAG; 
        if(pTag->m_pPar)
          pTag = pTag->m_pPar;
        else return false;
        pp += _countof(".parentnode")-1;
#ifdef _DEBUG
      CStyle& style = html.Style(pTag->m_nStyle);
      const char* id = style.NameID(html);
#endif
      }
    ptr = pp;
    if(pParent)
       *ptr++ = 0;

	if(strncmp(pp, ".id", sizeof(".id")-1)==0)
      {
        CStyle& style = html.Style(pThis->m_nStyle);
        const char* id = style.NameID(html);
        SToken& token = uni.Add();
        token.dw  = VAR_STRING;
        token.txt = (char*)id;
        ptr = pp + sizeof(".id")-1;
        return true;
      }
    token.mTag = pTag;
    if(token.dw == VALUE_TAG) 
      return true;
    }
  else
    {
    *p = 0;
    CTag* tag = NULL;
    if(Version >= AM2_NAK)
      tag = html.mBody->FindNearestTag(html, pThis, ptr);
    if(tag == NULL)
      tag = html.mBody->LinkScript( html, ptr );
    if ( tag )
      {
      token.mTag = tag;
      }
    else
    if(_stricmp(ptr, "document")==0 && _strnicmp(p+1, "title", _countof("title")-1)==0)
      {
      token.mTag = html.mBody;
      token.dw = VALUE_DOCUMENTTITLE;
      *p = '.';
      ptr = p + _countof("title");
      return true;
      }
    else
    {
    SLocalStd* pvar = FindVar(ptr);
    if(pvar && _strnicmp(p+1, "parentnode", _countofchar("parentnode"))==0)
      {  //переменная для родительского объекта 
         *p = '.';
         token.dw = VALUE_TAG;
         token.txt = ptr;
         token.mTag = NULL; 
         p += _countofchar(".parentnode");
         while(_strnicmp(p, ".parentnode", _countofchar(".parentnode"))==0)
           p += _countofchar(".parentnode");
         *p = 0;
         ptr=p+1;
         return true;
      }
    else 
    if(pvar)
      {
          char *pp = p+1;
          if(IsStdV2( pp, token, html))
          {
              *p = '.';
              token.dw  = VALUE_OBJECT;
              token.txt = ptr;
              token.mTag = NULL;
              while(*pp && !IsSpace(*pp) && *pp!='=') ++pp;
              if(*pp!='=')
                {
                if(*pp)
                  {
                  *pp = 0;
                  ptr = pp+1;
                  }
                else ptr = pp;
                }
              else ptr = pp;
              return true;
          }
          else
          if(_strnicmp( pp, "id", _countofchar("id"))==0)
          {
              *p = '.';
              token.dw  = VALUE_OBJECT;
              token.txt = ptr;
              token.mTag = NULL;
              pp+=_countofchar("id");
              if(IsSpace(*pp)) 
                *pp = 0;
              ptr = pp+1;
              return true;
          }
          else
          if(_strnicmp( pp, "close", _countofchar("close"))==0)
          {
              *p = '.';
              token.dw  = FUNC_CLOSE_POPUP;
              token.txt = ptr;
              token.mTag = NULL;
              pp+=_countofchar("close");
              if(IsSpace(*pp)) 
                *pp = 0;
              ptr = pp+1;
              return true;
          }
          else
          if(_strnicmp( pp, "src", _countofchar("src"))==0)
          {
              *p = '.';
              token.dw  = VALUE_OBJECT;
              token.txt = ptr;
              token.mTag = NULL;
              pp+=_countofchar("src");
              if(IsSpace(*pp)) 
                *pp = 0;
              ptr = pp+1;
              return true;
          }
          else
          if(_strnicmp( pp, "quality", _countofchar("quality"))==0)
          {
              *p = '.';
              token.dw = VAR_INTEGER;
              token.txt = "192";
              token.mTag = NULL;
              pp+=_countofchar("quality");
              if(IsSpace(*pp)) 
                *pp = 0;
              ptr = pp+1;
              return true;
          }
      }
    else
      {
      ASSUNO
      }
    }
    *p = '.';
    }
  p++;

  if(token.mTag==NULL && _strnicmp(ptr, "document.getElementById", _countof("document.getElementById")-1) == 0)
    {
         p += _countof("getElementById")-1;
         SkipSpace( p );
         if ( *p == '(' )
            {       
             // Начало аргумента
             SkipSpace( ++p );
             char *pp = p;
             // Надо дойти до завершающей скобки
             EndBrace( p );
             if(*p == '.')
               {
                 if(strncmp(p+1, "CreatePopupWindow", _countof("CreatePopupWindow")-1)==0)
                 {//для этого не нужно выполнять getElementById()"
                 *p = '.';
                 token.txt = p+1;
                 token.dw  =  FUNC_CREATEPOPUOWINDOW;
                 token.mTag = NULL;
                 p += _countof("CreatePopupWindow");
                 SkipSpace( p );
                 if ( *p == '(' )
                  {
                    // Начало аргумента
                    p++; SkipSpace( p );
                    //
                    token.txt = p;
                    token.dw  =  FUNC_CREATEPOPUOWINDOW;;
                    // Надо дойти до завершающей скобки
                    EndBrace( p );
                    ptr = p;
                    return true;
                  }
                 return false;
                 }

                 char tempstr1[256];
                 sprintf_s(tempstr1, "$$TMP%d$$", ++m_nTempCount);
                 m_TempStrings.push_back(tempstr1);
                 token.txt = (char*)m_TempStrings.at(m_TempStrings.size()-1).c_str();
                 token.dw  = VALUE_OBJECT;
                 token.mTag = NULL;
                 SToken& tokenEQ = uni.Add();
                 tokenEQ.dw = OPER_EQ;
                 tokenEQ.txt = "=";
                 SToken& tokenGETELEMENTBYID = uni.Add();
                 tokenGETELEMENTBYID.dw = FUNC_GETELEMENTBYID;
                 tokenGETELEMENTBYID.txt = pp;
                 pp = p;
                 p++;
                 while( *p && !IsSpace(*p) && *p!='=' ) p++;
                 char ch = *p;
                 *p = '\0';
                 SToken& tokenTMP = uni.Add();
                 tokenTMP.dw = VALUE_OBJECT;
                 char tempstr[256];
                 sprintf_s(tempstr, "%s%s", tempstr1, pp);
                 m_TempStrings.push_back(tempstr);
                 tokenTMP.txt = (char*)m_TempStrings.at(m_TempStrings.size()-1).c_str();
                 *p = ch;
                 ptr = p;
                 return true;
               }
             else
               {     
               *p = '\0';
               token.txt = pp;
               token.dw  = FUNC_GETELEMENTBYID;
               ptr = p+1;
               return true;
               }
            }
          return false;
    }

  if(IsStdV2( p, token, html ))
  {
  if(token.dw == VALUE_CANCELCHANGE)
   { // добавляю =1 чтоб не нарушать логику
       SToken& tokenEQ = uni.Add();
       tokenEQ.dw = OPER_EQ;
       tokenEQ.txt = "=";
       SToken& token1 = uni.Add();
       token1.dw = VAR_INTEGER;
       token1.txt = "1";
   }
  }
  else
  {
     uni.m_nCurrent -= uni.m_dwSizeST; //убираю созданый ранее token
     return false;
  }
  if ( IsSpace(*p) ) *p++ = 0;
  ptr = p;
  //
  return true;
  }
//
bool IsStdW( char*& ptr, SUniToken& uni, CTag* pThis, CHTML& html )
  {
  if ( _strnicmp( ptr, "window.", 7 ) != 0 )
    return false;
  //
  SToken& token = uni.Add();
  token.txt = ptr;
  token.mWnd = html.mWnd;
  //
  char* p = ptr+7;
  if(!IsStdW( p, token ))
  {
    uni.m_nCurrent -= uni.m_dwSizeST;
    return false;
  }

  if ( token.dw == WINDOW_EVENT )
  {
     if(*p == '.')
       {// что-то вроде - window.event.srcelement.value          
        // Начало аргумента
         SkipSpace( ++p );
       }
      token.txt = ptr + _countof("window.")-1;
      token.dw  = VALUE_WINDOWEVENT;
      token.mWnd = NULL;
      //пропустить параметры
      while( *p && !IsSpace(*p) && *p!= '=' && *p!= '<' )
         p++;
      ptr = p;
      if(*ptr == ' ')
        *ptr++ = '\0';
      return true;
  }
  else if( token.dw ==VALUE_WINDOWS_EXTERNAL_CURRENTPAGE)
  {
      AAA();
  }

  if ( IsSpace(*p) ) *p++ = 0;
  ptr = p;
  //
  if ( token.dw == WINDOW_EXT_CLOSE )
    {
    _static char* a = "=";
    _static char* b = "true";
    SToken& t1 = uni.Add();
    t1.dw = OPER_EQ;
    t1.txt = a;
    SToken& t2 = uni.Add();
    t2.dw = CONST_TRUE;
    t2.txt = b;
    t2.nVar = 1;
    }
  /*else
  if ( token.dw == WINDOW_EVENT_SRCELEMENT )
  {
      token.dw = VALUE_TAG; 
      token.mTag = pThis;
  }*/
  //
  return true;
  }
// переменные не определенные явно
bool IsAutoVarV( char*& ptr, SUniToken& uni )
{
    if (Version == LG35_8_KF)
        return false;
    char *pp1 = ptr;
    while(IsV(*pp1))
        pp1++;
    if(*pp1 == ' ')
    {
        char *pp2 = pp1;
        SkipSpace(++pp1);
        if(*pp1 == '=')
         {
          if( curT >= (_MAX_TEMP-1))
            return false;
          SToken& token = uni.Add();
          token.txt = ptr;
          token.dw  = KEY_DIM;
          //
          char ch = *pp2;
          *pp2 = 0;
          stdT[curT].name = token.txt;
          stdT[curT].dl = lstrlen(stdT[curT].name);
          *pp2 = ch;  //т.к. на слудующей итерации парсинга переменная будет найдена и поставлен 0;
          curT++;
          return true;
        }          
    }
    else
    if(*pp1 == '=')
    {
          char *pp2 = pp1;
          if( curT >= (_MAX_TEMP-1))
            return false;
          SToken& token = uni.Add();
          token.txt = ptr;
          token.dw  = KEY_DIM;
          //
          char ch = *pp2;
          *pp2 = 0;
          stdT[curT].name = token.txt;
          stdT[curT].dl = lstrlen(stdT[curT].name);
          *pp2 = ch;  //т.к. на слудующей итерации парсинга переменная будет найдена и поставлен 0;
          curT++;
          return true;         
    }

    return false;
}

bool CVBScript::IsSubroutine( char*& ptr, SUniToken& uni, CTag* pThis, CHTML& html )
{
    char *pp1 = ptr;
    size_t len = strlen(ptr);
    while(IsV(*pp1))
        pp1++;
    char subname[256]="";
    char pp1ch = *pp1;
    char *pp2 = pp1+1;
    if(*pp1 != '(' )
      {
       *pp1 = '\0';
       goto NO_PARAM;
      }
    if(*pp1 == '(' )
      {
       *pp1 = '\0';
       SkipSpace(pp2);
       if(*pp2 == ')' ) 
         {//тут без параметров
NO_PARAM:
         strncpy_s(subname, ptr, pp1-ptr);
         std::map<std::string, SUB_STRUCT>::iterator it = html.m_Subroutines.find(subname);
         if(it!=html.m_Subroutines.end())
           {   
#ifdef _DEBUG
           if(stricmp(subname,"GPVUpdate")==0)
             AAA();
           if(stricmp(subname,"GOPUpdate")==0)
             AAA();
#endif
            std::string vvv;
#ifdef _DEBUG
            char header[256];
            sprintf_s(header, "MR_Comment(\"%s\")\xD\xA ", subname);
            vvv = header;
#endif
            char *vv = ReplaceOperatorSelect( it->second.body.c_str() );
            if(vv)
            {
                vvv += vv;
                delete []vv;
            }
            else  vvv += it->second.body;
            m_TempStrings.push_back(vvv);
            std::string &tmpstr = m_TempStrings.at(m_TempStrings.size()-1);
            tmpstr.resize(tmpstr.size()*2);
            char *txt = (char*)tmpstr.c_str();
            if ( VBScriptScan( html, uni, pThis, txt, false ) )
              {     
                  *pp1 = pp1ch;             
                  if((pp2-ptr) < (int)len)
                  {
                    ptr = pp2;
                    if(*ptr == ')')
                      ptr++;
                  }
                  else{ ptr = pp2; *ptr='\x0'; };
                  return true;
              }
            else { ptr = pp2; *ptr='\x0'; };
           }
         }
       else
         {//с параметрами
         std::string  fullsub;
         strncpy_s(subname, ptr, pp2-ptr);
#ifdef _DEBUG
            char header[256];
            sprintf_s(header, "MR_Comment(\"%s\")\xD\xA ", subname);
            fullsub = header;
#endif
         std::map<std::string, SUB_STRUCT>::iterator it = html.m_Subroutines.find(subname);
         bool bOk = false;
         if(it == html.m_Subroutines.end())
         {
            it = CHTML::m_CommonSubroutines.find(subname);
            if(it != html.m_CommonSubroutines.end())
              bOk = true;
         }
         else bOk = true;
         if(bOk)
         {//подпрограмма с параметрами
#ifdef _DEBUG
           if(stricmp(subname,"DockState")==0)
             AAA();
          if(stricmp(subname,"LPumpRoomLEDs")==0)
             AAA();
#endif
           //char* pp3 = strchr(pp2+1, ')');
           //if(pp3==NULL)
           //  return false;
           int nBrackets = 1;
           char* pp3 = pp2+1;
           while(*pp3)
           {
             if(*pp3==')')
             {
                if(--nBrackets <= 0)
                  break;
             }
             else
             if(*pp3=='(')
               ++nBrackets;
             pp3++;
           }
           if(*pp3==0)
             return false;

           SUB_STRUCT subst = it->second; 
           char params[256]="";
           strncpy_s(params, pp1+1, pp3-pp1-1);
           if(it->second.bFunction)
             {
                fullsub += "dim " + it->first;
                fullsub + "\n";
             }
           char *prm = params;
           for(int ii=0; ii<subst.params.size(); ii++)
           {
              char *pcomma = strchr(prm, ',');        
              if(pcomma)
                *pcomma++ = 0;  
              char setpar[256];
              sprintf_s(setpar, " dim %s\n %s=%s\n", subst.params[ii].c_str(), subst.params[ii].c_str(), prm );
              fullsub += setpar;
              if(pcomma)
                prm = pcomma;
           }    
           fullsub += subst.body;
           while(char *vv = ReplaceOperatorSelect( fullsub.c_str()) )
           {
             fullsub = vv;
             delete []vv;
           }
           m_TempStrings.push_back(fullsub);
           std::string &tmpstr = m_TempStrings.at(m_TempStrings.size()-1);
           tmpstr.resize(tmpstr.size()*2);
           char *txt = (char*)tmpstr.c_str();
           char *txtx = new char[tmpstr.size()*2];
           strcpy(txtx, txt);
           SUniToken unix;
           int SavecurT = curT;
           size_t SaveTempStringsSize = m_TempStrings.size();
           if ( !VBScriptScan( html, unix, pThis, txtx, false ) )                              
              {//пока просто пропускаю скрипты с ошибками 
                  *pp1 = pp1ch;
                  ptr = pp3+1;
                  delete []txtx;
                  if(curT > SavecurT)
                    memset( &stdT[SavecurT], 0, sizeof(stdT[0])*(curT-SavecurT));
                  curT = SavecurT;
                  m_TempStrings.resize(SaveTempStringsSize);
                  return true;
              }   
           delete []txtx;
           curT = SavecurT;
           m_TempStrings.resize(SaveTempStringsSize);
           if ( !VBScriptScan( html, uni, pThis, txt, false ) )
                return false;
           else
              {
                  *pp1 = pp1ch;
                  ptr = pp3+1;
                  return true;
              }
          }
         }
      }
     *pp1 = pp1ch;
    return false;
}
//
bool CVBScript::IsDisplayDataRepository( char*& ptr, SUniToken& uni, CTag* pThis, CHTML& html  )
{
    if(Version != BERTH_ONPZ && Version < VSB_KF)
      return false;
    char varname[256];
    int ii=0;
    char *pp = ptr;
    while ( IsV( *pp ) ) 
    {
       if(ii < (_countof(varname)-1))
         varname[ii++] = *pp++;
       else return false;;
    }
    varname[ii] = 0;
    std::string varval = html.FindInDisplayDataRepository(varname, true);
    if(varval.size()>0 && m_TempStrings.capacity() > m_TempStrings.size())
    {
      SToken& token = uni.Add();
      token.dw  = VAR_STRING;
      m_TempStrings.push_back(varval);
      token.txt = (char*)m_TempStrings.at(m_TempStrings.size()-1).c_str();
      ptr = pp;
      return true;
    }
    const O_VAL* poval = html.GetCommomValue(varname);
    if(poval)
    {
        SToken& token = uni.Add();
        switch(poval->eType)
        {
        case enumValueStr:
          token.dw  = VAR_STRING;
          m_TempStrings.push_back(poval->ostr);
          token.txt = (char*)m_TempStrings.at(m_TempStrings.size()-1).c_str();
          break;
        default:
          token.dw  = VAR_INTEGER;
          char txt[64];
          sprintf_s(txt, "%d", poval->oint);
          m_TempStrings.push_back(txt);
          token.txt = (char*)m_TempStrings.at(m_TempStrings.size()-1).c_str();
          break;
        }
      ptr = pp;
      return true;
    }

    return false;
}
//
bool IsExit( char*& ptr, SUniToken& uni )
{
  SkipSpace( ptr );
  if(_strnicmp(ptr, "sub", _countof("sub")-1) != 0)
     return false;
  static char* a = "ExitSub";
  ptr += _countof("sub")-1;
  SToken& token = uni.Add();
  token.txt = a;
  token.dw  = KEY_EXITSUB;
  return true;
}
//
