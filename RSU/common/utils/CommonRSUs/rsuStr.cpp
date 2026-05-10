#include <rsuStr.h>
#include <locale>
#include <algorithm>
#include <cwctype>
#include <mio.hpp>
#include <CommProc.h>
//#include <cutf.h>
//#include <crossstring.h>

#ifdef __cplusplus
extern "C" {
#endif
size_t utf8_towchar(const char* s, size_t inSize, wchar_t* out, size_t outsize);
//std::string widetoutf8(const wchar_t* ws);

#ifdef __cplusplus
}
#endif

std::string widetoutf8(const std::wstring& ws);

typedef wint_t fn_case_transform (wint_t _C);

std::wstring s_2_ws(const std::string& s)
{
    std::wstring ret;
    if (!s.empty())
    {
        size_t wide_char_count = utf8_towchar(s.c_str(), s.length(), nullptr, 0);
        ret.resize(wide_char_count);
        utf8_towchar(s.c_str(), s.length(), (wchar_t*) ret.c_str(), ret.size());
    }
    return ret;
}

std::wstring case_transform(const std::string& src, fn_case_transform& f)
{
    std::wstring v_ws = s_2_ws(src);
    std::transform(v_ws.begin(), v_ws.end(), v_ws.begin(), f);
    return v_ws;
}

void _Ktoupper(char(&buffer)[])
{
    std::string str = _Ktoupper(std::string(buffer));
    strcpy(buffer, str.c_str());
}

std::string _Ktoupper(const std::string& src)
{
    return widetoutf8(case_transform(src, std::towupper));
}

void _Ktoupper( std::string& src)
{
    src = widetoutf8(case_transform(src, std::towupper));
}

// std::string _Ktolower(const std::string& src)
// {
// 	return widetoutf8(case_transform(src, std::towlower));
// }

// void _Ktolower(std::string& src)
// {
// 	src = widetoutf8(case_transform(src, std::towlower));
// }

int lc_compare(const std::string& first, const std::string& last, long long count = -1)
{
    if (!count)
        return 0;
    std::wstring fu = case_transform(first, std::towlower);
    std::wstring lu = case_transform(last, std::towlower);
    int ret = 0;
    if (count < 0)
        ret = std::wcscmp(fu.c_str(), lu.c_str());
    else
        ret = std::wcsncmp(fu.c_str(), lu.c_str(), count);
    return ret;
}

int _Kstricmp(const std::string& first, const std::string& last)
{
	return lc_compare(first, last);
}

int _Kstrnicmp(const std::string& first, const std::string& last, size_t count)
{
	return lc_compare(first, last, count);
}

void set_locale(const char* a_lname)
{
// #ifdef _WIN32
// 	SetConsoleOutputCP(65001);
// #endif
// 	std::setlocale(LC_ALL, a_lname);
}

int sprintf_s ( char * To, int MaxLen, const char * Fmt, ... )
{
    va_list arg;
    va_start(arg,Fmt);
    return vsnprintf ( To, MaxLen, Fmt, arg );
}
FILE * l_fopen ( const char * filename,
              const char * modes )
{
    return fopen ( filename, modes );
}
