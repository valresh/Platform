#include "PropsLoader.h"
#include "CommProc.h"

//#include "LogFile.h"


#define PROJECTS_DIR	"projects"

CPropsLoader::CPropsLoader ()
{
  m_errorMsg[0] = '\0';
}

CPropsLoader::~CPropsLoader ()
{
}

void CPropsLoader::GetTokenValue (char *szToken, char *szValue, bool bSecond, int nLen)
{
  szValue[0] = '\0';
  char *ptr = strchr (szToken, ';');
  if ( ptr )
  {
    int nPos = (ptr - szToken);
    if ( bSecond )
      strncpy (szValue, ptr + 1, nLen);
    else
    {
      strncpy (szValue, szToken, nLen);
      szValue[nPos] = '\0';
    }
  }
}
/*
g_bServerTCP
g_nServerPort
*/

bool CPropsLoader::DoLoad (bool bSystem)
{
  // Открываем системный (bSystem == true) или локальный (bSystem == false) файл настроек
  std::filesystem::path szPath(bSystem ? ROOT_PATH : PROJECT_ROOT);
  szPath = szPath / "PROP" / "Properties.csv";
  char szSep[] = "#";
  char szLine[_MAX_PATH];
  FILE *pFile = fopen (szPath.string().c_str(), "rt");
  if ( !pFile )
  {
    snprintf(m_errorMsg, MAX_PATH_LENGTH, "Не найден файл настроек:\n%s", szPath.string().c_str());
    return false;
  }
  while ( fgets ( szLine, _MAX_PATH-1, pFile) )
  {
    char *next_token = NULL;
    std::string key = "";
    int nCol = 0;
    char *szToken = strtok_r (szLine, szSep, &next_token);
    char szValue[_MAX_PATH] = "\0";
    while ( szToken != NULL )
    {
      bool bSecond = (nCol == 0);
      GetTokenValue (szToken, szValue, bSecond);
      if ( nCol == 0 && (lstrcmp (szValue, "MH") == 0 || lstrcmp (szValue, "GR") == 0 || lstrcmp (szValue, "HD") == 0) )
        break;
      if ( nCol == 1 )
      {
        key = szValue;
        for (propsMap_it_type iterator = m_map.begin(); iterator != m_map.end(); iterator++)
        {
          if (lstrcmp(szValue, iterator->first.c_str()) == 0)
          {
            key = iterator->first;
            break;
          }
        }
      }
      else if (nCol == 2)
      {
        propsMap_it_type iterator = (lstrlen(key.c_str()) > 0) ? m_map.find(key) : m_map.end();
        if (iterator != m_map.end())
          iterator->second = szValue;
        else
          m_map.insert(char_pair(key.c_str(), szValue));
      }
      szToken = strtok_r(NULL, szSep, &next_token);
      nCol ++;
    }
  }
  fclose (pFile);
  return true;
}

bool CPropsLoader::Load ()
{
  bool bResult = DoLoad (false);
  return bResult; //##//
  propsMap_it_type iterator = m_map.find ("Корневая папка");
  if (!bResult || iterator == m_map.end())
  {
    snprintf(m_errorMsg, MAX_PATH_LENGTH, "Не найдено значение ключа 'Корневая папка'");
    return false;
  }
  Info.Name[0] = 0;

  char szRootName[MAX_PRGNAME_LENGTH];
  lstrcpy(szRootName, iterator->second.c_str());
  auto projectRoot = std::filesystem::path(ROOT_PATH) / PROJECTS_DIR / szRootName / "";
  lstrcpy(PROJECT_ROOT, projectRoot.string().c_str());

  // CHECK !!!
  iterator->second = PROJECTS_DIR;
  iterator->second += std::filesystem::path::preferred_separator;
  iterator->second += szRootName;
  // ...
  lstrcpy (CURR_PROJECT, szRootName);
  iterator = m_map.find ("Папка РСУ");
  if ( iterator != m_map.end() )
  {
    lstrcpy (Info.dirProject, (std::filesystem::path(PROJECT_ROOT) / iterator->second / "").string().c_str());
    strcpy(Info.RSUFolder, iterator->second.c_str());
  }
  iterator = m_map.find ("Корневая папка");
  if ( iterator != m_map.end() )
    lstrcpy (Info.Name, iterator->second.c_str());
  bResult = DoLoad (false);
  if ( bResult )
  {
    iterator = m_map.find ("Корневая папка");
    if ( iterator != m_map.end() && lstrlen (iterator->second.c_str()) > 0 )
    {
      lstrcpy (Info.Name, iterator->second.c_str());
      strncpy(Info.TxtPath, (std::filesystem::path(PROJECT_ROOT) / "DATA" / "Txt" / "").string().c_str(), _MAX_PATH); // рабочий каталог
      snprintf (Info.BasePath, _MAX_PATH, "%.*s", _MAX_PATH - 1, PROJECT_ROOT);
      strncpy (Info.StatePath, (std::filesystem::path(PROJECT_ROOT) / "DATA" / "STATES" / "").string().c_str(), _MAX_PATH);	// каталог для сохранения состояний (в models.ini не описывается)
      strncpy (Info.dirScript, (std::filesystem::path(PROJECT_ROOT) / "DATA" / "Script" / "").string().c_str(), _MAX_DIR);	// каталог описаний мнемосхем
      strncpy (Info.dirScheme, (std::filesystem::path(PROJECT_ROOT) / "DATA" / "SCHEME" / "").string().c_str(), _MAX_DIR);	// каталог технологических рисунков
      snprintf (Info.namScheme, _MAX_FNAME, "%s", "" );								// имя файла общей технол. схемы (без расширения)
    }
    iterator = m_map.find ("Папка РСУ");
    if ( iterator != m_map.end() )
    {
      lstrcpy (Info.dirProject, (std::filesystem::path(PROJECT_ROOT) / iterator->second.c_str() / "").string().c_str());
      strcpy(Info.RSUFolder, iterator->second.c_str());
    }
  }

  return bResult;
}

bool CPropsLoader::GetBool (const char *szKeyName, bool& bValue)
{
  char szValue[_MAX_PATH] = "\0";
  if ( !GetString (szKeyName, szValue) )
    return false;
  int nValue = atoi (szValue);
  bValue = (nValue == 0) ? false : true;
  return true;
}

bool CPropsLoader::GetInt (const char *szKeyName, int& nValue)
{
  char szValue[_MAX_PATH] = "\0";
  if ( !GetString (szKeyName, szValue) )
    return false;
  nValue = atoi (szValue);
  return true;
}

bool CPropsLoader::GetDouble (const char *szKeyName, double& dValue)
{
  char szValue[_MAX_PATH] = "\0";
  if ( !GetString (szKeyName, szValue) )
    return false;
  char ok_decimal_point = localeconv()->decimal_point[0];
  char bad_decimal_point = (ok_decimal_point == '.') ? ',' : '.';
  for (int i = 0; szValue[i] != '\0'; ++i)
    if (szValue[i] == bad_decimal_point)
      szValue[i] = ok_decimal_point;
  dValue = atof(szValue);
  return true;
}

bool CPropsLoader::GetString (const char *szKeyName, char *szValue, int nLen)
{
  propsMap_it_type iterator = m_map.find (szKeyName);
  if ( iterator == m_map.end() )
  {
    szValue[0] = '\0';
    return false;
  }
  strcpy (szValue, iterator->second.c_str());
  //strcpy_s (szValue, nLen, iterator->second.c_str());
  return true;
}

void CPropsLoader::LogInfoDirs ()
{
  // CLogFile::Log ("CPropsLoader: PROJECT_ROOT %s", PROJECT_ROOT);
  // CLogFile::Log ("CPropsLoader: CURR_PROJECT %s", CURR_PROJECT);
  // CLogFile::Log ("CPropsLoader: Info.TxtPath %s", Info.TxtPath);
  // CLogFile::Log ("CPropsLoader: Info.BasePath %s", Info.BasePath);
  // CLogFile::Log ("CPropsLoader: Info.StatePath %s", Info.StatePath);
  // CLogFile::Log ("CPropsLoader: Info.dirScript %s", Info.dirScript);
  // CLogFile::Log ("CPropsLoader: Info.dirScheme %s", Info.dirScheme);
  // CLogFile::Log ("CPropsLoader: Info.dirProject %s", Info.dirProject);
}

