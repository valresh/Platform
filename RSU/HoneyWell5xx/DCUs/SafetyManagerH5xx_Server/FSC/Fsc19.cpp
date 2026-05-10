#include "FscList.h"
static char* sid = "Analog Output (AO)";
//
bool SFscList::FuncParser19( VAR_PARSER )
{
  FuncParserdo(fsc,p,dat,tmp);
  //
  SPointOut io;
  io.nIn  = -1;
  char FullName[256];
  strcpy_s ( FullName, (char*)(dat.data+1));
  int L = lstrlen ( FullName );
  while ( L > 0 && FullName[L-1] == ' ' )
    FullName[--L] = 0;
  CFSC_Info::eType PntType;
  io.nDbf = fsc->FindDbfTag( FullName, CFSC_Info::AO, &PntType );
  switch( PntType )
  {
  case CFSC_Info::FLD:
    p.mB = LFscBase::FLD;
    break;
  case CFSC_Info::COM:
    p.mB = LFscBase::COM;
    break;
  }
  io.Out  = NULL;
  p.nBuff = fsc->AddFscBuff( &io, sizeof(io) );
  return true;
}
//
//void FuncDebugs19( VAR_DEBUGS )
//  {
//  strcat_s( str, sid );
//  strcat_s( str, "\r\n------------------------------------------------------------\r\n" );
//  FuncDebugsdo( p, str );
//  }
//
#ifdef _WIN32
void SFscList::FuncPaints19( VAR_PAINTS )
{
  SPointOut* dat = fsc->PointOut(p.nBuff);
  if ( dat->nIn == -1 )
    {
    RECT rcr = p.rect; rcr.left = rcr.right-10;
    CMyBrush brush( font, 0xa000e0 );
    brush.Rect( rcr );
    }
  else
    {
    SDotValue* V = fsc->DotV(dat->nIn);
    if ( V->dwFlags & 0x2 )
      {
      RECT rcr = p.rect; rcr.left = rcr.right-10;
      CMyBrush brush( font, NOTCONNECTED );
      brush.Rect( rcr );
      }
    }
  int nNumb = dat->nDbf;
  p.BoxR( fsc, font, nNumb, p.rect );
  if ( nSelect == dat->nIn )
    {
    CMyPen pen( font, SELCOLOR );
    pen.Rect( p.rect );
    }
  p.BadRect( font, dat->nIn );
}
#endif
//
void SFscList::FuncSetLnk19( VAR_SETLNK )
{
  if ( x == p.rect.right ) 
    return;
  SPointOut* dat = (SPointOut*)fsc->Data(p.nBuff);
  if ( dat->nIn == -1 )
    dat->nIn = nLink;
  SDotValue* dot = fsc->DotV(nLink);
  ASS( dot->eType == enumValueDbl );
}
////-----------------------------------------------------------------------------
//UINT FuncCursor19( VAR_CURSOR )
//  {
//  SPointOut* dat = fsc->PointOut(p.nBuff);
//  return dat->nIn;
//  }
////-----------------------------------------------------------------------------
UINT* SFscList::FuncOutput19( VAR_OUTPUT )
{
  n = 0;
  return NULL;
}
////-----------------------------------------------------------------------------
void SFscList::FuncTimers19( VAR_TIMERS )
{
  SPointOut* dat = fsc->PointOut(p.nBuff);
  if ( dat->nIn != -1 )
    {
    SDotValue* V = fsc->DotV(dat->nIn);
    if ( dat->Out != NULL )
      {
      double* v = (double*)dat->Out;
      *v = V->Dbl();
      if ( V->dwFlags & 0x2 )
        V->dwFlags &= ~0x2;
      }
    else
      {
      V->dwFlags |= 0x2;
      }
    }
}
////-----------------------------------------------------------------------------
static LPCSTR s_pName[] = { "in" };
int SFscList::FuncQuickWatch19( IFscStorage* fsc, LFscBase *obj, IFscStorage::SVarInfo *pvi, int cVI, LPCSTR *ppPntName )
{
  SPointOut* dat = (SPointOut*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->nIn) };

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

int SFscList::SaveState19( IStateSer *psaver, IFscStorage* fsc, LFscBase *obj )
{
  SPointOut* dat = (SPointOut*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->nIn) };
  for( int i=0; i<_countof(vals); ++i )
  {
    psaver->WriteDotV( vals[i], s_pName[i] );
  }
  return 0;
}

int SFscList::RestoreState19( IStateSer *prest, IFscStorage* fsc, LFscBase *obj )
{
  SPointOut* dat = (SPointOut*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->nIn) };
  for( int i=0; i<_countof(vals); ++i )
  {
    prest->ReadDotV( vals[i], s_pName[i] );
  }
  return 0;
}
