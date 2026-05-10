#include "Queue.h"
#include "../Lang.h"
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Структура
struct Queue_Image : public SPROLOZ
  {
  const char* bmp;
  //XFORM x;
  //POINT pt;
  EDirect eDir;
  };
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//void ClearImage(SQueue& q,void* s)
//  {
//  Queue_Image* p = (Queue_Image*)s;
//  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Функция отображения
int PaintImage(DefinePaint)
  {
 // QueueP(Queue_Image);
 // //
	//Graphics gr( hDC );
 // //
 // Matrix matrix(p->x.eM11,p->x.eM12,p->x.eM21,
 //               p->x.eM22,p->x.eDx ,p->x.eDy);
 // //
 // SIZE sz = {p->bmp->GetWidth(),p->bmp->GetHeight()};
 // gr.SetTransform( &matrix );
 // gr.DrawImage( p->bmp, p->pt.x,  p->pt.y, sz.cx, sz.cy );
 // gr.ResetTransform();
 // //
  return 1;
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AddImage(DefineTegs)
  {
  //char szExt  [_MAX_EXT  ];
  //char szPath [_MAX_PATH ];
  //char szFile [_MAX_FNAME];
  //wchar_t wPath[_MAX_PATH];
  ////
  //_splitpath_s( teg[0].value, 0,0, 0,0, 0,0, szExt, _MAX_EXT );
  //lstrcpy( szFile, teg[0].value );
  //if ( !*szExt ) lstrcat( szFile, ".png" );
  //GetPathScn( szPath, szFile, false );
  ////
  //NetworkFile( szPath );
  ////
  //MultiByteToWideChar(CP_ACP, 0, szPath, -1, wPath, _MAX_PATH);
  //Bitmap* bmp = Bitmap::FromFile( wPath );
  //if ( bmp == NULL ) return;
  //
  AddQueueEx(Queue_Image);
  // Код, посылаемый в АРМ
  p->btSendType = QUEUE_NO;
  // Определение функции вывода
  //p->paint = &PaintImage;
  //p->clear = &ClearImage;
  //
  p->bmp = teg[0].value;
  p->eDir = dirUp;
  //
  STegObj obj[] =
    {
    { 'R', "rect" , &p->mRect },
    { 'E', "rc"   , &p->mRect },
    { 'N', "d"     , &p->eDir },
    { 'N', "dir"   , &p->eDir },
    { 'N', "direct", &p->eDir },
    };
  //
  ::ParserObjScn( teg, obj, sizeof(obj)/sizeof(STegObj) );
  //
  //SIZE sz = {p->bmp->GetWidth(),p->bmp->GetHeight()};
  ////
  //int cx = p->W();
  //int cy = p->H();
  //float dScaleX = float(cx)/sz.cx;
  //float dScaleY = float(cy)/sz.cy;
  //p->pt.x = int(p->mRect.left/dScaleX);
  //p->pt.y  = int(p->mRect.top /dScaleY);
  ////
  //switch ( eDir )
  //  {
  //  case dirUp:
  //    p->x.eM11 = dScaleX; p->x.eM12 = 0.;
  //    p->x.eM21 = 0.; p->x.eM22 = dScaleY;
  //    p->x.eDx  = 0;
  //    p->x.eDy  = 0;
  //    break;
  //  case dirRight:
  //    p->x.eM11 = 0.; p->x.eM12 = dScaleX;
  //    p->x.eM21 =-dScaleY; p->x.eM22 = 0.;
  //    p->x.eDx  = REAL(p->mRect.left+p->mRect.bottom);
  //    p->x.eDy  = REAL(p->mRect.top -p->mRect.left  );
  //    p->mRect.right = p->mRect.left+cy;
  //    p->mRect.bottom = p->mRect.top+cx;
  //    break;
  //  case dirLeft:
  //    p->x.eM11 = 0.; p->x.eM12 =-dScaleX;
  //    p->x.eM21 = dScaleY; p->x.eM22 = 0.;
  //    p->x.eDx  = REAL(p->mRect.left-p->mRect.top);
  //    p->x.eDy  = REAL(p->mRect.top+p->mRect.right);
  //    p->mRect.right = p->mRect.left+cy;
  //    p->mRect.bottom = p->mRect.top+cx;
  //  break;
  //  case dirDown:
  //    p->x.eM11 = 1.; p->x.eM12 = 0.;
  //    p->x.eM21 = 0.; p->x.eM22 =-1.;
  //    p->x.eDx  = 0.;
  //    p->x.eDy  = REAL(p->mRect.top+p->mRect.bottom);
  //    break;
  //  case dirFree:
  //    p->x.eM11 =-dScaleX; p->x.eM12 = 0.;
  //    p->x.eM21 = 0.; p->x.eM22 = dScaleY;
  //    p->x.eDx  = REAL(p->mRect.left+p->mRect.right);
  //    p->x.eDy  = 0.;
  //    break;
  //  };
  //
  managed::RegisterElement(teg->name, p);
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
