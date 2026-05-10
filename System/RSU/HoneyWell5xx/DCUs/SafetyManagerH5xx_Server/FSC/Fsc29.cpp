#include "FscList.h"
static char* sid = "Register";

struct SRegister
{
  UINT CU;
  UINT CD;
  UINT L;
  UINT Register;
  UINT C;
  UINT F;
  double value;
};

//
bool SFscList::FuncParser29( VAR_PARSER )
{
  FuncParserdo(fsc,p,dat,tmp);

  ASS( dat.data[0]==1);
  //ASS( dat.data[2]==1 || dat.data[2]==2 );//скорее всего регистров на FLD

  SRegister io;
  memset( &io, -1, sizeof(io) );

  SDotValue out(enumValueInt);
  switch( dat.data[1] )
  {
  case 'L':
    out.eType = enumValueInt;
    //ASS( dat.data[4]==6);
    break;
  case 'F':
    out.eType = enumValueDbl;
    //ASS( dat.data[4]==0);
    break;
  default:
    ASS(0);
  }
  io.F = fsc->AddFscDots(&out);

  p.nBuff = fsc->AddFscBuff( &io, sizeof(io) );

  ns_UT::SLocalTmp* item = tmp.Item();
  item->nCount = 1;
  item->dim[0] = io.F;

  return true;
}
//
//void FuncDebugs29( VAR_DEBUGS )
//  {
//  strcat_s( str, sid );
//  strcat_s( str, "\r\n------------------------------------------------------------\r\n" );
//  FuncDebugsdo( p, str );
//  }
//
#ifdef _WIN32
void SFscList::FuncPaints29( VAR_PAINTS )
{
  COLORREF color = 0x808000;
  if ( p.nBuff != -1 )
  {
    SRegister* dat = (SRegister*)fsc->Data(p.nBuff);
    if ( p.nBuff == nSelect )
      color = SELCOLOR;
  }
  RECT tmp = p.rect;
  ::InflateRect( &tmp, -1, 2 );
  CMyBrush brush( font, BACKGROUND );
  brush.Rect( tmp );
  CMyPen pen( font, color );
  pen.Rect( p.rect );
  RECT rc = p.rect;
  rc.left += 2;
  LPCSTR pInputs[] = {"CU", "CD","L", "Register", "C"};

  int dy = (rc.bottom-rc.top)/_countof(pInputs);
  rc.bottom = rc.top + dy;
  for( int i=0; i<_countof(pInputs); ++i, rc.top+=dy, rc.bottom+=dy )
  {
    font.DrawS( rc, pInputs[i], DT_LEFT );
  }
  rc.top-=dy, rc.bottom-=dy;
  font.DrawS( rc, "L", DT_RIGHT );
}
#endif
//
void SFscList::FuncSetLnk29( IFscStorage* fsc, SFscBase& p, UINT nLink, int x, int y )
{
  if ( nLink == -1 ) 
    return;
  if ( x == p.rect.right ) 
    return;
  //
  SDotValue* dot = fsc->DotV(nLink);

  SRegister* dat = (SRegister*)fsc->Data(p.nBuff);
  UINT *ins[] = { &dat->CU, &dat->CD, &dat->L, &dat->Register, &dat->C };
  int dy = (p.rect.bottom-p.rect.top)/_countof(ins);
  int ym = p.rect.top;
  for( int i=0; i<_countof(ins); ++i, ym+=dy )
  {
    if( y>ym && y<(ym+dy) )
    {
      ASS( -1==*ins[i] || *ins[i] == nLink );
      *ins[i] = nLink;
      break;
    }
  }
}
////-----------------------------------------------------------------------------
//UINT FuncCursor29( VAR_CURSOR )
//  {
//  return p.nBuff;
//  }
////-----------------------------------------------------------------------------
UINT* SFscList::FuncOutput29( VAR_OUTPUT )
{
  n = 1;
  SRegister* dat = (SRegister*)fsc->Data(p.nBuff);
  return &dat->F;
}
////-----------------------------------------------------------------------------
void SFscList::FuncTimers29( VAR_TIMERS )
{
  if( dt == 0 )
    return;
  SRegister* dat = (SRegister*)fsc->Data(p.nBuff);
  SDotValue* cu = fsc->DotV(dat->CU);
  SDotValue* cd = fsc->DotV(dat->CD);
  SDotValue* Cclear = fsc->DotV(dat->C);
  ASSD( !cu && !cd );
  SDotValue* L = fsc->DotV(dat->L);
  SDotValue* O = fsc->DotV(dat->F);
  SDotValue* I = fsc->DotV(dat->Register);
  bool load = false;
  bool clear = false;
  if( L )
  {
    switch( L->eType )
    {
    case enumValueBol:
      load = L->Bol();
      break;
    default:
      ASSD(0);
    }
  }
  if( Cclear )
  {
    switch( Cclear->eType )
    {
    case enumValueBol:
      clear = Cclear->Bol();
      break;
    default:
      ASSD(0);
    }
  }

  if( clear )
  {
    if( O )
    {
      switch( O->eType )
      {
      case enumValueDbl:
        O->dVal = 0;
        break;
      case enumValueInt:
        O->nVal = 0;
        break;
      default:
        ASSD(0);
      }
    }
  }
  else if( load )
  {
    if( I )
    {
      switch( I->eType )
      {
      case enumValueDbl:
        dat->value = I->Dbl();
        break;
      default:
        ASSD(0);
      }
    }
    if( O )
    {
      switch( O->eType )
      {
      case enumValueDbl:
        O->dVal = dat->value;
        break;
      case enumValueInt:
        O->nVal = (int)dat->value;
        break;
      default:
        ASSD(0);
      }
    }
  }
}
////-----------------------------------------------------------------------------
static LPCSTR s_pName[] = { "CU", "CD", "C", "L", "Register", "F" };
int SFscList::FuncQuickWatch29( IFscStorage* fsc, LFscBase *obj, IFscStorage::SVarInfo *pvi, int cVI, LPCSTR *ppPntName )
{
  SRegister* dat = (SRegister*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->CU), fsc->DotV(dat->CD), fsc->DotV(dat->C), fsc->DotV(dat->L), fsc->DotV(dat->Register), fsc->DotV(dat->F) };

  int J = 0;
  for( int n=0; n<_countof(vals) && n<cVI; ++n )
  {
    if( !vals[n] )
      continue;
    pvi[J].val = vals[n];
    strcpy_s( pvi[J].szName, s_pName[n] );
    ++J;
  }
  return J;
}

int SFscList::SaveState29( IStateSer *psaver, IFscStorage* fsc, LFscBase *obj )
{
  SRegister* dat = (SRegister*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->CU), fsc->DotV(dat->CD), fsc->DotV(dat->C), fsc->DotV(dat->L), fsc->DotV(dat->Register), fsc->DotV(dat->F) };
  for( int i=0; i<_countof(vals); ++i )
  {
    psaver->WriteDotV( vals[i], s_pName[i] );
  }
  return 0;
}

int SFscList::RestoreState29( IStateSer *prest, IFscStorage* fsc, LFscBase *obj )
{
  SRegister* dat = (SRegister*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->CU), fsc->DotV(dat->CD), fsc->DotV(dat->C), fsc->DotV(dat->L), fsc->DotV(dat->Register), fsc->DotV(dat->F) };
  for( int i=0; i<_countof(vals); ++i )
  {
    prest->ReadDotV( vals[i], s_pName[i] );
  }
  return 0;
}
