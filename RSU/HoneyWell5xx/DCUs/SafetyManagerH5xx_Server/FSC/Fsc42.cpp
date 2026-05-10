#include "FscList.h"
static char* sid = "Вход в Процедуру";
//
bool SFscList::FuncParser42( VAR_PARSER )
{
  FuncParserdo(fsc,p,dat,tmp);
  SProcInOut io;
  io.IO = -1;
  io.io = dat.data[0];
  // Заказываем память
  p.nBuff = fsc->AddFscBuff( &io, sizeof(io) );
  return true;
}
//
//void FuncDebugs42( VAR_DEBUGS )
//  {
//  strcat_s( str, sid );
//  strcat_s( str, "\r\n------------------------------------------------------------\r\n" );
//  FuncDebugsdo( p, str );
//  }
//
#ifdef _WIN32
void SFscList::FuncPaints42( VAR_PAINTS )
{
  SProcInOut* dat = (SProcInOut*)fsc->Data(p.nBuff);
  //
  RECT rcr = p.rect; rcr.left  = rcr.right-(rcr.bottom-rcr.top);
  RECT rcl = p.rect; rcl.right = rcr.left-10;
  //
  CMyPen pen( font, p.Color( nSelect, dat->IO ) );
  CMyBrush brush( font, 0x010203, true );
  pen.Rect( rcr );
  p.DrawL( font, rcl, "Вход в процедуру", 0, 2 );
  //
  char txt[64] = "@";
  txt[0] += dat->io;
  font.Draw( rcr, txt );
}
#endif
//
void SFscList::FuncSetLnk42( VAR_SETLNK )
{
// Не обрабатываем
}
////-----------------------------------------------------------------------------
//UINT FuncCursor42( VAR_CURSOR )
//  {
//  SProcInOut* dat = (SProcInOut*)fsc->Data(p.nBuff);
//  return dat->IO;
//  }
////-----------------------------------------------------------------------------
UINT* SFscList::FuncOutput42( VAR_OUTPUT )
{
  SProcInOut* dat = (SProcInOut*)fsc->Data(p.nBuff);
  n = -1;
  return &dat->IO;
}
////-----------------------------------------------------------------------------
void SFscList::FuncTimers42( VAR_TIMERS )
{
}
////-----------------------------------------------------------------------------
static LPCSTR s_pName[] = { "I" };
int SFscList::FuncQuickWatch42( IFscStorage* fsc, LFscBase *obj, IFscStorage::SVarInfo *pvi, int cVI, LPCSTR *ppPntName )
{
  SProcInOut* dat = (SProcInOut*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->IO) };
  int n = 0;
  for( n; n<_countof(vals) && n<cVI; ++n )
  {
    pvi[n].val = vals[n];
    strcpy_s( pvi[n].szName, s_pName[n] );
  }
  return n;
}

int SFscList::SaveState42( IStateSer *psaver, IFscStorage* fsc, LFscBase *obj )
{
  return 0;
}

int SFscList::RestoreState42( IStateSer *prest, IFscStorage* fsc, LFscBase *obj )
{
  return 0;
}
