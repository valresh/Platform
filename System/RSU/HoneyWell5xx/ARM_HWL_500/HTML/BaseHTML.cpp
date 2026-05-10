#include "../StdAfx.h"
#include "../ARM.h"
#include "../AplicFrm.h"
#include "BaseHTML.h"
#include "HTML.h"

//custom includes - not imported
#include <avaloniaWrappers/AfxControls.h>
//

int SBaseHTML::AddTexts( CHTML& html, const char* text, int dl )
  {
  if ( dl == 0 ) 
  {
      dl = lstrlen(text)+1;
      html.AddChar(dl);
  }
  if ( dl == 1 ) return 0;//Пустая строка
  int nCurr = 1;
  int nEnd  = html.CurrChar(0);
  char* ptr;
  if(!html.m_LoadAcceleration)
  {
  ptr = html.Buffer(nCurr);
  while ( nCurr < nEnd )
    {
    int ld = lstrlen(ptr)+1;
    if ( ld == dl )
      {
      if ( lstrcmpi( ptr, text ) == 0 )
        return nCurr;
      }
    nCurr += ld;
    ptr   += ld;
    }
  }
  else ptr = html.Buffer(nEnd);
  //
  lstrcpy( ptr, text );
  return html.CurrChar(dl);
  }

void SBaseHTML::AddItems( CHTML& html, int nCount, STegScn* teg )
  {
  if ( html.m_bBinData ) return;
  if ( !html.AddParam(nCount+1) ) return;
  m_nKolvo = nCount;// Количество параметров
  m_nPlace = html.CurrParam(nCount+1);
  //
  SParamsData* list = html.Param(m_nPlace);
  for ( int n = 0; n < nCount; n++ )
    {
    int dl = lstrlen(teg[n].name )+1;
    int ld = lstrlen(teg[n].value)+1;
    if ( !html.AddChar(dl+ld) )
      {
      ASSERT(0);
      m_nKolvo = 0;
      }
    list[n].nName  = AddTexts( html, teg[n].name , dl );
    list[n].nValue = AddTexts( html, teg[n].value, ld );
    }
  }

void SBaseHTML::SetLastName( CHTML& html, const char* text )
  {
  if ( html.m_bBinData ) return;
  if ( m_nPlace == 0 ) AddItems( html, 0, NULL );
  SParamsData* list = html.Param(m_nPlace);
  int dl = lstrlen(text)+1;
  if ( html.AddChar(dl) )
  list[m_nKolvo].nName = AddTexts( html, text, dl );
  }

void SBaseHTML::SetLastValue( CHTML& html, const char* text )
  {
  if ( html.m_bBinData ) return;
  if ( m_nPlace == 0 ) AddItems( html, 0, NULL );
  SParamsData* list = html.Param(m_nPlace);
  int dl = lstrlen(text)+1;
  if ( html.AddChar(dl) )
  list[m_nKolvo].nValue = AddTexts( html, text, dl );
  }

const char* SBaseHTML::GetName ( CHTML& html, int n)
  {
  SParamsData* d = html.Param(m_nPlace+n);
  return html.Buffer(d[0].nName);
  }

const char* SBaseHTML::GetValue( CHTML& html, int n)
  {
  SParamsData* d = html.Param(m_nPlace+n);
  return html.Buffer(d[0].nValue);
  }

const char* SBaseHTML::GetName(CHTML& html)
  {
  return GetName( html, m_nKolvo );
  }

const char* SBaseHTML::GetValue(CHTML& html)
  {
  return GetValue( html, m_nKolvo );
  }

int SBaseHTML::FindItem( CHTML& html, const char* pszName )
  {
  for ( int n = 0; n < m_nKolvo; n++ )
    {
    const char* name = GetName(html,n);
    if ( name != NULL )
    if ( lstrcmpi(name, pszName ) == 0 )
      return n;
    }
  return -1;
  }

const char* SBaseDSD::PropNameValue( CHTML& html, const char* text )
  {
//if ( !IsYou( "property" ) ) return NULL;
  int N = FindItem( html, "name" );
  if ( N >= 0 )
  if ( lstrcmpi( GetValue(html,N), text ) == 0 )
  return GetValue(html);
  return NULL;
  }
//
const char* SBaseDSD::GetStr( CHTML& html, const char* name )
  {
  SBaseDSD* dsd = html.DSD(m_nPlaceDSD+1);
  for ( int n = 0; n < m_nCountDSD; n++ )
    {
    const char* value = dsd[n].PropNameValue( html, name );
    if ( value ) return value;
    }
  return NULL;
  }
//
int SBaseDSD::GetInt(CHTML& html,const char* name)
  {
  const char* value = GetStr( html, name );
  if ( value && *value )
    {
    ASSERT( IsInt(value) );
    return atoi(value);
    }
  return -1;
  }
//
const char* SBaseDSD::PointName(CHTML& html)
  {
  return GetStr(html,"PointRefPointName");
  }
//
const char* SBaseDSD::CommaPointName(CHTML& html)
  {
  return GetStr(html,"CommaDelimitedPointNames");
  }
//
void SBaseDSD::ActionDSD( HWND hWnd, CHTML& html )
  {
  int n = FindItem( html, "type" );
  if ( n < 1 )
    cross::AfxMessageBox( "Непонятный тип" );
  else
  if ( lstrcmpi( GetValue(html,n), "HMIPage.PushButton" ) == 0 )
    PushButtons( hWnd, html );
  else
    cross::AfxMessageBox( "Неподдерживаемый тип" );
  }
//
void SBaseDSD::String( CHTML& html, cross::CString& str, int N )
  {
  for ( int n = 0; n < N; n++ )
    str += "  ";
  //
  str += "<";
  str += GetName(html);
  for ( int n = 0; n < m_nKolvo; n++ )
    {
    const char* name  = GetName (html,n);
    const char* value = GetValue(html,n);
    if ( name && value )
      {
      str += " ";
      str += name;
      str += "=";
      str += value;
      }
    }
  //
  if ( m_nCountDSD == 0 ) str += "/";
  str += ">";
  const char* value = GetValue(html);
  if ( value )
    {
    str += " ";
    str += value;
    }
  str += "\r\n";
  //
  SBaseDSD* dsd = html.DSD(m_nPlaceDSD+1);
	for ( INT_PTR n = 0; n < m_nCountDSD; n++ )
    dsd[n].String( html, str, N+1 );
  }
//
UINT SBaseDSD::Pipe(CHTML& html, char* pparam, CTag* pTag)
  {
  const char* comma = CommaPointName(html);
  const char* point = PointName(html);
  if ( comma == NULL &&  point == NULL ) return 0;
  //
  const char* valueC = GetStr( html, "CommaDelimitedParameters" );
  const char* valueP = GetStr( html, "PointRefParamName" );
  if ( valueC == NULL &&  valueP == NULL ) return 0;
  if (lstrcmpi( valueP, "{%cp_desc%}" ) == 0 ){
	  valueP = html.m_strDesc;
  } else if (lstrcmpi( valueP, "{%cp_rst%}" ) == 0 ){
	  valueP = html.m_strReset;
  } else if (lstrcmpi( valueP, "{%cp_pv%}" ) == 0 ){
	  valueP = html.m_strPV;
  }
  //
  char *ptr, *str;
  if ( comma )
    ptr = (char*)comma, str = (char*)valueC;
  else
    ptr = (char*)point, str = (char*)valueP;
  //
  if(Version == BERTH_ONPZ)
  if(_stricmp(ptr, "{%Null%}")==0 && _stricmp(str, "{%Null%}")==0)
     return 0;
  //
  ASSERT( str );
  //
  if ( lstrcmpi( ptr, "{%CurrentPoint%}" ) == 0 )
    ptr = html.Group().GetBuffer();
  else
  if ( !*ptr )
    ptr = html.Group().GetBuffer();
  //
  if ( !*ptr && str[0] && pTag)
  {
     char tagname[] = "tagname";
     ptr = (char*)Alias( html, pTag,  tagname );
  }
  //
  SUniBuffer& buff = html.PipeBuff();
  //
  UINT nType = 0;
  //
  int nCount = 0;
  while ( *ptr && *str )
    {
    char* p = strchr( ptr, ',' );
    if ( p ) *p = 0;
    char* s = strchr( str, ',' );
    if ( s ) *s = 0;
    //
    if ( m_nCountPip == 0 )
      {
      if ( !buff.Realloc(1) ) return 0;
      if ( nCount )
        buff.AddCurr();
      else
        m_nPlacePip = buff.AddCurr();
      }
    else
      buff.AddCurr();
    //
    cross::SParamValueH* obj = ((cross::SParamValueH*)buff.m_szBuffer)+m_nPlacePip+nCount;
    //
    while ( *ptr == ' ' ) ptr++;
    int dl = lstrlen(ptr)-1;
    while ( dl >= 0 && ptr[dl] == ' ' ) ptr[dl] = 0, dl--;
    if((Version == L35_11600_KF || Version == ITH_NAK || Version == AM2_NAK || Version ==DMW_NAK) 
       && html.m_strFile.CompareNoCase("sysdtlPIDA_fp") == 0 && _stricmp(str, "pida.pvp") == 0)
    {
        obj[0] = FindExperion( html, ptr, "pida.pv");
    }
    else
    if((Version == L35_11600_KF || Version == ITH_NAK || Version == AM2_NAK || Version ==DMW_NAK) 
        && html.m_strFile.CompareNoCase("sysdtlPIDA_fp") == 0 && _stricmp(str, "PIDA.SPP") == 0)
    {
        obj[0] = FindExperion( html, ptr, "PIDA.SP");
    }
    else
    if(Version==DMW_NAK && _stricmp(str, "PVAlgo.pvp") == 0) 
    {
         obj[0] = FindExperion( html, ptr, "PVAlgo.pv");
    }
    else
    {
	if( (Version == T1165P_ONPZ || Version == T1163_ONPZ)
		&& !strcmp(html.m_strFile, "TSB_TREND_POPUP_1")
		&& !strcmp(ptr, "{%PARAM1%}")) 
		ptr = "{%param1%}";
    obj[0] = FindExperion( html, ptr, str, &nType );
    if(nType)
    {
     if(pparam)
     {
       char param[256];
       strcpy_s(param, ptr);

       char* ref;
       if((strncmp(param, "{%", sizeof("{%")-1)==0) && (ref = strstr(param+sizeof("{%"), "%}")))
       {
        char namekey[64]="";
        strncpy_s(namekey, param+sizeof( "{%")-1, ref-param-sizeof( "{%")+1);
        std::string namekeyval =  html.FindInDisplayDataRepository(namekey);
        strcpy_s(param, namekeyval.c_str());
        }

       strcat_s(param, ".");
       strcat_s(param, str);
       lstrcpy(pparam, param);
     }
    }
    }
    //
    nCount++;
    if ( p ) *p++ = ',', ptr = p;
    else break;
    if ( s ) *s++ = ',', str = s;
    else break;
    }
  m_nCountPip = nCount;
  return nType;
  }
//
bool SBaseDSD::IsOK( CHTML& html )
  {
  cross::SParamValueH* vars = html.Value();
  vars += m_nPlacePip;
  for ( int n = 0; n < m_nCountPip; n++ )
    {
    const char* name = html.pipe.GetMnemoName( vars[n].nNumber );
    EDataTypes  type = (EDataTypes)html.pipe.GetMnemoType( vars[n].nNumber );
    CShBase* sh = html.pipe.Sh( vars[n].nNumber );
    if ( !IsTypeOk )
      return false;
    if ( vars[n].def == NULL )
      return false;
    }
  return true;
  }
//
cross::SParamValueH* SBaseDSD::Value( CHTML& html, int n )
  {
  cross::SParamValueH* vars = html.Value();
  return vars + m_nPlacePip + n;
  }
//
bool SBaseDSD::IsPushButtonAction0(CHTML& html)
{
	  int n = FindItem( html, "type" );
	  if ( n >= 1 )
	  {
		  if ( lstrcmpi( GetValue(html,n), "HMIPage.PushButton" ) == 0 )
		  {
			  int nType = GetInt(html,"PBActionType");
			  if (nType==0)
				  return true;
		  }
	  }
	  return false;
}

bool SBaseDSD::PushButtons( HWND hWnd, CHTML& html )
  {
  int nType = GetInt(html,"PBActionType");
  int nLrn  = GetInt(html,"TaskLrn");
  int          nParam1 = GetInt( html, "TaskParam1" );
  const char* szParam2 = GetStr( html, "TaskParam2" );
  const char* szParam3 = GetStr( html, "TaskParam3" );
  const char* szParam4 = GetStr( html, "TaskParam4" );
  ASSERT( nLrn == 21 );
  ASSERT( szParam2 != NULL );
  ASSERT( lstrcmpi( szParam3, "0" ) == 0 );
  ASSERT( lstrcmpi( szParam4, "0" ) == 0 || !*szParam4 );
  if( szParam2 != NULL )
  switch ( nType )
    {
    case 0: if(Version == DOTF_PM)
            {
              if(!isdigit(*szParam2))
                 return PushAction1( hWnd, html, szParam2 );
              else return false;
            }
            return PushAction0( hWnd, html, szParam2 );
    case 1: return PushAction1( hWnd, html, szParam2 );
    case 3: return PushAction3( hWnd, html, szParam2 );
    case 5: return PushAction5( hWnd, html, szParam2 );
    };
  cross::AfxMessageBox( "Неподдерживаемый тип нажатия" );
  return false;
  }
//
// Вызов скрипта, пока сделано вручную, передачей mVal 
bool SBaseDSD::PushAction0( HWND hWnd, CHTML& html, LPCTSTR szParam, std::string strValue, cross::SParamValueH* pVal )
  {
  if ((pVal==NULL)||(strValue==""))
  {
        cross::AfxMessageBox( "Неподдерживаемый тип нажатия" );
		return false;
  }
  else
  {
	  if (Version==Реформинг_КНПЗ)
	  {
	  		if (pVal->nNumber>-1)
			{
				const char* name = html.pipe.GetMnemoName( pVal->nNumber );
				EDataTypes  type = (EDataTypes)html.pipe.GetMnemoType( pVal->nNumber);
				void* val = html.pipe.ParamValue_H(*pVal);
				if ( val != NULL )
				{
				   if (pVal->def->eVal==enumValueChr)
				   {
						if (type==id_DEVCTL)
							ResetDEVCTL(html, val ,pVal,strValue);
						else if (type==id_FLAG)
							Set1_InFlag(html, val ,pVal,strValue,name);
				   }
				}
			}
	  }
  }
  return true;
  }
//
void SBaseDSD::ResetDEVCTL(CHTML& html, void* val ,cross::SParamValueH* pVal,std::string& strValue)
{
		if (strValue=="Сброс")
		{
				BYTE* pValue=(BYTE*) val;
				BYTE bResetValue=*pValue;
				BYTE bReset=5;
				if (bResetValue!=bReset)
				{
						if (IDYES==GetAppWnd()->MessageBox("Сбросить блокировки?",
						  NULL, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 ))
						{
								  BYTE bNew = bReset;
								  SSendToModel send;
								  send.Set( enumValueChr, &bResetValue, &bNew );
								  lstrcpy( send.szValue, pVal->def->name );
								  html.pipe.SendData( pVal->nNumber, send, true );
						}
				}
		}
}

void SBaseDSD::Set1_InFlag(CHTML& html, void* val ,cross::SParamValueH* pVal,std::string& strValue, std::string strName)
{
		std::string strMessage="";
		if (strValue=="Сброс")
			strMessage="Сбросить блокировку?";
		else
		if (strValue=="Групп-е ОТКЛ")
			strMessage="Выполнить Групповое ОТКЛЮЧЕНИЕ?";
		else
		if (strValue=="ОТКЛ")
		{
			if (strName=="VV001.RST")
				strMessage="Выполнить ОТКЛЮЧЕНИЕ П-1/П-1а?";
			else
				if (strName=="VV002.RST")
					strMessage="Выполнить ОТКЛЮЧЕНИЕ П-2/П-2а?";
		}
		else
		if (strValue=="Закрыть")
		{
			if (strName=="USV2_1.FLAGA")
				strMessage="Закрыть КПУ 2.1?";
			else
			if (strName=="USV2_2.FLAGA")
				strMessage="Закрыть КПУ 2.2?";
			else
			if (strName=="USV2_3.FLAGA")
				strMessage="Закрыть КПУ 2.3?";
			else
			if (strName=="USV3.FLAGA")
				strMessage="Закрыть КПУ 3?";
		}
		BYTE* pValue=(BYTE*) val;
		BYTE bOldValue=*pValue;
		if (bOldValue!=1)
		{
			if (IDYES==GetAppWnd()->MessageBox(strMessage.c_str(),
					NULL, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 ))
				{
								  BYTE bNew = 1;
								  SSendToModel send;
								  send.Set( enumValueChr, &bOldValue, &bNew );
								  lstrcpy( send.szValue, pVal->def->name );
								  html.pipe.SendData( pVal->nNumber, send, true );
				}
		}
}
// Переход на другую мнемосхему
bool SBaseDSD::PushAction1( HWND hWnd, CHTML& html, LPCTSTR szParam )
  {
  if (Version==Реформинг_КНПЗ)
  {
	  if (lstrcmp(szParam,"m50")==0)
		  return true;
  }
  if (Version == UKR_PRP 
	  && !lstrcmp(szParam,"330") 
	  && !html.m_strFile.CompareNoCase("Fire"))
  {
//	  GetAppWnd()->ButtonOk( barPAGE, "blokckem", hWnd );
	  return true;
  }
//  GetAppWnd()->ButtonOk( barPAGE, szParam, hWnd );
  return true;
  }
//
//
bool SBaseDSD::PushAction3( HWND hWnd, CHTML& html, LPCTSTR szParam )
  {
  int nParam1 = GetInt( html, "TaskParam1" );
  switch ( nParam1 )
    {
    case  7: // Тоже самое что тип 5
    case 20: // Тоже самое что тип 5
      ASSERT( html.m_bR300 );
 //     GetAppWnd()->GiveAmm()->GoToPage(szParam,html.Group());
    return true;
    case  8: // Вызов тренда по номеру
//		  GetAppWnd()->ButtonOk( barTREND, szParam );
    return true;
    case 49: // Вызов группы по номеру
//		  GetAppWnd()->ButtonOk( barGROUP, szParam );
    return true;
	case  50: // Вызов тренда по номеру
//		  GetAppWnd()->ButtonOk( barTREND, szParam );
    return true;
    case 61: // Вызов первой закладки на детальном окне
//		  GetAppWnd()->ButtonOk( barDETAIL, html.Group() );
    return true;
    };
  cross::AfxMessageBox( "Неподдерживаемый тип нажатия" );
  return false;
  }
//
// Вызов системного файла
bool SBaseDSD::PushAction5( HWND hWnd, CHTML& html, LPCTSTR szParam )
  {
  ASSERT( html.m_bR300 );
  int nParam1 = GetInt( html, "TaskParam1" );
  switch ( nParam1 )
    {
    case  7:
 //     GetAppWnd()->GiveAmm()->GoToPage(szParam,html.Group());
    return true;
	case  24://Ack Page
//		GetAppWnd()->GiveAmm()->GetMnemo()->MnemoAck();
    return true;
    case 61: // Вызов первой закладки на детальном окне
//		  GetAppWnd()->ButtonOk( barDETAIL, html.Group() );
    return true;
    };
  cross::AfxMessageBox( "Неподдерживаемый тип нажатия (системный)" );
  return false;
  }
//
