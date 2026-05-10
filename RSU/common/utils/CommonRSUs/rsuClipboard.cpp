#include <rsuClipboard.h>

void rsuTxtToClp(const std::string& a_Txt)
{
#ifdef _WIN32
  if( !OpenClipboard( NULL ))
    return;
  if( !EmptyClipboard() )
  {
    CloseClipboard();
    return;
  }
  size_t L = a_Txt.length() + 1;
  HGLOBAL hData = GlobalAlloc( GMEM_MOVEABLE|GMEM_ZEROINIT, L );
  if ( hData == NULL )
  {
    CloseClipboard();
    return;
  }
  LPSTR pclpTxt = (LPSTR)GlobalLock( hData );
  memcpy( pclpTxt, a_Txt.c_str(), L);
  GlobalUnlock( hData );
  SetClipboardData( CF_TEXT, hData );
  CloseClipboard();
#endif
}
