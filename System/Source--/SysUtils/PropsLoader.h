#ifndef PROPSLOADER_H
#define PROPSLOADER_H

#include <string>
#include <vector>
#include <map>
#include "crossplatform.h"

#define MAX_PATH_LENGTH			512
#define MAX_PRGNAME_LENGTH		64

typedef std::map<std::string, std::string> propsMap;
typedef propsMap::iterator propsMap_it_type;
typedef std::pair <std::string, std::string> char_pair;

class CPropsLoader
{
  public:
    CPropsLoader ();
    ~CPropsLoader ();

    bool Load ();
    char *GetErrorMsg() { return m_errorMsg; }

    bool GetBool (const char *szKeyName, bool& bValue);
    bool GetInt (const char *szKeyName, int& nValue);
    bool GetDouble (const char *szKeyName, double& dValue);
    bool GetString (const char *szKeyName, char *szValue, int nLen = _MAX_PATH);

  protected:
    propsMap m_map;
    char m_errorMsg[MAX_PATH_LENGTH];

    bool DoLoad (bool bSystem);
    void GetTokenValue (char *szToken, char *szValue, bool bSecond, int nLen = _MAX_PATH);
    void LogInfoDirs ();
};

#endif // PROPSLOADER_H
