#pragma once

#include "CommonRSUs.h"
#include <string>

COMMONRSUS_API int _Kstricmp(const std::string& first, const std::string& last);
COMMONRSUS_API int _Kstrnicmp(const std::string& first, const std::string& last, size_t count);

COMMONRSUS_API void _Ktoupper(char(&buffer)[]);
COMMONRSUS_API std::string _Ktoupper(const std::string& src);
COMMONRSUS_API void _Ktoupper(std::string& src);

COMMONRSUS_API std::string _Ktolower(const std::string& src);
COMMONRSUS_API void _Ktolower(std::string& src);

COMMONRSUS_API void set_locale(const char* a_lname = "ru_RU.utf8");