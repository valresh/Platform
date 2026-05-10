#include "StdAfx.h"
#include "HTML.h"
#include "VBScript.h"
//
#include "../ARM.h"
#include "../AplicFrm.h"
//#include "../PopupMnemo.h"

#include <Profile.h>
#include "linuxTypes/CString.h"

#include <PopupMnemo.h>
//
void CVBScript::TestPrint(char* id)
  {
  char txt[256];
  _static HFILE hFile = _lcreat( "d:/Test.vb", 0 );
  _hwrite( hFile, id, lstrlen(id) );
  _hwrite( hFile, "\r\n", 2 );
  UINT nCount = mComm.Count();
  for ( UINT n = 0; n < nCount; n++ )
    {
    SCommand& c = Comm( n );
    EValueTypeScript ae;
    mGVAR.Var( *html, c.nRes, ae );

    sprintf_s( txt, sizeof(txt), "%4d %4d %4d %4d %4d", 
    c.nOper, c.nRes , c.nArg1, c.nArg2, c.nArg3 );
    sprintf_s( txt, sizeof(txt), "%3d(%d) ", n, int(ae)  );
    _hwrite( hFile, txt, lstrlen(txt) );
    switch ( c.nOper )
      {
      case KEY_IF      :sprintf_s( txt, sizeof(txt), "IF    %3d", c.nArg1 );break;
      case KEY_ELSEIF  :sprintf_s( txt, sizeof(txt), "ELSEIF%3d", c.nArg1 );break;
      case KEY_THEN    :sprintf_s( txt, sizeof(txt), "THEN  %3d", c.nArg1 );break;
      case KEY_ELSE    :lstrcpy( txt, "ELSE  " ); break;
      case KEY_END     :lstrcpy( txt, "END   " ); break;
      case OPER_OR     :lstrcpy( txt, "OR    " ); break;
      case OPER_MOD    :lstrcpy( txt, "MOD   " ); break;
      case OPER_SUB    :lstrcpy( txt, "-     " ); break;
      case OPER_ADD    :lstrcpy( txt, "+     " ); break;
      case OPER_DIV    :lstrcpy( txt, "/     " ); break;
      case OPER_DIV_INT:lstrcpy( txt, "\\    " ); break;
      case OPER_EQ     :lstrcpy( txt, "=     " ); break;
      case OPER_BEQ    :lstrcpy( txt, ">=    " ); break;
      case OPER_MEQ    :lstrcpy( txt, "<=    " ); break;
      case OPER_NEQ    :lstrcpy( txt, "<>    " ); break;
      case OPER_AND    :lstrcpy( txt, "&     " ); break;
      case OPER_MUL    :lstrcpy( txt, "*     " ); break;
      case OPER_MEN    :lstrcpy( txt, "<     " ); break;
      case OPER_BOL    :lstrcpy( txt, ">     " ); break;
      case OPER_EQ_EQ  :lstrcpy( txt, "==    " ); break;
      case FUNC_MSGBOX   :lstrcpy( txt, "MSGBOX" ); break;
      case FUNC_ABS      :lstrcpy( txt, "ABS   " ); break;
      case FUNC_CDBL     :lstrcpy( txt, "CDBL  " ); break;
      case FUNC_CINT     :lstrcpy( txt, "CINT  " ); break;
      case FUNC_CSTR     :lstrcpy( txt, "CSTR  " ); break;
      case FUNC_FORM_N   :lstrcpy( txt, "FormatNumber" ); break;
      case FUNC_ISNUMERIC:lstrcpy( txt, "ISNUME" ); break;
      case FUNC_CHR      :lstrcpy( txt, "CHR   " ); break;
      case FUNC_BRACE    :lstrcpy( txt, "BRACE " ); break;
      case FUNC_DATAVALUE:  lstrcpy( txt, "FUNC_DATAVALUE " ); break;
      case FUNC_SET_DATAVALUE: lstrcpy( txt, "FUNC_SET_DATAVALUE " ); break;
      case FUNC_GETELEMENTBYID: lstrcpy( txt, "FUNC_GETELEMENTBYID " ); break;   
      case FUNC_MR_Comment: lstrcpy( txt, "Call Subroutine " ); break;   
      case KEY_TO:  lstrcpy( txt, "TO " ); break;
      case KEY_NEXT: lstrcpy( txt, "NEXT " ); break;
      default: 
        sprintf_s( txt, "Unknown[%d]", c.nOper );
        break;
      }
    _hwrite( hFile, txt, lstrlen(txt) );
    //
    if ( c.nRes != -1 )
      {
      GVAR& v = mGVAR.Obj( c.nRes );
      const char* name = mGVAR.Name(c.nRes);
      sprintf_s( txt, sizeof(txt), " %16s", name );
      _hwrite( hFile, txt, lstrlen(txt) );
      }
    if ( c.nArg1 != -1 && !( c.nOper == KEY_IF || c.nOper == KEY_ELSEIF ) )
      {
      GVAR& v = mGVAR.Obj( c.nArg1 );
      const char* name = mGVAR.Name(c.nArg1);
      sprintf_s( txt, sizeof(txt), " %16s", name );
      _hwrite( hFile, txt, lstrlen(txt) );
      }
    if ( c.nArg2 != -1 )
      {
      GVAR& v = mGVAR.Obj( c.nArg2 );
      const char* name = mGVAR.Name(c.nArg2);
      sprintf_s( txt, sizeof(txt), " %16s", name );
      _hwrite( hFile, txt, lstrlen(txt) );
      }
    if ( c.nArg3 != -1 )
      {
      GVAR& v = mGVAR.Obj( c.nArg3 );
      const char* name = mGVAR.Name(c.nArg3);
      sprintf_s( txt, sizeof(txt), " %16s", name );
      _hwrite( hFile, txt, lstrlen(txt) );
      }
    _hwrite( hFile, "\r\n", 2 );
    }
  _hwrite( hFile, "\r\n", 2 );
  }
//
bool CVBScript::Compile( char* txt, UINT* pRes, bool bLogic )
  {
  SUniToken uni;
  if ( !VBScriptScan( *html, uni, mLink, txt, pRes == NULL ) )
    return false;
  //
  int nCount = uni.Count();
  SToken* token = (SToken*)uni.m_szBuffer;
  if ( nCount == 0 )
    return false;
  //
  //
#ifdef _DEBUG
#if 0
  {
  _static HFILE hFile = _lcreat( "d:/script.txt", 0 );
  _hwrite( hFile, "----------------------------------------", 30 );
  _hwrite( hFile, "\r\n", 2 );
  int nCount = uni.Count();
  char TAB[64]="";
  bool bEnd= false;
  for ( int n = 0; n < nCount; n++ )
    {
    SToken& tk = uni.Obj(n);
    ASSERT( tk.txt );
    char txt[512];
    sprintf_s(txt, "%3d) %s%s\r\n", n, TAB, tk.txt); 
    _hwrite( hFile, txt, lstrlen(txt) );
    if(!bEnd && tk.dw == KEY_IF)
    {
       strcat_s(TAB, "~");
       bEnd = false;
    }
    else 
    if( bEnd && tk.dw == KEY_IF )
    {
       size_t len = strlen(TAB);
       if(len > 0)
         TAB[len-1] = 0;
       else TAB[0] = '?'; //что то не так
       bEnd = false;
    }
    else 
    if(tk.dw == KEY_END)
      bEnd = true;
    else bEnd = false;
    }
  }
#endif
#endif
  // Строим список переменных
  if ( !BuildTable( uni ) )
    return false;
  //
  if ( nCount == 1 && pRes )
    {
    if ( IsFunc(token[0]) )
      *pRes = AddArgument( token[0], bLogic );
    else
      *pRes = token[0].id;
    return false;
    }
  //
  // Строим список команд
  if ( pRes )
    {
    if ( !VBArgumentis( token, nCount ) )
      return false;
    }
  else
    {
    if ( VBScriptList( token, nCount, bLogic ) == -1 )
      return false;
    }
  return true;
  }
//
bool CVBScript::ParamValue( cross::SParamValueH& val, SToken& token )
  {
  bool b;
  char* value = token.txt;
  CTag* pTag = token.mTag;
  b = value != NULL;
  if ( AS(b) ) return false;
  //
  int dl = lstrlen( value );
  b = dl > 2;
  if ( AS(b) ) return false;
  //
  b = value[0] == '"' && value[dl-1] == '"';
  std::string nval;
  if ( AS(b) )
  {
      nval = html->FindInDisplayDataRepository(value, true);
      if(nval.size() == 0)
        return false;
      nval = "\"" + nval + "\"";
      value = (char*)nval.c_str();
      dl = lstrlen( value );
  }
  //
  char* p = strchr( value+1, '.' );
  if ( p == NULL ) 
     return false;
  value[dl-1] = 0, *p = 0;
  const char* aa = Alias( *html, pTag, value+1 );
  const char* bb = Alias( *html, pTag, p+1 );
  
  bool nFnd = true;
  if((Version == T1163_ONPZ || Version == T1165P_ONPZ)
	&& token.dw == PARAM_ALARMSEVERITY && strcmp(bb,"PV") == 0)
	{
		const char* bb2 = "AlarmSeverity"; 
		val = ::FindExperion( *html, aa, bb2 );
		nFnd = val.nNumber == -2;
		if(!nFnd)
		{
		    GVAR& var = mGVAR.Obj(token.id);
			var.nDef = token.dw = PARAM_DATAVALUE;
		}
	} 
  if(_strnicmp( aa, "{%CurrentPoint%}", sizeof("{%CurrentPoint%}")-1)==0)
   {
      const char* aa2 = html->Group().GetBuffer();
      val = ::FindExperion( *html, aa2, bb );
	  nFnd = val.nNumber == -2;
   }
   
  if ( nFnd )	
	val = ::FindExperion( *html, aa, bb );
	
  if ( val.nNumber == -2 ) // Не обнаружен в базе
    AAA();
  //
  value[dl-1] = '"', *p = '.';
  if ( val.def == NULL )//Sergej24
    return false;
  //
  b = val.def != NULL;
  if ( AS(b) ) return false;
  //
  return true;
  }
//
bool CVBScript::ParamValue( cross::SParamValueH& val, char* value, CTag* pTag )
  {
  bool b;
  b = value != NULL;
  if ( AS(b) ) return false;
  //
  int dl = lstrlen( value );
  b = dl > 2;
  if ( AS(b) ) return false;
  //
  b = value[0] == '"' && value[dl-1] == '"';
  if ( AS(b) ) return false;
  //
  char* p = strchr( value+1, '.' );
  if ( AS(b) ) return false;
  value[dl-1] = 0, *p = 0;
  const char* aa = Alias( *html, pTag, value+1 );
  const char* bb = Alias( *html, pTag, p+1 ); 
  if(_strnicmp( aa, "{%CurrentPoint%}", sizeof("{%CurrentPoint%}")-1)==0)
   {
      const char* aa2 = html->Group().GetBuffer();
      val = ::FindExperion( *html, aa2, bb );
   }
  else val = ::FindExperion( *html, aa, bb );
  if ( val.nNumber == -2 ) // Не обнаружен в базе
  {
      std::string nbb = html->FindInDisplayDataRepository(bb, true);
      if(nbb.size() > 0)
      {
      bb = nbb.c_str();
      val =  ::FindExperion( *html, aa, bb ); 
      }
  }
  //
  value[dl-1] = '"', *p = '.';
  if ( val.def == NULL )//Sergej24
    return false;
  //
  b = val.def != NULL;
  if ( AS(b) ) return false;
  //
  return true;
  }
//
bool CVBScript::ValueWindow( UINT dw, SValueValue& val )
  {
  val.mTag = NULL;
  //
  _static bool b;
  cross::CWnd* par = html->mWnd->GetParent();
  CRuntimeClass* cls = par->GetRuntimeClass();
  //
  switch ( dw )
    {
    case WINDOW_EXT_CLOSE   :
      val.eVal = enumValueBol;
      if ( cls != NULL && lstrcmp( cls->m_lpszClassName, "CPopupMnemo" ) == 0 )
        {
        CPopupMnemo* p = (CPopupMnemo*)par;
        val.Void = &p->mCloseWnd;
        }
      else
        {
        val.Void = &b;
        }
      break;
    case WINDOW_EXT_S_LEVEL :
      val.eVal = enumValueInt;
      val.Void = &theApp.m_nLevelSec;
      break;
    case WINDOW_EXT_CURRPAGE:
	  val.eVal = enumValueStr;
	  val.Void = html->WINDOWS_EXTERNAL_CURRENTPAGE;
      break;
    case WINDOW_EXT_TMZONE_T:
      val.eVal = enumValueStr;
      val.Void = &theApp.m_szTempMessZoneText;
      break;
    default:
      ASSERT(0);
      return false;
    };
  return true;
  }
//
bool CVBScript::Add( SToken& token, const char* pszName )
  {
  //
  UINT n = mGVAR.AddN(pszName);
  if ( As(n) ) return false;
  GVAR& var = mGVAR.Obj(n);
  token.id = n;
  var.nDef = token.dw;
  //
  void*v = NULL;
  int         i;
  double      d;
  DATA_CHANGED p;
  SValueValue u;
  SCTag sctag;
  SCTag cprop;
  DATA_CHANGED dc;
  if ( IsConst(token) )
    {
    var.nType = TYPE_INT;
    var.nSize = sizeof(int);
    v = &token.nVar;
    }
  else
  if ( IsParam(token) )
    {
    ASSERT( token.mTag );
    if ( token.mTag == NULL )
      return false;
    if(_strnicmp( token.txt, "\"CurrentPoint", sizeof("\"CurrentPoint")-1 )==0)
      {
      cross::CString strTemp=token.txt;
      cross::CString strTemp2 = strTemp.Mid(1, sizeof("\"CurrentPoint")-2);
      strTemp.Replace(strTemp2, html->Group().GetBuffer());
      if(LG35_8_KF == Version)
       {
       //заплатка: нет блоков - заменяю на то, откуда получаются значения точек
       if (token.txt[sizeof("\"CurrentPoint")-1]=='.')
        {
        char tmp[128]="";
        strcpy_s(tmp, token.txt+ sizeof("\"CurrentPoint"));
        if(strncmp(tmp, "LSL1.LSCTRL", sizeof("LSL1.LSCTRL")-1) == 0) 
           strTemp.Replace("LSL1.LSCTRL", "LSL1ACTIVE.PVFL");
        else
        if(strncmp(tmp, "LSL2.LSCTRL", sizeof("LSL2.LSCTRL")-1) == 0) 
           strTemp.Replace("LSL2.LSCTRL", "LSL2ACTIVE.PVFL");
        else
        if(strncmp(tmp, "LSL3.LSCTRL", sizeof("LSL3.LSCTRL")-1) == 0) 
           strTemp.Replace("LSL3.LSCTRL", "LSL3ACTIVE.PVFL");
        }
       }
      if ( !ParamValue( p, strTemp.GetBuffer(), token.mTag ) )
        return false;
      }
    else
    if(_strnicmp( token.txt, "\"Point", sizeof("\"Point")-1 )==0)
      {
      cross::CString strTemp=token.txt;
      char fname[32]="";
      strncpy_s(fname, token.txt+1, sizeof("Point")-1);
      fname[sizeof("Point")-1]=0;
      const char *tag = Alias( *html, token.mTag, fname);
      strTemp.Replace(fname, tag); 
      if ( !ParamValue( p, strTemp.GetBuffer(), token.mTag ) )
        return false;
      }
    else
    //if ( !ParamValue( p, token.txt, token.mTag ) )
    if ( !ParamValue( p, token ) )
    {
      if(Version==DOTF_PM) 
      { //параметры ARAM_DV_TAGNAME.CP_ALMx  могут быть не заполнены для shape, что по-видимому, означает что эти  алармы не учитывать
        if( _strnicmp(token.txt+1, "TAGNAME.CP_ALM", _countof("TAGNAME.CP_ALM")-1)==0 )
          {
          token.dw = VAR_INTEGER;
          var.nType = TYPE_INT;
          var.nSize = sizeof(int);
          i = 1;
          v = &i;
          }
        else
        if(_stricmp(token.txt+1, "TAGNAME.CP_ALARM\"" )==0)
          {
          token.dw = VAR_INTEGER;
          var.nType = TYPE_INT;
          var.nSize = sizeof(int);
          i = 1;
          v = &i;          
          }
        else 
        if( _stricmp(token.txt, "\"HZS2100.HS.PVFL\"")==0 )
          {
          token.dw = VAR_INTEGER;
          var.nType = TYPE_INT;
          var.nSize = sizeof(int);
          i = 0;
          v = &i;          
          }
        else
        return false;
      }
      else
      if(Version==T1163_ONPZ)
      {
         if( _strnicmp(token.txt+1, "DI_LL.PV", _countof("DI_LL.PV")-1)==0 )
          {
          token.dw = VAR_INTEGER;
          var.nType = TYPE_INT;
          var.nSize = sizeof(int);
          i = 0;
          v = &i;          
          }
         else
         if( _strnicmp(token.txt+1, "ANALOG_BAR.AlarmLimit", _countof("ANALOG_BAR.AlarmLimit")-1)==0 )
          {
          token.dw = VAR_DOUBLE;
          var.nType = TYPE_FLOAT;
          var.nSize = sizeof(d);
          d = NaN;
          v = &d;
          }
         else
         if( _strnicmp(token.txt+1, "ANALOG_BAR.AlarmDisabled", _countof("ANALOG_BAR.AlarmDisabled")-1)==0 )
          {
          token.dw = VAR_INTEGER;
          var.nType = TYPE_INT;
          var.nSize = sizeof(int);
          i = 0;
          v = &i;          
          }
         else
         return false;
      }
      else
      return false;
    }
    if ( IsParam(token) ) //т.к. заплатка выше для DOTF_PM может поменять
    {
    var.nType = TYPE_PARAM;
    var.nSize = sizeof p;
    v = &p;
    if (p.pMulti!=NULL)
      {
       if(m_vParams.size()==0)
          m_vParams.reserve(128);
       else if(m_vParams.size()>=128)
               {
               p.pMulti = NULL;
               ASSERT(0);   //здесь память нельзя еще раз величивать т.к. элементы сохранены в mGVAR c pMulti, созданными при первом занесениии 
               }
       m_vParams.push_back(p);
       v = (void*)&m_vParams[m_vParams.size()-1];
      }
    }
    }
  else
  if ( IsValue(token) )
    {
    if(VALUE_OBJECT == token.dw)
    {
        strcpy_s(cprop.m_name, token.txt);
        var.nType = TYPE_OBJECT; 
        var.nSize = sizeof(cprop);
        v=&cprop;
    }
    else
    if(VALUE_CUSTOMPROPERTY == token.dw)
    {
        strcpy_s(cprop.m_name, token.txt);
        var.nType = TYPE_CUSTOMPROPERTY; 
        var.nSize = sizeof(cprop);
        v=&cprop;
    }
    else
    if(VALUE_TAG == token.dw)
    {
        strcpy_s(sctag.m_name, token.txt);
        sctag.m_pTag = token.mTag;
        var.nType = TYPE_TAG; 
        var.nSize = sizeof(sctag);
        v = &sctag;
    }
    else
    if(VALUE_WINDOWEVENT  == token.dw)
    {
        strcpy_s(sctag.m_name, token.txt);
        sctag.objectType = otWindowEvent;
        var.nType = TYPE_OBJECT; 
        var.nSize = sizeof(sctag);
        v = &sctag;
    }
    else
    {
    ASSERT( token.mTag );
    if ( token.mTag == NULL )
      return false;
    if ( !token.mTag->ValueValue( *html, token.dw, u ) )
      return false;
    var.nType = TYPE_VALUE;
    var.nSize = sizeof(SValueValue);
    v = &u;
    }
    }
  else
  if ( IsWnd(token) )
    {
    if ( !ValueWindow( token.dw, u ) )
      return false;
    var.nType = TYPE_VALUE;
    var.nSize = sizeof(SValueValue);
    v = &u;
    }
  else
  if ( VAR_INTEGER == token.dw )
    {
    var.nType = TYPE_INT;
    var.nSize = sizeof(int);
    i = atoi( token.txt );
    v = &i;
    }
  else
  if ( VAR_DOUBLE == token.dw )
    {
    var.nType = TYPE_FLOAT;
    var.nSize = sizeof(double);
    d = atof( token.txt );
    v = &d;
    }
  else
  if ( VAR_STRING == token.dw )
    {
    var.nType = TYPE_STRING;
    var.nSize = lstrlen(token.txt)+1;
    v = token.txt;
    }
  else
  if(KEY_ARRAYELEMENT  == token.dw)
    {
        token.dw = VALUE_OBJECT;
        strcpy_s(sctag.m_name, token.txt);
        sctag.objectType = otARRAY;
        var.nType = TYPE_OBJECT; 
        var.nSize = sizeof(sctag);
        v = &sctag;
    }
  else
    {
    var.nType = TYPE_LOCAL;
    var.nSize = MAX_LOCAL;
    //var.nPlac = -1;
    //return true;
    }
  //
  var.nPlac = mGVAR.AddB(var.nSize);
  if ( As(var.nPlac) ) return false;
  //
  char* buff = mGVAR.Chars(n);
  if ( v )
    memcpy( buff, v, var.nSize );
  else
    memset( buff, 0, var.nSize );
  if ( lstrcmpi( pszName, "LOCAL_A" ) == 0 )
    {
    *buff = 1;
    }
  return true;
  }
//
bool CVBScript::BuildTable( SUniToken& uni )
  {
  char txt[256];
  int nCount = uni.Count();
  if ( nCount == 0 ) return false;
  SToken* token = (SToken*)uni.m_szBuffer;
  for ( int n = 0; n < nCount; n++ )
    {
    token[n].id = -1;
    if ( IsKey (token[n]) && token[n].dw != KEY_DIM && token[n].dw != KEY_ARRAY && token[n].dw != KEY_ARRAYELEMENT)
      continue;
    if ( IsOper(token[n]) )
      continue;
    if ( IsFunc(token[n]) )
      continue;
    //if ( IsWnd(token[n]) )
    //  continue;
    //
    if ( IsConst(token[n]) )
      lstrcpy( txt, "CONST_" );
    else
    if ( IsParam(token[n]) )
      {
      if ( token[n].dw == PARAM_QUALITYBAD )
        {
        lstrcpy( txt, "PARAM_QB_" );
        }
      else
      if ( token[n].dw == PARAM_QUALITYGOOD )
        {
        lstrcpy( txt, "PARAM_QG_" );
        }
      else
      if ( token[n].dw == PARAM_DATAVALUE )
        {
        lstrcpy( txt, "PARAM_DV_" );
        }
      else
      if ( token[n].dw == PARAM_DATACHANGED )
        {
        lstrcpy( txt, "PARAM_DC_" );
        }
      else
      if ( token[n].dw == PARAM_ISNAN )
        {
        lstrcpy( txt, "PARAM_IN_" );
        }
      else
      if ( token[n].dw == PARAM_DATAEXIST )
        {
        lstrcpy( txt, "PARAM_DATAEXIST" );
        }
      else
        {
        ASSERT(0);
        lstrcpy( txt, "PARAM_" );
        }
      }
    else
    if ( IsValue(token[n]) )
      lstrcpy( txt, "VALUE_" );
    else
    if ( IsWnd(token[n]) )
      lstrcpy( txt, "WINDOW_" );
    else
    if ( VAR_INTEGER == token[n].dw )
      lstrcpy( txt, "INT_" );
    else
    if ( VAR_DOUBLE == token[n].dw )
      lstrcpy( txt, "DBL_" );
    else
    if ( VAR_STRING == token[n].dw )
      lstrcpy( txt, "STR_" );
    else
    if ( token[n].dw == KEY_DIM )// Объявление локальных переменных
      {
        lstrcpy( txt, "LOCAL_" );
      }
    else
    if ( token[n].dw == KEY_ARRAY )// Объявление локального массива
      {
        lstrcpy( txt, "LOCALARRAY_" );
        if(token[n].nVar > 0)
        {            
            for(int ii=0; ii<token[n].nVar; ii++)
            {
                char arname[256];
                sprintf_s(arname, "LOCALARRAY_%s(%d", token[n].txt, ii);
                ::CharUpper( arname );
                SToken atoken;
                atoken.dw = KEY_DIM;
                if(mGVAR.Find( arname )==-1)
                {
                    Add( atoken, arname);
                }
            }
            continue;
        }
        AAA(); continue;
      }
    else
    if ( token[n].dw == KEY_ARRAYELEMENT )
        {
          char* pp = strchr(token[n].txt, '(');
          if(pp==NULL)
            return false;  
          char arname0[256];
          strncpy_s(arname0, token[n].txt, pp-token[n].txt);
          ::CharUpper( arname0 );
          if(IsInt(pp+1))
          {
              char arname[256];
              sprintf_s(arname, "LOCALARRAY_%s(%d", arname0, atoi(pp+1));
              UINT mm = mGVAR.Find( arname );
              if ( mm != -1 )
              {
                token[n].id = mm;
                token[n].dw = 0;
                continue;
              }   
              lstrcpy( txt, arname);              
          }
          else lstrcpy( txt, "ARRAYELEMENT_" );
        }
    else
    if ( token[n].dw == 0 )
      lstrcpy( txt, "LOCAL_" );
    else
      {
      lstrcpy( txt, "UNKNOWN_" );
      ASSERT(0);
      }
    //
    if ( *token[n].txt == '"' )
      {
      int dl = lstrlen( token[n].txt+1 );
      ASSERT( token[n].txt[dl] == '"' );
      token[n].txt[dl] = 0;
      lstrcat( txt, token[n].txt+1 );
      token[n].txt[dl] = '"';
      }
    else
      strcat_s( txt, token[n].txt );
    //
    ::CharUpper( txt );
    //
    _static int A = 0;A++;
    if ( A == 0 )
      KKK();
    UINT m = mGVAR.Find( txt );
    // Уже проинициализировали
    if ( m != -1 )
      {
      token[n].id = m;
      continue;
      }
    if ( !Add( token[n], txt ) )
      {
		  if(token[n].dw == PARAM_DATACHANGED)
		  {
			token[n].dw = VAR_INTEGER;
			token[n].txt = "1";
			if ( Add( token[n], txt ) )
			   continue;
		  }
		  else
		  if(token[n].dw == PARAM_QUALITYBAD)
		  {
			token[n].dw = VAR_INTEGER;
			token[n].txt = "1";
			if ( Add( token[n], txt ) )
			   continue;
		  }
          else
		  if(token[n].dw == PARAM_QUALITYGOOD)
		  {
			token[n].dw = VAR_INTEGER;
			token[n].txt = "0";
			if ( Add( token[n], txt ) )
			   continue;
		  }
          else
          if ( token[n].dw == PARAM_DATAEXIST )
          {
            token[n].dw = VAR_INTEGER;
			token[n].txt = "0";
			if ( Add( token[n], txt ) )
			   continue;
          }
          else
		  if(token[n].dw ==  PARAM_DATAVALUE)
		  {
            if(Version == BERTH_ONPZ)
            {
              if(_stricmp(token[n].txt, "strCtlOI")==0)
              {
			  token[n].dw = VAR_INTEGER;
			  token[n].txt = "0";
			  if ( Add( token[n], txt ) )
			   continue;
              }
              else
              if(_stricmp(token[n].txt, "strCtlSt2Cmd")==0)
              {
			  token[n].dw = VAR_INTEGER;
			  token[n].txt = "0";
			  if ( Add( token[n], txt ) )
			   continue;
              }
              else
              if(PARAM_DATAVALUE==token[n].dw && _stricmp(token[n].txt, "\"E_Domain.DomainOperatorLogin\"")==0)
              {
                strcpy_s(txt, "LOCAL_E_DOMAIN.DOMAINOPERATORLOGIN");
			    token[n].dw = 0;
			    token[n].txt = "E_Domain.DomainOperatorLogin";
                UINT mm = mGVAR.Find( token[n].txt );
                if ( mm != -1 )
                {
                  token[n].id = mm;
                  continue;
                }
			    if ( Add( token[n], txt ) )
			      continue;
              }
            }
			if( (Version == T1163_ONPZ  && html && strnicmp(html->m_strFile, "TSB_NASOS_1163_ZAGAZ_GR_1", 20)==0)
			 || (Version == T1165P_ONPZ && html && strnicmp(html->m_strFile, "TSB_NASOS_1165_ZAGAZ_GR_4", 20)==0))
			{
			  token[n].dw = VAR_INTEGER;
			  token[n].txt = "0";
			  if ( Add( token[n], txt ) )
			   continue;
			}
		  }
		  return false;
      }
    }
  return true;
  }
//
UINT CVBScript::AddMeas( SCommand& comm, bool bLogic )
  {
  EValueTypeScript e = _enumValueUnk;
  switch ( comm.nOper )
    {
    case OPER_BEQ  :
    case OPER_MEQ  :
    case OPER_NEQ  :
    case OPER_MEN  :
    case OPER_BOL  :
    case OPER_IS:
    case OPER_EQ_EQ: e = _enumValueBol; break;
    case OPER_EQ:
      ASSERT(0);
    default:
      {
      EValueTypeScript a,b;
      mGVAR.Var( *html, comm.nArg1, a );
      mGVAR.Var( *html, comm.nArg2, b );
      if ( a == _enumValueStr && b == _enumValueStr ) e = a;
      else
      if ( a == _enumValueDbl && b == _enumValueInt ) e = a;
      else
      if ( a == _enumValueInt && b == _enumValueDbl ) e = b;
      else
      if ( a == _enumValueInt && b == _enumValueChr ) e = a;
      else
      if ( a == _enumValueBol && b == _enumValueChr ) e = a;
      else
      if ( a == _enumValueObjects && b == _enumValueObjects ) 
          e = a;
      else
      if ( a == _enumValueVariant && b != _enumValueUnk && b <= _enumValueStr ) 
          e = b;
      else
        {
        ASSERT( a == b );
        e = a;
        }
      }
    }
  //
  UINT nType = 0;
  switch ( e )
    {
    case enumValueDbl: nType = TYPE_FLOAT ;break;
    case enumValueInt: nType = TYPE_INT   ;break;
    case enumValueChr:
    case enumValueBol: nType = TYPE_BOOL  ;break;
    case enumValueStr: nType = TYPE_STRING;break;
    case _enumValueObjects: nType = TYPE_OBJECT; break;
    default:
      ASSERT(0);
      return -1;
    };
  return AddTemp( nType );
  //
  }
//
UINT CVBScript::AddTemp( UINT nType )
  {
  //
  char szName[128];
  switch ( nType & 0xf )
    {
    case TYPE_BOOL  : sprintf_s( szName, sizeof(szName), "__TEMP_BOOL__%d", m_dwCurr++ ); break;
    case TYPE_INT   : sprintf_s( szName, sizeof(szName), "__TEMP_INT___%d", m_dwCurr++ ); break;
    case TYPE_FLOAT : sprintf_s( szName, sizeof(szName), "__TEMP_FLOAT_%d", m_dwCurr++ ); break;
    case TYPE_STRING: sprintf_s( szName, sizeof(szName), "__TEMP_STRNG_%d", m_dwCurr++ ); break;
    case TYPE_OBJECT: sprintf_s( szName, sizeof(szName), "__TEMP_OBJECT_%d", m_dwCurr++ ); break;
    case TYPE_VALUE:  sprintf_s( szName, sizeof(szName), "__TEMP_VALUE_%d", m_dwCurr++ ); break;
    case TYPE_UNKNOWN:sprintf_s( szName, sizeof(szName), "__TEMP_VARIANT_%d", m_dwCurr++ ); break; //пока не неизвесто какого типа получится
    default:
      ASSERT(0);
      return -1;
    };
  //
  UINT n = mGVAR.AddN( szName );
  if ( As(n) )
    return -1;
  if ( AS(n+1 == mGVAR.Count()) )
    return -1;
  //
  GVAR& var = mGVAR.Obj(n);
  var.nType = nType;
  switch ( nType & 0xf )
    {
    case TYPE_BOOL  : var.nSize = sizeof(bool);break;
    case TYPE_INT   : var.nSize = sizeof(int);break;
    case TYPE_FLOAT : var.nSize = sizeof(double);break;
    case TYPE_UNKNOWN:
    case TYPE_STRING: var.nSize = MAX_LOCAL; break;
    case TYPE_PARAM : var.nSize = sizeof(cross::SParamValueH);break;
    case TYPE_VALUE : var.nSize = sizeof(SValueValue);break;
    case TYPE_OBJECT: var.nSize = sizeof(SCTag); break;
    };
  var.nPlac = mGVAR.AddB(var.nSize);
  if ( As(var.nPlac) ) return -1;
  //
  return n;
  }
//
UINT CVBScript::AddComm( SCommand& comm )
  {
  if ( !mComm.Realloc(1) )
    return -1;
  UINT n = mComm.AddCurr();
  SCommand& a = mComm.Obj(n);
  memcpy( &a, &comm, sizeof(SCommand) );
  return n;
  }
//
char* MyStrChr2( char* &str, char chr)
{
    char* str0 = str;
    int nBrace = 0;
    bool bQuote = false;
    while(*str)
    {
        if(*str=='(')
          nBrace++;
        else
        if(*str==')')
          nBrace--;
        else
        if(*str=='"')
          bQuote = !bQuote;
        else 
        if(nBrace==0 && !bQuote && *str==chr)
        {
          *str++ = 0;
          break;
        }
        str++;
    }
    return str0;
}
//
int CVBScript::GetArgList( char* ptr, char** arg, int nMax )
  {
  int n = 0;
  do
    {
    if ( n == nMax ) 
        return n;
    arg[n] = MyStrChr2( ptr, ',' );
    if(arg[n][0] == 0)
      break;
    }
  while ( arg[n++][0] );
  return n;
  }
//
int CVBScript::FillArgList( const char* text, UINT* arg, int nMax )
  {
  cross::CString str = text;
  char* carg[16];
  memset( carg, 0, sizeof(carg) );
  int nArg = GetArgList( str.GetBuffer(), &carg[0], nMax );
  if ( nArg == -1 )
    return -1;
  for ( int n = 0; n < nArg; n++ )
    {
    while ( *carg[n] == ' ' ) carg[n]++;
    if ( *carg[n] )
      {
      bool b = Arguments( carg[n], arg[n], false );
      if ( As(b) )
        return -1;
      }
    }
  return nArg;
  }
//
UINT CVBScript::CommandFunc( SToken& token, int nMax, UINT nReturn )
  {
  if ( As(token.txt != NULL) ) return -1;
  //
  SCommand a;
  a.nOper = token.dw;//Операция
  UINT* arg = &a.nArg1;
  SToken *token1= &token;
  if(token1[1].dw == FUNC_BRACE)
    {
    if ( As(token1[1].txt != NULL) ) return -1;
    if(token1[0].dw == OPER_NOT)
      {
      int nArg = FillArgList( token1[1].txt, arg, nMax );
      a.nRes = AddTemp( nReturn );
      UINT nAdd = AddComm( a );
      }
    else
      {
    UINT* arg = &a.nRes;
    int nArg = FillArgList( token1[1].txt, arg, nMax );
    UINT nAdd = AddComm( a );
      }
    return a.nRes;
    }
  int nArg = FillArgList( token.txt, arg, nMax );
  a.nRes = AddTemp( nReturn );
  UINT nAdd = AddComm( a );
  if ( nAdd==-1 )
      return -1;
  return a.nRes;
  }
//
UINT CVBScript::AddArgument( SToken& token, bool bLogic )
  {
  UINT nRes = -1;
  switch ( token.dw )
    {
    case FUNC_MSGBOX   : nRes = FillMSGBOX   ( token ); break;
    case FUNC_ABS      : nRes = FillABS      ( token ); break;
    case FUNC_CDBL     : nRes = FillFunction ( token, TYPE_FLOAT  ); break;
    case FUNC_CINT     : nRes = FillFunction ( token, TYPE_INT    ); break;
    case FUNC_CSTR     : nRes = FillFunctionL ( token, 1, TYPE_STRING ); break;
    case FUNC_ISNUMERIC: nRes = FillFunction ( token, TYPE_BOOL   ); break;
    case FUNC_ISNAN    : nRes = FillFunction ( token, TYPE_BOOL   ); break;
    case FUNC_TRIM     : nRes = FillFunction ( token, TYPE_STRING ); break;
    case FUNC_LTRIM    : nRes = FillFunction ( token, TYPE_STRING ); break;
    case FUNC_RTRIM    : nRes = FillFunction ( token, TYPE_STRING ); break;
    case FUNC_MID      : nRes = FillFunctionL( token, 3, TYPE_STRING ); break;
    case FUNC_LEFT     : nRes = FillFunctionL( token, 2, TYPE_STRING ); break;
    case FUNC_RIGHT    : nRes = FillFunctionL( token, 2, TYPE_STRING ); break;
    case FUNC_INSTR    : nRes = FillFunctionL( token, 3, TYPE_INT );    break;
    case FUNC_LEN      : nRes = FillFunctionL( token, 1, TYPE_INT );    break;
    case FUNC_CHR      : nRes = FillFunction ( token, TYPE_STRING ); break;
    case FUNC_TYPENAME : nRes = FillFunction ( token, TYPE_STRING ); break;
    case FUNC_VARTYPE  : nRes = FillFunction ( token, TYPE_INT ); break;
    case FUNC_GETELEMENTBYID  : nRes = FillFunction ( token, TYPE_OBJECT ); break;
    case FUNC_BRACE    : nRes = FillBRACE    ( token, bLogic ); break;
    case FUNC_FORM_N   : nRes = FillFormatNum( token ); break;
    case FUNC_CREATEPOPUOWINDOW: nRes = FillFunctionL( token, 5, TYPE_BOOL ); break;//FillCreatePopupWindow( token ); break;
    case FUNC_LOGMESSAGE: nRes = FillFunction ( token, TYPE_BOOL   ); break;
    case FUNC_GETCUSTOMPROPERTY: nRes = FillGetcustomproperty( token ); break;
    case KEY_TO:          nRes = CommandFunc( token, 4, TYPE_INT ); break;
    case FUNC_CloseAllWindows: nRes = FillFuncWithEmptyParam( token, TYPE_BOOL ); break;
    case FUNC_DATAVALUE  : nRes = FillFunDataValue( token, TYPE_UNKNOWN ); break;
    case FUNC_CSNG      : nRes = FillFunction ( token, TYPE_FLOAT ); break;
    case FUNC_LCASE     : nRes = FillFunction ( token, TYPE_STRING ); break;
    case FUNC_UCASE     : nRes = FillFunction ( token, TYPE_STRING ); break;
    case FUNC_ROUND:      nRes = FillFunction ( token, TYPE_STRING ); break;
    case FUNC_EVAL      : nRes = FillFunction ( token, TYPE_INT ); break;
    case FUNC_REPLACE   : nRes = FillFunctionL( token, 5, TYPE_STRING );break;
    case FUNC_STRCOMP   : nRes = FillFunctionL( token, 3, TYPE_INT );break;
    case FUNC_page_GetStyleClassProperty: nRes = FillFunctionL( token, 2, TYPE_STRING ); break;
    case FUNC_FindPopup:  nRes = FillFunction ( token, TYPE_OBJECT ); break;
    case FUNC_CLOSE_POPUP: AAA(); break;
    case FUNC_page_stylesheetSrc: nRes = FillFuncWithEmptyParam( token, TYPE_STRING ); break;
    case FUNC_DisplayDataRepositoryGetValue: nRes = FillFunction ( token, TYPE_STRING ); break;
    case FUNC_DisplayDataRepositoryPutValue: nRes = FillFunctionL( token, 2, TYPE_BOOL ); break;
    case FUNC_MR_Comment   : nRes = FillFunction( token, TYPE_INT );break;
    case FUNC_PAGE_GETATTRIBUTE: nRes = FillFunction( token, TYPE_OBJECT ); break;
    case FUNC_PAGE_SETATTRIBUTE: nRes = FillFunctionL( token, 2, TYPE_BOOL ); break;
    case FUNC_ISNULL : nRes = FillFunction( token, TYPE_BOOL ); break;
    case FUNC_AlarmStateIconfactory : nRes = FillAlarmStateIconfactory( token ); break;
    case FUNC_GETATTRIBUTE: nRes = FillGetAttribute( token ); break;
    case FUNC_SETATTRIBUTE: nRes = FillSetAttribute( token ); break;
    case FUNC_SecurityLevel: nRes = FillFunction( token, TYPE_INT );break;
    case FUNC_SET_DATAVALUE: nRes = FillFunctionL( token, 2, TYPE_INT ); break;
    default:
    if ( IsKey(token) || IsOper(token) || IsFunc(token) )
      {
      ASSERT(0);
      AAA();
      }
    else
      nRes = token.id;
    break;
    }
  //Sergej As(nRes);
  return nRes;
  }
//
inline UINT Inv( EValueType t )
  {
  switch ( t )
    {
    case enumValueDbl: return TYPE_FLOAT;
    case enumValueInt: return TYPE_INT;
    case enumValueChr: return TYPE_INT;
    case enumValueBol: return TYPE_BOOL;
    case enumValueStr: return TYPE_STRING;
    }
  return 0;
  }
//
bool CVBScript::EqType( UINT a1, UINT a2 )
  {
  GVAR& v1 = mGVAR.Obj(a1);
  GVAR& v2 = mGVAR.Obj(a2);
  UINT t1 = v1.nType&0xf;
  UINT t2 = v2.nType&0xf;
  if ( t1 == t2 ) return true;
  //
  if ( t1 == TYPE_PARAM )
    {
    cross::SParamValueH* v = mGVAR.Param(a1);
    if ( v->def != NULL )
      t1 = Inv( v->def->eVal );
    }
  if ( t2 == TYPE_PARAM )
    {
    cross::SParamValueH* v = mGVAR.Param(a2);
    if ( v->def != NULL )
      t2 = Inv( v->def->eVal );
    }
  //
  if ( t1 == TYPE_VALUE )
    {
    SValueValue* v = mGVAR.Value(a1);
    t1 = Inv( v->eVal );
    }
  if ( t2 == TYPE_VALUE )
    {
    SValueValue* v = mGVAR.Value(a2);
    t2 = Inv( v->eVal );
    }
  //
  if ( t1 == t2 ) return true;
//  ASSERT(0);
  return true;
  }
//
UINT CVBScript::AddCommand( UINT nOper, UINT nArg1, SToken& token, UINT* pRes, bool bLogic )
  {
  if ( As(nArg1) ) return -1;
  //
  SCommand a;
  a.nOper = nOper;//Операция
  if ( pRes != NULL )
    {
    a.nArg1 = nArg1;//Первый аргумент
    if(token.dw == OPER_NOT)
      {
        SToken *token1 = &token+1;
        UINT nNotArg = AddArgument(*token1, true);
        if(As(nNotArg))
          return -1;
        SCommand a0;
        a0.nOper = token.dw;//Операция
        a0.nArg1 = nNotArg;
        a0.nRes = AddTemp( TYPE_BOOL );
        UINT nComm = AddComm( a0 );
        if(As(nComm))
          return -1;
        a.nArg2 = a0.nRes;
      }
    else
    a.nArg2 = AddArgument(token,bLogic);//Второй аргумент
    GVAR& v1 = mGVAR.Obj(nArg1);
    GVAR& v2 = mGVAR.Obj(a.nArg2);
    EqType(nArg1,a.nArg2);
    a.nRes  = AddMeas(a,bLogic);//Результат
    *pRes   = a.nRes;
    }
  else
    {
    a.nArg1 = nArg1;//Первый аргумент
    GVAR& v1 = mGVAR.Obj(nArg1);
    GVAR& v2 = mGVAR.Obj(token.id);
    if ( v2.nType & TYPE_LOCAL )
      {
      if(v1.nType != TYPE_PARAM || a.nOper != OPER_EQ)
        v2.nType = ( v2.nType & TYPE_LOCAL ) | v1.nType;
      else
        {   //тот же тип что параметр    
            cross::SParamValueH* ss = (cross::SParamValueH*)(mGVAR.Param(a.nArg1));
            if(ss->def)
             {
             EValueType et = (EValueType)ss->def->eVal;
             v2.nType = ( v2.nType & TYPE_LOCAL ) | Inv( et );
             }
        }
      }
    EqType(nArg1,token.id);
    // Операция присваивания
    a.nRes  = token.id;//Результат
    a.nArg2 = -1;//Второй аргумент
    }
  if ( As(a.nRes) ) return -1;
  return AddComm( a );
  }
//
bool CVBScript::Arguments( char* txt, UINT& nRes, bool bLogic )
  {
  _static int A = 0; A++;
  if ( A == 16 )
    KKK();
  bool b = Compile( txt, &nRes, bLogic );
  if ( b )
    {
    SCommand& last = CommLast();
    nRes = last.nRes;
    }
  else
    {
    KKK();
    }
//Sergej  return !As(nRes);
  return nRes != -1;
  }
// 3 - аргумента
UINT CVBScript::FillMSGBOX( SToken& token )
  {
  if ( AS(token.txt != NULL) ) return -1;
  //
  cross::CString str = token.txt;
  char* a0 = str.GetBuffer();
  char* a1 = MyStrChr( a0, ',' );
  if ( AS( a1 != NULL ) ) return -1; *a1++ = 0;
  char* a2 = MyStrChr( a1, ',' );
  //if ( AS( a2 != NULL ) ) return -1; *a2++ = 0;
  char* a3 = NULL;
  char aa2[4] = "\" \"";
  char aa3[4] = "\" \"";
  if (a2==NULL)
  {
      a2= aa2; a3= aa3;
  }
  else
  {
  *a2++ = 0;
  a3 = MyStrChr( a2, ',' );
  if ( AS( a3 == NULL ) ) return -1;
  }
  //
  SCommand a;
  a.nOper = token.dw;//Операция
  //
  bool b;
  b = Arguments( a0, a.nArg1, false );
  if ( !b ) return -1;
  b = Arguments( a1, a.nArg2, false );
  if ( !b ) return -1;
  b = Arguments( a2, a.nArg3, false );
  if ( !b ) return -1;
  //
  a.nRes = AddTemp( TYPE_INT );
  UINT nAdd = AddComm( a );
  if ( As(nAdd) ) return -1;
  return a.nRes;
  }
// 2 - аргумента
UINT CVBScript::FillFormatNum( SToken& token )
  {
  if ( AS(token.txt != NULL) ) return -1;
  //
  cross::CString str = token.txt;
  char* a0 = str.GetBuffer();
  char* a1 = MyStrChr( a0, ',' );
  if ( AS( a1 != NULL ) ) return -1; *a1++ = 0;
  char* a2 = MyStrChr( a1, ',' );
  if ( AS( a2 == NULL ) ) return -1;
  //
  SCommand a;
  a.nOper = token.dw;//Операция
  //
  bool b;
  b = Arguments( a0, a.nArg1, false );
  if ( !b ) return -1;
  b = Arguments( a1, a.nArg2, false );
  if ( !b ) return -1;
  //
  a.nRes = AddTemp( TYPE_STRING );
  UINT nAdd = AddComm( a );
  if ( As(nAdd) ) return -1;
  return a.nRes;
  }
//
UINT CVBScript::FillABS( SToken& token )
  {
  SCommand a;
  a.nOper = token.dw;//Операция
  bool b = Arguments( token.txt, a.nArg1, false );
  if ( !b ) return -1;
  //
  GVAR& var = mGVAR.Obj(a.nArg1);
  //
  a.nRes = AddTemp( var.nType );
  UINT nAdd = AddComm( a );
  if ( As(nAdd) ) return -1;
  return a.nRes;
  }
//
UINT CVBScript::FillBRACE( SToken& token, bool bLogic )
  {
  SCommand a;
  a.nOper = token.dw;//Операция
  bool b = Arguments( token.txt, a.nArg1, bLogic );
  if ( !b ) return -1;
  //
  GVAR& v2 = mGVAR.Obj(a.nArg1);
  const char* n1 = mGVAR.Name(a.nArg1);
  //
  a.nRes = AddTemp( v2.nType );
  UINT nAdd = AddComm( a );
  if ( As(nAdd) ) return -1;
  return a.nRes;
  }
//
UINT CVBScript::FillFunction ( SToken& token, UINT nType )
  {
  SCommand a;
  a.nOper = token.dw;//Операция
  if(token.txt[0])
  {
  bool b = Arguments( token.txt, a.nArg1, false );
  if ( !b ) return -1;
  //
  GVAR& v2 = mGVAR.Obj(a.nArg1);
  const char* n1 = mGVAR.Name(a.nArg1);
  }
  //
  a.nRes = AddTemp( nType );
  UINT nAdd = AddComm( a );
  if ( As(nAdd) ) return -1;
  return a.nRes;
  }
//
UINT CVBScript::FillFunctionL( SToken& token, int nMax, UINT nType )
  {
  SCommand a;
  a.nOper = token.dw;
  UINT* arg = &a.nArg1;
  int nArg = FillArgList( token.txt, arg, min(nMax, 6) ); //в SCommand сейчас max 6 аргументов
  a.nRes = AddTemp( nType );
  UINT nAdd = AddComm( a );
  return a.nRes;
  }
//
UINT CVBScript::FillCreatePopupWindow(SToken& token)
{
  SCommand a;
  a.nOper = token.dw;//Операция
  //
  char *ptr = token.txt;
  SkipSpace( ptr );
  if(*ptr == '"')
    {
      if(char *ptr2 = strchr(ptr+1, '"'))
        *++ptr2 = 0; 
      else return -1;
    }
  else return -1;
  bool b;
  b = Arguments( ptr, a.nArg1, false );
  if ( !b ) 
     return -1;
  a.nRes = AddTemp( TYPE_BOOL );
  UINT nAdd = AddComm( a );
  if ( As(nAdd) ) 
     return -1;
  return a.nRes;
  
  return -1;
}

UINT CVBScript::FillAlarmStateIconfactory( SToken& token )
  {
  SCommand a;
  a.nOper = token.dw;
  bool b;
  if(stricmp(token.txt, ".LowColor")==0)
     b = Arguments( "1", a.nArg1, false );
  else
  if(stricmp(token.txt, ".HighColor")==0)
     b = Arguments( "2", a.nArg1, false );
  else
  if(stricmp(token.txt, ".UrgentColor")==0)
     b = Arguments( "3", a.nArg1, false );
  else
     b = Arguments( "0", a.nArg1, false );
  if ( !b ) 
    return -1;
  //
  a.nRes = AddTemp( TYPE_INT );
  UINT nAdd = AddComm( a );
  if ( As(nAdd) ) 
      return -1;
  return a.nRes;
  }

UINT CVBScript::FillGetcustomproperty( SToken& token )
  {
  SCommand a;
  a.nOper = token.dw;//Операция
  char *pp = strchr(token.txt, '.');
  if(pp==0)
    return -1;
  *pp++=0;
  char *pb = strchr(pp, '(');
  if(pb==0)
    return -1;
  char *pq1 = strchr(pb+1, '"');
  if(pq1==0)
    return -1;
  char *pq2 = strchr(pq1+1, '"');
  if(pq2==0)
    return -1;
  char *pc = strchr(pq2+1, ',');
  if(pc==0)
    return -1;
  *pc++ = 0;
  bool b = Arguments( token.txt, a.nArg1, false );
  if ( !b ) 
      return -1;
  b = Arguments( pq1, a.nArg2, false );
  if ( !b ) 
      return -1;
  b = Arguments( pc, a.nArg3, false);
  if ( !b ) 
      return -1;
  //
  GVAR& v2 = mGVAR.Obj(a.nArg1);
#ifdef _DEBUG
  const char* n1 = mGVAR.Name(a.nArg1);
  const char* n2 = mGVAR.Name(a.nArg2);
  const char* n3 = mGVAR.Name(a.nArg3);
#endif
  //
  a.nRes = AddTemp( TYPE_STRING );
  UINT nAdd = AddComm( a );
  if ( As(nAdd) ) return -1;
  return a.nRes;
  }
//

UINT CVBScript::FillGetAttribute( SToken& token )
  {
  SCommand a;
  a.nOper = token.dw;//Операция
  char *pp = strchr(token.txt, '.');
  if(pp==0)
    return -1;
  *pp++=0;
  char *pb = strchr(pp, '(');
  if(pb==0)
    return -1;
  char *pq1 = strchr(pb+1, '"');
  if(pq1==0)
    return -1;
  char *pq2 = strchr(pq1+1, '"');
  if(pq2==0)
    return -1;
  bool b = Arguments( token.txt, a.nArg1, false );
  if ( !b ) 
      return -1;
  b = Arguments( pq1, a.nArg2, false );
  if ( !b ) 
      return -1;
  //
  GVAR& v2 = mGVAR.Obj(a.nArg1);
#ifdef _DEBUG
  const char* n1 = mGVAR.Name(a.nArg1);
  const char* n2 = mGVAR.Name(a.nArg2);
#endif
  //
  a.nRes = AddTemp( TYPE_STRING );
  UINT nAdd = AddComm( a );
  if ( As(nAdd) ) return -1;
  return a.nRes;
  }
//
UINT CVBScript::FillSetAttribute( SToken& token )
  {//без скобок
  SCommand a;
  a.nOper = token.dw;//Операция
  char *pp = strchr(token.txt, '.');
  if(pp==0)
    return -1;
  *pp++=0;
  char *pq1 = strchr(pp+1, '"');
  if(pq1==0)
    return -1;
  char *pq2 = strchr(pq1+1, '"');
  if(pq2==0)
    return -1;
  char *pc = strchr(pq2+1, ',');
  if(pc==0)
    return -1;
  *pc++ = 0;
  bool b = Arguments( token.txt, a.nArg1, false );
  if ( !b ) 
      return -1;
  b = Arguments( pq1, a.nArg2, false );
  if ( !b ) 
      return -1;
  b = Arguments( pc, a.nArg3, false);
  if ( !b ) 
      return -1;
  //
  GVAR& v2 = mGVAR.Obj(a.nArg1);
#ifdef _DEBUG
  const char* n1 = mGVAR.Name(a.nArg1);
  const char* n2 = mGVAR.Name(a.nArg2);
  const char* n3 = mGVAR.Name(a.nArg3);
#endif
  //
  a.nRes = AddTemp( TYPE_STRING );
  UINT nAdd = AddComm( a );
  if ( As(nAdd) ) return -1;
  return a.nRes;
  }
//
UINT CVBScript::FillForTo(SToken *token, int nCount)
{
    return -1;
}
//
UINT CVBScript::FillFuncWithEmptyParam(SToken& token, UINT nType )
{
  SCommand a;
  a.nOper = token.dw;//Операция
  a.nRes = AddTemp( nType );
  UINT nAdd = AddComm( a );
  if ( As(nAdd) ) return -1;
  return a.nRes;
}

UINT CVBScript::FillFunDataValue ( SToken& token, UINT nType )
  {
  SCommand a;
  a.nOper = token.dw;//Операция
  bool b = Arguments( token.txt, a.nArg1, false );
  if ( !b ) return -1;
  //
  GVAR& v2 = mGVAR.Obj(a.nArg1);
  const char* n1 = mGVAR.Name(a.nArg1);
  //
  a.nArg2 = AddTemp( TYPE_OBJECT );
  EValueTypeScript eB;
  SCTag *tg = (SCTag*)mGVAR.Var( *html, a.nArg2, eB );
  tg->m_pTag = token.mTag;
  
  //
  a.nRes = AddTemp( nType );
  UINT nAdd = AddComm( a );
  if ( As(nAdd) ) return -1;
  return a.nRes;
  }
//
