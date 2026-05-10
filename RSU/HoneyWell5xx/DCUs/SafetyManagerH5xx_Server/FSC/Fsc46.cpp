#include "FscList.h"
#include <crosslocale.h>
static char* sid = "Multiple Input (BI)";
// 
bool SFscList::FuncParser46( VAR_PARSER )
{
  FuncParserdo(fsc,p,dat,tmp);
  p.ValidateB();
  //
  SDotValue dot(enumValueDbl);
  SPointIn io;

  char FullName[256];
  strcpy_s( FullName, (char*)(dat.data+1));
  int L = lstrlen ( FullName );
  while ( L > 0 && FullName[L-1] == ' ' )
    FullName[--L] = 0;
  io.nDbf = fsc->FindDbfTag( FullName, CFSC_Info::BI );
  io.In   = NULL;
  
  LPCSTR pszVal = fsc->DbfName( io.nDbf, 24 );
  if( pszVal && *pszVal )
    dot.dVal = cross::locale::safe_atof( pszVal );

  io.nOut = fsc->AddFscDots(&dot);

  p.nBuff = fsc->AddFscBuff( &io, sizeof(io) );

  //
  ns_UT::SLocalTmp* item = tmp.Item();
  item->nCount = 1;
  item->dim[0] = io.nOut;
  return true;
}
//
//void FuncDebugs46( VAR_DEBUGS )
//  {
//  strcat_s( str, sid );
//  strcat_s( str, "\r\n------------------------------------------------------------\r\n" );
//  FuncDebugsdo( p, str );
//  }
//
#ifdef _WIN32
void SFscList::FuncPaints46( VAR_PAINTS )
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
void SFscList::FuncSetLnk46( VAR_SETLNK )
{
// Не используется
}
////-----------------------------------------------------------------------------
//UINT FuncCursor46( VAR_CURSOR )
//  {
//  SPointIn* dat = fsc->PointIn(p.nBuff);
//  return dat->nOut;
//  }
////-----------------------------------------------------------------------------
UINT* SFscList::FuncOutput46( VAR_OUTPUT )
{
  SPointIn* dat = fsc->PointIn(p.nBuff);
  n = 1;
  return &dat->nOut;
}
////-----------------------------------------------------------------------------
void SFscList::FuncTimers46( VAR_TIMERS )
{
  SPointIn* dat = fsc->PointIn(p.nBuff);
  SDotValue* V = fsc->DotV(dat->nOut);
  if ( dat->In != NULL )
    {
    ASSD( V->eType == enumValueDbl );
    double* v = (double*)dat->In;
    if ( (V->dwFlags & 1) == 0 )
      {
      V->Set(*v);
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
int SFscList::FuncQuickWatch46( IFscStorage* fsc, LFscBase *obj, IFscStorage::SVarInfo *pvi, int cVI, LPCSTR *ppPntName )
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

int SFscList::SaveState46( IStateSer *psaver, IFscStorage* fsc, LFscBase *obj )
{
  SPointIn* dat = (SPointIn*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->nOut) };
  for( int i=0; i<_countof(vals); ++i )
  {
    psaver->WriteDotV( vals[i], s_pName[i] );
  }
  return 0;
}

int SFscList::RestoreState46( IStateSer *prest, IFscStorage* fsc, LFscBase *obj )
{
  SPointIn* dat = (SPointIn*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->nOut) };
  for( int i=0; i<_countof(vals); ++i )
  {
    prest->ReadDotV( vals[i], s_pName[i] );
  }
  return 0;
}
