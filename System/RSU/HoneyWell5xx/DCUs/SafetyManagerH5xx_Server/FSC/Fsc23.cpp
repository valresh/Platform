#include "FscList.h"
static char* sid = "Операция NAND";
// 
bool SFscList::FuncParser23( VAR_PARSER )
{
  return MultiIN(fsc,p,dat,tmp);
}
//
//void FuncDebugs23( VAR_DEBUGS )
//  {
//  strcat_s( str, sid );
//  strcat_s( str, "\r\n------------------------------------------------------------\r\n" );
//  FuncDebugsdo( p, str );
//  }
//
#ifdef _WIN32
void SFscList::FuncPaints23( VAR_PAINTS )
{
  RECT rcl = p.rect; rcl.right -= 6;
  RECT rcr = p.rect;
  rcr.left   = rcl.right;
  rcr.top    = (rcl.top+rcl.bottom)/2-3;
  rcr.bottom = (rcl.top+rcl.bottom)/2+3;
  //
  COLORREF color = p.MultiOut( fsc, font, nSelect, "N&&", &rcl );
  //
  CMyPen pen( font, color );
  CMyBrush brush( font, 0x101010, true );
  pen.Ellipse( rcr );
}
#endif
//// Входы только слева
void SFscList::FuncSetLnk23( VAR_SETLNK )
{
  if ( x == p.rect.right ) 
    return;
  p.MultiLnk( fsc, nLink, y );
}
////-----------------------------------------------------------------------------
//UINT FuncCursor23( VAR_CURSOR )
//  {
//  SMultiIN* dat = (SMultiIN*)fsc->Data(p.nBuff);
//  return dat->O;
//  }
////-----------------------------------------------------------------------------
UINT* SFscList::FuncOutput23( VAR_OUTPUT )
{
  SMultiIN* dat = (SMultiIN*)fsc->Data(p.nBuff);
  n = 1;
  return &dat->O;
}
////-----------------------------------------------------------------------------
void SFscList::FuncTimers23( VAR_TIMERS )
{
  SMultiIN* dat = (SMultiIN*)fsc->Data(p.nBuff);
  SDotValue* O = fsc->DotV(dat->O);
  if ( O->dwFlags & 1 )
    return;
  //
  bool o = true;
  UINT nLink = dat->I;
  while ( nLink != -1 )
    {
    UINT nItem = fsc->ItemLink(nLink);
    SDotValue* i = fsc->DotV(nItem);
    o &= i->Bol();
    if ( !o )
      break;
    nLink = fsc->NextLink(nLink);
    }
  O->Set( !o );
}
////-----------------------------------------------------------------------------
static LPCSTR s_pName[] = { "O" };
int SFscList::FuncQuickWatch23( IFscStorage* fsc, LFscBase *obj, IFscStorage::SVarInfo *pvi, int cVI, LPCSTR *ppPntName )
{
  return 0;
}

int SFscList::SaveState23( IStateSer *psaver, IFscStorage* fsc, LFscBase *obj )
{
  SMultiIN* dat = (SMultiIN*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->O) };
  for( int i=0; i<_countof(vals); ++i )
  {
    psaver->WriteDotV( vals[i], s_pName[i] );
  }
  return 0;
}

int SFscList::RestoreState23( IStateSer *prest, IFscStorage* fsc, LFscBase *obj )
{
  if( prest->GetSavedVersionFormat() < 0x03000002 )
    return 0;
  SMultiIN* dat = (SMultiIN*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->O) };
  for( int i=0; i<_countof(vals); ++i )
  {
    prest->ReadDotV( vals[i], s_pName[i] );
  }
  return 0;
}
