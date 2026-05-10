#include <crossplatform.h>
#include <FlexRsuCommon.h>
#include <rsus/rsuPaths.h>
#include <hashRSU.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace ns_FlexRsu
{
  std::string GetPrefix4Names()
  {
      std::string s_path = fs::path(rsuGetExecutionPath()).parent_path().generic_string();
      return std::to_string(CreateHASH32(s_path.c_str(), true));   
  }

  void GetSharedName( LPSTR pszName, size_t sz )
  {
      snprintf( pszName, sz, "%s_FlexRsu_Shared", GetPrefix4Names().c_str() );
  }

  void GetHeaderMessagesName( LPSTR pszName, size_t sz )
  {
    snprintf( pszName, sz, "%s Header Messages", GetPrefix4Names().c_str() );
  }

  void GetMessagesDataName( LPSTR pszName, size_t sz, DWORD index )
  {
    snprintf( pszName, sz, "%s Data Messages %d", GetPrefix4Names().c_str(), (int) index );
  }

  LPCSTR GetMsgToOperatorFilePath()
  {
    static std::string s = "";
    if( s == "")
        s = rsu_cp::makepath_with_ext_cp( rsuGetMemoryPath(), "Msg2UserFlex", ".dat").generic_string();      
    return s.c_str();
  }

  static LPCSTR s_szRsuDir = "TRSU";
  static LPCSTR s_szLogicDir = "Logiс";
  static LPCSTR s_szAuditDir = "Audit";

  void GetPath2Logic( LPSTR pszName, size_t s, LPCSTR pszPrjName )
  {
    fs::path p = rsu_cp::makepath_cp(rsuGetProjectPath(), s_szRsuDir, pszPrjName) / fs::path(s_szLogicDir);
    std::string n = p.generic_string();
    strncpy(pszName, n.c_str(), s);
  }

  void GetPathTRSU( LPSTR pszName, size_t s )
  {
      auto path = (std::filesystem::path(rsuGetProjectPath()) / s_szRsuDir).string();
      strncpy(pszName, path.c_str(), s);
  }

  void GetPath2Audit( LPSTR pszName, size_t s, LPCSTR pszPrjName )
  {
      fs::path p = rsu_cp::makepath_cp(rsuGetProjectPath(), s_szRsuDir, pszPrjName) / fs::path(s_szAuditDir);
      std::string n = p.generic_string();
      strncpy(pszName, n.c_str(), s);
  }

  LPCSTR GetLogicFileExtension()
  {
    static LPCSTR s_szFilesExt = "xml";
    return s_szFilesExt;
  }

  LPCSTR GetAlarmFileName(bool bDubl)
  {
    static CHAR szPath0[_MAX_PATH] = {0};
    static CHAR szPath1[_MAX_PATH] = {0};
    if ( *szPath0 == CHAR('\0'))
    {
        std::string m = rsuGetMemoryPath();
        std::string p = rsu_cp::makepath_with_ext_cp(m, "AlarmFlex-0.dat", "").generic_string();
        strncpy(szPath0, p.c_str(), sizeof(szPath0));
        p = rsu_cp::makepath_with_ext_cp(m, "AlarmFlex-1.dat", "").generic_string();
        strncpy(szPath1, p.c_str(), sizeof(szPath1));
    }
    return (bDubl) ? szPath1 : szPath0;
  }
}
