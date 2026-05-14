#pragma once
#include <string>
#include <algorithm>
#include <cwctype>
//#include <cutf.h>
#include <locale.h>

#define FMT_HEADER_ONLY
//#include "format.h"

#ifndef _WIN32
	#define _towupper_l towupper_l
	#define _towlower_l towlower_l
#endif

namespace cross
{
	//
	// Строка для работы с utf-8 содержимым на Lin/Win платформах
	//
	class string
	{
	public:
		static inline std::wstring s_2_ws(const std::string& s)
		{
			std::wstring ret;
			if (!s.empty())
			{
				size_t wide_char_count = utf8towchar(s.c_str(), s.length(), nullptr, 0);
				ret.resize(wide_char_count);
				utf8towchar(s.c_str(), s.length(), (wchar_t*)ret.c_str(), ret.size());
			}
			return ret;
		}

#ifdef _WIN32
		static inline std::string toupper(const std::string& src)
		{
			return widetoutf8(case_transform(src.c_str(), _towupper_l));
		}

		static inline void toupper(std::string& src)
		{
			src = widetoutf8(case_transform(src.c_str(), _towupper_l));
		}

		static inline std::string tolower(const std::string& src)
		{
			return widetoutf8(case_transform(src.c_str(), _towlower_l));
		}

		static inline void tolower(std::string& src)
		{
			src = widetoutf8(case_transform(src.c_str(), _towlower_l));
		}

		static inline int stricmp(const std::string& first, const std::string& last)
		{
			return string::lc_compare(first.c_str(), last.c_str());
		}

		static inline int strnicmp(const std::string& first, const std::string& last, size_t count)
		{
			return string::lc_compare(first.c_str(), last.c_str(), count);
		}

		static inline int stricmp(const char* first, const char* last)
		{
			return string::lc_compare(first, last);
		}

		static inline int strnicmp(const char* first, const char* last, size_t count)
		{
			return string::lc_compare(first, last, count);
		}
#else
		static std::string toupper(const std::string& src);

		static void toupper(std::string& src);

		static std::string tolower(const std::string& src);

		static void tolower(std::string& src);

		static int stricmp(const std::string& first, const std::string& last);

		static int strnicmp(const std::string& first, const std::string& last, size_t count);

		static int stricmp(const char* first, const char* last);

		static int strnicmp(const char* first, const char* last, size_t count);
#endif

		// for #define to prevent recursion (crossplaform.h)
		static inline char* __strcpy(char* __restrict __dest, const char* __restrict __src)
		{
			return cross::string::strcpy(__dest, __src);
		}

		// for #define to prevent recursion (crossplaform.h)
		static inline char* __strncpy(char* __restrict s1, const char* __restrict s2, size_t n)
		{
			return cross::string::strncpy(s1, s2, n);
		}

		static inline char* strcpy(char* __restrict __dest, const char* __restrict __src)
		{
			auto length = strlen(__src);
			memcpy(__dest, __src, length + 1);
			return __dest;
		}

		// vladexl:  from glibc
		static inline char* strncpy(char* __restrict s1, const char* __restrict s2, size_t n)
		{
		   size_t size = strnlen(s2, n);
		   if (size != n)
		   {
			   memset(s1 + size, '\0', n - size);
		   }

		   return (char*)memcpy(s1, s2, size);
		}
		
		//
		// @brief Example: cross::string::starts_with("День", U'Д')  [UTF-32 character literal]
		//
		static bool starts_with(const char * str, uint32_t symbol);

		//
		// @brief Example: cross::string::front("День") == U'Д'  [UTF-32 character literal]
		//
		static uint32_t front(const char* str);

		//
		// @brief Example: auto result = cross::string::replace("original string", "from", "to")
		// @brief Replaces all occurrences of substring 'from' to 'to' 
		//
		static std::string replace(std::string str, const std::string& from, const std::string& to);

		//
		// @brief Converts source string in any code page to UTF-8.
		//
		// @param sourceCodePage https://learn.microsoft.com/en-us/windows/win32/intl/code-page-identifiers, locale::get_codepage_id() can convert from string names
		// @throws std::invalid_argument
		// @throws std::runtime_error
		static void convert_to_utf8(unsigned int sourceCodePageId, const char* sourceText, char* targetText, size_t targetSize);

		//
		// @brief Converts source string in any code page to UTF-8.
		//
		// @param sourceCodePage https://learn.microsoft.com/en-us/windows/win32/intl/code-page-identifiers, locale::get_codepage_id() can convert from string names
		// @throws std::invalid_argument
		// @throws std::runtime_error
		static void convert_to_utf8(unsigned int sourceCodePageId, const char* sourceText, size_t sourceSize, char* targetText, size_t targetSize);

		//
		// @brief Converts source utf8-string to windows-1251.
		//
		// @throws std::invalid_argument
		// @throws std::runtime_error
		static void convert_to_1251(const char* sourceText, char* targetText, size_t targetSize);

		//
		// @brief Converts source utf8-string to windows-1251
		//
		// @throws std::invalid_argument
		// @throws std::runtime_error
		static void convert_to_1251(const char* sourceText, size_t sourceSize, char* targetText, size_t targetSize);

		//
		// @brief Converts source utf8-string to windows-1251
		//
		// @throws std::invalid_argument
		// @throws std::runtime_error
		static std::string convert_to_1251(const char* sourceText);

		/// <summary>
		/// Example: std::vector<std::string> cStrArr; cross::string::split_strings(str, del, std::back_inserter(cStrArr));
		/// </summary>
		template<class Iter> static Iter split_strings(const std::string& s, const std::string& delim, Iter out)
		{
			if (delim.empty()) {
				*out++ = s;
				return out;
			}
			size_t a = 0, b = s.find(delim);
			for (; b != std::string::npos;
				a = b + delim.length(), b = s.find(delim, a))
			{
				*out++ = std::move(s.substr(a, b - a));
			}
			*out++ = std::move(s.substr(a, s.length() - a));
			return out;
		}

		static void replaceAll(std::string& str, const std::string& from, const std::string& to) {
			if (from.empty())
				return;
			size_t start_pos = 0;
			while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
				str.replace(start_pos, from.length(), to);
				start_pos += to.length();
			}
		}

		static std::string& rtrim(std::string& s) {
			if (s.size() > 0 && s[s.size() - 1] == '\n')
				s.pop_back();
			if (s.size() > 0 && s[s.size() - 1] == '\r')
				s.pop_back();
			return s;
		}

		static char * tabs_to_spaces(char* buffer);

	private:
#ifndef _WIN32
		typedef locale_t _locale_t;
#endif
		typedef wint_t fn_case_transform(wint_t _C, _locale_t _Locale);

		static inline int lc_compare(const char* first, const char* last, long long count = -1)
		{
			if (!count)
			{
				return 0;
			}

			std::wstring fu = case_transform(first, _towlower_l);
			std::wstring lu = case_transform(last, _towlower_l);
			int ret = 0;
			if (count < 0)
				ret = std::wcscmp(fu.c_str(), lu.c_str());
			else
				ret = std::wcsncmp(fu.c_str(), lu.c_str(), count);
			return ret;
		}

		static inline std::wstring case_transform(const char * src, fn_case_transform& f)
		{
			static _locale_t independentLocale = nullptr;
			if (!independentLocale)
			{
#ifdef _WIN32
				independentLocale = _create_locale(LC_ALL, "");
#else
				independentLocale = newlocale(LC_ALL_MASK, "", 0);
#endif
			}

			std::wstring v_ws = s_2_ws(src);
			for (wchar_t* isrc = (wchar_t*)v_ws.c_str(); *isrc; isrc++)
			{
				*isrc = f(*isrc, independentLocale);
			}

			return v_ws;
		}

		static inline std::wstring s_2_ws(const char* s)
		{
			std::wstring ret;
			if (s && *s)
			{
				auto length = strlen(s);
				size_t wide_char_count = utf8towchar(s, length, nullptr, 0);
				ret.resize(wide_char_count);
				utf8towchar(s, length, (wchar_t*)ret.c_str(), ret.size());
			}

			return ret;
		}
	};
	
	//
	// @brief Example: std::string message = cross::format("Правильный ответ {0}", 42);
	//
	template <typename... T>
	static std::string format(fmt::v11::format_string<T...> fmt, T&&... args)
	{
		return fmt::format(fmt, std::forward<T>(args)...);
	}
};

#ifndef _WIN32
	#undef _towupper_l
	#undef _towlower_l
#endif

