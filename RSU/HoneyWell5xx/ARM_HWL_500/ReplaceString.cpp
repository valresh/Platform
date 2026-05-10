#include "stdafx.h"
#include "ReplaceString.h"
#include <map>

std::string ReplaceString(std::string subject, const std::string& search,  const std::string& replace) 
{
    size_t pos = 0;
    while((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
    return subject;
}

std::string ReplaceStrings(std::string subject, std::map<std::string, std::string> *search) 
{
  if(search && search->size())
  {
      for(std::map<std::string, std::string>::iterator p=search->begin(); p!=search->end(); p++)
      {
          //if(subject.find('#') == std::string::npos) //заменяем только теги начинающиеся с #
          //   break;
          subject = ReplaceString(subject, p->first, p->second);
      }
  }
  return subject;
}

std::wstring ReplaceStrings(std::wstring subject, std::map<std::string, std::string> *search) 
{
  if(search && search->size())
  {
      for(std::map<std::string, std::string>::iterator p=search->begin(); p!=search->end(); p++)
      {
      //     WCHAR wfirst[4*_MAX_PATH];
      //     WCHAR wsecond[4*_MAX_PATH];
      //     MultiByteToWideChar(CP_ACP, 0, p->first.c_str(), -1, wfirst, 4*_MAX_PATH-1);
      //     MultiByteToWideChar(CP_ACP, 0, p->second.c_str(), -1, wsecond, 4*_MAX_PATH-1);
      //     subject = ReplaceString(subject, std::wstring(wfirst), std::wstring(wsecond));
       }
  }
  return subject;
}


