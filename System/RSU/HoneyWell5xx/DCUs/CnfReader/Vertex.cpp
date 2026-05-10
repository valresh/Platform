#include "Vertex.h"
#include <TinyXmlReader.h>

void KVertex::Parse( tinyxml2::XMLElementPtr &root )
{
  int *p = &vertex.x;
  for( tinyxml2::XMLNode *node = root->FirstChild(); node != NULL; node = node->NextSibling(), ++p )
  {
    tinyxml2::XMLElementPtr item = node->ToElement();
#ifdef _DEBUG
    LPCSTR element_name = item->Name();
#endif
    TINYXMLHELPER::GetXMLData( item, *p );
    if( *p < 0 )
      *p *= -1;
  }
}
