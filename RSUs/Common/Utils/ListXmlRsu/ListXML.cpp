#include "stdafx.h"
//#include <CommProc.h>
#include <ListXML.h>
//#include <FormatScn.h>
//
char CListXML::ms_chVer = 2;
//
CListXML::CListXML()
: mItem(mPara)
, mPara(mName)
, mName(mChar,1)//lstrcmpi
, m_buffALL(NULL)
, m_nCode(-1)
  {
  }
//
CListXML::~CListXML()
  {
  ClearAll();
  }
//
void CListXML::ClearAll()
  {
  if ( m_buffALL )
    {
    ::BuffFree(m_buffALL);
    mItem.m_szBuffer = NULL;
    mPara.m_szBuffer = NULL;
    mName.m_szBuffer = NULL;
    mChar.m_szBuffer = NULL;
    m_buffALL = NULL;
    }
  mItem.Clear();
  mPara.Clear();
  mName.Clear();
  mChar.Clear();
  }
//
UINT CListXML::Load( const char* pszFile )
  {
  char szFullBin[_MAX_PATH],szPath[_MAX_PATH];
  _fullpath(  szPath, pszFile, _MAX_PATH );
  GetBinFile( szFullBin, szPath );
#ifndef _WIN64
  strcat_s(szFullBin, ".32"); // разные имена bin-файлов для x32 и x64 
#endif
  return Read( szPath, szFullBin );
  }
//
UINT CListXML::Read( const char* pszFile, const char* pszBin )
  {
  ClearAll();
  //
  _WIN32_FILE_ATTRIBUTE_DATA attr;
  char szFullBin[_MAX_PATH];
  if ( pszBin != NULL )
    {
    // Ищем бинарный файл и сравниваем с последней датой изменения
    if ( Binary( pszFile, pszBin, attr, szFullBin ) )
      return 0;
    }
  //
  char* szBuffer = ::FileToBuff( pszFile );
  if ( szBuffer == NULL ) return 1;
  //
  // Созда¸м пустую строку
  if ( !mChar.Realloc(1) )return 10;
  mChar.AddCurr();
  //
  char* ptr = szBuffer;
  STegScn* scn = NULL;
  int      nCount = 0;
  SUniChar uni;
  UINT nReturn = Parser( ptr, INVALID_HXML, scn, nCount, uni );
  ::BuffFree( szBuffer );
  ::ParserFree(scn);
//Test( pszFile );
  if ( nReturn == 0 )
    {
    nReturn = TestFormat(0);
    UINT nCount = mItem.Count();
    for ( UINT n = 0; n < nCount; n++ )
      {
      SItemXML& item = Item(HXML(n));
      if ( nReturn == 0 )
      if ( item.m_nCount & 0x80000000 )
        {
        //Жопа, но этого не должно быть
        }
      item.m_nCount &= 0x7fffffff;
      }
    }
  if ( nReturn == 0 && pszBin != NULL )
    Test( szFullBin );
  if ( nReturn == 0 )
  if ( pszBin != NULL )
    Save( szFullBin, attr );
  //
  return nReturn;
  }
//
void CListXML::Save( const char* pszFile, _WIN32_FILE_ATTRIBUTE_DATA& attr )
  {
  HFILE hFile = _lcreat( pszFile, 0 );
  if ( hFile != HFILE_ERROR )
    {
    _hwrite( hFile, &ms_chVer, 1 );
    _hwrite( hFile, (char*)&attr.ftLastWriteTime, sizeof(FILETIME) );
    mItem.Write( hFile );
    mPara.Write( hFile );
    mName.Write( hFile );
    mChar.Write( hFile );
    _lclose( hFile );
    }
  }
//
bool CListXML::Binary( const char* pszFile, const char* pszBin, _WIN32_FILE_ATTRIBUTE_DATA& attr, char* pszFullBin )
  {
  sprintf_s( pszFullBin, _MAX_PATH, "%s.bin", pszBin );
  if ( !GetAttr( pszFile, attr ) )
    return false;
  //
  DWORD dwSize;
  char* szBuffer = ::FileToBuff( pszFullBin, dwSize );
  if ( szBuffer == NULL ) return false;
  //
  #define Return { ::BuffFree(szBuffer); return false;}
  char* ptr = szBuffer;
  if ( *ptr != ms_chVer ) Return;
  ptr++;
  //
  FILETIME* F = (FILETIME*)ptr;
  if ( !EqAttr( attr, F[0] ) ) Return;
  ptr += sizeof(FILETIME);
  //
  mItem.Read( ptr );
  mPara.Read( ptr );
  mName.Read( ptr );
  mChar.Read( ptr );
  //
  m_buffALL = szBuffer;
  return true;
  }
//
UINT CListXML::TestFormat(HXML N)
  {
  while ( N != INVALID_HXML )
    {
    SItemXML& item = Item(N);
    //const char* nameI  = Name (N);
    //const char* valueI = Value(N);
    //const char* name1  = Name (N,1);
    //const char* value1 = Value(N,1);
    if ( item.m_nCount & 0x80000000 )
      return 100;
    item.m_nCount |= 0x80000000;
    if ( item.m_nСhild != INVALID_HXML )
      {
      UINT nReturn = TestFormat( item.m_nСhild );
      if ( nReturn )
        return nReturn;
      }
    N = item.m_nNext;
    }
  return 0;
  }
//
// Переводим возможный UNICODE в нормальные символы
const char* UTF8( const char* pText, SUniChar& uni )
  {
  int dl = lstrlen(pText)+1;
  if ( dl > 1 )
    {
    if ( !uni.Realloc( dl*4 ) )
      return false;
    WCHAR* w = (WCHAR*)uni.m_szBuffer;
     CHAR* s = ( CHAR*)uni.m_szBuffer+dl*2;
    int a1 = MultiByteToWideChar(CP_UTF8,0,pText,dl,w,dl);
    int b1 = WideCharToMultiByte(CP_ACP,0,w,dl,s,2*dl,0,0);
    if ( a1 == 0 || b1 == 0 )
      return false;
    return s;
    }
  return pText;
  }
//
inline void Shift( char* text, int N )
  {
  while ( *text )
    {
    *text = text[N];
    text++;
    }
  }
//
void AnalizText( const char* pszText )
  {
  if ( lstrcmp( pszText, "&lt;None&gt;" ) == 0 )
    KKK();
  char* text = (char*)pszText;
  //
  int n = 0;
  while ( text[n] )
    {
    if ( text[n] == '&' )
      {
      if ( _strnicmp( text+n, "&nbsp;", 6 ) == 0 )
        text[n] = ' ', Shift( text+n+1, 5 );
      else
      if ( _strnicmp( text+n, "&quot;", 6 ) == 0 )
        text[n] = '"', Shift( text+n+1, 5 );
      else
      if ( _strnicmp( text+n, "&lt;", 4 ) == 0 )
        text[n] = '<', Shift( text+n+1, 3 );
      else
      if ( _strnicmp( text+n, "&gt;", 4 ) == 0 )
        text[n] = '>', Shift( text+n+1, 3 );
      else
        {
        ASS(0);
        text[n] = 0;
        break;
        }
      }
    n++;
    };
  }
//
bool CListXML::SUniPara::Add ( const char* pName, const char* pValue, SUniChar& uni, UINT nCode )
  {
  if ( !Realloc(1) ) return false;
  SLocal* l = (SLocal*)m_szBuffer;
  UINT n = AddCurr();
  //
  if ( nCode == CP_UTF8 ) pName  = UTF8( pName , uni );
  AnalizText( pName );
  l[n].m_nName  = name.Add( pName  );
  //
  if ( nCode == CP_UTF8 ) pValue = UTF8( pValue, uni );
  AnalizText( pValue );
  l[n].m_nValue = name.Add( pValue );
  //
  return l[n].m_nName != INVALID_HNAME && l[n].m_nValue != INVALID_HNAME;
  }
//
UINT CListXML::Parser( char*& ptr, HXML nParent, STegScn*& scn, int& nCount, SUniChar& uni )
  {
  static char* cdata = "<![CDATA[";
  static int   ldata = lstrlen(cdata);
  static char* ccomm = "<!--";
  static int   lcomm = lstrlen(ccomm);
  static char* nnn = "/";
  static char* ppp = "";
  static STegScn tag[2];
  STegScn* teg;
  HXML nNext = INVALID_HXML;
  int nReturn;
  while ( *ptr )
    {
    SkipSpace( ptr );
    //
    if ( _strnicmp( ptr, cdata, ldata ) == 0 )
      {
      ptr += ldata;
      SkipSpace( ptr );
      tag[0].name  = cdata+1;
      tag[0].value = ptr;
      tag[1].name  = nnn;
      tag[1].value = ppp;
      //
      char* p = strstr( ptr, "]]>" );
      if ( p == NULL ) return 11;
      *p = 0;
      ptr = p+3;

      teg = tag;
      nReturn = 2;
      }
    else
    if ( _strnicmp( ptr, ccomm, lcomm ) == 0 )
      {
      ptr += ldata;
      //
      char* p = strstr( ptr, "-->" );
      if ( p == NULL ) return 11;
      ptr = p+3;
      continue;
      }
    else
      {
      nReturn = ::ParserXML( ptr, scn, nCount );
      teg = scn;
      }
    //
    if ( nReturn > 0 ) //Все нормально
      {
      if ( *teg[0].name == '/' )
        {
        if ( nParent == INVALID_HXML )
          return 4;
        const char* name = Name(nParent);
        const char* para = teg[0].name+1;
        if ( m_nCode == CP_UTF8 ) para = UTF8(para,uni);
        if ( lstrcmpi( name, para ) )
          return 5;
        return 0;
        }
      else
        {
        bool bChild = lstrcmpi( teg[nReturn-1].name, "/" ) == 0;
        if ( bChild ) nReturn--,teg[nReturn].name = NULL;
        //
        if ( !mItem.Realloc(1) )
          return 6;
        if ( !mPara.Realloc(nReturn) )
          return 7;
        //
        HXML nItem = (HXML)mItem.AddCurr();
        SItemXML& obj = Item(nItem);
        obj.m_nCount  = nReturn;
        obj.m_nPlace  = mPara.Count();
        obj.m_nParent = nParent;
        obj.m_nPrev   = nNext;
        obj.m_nСhild  = INVALID_HXML ;
        obj.m_nNext   = INVALID_HXML ;
        obj.m_nFree   = INVALID_HNAME;
        obj.m_nData   =-1;
        //
        if ( nNext != INVALID_HXML )
          {
          SItemXML& next = Item(nNext);
          next.m_nNext = nItem;
          }
        nNext = nItem;
        //
        for ( int n = 0; n < nReturn; n++ )
          if ( !mPara.Add(teg[n].name,teg[n].value,uni,m_nCode) )
            return 8;
        if ( bChild ) continue;
        if ( lstrcmpi( teg[0].name, "?xml" ) == 0 )
          {
          for ( int n = 1; n < nReturn; n++ )
          if ( lstrcmpi( teg[n].name, "encoding" ) == 0 )
            {
            if ( lstrcmpi( teg[n].value, "windows-1251" ) == 0 )
              m_nCode = 1251;
            else
            if ( lstrcmpi( teg[n].value, "utf-8" ) == 0 )
              m_nCode = CP_UTF8;
            break;
            }
          continue;
          }
        //
        UINT oldСount = mItem.Count();
        obj.m_nСhild  = (HXML)oldСount;
        UINT nParser  = Parser( ptr, nItem, scn, nCount, uni );
        UINT newСount = mItem.Count();
        if ( newСount == oldСount )
          obj.m_nСhild = INVALID_HXML;
        //
        if ( nParser != 0 )
          return nParser;
        }
      }
    else
    if ( nReturn == -1 ) //Текстовое оформление
      {
      SItemXML& item = Item( nParent );
      const char* name = Name( nParent );
      char szFind[256];
      sprintf_s( szFind, "</%s>", name );
      char* find = strstr( ptr, szFind );
      if ( find == NULL )
        return 2;
      //
      *find = 0;
      if ( *ptr == '"' )
        {
        char* end = find-1;
        ASS( *end == '"' );
        ptr++,*end = 0;
        AnalizText( ptr );
        item.m_nFree = mName.Add(ptr);
        }
      else
        {
        AnalizText( ptr );
        item.m_nFree = mName.Add(ptr);
        }
      *find = '<';
      ptr = find;
      }
    else
    if ( nReturn == 0 ) //Конец документа
      {
      return 0;
      }
    else //Ошибка
      {
      return 3;
      }
    }
  return 0;
  }
//
void CListXML::Test( const char* pszFile )
  {
  char szFile[_MAX_PATH];
  lstrcpy( szFile, pszFile );
  lstrcat( szFile, ".test" );
  HFILE hFile = _lcreat( szFile, 0 );
  if ( hFile != HFILE_ERROR )
    {
    Test( hFile, 0, 0 );
    _lclose( hFile );
    }
  }
//
void CListXML::Test( HFILE hFile, HXML N, int nLevel )
  {
  do
  {
  SItemXML& item = Item(N);
  const char* nameI  = Name (N);
  const char* valueI = Value(N);
  //
  for ( int n = 0; n < nLevel; n++ )
  _hwrite( hFile, "  ", 2 );
  _hwrite( hFile, "<", 1 );
  for ( UINT n = 0; n < item.m_nCount; n++ )
    {
    if ( n > 0 )
    _hwrite( hFile, " ", 1 );
    const char* name  = Name ( N, n );
    const char* value = Value( N, n );
    _hwrite( hFile, name, lstrlen(name) );
    if ( *value )
      {
      _hwrite( hFile, "=\"", 2 );
      _hwrite( hFile, value, lstrlen(value) );
      _hwrite( hFile, "\"", 1 );
      }
    }
  //
  if ( item.m_nСhild == INVALID_HXML && item.m_nFree == INVALID_HNAME )
    {
    if ( item.m_nCount > 1 )
      _hwrite( hFile, "/>\r\n", 4 );
    else
      {
      _hwrite( hFile, "></", 3 );
      _hwrite( hFile, nameI, lstrlen(nameI) );
      _hwrite( hFile, ">\r\n", 3 );
      }
    }
  else
  if ( item.m_nFree != INVALID_HNAME )
    {
    ASS( item.m_nСhild == INVALID_HXML );
    _hwrite( hFile, ">", 1 );
    const char* value = mName.Name(item.m_nFree);
    _hwrite( hFile, value, lstrlen(value) );
    _hwrite( hFile, "</", 2 );
    _hwrite( hFile, nameI, lstrlen(nameI) );
    _hwrite( hFile, ">\r\n", 3 );
    }
  else
    {
    _hwrite( hFile, ">\r\n", 3 );
    Test( hFile, item.m_nСhild, nLevel+1 );
    for ( int n = 0; n < nLevel; n++ )
    _hwrite( hFile, "  ", 2 );
    _hwrite( hFile, "</", 2 );
    _hwrite( hFile, nameI, lstrlen(nameI) );
    _hwrite( hFile, ">\r\n", 3 );
    }
  N = item.m_nNext;
  }
  while ( N != INVALID_HXML );
  }
//
HXML CListXML::FindItem( HXML nParent, const char* _name )
  {
  HXML N = 0;
  if ( UINT(nParent) < mItem.Count() )
    {
    SItemXML& item = Item(nParent);
    N = item.m_nСhild;
    }
  if ( UINT(N) < mItem.Count() )
    {
    const char* name = Name(N);
    if ( lstrcmpi( name, _name ) == 0  )
       return N;
    return NextItem( N, _name );
    }
  return INVALID_HXML;
  }
//
HXML CListXML::NextItem( HXML nCurent, const char* _name )
  {
  if ( UINT(nCurent) < mItem.Count() )
    {
    SItemXML* item = (SItemXML*)mItem.m_szBuffer;
    HXML N = item[UINT(nCurent)].m_nNext;
    while ( N != INVALID_HXML )
      {
      const char* name = Name(N);
      if ( lstrcmpi( name, _name ) == 0  )
        return N;
      N = item[UINT(N)].m_nNext;
      }
    }
  return INVALID_HXML;
  }
//
HXML CListXML::NextItem( HXML nCurent )
  {
  if ( UINT(nCurent) < mItem.Count() )
    {
    const char* _name = Name( nCurent );
    SItemXML* item = (SItemXML*)mItem.m_szBuffer;
    HXML N = item[UINT(nCurent)].m_nNext;
    while ( N != INVALID_HXML )
      {
      const char* name = Name(N);
      if ( lstrcmpi( name, _name ) == 0  )
        return N;
      N = item[UINT(N)].m_nNext;
      }
    }
  return INVALID_HXML;
  }
//
UINT CListXML::CountItem( HXML nParent, const char* _name )
  {
  UINT nCount = 0;
  HXML N = FindItem( nParent, _name );
  while ( N != INVALID_HXML )
    {
    nCount++;
    N = NextItem( N, _name );
    }
  return nCount;
  }
//
HXML CListXML::IsItems( HXML nParent, const char* _name, UINT nCount )
  {
  UINT n = CountItem( nParent, _name );
  if ( n != nCount )
    return INVALID_HXML;
  return FindItem( nParent, _name );
  }
//
