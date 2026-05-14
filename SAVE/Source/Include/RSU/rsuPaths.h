#pragma once
#include <CommonRSUs.h>

COMMONRSUS_API const std::string& rsuGetProjectPath();//корневая папка выбранного проекта
COMMONRSUS_API const std::string& rsuGetExecutionPath();//system/exe или system/exe_64
COMMONRSUS_API const std::string& rsuGetMemoryPath();
COMMONRSUS_API const std::string& rsuGetPath2Rsu();//корневая папка RSU проекта м.б. пустым
COMMONRSUS_API void rsuSetPath2Rsu(const std::string& pszPath );
COMMONRSUS_API const std::string& rsuGetTrendPath();// Возвращает путь к директории с файлами трендов
COMMONRSUS_API const std::string& rsuGetAcyPath();// Возвращает полный путь файла с описанием тэгов
COMMONRSUS_API void rsuTakeOffReadOnly(const std::string& szFileName);
COMMONRSUS_API void rsuSetProjectPath(const std::string& a_PrjRootDir);
