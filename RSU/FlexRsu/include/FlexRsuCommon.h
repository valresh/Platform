#pragma once

#ifdef FLEXRSUCOMMON_EXPORTS
#define FLEXRSUCOMMON_API _EXP
#else
#define FLEXRSUCOMMON_API _IMP
#endif

namespace ns_FlexRsu
{
  FLEXRSUCOMMON_API void GetSharedName( LPSTR pszName, size_t s );
  template<size_t _s> void GetSharedName( char (&szName)[_s])
  {
    return GetSharedName( szName, _s );
  }

  FLEXRSUCOMMON_API void GetHeaderMessagesName( LPSTR pszName, size_t s );
  template<size_t _s>void GetHeaderMessagesName( char (&szName)[_s])
  {
    return GetHeaderMessagesName( szName, _s );
  }

  FLEXRSUCOMMON_API void GetMessagesDataName( LPSTR pszName, size_t s, DWORD index );
  template<size_t _s>void GetMessagesDataName( char (&szName)[_s], DWORD index)
  {
    return GetMessagesDataName( szName, _s, index );
  }

  // Возвращает полный путь к файлу сообщений
  FLEXRSUCOMMON_API LPCSTR GetMsgToOperatorFilePath();

  //////////////////////////////////////////////////////////////////////////
  FLEXRSUCOMMON_API void GetPath2Logic( LPSTR pszName, size_t s, LPCSTR pszPrjName );
  template<size_t _s>void GetPath2Logic( char (&szName)[_s], LPCSTR pszPrjName )
  {
    return GetPath2Logic( szName, _s, pszPrjName );
  }

  FLEXRSUCOMMON_API void GetPath2Audit( LPSTR pszName, size_t s, LPCSTR pszPrjName );
  template<size_t _s>void GetPath2Audit( char (&szName)[_s], LPCSTR pszPrjName )
  {
    return GetPath2Audit( szName, _s, pszPrjName );
  }

  FLEXRSUCOMMON_API void GetPathTRSU( LPSTR pszName, size_t s );
  template<size_t _s>void GetPathTRSU( char (&szName)[_s] )
  {
    return GetPathTRSU( szName, _s );
  }


  FLEXRSUCOMMON_API LPCSTR GetLogicFileExtension();

  // Возвращает полный путь к аларм лог-файлу
  FLEXRSUCOMMON_API LPCSTR GetAlarmFileName(bool bDubl);
}
