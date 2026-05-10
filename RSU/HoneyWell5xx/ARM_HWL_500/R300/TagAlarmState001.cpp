#include "StdAfx.h"
#include "../ARM.h"
#include "../AplicFrm.h"
#include "TagAlarmState001.h"

CTagAlarmState001::CTagAlarmState001(int n)
  {
  }
//
void CTagAlarmState001::Analiz( ANALIZ )
  {
  CTagDIV::Analiz( html, teg );
  }
//
void CTagAlarmState001::Kerdyk( KERDYK )
  {
  CTag::Kerdyk( hWnd, html );
  if (Version == LG35_8_KF && (!html.m_strFile.CompareNoCase("sysdtlmos_fp") || !html.m_strFile.CompareNoCase("sysdtlpos_fp")))
	  return;
  // Вынужденная мера, потом надо убрать
  CharMP szExt;
  ::GiveDACA( szExt, html.m_strObj );
  mVal = FindExperion( html, szExt, "NAME" );
  }
//
void CTagAlarmState001::Matrix( LOCALS )
  {
  CStyle& style = Style(html);
  char* id = style.NameID( html );
  if ( style.LocalD( hDC ) ) return;
  //
  CAlarmBase* al = html.pipe.Alarm( mVal.nNumber );
  LPCTSTR name = html.pipe.GetMnemoName(mVal.nNumber);
  if ( al == NULL ) return;
  //
  RECT rect = style.FullRect();
  //
  SIZE sz = { 16, 16 };// Заведомо знаем размер картинки
  int w = ((rect.right-rect.left)-sz.cx)/2;
  int h = ((rect.bottom-rect.top)-sz.cy)/2;
  POINT pt = { rect.left+w, rect.top+h };
  //
  BYTE btKvit = al->notACK ? 0 : 1;
  // Для квитирования нужны права
  bool  bUnit = html.pipe.IsTrueUnit( mVal.nNumber );
  if ( !bUnit ) btKvit = 1;
  //
  BYTE btOnOf = 1;// В отличии от сигнализаций, здесь предистория неизвестна
  //
  theApp.DrawAlarmIcon( hDC, pt, NULL, al->btEHLN, btKvit, btOnOf,al->Shelved );
  //
  }
//
