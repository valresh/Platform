#include "TagSkipFilter.h"
#include <macros/AutoCloser.h>
#include <FlexRsuCommon.h>
#include <algorithm>
#include <clocale>

using namespace std;

KTagSkipFilter::KTagSkipFilter()
: m_bCfgRead( false )
{
}

KTagSkipFilter& KTagSkipFilter::Instance()
{
  static KTagSkipFilter _Impl;
  return _Impl;
}

bool KTagSkipFilter::SkipWriteEvent( SSendToModel &send )
{
  GetList();

  char szBuf[_countof(send.szName)] = {};
  strcpy_s( szBuf, send.szName );
  /*LPSTR p = strchr( szBuf, '.');
  if( p )
    *p = 0;*/

  tTagsSkip_::iterator itf = std::find( m_tagsSkipCont.begin(), m_tagsSkipCont.end(), szBuf );
  if( m_tagsSkipCont.end() != itf )
    return true;

  return false;
}

void KTagSkipFilter::GetList()
{
    if (m_bCfgRead)
        return;
    m_bCfgRead = true;

    char szPath[_MAX_PATH] = { 0 };
    ns_FlexRsu::GetPathTRSU(szPath);

#ifdef _WIN32
    strcat_s(szPath, "\\tsf.cfg");
#else
    strcat(szPath, "/tsf.cfg");
#endif

#ifndef _WIN32
    setlocale(LC_ALL, "ru_RU.UTF-8");
#endif

    FILE* pFile = nullptr;
#ifdef _WIN32
    if (fopen_s(&pFile, szPath, "rt") != 0 || !pFile)
        return;
#else
    pFile = fopen(szPath, "rt");
    if (!pFile)
        return;
#endif

    KAutoCloser<FILE*> file(pFile, fclose);

    char szLine[_MAX_PATH] = { 0 };
    while (fgets(szLine, sizeof(szLine), pFile))
    {
        char* pszLine = szLine;
        while (*pszLine)
        {
            if (*pszLine <= 13)
            {
                *pszLine = 0;
                break;
            }
            ++pszLine;
        }
        m_tagsSkipCont.push_back(szLine);
    }
}
