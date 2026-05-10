#include "StdAfx.h"
#include "../ARM.h"
#include "../AplicFrm.h"
#include "TagShpGPVSt.h"

CTagShpGPVState::CTagShpGPVState(int n)
  {
  mParam = n;
  }
//
void CTagShpGPVState::Analiz( ANALIZ )
  {
  CTagDIV::Analiz( html, teg );
  }
//
void CTagShpGPVState::Kerdyk( KERDYK )
  {
  Clear();// Удаляем дочерние продукты
  m_numberOfShapesAnimated = -2;
  Shape( html );// Читаем shape-файл
  ASSERT( m_numberOfShapesAnimated == 3 );
  ASSERT( m_useFirstShapeForBadValue == 0 );
  CTag::Kerdyk( hWnd, html );
  CString objName = html.m_strObj;
  if(Version == AM2_NAK) 
	  objName = html.m_strGroup + ".DEVCTLA";
  mVal = FindExperion( html, objName, "PV" );
  if ( AS( mVal.def != NULL ) ) return;
  if ( mVal.def->dwLog == id_FLAG)
    mParam -= 4;
  }
//
void CTagShpGPVState::PaintThis( LOCALS )
  {
  if ( m_numberOfShapesAnimated == -1 )
    return; // Ошибка чтения shape-файла
  //
  CStyle& style = Style(html);
  char* id = style.NameID( html );
  //
  BYTE val = html.pipe.ValueB_H(mVal);
  INT_PTR nCount = GetSize();
  CTag**     obj = GetData();
  if ( val == 0xff )
    {
    if ( nCount == 3 )
      obj[0]->PaintThis( hDC, html );//checked_err
    else
      ASSUNO;
    }
  else
  if ( nCount == 3 )
    {
    int n = (val == mParam) ? 1 : 0;// checked : unchecked
    obj[n]->PaintThis( hDC, html );
    }
  else
    ASSUNO;
  }
//
