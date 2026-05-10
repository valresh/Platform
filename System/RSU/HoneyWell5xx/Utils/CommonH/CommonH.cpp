#include <CommonH.h>
#include <MemoryServiceH5xx.h>
#include <rsuPaths.h>
#include <stdlib.h>
#include <stdio.h>

static LPCSTR s_szNameCBH = "Весь ControlBuilderHoneywell";
static LPCSTR s_szNameOneMnemo = "МнемосхемаCBH";
static LPCSTR s_szNameOneAlmGr = "АлармГруппаCBH";

LPCSTR GetNameControlBuilderHoneywell()
{
  return s_szNameCBH;
}

LPCSTR GetOneMnemoNameControlBuilderHoneywell()
{ 
  return s_szNameOneMnemo;
}

CBase* CreateControlBuilderHSystemData()
{
  return KMemoryServiceH5xx::Instance().CreateObjectsStruct( s_szNameCBH, CCBHSystem::TypeID, -1 );
}

CCBHSystem* GetControlBuilderHSystemData()
{
  CCBHSystem* pSystem = (CCBHSystem*)KMemoryServiceH5xx::Instance().FindObjectsStruct( s_szNameCBH, CCBHSystem::TypeID, -1 );
  if ( pSystem && pSystem->size == sizeof(CCBHSystem) )
    return pSystem;
  return NULL;
}

LPCSTR GetAlarmFileName(bool bDubl)
{
  static CHAR szPath0[_MAX_PATH] = {0};
  static CHAR szPath1[_MAX_PATH] = {0};
  if ( *szPath0 == CHAR('\0'))
  {
    sprintf( szPath0, "%sAlarmCBH-0.dat", rsuGetMemoryPath().c_str() );
    sprintf( szPath1, "%sAlarmCBH-1.dat", rsuGetMemoryPath().c_str());
  }
  return (bDubl) ? szPath1 : szPath0;
}

LPCSTR GetOneAlmGrNameControlBuilderHoneywell()
{ 
  return s_szNameOneAlmGr;
}