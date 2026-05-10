#include "StdAfx.h"
#include "../ARM.h"
#include "../AplicFrm.h"
#include "TagShpPVFLSt.h"

CTagShpPVFLState::CTagShpPVFLState(int n)
  {
  mParam = n;
  }
//
void CTagShpPVFLState::Analiz( ANALIZ )
  {
  CTagDIV::Analiz( html, teg );
  }
//
void CTagShpPVFLState::Kerdyk( KERDYK )
  {
  Clear();// Удаляем дочерние продукты
  m_numberOfShapesAnimated = -2;
  Shape( html );// Читаем shape-файл
  ASSERT( m_numberOfShapesAnimated == 2 );
  ASSERT( m_useFirstShapeForBadValue == 0 );
  CTag::Kerdyk( hWnd, html );
  if (Version == LG35_8_KF && !html.m_strFile.CompareNoCase("sysdtlmos_fp")){
	mVal = FindExperion( html, html.m_strObj, "MOS.PVFL" );
  } else if (Version == LG35_8_KF && !html.m_strFile.CompareNoCase("sysdtlpos_fp")){
	mVal = FindExperion( html, html.m_strObj, "POS.PVFL" );
  } else {
	mVal = FindExperion( html, html.m_strObj, "PVFL" );
  }
  }
//
void CTagShpPVFLState::PaintThis( LOCALS )
  {
  if (Version == LG35_8_KF && !html.m_strFile.CompareNoCase("sysdtlflaga_rr_fp"))
	  return;
  if ( m_numberOfShapesAnimated == -1 )
    return; // Ошибка чтения shape-файла
  //
  CStyle& style = Style(html);
  char* id = style.NameID( html );
  if ( style.LocalD( hDC ) ) return;
  //
  BYTE val = html.pipe.ValueB_H(mVal);
  INT_PTR nCount = GetSize();
  CTag**     obj = GetData();
  if ( val == 0xff )
    {
    }
  else
  if ( nCount == 2 )
    {
    int n = (val == mParam) ? 1 : 0;// checked : unchecked
    obj[n]->PaintThis( hDC, html );
    }
  else
    ASSUNO;
  }
//

void CTagShpPVFLState::Action( CHTML& html, HWND hWnd, bool bDown )
  {
	  BYTE nOld = html.pipe.ValueB_H(mVal);
	  if (nOld != mParam)
	  {
		BYTE nNew=mParam;
		SSendToModel send;
        lstrcpy( send.szValue, mVal.def->name );
        send.Set( enumValueChr, &nOld, &nNew );
        html.pipe.SendData( mVal.nNumber, send, true );
	  }
  }