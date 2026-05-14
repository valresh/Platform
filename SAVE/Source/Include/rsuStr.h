#pragma once

//#include "CommonRSUs.h"
#include <string>

int _Kstricmp(const std::string& first, const std::string& last);
int _Kstrnicmp(const std::string& first, const std::string& last, size_t count);

void _Ktoupper(char(&buffer)[]);
std::string _Ktoupper(const std::string& src);
void _Ktoupper(std::string& src);

std::string _Ktolower(const std::string& src);
void _Ktolower(std::string& src);

void set_locale(const char* a_lname = "ru_RU.utf8");
