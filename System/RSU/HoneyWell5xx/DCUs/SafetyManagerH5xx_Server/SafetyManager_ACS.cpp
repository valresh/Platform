#include "SafetyManager.h"
#ifdef _WIN32
#include "../SafetyManagerH5xx_Debug/SafetyManagerDebug_dll.h"
#endif

#ifdef _WIN32
void KSafetyManager::ShowACSObject( HWND hMainWnd, char * Name, void * pData )
{
  ShowDebugWindow( Name, this );
}
#endif
