#pragma once
#include <Common.h>
#include <list>
#include <string>

class KTagSkipFilter
{
  bool m_bCfgRead;
  typedef std::list<std::string> tTagsSkip_;
  tTagsSkip_ m_tagsSkipCont;
  void GetList();
public:
  KTagSkipFilter();
  bool SkipWriteEvent( SSendToModel &send );
  static KTagSkipFilter& Instance();
};
