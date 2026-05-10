#ifdef _WIN32
#include <crossplatform.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <stdarg.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <crosslock.h>
#include "CRsuLogFile.h"
#include <share.h>
#include <crosslock.h>

#define MSGBUFSIZE		4096
#define TEST_FILE_NAME	"Logging.txt"

BOOL CRsuLogFile::c_bLogging = FALSE;
char CRsuLogFile::c_szLogPath[_MAX_PATH+1] = { 0 };
//CRITICAL_SECTION CRsuLogFile::c_cCritSect;
std::recursive_mutex CRsuLogFile::c_cCritSect;

CRsuLogFile::CRsuLogFile ()
{
}

CRsuLogFile::~CRsuLogFile ()
{
}

void CRsuLogFile::Initialize (const char *szFileName, BOOL bDeleteOld)
{
	//InitializeCriticalSection (&c_cCritSect);
	::GetModuleFileName (NULL, c_szLogPath, _MAX_PATH);
	if ( szFileName )
	{
		char *pPtr = strrchr (c_szLogPath, (int)_T('\\'));
		if ( pPtr )
		{
			char szTestFile[_MAX_PATH + 1] = { 0 };
			pPtr++;
			*pPtr = '\0';
			strcpy_s (szTestFile, _MAX_PATH, c_szLogPath);
			strcat_s (szTestFile, _MAX_PATH, TEST_FILE_NAME);
			if ( !_access(szTestFile, 0) )
			{
				strcat_s (c_szLogPath, _MAX_PATH, szFileName);
				strcat_s (c_szLogPath, _MAX_PATH, ".log");
				c_bLogging = TRUE;
			}
		}
	}
	else
	{
		char *pPtr = strchr (c_szLogPath, '.');
		if ( !pPtr )
			return;
		pPtr++;
		int nLen = _MAX_PATH - (int)(pPtr - c_szLogPath);
		strcpy_s (pPtr, (size_t)nLen, "log");
	}
	if ( c_bLogging && !_access(c_szLogPath, 0) && bDeleteOld )
		::DeleteFile (c_szLogPath);
}

void CRsuLogFile::Log (const char *szFormat, ...)
{
	if ( !c_bLogging )
		return;
	//EnterCriticalSection (&c_cCritSect);
	LOCK(c_cCritSect);
	SYSTEMTIME st;
	GetLocalTime (&st);
	char szBuffer[MSGBUFSIZE] = { 0 };
	DWORD dwThreadID = ::GetCurrentThreadId();
	int nTimeLen = sprintf_s (szBuffer, MSGBUFSIZE, "%02d:%02d:%02d:%03d: (%d) ", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, dwThreadID);
	va_list argList;
	va_start (argList, szFormat);
	int nLen = _vstprintf_s (&szBuffer[nTimeLen], MSGBUFSIZE - nTimeLen, szFormat, argList);
/*
	if ( nLen > 0 )
		szBuffer[nLen] = '\n';
*/
	strcat_s (szBuffer, MSGBUFSIZE, "\n");
	szBuffer[MSGBUFSIZE-1] = '\0';
	FILE* pOutFile = _tfsopen (c_szLogPath, _T("a"), _SH_DENYNO);
	if (pOutFile != NULL)
	{
		fputs(szBuffer, pOutFile);            
		fflush(pOutFile);
		fclose(pOutFile);
	}
	//LeaveCriticalSection (&c_cCritSect);
}
#endif
