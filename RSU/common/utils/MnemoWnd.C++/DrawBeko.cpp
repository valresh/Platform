//#include "MnemoWnd.h"
#include <time.h>
#include "PipeClient.h"
#include "LogFile.h"

#ifdef _WIN32
#define timegm _mkgmtime
#endif

//void DrawEllipse( HDC hDC, RECT& rc, COLORREF color )
//  {
//  HBRUSH hBrush = ::CreateSolidBrush( color );
//  if ( hBrush )
//    {
//    HBRUSH hOldB = (HBRUSH)::SelectObject( hDC, hBrush );
//    HPEN hPen = ::CreatePen( PS_SOLID,0,color);
//    if ( hPen )
//      {
//      HPEN hOldP = (HPEN)::SelectObject( hDC, hPen );
//      ::Ellipse( hDC, rc.left, rc.top, rc.right, rc.bottom );
//      ::SelectObject( hDC, hOldP );
//      ::DeleteObject( hPen );
//      }
//    ::SelectObject( hDC, hOldB );
//    ::DeleteObject( hBrush );
//    }
//  }
////
//void DrawDC( HDC hDC, LPDRAWITEMSTRUCT item )
//  {
//	  CLogFile::LogEx (LOGLEVEL_HIGH, "DrawDC_0");
//  HBRUSH hBrush = ::CreateSolidBrush( 0xCC00FF );
//  DWORD dwThreadID = ::GetCurrentThreadId();
//  if ( hBrush )
//    {
//    ::FillRect( hDC, &item->rcItem, hBrush );
//    ::DeleteObject( hBrush );
//    }
//  ::DrawEdge( hDC, &item->rcItem, EDGE_RAISED, BF_RECT );
//  //
//  //
//  bool isTrue = false;
//  COLORREF color = 0xFF0000;
//  //
//  CPipeClient* pipe = CPipeClient::ms_pMainPipe;
//  CLogFile::LogEx (LOGLEVEL_LOW, "DrawDC_0: pipe %16lx", pipe);
//  if ( pipe )
//  {
//	  KTcpTransport *pTransport = pipe->GetTcpTransport ();
//	  UINT_PTR hSocket = pTransport ? pTransport->GetSocket() : 0;
//
//	  CLogFile::LogEx (LOGLEVEL_HIGH, "DrawDC_1: connected %d, hSocket %16lx, ms_szServer %s, ms_szCompNm %s",
//	  pipe->IsConnected(), hSocket, pipe->ms_szServer, pipe->ms_szCompNm);
//  }
//  if ( pipe && pipe->IsConnected() )
//    {
//    CShSystem& sh = GiveSystem();
//	CLogFile::LogEx (LOGLEVEL_HIGH, "DrawDC_2: sh.StartStatus %d", (int)sh.StartStatus);
//    if ( IsTypeOK )
//    switch ( sh.StartStatus )
//      {
//      case CSystem::STATUS_PAUSE  : color = 0x33FFFF; break;
//      case CSystem::STATUS_START  : color = 0x00FF00; break;
//      default:
//      case CSystem::STATUS_UNKNOWN: color = 0x0000FF; break;
//      }
//    isTrue = IsTrueModel();
//    }
//  //
//  RECT rc = item->rcItem;
//  ::InflateRect( &rc, -5, -5 );
//  ::DrawEllipse( hDC, rc, color );
//  //
//  if ( !isTrue )
//    {
//    ::InflateRect( &rc, -4, -4 );
//    ::DrawEllipse( hDC, rc, 0x0000FF );
//    }
//  //
//  }
////
//// Отрисовка состояния модели
//void DrawBeko( LPDRAWITEMSTRUCT item )
//  {
//  static HBITMAP hBitmap = NULL;
//  static int nWidth  = 0;
//  static int nHeight = 0;
//  int h = item->rcItem.bottom-item->rcItem.top;
//  int w = item->rcItem.right-item->rcItem.left;
//  if ( nWidth < w || nHeight < h )
//    {
//    if ( hBitmap != NULL )
//      ::DeleteObject( hBitmap );
//    hBitmap = NULL;
//    nWidth  = w;
//    nHeight = h;
//    }
//  if ( hBitmap == NULL )
//    hBitmap = ::CreateCompatibleBitmap( item->hDC, w, h );
//  if ( hBitmap == NULL )
//    return;
//  HDC hdc = ::CreateCompatibleDC( item->hDC );
//  if ( hdc != NULL )
//    {
//    HBITMAP hOld = (HBITMAP)::SelectObject( hdc, hBitmap );
//    ::DrawDC( hdc, item );
//    ::BitBlt( item->hDC, item->rcItem.left, item->rcItem.top, w, h, hdc, 0, 0, SRCCOPY );
//    ::SelectObject( hdc, hOld );
//    ::DeleteDC( hdc );
//    }
//  }
////------------------------------------------------------------------------------------------------
//// Функции, которые используют статическую переменную ms_pMainPipe
////
//// Получение модельного времени
CLIENTDLL_API __time64_t GiveModelTime()
{
	auto startTime = managed::NetworkHub.GetStartTime();
	if (managed::NetworkHub.IsConnected() && startTime)
	{
		return startTime + __time64_t(managed::NetworkHub.GetSimulationTime() * 3600);
	}

	auto utcTime = time(NULL);
	return timegm(localtime(&utcTime));
}
//
// Соответствие модели
CLIENTDLL_API bool IsTrueModel()
{
	return managed::NetworkHub.IsConnected();
}
//
// Соответствие работающей модели
CLIENTDLL_API bool IsWorkModel()
{
	return managed::NetworkHub.IsConnected() && managed::NetworkHub.GetStatus() == CSystem::STATUS_START;
}
//
//// Отказ РСУ
//bool IsNoPCY()
//  {
//  CShSystem& sh = GiveSystem();
//  if ( !IsTypeOK ) return false;
//  return IsWorkModel() && sh.bNoPCYWork;
//  }
////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
