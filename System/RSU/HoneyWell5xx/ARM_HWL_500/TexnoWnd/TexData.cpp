// DialName.cpp : implementation file
//
#include "stdafx.h"
#include "../ARM.h"
#include "TexData.h"
#include "ArrayObj.h"
#include "TestTime.h"
//
const char CTexData::m_chVer = 4;
CTexData::CTexData()
: mItem(mChar)
  {
  }
  
CTexData::~CTexData()
  {
  ClearAll();
  }

void CTexData::ClearAll()
  {
  mItem.Clear();
  mChar.Clear();
  }
//
CString CTexData::LoadList( class CArrayObj& xml )
  {
  CModelInfo& model = ::ModelInfo();
  TCHAR szCurr[_MAX_PATH], szBin[_MAX_PATH], szXml[_MAX_PATH];
  //
  ::GetCurrentDirectory( _MAX_PATH, szCurr );
  ::SetCurrentDirectory( model.dirScheme );
  CString strError;
  //
  bool bBinary = false;
  sprintf_s( szXml, _MAX_PATH, _T("%s%s.xml"), model.dirScheme, xml.Menu() );
  ::GetBinFile( szBin, szXml );
#ifdef _WIN64
  lstrcat( szBin, ".List64" );
#else
  lstrcat( szBin, ".List32" );
#endif 
  ATTR32 attr;
  if ( ::GetAttr( szXml, attr ) )
    {
    DWORD dwSize;
    char* szBuffer = ::FileToBuff( szBin, dwSize );
    if ( szBuffer )
      {
      bBinary = ReadBin( szBuffer, attr );
      if ( bBinary ) ::BuffFree( szBuffer );
      }
    }
  //
  if ( !bBinary )
    {
    strError = ReadList( xml );
    if ( strError.IsEmpty() )
      SaveBin( szBin, attr, xml );
    }
  ::SetCurrentDirectory( szCurr );
  return strError;
  }
// Чтение бинарного файла
bool CTexData::ReadBin( char* ptr, ATTR32 attr )
  {
  if ( *ptr != m_chVer ) return false;//Версия
  ptr++;
  FILETIME* F = (FILETIME*)ptr;ptr += sizeof(FILETIME);
  if ( !EqAttr(attr,F[0]) ) return false;
  // Проверяем файлы на изменение
  ATTR32 attrCrd,attrEmf;
  TCHAR szEmf[_MAX_PATH], szCrd[_MAX_PATH];
  int* N = (int*)ptr;ptr+=sizeof(int);
  for ( int  n = 0; n < N[0]; n++ )
    {
    F = (FILETIME*)ptr;ptr += 2*sizeof(FILETIME);
    sprintf_s( szEmf, _MAX_PATH, _T("%s.emf" ), ptr );
    sprintf_s( szCrd, _MAX_PATH, _T("%s.crd" ), ptr );
    ptr += lstrlen(ptr)+1;
    if ( GetAttr(szCrd,attrCrd)&&EqAttr(attrCrd,F[0]) )
    if ( GetAttr(szEmf,attrEmf)&&EqAttr(attrEmf,F[1]) )
      continue;
    return false;
    }
  //
  ClearAll();
  mItem.Read( ptr );
  mChar.Read( ptr );
  return true;
  }
// Запись бинарного файла
void CTexData::SaveBin( char* bin, ATTR32 attr, CArrayObj& xml )
  {
  TCHAR  szCrd[_MAX_PATH], szEmf[_MAX_PATH];
  ATTR32 attrCrd,attrEmf;
  //
  HFILE hFile = _lcreat( bin, 0 );
  if ( hFile == HFILE_ERROR ) return;
  //
  _hwrite( hFile, &m_chVer, 1 );
  _hwrite( hFile, (char*)&attr.ftLastWriteTime, sizeof(FILETIME) );
  int nCount = xml.CountMenu();
  _hwrite( hFile, (char*)&nCount, sizeof(int) );
  //
  HXML nItem = xml.NextMenu();
  while ( nItem != INVALID_HXML )
    {
    const char* file = xml.Param( nItem, "file" );
    sprintf_s( szEmf, _MAX_PATH, _T("%s.emf" ), file );
    sprintf_s( szCrd, _MAX_PATH, _T("%s.crd" ), file );
    GetAttr( szCrd, attrCrd ), GetAttr( szEmf, attrEmf );
    _hwrite( hFile, (char*)&attrCrd.ftLastWriteTime, sizeof(FILETIME) );
    _hwrite( hFile, (char*)&attrEmf.ftLastWriteTime, sizeof(FILETIME) );
    _hwrite( hFile, file, lstrlen(file)+1 );
    nItem = xml.NextMenu(nItem);
    }
  //
  mItem.Write( hFile );
  mChar.Write( hFile );
  _lclose( hFile );
  }
//
int SortCrd( const void * d1, const void * d2 )
	{
  SItemCrd* p1 = (SItemCrd*)d1;
  SItemCrd* p2 = (SItemCrd*)d2;
  if ( p1->eType > p2->eType ) return  1; 
  if ( p1->eType < p2->eType ) return -1; 
  return 0;
	}
//
CString CTexData::ReadList( CArrayObj& _xml )
  {
  CString strError;
  //
  CArrayObj xml(1001);
  bool bReturn = xml.ChangeMenu( _xml.Menu() );
  if ( !bReturn )
    {
    strError.Format( "Ошибка чтения '%s'", _xml.Menu() );
    return strError;
    }
  //
  if ( !mChar.Realloc(1) )
    {
    strError = "Ошибка заказа памяти";
    return strError;
    }
  mChar.AddCurr();
  //
  SUniBuffer mOrd(sizeof(int));
  int nError;
  HXML nItem = xml.NextMenu();
  while ( nItem != INVALID_HXML )
    {
    const char* file = xml.Param( nItem, "file" );
    xml.LoadCrd(file,nError,true);
    if ( nError == 0 )
      {
      HCHAR nFile = mChar.AddBuff(file);
      AddItems( xml.mTags, nFile, mOrd );
      }
    else
      {
      if ( strError.IsEmpty() )
      strError = "Ошибки:\n";
      CString str;
      str.Format( "%s %d\n", file, nError );
      strError += str;
      }
    nItem = xml.NextMenu(nItem);
    }
  //
  mOrd.Clear();
  qsort( mItem.m_szBuffer, mItem.Count(), sizeof (SItemCrd), SortCrd );
  return strError;
  }
//
void CTexData::AddItems( SUniTags& mTags, HCHAR nFile, SUniBuffer& mOrd )
  {
  SItemCrd add;
  memset( &add, 0, sizeof(add) );
  add.m_nCount = 1;
  int nCount = mTags.Count();
  if ( !mItem.Realloc(nCount) ) return;
  //
  for ( int n = 0; n < nCount; n++ )
    {
    STagsObj& tag = mTags.Tags(n);
    if ( tag.m_eType == id_Goto   )
    if ( tag.color == 0 ) continue;
    //
	if (tag.m_eType>id_ModelData)
		tag.m_eType=id_ModelData;
    add.eType = (EDataTypes)tag.m_eType;
    add.m_nFile[0] = nFile;
    add.m_nNumb[0] = n;
    char* szTags  = mTags.Comm(n);
	if (( !*szTags )||theApp.IsModelDataType(tag.m_eType))
      szTags = mTags.Name(n);
    //
    int* ord = (int*)mOrd.m_szBuffer;
    int K = mItem.Count();
    int no = 0, nb = -1, ne = K;
    int nRes = -1;
    while ( (ne - nb) > 1 )
      {
      int nn = ( nb + ne ) >> 1;
      no = ord[nn];
      nRes = Lstrcmp( szTags, Name(no) );
      if ( nRes == 0 )
        {
        SItemCrd& item = mItem.Item(no);
        if ( (UINT)item.eType > tag.m_eType ) nRes =  1; 
        if ( (UINT)item.eType < tag.m_eType ) nRes = -1; 
        if ( nRes == 0 )
        break;
        }
      if ( nRes < 0 ) ne = nn;
      else            nb = nn;
      }
    // Добавляем имя файла
    if ( nRes == 0 )
      {
      SItemCrd& item = mItem.Item(no);
      int yy = 90;
      }
    // Новое имя сразу за nb
    else
    if ( mOrd.Realloc(1) )
      {
      HCHAR nChar = mChar.AddBuff(szTags);
      if ( nChar != INVALID_HCHAR )
        {
        add.m_nName = nChar;
        memcpy( mItem.m_szBuffer+mItem.m_nCurrent, &add, sizeof(add) );
        mItem.AddCurr();
        mOrd .AddCurr();
        ord = (int*)mOrd.m_szBuffer;
        for ( int nn = K - 1; nn > nb; nn-- )
        ord[nn+1] = ord[nn];
        ord[nb+1] = K;
        }
      }
    //
    }
  }
