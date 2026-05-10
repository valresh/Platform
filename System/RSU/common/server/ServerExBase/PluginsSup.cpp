#include <iostream>
#include <crossplatform.h>
#include <macros/AutoCloser.h>
#include "ServerThreadBase.h"
#include <dylib.hpp>
#include <crossstring.h>
#include "Names.h"
#include "ServerExBase.h"
#include "LogFile.h"

SExtensions g_extensionsCont[ecCount] = { NULL };
#ifdef _WIN32
const char* LibraryExtension = ".dll";
#else
const char* LibraryExtension = ".so";
#endif

static std::vector<std::string> s_szMasks = { "ServerExPlugin", "ServerExSharedPlugin" }; //"FlexRsu_ServerExSharedPlugin" };

dylib::native_symbol_type try_get_symbol(const dylib* lib, const char* symbol_name)
{
    try {
        return lib->get_symbol(symbol_name);
    }
    catch (...)
    {
        return nullptr;
    }
}

SERVEREXBASE_API void LoadPlugins()
{
  ASS(InitNamesAndVariables(false, false));
  GiveCommData("Модель", CExternalCondition::TypeID, (void**)&pExt);
  GiveCommData("Модель", CSystem::TypeID, (void**)&pSys);
  LPCSTR pExe = GetExePath();
  int c = 0;

  for (const auto& entry : std::filesystem::directory_iterator(pExe))
  {
      if (!entry.is_regular_file() || entry.path().extension () != LibraryExtension)
      {
          continue;
      }

      std::string filename(entry.path().filename().string());
      bool found = std::any_of(s_szMasks.begin(), s_szMasks.end(),
          [&filename](const auto& s) { return filename.find(s) != std::string::npos; });
      if (!found)
          continue;
      dylib* hLib = NULL;
      try
      {
          hLib = new dylib(entry.path().parent_path(), filename, dylib::no_filename_decorations);
      }
      catch (dylib::load_error& e)
      {
          CLogFile::Stream (LOGLEVEL_URGENT) << "Error: Can't load " << entry.path().filename() << ". Error:" << e.what();
          continue;
      }

      if (!hLib)
      {
          continue;
      }

      CLogFile::Stream(LOGLEVEL_HIGH) << "Plugin: " << entry.path().filename() << " loaded\n";
      g_extensionsCont[c].pExtractumRSUCall = (tExtractumRSU)try_get_symbol(hLib, "ExtractumRSU");
      g_extensionsCont[c].pSizeShRSUCall = (tSizeShRSU)try_get_symbol(hLib, "SizeShRSU");
      g_extensionsCont[c].pPutModelRSUCall = (tPutModelRSU)try_get_symbol(hLib, "PutModelRSU");
      g_extensionsCont[c].pGetRsuData = (tGetRsuData)try_get_symbol(hLib, "GetRsuData");
      g_extensionsCont[c].pAlarmsSupport = (tAlarmsSupport)try_get_symbol(hLib, "AlarmsSupport");
      g_extensionsCont[c].pMessagesSupport = (tAlarmsSupport)try_get_symbol(hLib, "MessagesSupport");
      g_extensionsCont[c].pAnyOtherRequest = (tAnyGetSetRequest)try_get_symbol(hLib, "AnyOtherRequest");
      g_extensionsCont[c].pEventsSupport = (tAlarmsSupport)try_get_symbol(hLib, "EventsSupport");
      g_extensionsCont[c].pFindConnPointEx = (tFindConnPointEx)try_get_symbol(hLib, "FindConnPointEx");
      g_extensionsCont[c].pFindHoney = (tFindHoney)try_get_symbol(hLib, "FindHoney");
      strcpy_s(g_extensionsCont[c].szLibraryName, filename.c_str());
      c++;
      if (c == _countof(g_extensionsCont))
          break;

  }
}
