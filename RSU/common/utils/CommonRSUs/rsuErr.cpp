#include <rsuErr.h>
#undef fopen

int iKKK = 0;
void KKK()
{
  iKKK++;
}

#if defined(_WIN64)
BYTE* g_pFunc = NULL;
void InitBreak()
{
	HANDLE hHeap = HeapCreate(HEAP_CREATE_ENABLE_EXECUTE, 0x01000, 0);
	g_pFunc = (BYTE*)HeapAlloc(hHeap, 0, 1024);
	BYTE nCode[] = {0x48, 0x8B, 0x14, 0x24, 0x48, 0x83, 0xEC, 0x38, 0x4C, 0x8D, 0x44, 0x24, 0x40, 0x41, 0xB9, 0x01, 0x00, 0x00, 0x00, 0x48, 0x83, 0xC9, 0xFF, 0xC6, 0x44, 0x24, 0x40, 0xF1, 0x48, 0xC7, 0x44, 0x24, 0x20, 0x00, 0x00, 0x00, 0x00, 0x48, 0xB8};
	UINT nCodePos = 0;
	memcpy(g_pFunc, nCode, sizeof(nCode));
	nCodePos += sizeof(nCode);	
	__int64 nAddrWriteProcessMemory = (__int64)WriteProcessMemory;
	memcpy(&g_pFunc[nCodePos], &nAddrWriteProcessMemory, sizeof(nAddrWriteProcessMemory));	
	nCodePos +=sizeof(nAddrWriteProcessMemory);
	g_pFunc[nCodePos++] = 0xFF;
	g_pFunc[nCodePos++] = 0xD0;
	g_pFunc[nCodePos++] = 0x48;
	g_pFunc[nCodePos++] = 0x83;
	g_pFunc[nCodePos++] = 0xC4;
	g_pFunc[nCodePos++] = 0x38;
	g_pFunc[nCodePos++] = 0xC3;
}
#else 
void InitBreak(){}
#endif

#if defined(_WIN64)
static int g_bBreak = 0;
typedef void (WINAPI *FuncBreak)();
#define BREAK {((FuncBreak)g_pFunc)();\
  g_bBreak++;}
#else
#define BREAK __asm {__emit 0xF1};
#endif // _WIN64

typedef void (*pAbortProc)();
struct CErrInfo
{
  CErrInfo( char * pInfo, ... );
  ~CErrInfo();
  static pAbortProc pAbort;
};

pAbortProc CErrInfo::pAbort = NULL;

#define AbortApp() if ( CErrInfo::pAbort )(*CErrInfo::pAbort)(); else{ /*exit(1);*/}

int NumbAssert = 0;
void rsuSysAssert( LPCSTR File, int Line )
{
  NumbAssert++;
  if ( NumbAssert > 10 )
    AbortApp();
  //exit( 1 );          
#ifdef _WIN32          
  if ( IsDebuggerPresent())
  {
    BREAK
  }
#endif
}
