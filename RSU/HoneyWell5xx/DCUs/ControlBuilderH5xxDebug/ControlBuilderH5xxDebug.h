#pragma once
#include <H_Data.h>
#include <IControlBuilder.h>
#include <crossplatform.h>

#ifdef CONTROLBUILDERH5XXDEBUG_EXPORTS
#define CONTROLBUILDERH5XXDEBUG_API _EXP
#else
#define CONTROLBUILDERH5XXDEBUG_API _IMP
#endif


typedef void (*tSetProgress)( LPCSTR Title, int Proc );
extern CONTROLBUILDERH5XXDEBUG_API tSetProgress pSetProgress;

typedef void (*tCloseProgress)();
extern CONTROLBUILDERH5XXDEBUG_API tCloseProgress pCloseProgress;

CONTROLBUILDERH5XXDEBUG_API bool ShowDebugWnd( SH_Module *pRoot, IControlBuilder *pCB, char * pszName, LPCSTR pszObjName, void * pData, bool bEnternalCall );
