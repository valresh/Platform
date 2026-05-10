#include "StdAfx.h"
#include "../ARM.h"
#include "../AplicFrm.h"
#include "TagShpGOPSt.h"

CTagShpGOPState::CTagShpGOPState(int n)
  {
  mParam = n;
  }
//
void CTagShpGOPState::Analiz( ANALIZ )
  {
  CTagDIV::Analiz( html, teg );
  }
//
void CTagShpGOPState::Kerdyk( KERDYK )
  {
  Clear();// Удаляем дочерние продукты
  m_numberOfShapesAnimated = -2;
  Shape( html );// Читаем shape-файл
  ASSERT( m_numberOfShapesAnimated == 2 );
  ASSERT( m_useFirstShapeForBadValue == 0 );
  CTag::Kerdyk( hWnd, html );
  CString objName = html.m_strObj;
  if(Version == AM2_NAK)
	  objName = html.m_strGroup + ".DEVCTLA";
  mVal = FindExperion( html, objName, "OP" );
  }
//
void CTagShpGOPState::PaintThis( LOCALS )
  {
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

void CTagShpGOPState::Action( CHTML& html, HWND hWnd, bool bDown, CPoint * pt )
  {
	  if (Version == LG35_8_KF && html.m_strFile == "sysdtlDEVCTLA_fp"){
		  char oi0 = html.pipe.ValueB_H(FindExperion(html, html.m_strGroup, "DEVCTLA.OI[0]"));
		  char oi1 = html.pipe.ValueB_H(FindExperion(html, html.m_strGroup, "DEVCTLA.OI[1]"));
		  char oi2 = html.pipe.ValueB_H(FindExperion(html, html.m_strGroup, "DEVCTLA.OI[2]"));
		  char pi0 = html.pipe.ValueB_H(FindExperion(html, html.m_strGroup, "DEVCTLA.PI[0]"));
		  char pi1 = html.pipe.ValueB_H(FindExperion(html, html.m_strGroup, "DEVCTLA.PI[1]"));
		  char pi2 = html.pipe.ValueB_H(FindExperion(html, html.m_strGroup, "DEVCTLA.PI[2]"));
		  if (oi0 == 1 || oi1 == 1 || oi2 == 1) {
			  ::MessageBox(hWnd, "Interlock Error", "Interlock Error", MB_OK);
			  return;
		  }
		  //if (pi0 == 0 || pi1 == 0 || pi2 == 0) return;
		  //MIHAIL отключили блокировку по PI
		  //if (pi0 == 0 && mParam == 4) return;
		  //else if (pi1 == 0 && mParam == 5) return;
		  //else if (pi2 == 0 && mParam == 6) return;
	  }

      if (Version != LG35_8_KF)
      {
      SParamValueH val = FindExperion(html, html.m_strGroup, "DEVCTLA.MODEATTR");
      if(val.def)
        { 
         if(val.def->eVal == enumValueChr)
           {
           char MDAttr = html.pipe.ValueB_H(val);
           if( MDAttr == 2) //PROGRAMM ?
             {
               ::MessageBox(hWnd, "Invalid Mode Attribute", "", MB_OK);
               return;
             }
           }
        }
      }

	  BYTE nOld = html.pipe.ValueB_H(mVal);
	  if ((nOld != mParam) &&  mVal.def)
	  {
		BYTE nNew=mParam;
		SSendToModel send;
        lstrcpy( send.szValue, mVal.def->name );
        send.Set( enumValueChr, &nOld, &nNew );
        html.pipe.SendDataEx( mVal.nNumber, send, true );
	  }
  }