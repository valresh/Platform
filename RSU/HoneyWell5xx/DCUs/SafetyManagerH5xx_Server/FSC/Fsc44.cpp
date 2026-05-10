#include "FscList.h"
static char* sid = "Выход из Процедуры";
//
bool SFscList::FuncParser44( VAR_PARSER )
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
//void FuncDebugs44( VAR_DEBUGS )
//  {
//  strcat_s( str, sid );
//  strcat_s( str, "\r\n------------------------------------------------------------\r\n" );
//  FuncDebugsdo( p, str );
//  }
//
#ifdef _WIN32
void SFscList::FuncPaints44( VAR_PAINTS )
{
  SProcInOut* dat = (SProcInOut*)fsc->Data(p.nBuff);
  //
  RECT rcl = p.rect; rcl.right = rcl.left+(rcl.bottom-rcl.top);
  RECT rcr = p.rect; rcr.left  = rcl.right+10;
  //
  CMyPen pen( font, p.Color( nSelect, dat->IO ) );
  CMyBrush brush( font, 0x010203, true );
  pen.Rect( rcl );
  p.DrawL( font, rcr, "Выход из процедуры", 0, 2 );
  //
  char txt[64] = "@";
  txt[0] += dat->io;
  font.Draw( rcl, txt );
}
#endif
//
void SFscList::FuncSetLnk44( VAR_SETLNK )
{
  // Вход только слева
  if ( x == p.rect.right ) 
    return;
  SProcInOut* dat = (SProcInOut*)fsc->Data(p.nBuff);
  dat->IO = nLink;
}
////-----------------------------------------------------------------------------
//UINT FuncCursor44( VAR_CURSOR )
//  {
//  SProcInOut* dat = (SProcInOut*)fsc->Data(p.nBuff);
//  return dat->IO;
//  }
////-----------------------------------------------------------------------------
UINT* SFscList::FuncOutput44( VAR_OUTPUT )
{
  SProcInOut* dat = (SProcInOut*)fsc->Data(p.nBuff);
  n = -1;
  return &dat->IO;
}
////-----------------------------------------------------------------------------
void SFscList::FuncTimers44( VAR_TIMERS )
{
}
////-----------------------------------------------------------------------------
static LPCSTR s_pName[] = { "O" };
int SFscList::FuncQuickWatch44( IFscStorage* fsc, LFscBase *obj, IFscStorage::SVarInfo *pvi, int cVI, LPCSTR *ppPntName )
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

int SFscList::SaveState44( IStateSer *psaver, IFscStorage* fsc, LFscBase *obj )
{
  return 0;
}

int SFscList::RestoreState44( IStateSer *prest, IFscStorage* fsc, LFscBase *obj )
{
  return 0;
}
