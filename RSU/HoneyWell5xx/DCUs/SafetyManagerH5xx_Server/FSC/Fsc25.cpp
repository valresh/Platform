#include "FscList.h"
static char* sid = "Операция NXOR";
//
bool SFscList::FuncParser25( VAR_PARSER )
{
  return LogicDB(fsc,p,dat,tmp);
}
//
//void FuncDebugs25( VAR_DEBUGS )
//  {
//  strcat_s( str, sid );
//  strcat_s( str, "\r\n------------------------------------------------------------\r\n" );
//  FuncDebugsdo( p, str );
//  }
//
#ifdef _WIN32
void SFscList::FuncPaints25( VAR_PAINTS )
{
  p.rect.right -= 6;
  COLORREF color = p.LogicOut( fsc, font, nSelect, "=1" );
  p.rect.right += 6;
  //
  RECT rcr = p.rect;
  rcr.left   = p.rect.right-6;
  rcr.top    = (p.rect.top+p.rect.bottom)/2-3;
  rcr.bottom = (p.rect.top+p.rect.bottom)/2+3;
  //
  CMyPen pen( font, color );
  CMyBrush brush( font, 0x101010, true );
  pen.Ellipse( rcr );
}
//
void FuncSetLnk25( VAR_SETLNK )
{
  // Вход только справа
  if ( x == p.rect.right ) 
    return;
  p.LogicLnk( fsc, nLink, y );
}
#endif
////-----------------------------------------------------------------------------
//UINT FuncCursor25( VAR_CURSOR )
//  {
//  SLogicDB* dat = (SLogicDB*)fsc->Data(p.nBuff);
//  int x = (p.rect.left+p.rect.right)/2;
//  int y = (p.rect.top+p.rect.bottom)/2;
//  return ( pt.x < x ) ? ((pt.y < y) ? dat->In1 : dat->In2) : dat->O;
//  }
////-----------------------------------------------------------------------------
UINT* SFscList::FuncOutput25( VAR_OUTPUT )
{
  SLogicDB* dat = (SLogicDB*)fsc->Data(p.nBuff);
  n = 1;
  return &dat->O;
}
////-----------------------------------------------------------------------------
void SFscList::FuncTimers25( VAR_TIMERS )
{
  SLogicDB* dat = (SLogicDB*)fsc->Data(p.nBuff);
  SDotValue* O = fsc->DotV(dat->O);
  if ( O->dwFlags & 1 )
    return;
  //
  SDotValue* I1 = fsc->DotV(dat->In1);
  SDotValue* I2 = fsc->DotV(dat->In2);
  if ( I1 == NULL || I2 == NULL )
    return;
  //
  bool i1 = I1->Bol();
  bool i2 = I2->Bol();
  bool o  = true;
  if ( i1 && i2 ) o = false;
  else
  if (!i1 &&!i2 ) o = false;
  //
  O->Set(!o);
}
////-----------------------------------------------------------------------------
int SFscList::FuncQuickWatch25( IFscStorage* fsc, LFscBase *obj, IFscStorage::SVarInfo *pvi, int cVI, LPCSTR *ppPntName )
{
  return 0;
}

int SFscList::SaveState25( IStateSer *psaver, IFscStorage* fsc, LFscBase *obj )
{
  return 0;
}

int SFscList::RestoreState25( IStateSer *prest, IFscStorage* fsc, LFscBase *obj )
{
  return 0;
}
