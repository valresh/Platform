#pragma once
#include <macros/IntrusivePtr.h>
#include <list>
#include <string>
#include <macros/base_iterator.h>

class IXmlNode;
typedef TIntrusivePtr<IXmlNode> tXmlNodePtr_;

struct SAttribute
{
  std::string szName;
  std::string szValue;
};
typedef std::list<tXmlNodePtr_> tChildrenCont_;
typedef std::list<SAttribute> tAttrCont_;

typedef n_RSUs::base_iterator<tChildrenCont_> tChildrenIterator_;
typedef n_RSUs::base_iterator<tAttrCont_> tAttrsIterator_;

class IXmlNode : public ref_counted
{
public:
  virtual ~IXmlNode(){}
  virtual tXmlNodePtr_ AddChild( tXmlNodePtr_ ) = 0;
  virtual void AddAttr( LPCSTR pszName, LPCSTR pszValue, LPCSTR sep = " " ) = 0;
  virtual LPCSTR GetNodeType() = 0;
  virtual tChildrenIterator_ EnumChildren() = 0;
  virtual tAttrsIterator_ EnumAttrs() = 0;
  virtual LPCSTR GetData() = 0;
};
