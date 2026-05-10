#include "FscList.h"
static char* sid = "Операция & Horz";
//
bool SFscList::FuncParser21( VAR_PARSER )
{
  MultiIN(fsc,p,dat,tmp);
  //
  ns_UT::SLocalTmp* item = tmp.Item();
  item->nDirect = 1;
  return true;
}
//
//void FuncDebugs21( VAR_DEBUGS )
//  {
//  strcat_s( str, sid );
//  strcat_s( str, "\r\n------------------------------------------------------------\r\n" );
//  FuncDebugsdo( p, str );
//  }
//
#ifdef _WIN32
void SFscList::FuncPaints21( VAR_PAINTS )
{
  p.MultiOut( fsc, font, nSelect, "&&" );
}
#endif
//// Входы только сверху
void SFscList::FuncSetLnk21( VAR_SETLNK )
{
  if ( y == p.rect.bottom ) 
    return;
  p.MultiLnk( fsc, nLink, x );
  //
  SDotValue* lnk = fsc->DotV(nLink);
  if ( lnk->eType == enumValueDbl ) //AND filter
  {
    SMultiIN*  dat = (SMultiIN*)fsc->Data(p.nBuff);
    SDotValue* dot = fsc->DotV(dat->O);
    dot->eType = enumValueDbl;
  }
}
////-----------------------------------------------------------------------------
//UINT FuncCursor21( VAR_CURSOR )
//  {
//  SMultiIN* dat = (SMultiIN*)fsc->Data(p.nBuff);
//  return dat->O;
//  }
////-----------------------------------------------------------------------------
UINT* SFscList::FuncOutput21( VAR_OUTPUT )
{
  SMultiIN* dat = (SMultiIN*)fsc->Data(p.nBuff);
  n = 1;
  return &dat->O;
}
////-----------------------------------------------------------------------------
void SFscList::FuncTimers21( VAR_TIMERS )
{
  SMultiIN* dat = (SMultiIN*)fsc->Data(p.nBuff);
  SDotValue* O = fsc->DotV(dat->O);
  if ( O->dwFlags & 1 )
    return;
  //
  double d = 0;
  bool   o = true;
  UINT nLink = dat->I;
  while ( nLink != -1 )
    {
    UINT nItem = fsc->ItemLink(nLink);
    SDotValue* i = fsc->DotV(nItem);
    switch ( i->eType )
      {
      case enumValueBol: o &= i->Bol(); break;
      case enumValueDbl: d  = i->dVal ; break;
      }
    if ( !o )
      {
      d = 0;
      break;
      }
    nLink = fsc->NextLink(nLink);
    }
  //
  switch ( O->eType )
    {
    case enumValueBol: O->Set(o); break;
    case enumValueDbl: O->Set(d); break;
    }
}
////-----------------------------------------------------------------------------
static LPCSTR s_pName[] = { "O" };
int SFscList::FuncQuickWatch21( IFscStorage* fsc, LFscBase *obj, IFscStorage::SVarInfo *pvi, int cVI, LPCSTR *ppPntName )
{
  return 0;
}

int SFscList::SaveState21( IStateSer *psaver, IFscStorage* fsc, LFscBase *obj )
{
  SMultiIN* dat = (SMultiIN*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->O) };
  for( int i=0; i<_countof(vals); ++i )
  {
    psaver->WriteDotV( vals[i], s_pName[i] );
  }
  return 0;
}

int SFscList::RestoreState21( IStateSer *prest, IFscStorage* fsc, LFscBase *obj )
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
