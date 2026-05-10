#include "FscList.h"
static char* sid = "Дискретный переход доставка от multi";
//
bool SFscList::FuncParser0F( VAR_PARSER )
{
  FuncParserdo(fsc,p,dat,tmp);
  //
  SGoto io;
  FillGoto( io, dat );
  p.nBuff = fsc->AddFscBuff( &io, sizeof(io) );
  return true;
}
////
//void FuncDebugs0F( VAR_DEBUGS )
//  {
//  strcat_s( str, sid );
//  strcat_s( str, "\r\n------------------------------------------------------------\r\n" );
//  FuncDebugsdo( p, str );
//  }
//
#ifdef _WIN32
void SFscList::FuncPaints0F( VAR_PAINTS )
{
  SGoto* dat = (SGoto*)fsc->Data(p.nBuff);
  //
  RECT rcr = p.rect; rcr.left  = rcr.right-(rcr.bottom-rcr.top);
  RECT rcl = p.rect; rcl.right = rcr.left-10;
  //
  COLORREF color = dat->nData == -1 ? BADCOLOR:((dat->nData == nSelect) ? SELCOLOR :BOXCOLOR);
  CMyPen pen( font, color );
  CMyBrush brush( font, 0x010203, true );
  ::InflateRect( &rcr, 1, 1 );
  pen.Ellipse( rcr );
  pen.Rect( rcl );
  p.DrawL( font, rcl, "Переход", 0, 2 );
  //
  char txt[64*4];
  //
  sprintf_s( txt, 16, "%d", dat->nSrc );
  p.DrawC( font, rcr, txt, 0, 2 );
  //
  sprintf_s( txt, 16, "%d", dat->nDst );
  p.DrawC( font, rcr, txt, 1, 2 );
  //
  rcr.right -= 2;
  sprintf_s( txt, 16, "%d", dat->nIndex );
  p.DrawL( font, txt, 2, &rcr );
}
#endif
//
void SFscList::FuncSetLnk0F( VAR_SETLNK )
{
  // Не используется
}
////-----------------------------------------------------------------------------
//UINT FuncCursor0F( VAR_CURSOR )
//  {
//  SGoto* dat = (SGoto*)fsc->Data(p.nBuff);
//  return dat->nData;
//  }
////-----------------------------------------------------------------------------
UINT* SFscList::FuncOutput0F( VAR_OUTPUT )
{
  n = 0;
  return NULL;
}
////-----------------------------------------------------------------------------
void SFscList::FuncTimers0F( VAR_TIMERS )
{
}
////-----------------------------------------------------------------------------
int SFscList::FuncQuickWatch0F( IFscStorage* fsc, LFscBase *obj, IFscStorage::SVarInfo *pvi, int cVI, LPCSTR *ppPntName )
{
  return 0;
}

int SFscList::SaveState0F( IStateSer *psaver, IFscStorage* fsc, LFscBase *obj )
{
  return 0;
}

int SFscList::RestoreState0F( IStateSer *prest, IFscStorage* fsc, LFscBase *obj )
{
  return 0;
}
