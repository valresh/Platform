#pragma once
#include <ServerExPluginTypes.h>

struct SExtensions
{
  tExtractumRSU pExtractumRSUCall;
  tSizeShRSU  pSizeShRSUCall;
  tPutModelRSU pPutModelRSUCall;
  tGetRsuData pGetRsuData;
  tAlarmsSupport pAlarmsSupport;
  tAnyGetSetRequest pAnyOtherRequest;
  tAlarmsSupport pMessagesSupport;
  tAlarmsSupport pEventsSupport;
  tFindConnPointEx pFindConnPointEx;
  tFindHoney pFindHoney;
  char szLibraryName[64];
};

enum
{
  ecCount = 15
};
extern SExtensions g_extensionsCont[ecCount];
