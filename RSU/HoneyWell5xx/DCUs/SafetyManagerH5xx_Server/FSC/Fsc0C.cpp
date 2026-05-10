#include "FscList.h"
static char* sid = "Analog Input (AI)";
// 
bool SFscList::FuncParser0C( IFscStorage* fsc, SFscBase& p, SfscAbstruct& dat, SUniTemp& tmp )
{
  FuncParserdo(fsc,p,dat,tmp);
  //
  SDotValue dot(enumValueDbl);
  SPointIn io;
  io.nOut = fsc->AddFscDots(&dot);
  char FullName[256];
  strcpy_s ( FullName, (char*)(dat.data+1));
  int L = lstrlen ( FullName );
  while ( L > 0 && FullName[L-1] == ' ' )
    FullName[--L] = 0;
  CFSC_Info::eType PntType;
  io.nDbf = fsc->FindDbfTag( FullName, CFSC_Info::AI, &PntType );
  switch( PntType )
  {
  case CFSC_Info::FLD:
    //ASSD( !p.mB || p.mB == LFscBase::FLD );
    p.mB = LFscBase::FLD;
    break;
  case CFSC_Info::COM:
    ASSD( !p.mB || p.mB == LFscBase::COM );
    p.mB = LFscBase::COM;
    break;
  case CFSC_Info::SYS:
    //ASSD( !p.mB || p.mB == LFscBase::SYS );
    p.mB = LFscBase::SYS;
    break;
  case CFSC_Info::Other:
  case CFSC_Info::CAB:
    ASSD( p.mB == LFscBase::FLD );
    break;
  default:
    ASSD(0);
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
//void FuncDebugs0C( VAR_DEBUGS )
//  {
//  strcat_s( str, sid );
//  strcat_s( str, "\r\n------------------------------------------------------------\r\n" );
//  FuncDebugsdo( p, str );
//  }
//
#ifdef _WIN32
void SFscList::FuncPaints0C( VAR_PAINTS )
{
  SPointIn* dat = fsc->PointIn(p.nBuff);
  SDotValue* V = fsc->DotV(dat->nOut);
  if ( V->dwFlags & 0x2 )
    {
    RECT rcl = p.rect; rcl.right = rcl.left+10;
    CMyBrush brush( font, NOTCONNECTED );
    brush.Rect( rcl );
    }
  RECT rcl = p.rect; rcl.right = (rcl.left*8+rcl.right*27)/35;
  RECT rcr = p.rect; rcr.left = rcr.right-(rcr.bottom-rcr.top);
  int nNumb = dat->nDbf;
  COLORREF color = p.BoxL( fsc, font, nNumb, rcl, NULL, "0-20 mA" );
  {
  CMyPen pn( font, 0x808080 );
  int h = (rcr.bottom+rcr.top)/2;
  pn.Horz( h  , rcl.right,rcr.left );
  pn.Horz( h-2, rcl.right,rcr.left );
  pn.Horz( h+2, rcl.right,rcr.left );
  }
  if ( nSelect == dat->nOut )
    color = SELCOLOR;
  CMyPen pen( font, color );
  pen.Rect( rcr );
  POINT pt0[2] = {{rcr.left,rcr.bottom-2},{rcr.right-2,rcr.top}};
  pen.Points( pt0, 2, 0 );
  POINT pt1[2] = {{rcr.left+2,rcr.bottom},{rcr.right,rcr.top+2}};
  pen.Points( pt1, 2, 0 );
  //
  p.DrawL( font, rcr, "A", 0 );
  p.DrawR( font, rcr, "D", 2 );
  //
}
#endif
//
void SFscList::FuncSetLnk0C( VAR_SETLNK )
{
  // Не используется
}
////-----------------------------------------------------------------------------
//UINT FuncCursor0C( VAR_CURSOR )
//  {
//  SPointIn* dat = fsc->PointIn(p.nBuff);
//  return dat->nOut;
//  }
////-----------------------------------------------------------------------------
UINT* SFscList::FuncOutput0C( VAR_OUTPUT )
{
  SPointIn* dat = fsc->PointIn(p.nBuff);
  n = 1;
  return &dat->nOut;
}
////-----------------------------------------------------------------------------
void SFscList::FuncTimers0C( IFscStorage* fsc, SFscBase& p, double dt )
{
  SPointIn* dat = fsc->PointIn(p.nBuff);
  SDotValue* V = fsc->DotV(dat->nOut);
  if ( dat->In != NULL )
    {
    double* v = (double*)dat->In;
    if ( (V->dwFlags & 1) == 0 )
      {
      const char* a = fsc->DbfName( dat->nDbf, 27 );
      int nNumb = atoi(a);
      double d = *v;
      switch ( nNumb )
        {
        case 63:case 64:case 66:// 125
        case 68:case 69:case 70:// 140
          d = *v * 0.001;
          break;
        }
      V->Set( d );
      /*if( p.pFldCopyTo )
        *(double*)p.pFldCopyTo = d;*/
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
int SFscList::FuncQuickWatch0C( IFscStorage* fsc, LFscBase *obj, IFscStorage::SVarInfo *pvi, int cVI, LPCSTR *ppPntName )
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

int SFscList::SaveState0C( IStateSer *psaver, IFscStorage* fsc, LFscBase *obj )
{
  SPointIn* dat = (SPointIn*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->nOut) };
  for( int i=0; i<_countof(vals); ++i )
  {
    psaver->WriteDotV( vals[i], s_pName[i] );
  }
  return 0;
}

int SFscList::RestoreState0C( IStateSer *prest, IFscStorage* fsc, LFscBase *obj )
{
  SPointIn* dat = (SPointIn*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->nOut) };
  for( int i=0; i<_countof(vals); ++i )
  {
    prest->ReadDotV( vals[i], s_pName[i] );
  }
  return 0;
}
