#include "StdAfx.h"
#include "../ARM.h"
#include "../AplicFrm.h"
#include "TagAlarmTEXT.h"
#include "../Extensions.h"
//
//-------------------------------------------------------------------
CTagAlarmTEXT::CTagAlarmTEXT(int n)
  {
  mParam = n;
  }
//
void CTagAlarmTEXT::Analiz( ANALIZ )
  {
  AnalizTextArea( this, html, teg );
  }
//
void CTagAlarmTEXT::Kerdyk( KERDYK )
  {
  CTag::Kerdyk( hWnd, html );
  //
  CStyle& style = Style(html);
  style.m_eVisible = enumVisible;
  //
  if ( mVal.def )
    {
    if ( lstrcmpi( mVal.def->name, "HIALM.TYPE" ) == 0 )
      {
      mDbl.nNumber = mVal.nNumber;
      mDbl.def = ::NameToValueEx( mVal.def->dwLog, "HIALM.PR" );
      }
    else ASSUNO
    }
  }
//
void CTagAlarmTEXT::Matrix( LOCALS )
  {
  CStyle& style = Style(html);
  char* id = style.NameID( html );
  if ( style.LocalD( hDC ) ) return;
  if ( style.m_eVisible == enumHidden )
    return;
  //
  RECT rect = style.OutsRect();
  if ( rect.left == rect.right ) return;
  if ( rect.top == rect.bottom ) return;
  //
  //if ( style.m_nFillStyle != -1 )
  //  style.Rect( hDC, html, rect );
  //
  BYTE val = html.pipe.ValueB_H( mVal );
  if ( val == 0/*none*/ || val == 0xff/*error*/ )
    return;
  // Определяем текст
  char txt[512] = "";
  const char* szEnum = ::EnumStrH( *mVal.def, val );
  if ( szEnum == NULL || *szEnum == '\0' )
    return;
  lstrcpy( txt, szEnum );
  // Определяем цвет
  style.m_clrText = GetColorPriorityAlarm( html.pipe, mDbl );
  //
  if ( style.m_clrText != 0xff000000 )
  style.TextOut( hDC, style.m_clrText, rect, txt, true );
  //CMyPen pen( hDC, 0xff0000 );
  //pen.Rect( rect );
  }
//
