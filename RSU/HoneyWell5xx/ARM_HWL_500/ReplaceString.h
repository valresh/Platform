#ifndef REPLACESTRING_H
#define REPLACESTRING_H
#include <string>
#include <map>
std::string ReplaceString(std::string subject, const std::string& search,  const std::string& replace);
std::string ReplaceStrings(std::string subject, std::map<std::string, std::string> *search);
std::wstring ReplaceStrings(std::wstring subject, std::map<std::string, std::string> *search); 

template <typename Type>
std::basic_string<Type> ReplaceString(std::basic_string<Type> subject, const std::basic_string<Type>& search,  const std::basic_string<Type>& replace) 
{
    size_t pos = 0;
    while((pos = subject.find(search, pos)) != std::basic_string<Type>::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
    return subject;
}

#endif
