#pragma once
#include <CommProc.h>
//#include <crosspath.h>
#include <stdio.h>
#include <fstream>

#include <QtCore/QtGlobal>

#ifdef COMMONRSUS_EXPORTS
#define COMMONRSUS_API Q_DECL_EXPORT
#else
#define COMMONRSUS_API Q_DECL_IMPORT
#endif

// Возвращает полный путь к файлу сообщений
COMMONRSUS_API const char* GetModel2OperatorMessagesFilePath();
COMMONRSUS_API int RsuPackager( LPCSTR pszSrcFile, LPCSTR pszArchive, LPCSTR pszNamePackAs );
COMMONRSUS_API bool RsuExtractor(std::fstream& f, LPCSTR pszArchive);
COMMONRSUS_API FILE* RsuExtractor( LPCSTR pszArchive );
COMMONRSUS_API bool is_valid_utf8( LPCSTR string );
COMMONRSUS_API int convert_utf8_to_windows1251( LPCSTR utf8, LPSTR windows1251, size_t n );
