#include "StdAfx.h"
#include "../ARM.h"
#include "ArrayObj.h"
#include "TexData.h"
#include "Queue.h"

const char g_chVer = 5;

BOOL FileIsPresent(LPCTSTR pszFileName)
  {
  DWORD dwFileAttr = GetFileAttributes(pszFileName);
  return ((dwFileAttr != (DWORD)-1) &&
         ((dwFileAttr & FILE_ATTRIBUTE_DIRECTORY) == 0));
  }

CArrayObj::CArrayObj(int nMonitor)
: mTags(mName)
, m_szBuff(NULL)
, m_nMonitor(nMonitor)
  {
  ChangeMenu( "Menu" );
  hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
  }

CArrayObj::~CArrayObj(void)
  {
  ClearsAll();
  }

void CArrayObj::ClearsAll()
  {
  // Признак того, что использовались данные
  // из 'пережёванного' файла.
  if ( m_szBuff )
    {
    mTags.m_szBuffer = NULL;
    mName.m_szBuffer = NULL;
    ::BuffFree(m_szBuff);
    m_szBuff = NULL;
    }
  else
    {
    mTags.Clear();
    mName.Clear();
    }
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
char* CArrayObj::KeyName( const char* name )
  {
  static char key[256];
//sprintf_s( key, sizeof(key), "%s-%s", m_strMenu, name );
  sprintf_s( key, sizeof(key), "%d-%s", m_nMonitor, name );
  return key;
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
HMyEMF CArrayObj::LoadCrd( LPCTSTR pszFileName, int& nError, bool bBuildList )
  {
  nError = 0;
  CModelInfo& model = ::ModelInfo();
  //
  TCHAR szEmf[_MAX_PATH], szCrd[_MAX_PATH], szBin[_MAX_PATH];
  sprintf_s( szEmf, _MAX_PATH, _T("%s%s.emf" ), model.dirScheme, pszFileName );
#ifdef _WIN64
  sprintf_s( szCrd, _MAX_PATH, _T("%s%s.bin" ), model.dirScheme, pszFileName );
#else
  sprintf_s( szCrd, _MAX_PATH, _T("%s%s.bin32" ), model.dirScheme, pszFileName );
#endif
  GetBinFile( szBin, szCrd );
  sprintf_s( szCrd, _MAX_PATH, _T("%s%s.crd" ), model.dirScheme, pszFileName );
  //
  NetworkFile( szEmf );
  NetworkFile( szCrd );
  //
  _WIN32_FILE_ATTRIBUTE_DATA attrCrd,attrEmf;
  char* szBuff = NULL;
  if ( GetAttr( szCrd, attrCrd ) && GetAttr( szEmf, attrEmf ) )
    {
    DWORD dwSize;
    char* szBuffer = ::FileToBuff( szBin, dwSize );
    if ( szBuffer )
      {
      char* ptr = szBuffer;
      if ( *ptr == g_chVer )//Версия
        {
        ptr++;
        FILETIME* F = (FILETIME*)ptr;ptr += 2*sizeof(FILETIME);
        if ( EqAttr(attrCrd,F[0])&&EqAttr(attrEmf,F[1]) )
          {
          ClearsAll();
          mTags.Read( ptr );
          mName.Read( ptr );
          szBuff = szBuffer;
          }
        }
      if ( szBuff ) m_szBuff = szBuff;
      else     ::BuffFree( szBuffer );
      }
    }
  //
  ENHMETAHEADER hdr;
  HMyEMF hEmf = NULL;
  if ( !bBuildList )
    {
    hEmf = ::EmfCreate();
    if ( hEmf == NULL ) nError = -1;
    else
    if ( !EmfOpen( hEmf, szEmf ) ) nError = -2,EmfDelete(hEmf);
    //
    if ( hEmf == NULL ) return hEmf;
    hdr = ::EmfHeader(hEmf);
    }
  else
    {
    HFILE hFile = _lopen( szEmf, OF_READ );
    if ( hFile == HFILE_ERROR ) nError = -2;
    else
      {
      int nSize = _hread( hFile, &hdr, sizeof(hdr) );
      _lclose( hFile );
      if ( nSize != sizeof(hdr) ) nError = -2;
      }
    if ( nError != 0 ) return hEmf;
    }
  //
  if ( szBuff != NULL ) return hEmf;
  //
#if 0
  double scaleX = (double)(hdr.szlMicrometers.cx)/(hdr.szlDevice.cx*1000.);
  double scaleY = (double)(hdr.szlMicrometers.cy)/(hdr.szlDevice.cy*1000.);
#else
  double scaleX = (double)(25.4)/(96);
  double scaleY = (double)(25.4)/(96);
#endif
  nError = ReadCrd( szCrd, scaleX, scaleY );
  if ( nError != 0 )  return ::EmfDelete(hEmf);
  //
  HFILE hFile = _lcreat( szBin, 0 );
  if ( hFile != HFILE_ERROR )
    {
    _hwrite( hFile, &g_chVer, 1 );
    _hwrite( hFile, (char*)&attrCrd.ftLastWriteTime, sizeof(FILETIME) );
    _hwrite( hFile, (char*)&attrEmf.ftLastWriteTime, sizeof(FILETIME) );
    mTags.Write( hFile );
    mName.Write( hFile );
    _lclose( hFile );
    }
  return hEmf;
  }

bool CArrayObj::AlwaysLeaveSensorName()
{
	return true;
}

int CArrayObj::ReadCrd(LPCTSTR pszFilePath,double scaleX,double scaleY)
	{
  //
	CStdioFile file;
	if(!file.Open(pszFilePath,CFile::modeRead))
		return -3;
  //
  SUniChar mNam;
  if ( !mNam.Realloc(1) ) return -4;
  mNam.AddCurr();
  SUniTags mTag(mNam);
  //
  int line = 1;
	CString str;
  STagsObj obj;
	while(file.ReadString(str))
		{
    obj.color     =  0;
    obj.nFont     =  0;
    obj.nDots     =  0;
    obj.m_nNumber = -1;
	obj.m_nSocketNumber=-1;
    // добавляем в массив
    if ( !mTag.Realloc(1) ) return -5;
    if ( line > 380 )
		KKK();
    //
    obj.m_nName = mNam.AddBuff( str );
    // комментарий
	CString strComment;
    line++;file.ReadString(strComment);
	// тип
	line++;file.ReadString(str);
	_stscanf_s(str,_T("%d"),&obj.m_eType);
    if (obj.m_eType==id_Ventil)
		strComment="#Вентиль";
	if (obj.m_eType==id_3xZadv)
		strComment="#Открытие";
	if (obj.m_eType==id_ElectroNagrev)
		strComment="#Состояние";
	if (obj.m_eType>=id_ModelData)
	{
		int nDollarIndex=strComment.Find('$');
		if (nDollarIndex!=-1)
		{
			if (nDollarIndex<strComment.GetLength()-1)
			{
				CString strColorScheme=strComment.Mid(nDollarIndex+1,1);
				int nColorScheme = atoi( strColorScheme );
				obj.m_colorModelScheme=(EColorScheme)nColorScheme;
				strComment=strComment.Left(nDollarIndex);
			}
		}
		else
			obj.m_colorModelScheme=eColor_0_1;
	}
    obj.m_nComm = mNam.AddBuff( strComment);
		// координаты
		line++;file.ReadString(str);line++;
    obj.nModelDialog=0;
	if (theApp.IsSpecificModelDataType(obj.m_eType))
		obj.nModelDialog=obj.m_eType;
    if ( obj.m_eType == id_Dialog   ) continue;
    if ( obj.m_eType == id_ShSens   ) continue;
    if ( obj.m_eType == id_Reg      ) continue;
    //if ( obj.m_eType == id_ShLocReg ) continue;
    //if ( obj.m_eType == id_Sensor ) continue;
    //
    char* ptr = str.GetBuffer();
    for ( int n = 0; n < 4; n++ )
      {
      while ( *ptr == ' ' ) ptr++;
      char* end = ptr;
      while ( *end != ' ' && *end ) end++;
      *end = 0;
      double d = atos(ptr);
      ptr = end+1;
      switch ( n )
        {
        case 0:obj.rc.left   = (int)( d/scaleX+0.5)-2;break;
        case 1:obj.rc.bottom = (int)(-d/scaleY+0.5)+2;break;
        case 2:obj.rc.right  = (int)( d/scaleX+0.5)+2;break;
        case 3:obj.rc.top    = (int)(-d/scaleY+0.5)-2;break;
        }
      }
    // Особенность имени клапана
    if ( obj.m_eType == id_Klapan )
      {
      char* name = mNam.Name(obj.m_nName);
      if ( _strnicmp( name, ".Рег.", 5 ) == 0 )
        {
        UINT dl = UINT(obj.m_nName);
        obj.m_nName = (HCHAR)(dl+5);
        }
      }
    else
    if ( obj.m_eType == id_Zadv )
      {
		  char* name = mNam.Name(obj.m_nName);
		  if (CZadvigkiFromKlapan::IsZadvigkaFromKlapanSborka(name))
				obj.m_eType=id_Klapan;
      }
    else
    // Уровни
    if ( obj.m_eType == id_Level )
      {
	  const char* name = mNam.Name( obj.m_nName );
      const char* comm = mNam.Name( obj.m_nComm );
      obj.color = RGB(0,0,255);
      char* p = strchr( mNam.Name(obj.m_nName), '#' );
      if ( p != 0 )
        {
        *p = 0;
        obj.m_nComm = obj.m_nName;
        UINT dl = UINT(obj.m_nComm)+lstrlen(mNam.Name(obj.m_nComm));
        obj.m_nName = (HCHAR)(dl+1);
        }
      else
        obj.m_nComm = obj.m_nName;
      }
    else
	if (( obj.m_eType == id_Sensor )||( obj.m_eType == id_Sensor_45 )||( obj.m_eType == id_Sensor_46 )||( obj.m_eType == id_Sensor_47 ))
      {
		  char* name = mNam.Name(obj.m_nName);
		  bool bLeaveSensorName=false;
		  if (theApp.IsSensorNameInFile(name))
				bLeaveSensorName=true;
		  if (AlwaysLeaveSensorName())
				bLeaveSensorName=true;
		  if ((!bLeaveSensorName)&&(name[0]=='_'))
			{
			UINT dl = UINT(obj.m_nName);
			obj.m_nName = (HCHAR)(dl+1);
			}
      }
	else
    // Прямопоказывающие датчики
    if ( obj.m_eType == id_SensPlace )
      {
      obj.nFont = 18;// Значения
      obj.nDots = -3;// по умолчанию
      // FFFF00,18,0 ( цвет, высота шрифта и формат вывода
      char* s = strchr( mNam.Name(obj.m_nComm), ',' );
      if ( s != 0 )
        {
        *s++ = 0;
        obj.color = ::ParserColors( mNam.Name(obj.m_nComm) );
        char* t = strchr( s, ',' );
        if ( t != 0 )
          {
          *t++ = 0;
          obj.nFont = atoi(s);
          obj.nDots = atoi(t);
          }
        else
          {
          int m = atoi(s);
          if ( m > 6 )
            obj.nFont = m;
          else
            obj.nDots = m;
          }
        }
      else
        obj.color = ::ParserColors( mNam.Name(obj.m_nComm) );
      // Тоже самое, что у уровня
      char* p = strchr( mNam.Name(obj.m_nName), '#' );
      if ( p != 0 )
        {
        *p = 0;
        obj.m_nComm = obj.m_nName;
        UINT dl = UINT(obj.m_nComm)+lstrlen(mNam.Name(obj.m_nComm));
        obj.m_nName = (HCHAR)(dl+1);
        }
      else
        obj.m_nComm = obj.m_nName;
      }
    else
    // Вызов диалогов
    if ( obj.m_eType == id_Library )
      {
      HCHAR a = obj.m_nComm;
      obj.m_nComm = obj.m_nName;
      obj.m_nName = a;
      }
    else
    // Выделение пустых переходов
    if ( obj.m_eType == id_Goto && ::IsMnemoStyle(MNEMO_EDIT) )
      {
      CString strComm = mNam.Name(obj.m_nComm);
      int Dvd = strComm.Find ( '|' );//Переход
      if ( Dvd >= 0 )
        strComm = strComm.Left ( Dvd );
	    if ( strComm.IsEmpty() )
        obj.color = 0xff0000;// Имя перехода пустое
      else
        {
        CModelInfo& model = ::ModelInfo();
        char file[_MAX_PATH];
        sprintf_s( file, sizeof(file), "%s%s.emf", model.dirScheme, strComm );
        if ( !FileIsPresent(file) )
          obj.color = 0x0000ff;// Файла нет на диске
        else
          {
          const char* ptr = FileToMenu(strComm);
          if ( ptr == NULL )
          obj.color = 0x00ff00;// Файл не включён в menu.cfg
          }
        //
        char szName[_MAX_FNAME];
        _splitpath_s( pszFilePath, 0,0,0,0,szName,sizeof(szName),0,0);
        if ( lstrcmpi( strComm, szName ) == 0 )
          obj.color = 0xffff00;// Файл ссылается сам на себя
        }
      }
    else
    if ( obj.m_nComm == 0 )
      obj.m_nComm = obj.m_nName;
    //
    memcpy( mTag.m_szBuffer+mTag.m_nCurrent, &obj, sizeof(obj) );
    mTag.AddCurr();
		}
  //
  ClearsAll();
  mName.Copy(&mNam);
  mTags.Copy(&mTag);
  //
  //char text[1024];
  //sprintf_s( text, sizeof(text), "%s.Жопа", pszFilePath );
  //HFILE hFile = _lcreat( text, 0 );
  //UINT nCount = mTags.Count();
  //STagsObj* a = (STagsObj*)mTags.m_szBuffer;
  //for ( UINT n = 0; n < nCount; n++ )
  //  {
  //  const char* name = mName.Name( a[n].m_nName );
  //  const char* comm = mName.Name( a[n].m_nComm );
  //  sprintf_s( text, sizeof(text), "%3d %s(%s)\r\n", a[n].m_eType, name, comm );
  //  _hwrite( hFile, text, lstrlen(text) );
  //  }
  //_lclose( hFile );
  //
	return 0;
	}
//
int CArrayObj::NextShapeFromPoint( POINT point, int find )
  {
  find++;
  int nCnt = mTags.Count();
  STagsObj* obj = (STagsObj*)mTags.m_szBuffer;
  for ( int n = find; n < nCnt; n++)
  if ( ::PtInRect( &obj[n].rc, point ) )
  return n;
  return -1;
  }
//
int CArrayObj::FindShapeFromPoint( POINT point )
  {
  int nCnt = mTags.Count();
  STagsObj* obj = (STagsObj*)mTags.m_szBuffer;
  for ( int n = 0; n < nCnt; n++)
  if ( obj[n].m_eType != id_Dialog )
    {
    if ( obj[n].m_eType == id_Zadv )
      {
      char* pszName = mTags.Comm(n);
      if ( *pszName == '.' )
        {
        if ( _strnicmp( pszName, ".Вх." , 4 ) == 0 ) continue;
        if ( _strnicmp( pszName, ".ВХ." , 4 ) == 0 ) continue;
        if ( _strnicmp( pszName, ".Бп." , 4 ) == 0 ) continue;
        if ( _strnicmp( pszName, ".БП." , 4 ) == 0 ) continue;
        if ( _strnicmp( pszName, ".Вых.", 5 ) == 0 ) continue;
        if ( _strnicmp( pszName, ".ВЫХ.", 5 ) == 0 ) continue;
        if ( _strnicmp( pszName, ".БП2.", 5 ) == 0 ) continue;
        }
      }
    if ( ::PtInRect( &obj[n].rc, point ) )
      return n;
    }
  return -1;
  }
//
INT_PTR CArrayObj::FindShapeFromRect ( RECTL& rcl, int type )
  {
  CRect rc = ::GetRect(rcl);
  // На фоне горького опыта
  if ( rc.Width() > 1000 || rc.Height() > 1000 ) return -1;
  POINT point = rc.CenterPoint();
  //
  int nCnt = mTags.Count();
  STagsObj* obj = (STagsObj*)mTags.m_szBuffer;
  for ( int n = 0; n < nCnt; n++)
  if ( obj[n].m_eType == type )
  if ( ::PtInRect( &obj[n].rc, point ) )
  return n;
  return -1;
  }
//--------------------------------------------------------------------------------
void CArrayObj::BuildClient( CSocketClient& sockClient)
  {
  CharMP szName;
  int nCnt = mTags.Count();
  STagsObj* obj = (STagsObj*)mTags.m_szBuffer;
  sockClient.Begin();
  for( int n = 0; n < nCnt; n++ )
    {
    lstrcpy( szName, mName.Name(obj[n].m_nName) );
    UINT mbType = obj[n].m_eType;
	if (obj[n].m_eType>id_ModelData)
	{
	    obj[n].nModelDialog=obj[n].m_eType;
		obj[n].m_eType=id_ModelData;
		mbType =id_ModelData;
	}
    switch ( obj[n].m_eType )
      {
      case id_SensPlace  :
      case id_Sensor_45  :
	  case id_Sensor_46  :
	  case id_Sensor_47  :
      case id_Level      : mbType = id_Sensor; break;
      //
	  case id_ZadvZaglushka:
      case id_ShBallZadv :mbType = id_Zadv; break;
	  case id_Ventil     :mbType = id_ModelData; break;
	  case id_3xZadv     :mbType = id_ModelData; break;
      case id_ElectroNagrev:mbType = id_ModelData; break;
      //
      case id_ShZaslonka : mbType = id_HS; break;
      //
  	  case id_HS         :
        {
        int nTeg = ::FindTagEx( mName.Name(obj[n].m_nName), obj[n].m_eType );
        if ( nTeg >= 0 )
          obj[n].color = ::TegValueI( nTeg, "invert" );
        }
        break;
      //
  	  case id_Pump      :
        {
        char* name = mName.Name(obj[n].m_nName);
        int nTeg = ::FindTagEx( name, obj[n].m_eType );
        if ( nTeg < 0 )
          {
          nTeg = ::FindTag( name, obj[n].m_eType );
          if ( nTeg >= 0 )
            {
            obj[n].m_nSocketNumber = sockClient.FindObj( nTeg, name, NULL );
            continue;
            }
          }
        }
        break;
      case id_Sensor    :
  	  case id_KVO       :
	  case id_PHS:
		  break;
	  case id_Klapan    :	{
							char* name = mName.Name(obj[n].m_nName);
							if (CZadvigkiFromKlapan::IsZadvigkaFromKlapanSborka(name))
							{
								int nShift=CZadvigkiFromKlapan::GetKlapanNameShift(name);
								name+=nShift;
							}
							obj[n].m_nSocketNumber= sockClient.AddSxema( name, id_Klapan );
							continue;
							}
							break;
      case id_Zadv      :
	  case id_ModelData :
      case id_Forsunka  : break;
      case id_Turbina   :
      case id_ShLocReg  :
        mbType = id_Value;
        if ( strchr( szName, '.' ) == NULL )
          lstrcat( szName, ".PV" );
      break;
      default: continue;
      };
	obj[n].m_nSocketNumber= sockClient.AddSxema( szName, mbType );
    if (obj[n].m_eType==id_Ventil)
		obj[n].m_colorModelScheme=eColor_0_100;
	if (obj[n].m_eType==id_3xZadv)
		obj[n].m_colorModelScheme=eColor_0_1;
	if (obj[n].m_eType==id_ElectroNagrev)
		obj[n].m_colorModelScheme=eColor_0_1;
	if (mbType==id_ModelData)
	{
		//::EnterCriticalSection(&CArrayObj::g_lock);
		//sockClient.GetModelData(mbType,obj[n].m_nSocketNumber, szName, mName.Name(obj[n].m_nComm));
		//::LeaveCriticalSection(&CArrayObj::g_lock);
	}
    //
	  }
  SetEvent(hEvent);
  }
//--------------------------------------------------------------------------------
inline int Vn( double d, int n )
  {
  if ( n >= 0 ) return n;
  double v = fabs(d);
  while ( v > 1 )
    {
    v *= 0.1,n++;
    if ( n == 0 ) break;;
    }
  return -n;
  }
//
void CArrayObj::DrawAllShapes( HDC hDC, CSocketClient& sockClient )
  {
  int nCnt = mTags.Count();
  STagsObj* obj = (STagsObj*)mTags.m_szBuffer;
  if ( (GetAsyncKeyState ( VK_CONTROL ) & 0x8000) != 0 )
  {
    HBRUSH hBrush = ::CreateSolidBrush(0x0000FF);
    if ( hBrush )
      {
	    for( int n = 0; n < nCnt; n++ )
		    {
		  CString strName = mTags.Name(n);
		  CString strFind;
		  if ((obj[n].m_eType==id_ModelData)&&(obj[n].nModelDialog>id_ModelData))
				strFind=theApp.FindPointInArray(obj[n].nModelDialog,strName);
		  else
				strFind=theApp.FindPointInArray(obj[n].m_eType,strName);
		  if (strFind!="Не найдены")
		  {
				::FrameRect(hDC,&obj[n].rc,hBrush);
				RECT rc=obj[n].rc;
				for (int i=0;i<3;i++)
				{
					::InflateRect( &rc, 1, 1 );
					::FrameRect(hDC,&rc,hBrush);
				}
		  }
		    }
      ::DeleteObject(hBrush);
      }
  }
  if ( ::IsMnemoStyle(MNEMO_BBOX) )
    {
    HBRUSH hBrush = ::CreateSolidBrush(0x0080FF);
    if ( hBrush )
      {
      char szText[32];
      ::SetTextColor( hDC, 0xff0000 );
	    for( int n = 0; n < nCnt; n++ )
		    {
        ::FrameRect(hDC,&obj[n].rc,hBrush);
        if ( ::IsMnemoStyle(MNEMO_EDIT) )
          {
          sprintf_s( szText, sizeof(szText), "%d", obj[n].m_eType );
          ::DrawText( hDC, szText, lstrlen( szText ), &obj[n].rc,
            DT_SINGLELINE|DT_VCENTER|DT_CENTER );
          }
		    }
      ::DeleteObject(hBrush);
      }
    }
  //
  COLORREF color;
  double dValue;
  bool bIgnoreFillFigure;
  //
	for( int n = 0; n < nCnt; n++ )
  //if ( obj[n].m_nNumber != -1 )
  if ( obj[n].m_nSocketNumber != -1 )
	  {
    bIgnoreFillFigure=false;
    color = 0x0080FF;
    //CShBase& sh = client.SH( obj[n].m_nNumber );
	//UINT eType=client.GetMnemoType(obj[n].m_nNumber );
	CShBase& sh = sockClient.SH( obj[n].m_nSocketNumber );
	UINT eType=sockClient.GetMnemoType(obj[n].m_nSocketNumber );
    if ( IsTypeOK )
    switch ( eType )
      {
  	  case id_KVO       :
        {
        CShKVO& SH = (CShKVO&)sh;
      //color = ( SH.PV != 0 ) ? 0x00FF00 : 0x0000FF;
        color = SH.Eq_State( CKVO::On ) ? 0x00FF00 : 0x0000FF;
        }
      break;
  	  case id_Pump      :
        {
        CShPump& SH = (CShPump&)sh;
      //color = ( SH.PV != 0 ) ? 0x00FF00 : 0x0000FF;
        color = SH.Eq_State( CPumpData::Work ) ? 0x00FF00 : 0x0000FF;
        }
      break;
	  case id_PHS       :
        {
				CShPolyHS& SH = (CShPolyHS&)sh;
				color=(SH.SP==1)?0x00FF00 : 0x000000;
        }
      break;
  	  case id_HS        :
        {
        CShHS& SH = (CShHS&)sh;
        if ( obj[n].color == 1 )
          color = ( SH.mOn != 0 ) ? 0x0000FF : 0x00FF00;
        else
          color = ( SH.mOn != 0 ) ? 0x00FF00 : 0x0000FF;
        }
      break;
      case id_Forsunka:
        {
        CShForsunka& SH = (CShForsunka&)sh;
      //color = ( SH.isFIRE & 0x2 ) ? 0x0000FF : 0x000000F;
        color = ( SH.isFire ) ? 0x0000FF : 0x000000F;
        }
      break;
      case id_Klapan:
        {
        CShKlapan& SH = (CShKlapan&)sh;
        dValue = SH.Vent;
		char* name = mName.Name(obj[n].m_nName);
		if (CZadvigkiFromKlapan::IsVentIn(name))
			dValue = SH.Vent_In;
		else
		if (CZadvigkiFromKlapan::IsVentOut(name))
			dValue = SH.Vent_Out;
		else
			if (CZadvigkiFromKlapan::IsVentBp(name))
				dValue = SH.Vent_BP;
			else
				if (CZadvigkiFromKlapan::IsVentBp2(name))
					dValue = SH.Vent_BP2;
			    else
				if (CZadvigkiFromKlapan::IsDrainIn(name))
					dValue = SH.Vent_Drainage_In;
				else
				if (CZadvigkiFromKlapan::IsDrainOut(name))
					dValue = SH.Vent_Drainage_Out;
             if ( dValue < 1.0 ) color = 0x0000FF;// Красный
        else if ( dValue < 10. ) color = 0x0099FF;// Оранжевый
        else if ( dValue < 50. ) color = 0x00FFFF;// Жёлтый
        else                     color = 0x00FF00;// Зелёный
        }
      break;
      case id_Zadv:
        {
        CShZadv& SH = (CShZadv&)sh;
        dValue = SH.Vent;
             if ( dValue < 1.0 ) color = 0x0000FF;// Красный
        else if ( dValue < 10. ) color = 0x0099FF;// Оранжевый
        else if ( dValue < 50. ) color = 0x00FFFF;// Жёлтый
        else                     color = 0x00FF00;// Зелёный
		if (obj[n].m_eType==id_3xZadv)
		{
			COLORREF clrSecond=( dValue < 1.0)?0x00FF00 : 0x0000FF;
			TransparentRect( hDC, obj[n].rc, clrSecond, 0x040302 );
		}
		else
		if (obj[n].m_eType==id_ZadvZaglushka)
		{

			COLORREF clrSecond=( SH.bZaglushka)?0x0000FF : 0x00FF00;
			TransparentRect( hDC, obj[n].rc, clrSecond, 0x040302 );
		}
        }
      break;
      case id_ModelData:
        {
			if (obj[n].nModelDialog==id_Reductor)
				bIgnoreFillFigure=true;
			CShModelData& SH = (CShModelData&)sh;
			dValue = SH.dValue;
			if (obj[n].m_colorModelScheme==eColor_0_100)
			{
				if ( dValue < 1.0 ) color = 0x0000FF;// Красный
				else if ( dValue < 10. ) color = 0x0099FF;// Оранжевый
				else if ( dValue < 50. ) color = 0x00FFFF;// Жёлтый
				else                     color = 0x00FF00;// Зелёный
			}
			else
			if (obj[n].m_colorModelScheme==eColor_0_1)
			{
				if ( dValue < 1.0 ) color = 0x0000FF;// Красный
				else color = 0x00FF00;// Зелёный
				if (obj[n].nModelDialog==id_3xZadv)
				{
					COLORREF clrSecond=( dValue < 1.0)?0x00FF00 : 0x0000FF;
					TransparentRect( hDC, obj[n].rc, clrSecond, 0x040302 );
				}
			}
        }
      break;
      case id_Value:
      break;
      case id_Sensor   :
        {
        if ( obj[n].m_eType == id_Level )
          {
          CShSensor& SH = (CShSensor&)sh;
          dValue = SH.SensValue/100.0;
          CRect rc = obj[n].rc;
          int h = int(dValue*rc.Height());
          rc.top = rc.bottom-h;
          if ( rc.top > rc.bottom ) rc.top = rc.bottom;
          if ( rc.top < obj[n].rc.top ) rc.top = obj[n].rc.top;
          TransparentStd( hDC, rc, 0xff0000/*obj[n].color*/ );
          }
        else
        if ( obj[n].m_eType == id_SensPlace )
          {
          CShSensor& SH = (CShSensor&)sh;
          dValue = SH.ShowValue;
          char text[512];
          static char fmt[8] = "%.0f";
          fmt[2] = '0'+Vn(dValue,obj[n].nDots);
          sprintf_s( text, 512, fmt, dValue );
          CMyFonts font( hDC, obj[n].color );
          font.GiveArial( obj[n].nFont );
          //font.GiveCourier( 18 );
          int nMode = ::SetBkMode( hDC, TRANSPARENT );
          font.Draw( obj[n].rc, text );
          ::SetBkMode( hDC, nMode );
          }
		else
        if ( ( obj[n].m_eType == id_Sensor_45 ) )
          {
          CShSensor& SH = (CShSensor&)sh;
		  dValue = SH.ErrValue/100.0;
          CRect rc = obj[n].rc;  
		  int h = rc.Height();
		  double kTop = 0.18, kBottom = 0.15;
		  rc.bottom -= LONG(h * kBottom);
		  rc.top    += LONG(h * kTop);
          rc.top = rc.bottom - int(dValue * rc.Height());
          if ( rc.top > rc.bottom ) rc.top = rc.bottom;
          if ( rc.top < obj[n].rc.top ) rc.top = obj[n].rc.top;
		  TransparentRect( hDC, rc, 0x00ff00, 0x060504 );
          }
		else
			if (( obj[n].m_eType == id_Sensor_46 )||( obj[n].m_eType == id_Sensor_47 ))
				color = 0x0;//MIHAIL иначе красились в оранжевый бантики
        }
        continue;
      default:
        continue;
      };
		if (!bIgnoreFillFigure)
		{
			TransparentStd( hDC, obj[n].rc, color );
		}
	  }
  //
  if ( ::IsMnemoStyle(MNEMO_EDIT) )
	for( int n = 0; n < nCnt; n++ )
  if ( obj[n].m_eType == id_Goto )
  if ( PulsTimer() && obj[n].color != 0 )
    {
    CMyBrush brush( hDC, obj[n].color );
    brush.Rect( obj[n].rc );
    }
  //
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Рекурсивное построение меню
bool CArrayObj::Menu(HMENU hMenu,UINT id,const char* pszCurr,bool bOwnerDraw,HXML nItem)
  {
  if ( nItem == INVALID_HXML )
    {
    nItem = FindItem( nItem, "Root" );
    if ( nItem == INVALID_HXML ) return false;
    nItem = FindItem( nItem, "Item" );
    if ( nItem == INVALID_HXML ) return false;
    }
  // Проход по дочерним элементам
  while ( nItem != INVALID_HXML )
    {
    int mf = IsCheck( nItem, pszCurr ) ? MF_CHECKED : 0;
    if ( bOwnerDraw ) mf |= MF_OWNERDRAW;
    const char* menu = Param( nItem, "menu" );
    SItemXML& item = Item( nItem );
    if ( item.m_nChild == INVALID_HXML )
      ::AppendMenu(hMenu,MF_BYCOMMAND|MF_ENABLED|mf ,UINT(nItem)+id,menu);
    else
      {
      HXML nChild = FindItem( nItem, "Item" );
      if ( nChild != INVALID_HXML )
        {
        HMENU hPopup = ::CreatePopupMenu();
        if ( hPopup != NULL )
          {
          ::AppendMenu(hMenu,MF_POPUP|MF_ENABLED|mf,(UINT_PTR)(hPopup),menu);
          Menu(hPopup,id,pszCurr,bOwnerDraw,nChild);
          }
        }
      }
    nItem = NextItem( nItem, "Item" );
    }
  return true;
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Поиск текста меню по имени файла
const char* CArrayObj::FileToMenu(const char* pszFile,HXML nItem)
  {
  if ( nItem == INVALID_HXML )
    {
    nItem = FindItem( nItem, "Root" );
    if ( nItem == INVALID_HXML ) return NULL;
    nItem = FindItem( nItem, "Item" );
    if ( nItem == INVALID_HXML ) return NULL;
    }
  // Проход по дочерним элементам
  while ( nItem != INVALID_HXML )
    {
    SItemXML& item = Item( nItem );
    if ( item.m_nChild == INVALID_HXML )
      {
      const char* menu = Param( nItem, "menu" );
      const char* file = Param( nItem, "file" );
      if ( lstrcmpi( file, pszFile ) == 0 )
        return menu;
      }
    else
      {
      HXML nChild = FindItem( nItem, "Item" );
      const char* pszMenu = FileToMenu(pszFile,nChild);
      if ( pszMenu != NULL )
        return pszMenu;
      }
    nItem = NextItem( nItem, "Item" );
    }
  return NULL;
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Перебор всех пунктов меню
HXML CArrayObj::NextMenu(HXML nItem)
  {
  UINT nCount = CountAll();
  for ( UINT n = UINT(nItem)+1; n < nCount; n++ )
    {
    SItemXML& item = Item( (HXML)n );
    if ( item.m_nChild != INVALID_HXML )
      continue;
    const char* name = Name( (HXML)n );
    if ( lstrcmpi( name, "item" ) == 0 )
      return HXML(n);
    }
  return INVALID_HXML;
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int CArrayObj::CountMenu()
  {
  int n = 0;
  HXML nItem = NextMenu();
  while ( nItem != INVALID_HXML ) nItem = NextMenu(nItem),n++;
  return n;
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Проверка - надо ли ставить 'галку' у пункта меню
bool CArrayObj::IsCheck(HXML nItem,const char* pszCurr)
  {
  SItemXML& item = Item( nItem );
  if ( item.m_nChild == INVALID_HXML )
    {
    const char* menu = Param( nItem, "menu" );
    return lstrcmpi(pszCurr,menu) == 0;
    }
  nItem = CFileXML::FindItem( nItem, "Item" );
  while ( nItem != INVALID_HXML )
    {
    if ( IsCheck( nItem, pszCurr ) )
      return true;
    nItem = NextItem( nItem, "Item" );
    }
  return false;
  }
//--------------------------------------------------------------------------------
bool CArrayObj::ChangeMenu( const char* pszFile )
  {
  if ( lstrcmpi( m_strMenu, pszFile ) == 0 )
    {
    HXML nItem = NextMenu();
    if ( nItem != INVALID_HXML )
      return true;
    // В противном случае - пробуем ещё раз
    }
  m_strMenu = pszFile;
  //
  CModelInfo& model = ::ModelInfo();
  char szFile[_MAX_PATH];
  sprintf_s( szFile, sizeof(szFile), "%s%s.xml", model.dirScheme, pszFile );
  //
  int nReturn = Load( szFile );
  return nReturn == 0;
  }
//--------------------------------------------------------------------------------
