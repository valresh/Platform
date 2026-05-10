#include "Coord.h"

KCoord::KCoord(void)
{
  ZeroMemory( &coord, sizeof(coord) );
}

KCoord::~KCoord(void)
{
}

void KCoord::Parse( tinyxml2::XMLElementPtr &root )
{
    LONG val;
  for( tinyxml2::XMLNode *node = root->FirstChild(); node != NULL; node = node->NextSibling())
  {
    tinyxml2::XMLElementPtr item = node->ToElement();
    TINYXMLHELPER::GetXMLData(item, val);
    LPCSTR element_name = item->Name();   
    if (val < 0)
        val *= -1;
    if (strcmp(element_name, "Left") == 0)
        coord.left = val;
    else
    if (strcmp(element_name, "Top") == 0)
        coord.top = val;
    else
    if (strcmp(element_name, "Right") == 0)
        coord.right = val;
    else
    if (strcmp(element_name, "Bottom") == 0)
        coord.bottom = val;
  }
}

KCoord::operator SRECT()
{
  return coord;
}
