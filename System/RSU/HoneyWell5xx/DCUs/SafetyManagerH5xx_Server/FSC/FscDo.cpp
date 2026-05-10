#include "FscList.h"

// Неизвестные объекты
bool SFscList::FuncParserdo( IFscStorage* fsc, SFscBase& p, SfscAbstruct& dat, SUniTemp& tmp )
{
  ns_UT::SLocalTmp   local;
  local.nCount  = 0;
  local.nDirect = 0;
  memset( local.dim, -1, sizeof(local.dim) );
  UINT N = tmp.AddObj( &local );
  //
  p.rect.SetRect( dat.x, dat.y, dat.x + dat.cx, dat.y + dat.cy );
  memcpy( &p.realSMrect, &p.rect, sizeof(p.realSMrect) );
  p.rect.OffsetRect( 5, 5);
  //
  p.mT = dat.type;
  p.mB = dat.b;// 0,1,3
  p.ValidateB();
  p.ID = dat.id;
#ifdef MY_DEBUG
  p.mU = dat.unk0;
  memcpy( p.szData, dat.data, sizeof(p.szData) );
#endif
  //
  p.nBuff = -1;

  return true;
}

void SFscList::FuncSetLnkdo( IFscStorage* fsc, SFscBase& p, UINT nLink, int x, int y )
{
#pragma message("!!!  возможно надо сделать\n")
  ASS(0);
}

#ifdef _WIN32
void SFscList::FuncPaintsdo( IFscStorage* fsc, SFscBase& p, CMyFont& font, UINT nSelect )
{
  CMyBrush brush( font, BADCOLOR );
  brush.Rect( p.rect );
}
#endif

UINT* SFscList::FuncOutputdo( IFscStorage* fsc, SFscBase& p, int& n )
{
  n = 0;
  return NULL;
}

void SFscList::FuncTimersdo( VAR_TIMERS )
{
}

int SFscList::FuncQuickWatchdo( IFscStorage* fsc, LFscBase *obj, IFscStorage::SVarInfo *pvi, int cVI, LPCSTR *ppPntName )
{
  return 0;
}

int SFscList::SaveStatedo( IStateSer *psaver, IFscStorage* fsc, LFscBase *obj )
{
  return 0;
}

int SFscList::RestoreStatedo( IStateSer *prest, IFscStorage* fsc, LFscBase *obj )
{
  return 0;
}
