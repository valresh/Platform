#include "StdAfx.h"
#include "HTML.h"
#include "TagSCRIPT.h"
#include "TagTextArea.h"
#include "VBScript.h"
#include "..\\ReplaceString.h"

#include "Style.h"

#include "Tag.h"
#include <map>

CTagSCRIPT::CTagSCRIPT(void)
: m_eLang(langUnk)
, m_eType(eventUnk)
, m_pLinkS(NULL)
  {
  }
//
#undef  STD_VAR
#define STD_VAR CHTML& html, CTagSCRIPT& a, char* value
typedef void (*LPFuncs)( STD_VAR );
//
void scriptLanguage( STD_VAR )
  {
  if ( !*value ) return;
  ELanguage eLang = langUnk;
       if ( lstrcmpi(value,"VBScript"   ) == 0 ) eLang = langVBScript;
  else if ( lstrcmpi(value,"JScript"    ) == 0 ) eLang = langJScript ;
  else if ( lstrcmpi(value,"javascript" ) == 0 ) eLang = langJScript ;
  else
    {
    ASSERT(0);
    }
  a.m_eLang = eLang;
  }
//
EventType GetEventType(const char * value)
{
  EventType eEvent = eventUnk;
  if(value != NULL && *value)
  {
       if ( lstrcmpi(value,"OnUpdate"    ) == 0 ) eEvent = eventOnUpdate;
  else if ( lstrcmpi(value,"OnDataChange") == 0 ) eEvent = eventOnDataChange;
  else if ( lstrcmpi(value,"OnClick"     ) == 0 ) eEvent = eventOnClick;
  else if ( lstrcmpi(value,"OnChange"    ) == 0 ) eEvent = eventOnChange;
  else if ( lstrcmpi(value,"OnShapeLoad" ) == 0 ) eEvent = eventOnShapeLoad;
  else if ( lstrcmpi(value,"OnMouseDown" ) == 0 ) 
      eEvent = eventOnMouseDown;
  else if ( lstrcmpi(value,"OnMouseUp"   ) == 0 ) eEvent = eventOnMouseUp;
  else if ( (Version!=LG35_8_KF) && (lstrcmpi(value,"onperiodicupdate") == 0) ) 
          eEvent = eventOnPeriodIcupdate;
  else if ( (Version!=LG35_8_KF) && (lstrcmpi(value,"onpagecomplete") == 0) ) 
          eEvent = eventOnPageComplete;
    else if ( (Version!=LG35_8_KF) && (lstrcmpi(value,"onactivate") == 0) ) 
          eEvent = eventOnClick;
  }
  return eEvent;
}
//
void scriptEvent( STD_VAR )
  {
  if ( !*value ) return;
  
  a.m_eType = GetEventType(value);
  }
//
void scriptFor( STD_VAR )
  {
  CStyle& style = a.Style(html);
  style.NameID( html, value );
  }
//
void scriptId( STD_VAR )
  {
  }
//
void scriptDefer( STD_VAR )
  {
  ASSERT( *value == 0 );
  }
//
void scriptType( STD_VAR )
  {
  ASSERT( *value == 0 );
  }
//
void scriptSrc( STD_VAR )
  {
  // Встречается в sysAlarmSummary
  }
//
void scriptShaperefcount( STD_VAR )
  {
  ASSERT( ::IsInt(value) );
  int v = atoi(value);
  ASSERT( v == 1 );
  }
//
void replacechar(char * str, char ch1, char ch2)
{
    for(int kk=0; str[kk]; kk++)
       if(str[kk] == ch1)
         str[kk] = ch2;
}

static int selectNum = 0; 

//замена оператора Select на последовательность if
char *ReplaceOperatorSelect(const char* ptr)
{
    int len = (int)strlen(ptr);
    const char *pp = strstr(ptr, "Select Case");
    if(!pp)
       pp = strstr(ptr, "select case");
    char* p = (char*)pp;
    if (!pp)
    {
      pp = strstr(ptr, "Select");
      if (!pp)
          return NULL;
      char *pp0 = (char*)pp + _countof("Select")-1;
      if(*pp0 == ' ' || *pp0 == '\t')
      {
      SkipSpace( pp0 );
      if(_strnicmp(pp0, "Case", _countof("Case")-1) != 0)
        return NULL;
      p = pp0 + _countof("Case")-1;
      }
      else return NULL;
    }
    else p = (char*)pp + sizeof("Select Case")-1;
    SkipSpace( p );
    char varexpression[256]="";
    char Else[512] = "";
    for(size_t ii=0; ii<(_countof(varexpression)-1); ii++)
       if(*p!=' ' && *p!=0xD && *p!=0xA && *p!='\t') 
         {
         if(*p == '"')
         {
           varexpression[ii++] = *p++;
           varexpression[ii] = 0;
           if(ii>=(_countof(varexpression)-1))
             return NULL;
           while(*p != '"')
           {
           if (*p == 0)
             return NULL;
           if(ii>=(_countof(varexpression)-1))
             return NULL;
           varexpression[ii++] = *p++;
           varexpression[ii] = 0;
           }
         }
         varexpression[ii] = *p++;
         varexpression[ii+1] = 0;
         }
       else
       {
         char *pp3= strchr(varexpression, '('); 
         if( pp3 && pp3[1] != '\'')
           if(char pp4 = strchr(pp3, ')') == NULL)
           {//дописываю до ')'
             do
             {
             if(strncmp(p, "_\xD\xA", _countof("_\xD\xA")-1) == 0)
               p += 3;
             varexpression[ii++] = *p++;
             varexpression[ii] = 0;
             if(ii>=(_countof(varexpression)-1))
               return NULL;
             }
             while(*p != ')');  
             varexpression[ii++] = *p++;
             varexpression[ii] = 0;
           }
         break;
       }

    char SelectBuf[2*8192]="";
    char var[128];
    ++selectNum; 
    sprintf_s(var, "SELECTtemp%X", selectNum);
    sprintf_s( SelectBuf, " dim %s\xD\xA %s = %s\xD\xA", var, var, varexpression);
    int nCase=0;
    bool bEndIf = false;
    while(true)
    {
    SkipSpace( p );
    if ( *p == '\'' )
      {
      char* s = strchr( p, '\xA' );
      if ( s == NULL )
		{ *p = 0; break; }
      else
        while ( p < s ) 
           p++;
      SkipSpace( p );
      }
    if(_strnicmp( p, "case", sizeof("case")-1) != 0)
       break;
    p += sizeof("case")-1;
    bEndIf = false;
    nCase++;
    char tmp1[256]="";
    char tmp2[1024]="";
    SkipSpace( p );
    for(size_t ii=0; ii<(_countof(tmp1)-2); ii++)
       if((*p!=' ') && (*p!='\t') && (*p!='\xA') && (*p!='\xD')) 
         {
         tmp1[ii] = *p++;
         tmp1[ii+1] = 0;
         }
       else break;
    for(size_t ii=0; ii<(_countof(tmp2)-1); ii++)
       if(*p==0xD || *p==0xA)
         {
         SkipSpace( p );
         if( (_strnicmp(p, "end select", sizeof("end select")-1)==0)
             || (_strnicmp(p, "case ", sizeof("case ")-1)==0)
           )
            break;
         else
          {
          tmp2[ii++] = '\n';
          tmp2[ii] = *p++;
          tmp2[ii+1] = 0;
          }
         }
       else
       if(*p==0)
         break;
       else
         {
         tmp2[ii] = *p++;
         tmp2[ii+1] = 0;
         }
    char _if[1024]="";
    if(nCase>1)
       {
       if( _stricmp(tmp1, "Else") == 0 || _stricmp(tmp1, "Else:") == 0)
         sprintf_s( Else, " %s ", tmp2);
       else if(_strnicmp(tmp2, " if ", sizeof(" if ")-1) != 0)
               {
               replacechar(tmp2, '\n', ' ');
               if(Version < AM2_NAK) //пока так 
                 sprintf_s(_if, " end if if %s=%s then %s ", var, tmp1, tmp2);
               else sprintf_s(_if, "\n elseif %s=%s\n then\n %s\n ", var, tmp1, tmp2);
               bEndIf = true;
               }
            else{
                if( Version < AM2_NAK) //пока так 
                  sprintf_s(_if, " end if if %s=%s then\n %s\n end if\n", var, tmp1, tmp2);
                else sprintf_s(_if, "\n elseif %s=%s\n then\n %s\n end if\n ", var, tmp1, tmp2);
                }
       }
    else
      if(_strnicmp(tmp2, " if ", sizeof(" if ")-1) != 0)
        {
        replacechar(tmp2, '\n', ' ');
        if(Version < AM2_NAK) //пока так 
          sprintf_s(_if, " if %s=%s then %s ", var, tmp1, tmp2);
        else sprintf_s(_if, " if %s=%s\n then\n %s \n", var, tmp1, tmp2);
        bEndIf = true;
        }
      else{
          /*if(char *ref1 = strstr(tmp2, "elseif"))
            {
             if(char *ref2 = strstr(ref1, "end if"))
             {
             //replacechar(ref1, '\n', ' ');
             ref2[-1] = '\n';
             cross::CString tmp2_2(ref1);
             tmp2_2.Replace("elseif", "else\nif");
             tmp2_2 +=  "\nend if\n";
             strcpy(ref1, tmp2_2.GetBuffer());
             }
            }*/
          sprintf_s(_if, " if %s=%s then\n %s\n end if\n", var, tmp1, tmp2);
         }
    strcat_s(SelectBuf, _if); 
    }

    if((_strnicmp(p, "end select", sizeof("end select")-1)==0)&&(nCase>0))
      {
      p += sizeof("end select")-1;
      int sbuflen = (int)strlen(SelectBuf);
      int add = sbuflen - (int)(p-pp); //на это увеличилась длина
      int elen = (int)strlen(Else);
      int newlen = (elen + len + add + sizeof(" end if")) * 2;
      char *newbuf = new char[newlen];
      memset(newbuf, 0, newlen);
      if (elen)
         strcpy(newbuf, Else); 
      strncpy(newbuf+elen, ptr, pp-ptr);
      strcpy(newbuf + elen + (pp-ptr), SelectBuf);
      if(bEndIf || elen)
        strcat(newbuf, " end if\n");
      strcat(newbuf, p);
      int ll = (int)strlen(newbuf);
      return newbuf;
      }

    return NULL;
}
//
//замена вызовов call xxx на имеющиеся подпрограммы с установкой параметров
char* ReplaceCall(CHTML& html, const char* _ptr)
{
    //if(Version == LG35_8_KF)
      return NULL;

    char *ptr0 = (char*)strstr(_ptr, "call ");
    if (ptr0==NULL)
      return NULL;
    std::string newscript(_ptr);
    char *ptr = (char*)_ptr;
    while(char *pp = (char*)strstr(ptr, "call "))
      {
      char *pp1 = (char*)pp + _countofchar("call ");
      ptr = pp1;
      SkipSpace(pp1);  //pp1 начало имени фукции
      char *pp2 = pp1+1;
      while(*pp2 != ' ' && *pp2 != '(')
        pp2++;
      char subname[256]="";
      strncpy_s(subname, pp1, pp2-pp1);
      std::map<std::string, SUB_STRUCT>::iterator it = html.m_Subroutines.find(subname);
      if(it==html.m_Subroutines.end())
      {
        if(*pp2 == '(')
          {
          char* pp3 = strchr(pp2+1, ')');
          if(pp3==NULL)
            return NULL; 
          strncpy_s(subname, pp1, pp3-pp1+1);
          }
        newscript = ReplaceString(newscript, subname, ""); //заменяю вызов отсутвующего скрипта на пробел
        continue;
      }
      SUB_STRUCT subst = it->second; 
      SkipSpace(pp2);
      std::string  fullsub;
      if(*pp2 != '(')
       {////подпрограмма без параметров
         if(it->second.bFunction)
          {
             fullsub = "dim " + it->first;
             fullsub + "\n";
          }
         fullsub += subst.body;
         newscript = ReplaceString(newscript, subname, fullsub);
       }
      else
       { //подпрограмма с параметрами
        char* pp3 = strchr(pp2+1, ')');
        if(pp3==NULL)
          return NULL;
        ptr = pp3;
        strncpy_s(subname, pp1, pp3-pp1+1);
        char params[256]="";
        strncpy_s(params, pp2+1, pp3-pp2-1);
        if(it->second.bFunction)
          {
             fullsub = "dim " + it->first;
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
        newscript = ReplaceString(newscript, subname, fullsub);
       }
      }
    if( newscript.size() )
    {
     char *newbuf = new char[newscript.size()*2+1];
     strcpy_s(newbuf, newscript.size()*2, newscript.c_str());
     return newbuf;
    }
   return NULL;
}
//
void CTagSCRIPT::Analiz( ANALIZ )
  {
  struct SLocal { char* name; LPFuncs func; };
  _static SLocal list[] =
    {
    { "language"     , scriptLanguage      },
    { "event"        , scriptEvent         },
    { "for"          , scriptFor           },
    { "defer"        , scriptDefer         },
    { "id"           , scriptId            },
    { "src"          , scriptSrc           },
    { "type"         , scriptType          },
    { "shaperefcount", scriptShaperefcount },
    }InitLocal;
  //
  int n = 0;
  while ( teg[n].name )
    {
    SLocal* find = FindLocal( teg[n].name );
    ASSERT( find );
    if ( find != NULL )
    (*find->func)(html,*this,teg[n].value);
    n++;
    }
  //
  }

  void CTagSCRIPT::Kerdyk(HWND hWnd, CHTML &html)
  {
// CTag::Kerdyk( hWnd, html );
  }

  void CTagSCRIPT::Script(HWND hWnd, CHTML &html)
  {
      CTag::Script( hWnd, html );
      if( (html.m_strFile.CollateNoCase("sncdtlCPCa_fp")!=0) && (html.m_strFile.CollateNoCase("sncdtlSPCMa_fp")!=0)
          && (html.m_strFile.CollateNoCase("sysdtldevctlawarningAVS_fp")!=0) && (html.m_strFile.CollateNoCase("sysdtldevctlastop_fp")!=0)
          && (Version < DOTF_PM) //здесь всегда проверяем скрипты
          ) //не понял смысл пропуска по m_bR300- нужны скрипты фейсплейтам
          if ( html.m_bR300 )
              return;
      //
      CStyle& style = Style(html);
      //
      const char* name  = style.GetName (html);// SCRIPT
      const char* v     = style.GetValue(html);
      char* value;                       // код
      const char* id    = style.NameID  (html);// ID Тэга
      if(*id==0)
          return;

#ifdef _DEBUG
      if ( lstrcmpi( id, "Page" ) == 0 )
          KKK();
#endif
      //
      size_t nSriptLen = strlen(v);
      if ( nSriptLen <3 )
          return;
      if ( m_eLang != ELanguage::langVBScript ) return;
      //
      if ( m_eType == eventUnk )
      {
          //if(LG35_8_KF!=Version)
          //  html.GetSubroutines(v);
          return;
      }

      if ( m_eType == eventOnShapeLoad ) return;
      //
      ASSERT( html.mBody != NULL );
      if(m_pLinkS == NULL)
      {
          if(!html.m_LoadAcceleration)
              m_pLinkS = html.mBody->LinkScript( html, style.mId );
          else
              m_pLinkS = html.mBody->LinkScript( html, id );
      }
      else AAA();

      if ( m_pLinkS == NULL )
          return;

      if ( lstrcmpi( id, "Page" ) == 0 )
      {
          if(eventOnClick == m_eType)
              html.m_pScriptClick = this;
      }
      //
#ifdef _DEBUG
      if ( lstrcmpi( id, "ProductCmbx_1" ) == 0 )
          AAA();
      if ( lstrcmpi( id, "sd") == 0 )
          AAA();
#endif
      char *v2 = ReplaceCall(html, v);
      if(v2)
      {
          char *v3 = ReplaceOperatorSelect( v2 );
          if(v3)
          {
              delete[] v2;
              v2 = v3;
          }
      }
      else v2 = ReplaceOperatorSelect( v );
      if(v2)
      {
          value = v2;
          nSriptLen = strlen(v2);
      }
      else
      {
          value = new char[nSriptLen*2];
          strcpy(value, v);
          memset(value + nSriptLen, 0, nSriptLen);
      }
      lstrcat(value, "\n");
      mScript.SetVars( &html, m_pLinkS, id );
      if ( !mScript.Compile( value, NULL, false ) )
      {
          m_pLinkS = NULL;
          delete[] value;
          if( lstrcmpi( id, "Page" ) == 0 && eventOnClick == m_eType)
              html.m_pScriptClick = NULL;
          return;
      }
      mScript.Post();
      //
      CStyle& styl = m_pLinkS->Style(html);
      LPCTSTR link  = styl.GetName (html);
      switch ( m_eType )
      {
      case eventOnUpdate:
          ASSERT( m_pLinkS->m_pChange == NULL );
          m_pLinkS->m_pChange = this;
          break;
      case eventOnDataChange:
          ASSERT( m_pLinkS->m_pChange == NULL );
          m_pLinkS->m_pChange = this;
          break;
      case eventOnPeriodIcupdate:
          if( m_pLinkS->m_pChange == NULL )
              m_pLinkS->m_pChange = this;
          break;
      case eventOnPageComplete:
          if( m_pLinkS->m_pPageComplete == NULL )
              m_pLinkS->m_pPageComplete = this;
          break;
      case eventOnClick:
          ASSERT( m_pLinkS->m_pClickM == NULL );
          if(m_pLinkS->m_pClickM==NULL)
              m_pLinkS->m_pClickM = this;
          else KKK();
          break;
      case eventOnChange:
          ASSERT( m_pLinkS->m_pClickM == NULL );
          m_pLinkS->m_pClickM = this;
          break;
      case eventOnMouseDown:
          ASSERT( m_pLinkS->m_pMouseD == NULL );
          m_pLinkS->m_pMouseD = this;
          break;
      case eventOnMouseUp:
          ASSERT( m_pLinkS->m_pMouseU == NULL );
          m_pLinkS->m_pMouseU = this;
          break;
      default:
          ASSERT(0);
          break;
      }
      delete[] value;
#ifdef _DEBUG
  //mScript.TestPrint( (char*)id );
#endif
  }
//

//
/*virtual*/ /*void CTagSCRIPT::Script( KERDYK )
  {
  CTag::Script( hWnd, html );
  if( (html.m_strFile.CollateNoCase("sncdtlCPCa_fp")!=0) && (html.m_strFile.CollateNoCase("sncdtlSPCMa_fp")!=0) 
      && (html.m_strFile.CollateNoCase("sysdtldevctlawarningAVS_fp")!=0) && (html.m_strFile.CollateNoCase("sysdtldevctlastop_fp")!=0)
      && (Version < DOTF_PM) //здесь всегда проверяем скрипты
    ) //не понял смысл пропуска по m_bR300- нужны скрипты фейсплейтам  
  if ( html.m_bR300 )
  return;
  //
  CStyle& style = Style(html);
  //
  const char* name  = style.GetName (html);// SCRIPT
  const char* v     = style.GetValue(html);
  char* value;                       // код
  const char* id    = style.NameID  (html);// ID Тэга
  if(*id==0)
    return;

#ifdef _DEBUG
  if ( lstrcmpi( id, "Page" ) == 0 )
    KKK();
#endif
  //
  size_t nSriptLen = strlen(v);
  if ( nSriptLen <3 ) 
      return;
  if ( m_eLang != ELanguage::langVBScript ) return;
  //
  if ( m_eType == eventUnk ) 
  {
    //if(LG35_8_KF!=Version)
    //  html.GetSubroutines(v);
   return;
  }

  if ( m_eType == eventOnShapeLoad ) return;
  //
  ASSERT( html.mBody != NULL );
  if(m_pLinkS == NULL)
  {
	  if(!html.m_LoadAcceleration)
		m_pLinkS = html.mBody->LinkScript( html, style.mId );
	  else 
		m_pLinkS = html.mBody->LinkScript( html, id );
  }
  else AAA();

  if ( m_pLinkS == NULL ) 
     return;

  if ( lstrcmpi( id, "Page" ) == 0 )
  {
     if(eventOnClick == m_eType)
        html.m_pScriptClick = this;
  }
  //
#ifdef _DEBUG
  if ( lstrcmpi( id, "ProductCmbx_1" ) == 0 )
    AAA();
  if ( lstrcmpi( id, "sd") == 0 )
    AAA();
#endif
  char *v2 = ReplaceCall(html, v);
  if(v2)
    {
     char *v3 = ReplaceOperatorSelect( v2 );
     if(v3)
       {
           delete[] v2;
           v2 = v3;       
       }
    }
  else v2 = ReplaceOperatorSelect( v );
  if(v2)
    {
     value = v2;
     nSriptLen = strlen(v2);
    }
  else
  {
  value = new char[nSriptLen*2]; 
  strcpy(value, v);
  memset(value + nSriptLen, 0, nSriptLen);
  }
  lstrcat(value, "\n");
  mScript.SetVars( &html, m_pLinkS, id );
  if ( !mScript.Compile( value, NULL, false ) )
    {
    m_pLinkS = NULL;
	delete[] value;
    if( lstrcmpi( id, "Page" ) == 0 && eventOnClick == m_eType)
       html.m_pScriptClick = NULL;
    return;
    }
  mScript.Post();
  //
  CStyle& styl = m_pLinkS->Style(html);
  LPCTSTR link  = styl.GetName (html);
  switch ( m_eType )
    {
    case eventOnUpdate:
      ASSERT( m_pLinkS->m_pChange == NULL );
      m_pLinkS->m_pChange = this;
      break;
    case eventOnDataChange:
      ASSERT( m_pLinkS->m_pChange == NULL );
      m_pLinkS->m_pChange = this;
      break;
    case eventOnPeriodIcupdate:
      if( m_pLinkS->m_pChange == NULL )
        m_pLinkS->m_pChange = this;
      break;
    case eventOnPageComplete:
      if( m_pLinkS->m_pPageComplete == NULL )
        m_pLinkS->m_pPageComplete = this;
      break;
    case eventOnClick:
      ASSERT( m_pLinkS->m_pClickM == NULL );
      if(m_pLinkS->m_pClickM==NULL)
        m_pLinkS->m_pClickM = this;
      else KKK();
      break;
    case eventOnChange:
      ASSERT( m_pLinkS->m_pClickM == NULL );
      m_pLinkS->m_pClickM = this;
      break;
    case eventOnMouseDown:
      ASSERT( m_pLinkS->m_pMouseD == NULL );
      m_pLinkS->m_pMouseD = this;
      break;
    case eventOnMouseUp:
      ASSERT( m_pLinkS->m_pMouseU == NULL );
      m_pLinkS->m_pMouseU = this;
      break;
    default:
      ASSERT(0);
      break;
    }
	delete[] value;
#ifdef _DEBUG
//mScript.TestPrint( (char*)id );
#endif
  }*/
//
bool CTag::ValueValue( CHTML& html, DWORD dw, SValueValue& val )
  {
  val.mTag = NULL;
  _static bool b;
  CStyle& style = Style(html);
#ifdef _DEBUG
  const char* id = style.NameID(html);
#endif
  style.m_dwFlags |= HDX_VBASIC;
  switch ( dw )
    {
    case VALUE_TEXTCOLOR       :
      val.eVal = enumValueInt;
      val.Void = &style.m_clrText;
      style.m_dwFlags |= HDX_CLRTEXT;
      break;
    case VALUE_STYLE_VISIBILITY:
      val.eVal = enumValueInt;
      val.Void = &style.m_eVisible;
      break;
    case VALUE_STYLE_PIXELLEFT:
      val.eVal = enumValueFlt;
      val.Void = &style.mcross::RECT.X;
      break;
    case VALUE_STYLE_PIXELHEIGHT:
      val.eVal = enumValueFlt;
      val.Void = &style.mcross::RECT.Height;
      break;
    case VALUE_STYLE_PIXELWIDTH:
      val.eVal = enumValueFlt;
      val.Void = &style.mcross::RECT.Width;
      break;
    case VALUE_VISIBLERANGEHI  :
      val.eVal = enumValueDbl;
      val.Void = &style.m_dRangeHi;
      style.m_dwFlags |= HDX_RANGELOHI;
      break;
    case VALUE_VISIBLERANGELO  :
      val.eVal = enumValueDbl;
      val.Void = &style.m_dRangeLo;
      style.m_dwFlags |= HDX_RANGELOHI;
      break;
    case VALUE_VISIBLERANGEORIGIN:
      val.eVal = enumValueDbl;
      val.Void = &style.m_dRangeOrigin;
      style.m_dwFlags |= HDX_RANGEORIGIN;
      break;
    case VALUE_LEVELFILLCOLOR  :
      val.eVal = enumValueInt;
      val.Void = &style.m_clrFillLvl;
      style.m_dwFlags |= HDX_CLRFILLL;
      break;
    case VALUE_FILLCOLOR       :
      val.eVal = enumValueInt;
      val.Void = &style.m_clrFill;
      style.m_dwFlags |= HDX_CLRFILL;
      style.m_bClrFillCanChangedByScript = true;
      break;
    case VALUE_FILLCOLORBLINK       :
      val.eVal = enumValueInt;
      val.Void = &style.mFillColorBlink;
      break;
    case VALUE_LINECOLOR       :
      val.eVal = enumValueInt;
      val.Void = &style.m_clrLine;
      style.m_dwFlags |= HDX_CLRLINE;
      break;
    case VALUE_LINECOLORBLINK:
      val.eVal = enumValueBol;
      val.Void = &style.mLineColorBlink;
      break;
    case VALUE_TEXTCOLORBLINK:
      val.eVal = enumValueBol;
      val.Void = &style.mTextColorBlink;
      break;
    case VALUE_GRADIENTFILLCOLOR:
      val.eVal = enumValueInt;  
      val.Void = &style.m_clrFillGrd;
      style.m_dwFlags |= HDX_CLRFILLG;
      break;
    case VALUE_LINESTYLE:
      val.eVal = enumValueInt;  
      val.Void = &style.m_nLineStyle;
      style.m_dwFlags |= HDX_LINE_S;
      break;
    case VALUE_LINEWIDTH:
      val.eVal = enumValueInt;  
      val.Void = &style.m_nLineWidth;
      style.m_dwFlags |= HDX_LINE_W;
      break;
    case VALUE_HOSTPOPULATION  :
      val.eVal = enumValueBol;
      val.Void = &b;
      break;
    case VALUE_VALUE           :
      val.eVal = enumValueDbl;
      val.mTag = this;// Для вывода значения
      val.Void = m_szScriptValue;// NULL
	  if (this->GetType() == TT_SPAN)
		  val.Accept = ((CTagSPAN*)this)->m_pAcceptCheckboxValueCallback;
	  else 
	  if (GetType() == TT_TEXTAREAWITHCALLBACK)
		  val.Accept = ((CTagTextArea*)this)->m_pAcceptValueCallback;
      else
		  val.Accept = 0;
      break;
    case VALUE_INNERTEXT       :
      val.eVal = enumValueStr;
      val.mTag = this;// Для вывода значения
      val.Void = m_szScriptValue;// NULL
      break;
	case VALUE_NUMERICDISPLAYFORMAT       :
      val.eVal = enumValueStr;
      val.mTag = this;// Для вывода значения
      val.Void = m_szNumericDisplayValue;// NULL
      break;
    case VALUE_ROTATION        :
      val.eVal = enumValueDbl;
      val.Void = &style.m_dAngleNew;
      style.m_dwFlags |= HDX_ROTATE;
      break;
    case VALUE_CANCELCHANGE:
      val.eVal = enumValueBol;
      val.Void = &m_bCancelChange;
      break;
    case VALUE_STYLECLASS:
      val.eVal = enumValueStr;
      val.Void = &style.m_sStyleClass; //&style.mClass;
      break;
    case VALUE_ALARM:
      val.eVal = enumValueInt;
      val.Void = &style.m_alarm; 
      break;
    case VALUE_TITLE:
      val.eVal = enumValueStr;
      val.Void = &style.m_szTitle;
      break;
    case VALUE_DOCUMENTTITLE:
      val.eVal = enumValueStr;
      val.Void = &html.m_szTitle;
      break;
    case VALUE_STYLE_FONTWEIGHT:
      val.eVal = enumValueStr;
      val.Void = &style.m_fontWeight;
      break;
    case VALUE_STRARTARROW:
      val.eVal = enumValueInt;
      val.Void = &style.m_nStartArrow;
      break;
    case VALUE_ENDARROW:
      val.eVal = enumValueInt;
      val.Void = &style.m_nEndArrow;
      break;
    case VALUE_WINDOWS_EXTERNAL_CURRENTPAGE:
      val.eVal = enumValueStr;
      val.Void = &html.WINDOWS_EXTERNAL_CURRENTPAGE;
      break;
    case VALUE_SRC:
      val.eVal = enumValueStr;
      val.Void = style.SrcFile(html);
      break;
    case VALUE_SHELVED:
      val.eVal = enumValueBol;
      val.Void = &m_shelved;
      break;
    case VALUE_RTN:
      val.eVal = enumValueBol;
      val.Void = &m_RTN;
      break;
    case VALUE_ACKED:
      val.eVal = enumValueBol;
      val.Void = &this->m_acknowledged;
      break;
    case VALUE_PRIORITY:
      val.eVal = enumValueInt;
      val.Void = &m_priority;
      break;
    default:
      ASSERT(0);
      return false;
    };
  return true;
  }
//
/*virtual*/ void CTagSCRIPT::Matrix( LOCALS )
  {
  CStyle& style = Style(html);
  char* id = style.NameID( html );
  if ( style.LocalD( hDC ) ) return;
  //
  if ( m_pLinkS == NULL ) 
	  return;
  //"OnUpdate"     = eventOnUpdate     Установка параметров
  //"OnDataChange" = eventOnDataChange Для кнопок
  //"OnClick"      = eventOnClick      Нажатие на кнопках
  //"OnChange"     = eventOnChange     Изменение в EditBox-ах
  //"OnShapeLoad"  = eventOnShapeLoad;
  const char* name  = style.GetName (html);// SCRIPT
  const char* value = style.GetValue(html);// код
  if ( lstrcmpi( id, "ScData" ) == 0 )
    {
    int yy = 90;
    }

#ifdef _DEBUG
if ( lstrcmpi( id, "ProductCmbx_1" ) == 0 )
   AAA();
if ( lstrcmpi( id, "sd") == 0)
   AAA();
#endif

  if ( m_eType == eventOnUpdate || m_eType == eventOnDataChange || m_eType == eventOnPeriodIcupdate)
    mScript.Run();
  else 
  if(Version!=LG35_8_KF) 
    {
    if (html.m_nTimerCount == 1 && m_eType == eventOnPageComplete) //на 0 шаге данные еще не прочинаны с сервера
      mScript.Run();
    }
  }
//
int CTagSCRIPT::RunScript()
  {
  if ( m_eType == eventOnClick
    || m_eType == eventOnChange
    || m_eType == eventOnMouseDown
    || m_eType == eventOnMouseUp )
    return mScript.Run();
  else
    {
    ASSERT(0);
	return -1;
    }
  }
//
void CTag::Values(CHTML& html)
  {
  ASSERT( m_szScriptValue );
  double val = *(double*)m_szScriptValue;
  CStyle& style = Style(html);
  if ( TYPE_PNT == TYPE_TEXT )
		return;
  char* filtr = style.Filtr(html);
  if ( *filtr )
    if(strchr(filtr, 's'))
      sprintf_s( m_szScriptValue, 128, filtr,  m_szScriptValue);
    else sprintf_s( m_szScriptValue, 128, filtr, val );
  else
    sprintf_s( m_szScriptValue, 511, "%f", val );
  }
//
void CVBScript::Post()
  {
  //
  // Проверка целостности кода
  SCommand* comm = (SCommand*)mComm.m_szBuffer;
  UINT nComm = mComm.Count();
  for ( UINT n = 0; n < nComm; n++ )
    {
    switch ( comm[n].nOper )
      {
      case KEY_IF    :
      case KEY_ELSEIF:
      case KEY_THEN  :
        if ( comm[n].nArg1 == -1 )
          {
          comm[n].nArg1 = nComm;
          }
        break;
      }
    }
  GVAR* list = (GVAR*)mGVAR.m_szBuffer;
  UINT nCount = mGVAR.Count();
  int N = 0;
  for ( UINT n = 0; n < nCount; n++ )
    {
    GVAR& var = mGVAR.Obj( n );
    if ( var.nDef != VALUE_VALUE ) continue;
    //
    SValueValue* val = mGVAR.Value( n );
    CStyle& style = val->mTag->Style(*html);
    const char* mame  = mGVAR.Name (n);// SCRIPT
    const char* name  = style.GetName (*html);// SCRIPT
    ASSERT( lstrcmpi( name, "TEXTAREA" ) == 0 || lstrcmpi( name, "SPAN" ) == 0 || lstrcmpi( name, "DIV" ) == 0 );
    //
    //UINT nPlac = mGVAR.AddB(MAX_LOCAL);
    //val->Void = mBuff.m_szBuffer + nPlac;
    //val->mTag->m_szScriptValue = (char*)val->Void;
    }
  }
//
