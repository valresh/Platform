#include <crossplatform.h>
#include <crossstring.h>
#include "TrendFile.h"
#include "Names.h"
#include "Common.h"
#include "CommProc.h"
#include <macros/AutoCloser.h>
#include <TrendsDefines.h>
#include <iostream>
#include <fstream>
#include <rsus/rsuPaths.h>

//
CTrendFile::CTrendFile()
: m_pHeadT(NULL)
, m_dwName(0)
, mName( mChar )
  {
  memset( &mHeader, 0, sizeof ( CTrendHeader ) );
  }
//
CTrendFile::~CTrendFile()
  {
  CloseTrend();
  m_hHeadMap.unmap();
  m_pHeadT = NULL;
  mName.Clear();
  mChar.Clear();
  }
//
// Открываем файл описаний
bool CTrendFile::OpenTrendHead()
{
  if ( !m_hHeadMap.is_mapped() )
  {
      char szFile[_MAX_PATH * 4];
      sprintf(szFile, "%s_Trends.dat.hdr", rsuGetTrendPath().c_str());
      std::error_code error;
      struct stat st;
      if (stat(szFile, &st) != 0 || st.st_size < (off_t)sizeof(CTrendHeader)) {
          return false;
      }

      m_hHeadMap.map(szFile, 0, sizeof(CTrendHeader), error);
      if (error) {
          return false;
      }

      CTrendHeader* pTmpHeader = (CTrendHeader*)m_hHeadMap.data();
      DWORD kVarInFile = pTmpHeader->kVar;
      size_t dwFullSize = sizeof(CTrendHeader) + (kVarInFile * sizeof(STrendId));

      if (st.st_size < (off_t)dwFullSize) {
          dwFullSize = st.st_size;
      }

      m_hHeadMap.unmap();
      m_hHeadMap.map(szFile, 0, dwFullSize, error);
      if ( m_hHeadMap.is_mapped() )
      {
        m_pHeadT = (CTrendHeaderEx*)m_hHeadMap.data();
      if ( m_pHeadT == NULL )
        {        
        m_hHeadMap.unmap();
        }
      else
        {
        memcpy( &mHeader, m_pHeadT, sizeof(mHeader) );
        mHeader.StartTime = 0;
        }
      }
    }
  //
  if ( m_pHeadT != NULL )
    {
    int nBuild = BuildHeader();
    if ( nBuild == 0 )
      return false;
    }
  //
  return m_hHeadMap.is_mapped();
}
//
bool CTrendFile::OpenTrendData()
{  
  if ( !OpenTrendHead() )
    return false;
  //
  if (m_hDataMap.is_mapped())
  {
      if (mHeader.dwName == m_pHeadT->dwName)
          return true;
      m_hDataMap.unmap();
  }
  mHeader.dwName = m_pHeadT->dwName;
  //
  struct stat st;
  char szFile[_MAX_PATH * 4];
  snprintf(szFile, sizeof(szFile), "%s_Trends.dat", rsuGetTrendPath().c_str());
  std::error_code error;
  if (stat(szFile, &st) != 0) {
      return false;
  }

  size_t dwSize = (size_t)st.st_size;
  if (dwSize == 0) {
      return false;
  }

  m_hDataMap.map(szFile, 0, dwSize, error);
  return m_hDataMap.is_mapped();
}
//
DWORD CTrendFile::CloseTrend()
  {
  m_hDataMap.unmap();
  return 0;
  }
//
int CTrendFile::BuildHeader()
{
  //
  // Проверка на "вшивость"
  bool bFill = false;
  // Достаточно проверить время старта
  if( mHeader.StartTime != m_pHeadT->StartTime )
    bFill = true;
  DWORD kVar = mHeader.kVar;
  DWORD dwName = mHeader.dwName;
  memcpy( &mHeader, m_pHeadT, sizeof(mHeader) );
  mHeader.dwName = dwName;
  if ( !bFill )
    return 1;

  bool bRestored = false;

  char szBin[_MAX_PATH] = { 0 };
  sprintf_s( szBin, _MAX_PATH, "%sServerExTrendPoints.bin", ::GetBinPath() );
  DWORD dwSize = 0;
  KAutoCloser<char*> szBuffer( ::FileToBuff( szBin, dwSize ), ::BuffFree);
  if( dwSize > sizeof(kVar) )
  {
    char* ptr = szBuffer;
    DWORD t = *(DWORD*)ptr;
    ptr += sizeof(kVar);
    if( t==kVar )
    {
      bRestored = true;
      mName.Read(ptr);
      mChar.Read(ptr);
    }
    else
    {
#ifdef _WIN32
      char resCounter[128];
      sprintf_s( resCounter, sizeof(resCounter), "BuildHeader не удалась попытка восстановления при t=%d и kVar=%d \n", t, kVar );
      OutputDebugString( resCounter );
#endif
    }
  }
  //
  if( !bRestored )
  {
#ifdef _WIN32
      OutputDebugString( "BuildHeader" );
    LARGE_INTEGER finish, start;
    QueryPerformanceCounter( &start );
#endif
    //KCritialSectionGuard g( &m_Sec );
    LOCK(m_Sec);
    mName.Clear();
    mChar.Clear();
    UINT nItem = mChar.AddObj((char*) "");
    kVar = mHeader.kVar;
    if ( nItem == -1 )
    {
      mHeader.kVar = 0;
      return 0;
    }
    //
    int nb,N = 0;
    const char* pszClass;
    const char* pszEntry;
    CharMP point;
    for ( UINT n = 0; n < kVar; n++ )
    {
      STrendId nID = m_pHeadT->nId[n];
      if( (nID.nId & 0x80000000) == 0x80000000  )
      {
        nID.nId -= 0x80000000;
        WORD offset = LOWORD(nID.nId);
        WORD type = HIWORD(nID.nId);
        HNAME hName = mName.Find( nID.name, &nb );
        if ( hName == INVALID_HNAME )
          mName.Add( nID.name, nb );
        else
        {
          strcpy_s( point, nID.name );
          while ( 1 )
          {
            strcat( point, ".X" );
            hName = mName.Find( point, &nb );
            if ( hName == INVALID_HNAME )
            {
              mName.Add( point, nb );
              break;
            }
            if ( lstrlen( point ) > 200 )
              sprintf_s( point, SizeMP, "Жопа.%d", N++ );
          }
        }
      }
      else if( 0!=nID.nId )
      {
        GetNameEx( nID.nId, &pszClass, &pszEntry );
        if( *pszEntry )
          sprintf_s( point, SizeMP, "%s.%s", pszClass, pszEntry );
        else
          strcpy_s( point, pszClass );
        HNAME hName = mName.Find( point, &nb );
        if ( hName == INVALID_HNAME )
          mName.Add( point, nb );
        else
        {
          while ( 1 )
          {
            strcat( point, ".X" );
            hName = mName.Find( point, &nb );
            if ( hName == INVALID_HNAME )
            {
              mName.Add( point, nb );
              break;
            }
            if ( lstrlen( point ) > 200 )
              sprintf_s( point, SizeMP, "Жопа.%d", N++ );
          }
        }
      }
    }
    UINT NC = mName.Count();
    ASS( mName.Count() == kVar );
    std::ofstream hFile;
    hFile.open(szBin, std::ios::trunc | std::ios::binary);
    if ( hFile.is_open() )
    {
      hFile.write((char*)&kVar, sizeof(kVar));
      mName.Write( hFile );
      mChar.Write( hFile );
    }
    hFile.close();
#ifdef _WIN32
    QueryPerformanceCounter( &finish );
    LONGLONG LI = (finish.QuadPart - start.QuadPart); 
    char resCounter[128];
    sprintf_s( resCounter, sizeof(resCounter), "= %I64d\n", LI );
    OutputDebugString( resCounter );
#endif
  }
  return -1;
}
//
//
CTrendHeaderEx* CTrendFile::GetHeader()
  {
  if ( !OpenTrendHead() )
    return NULL;
  //
  if ( mHeader.kVar == 0 )
    return NULL;
  //
  return m_pHeadT;
  }
//
STrendId* CTrendFile::GetHeader(CTrendHeader* header)
{
  if ( !OpenTrendHead() )
    return NULL;
  //
  if ( mHeader.kVar == 0 )
    return NULL;
  //
  memcpy(header,m_pHeadT,sizeof(CTrendHeader));
  //
  return m_pHeadT->nId;
}
//
void CTrendFile::BuildNames(SToTrend* to)
  {
  for ( int m = 0; m < to->nCount; m++ )
    {
    if  ( *to->szName[m] )
      {
      HNAME hFind = mName.Find( to->szName[m] );
      if ( hFind != INVALID_HNAME )
          to->nIndex[m] = (intptr_t)hFind;
      else
        to->nIndex[m] = -1;
      }
    else
      to->nIndex[m] = -1;
    }
  }
//
/*
SIZE_T debug_as = 0;
void* debug_begin = nullptr;
void* debug_end = nullptr;
*/
float* CTrendFile::MapTrendFile(uint64_t Addr, uint64_t& dwBegin, uint64_t& dwEnd)
{
/*
    debug_as = 0;
    debug_begin = nullptr;
    debug_end = nullptr;
*/
  DWORD dwBlock = mHeader.kVar * sizeof(float);
  uint64_t kPage = Addr / mio::page_size();
  dwBegin = kPage * mio::page_size();
  //kot не рисовались тренды на АВТ-4
  DWORD dwBlkAlloc=2048;
  float* pbBegWnd=NULL;
  size_t AllocSize=0;
  char szFile[_MAX_PATH];
  sprintf_s(szFile, _MAX_PATH, "%s_Trends.dat", rsuGetTrendPath().c_str());
  m_hDataMap.unmap();
  std::error_code error;
  while ( pbBegWnd == NULL )
  {
		dwBlkAlloc /= 2;
		if ( dwBlkAlloc == 0 ) 
			break;// Это уже большая жопа
		AllocSize = dwBlock * dwBlkAlloc;
		size_t AllocPages = AllocSize / mio::page_size() + 2;
		AllocSize = AllocPages * mio::page_size();
        
        m_hDataMap.map(szFile, dwBegin, AllocSize, error);
        if (error.value() == 0)
        {
            pbBegWnd = (float*)m_hDataMap.data();
            /*
            debug_as = AllocSize;
            debug_begin = pbBegWnd;
            debug_end = pbBegWnd + AllocSize;
            */
        }
        KKK();
  }
  if ( pbBegWnd == NULL )
    {
		DWORD Err = error.value();
		return NULL;
    }
  dwEnd = dwBegin + AllocSize;
  // Учитываем блочность
  dwEnd -= ( ((dwBlock/ mio::page_size())+2) * mio::page_size());
  return pbBegWnd;
}
//
int CTrendFile::FromTrend(SToTrend* to, char* from)
{
  if ( !OpenTrendData() )
    return 1;
  //
  if ( to->nStartTime != mHeader.StartTime )
  {
    BuildNames(to);//Надо найти соответствие ID и имени
  }
  //
  SFromTrend* trend = (SFromTrend*)from;
  trend->nTimeS = mHeader.StartTime;
  //
  // Начальная инициализация
  for ( int m = 0; m < to->nCount; m++ )
    {
    trend->nCount[m] = 0;
    trend->nIndex[m] = to->nIndex[m];
    }
  //
  // Условия заполнения данных
  int nLastTime = int(mHeader.StartTime+(mHeader.kBlk-1)*mHeader.StepTc);
  bool bFill = true;
  if ( mHeader.StartTime >= to->nLastTime  ) 
      bFill = false;
  if ( time_t(nLastTime) <= to->nBeginTime ) 
      bFill = false;
  if ( mHeader.kBlk < 2 ) 
      bFill = false;
  if ( !bFill )
    return 3;
  if ( nLastTime > to->nLastTime )
    nLastTime = int(to->nLastTime);
  //
  // Разрешение графика (пиксель/время)
  double dx = double(to->nPoint)/(to->nLastTime-to->nBeginTime);
  double dStep = dx*mHeader.StepTc;
  //
  // Ищем минимальный номер, который попадает в растр графика
  int nBH = int(to->nBeginTime-mHeader.StartTime);
  int nLH = int(    nLastTime -mHeader.StartTime);
  int nC = (long)(nBH)/(int)(mHeader.StepTc);
  int nL = (long)(nLH)/(int)(mHeader.StepTc);
  double X = 0;
  if ( nC < 0 )
    {
    X = -dStep*nC;
    nC = 0;
    }
  //
  uint64_t dwBegin=0,dwEnd=0;
  const DWORD dwBlock  = mHeader.kVar * sizeof(float);
  uint64_t dwAddr   = dwBlock * (uint64_t)nC;
  float* flTrend = MapTrendFile(dwAddr,dwBegin,dwEnd);

  if ( flTrend == NULL )
    return 4;
  //
  _Sxy* data[MAX_TREND];
  int   xOld[MAX_TREND];
  float y,yOld[MAX_TREND],iOld[MAX_TREND],aOld[MAX_TREND];
  //
  #undef  FIRST
  #define FIRST 3
  float* flData = flTrend + ( dwAddr - dwBegin )/sizeof(float);
  // Заполняем начальные точки (MoveTo)
  int x = (int)(X);
  for ( int m = 0; m < to->nCount; m++ )
  {
    if ( to->nIndex[m] >= 0 )
      {
      data[m] = trend->data+(to->nPoint+1)*2*m;
      if ( dwBegin == 0 && nC < FIRST )
        y = flData[to->nIndex[m]+mHeader.kVar*FIRST];
      else
        y = flData[to->nIndex[m]];
      yOld[m] = y;
      iOld[m] = y;
      aOld[m] = y;
      trend->nIndex[m] = to->nIndex[m];
      data[m][trend->nCount[m]].x = x;
      data[m][trend->nCount[m]].y = y;
      trend->nCount[m]++;
      }
  }
  //
  while( x < to->nPoint && nC < nL )
  {
    int prevX = x;
    int N = 0;
    do
    {
      nC++;
      dwAddr += dwBlock;
      X += dStep;
      x = (int)(X);
      ++N;
    }while( prevX==x && x < to->nPoint && nC < nL );

    if( x >= to->nPoint || nC >= nL )
      break;

    if ( x > to->nPoint ) //Дошли до конца рамки
      x = to->nPoint;
    if ( dwBegin > dwAddr || dwAddr >= dwEnd )
      {
        m_hDataMap.unmap();
        flTrend = MapTrendFile(dwAddr,dwBegin,dwEnd);
      }
    if ( flTrend == NULL )
      return 4;
    //
    flData = flTrend + ( dwAddr - dwBegin )/sizeof(float);
    for ( int m = 0; m < to->nCount; m++ )
    {
      if ( to->nIndex[m] >= 0 )
      {
      if ( dwBegin == 0 && nC < FIRST )
        y = flData[to->nIndex[m]+mHeader.kVar*FIRST];
      else
        y = flData[to->nIndex[m]];
      if ( x != xOld[m] )
        {
        int M = trend->nCount[m];
        if ( iOld[m] != yOld[m] && aOld[m] != yOld[m] )
          {
          data[m][--M].x = xOld[m];
          data[m][M++].y = iOld[m];
          data[m][M  ].x = xOld[m];
          data[m][M++].y = aOld[m];
          }
        else
          {
          if ( iOld[m] != yOld[m] )
            {
            data[m][M  ].x = xOld[m];
            data[m][M++].y = iOld[m];
            }
          else
          if ( aOld[m] != yOld[m] )
            {
            data[m][M  ].x = xOld[m];
            data[m][M++].y = aOld[m];
            }
          }
        //
        xOld[m] = x;
        yOld[m] = y;
        iOld[m] = y;
        aOld[m] = y;
        if ( M > 1 && data[m][M-1].y == y && data[m][M-2].y == y )
        M--;
        data[m][M  ].x = x;
        data[m][M++].y = y;
        trend->nCount[m] = M;
        }
      else
        {
        iOld[m] = std::min<float>(y,iOld[m]);
        aOld[m] = std::max(y,aOld[m]);
        }
      }
    }
  }//while
  //
  m_hDataMap.unmap();
  trend->askKind = 5;
  return 0;
  }
//