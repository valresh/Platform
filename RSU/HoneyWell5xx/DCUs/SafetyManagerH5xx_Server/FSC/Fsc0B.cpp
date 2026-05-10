#include "FscList.h"
static char* sid = "Digital Input";
//
bool SFscList::FuncParser0B( IFscStorage* fsc, SFscBase& p, SfscAbstruct& dat, SUniTemp& tmp )
{
  FuncParserdo(fsc,p,dat,tmp);
  //
  SDotValue dot(enumValueBol);
  SPointIn io;
  io.nOut = fsc->AddFscDots(&dot);
  char FullName[256];
  strcpy_s( FullName, (char*)(dat.data+1));
  int L = lstrlen ( FullName );
  while ( L > 0 && FullName[L-1] == ' ' )
    FullName[--L] = 0;
  CFSC_Info::eType PntType;
  io.nDbf = fsc->FindDbfTag( FullName, CFSC_Info::DI, &PntType );
  switch( PntType )
  {
  case CFSC_Info::FLD:
    p.mB = LFscBase::FLD;
    break;
  case CFSC_Info::COM:
    p.mB = LFscBase::COM;
    break;
  case CFSC_Info::PNL:
    p.mB = LFscBase::FLD;
    break;
  case CFSC_Info::MOV:
    p.mB = LFscBase::FLD;
    break;
  case CFSC_Info::BNS:
    p.mB = LFscBase::FLD;
    break;
  case CFSC_Info::MCC:
    p.mB = LFscBase::FLD;
    break;
  }
  io.In   = NULL;
  p.nBuff = fsc->AddFscBuff( &io, sizeof(io) );
  //
  ns_UT::SLocalTmp* item = tmp.Item();
  item->nCount = 1;
  item->dim[0] = io.nOut;
  return true;
}
//
//void FuncDebugs0B( VAR_DEBUGS )
//  {
//  strcat_s( str, sid );
//  strcat_s( str, "\r\n------------------------------------------------------------\r\n" );
//  FuncDebugsdo( p, str );
//  }
//
#ifdef _WIN32
void SFscList::FuncPaints0B( VAR_PAINTS )
{
  SPointIn* dat = fsc->PointIn(p.nBuff);
  SDotValue* V = fsc->DotV(dat->nOut);
  if ( V->dwFlags & 0x2 )
    {
    RECT rcl = p.rect; rcl.right = rcl.left+10;
    CMyBrush brush( font, NOTCONNECTED );
    brush.Rect( rcl );
    }
  int nNumb = dat->nDbf;
  p.BoxL( fsc, font, nNumb, p.rect );
  if ( nSelect == dat->nOut )
    {
    CMyPen pen( font, SELCOLOR );
    pen.Rect( p.rect );
    }
}
#endif
//
void SFscList::FuncSetLnk0B( VAR_SETLNK )
{
  // Не используется
}
////-----------------------------------------------------------------------------
//UINT FuncCursor0B( VAR_CURSOR )
//  {
//  SPointIn* dat = fsc->PointIn(p.nBuff);
//  return dat->nOut;
//  }
////-----------------------------------------------------------------------------
UINT* SFscList::FuncOutput0B( VAR_OUTPUT )
{
  SPointIn* dat = fsc->PointIn(p.nBuff);
  n = 1;
  return &dat->nOut;
}
////-----------------------------------------------------------------------------
void SFscList::FuncTimers0B( IFscStorage* fsc, SFscBase& p, double dt )
{
  SPointIn* dat = fsc->PointIn(p.nBuff);
  SDotValue* V = fsc->DotV(dat->nOut);
  if ( dat->In != NULL  )
    {
    BYTE* v = (BYTE*)dat->In;
    bool b = *v != 0 ? true : false;
    if ( (V->dwFlags & 1) == 0 )
    {
      V->Set(b);
      /*if( p.pFldCopyTo )
        *(bool*)p.pFldCopyTo = b;*/
    }
    if ( V->dwFlags & 0x2 )
      V->dwFlags &= ~0x2;
    }
  else
    {
    V->dwFlags |= 0x2;
    }
}
////-----------------------------------------------------------------------------
static LPCSTR s_pName[] = { "out" };
int SFscList::FuncQuickWatch0B( IFscStorage* fsc, LFscBase *obj, IFscStorage::SVarInfo *pvi, int cVI, LPCSTR *ppPntName )
{
  SPointIn* dat = (SPointIn*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->nOut) };

  int n = 0;
  for( n; n<_countof(vals) && n<cVI; ++n )
  {
    pvi[n].val = vals[n];
    strcpy_s( pvi[n].szName, s_pName[n] );
  }
  if( ppPntName )
    *ppPntName = fsc->DbfName( dat->nDbf, 1 );
  return n;
}

int SFscList::SaveState0B( IStateSer *psaver, IFscStorage* fsc, LFscBase *obj )
{
  SPointIn* dat = (SPointIn*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->nOut) };
  for( int i=0; i<_countof(vals); ++i )
  {
    psaver->WriteDotV( vals[i], s_pName[i] );
  }
  return 0;
}

int SFscList::RestoreState0B( IStateSer *prest, IFscStorage* fsc, LFscBase *obj )
{
  SPointIn* dat = (SPointIn*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->nOut) };
  for( int i=0; i<_countof(vals); ++i )
  {
    prest->ReadDotV( vals[i], s_pName[i] );
  }

  return 0;
}
