#include "FscList.h"
static char* sid = "Операция OR";
// 
bool SFscList::FuncParser1F( VAR_PARSER )
{
  return MultiIN(fsc,p,dat,tmp);
}
//
//void FuncDebugs1F( VAR_DEBUGS )
//  {
//  strcat_s( str, sid );
//  strcat_s( str, "\r\n------------------------------------------------------------\r\n" );
//  FuncDebugsdo( p, str );
//  }
//
#ifdef _WIN32
void SFscList::FuncPaints1F( VAR_PAINTS )
{
  p.MultiOut( fsc, font, nSelect, "OR" );
}
#endif
//// Входы только слева
void SFscList::FuncSetLnk1F( VAR_SETLNK )
{
  if ( x == p.rect.right )
    return;
  p.MultiLnk( fsc, nLink, y );
}
////-----------------------------------------------------------------------------
//UINT FuncCursor1F( VAR_CURSOR )
//  {
//  SMultiIN* dat = (SMultiIN*)fsc->Data(p.nBuff);
//  return dat->O;
//  }
////-----------------------------------------------------------------------------
UINT* SFscList::FuncOutput1F( VAR_OUTPUT )
{
  SMultiIN* dat = (SMultiIN*)fsc->Data(p.nBuff);
  n = 1;
  return &dat->O;
}
////-----------------------------------------------------------------------------
void SFscList::FuncTimers1F( VAR_TIMERS )
{
  SMultiIN* dat = (SMultiIN*)fsc->Data(p.nBuff);
  SDotValue* O = fsc->DotV(dat->O);
  if ( O->dwFlags & 1 )
    return;
  //
  bool o = false;
  UINT nLink = dat->I;
  while ( nLink != -1 )
    {
    UINT nItem = fsc->ItemLink(nLink);
    SDotValue* i = fsc->DotV(nItem);
    o |= i->Bol();
    if ( o )
      break;
    nLink = fsc->NextLink(nLink);
    }
  O->Set( o );
}
////-----------------------------------------------------------------------------
static LPCSTR s_pName[] = { "O" };
int SFscList::FuncQuickWatch1F( IFscStorage* fsc, LFscBase *obj, IFscStorage::SVarInfo *pvi, int cVI, LPCSTR *ppPntName )
{
  return 0;
}

int SFscList::SaveState1F( IStateSer *psaver, IFscStorage* fsc, LFscBase *obj )
{
  SMultiIN* dat = (SMultiIN*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->O) };
  for( int i=0; i<_countof(vals); ++i )
  {
    psaver->WriteDotV( vals[i], s_pName[i] );
  }
  return 0;
}

int SFscList::RestoreState1F( IStateSer *prest, IFscStorage* fsc, LFscBase *obj )
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
