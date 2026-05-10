#include "FscList.h"
static char* sid = "Операция OR Horz";
// 
bool SFscList::FuncParser22( VAR_PARSER )
{
  MultiIN(fsc,p,dat,tmp);
  //
  ns_UT::SLocalTmp* item = tmp.Item();
  item->nDirect = 1;
  return true;
}
//
//void FuncDebugs22( VAR_DEBUGS )
//  {
//  strcat_s( str, sid );
//  strcat_s( str, "\r\n------------------------------------------------------------\r\n" );
//  FuncDebugsdo( p, str );
//  }
//
#ifdef _WIN32
void SFscList::FuncPaints22( VAR_PAINTS )
{
  p.MultiOut( fsc, font, nSelect, "OR" );
}
#endif
//// Входы только сверху
void SFscList::FuncSetLnk22( VAR_SETLNK )
{
  if ( y == p.rect.bottom ) 
    return;
  p.MultiLnk( fsc, nLink, x );
  //
  SDotValue* lnk = fsc->DotV(nLink);
  if ( lnk->eType == enumValueDbl ) //OR filter
  {
    SMultiIN*  dat = (SMultiIN*)fsc->Data(p.nBuff);
    SDotValue* dot = fsc->DotV(dat->O);
    dot->eType = enumValueDbl;
  }
}
////-----------------------------------------------------------------------------
//UINT FuncCursor22( VAR_CURSOR )
//  {
//  SMultiIN* dat = (SMultiIN*)fsc->Data(p.nBuff);
//  return dat->O;
//  }
////-----------------------------------------------------------------------------
UINT* SFscList::FuncOutput22( VAR_OUTPUT )
{
  SMultiIN* dat = (SMultiIN*)fsc->Data(p.nBuff);
  n = 1;
  return &dat->O;
}
////-----------------------------------------------------------------------------
void SFscList::FuncTimers22( VAR_TIMERS )
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
int SFscList::FuncQuickWatch22( IFscStorage* fsc, LFscBase *obj, IFscStorage::SVarInfo *pvi, int cVI, LPCSTR *ppPntName )
{
  return 0;
}

int SFscList::SaveState22( IStateSer *psaver, IFscStorage* fsc, LFscBase *obj )
{
  SMultiIN* dat = (SMultiIN*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->O) };
  for( int i=0; i<_countof(vals); ++i )
  {
    psaver->WriteDotV( vals[i], s_pName[i] );
  }
  return 0;
}

int SFscList::RestoreState22( IStateSer *prest, IFscStorage* fsc, LFscBase *obj )
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
