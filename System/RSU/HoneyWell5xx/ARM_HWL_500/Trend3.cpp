// Trend3.cpp : implementation file
//
#include "stdafx.h"
#include "ARM.h"
#include "Trend3.h"
//
#include "ViewTrend3.h"
#include "ViewTrend4.h"
#include "ViewTrend.h"
#include <algorithm>
#include <fstream>
#include <map>
//CDataTrends& CViewTrend   ::Data() { _static CDataTrends mData("Groups.txt","GROUP"); return mData; };
CDataTrends& CViewTrend   ::Data() { _static CDataTrends mData("GroupDisplay.csv",""); return mData; };
CDataTrends& CViewTrend4  ::Data() { _static CDataTrends mData("TrendsGroup.csv",""); return mData; };
CDataTrends& CViewTrend3  ::Data() { _static CDataTrends mData("TrendsGroup.csv",""); return mData; };
//
// CDataTrends
CDataTrends::CDataTrends(const char* pszFile,const char* find)
: SUniBuffer( sizeof(STrends) ), ms_chVer(2)
, mName( mChar )
  {
  LoadFile(pszFile,find);
  }
//
CDataTrends::~CDataTrends()
  {
// Сохранение только при нажатии кнопки
//SaveFile();
  }
//
void CDataTrends::FillCombo( CComboBox& wnd )
  {
  STrends* a = (STrends*)m_szBuffer;
  int nCount = Count();
  for ( int n = 0; n < nCount; n++ )
  if ( a[n].nNumber > 0 )
    {
    LPCTSTR name = Text( a[n].hTitle );
    int m = wnd.AddString(name);
    wnd.SetItemData( m, n );
    }
  }

std::map<std::string,int> CDataTrends::FillMap()
{
  std::map<std::string,int> mapPointInTrendsGroup;
  STrends* a = (STrends*)m_szBuffer;
  int nCount = Count();
  for ( int n = 0; n < nCount; n++ )
  {
	  if ( a[n].nNumber > 0 )
		{
			if (a[n].TrueGrup())
			{
					for(int i = 0; i< 32; i++)
					{
						char szPoint[32];
						char szParam[32];
						lstrcpy( szPoint, Text( a[n].point[i].hPoint ) );
						lstrcpy( szParam, Text( a[n].point[i].hParam ) );
						if (lstrcmp(szPoint,"")==0)
							continue;
						CorrectName(szPoint, szParam);
						std::string pointName(Text(a[n].point[i].hPoint));
						std::string paramName(Text(a[n].point[i].hParam));
						size_t findPoint=paramName.find('.');
						if (findPoint!=std::string::npos)
						{ 
							paramName=paramName.substr(0,findPoint);
						}
						std::string fullStringName=pointName+"."+paramName;
						std::transform(fullStringName.begin(), fullStringName.end(),fullStringName.begin(), ::toupper);
						mapPointInTrendsGroup[fullStringName]=n+1;
					}
			}
		}
  }
  return mapPointInTrendsGroup;
}

//
void CDataTrends::SetCurSel( CComboBox& wnd, int N )
  {
  STrends* a = (STrends*)m_szBuffer;
  int M = 0;
  int nCount = Count();
  for ( int n = 0; n < nCount; n++ )
  if ( a[n].nNumber > 0 )
    {
    if ( n == N )
      {
      wnd.SetCurSel( M );
      break;
      }
    M++;
    }
  }
//
int CDataTrends::ItemNumber(int N)
  {
  if ( N < 1 ) return 0;
  if ( UINT(N) > Count() ) return 0;
  return N-1;
  }
//-----------------------------------------------------------------------------
int CDataTrends::PrevTrend( int N )
  {
  if ( N <= 0 ) return -1;
  STrends* a = (STrends*)m_szBuffer;
  return a[N-1].nNumber;
  }
//
int CDataTrends::NextTrend( int N )
  {
  if ( (N+1) == Count() ) return -1;
  STrends* a = (STrends*)m_szBuffer;
  return a[N+1].nNumber;
  }
//-----------------------------------------------------------------------------
bool SPoint::IsYou( const char* name, class CDataTrends* data )
  {
  if  ( hPoint != INVALID_HNAME )
    {
    const char* text = data->Text(hPoint);
    return lstrcmpi( name, text ) == 0;
    }
  return false;
  }
// Проверка подлинности номера группы
int CDataTrends::TrueGrup( const char* ptr  )
  {
  // Проверяем, чтобы были только цифры
  if ( !*ptr ) return -1;
  if ( !::IsInt(ptr) ) return -1;
  // Ищем группу по номеру
  int N = atoi(ptr);
  STrends* item = Item(N-1);
  if ( item == NULL )
    return -1;
  //
  for ( int n = 0; n < MAX_TREND; n++ )
  if ( item->TrueGrup() )
    return N;
  return -1;
  }
// По имени тэга определяем группу
int CDataTrends::FindGrup( const char* name )
  {
  UINT nCount = Count();
  STrends* item = Item(UINT(0));
  for ( UINT n = 0; n < nCount; n++ )
  if ( item[n].FindGrup( name, this ) )
    return n+1;
  return -1;
  }
//-----------------------------------------------------------------------------
COLORREF arrColor[] =   {0xffff00,0xffffff,0x00ff00,0x00ffff,0xff00ff,0x0000ff,0xff0000,0xcc0066,
						0x6dd72d,0x2f72df, 0x6228da,0xc7cb61,0xa0a0a0,0x38d678, 0x6ad9d2, 0xc15dd4,
						0x616ad9, 0xc44d02,0xc54f5c,0x64cd72,0x3c6cd6,0x9061cf,0x5c6124,0x5c5c5c,
						0x195c1c,0x216165,0x441357,0x141956,0x6c1a20,0x7f2c4a,0x2d5a22,0x142858};
//
void CDataTrends::LoadFile(const char* pszFile,const char* find)
  {
  //
  bool bEmpty = lstrcmpi( pszFile, "Detail.txt" ) == 0;
  //
  CharMP szPath;
  sprintf_s( szPath, "%s%s", ::GetAcyPath(), pszFile );
  ::GetBinFile( m_szBin, szPath, "trend" );
  //
  bool bRead = true;
  if ( GetAttr( szPath, mFileAttr ) || bEmpty )
    {
    DWORD dwSize;
    char* szBuffer = ::FileToBuff( m_szBin, dwSize );
    if ( szBuffer )
      {
      char* ptr = szBuffer;
      if ( *ptr == ms_chVer )//Версия
        {
        ptr++;
        FILETIME* F = (FILETIME*)ptr;
        if ( EqAttr( mFileAttr, F[0] ) || bEmpty )
          {
          ptr += sizeof(FILETIME);
                Read(ptr);
          mName.Read(ptr);
          mChar.Read(ptr);
          bRead = false;
          }
        }
      ::BuffFree( szBuffer );
      }
    }
  //
  if ( bRead )
    {
    DWORD dwSize;
    char* szBuffer = ::FileToBuff( szPath, dwSize );
    if ( szBuffer != NULL )
      {
      ParserTxt(szBuffer,find);
      ::BuffFree( szBuffer );
      SaveFile();
      }
    }
  }
//
void CDataTrends::SaveCvs()
{
	std::string result = "НомерГруппы;ИмяГруппы;Точка1;Точка2;Точка3;Точка4;Точка5;Точка6;Точка7;Точка8;Точка9;Точка10;Точка11;Точка12;Точка13;Точка14;Точка15;Точка16;Точка17;Точка18;Точка19;Точка20;Точка21;Точка22;Точка23;Точка24;Точка25;Точка26;Точка27;Точка28;Точка29;Точка30;Точка31;Точка32\n";
	STrends* a = (STrends*)m_szBuffer;
	int nCount = Count();
	for ( int n = 0; n < nCount; n++ )
	{
		if ( a[n].nNumber > 0 )
		{
			char line[2048] = { 0 };
			sprintf(line, "%d", a[n].nNumber);
			const char* name = Text(a[n].hTitle);
			sprintf(line, "%s;%s;", line, name);
			for (int t = 0; t<MAX_TREND; t++)
			{
				SPoint* pt = &(a[n].point[t]);
				if (INVALID_HNAME == pt->hPoint)
				{
					sprintf(line, "%s;", line);
				}
				else
				{
					const char* s_point = Text(pt->hPoint);
					const char* s_param = Text(pt->hParam);
					if (strlen(s_point) && strlen(s_param))
						sprintf(line, "%s%s.%s;", line, s_point, s_param);
					else
						sprintf(line, "%s;", line);
				}
			}
			line[strlen(line)-1] = 0;
			result += line;
			result += "\n";
		}
	}
	std::string path = GetBasePath();
	path += "DATA\\ACY\\trendsGroup.csv";	
	std::ofstream file(path.c_str(), std::ios::out | std::ios::trunc);
	if (file.is_open())
	{
		file.write(result.c_str(), result.size());
		file.close();
	}
}
void CDataTrends::SaveFile()
  {
  HFILE hFile = _lcreat( m_szBin, 0 );
  if ( hFile != HFILE_ERROR )
    {
    _hwrite( hFile, &ms_chVer, 1 );
    _hwrite( hFile, (char*)&mFileAttr.ftLastWriteTime, sizeof(mFileAttr.ftLastWriteTime) );
          Write( hFile );
    mName.Write( hFile );
    mChar.Write( hFile );
    _lclose( hFile );
    }
  }
//
void CDataTrends::ParserXml(char* ptr,const char* find)
  {
  mChar.AddObj("");
  while ( *ptr )
    {
    int nReturn;
    STegScn* teg = theApp.ParsSCN( ptr, nReturn );
    if ( nReturn > 0 ) //Все нормально
      {
      if ( !Realloc(1) )
        break;
      if ( lstrcmpi( teg[0].name, "group" ) )
        continue;
      STrends obj(arrColor);
      //
      int m;
      for ( int n = 1; n < nReturn; n++ )
      if ( lstrcmpi( teg[n].name, "name" ) == 0 )
        {
        obj.hTitle = AddText(teg[n].value);
        }
      else
      if ( lstrcmpi( teg[n].name, "number" ) == 0 )
        {
        obj.nNumber = atoi(teg[n].value);
        }
      else
      if ( lstrcmpi( teg[n].name, "scaleEP" ) == 0 )
        {
        obj.nScaleEng = atoi(teg[n].value);
        obj.nScaleEng = max( obj.nScaleEng, 0 );
        obj.nScaleEng = min( obj.nScaleEng, 1 );
        }
      else
      if ( lstrcmpi( teg[n].name, "scale" ) == 0 )
        {
        obj.nRazmah = atoi(teg[n].value);
        }
      else
      if ( lstrcmpi( teg[n].name, "select" ) == 0 )
        {
        obj.nSelect = atoi(teg[n].value);
        }
      else
      if ( lstrcmpi( teg[n].name, "int" ) == 0 )
        {
        obj.nInterval = atoi(teg[n].value);
        }
      else if ( StrScn( teg[n].name, "point", m ) ) obj.point[m].hPoint = AddText(teg[n].value);
      else if ( StrScn( teg[n].name, "param", m ) ) obj.point[m].hParam = AddText(teg[n].value);
      else if ( StrScn( teg[n].name, "view" , m ) ) obj.point[m].mView  = atoi(teg[n].value) != 0;
      else if ( StrScn( teg[n].name, "color", m ) ) obj.point[m].color  = ::ParserColors(teg[n].value);
      else if ( StrScn( teg[n].name, "min"  , m ) ) obj.point[m].dMinEU = (float)atof(teg[n].value);
      else if ( StrScn( teg[n].name, "max"  , m ) ) obj.point[m].dMaxEU = (float)atof(teg[n].value);
      else if ( StrScn( teg[n].name, "minPR", m ) ) obj.point[m].dMinPR = (float)atof(teg[n].value);
      else if ( StrScn( teg[n].name, "maxPR", m ) ) obj.point[m].dMaxPR = (float)atof(teg[n].value);
      //
      AddObj( &obj );
      }
    else
    if ( nReturn == -1 ) //Текстовое оформление
      {
      break;
      }
    else
    if ( nReturn == 0 ) //Конец документа
      {
      break;
      }
    else //Ошибка
      {
      break;
      }
    }
  }
//
void CDataTrends::ParserTxt(char* ptr,const char* find)
  {
  char* pts[32];
  mChar.AddObj("");

  //SkipLine(ptr);
  while(*ptr)
  {
	  char* group = ptr;
	  char* name = strchr(ptr, ';'); 
	  ptr = name + 1; 
	  *name = 0; 
	  name++;
	  for(int i = 0; i< 32; i++)
	  {
		  //pts[i] = 0;
		  pts[i] = strchr(ptr, ';'); 
		  ptr = pts[i] + 1; 
		  *pts[i] = 0; 
		  pts[i]++;
	  }
	  char* _end = strchr(ptr, '\n'); 
	  ptr = _end + 1; 
	  *_end = 0; 
	  _end++;
	  // 
	  if ( AS(::IsInt( group )) ) continue;
    
    UINT nGroup = atoi( group );
    if ( AS( 0 < nGroup && nGroup <= 10000     ) ) continue;
    //
    char szText[128];
    for ( UINT n = Count(); n < nGroup; n++ )
      {
      STrends obj(arrColor);
      sprintf_s( szText, sizeof(szText), "Группа %d", n+1 );
      obj.hTitle = AddText(szText);
      obj.nNumber = n+1;
      AddObj( &obj );
      }
    if ( AS( nGroup <= Count() ) ) continue;
    //
    STrends* obj = Item(nGroup-1);
	obj->hTitle = AddText(name);
	for(int i = 0; i< 32; i++)
	{
		if (*pts[i] >= 0x20)
		{
			char * point = pts[i];
			char * param = strchr(pts[i], '.');
			*param++ = 0;
			obj->point[i].hPoint = AddText(point);
			obj->point[i].hParam = AddText(param);
			obj->point[i].mView  = true;
		}
	}
  }

  
  /*while ( *ptr )
    {
    char* point = strstr( ptr, find );
    if ( point == NULL ) break;
    point += 5;
    SkipSpace( point );
    //
    char* group = strchr( point, ' ' );
    if ( AS( group != NULL ) ) return;
    *group++ = 0;
    SkipSpace( group );
    //
    char* nomer = strchr( group, ' ' );
    if ( AS( nomer != NULL ) ) return;
    *nomer++ = 0;
    SkipSpace( nomer );
    //
    char* param = nomer;//strchr( nomer, ' ' );
    while ( BYTE(*param) > 0x20 ) param++;
    if ( *param == 0x20 )
      {
      *param++ = 0;
      SkipSpace( param );
      ptr = param;
      while ( BYTE(*ptr) > 0x20 ) ptr++;
      if ( *ptr ) *ptr++ = 0;
      }
    else
      {
      *param++ = 0;
      ptr = param;
      //
      _static char* empty = "";
      param = empty;
      }
    //
    if ( AS(::IsInt( group )) ) continue;
    if ( AS(::IsInt( nomer )) ) continue;
    UINT nGroup = atoi( group );
    UINT nNomer = atoi( nomer );
    if ( AS( 0 < nNomer && nNomer <= MAX_TREND ) ) continue;
    if ( AS( 0 < nGroup && nGroup <= 10000     ) ) continue;
    nNomer--;
    //
    char szText[128];
    for ( UINT n = Count(); n < nGroup; n++ )
      {
      STrends obj(arrColor);
      sprintf_s( szText, sizeof(szText), "Группа %d", n+1 );
      obj.hTitle = AddText(szText);
      obj.nNumber = n+1;
      AddObj( &obj );
      }
    if ( AS( nGroup <= Count() ) ) continue;
    //
    STrends* obj = Item(nGroup-1);
    obj->point[nNomer].hPoint = AddText(point);
    obj->point[nNomer].hParam = AddText(param);
    obj->point[nNomer].mView  = true;
    //
    }*/
  }
//