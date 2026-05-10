#include "FscList.h"
static char* sid = "Операция &";
//
bool SFscList::FuncParser1E( VAR_PARSER )
{
  MultiIN(fsc,p,dat,tmp);
  return true;
}
//
//void FuncDebugs1E( VAR_DEBUGS )
//  {
//  strcat_s( str, sid );
//  strcat_s( str, "\r\n------------------------------------------------------------\r\n" );
//  FuncDebugsdo( p, str );
//  }
//
#ifdef _WIN32
void SFscList::FuncPaints1E( VAR_PAINTS )
{
  p.MultiOut( fsc, font, nSelect, "&&" );
}
#endif
//// Входы только слева
void SFscList::FuncSetLnk1E( VAR_SETLNK )
{
  if ( x == p.rect.right ) 
    return;
  p.MultiLnk( fsc, nLink, y );
  //
  SDotValue* lnk = fsc->DotV(nLink);
  SMultiIN*  dat = (SMultiIN*)fsc->Data(p.nBuff);
  SDotValue* dot = fsc->DotV(dat->O);

  if( lnk->eType == enumValueDbl ) //AND filter
  {
    dot->eType = enumValueDbl;
  }
  else if( lnk->eType == enumValueI64 )
  {
    dot->eType = enumValueDbl;
  }
  else if( lnk->eType == enumValueInt )
  {
    dot->eType = enumValueInt;
  }
  else if( lnk->eType == enumValueBol ){}
  else
  {
    KKK();
  }
}
////-----------------------------------------------------------------------------
//UINT FuncCursor1E( VAR_CURSOR )
//  {
//  SMultiIN* dat = (SMultiIN*)fsc->Data(p.nBuff);
//  return dat->O;
//  }
////-----------------------------------------------------------------------------
UINT* SFscList::FuncOutput1E( VAR_OUTPUT )
{
  SMultiIN* dat = (SMultiIN*)fsc->Data(p.nBuff);
  n = 1;
  return &dat->O;
}
////-----------------------------------------------------------------------------
void SFscList::FuncTimers1E( VAR_TIMERS )
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
      case enumValueI64:
      case enumValueDbl: d  = i->dVal ; break;
      case enumValueInt: d  = i->nVal ; break;
      default:
        KKK();
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
    case enumValueInt:
    case enumValueI64:
    case enumValueDbl: O->Set(d); break;
    default:
      KKK();
    }
}
////-----------------------------------------------------------------------------
static LPCSTR s_pName[] = { "O" };
int SFscList::FuncQuickWatch1E( IFscStorage* fsc, LFscBase *obj, IFscStorage::SVarInfo *pvi, int cVI, LPCSTR *ppPntName )
{
  return 0;
}

int SFscList::SaveState1E( IStateSer *psaver, IFscStorage* fsc, LFscBase *obj )
{
  SMultiIN* dat = (SMultiIN*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->O) };
  for( int i=0; i<_countof(vals); ++i )
  {
    psaver->WriteDotV( vals[i], s_pName[i] );
  }
  return 0;
}

int SFscList::RestoreState1E( IStateSer *prest, IFscStorage* fsc, LFscBase *obj )
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
