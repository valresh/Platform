#ifndef CString_H
#define CString_H


//ИЗ НЕЙРОСЕТИ, ЧТО БЫ НЕ ПРАВИТЬ cross::CString 

#include <format>
#include <string>
#include <algorithm>
#include <cctype>
#include <iostream>

namespace cross {



// #ifndef  lstrlen
// #define lstrlen strlen
// #endif

// Функция преобразует строку в нижний регистр
inline std::string toLowerCase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    return result;
}


// Функция для сравнения строк без учета регистра
inline int lstrcmpi(const std::string& str1, const std::string& str2) {
    // Используем алгоритм transform для приведения строк к нижнему регистру
    auto it1 = str1.begin();
    auto it2 = str2.begin();

    while (it1 != str1.end() && it2 != str2.end()) {
        // Сравниваем символы в нижнем регистре
        int result = std::tolower(*it1) - std::tolower(*it2);
        if (result != 0) {
            return result;
        }
        ++it1;
        ++it2;
    }

    // Если дошли до конца одной из строк
    if (it1 == str1.end() && it2 == str2.end()) {
        return 0; // строки равны
    } else if (it1 == str1.end()) {
        return -1; // первая строка короче
    } else {
        return 1; // вторая строка короче
    }
}


inline void lstrcpy(char* dest,  const char* src, size_t size) {
    if (size == 0) {
        throw std::invalid_argument("Размер буфера должен быть больше 0");
    }

    // Копируем строку с учетом размера буфера
    strncpy(dest, src, size - 1);

    dest[size - 1] = '\0'; // Гарантируем нулевой терминатор
}

// inline void lstrcpy(char* dest, const char* src) {
//     if ( strlen(src) == 0) {
//         throw std::invalid_argument("Размер буфера должен быть больше 0");
//     }

//     // Копируем строку с учетом размера буфера
//     strncpy(dest, src, strlen(src));

//     dest[strlen(src) - 1] = '\0'; // Гарантируем нулевой терминатор
// }

class CString : public std::string{
private:
    std::string m_data;  // Хранилище строки

public:

    // cross::CString Format(cross::CString format/*строка формата*/, int arc, const size_t szbuf = 15)
    // {
    //     cross::CString str;
    //     if (szbuf > 15) str.reserve(szbuf);
    //     sprintf_s(const_cast<char*>(str.c_str()), szbuf, const_cast<char*>(format.c_str()), arc);
    //     int sz = str.size();
    //     cross::CString s(str.c_str());
    //     return s;
    // }


    CString Replace(  const CString& from, const CString& to)
    {
        size_t start_pos{0};
        while ((start_pos = m_data.find(from, start_pos)) != std::string::npos) {
            m_data.replace(start_pos, from.length(), to);
            // В случае, если to содержит from, например, замена «x» на «yx»
            start_pos += to.length();
        }
        return m_data;
    }


    CString MakeUpper()
    {
        std::transform(m_data.begin(), m_data.end(), m_data.begin(), ::toupper);
        return m_data;
    }


    bool IsEmpty() {

        if(m_data.size()==0)
            return true;
        else
            return false;
    }



    // Конструкторы
    CString() = default;  // Конструктор по умолчанию

    CString(const char* str) : m_data(str) {}  // Конструктор из const char*

    CString(const std::string& str) : m_data(str) {}  // Конструктор из std::string

    // Деструктор не нужен, так как std::string управляет памятью автоматически

    // Операторы присваивания
    cross::CString& operator=(const char* str) {
        m_data = str;
        return *this;
    }

    CString& operator=(const std::string& str) {
        m_data = str;
        return *this;
    }

    std::string getStdString() {
        return m_data;
    }


    CString Mid(size_t pos, size_t n) {
        return m_data.substr(pos, n);
    }

    CString Mid(size_t pos) {
        return m_data.substr(pos);
    }


    int GetLength() {
        return m_data.length();
    }

    int Compare(std::string format) {
        return m_data.compare(format);
    }

    CString Format(CString format, const char* arg1, const char* arg2 , const char* arg3) {

        std::string str = format.getStdString();



        str.replace(str.begin(), str.end(), "%s", "{}");
        str.replace(str.begin(), str.end(), "%d", "{}");
        str.replace(str.begin(), str.end(), "%f", "{}");

        std::string_view view(str.data(), str.size());

        auto a =  std::vformat(view, std::make_format_args(arg1, arg2, arg3)); // OK

        return CString(a);
    }


    CString Format(CString format, const char* arg1, const char* arg2) {

        std::string str = format.getStdString();



        str.replace(str.begin(), str.end(), "%s", "{}");
        str.replace(str.begin(), str.end(), "%d", "{}");
        str.replace(str.begin(), str.end(), "%f", "{}");

        std::string_view view(str.data(), str.size());

        auto a =  std::vformat(view, std::make_format_args(arg1, arg2)); // OK

        return CString(a);
    }






    cross::CString Format(cross::CString format, const char* arg1) {

        std::string str = format.getStdString();

        str.replace(str.begin(), str.end(), "%s", "{}");
        str.replace(str.begin(), str.end(), "%d", "{}");
        str.replace(str.begin(), str.end(), "%f", "{}");

        std::string_view view(str.data(), str.size());

        auto a =  std::vformat(view, std::make_format_args(arg1)); // OK

        return cross::CString(a);
    }


    // std::string FormatString(const std::string& format, const std::vector<std::any>& args) {
    //     std::stringstream stream;
    //     size_t argIndex = 0;
    //     bool inBraces = false;
    //     std::string currentToken;

    //     for (char ch : format) {
    //         if (ch == '%') {
    //             if (inBraces) {
    //                 currentToken += ch;
    //             } else {
    //                 if (!currentToken.empty()) {
    //                     stream << currentToken;
    //                     currentToken.clear();
    //                 }
    //                 if (argIndex < args.size()) {
    //                     if (auto val = std::any_cast<std::string>(args[argIndex])) {
    //                         stream << *val;
    //                     } else if (auto val = std::any_cast<int>(args[argIndex])) {
    //                         stream << *val;
    //                     } else if (auto val = std::any_cast<double>(args[argIndex])) {
    //                         stream << *val;
    //                     }
    //                     argIndex++;
    //                 }
    //             }
    //         } else {
    //             currentToken += ch;
    //         }
    //     }

    //     if (!currentToken.empty()) {
    //         stream << currentToken;
    //     }

    //     return stream.str();
    // }


    //
    bool CompareNoCase(const std::string& str2) {
        if (m_data.size() != str2.size())
            return false;

        return std::equal(m_data.begin(), m_data.end(), str2.begin(), [](char ch1, char ch2) {
            return std::tolower(static_cast<unsigned char>(ch1)) == std::tolower(static_cast<unsigned char>(ch2));
        });
    }

    int CollateNoCase(const std::string& str2, const std::locale& loc = std::locale()) {
        std::collate<char> const& coll = std::use_facet<std::collate<char>>(loc);

        // Преобразование в нижний регистр с учётом локали
        std::string lower1 = toLowerCase(m_data);
        std::string lower2 = toLowerCase(str2);

        std::transform(lower1.begin(), lower1.end(), lower1.begin(), [&loc](char c) {
            return std::use_facet<std::ctype<char>>(loc).tolower(c);
        });

        std::transform(lower2.begin(), lower2.end(), lower2.begin(), [&loc](char c) {
            return std::use_facet<std::ctype<char>>(loc).tolower(c);
        });

        return coll.compare(lower1.data(), lower1.data() + lower1.size(),
                            lower2.data(), lower2.data() + lower2.size());
    }


    size_t Find(const std::string& str) {
        return m_data.find(str);
    }

    size_t Find(const char* str) {
        return m_data.find(str);
    }

    size_t Find(char str) {
        return m_data.find(str);
    }

    cross::CString& operator+=(const std::string& str) {
        m_data += str;
        return *this;
    }

    char*  GetBuffer() {
        return m_data.data();
    }


    // Оператор преобразования в const char*
    operator const char* () const {
        return m_data.c_str();
    }

    // Методы для работы со строкой
    size_t length() const {
        return m_data.length();
    }

    bool empty() const {
        return m_data.empty();
    }

    void clear() {
        m_data.clear();
    }

    void append(const char* str) {
        m_data.append(str);
    }

    void append(const std::string& str) {
        m_data.append(str);
    }

    // Оператор вывода в поток
    friend std::ostream& operator<<(std::ostream& os, const cross::CString& str) {
        return os << str.m_data;
    }



    // Оператор сравнения
    bool operator==(const cross::CString& other) const {
        return m_data == other.m_data;
    }

    // template<typename... Args>
    // std::string FormatString(const std::string& format, Args&&... args) {
    //     std::stringstream stream;
    //     std::vector<std::any> values = {std::forward<Args>(args)...};
    //     size_t argIndex = 0;
    //     bool inBraces = false;
    //     std::string currentToken;

    //     for (char ch : format) {
    //         if (ch == '%') {
    //             if (inBraces) {
    //                 currentToken += ch;
    //             } else {
    //                 if (!currentToken.empty()) {
    //                     stream << currentToken;
    //                     currentToken.clear();
    //                 }

    //                 if (argIndex < values.size()) {
    //                     if (auto val = std::any_cast<std::string>(values[argIndex])) {
    //                         stream << *val;
    //                     } else if (auto val = std::any_cast<int>(values[argIndex])) {
    //                         stream << *val;
    //                     } else if (auto val = std::any_cast<double>(values[argIndex])) {
    //                         stream << *val;
    //                     }
    //                     argIndex++;
    //                 }
    //             }
    //         } else {
    //             currentToken += ch;
    //         }
    //     }

    //     if (!currentToken.empty()) {
    //         stream << currentToken;
    //     }

    //     return stream.str();
    // }

};

}
#endif
