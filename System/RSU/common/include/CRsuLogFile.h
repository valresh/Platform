#pragma once

#include <mutex>

#ifndef _MAX_PATH
#define _MAX_PATH   260
#endif // _MAX_PATH

#define LOGLEVEL_LOW	0
#define LOGLEVEL_MEDIUM	1
#define LOGLEVEL_HIGH	2
#define LOGLEVEL_URGENT	3

class CRsuLogFile
{
public:
	CRsuLogFile();
	~CRsuLogFile ();

	static void Initialize (const char *szFileName = NULL, BOOL bDeleteOld = TRUE);
	static void Log (const char *szFormat, ...);
	static void LogEx (int nMsgLevel, const char *szFormat, ...);
	static void GetLogPath (char *szLogPath, int nLength);
	static BOOL IsLogging () { return c_bLogging; }
	static BOOL IsTesting ();
	static BOOL IsLocalSyncEnabled ();

protected:
	static BOOL c_bLogging;
	static char c_szLogPath[_MAX_PATH+1];
	static std::recursive_mutex c_cCritSect;
	static int c_nLogLevel;

	static void DoLog (const char *szFormat, va_list argList);
	static void GetLogLevel ();
};
