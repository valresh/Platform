//#include "StdAfx.h"
//#include "../ARM.h"
#include "../AplicFrm.h"
#include "TagSCRIPT.h"
#include "TagINPUT.h"
#include "TagSPAN.h"

#include "Style.h"

//custom includes

#include <avaloniaWrappers/AfxControls.h>
#include <avaloniaWrappers/Gdiplus.h>

CTagINPUT::CTagINPUT(void)
: m_eInputType(inputButton)
, m_nPos(-1)
  {
  memset( m_szPopup, 0, sizeof(m_szPopup) );
  m_ptPopup.x = -1;
  m_ptPopup.y = -1;
  }
//
#undef  STD_VAR
#define STD_VAR CHTML& html, CTagINPUT& a, CStyle& style, char* value
typedef void (*LPFuncs)( STD_VAR );
//
void inputClass( STD_VAR )
  {
    if(strstr(value, "hsc.popup."))
      a.m_ClickType =  ctPopup;
  }
//
void inputValue( STD_VAR )
  {
  ASSERT( lstrlen(value) < sizeof(a.m_szScriptValue) );
  AnalizText( value );
  lstrcpy( a.m_szScriptValue, value );
  }
//
void inputTabIndex( STD_VAR )
  {
  //ASSERT( ::IsInt(value) );
  //style.nTabIndex = atoi(value);
  }
//
void inputHdxProperties( STD_VAR )
  {
  a.AnalizHdx( html, value );
  }
//
void inputType( STD_VAR )
  {
  if ( lstrcmpi( value, "button" ) == 0 )
    {
    a.m_eInputType = inputButton;
    style.AddFlag(TYPE_PUSHBUTTON);
    }
  else
  if ( lstrcmpi( value, "text" ) == 0 )
    a.m_eInputType = inputText;
  else
  if ( lstrcmpi( value, "checkbox" ) == 0 )
  {
    a.m_eInputType = inputCheckBox;
    style.AddFlag(TYPE_CHECKBOX);
  }
  else
    {
    ASSERT( 0 );
    }
  }
//
void inputTitle( STD_VAR )
  {
   strcpy_s(style.m_szTitle, value);
  }
//
void inputStyle           ( STD_VAR ){a.AnalizStyle  (html,value);}
void inputID              ( STD_VAR )
{
 style.NameID(html,value);
#ifdef _DEBUG
 if(_stricmp(value, "shape008_ScPbLeft")==0)
    AAA();
#endif 
}
void inputPOPUPDISPLAYFILE( STD_VAR ){style.PopupName(html,value);}
void inputPOPUPLEFT       ( STD_VAR ){style.PopupPosX(html,value);}
void inputPOPUPTOP        ( STD_VAR ){style.PopupPosY(html,value);}
void inputPOSITIONING     ( STD_VAR ){style.PopupPosY(html,value);}
void inputOnDataChange    ( STD_VAR ){style.DatChange(html,value);}
//
void inputSHORTCUTMENUCUSTOM( STD_VAR )
  {
  }
//
void inputSHORTCUTMENUFILE( STD_VAR )
  {
  }
//
void inputGlobalscripts( STD_VAR )
  {
  }
//
void inputShapesrc( STD_VAR )
  {
  }

void inputHDX_LOCK( STD_VAR )
  {
  ASSERT( lstrcmp( value, "-1" ) == 0 );
  }
//
void inputOnKeyUp         ( STD_VAR ){/*Встречается в sysAlarmSummary*/}
void inputOnPropertyChange( STD_VAR ){/*Встречается в sysAlarmSummary*/}
void inputMaxLength       ( STD_VAR ){/*Встречается в sysAlarmSummary*/}
void inputChecked         ( STD_VAR ){/*Встречается в sysAlarmSummary*/}
void inputDisabled        ( STD_VAR ){/*Встречается в sysAlarmSummary*/}
void inputOnMouseLeave    ( STD_VAR ){}

void inputOnClick( STD_VAR )
{
  if(Version== LG35_8_KF)
    return;

  if(strlen(value)>2)
    {
    char* pValue = value;
    if(value[0]=='\'')
       pValue++;

      {
        pValue++;
        size_t len= strlen(pValue);
        if(pValue[len-1]=='\'')
           pValue[len-1] = 0;
        a.m_ScriptTextId = style.AddTexts( html, pValue );
        a.m_ScriptTexteType = eventOnClick;
      }
    }   
}
//
void CTagINPUT::Analiz( ANALIZ )
  {
  struct SLocal {  char* name; LPFuncs func; };
  _static SLocal list[] =
    {
    { "class"             , inputClass             },
    { "style"             , inputStyle             },
    { "value"             , inputValue             },
    { "id"                , inputID                },
    { "type"              , inputType              },
    { "title"             , inputTitle             },
    { "tabIndex"          , inputTabIndex          },
    { "hdxproperties"     , inputHdxProperties     },
    { "POPUPDISPLAYFILE"  , inputPOPUPDISPLAYFILE  },
    { "POPUPLEFT"         , inputPOPUPLEFT         },
    { "POPUPTOP"          , inputPOPUPTOP          },
    { "POSITIONING"       , inputPOSITIONING       },
    { "SHORTCUTMENUCUSTOM", inputSHORTCUTMENUCUSTOM},
    { "SHORTCUTMENUFILE"  , inputSHORTCUTMENUFILE  },
    { "globalscripts"     , inputGlobalscripts     },
    { "shapesrc"          , inputShapesrc          },
    { "ondatachange"      , inputOnDataChange      },
    { "onkeyup"           , inputOnKeyUp           },
    { "onpropertychange"  , inputOnPropertyChange  },
    { "maxLength"         , inputMaxLength         },
    { "checked"           , inputChecked           },
    { "disabled"          , inputDisabled          },
    { "HDX_LOCK"          , inputHDX_LOCK          },
    { "onmouseleave"      , inputOnMouseLeave      },
    { "onclick"           , inputOnClick           },
    }InitLocal;
  //
  CStyle& style = Style(html);
  int n = 0;
  while ( teg[n].name )
    {
    SLocal* find = FindLocal( teg[n].name );
    ASSERT( find );
    if ( find != NULL )
    (*find->func)(html,*this,style,teg[n].value);
    n++;
    }
  
  if (Version == LG35_8_KF){
	  char * id = style.NameID(html);
	  if (strstr(id, "_ScPbRight")){
		  style.m_nBkgImage = style.AddTexts( html, "./BL01_1_files/all_navtitle_lib_01_files/right.gif" );
	  } else if (strstr(id, "_ScPbLeft")){
		  style.m_nBkgImage = style.AddTexts( html, "./BL01_1_files/all_navtitle_lib_01_files/left.gif" );
	  } else if (strstr(id, "_ScPbUp")){
		  style.m_nBkgImage = style.AddTexts( html, "./BL01_1_files/all_navtitle_lib_01_files/up.gif" );
	  } else if (strstr(id, "_ScPbDown")){
		  style.m_nBkgImage = style.AddTexts( html, "./BL01_1_files/all_navtitle_lib_01_files/down.gif" );
	  }
	  style.Queue(html, this);
	  style.m_dwTypeObj = TYPE_BINDING | TYPE_PUSHBUTTON;
  }
  else
    {
    style = Style(html);
    std::string imgname = style.GetCSSStyleImage();
    if(imgname.size()>0)
      {
        if ( LoadImages( imgname.c_str(), mImg ) )
         {
         if ( html.m_bBinData ) 
             return;
         //style.m_dwTypeObj |= TYPE_IMAGE;
         }
      }
    }
  //
  if ( !(style.m_dwFlagObj & STYLE_PAD_L) ) style.mL.padding = 3;
  if ( !(style.m_dwFlagObj & STYLE_PAD_R) ) style.mR.padding = 3;
  if ( !(style.m_dwFlagObj & STYLE_PAD_T) ) style.mT.padding = 3;
  if ( !(style.m_dwFlagObj & STYLE_PAD_B) ) style.mB.padding = 3;
  style.m_dwFlags |= HDX_PRESENT;

  if ( !(style.m_dwFlags & HDX_ALIGN) )
    style.align |= DT_CENTER;

  char* id = style.NameID( html );
  if ( lstrcmp( id, "PBCover" ) == 0 )
    style.mRect.Width = 0, style.mRect.Height = 0;
  else
  if ( lstrcmp( id, "pshName" ) == 0 || lstrcmp( id, "shape003_pshName" ) == 0
  ||   lstrcmp( id, "pshEU"   ) == 0 || lstrcmp( id, "shape003_pshEU"   ) == 0
  ||   lstrcmp( id, "pshHide" ) == 0 || lstrcmp( id, "shape003_pshHide" ) == 0 )
    {
    style.m_clrFill = 0xced3d6;
    }
  else
  if ( !(style.m_dwFlags & HDX_CLRFILL) )
    style.m_clrFill = style.colorBkg;
  //
  if ( !::IsMnemoStyle(MNEMO_EDIT) )
    {
    if ( lstrcmpi( html.m_strFile, "100\\d120" ) == 0 )
    if ( lstrcmpi( m_szScriptValue, "Сброс" ) == 0 )
      style.m_eVisible = enumHidden;
    if ( lstrcmpi( html.m_strFile, "200\\d231" ) == 0 )
    if ( lstrcmpi( m_szScriptValue, "Сброс" ) == 0 )
      style.m_eVisible = enumHidden;
    //if ( lstrcmpi( html.m_strFile, "200\\d206" ) == 0 )
    //if ( lstrcmpi( m_szScriptValue, "Возврат" ) == 0 )
    //  style.m_eVisible = enumHidden;
    //if ( lstrcmpi( html.m_strFile, "200\\d224" ) == 0 )
    //if ( lstrcmpi( m_szScriptValue, "Возврат" ) == 0 )
    //  style.m_eVisible = enumHidden;
    }
  //
  // сохраняем в span'е ссылку на вложенный checkbox, чтобы можно было обратиться к checkbox'у из span'а
  if ( m_eInputType == inputCheckBox )
    {
	    CTagSPAN *parent = (CTagSPAN *)this->m_pPar;
	    parent->m_pCheckBoxInput = this;
    }
  }
//
/*virtual*/ bool CTagINPUT::Close()
  {
  return m_eInputType != inputButton;
  }
//
void CTagINPUT::Matrix( LOCALS )
  {
  CStyle& style = Style(html);
  char* id = style.NameID( html );
  if ( style.LocalD( hDC ) ) return;
#ifdef _DEBUG
  if (_stricmp(id, "pushbutton011")==0)
	  AAA();
#endif  
  // для чекбокса необходимо выполнить запрос данных, даже если он скрыт, поэтому в этом месте выходить нельзя
  if ( m_eInputType != inputCheckBox && style.m_eVisible == enumHidden )
    return;
  cross::cross::RECT cross::RECT = style.Fullcross::RECT();
  //
  style.GetCSSStyle();
  //  if ( style.GetFlag( TYPE_PUSHBUTTON ) )
  if ( m_eInputType == inputButton )
    {
    int nParam1 = 0;
    SBaseDSD* dsd = Dsd(html);
    if ( dsd != NULL )
      nParam1 = dsd->GetInt( html, "TaskParam1" );
    bool x = nParam1 == 7 || nParam1 == 61;
	bool bDrawButton=!x;
    bool bOtherButton = false;
    bool bDrawEdge = true;
    DWORD dwClrLine = style.m_clrLine;
	if (style.colorBkg==0xffffffff)
      {       
		bDrawButton=false;
        if ((Version == LG35_8_KF) && (html.m_strFile.CompareNoCase("M01")==0) && (strcmp(id, "pushbutton005")==0))
           bDrawButton = true;
        else
        if(Version == DOTF_PM)
         {
          if (!x && _strnicmp(id, "shape", sizeof("shape")-1)==0)
           {
           bDrawButton = true;
           bOtherButton = true;
           }
          else if ( dsd != NULL )
          {
             const char* szParam2 = dsd->GetStr( html, "TaskParam2" );
             if (szParam2 && *szParam2 && !isdigit(*szParam2) )
             {
              bDrawButton = true;
              bDrawEdge = false;
              if(style.m_bNoLineColor)
                 bDrawButton = false;
              else if(dwClrLine==0)
                      dwClrLine = RGB(1,1,1);
             }
             else KKK();
          }
        }
        else
        if(Version == T1165P_ONPZ)
        {
          if(style.mL.eStyle == eBorderOutset)
            bDrawButton = true;
        }       
      }
	if (Version == LG35_8_KF && !bDrawButton )
		if (strstr(id, "_PbVAR") || strstr(id, "_PbSTOP") || strstr(id, "_PbREV") || strstr(id, "_PbBP"))
			bDrawButton = true;
    if ( bDrawButton )
    {  
       if(Version == BERTH_ONPZ)
          bOtherButton = true;

       if(bOtherButton)
        {            
            int Width = style.mL.width;
            if (Width<1) Width = 1;
            if ( style.m_clrLine != -1 )
               {
               cross::cross::RECT cross::RECT3(cross::RECT);
               ::Inflatecross::RECT( &cross::RECT3, -1, -1 );
               CMyPen pen( hDC, style.m_clrLine, Width);
               pen.cross::RECT( cross::RECT3 );
               }
            cross::RECT cross::RECT2(cross::RECT);
            ::Inflatecross::RECT( &cross::RECT2, -Width+1, -Width+1 );
            if ( m_bPressed  )
              ::DrawEdge( hDC, &cross::RECT2, BDR_SUNKEN, BF_cross::RECT|BF_SOFT );
            else
              ::DrawEdge( hDC, &cross::RECT2, BDR_RAISED, BF_cross::RECT );
            ::Inflatecross::RECT( &cross::RECT2, -2, -2 );
            if ( m_bPressed ) 
               ::Offsetcross::RECT( &cross::RECT2, 1, 1 );
            if ( style.m_clrFill != -1 )
               {
               CMyBrush brush( hDC, style.m_clrFill );
               brush.cross::RECT( cross::RECT2 );
               }

            cross::RECT = style.Outscross::RECT();
            if ( m_bPressed ) 
               ::Offsetcross::RECT( &cross::RECT, 1, 1 );
            bDrawButton =false;
        }
       else
       if(bDrawEdge)         
          style.Button( hDC, cross::RECT, m_bPressed );
    }
    if(!bOtherButton)
    cross::RECT = style.Outscross::RECT();
    if ( bDrawButton )
    if ( m_bPressed ) 
        ::Offsetcross::RECT( &cross::RECT, 1, 1 );
	if (!bDrawButton) 
    {
      if(Version >= T1163_ONPZ)
      {
        if(style.mL.eStyle == eBorderRidge)
        {   
            int Width = style.mL.width;
            if (Width<1) Width = 1;
            {
            CMyPen penc( hDC, style.mL.color, Width);
            penc.Vert(cross::RECT.left, cross::RECT.top, cross::RECT.bottom);
            }
            {
            CMyPen peni( hDC, ~style.mL.color & 0x00ffffff, Width);
            peni.Vert(cross::RECT.left + Width, cross::RECT.top, cross::RECT.bottom);
            }
        }
        if(style.mR.eStyle == eBorderRidge)
        {
            int Width = style.mR.width;
            if (Width<1) Width = 1;
            {
            CMyPen penc( hDC, style.mR.color, Width);
            penc.Vert(cross::RECT.right, cross::RECT.top, cross::RECT.bottom);
            }
            {
            CMyPen peni( hDC, ~style.mR.color & 0x00ffffff, Width);
            peni.Vert(cross::RECT.right - Width, cross::RECT.top, cross::RECT.bottom);
            }
        }
        if(style.mT.eStyle == eBorderRidge)
        {   
            int Width = style.mT.width;
            if (Width<1) Width = 1;
            {
            CMyPen penc( hDC, style.mT.color, Width);
            penc.Horz(cross::RECT.top, cross::RECT.left, cross::RECT.right);
            }
            {
            CMyPen peni( hDC, ~style.mT.color & 0x00ffffff, Width);
            peni.Horz(cross::RECT.top + Width, cross::RECT.left, cross::RECT.right);
            }
        }
        if(style.mB.eStyle == eBorderRidge)
        {   
            int Width = style.mB.width;
            if (Width<1) Width = 1;
            {
            CMyPen penc( hDC, style.mB.color, Width);
            penc.Horz(cross::RECT.bottom, cross::RECT.left, cross::RECT.right);
            }
            {
            CMyPen peni( hDC, ~style.mB.color & 0x00ffffff, Width);
            peni.Horz(cross::RECT.bottom - Width, cross::RECT.left, cross::RECT.right);
            }
        }
      }
    }
	else {
	 Graphics graphics( hDC );
     graphics.SetSmoothingMode(SmoothingModeHighQuality);
     graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
     graphics.SetPixelOffsetMode(PixelOffsetModeHighQuality);

	 Gdiplus::Color stdBorderColor;
     stdBorderColor.SetFromCOLORREF( 0x00c2c2c2 );
	 Gdiplus::Pen stdBorderPen( stdBorderColor, 1);
	 Gdiplus::cross::RECT r(cross::RECT.left, cross::RECT.top, abs(cross::RECT.right-cross::RECT.left), abs(cross::RECT.bottom-cross::RECT.top));
	 graphics.Drawcross::RECTangle(&stdBorderPen, r);

	 if (dwClrLine)
	 {
		 LONG brdWidth = (LONG)style.m_nBorderWidth;
		 if (brdWidth<1) brdWidth = 1;
         cross::Gdiplus::Color borderColor;
		 borderColor.SetFromCOLORREF( dwClrLine );
		 Gdiplus::Pen borderPen( borderColor, (REAL)brdWidth);
		 Gdiplus::cross::RECT r2(
			 cross::RECT.left-brdWidth,
			 cross::RECT.top-brdWidth,
			 abs(cross::RECT.right-cross::RECT.left)+2*brdWidth,
			 abs(cross::RECT.bottom-cross::RECT.top)+2*brdWidth);
		 graphics.Drawcross::RECTangle(&borderPen, r2);
	 }
	}
    }
  else
  if ( m_eInputType == inputCheckBox )
    {
	CStyle& style = html.Style(this->m_pPar->m_nStyle);
    ::Inflatecross::RECT( &cross::RECT, -2, -2 );
    DWORD dwFlag = DFCS_BUTTONCHECK;
    //
    void* val = html.pipe.ParamValue_H(mVal);
    if ( val != NULL )
      {
      switch ( mVal.def->eVal )
        {
        case enumValueBol:
        case enumValueChr:
          if ( *(char*)val > 0 )
			{
				*((double*)this->m_pPar->m_szScriptValue) = 1;
				if (!style.m_inverseLogic) dwFlag |= DFCS_CHECKED;
				else dwFlag &= ~DFCS_CHECKED;
			}
			else
			{
				*((double*)this->m_pPar->m_szScriptValue) = 0;
				if (!style.m_inverseLogic) dwFlag &= ~DFCS_CHECKED;
				else dwFlag |= DFCS_CHECKED;
			}
        break;
		case enumValueStr:
             if ((Version!=LG35_8_KF) && (mVal.def->dwLog == id_FLAG))
               {
                 CShFLAG sh;
			     if ( html.pipe.GetData( mVal.nNumber, &sh ) )
                 {
			        if(_countof(sh.STATETEXT)>=2 &&_stricmp(sh.STATETEXT[0], (char*)val )==0)
                       val = (void*)"off";
                    else
                       val = (void*)"on";
                 }
               }
			if (!lstrcmpi((char*)val, "on"))
			{
				*((double*)this->m_pPar->m_szScriptValue) = 1;
				if (!style.m_inverseLogic) dwFlag |= DFCS_CHECKED;
				else dwFlag &= ~DFCS_CHECKED;
			}
			else if (!lstrcmpi((char*)val, "off"))
			{
				*((double*)this->m_pPar->m_szScriptValue) = 0;
				if (!style.m_inverseLogic) dwFlag &= ~DFCS_CHECKED;
				else dwFlag |= DFCS_CHECKED;
			}
			else{}
		break;
       default:
         ASSUNO
        };
      }
    else
    if(m_pPar != NULL && typeid(*m_pPar) == typeid(CTagSPAN))
       {
         if(strcmp(m_pPar->m_szScriptValue, "0") == 0)
            dwFlag &= ~DFCS_CHECKED;
         else if(strcmp(m_pPar->m_szScriptValue, "1") == 0)
         {
           dwFlag |= DFCS_CHECKED;
           style.m_eVisible = m_pPar->Style(html).m_eVisible;
         }
        }
    else
      {
        dwFlag |= DFCS_INACTIVE;
      }
    //
    if (style.m_eVisible == enumHidden) return;
	::DrawFrameControl( hDC, &cross::RECT, DFC_BUTTON, dwFlag );
    }
  else
  if ( m_eInputType == inputText )
    {
    }
  else
    ASSUNO;
  //
  switch ( TYPE_PNT )
    {
    case TYPE_IMAGE:
      OutImage( hDC, mImg, cross::RECT );
    break;
    default:
      if ( mImg.Yes() )
      OutImage( hDC, mImg, cross::RECT );
    };
  if ((Version == UKR_PRP && html.m_strFile == "screen2" && !*id)
      || (Version >= DOTF_PM && m_eInputType == inputCheckBox) 
      )
      return; // чтобы не выводило текст "on" на чекбоксе
  style.TextOut( hDC, style.m_clrText, cross::RECT, m_szScriptValue, false );
  //
  }
/*
вызывается при изменении свойства Value чекбокса из скрипта
*/
void CTagINPUT::CheckboxCallbackFromScript(CHTML& html, int v)
{
  if ( m_eInputType == inputCheckBox )
  {
	  void* val = html.pipe.ParamValue_H(mVal);
	  if ( val != NULL && mVal.def->eVal == enumValueChr )
	  {
		SSendToModel send;
		BYTE bNew, bOld = *(char*)val;
		bNew = v;
		*((double*)this->m_pPar->m_szScriptValue) = (v)?1:0;
		send.Set( enumValueChr, &bOld, &bNew );
		lstrcpy( send.szValue, mVal.def->name );
		html.pipe.SendData( mVal.nNumber, send);
	  }
	  else if ( val != NULL && mVal.def->eVal == enumValueStr )
	  {
		SSendToModel send;
		char *bNew, *bOld = (char*)val;
		if (v) bNew = "ON";
		else bNew = "OFF";
		*((double*)this->m_pPar->m_szScriptValue) = (v)?1:0;
		send.Set( enumValueStr, bOld, bNew );
		lstrcpy( send.szValue, mVal.def->name );
		html.pipe.SendData( mVal.nNumber, send);
	  }
	  return;
  }
}
bool CTagINPUT::ActionLG35_8_KF(CHTML& html, HWND hWnd, bool bDown)
{
  CStyle& style = Style(html);
  char* id = style.NameID( html );

  if (html.m_strFile == "M01")
  {
	  if (!strcmp(id, "pushbutton001")) // Выбор скорости
	  {
/*
Dim SWText

If me.DataValue ("CK1_SPEED.FLAGA.PVFL") = 0 then
	SWText = "с Низкой на Высокую"
else
	SWText = "с Высокой на Низкую"
end if

MsgBox "ВНИМАНИЕ!!! ДАЛЬНЕЙШИЕ ДЕЙСТВИЯ ПРИВЕДУТ К ИЗМЕНЕНИЮ СКОРОСТИ КОМПРЕССОРА!", vbOKOnly, "ВНИМАНИЕ !!! !!! !!!"
If me.DataValue("CK1_SPEED.FLAGA.PVFL") = 0 Then
	If MsgBox("Для переключения скорости " & SWText & " нажмите кнопку <Yes>. Для отмены нажмите кнопку <No>", vbYesNo or vbDefaultButton2, "УПРАВЛЕНИЕ СКОРОСТЬЮ КОМПРЕССОРА")= vbYes Then	
		me.DataValue("CK1_SPEED.FLAGA.PVFL") = 1
	End If
else
	If MsgBox("Для переключения скорости " & SWText & " нажмите кнопку <Yes>. Для отмены нажмите кнопку <No>", vbYesNo or vbDefaultButton2, "УПРАВЛЕНИЕ СКОРОСТЬЮ КОМПРЕССОРА")= vbYes Then
		me.DataValue("CK1_SPEED.FLAGA.PVFL") = 0 
	End if
End if
*/
		  SParamValueH v = FindExperion( html, "CK1_SPEED", "FLAGA.PVFL" );
		  void* val = html.pipe.ParamValue_H(v);
		  if (!val || v.def->eVal != 3)
		  {
			  //__debugbreak();
			  return true;
		  }
		  char old = *(char*)val;
		  char _new;

		  ::MessageBox(hWnd, "ВНИМАНИЕ!!! ДАЛЬНЕЙШИЕ ДЕЙСТВИЯ ПРИВЕДУТ К ИЗМЕНЕНИЮ СКОРОСТИ КОМПРЕССОРА!", "ВНИМАНИЕ !!! !!! !!!", MB_OK);

		  if (old == 0)
		  {
			  if (IDYES == ::MessageBox(hWnd, "Для переключения скорости с Низкой на Высокую нажмите кнопку <Yes>. Для отмены нажмите кнопку <No>", "УПРАВЛЕНИЕ СКОРОСТЬЮ КОМПРЕССОРА", MB_YESNO))
			  {
				  _new = 1;
				  SSendToModel send(v.def);
				  send.Set(v.def->eVal, &old, &_new);
				  html.pipe.SendData(v.nNumber, send);
			  }
		  }
		  else
		  {
			  if (IDYES == ::MessageBox(hWnd, "Для переключения скорости с Высокой на Низкую нажмите кнопку <Yes>. Для отмены нажмите кнопку <No>", "УПРАВЛЕНИЕ СКОРОСТЬЮ КОМПРЕССОРА", MB_YESNO))
			  {
				  _new = 0;
				  SSendToModel send(v.def);
				  send.Set(v.def->eVal, &old, &_new);
				  html.pipe.SendData(v.nNumber, send);
			  }
		  }
		  return true;
	  }
	  else if (!strcmp(id, "pushbutton004")) // Останов ЦК-1
	  {
/*
MsgBox "ВНИМАНИЕ!!! ДАЛЬНЕЙШИЕ ДЕЙСТВИЯ ПРИВЕДУТ К ОСТАНОВУ КОМПРЕССОРА ЦК-1!", vbOKOnly, "ВНИМАНИЕ !!! !!! !!!"
If me.DataValue("CK1_AV_STOP.FLAGA.PVFL") = 0 Then
	If MsgBox("Для останова компрессора ЦК-1 нажмите кнопку <Yes>. Для отмены нажмите кнопку <No>", vbYesNo or vbDefaultButton2, "ОСТАНОВ КОМПРЕССОРА")= vbYes Then	
		me.DataValue("CK1_AV_STOP.FLAGA.PVFL") = 1
	End If
End if
*/
		  SParamValueH v = FindExperion( html, "CK1_AV_STOP", "FLAGA.PVFL" );
		  CShBase* sh = html.pipe.Sh( v.nNumber );
          if ( !IsTypeOk  )
			html.pipe.End();
		  void* val = html.pipe.ParamValue_H(v);
		  if (!val || v.def->eVal != 3)
		  {
			  //__debugbreak();
			  return true;
		  }
		  char old = *(char*)val;
		  char _new;

		  ::MessageBox(hWnd, "ВНИМАНИЕ!!! ДАЛЬНЕЙШИЕ ДЕЙСТВИЯ ПРИВЕДУТ К ОСТАНОВУ КОМПРЕССОРА ЦК-1!", "ВНИМАНИЕ !!! !!! !!!", MB_OK);
		  
		  if (old == 0)
		  {
			  if (IDYES == ::MessageBox(hWnd, "Для останова компрессора ЦК-1 нажмите кнопку <Yes>. Для отмены нажмите кнопку <No>", "ОСТАНОВ КОМПРЕССОРА", MB_YESNO))
			  {
				  _new = 1;
				  SSendToModel send(v.def);
				  send.Set(v.def->eVal, &old, &_new);
				  html.pipe.SendData(v.nNumber, send);
			  }
		  }
		  return true;
	  }
	  else if (!strcmp(id, "pushbutton016")) // Регенерация ЦК-1
	  {
/*
MsgBox "ВНИМАНИЕ!!! ДАЛЬНЕЙШИЕ ДЕЙСТВИЯ ПРИВЕДУТ К ИЗМЕНЕНИ РЕЖИМА РАБОТЫ КОМПРЕССОРА ЦК-1!", vbOKOnly, "ВНИМАНИЕ !!! !!! !!!"
		If me.DataValue("CK1_REG.FLAGA.PVFL") = 0 Then
			If MsgBox("Для переключения компрессора ЦК-1 в режим 'РЕГЕНЕРАЦИЯ' нажмите кнопку <Yes>. Для отмены нажмите кнопку <No>", vbYesNo or vbDefaultButton2, "ИЗМЕНЕНИЕ РЕЖИМА РАБОТЫ КОМПРЕССОРА")= vbYes Then	
				me.DataValue("CK1_REG.FLAGA.PVFL") = 1
			End If
		else If me.DataValue("CK1_REG.FLAGA.PVFL") = 1 Then
					If MsgBox("Для переключения компрессора ЦК-1 в режим 'РЕАКЦИЯ' нажмите кнопку <Yes>. Для отмены нажмите кнопку <No>", vbYesNo or vbDefaultButton2, "ИЗМЕНЕНИЕ РЕЖИМА РАБОТЫ КОМПРЕССОРА")= vbYes Then	
					me.DataValue("CK1_REG.FLAGA.PVFL") = 0
					End If
			End If
		End If
		*/
		  SParamValueH v = FindExperion( html, "CK1_REG", "FLAGA.PVFL" );
		  void* val = html.pipe.ParamValue_H(v);
		  if (!val || v.def->eVal != 3)
		  {
			  //__debugbreak();
			  return true;
		  }
		  char old = *(char*)val;
		  char _new;

		  ::MessageBox(hWnd, "ВНИМАНИЕ!!! ДАЛЬНЕЙШИЕ ДЕЙСТВИЯ ПРИВЕДУТ К ИЗМЕНЕНИ РЕЖИМА РАБОТЫ КОМПРЕССОРА ЦК-1!", "ВНИМАНИЕ !!! !!! !!!", MB_OK);

		  if (old == 0)
		  {
			  if (IDYES == ::MessageBox(hWnd, "Для переключения компрессора ЦК-1 в режим 'РЕГЕНЕРАЦИЯ' нажмите кнопку <Yes>. Для отмены нажмите кнопку <No>", "ИЗМЕНЕНИЕ РЕЖИМА РАБОТЫ КОМПРЕССОРА", MB_YESNO))
			  {
				  _new = 1;
				  SSendToModel send(v.def);
				  send.Set(v.def->eVal, &old, &_new);
				  html.pipe.SendData(v.nNumber, send);
			  }
		  }
		  else
		  {
			  if (IDYES == ::MessageBox(hWnd, "Для переключения компрессора ЦК-1 в режим 'РЕАКЦИЯ' нажмите кнопку <Yes>. Для отмены нажмите кнопку <No>", "ИЗМЕНЕНИЕ РЕЖИМА РАБОТЫ КОМПРЕССОРА", MB_YESNO))
			  {
				  _new = 0;
				  SSendToModel send(v.def);
				  send.Set(v.def->eVal, &old, &_new);
				  html.pipe.SendData(v.nNumber, send);
			  }
		  }
		  return true;
	  }
  }
  else if (html.m_strFile == "M04")
  {
	  if (!strcmp(id, "pushbutton004")) // Останов ПК-1
	  {
/*
MsgBox "ВНИМАНИЕ!!! ДАЛЬНЕЙШИЕ ДЕЙСТВИЯ ПРИВЕДУТ К ОСТАНОВУ КОМПРЕССОРА ПК-1!", vbOKOnly, "ВНИМАНИЕ !!! !!! !!!"
If me.DataValue("PK1.SB_S.PVFL") = 0 Then
	If MsgBox("Для останова компрессора ПК-1 нажмите кнопку <Yes>. Для отмены нажмите кнопку <No>", vbYesNo or vbDefaultButton2, "ОСТАНОВ КОМПРЕССОРА")= vbYes Then	
		me.DataValue("PK1.SB_S.PVFL") = 1
	End If
End if
*/
		  SParamValueH v = FindExperion( html, "PK1", "SB_S.PVFL" );
		  CShBase* sh = html.pipe.Sh( v.nNumber );
          if ( !IsTypeOk  )
			html.pipe.End();
		  void* val = html.pipe.ParamValue_H(v);
		  if (!val || v.def->eVal != 3)
		  {
			  //__debugbreak();
			  return true;
		  }
		  char old = *(char*)val;
		  char _new;

		  ::MessageBox(hWnd, "ВНИМАНИЕ!!! ДАЛЬНЕЙШИЕ ДЕЙСТВИЯ ПРИВЕДУТ К ОСТАНОВУ КОМПРЕССОРА ПК-1!", "ВНИМАНИЕ !!! !!! !!!", MB_OK);
		  
		  if (old == 0)
		  {
			  if (IDYES == ::MessageBox(hWnd, "Для останова компрессора ПК-1 нажмите кнопку <Yes>. Для отмены нажмите кнопку <No>", "ОСТАНОВ КОМПРЕССОРА", MB_YESNO))
			  {
				  _new = 1;
				  SSendToModel send(v.def);
				  send.Set(v.def->eVal, &old, &_new);
				  html.pipe.SendData(v.nNumber, send);
			  }
		  }
		  return true;
	  }
	  else if (!strcmp(id, "pushbutton005")) // Останов ПК-2
	  {
/*
MsgBox "ВНИМАНИЕ!!! ДАЛЬНЕЙШИЕ ДЕЙСТВИЯ ПРИВЕДУТ К ОСТАНОВУ КОМПРЕССОРА ПК-2!", vbOKOnly, "ВНИМАНИЕ !!! !!! !!!"
If me.DataValue("PK2.SB_S.PVFL") = 0 Then
	If MsgBox("Для останова компрессора ПК-2 нажмите кнопку <Yes>. Для отмены нажмите кнопку <No>", vbYesNo or vbDefaultButton2, "ОСТАНОВ КОМПРЕССОРА")= vbYes Then	
		me.DataValue("PK2.SB_S.PVFL") = 1
	End If
End if
*/
		  SParamValueH v = FindExperion( html, "PK2", "SB_S.PVFL" );
		  CShBase* sh = html.pipe.Sh( v.nNumber );
          if ( !IsTypeOk  )
			html.pipe.End();
		  void* val = html.pipe.ParamValue_H(v);
		  if (!val || v.def->eVal != 3)
		  {
			  //__debugbreak();
			  return true;
		  }
		  char old = *(char*)val;
		  char _new;

		  ::MessageBox(hWnd, "ВНИМАНИЕ!!! ДАЛЬНЕЙШИЕ ДЕЙСТВИЯ ПРИВЕДУТ К ОСТАНОВУ КОМПРЕССОРА ПК-2!", "ВНИМАНИЕ !!! !!! !!!", MB_OK);
		  
		  if (old == 0)
		  {
			  if (IDYES == ::MessageBox(hWnd, "Для останова компрессора ПК-2 нажмите кнопку <Yes>. Для отмены нажмите кнопку <No>", "ОСТАНОВ КОМПРЕССОРА", MB_YESNO))
			  {
				  _new = 1;
				  SSendToModel send(v.def);
				  send.Set(v.def->eVal, &old, &_new);
				  html.pipe.SendData(v.nNumber, send);
			  }
		  }
		  return true;
	  }
	  else if (!strcmp(id, "pushbutton006")) // Реж. регенерации
	  {
/*
MsgBox "ВНИМАНИЕ!!! ДАЛЬНЕЙШИЕ ДЕЙСТВИЯ ПРИВЕДУТ К ИЗМЕНЕНИ РЕЖИМА РАБОТЫ КОМПРЕССОРА ПК-1!", vbOKOnly, "ВНИМАНИЕ !!! !!! !!!"
If me.DataValue("PK1_REG.FLAGA.PVFL") = 0 Then
	If MsgBox("Для переключения компрессора ПК-1 в режим 'РЕГЕНЕРАЦИЯ' нажмите кнопку <Yes>. Для отмены нажмите кнопку <No>", vbYesNo or vbDefaultButton2, "ИЗМЕНЕНИЕ РЕЖИМА РАБОТЫ КОМПРЕССОРА")= vbYes Then	
		me.DataValue("PK1_REG.FLAGA.PVFL") = 1
	End If
else 
	If me.DataValue("PK1_REG.FLAGA.PVFL") = 1 Then
		If MsgBox("Для переключения компрессора ПК-1 в режим 'РАБОТА' нажмите кнопку <Yes>. Для отмены нажмите кнопку <No>", vbYesNo or vbDefaultButton2, "ИЗМЕНЕНИЕ РЕЖИМА РАБОТЫ КОМПРЕССОРА")= vbYes Then	
			me.DataValue("PK1_REG.FLAGA.PVFL") = 0
		End If
	End If
End If
*/
		  SParamValueH v = FindExperion( html, "PK1_REG", "FLAGA.PVFL" );
		  void* val = html.pipe.ParamValue_H(v);
		  if (!val || v.def->eVal != 3)
		  {
			  //__debugbreak();
			  return true;
		  }
		  char old = *(char*)val;
		  char _new;

		  ::MessageBox(hWnd, "ВНИМАНИЕ!!! ДАЛЬНЕЙШИЕ ДЕЙСТВИЯ ПРИВЕДУТ К ИЗМЕНЕНИ РЕЖИМА РАБОТЫ КОМПРЕССОРА ПК-1!", "ВНИМАНИЕ !!! !!! !!!", MB_OK);

		  if (old == 0)
		  {
			  if (IDYES == ::MessageBox(hWnd, "Для переключения компрессора ПК-1 в режим 'РЕГЕНЕРАЦИЯ' нажмите кнопку <Yes>. Для отмены нажмите кнопку <No>", "ИЗМЕНЕНИЕ РЕЖИМА РАБОТЫ КОМПРЕССОРА", MB_YESNO))
			  {
				  _new = 1;
				  SSendToModel send(v.def);
				  send.Set(v.def->eVal, &old, &_new);
				  html.pipe.SendData(v.nNumber, send);
			  }
		  }
		  else
		  {
			  if (IDYES == ::MessageBox(hWnd, "Для переключения компрессора ПК-1 в режим 'РАБОТА' нажмите кнопку <Yes>. Для отмены нажмите кнопку <No>", "ИЗМЕНЕНИЕ РЕЖИМА РАБОТЫ КОМПРЕССОРА", MB_YESNO))
			  {
				  _new = 0;
				  SSendToModel send(v.def);
				  send.Set(v.def->eVal, &old, &_new);
				  html.pipe.SendData(v.nNumber, send);
			  }
		  }
		  return true;
	  }
	  else if (!strcmp(id, "pushbutton007")) // Реж. регенерации
	  {
/*
MsgBox "ВНИМАНИЕ!!! ДАЛЬНЕЙШИЕ ДЕЙСТВИЯ ПРИВЕДУТ К ИЗМЕНЕНИ РЕЖИМА РАБОТЫ КОМПРЕССОРА ПК-2!", vbOKOnly, "ВНИМАНИЕ !!! !!! !!!"
If me.DataValue("PK2_REG.FLAGA.PVFL") = 0 Then
	If MsgBox("Для переключения компрессора ПК-2 в режим 'Регенерация' нажмите кнопку <Yes>. Для отмены нажмите кнопку <No>", vbYesNo or vbDefaultButton2, "ИЗМЕНЕНИЕ РЕЖИМА РАБОТЫ КОМПРЕССОРА")= vbYes Then	
		me.DataValue("PK2_REG.FLAGA.PVFL") = 1
	End If
else 
	If me.DataValue("PK2_REG.FLAGA.PVFL") = 1 Then
		If MsgBox("Для переключения компрессора ПК-2 в режим 'Работа' нажмите кнопку <Yes>. Для отмены нажмите кнопку <No>", vbYesNo or vbDefaultButton2, "ИЗМЕНЕНИЕ РЕЖИМА РАБОТЫ КОМПРЕССОРА")= vbYes Then	
			me.DataValue("PK2_REG.FLAGA.PVFL") = 0
		End If
	End If
End If
*/
		  SParamValueH v = FindExperion( html, "PK2_REG", "FLAGA.PVFL" );
		  void* val = html.pipe.ParamValue_H(v);
		  if (!val || v.def->eVal != 3)
		  {
			  //__debugbreak();
			  return true;
		  }
		  char old = *(char*)val;
		  char _new;

		  ::MessageBox(hWnd, "ВНИМАНИЕ!!! ДАЛЬНЕЙШИЕ ДЕЙСТВИЯ ПРИВЕДУТ К ИЗМЕНЕНИ РЕЖИМА РАБОТЫ КОМПРЕССОРА ПК-2!", "ВНИМАНИЕ !!! !!! !!!", MB_OK);

		  if (old == 0)
		  {
			  if (IDYES == ::MessageBox(hWnd, "Для переключения компрессора ПК-2 в режим 'РЕГЕНЕРАЦИЯ' нажмите кнопку <Yes>. Для отмены нажмите кнопку <No>", "ИЗМЕНЕНИЕ РЕЖИМА РАБОТЫ КОМПРЕССОРА", MB_YESNO))
			  {
				  _new = 1;
				  SSendToModel send(v.def);
				  send.Set(v.def->eVal, &old, &_new);
				  html.pipe.SendData(v.nNumber, send);
			  }
		  }
		  else
		  {
			  if (IDYES == ::MessageBox(hWnd, "Для переключения компрессора ПК-2 в режим 'РАБОТА' нажмите кнопку <Yes>. Для отмены нажмите кнопку <No>", "ИЗМЕНЕНИЕ РЕЖИМА РАБОТЫ КОМПРЕССОРА", MB_YESNO))
			  {
				  _new = 0;
				  SSendToModel send(v.def);
				  send.Set(v.def->eVal, &old, &_new);
				  html.pipe.SendData(v.nNumber, send);
			  }
		  }
		  return true;
	  }
  }
  else 
  {
	  SParamValueH val;
	  double max = 100, min = 0;
	  double Raise = 0.1, fRaise = 1;
	  std::string objName = html.m_strObj;
	  std::string key = html.strCtrlBlockName;
      cross::CString aNm = "SP", mNm = "OP";
	  if(html.m_strFile == "sncdtlCPCa_fp"
		|| html.m_strFile == "sncdtlSPCMa_fp")
	  {
	    Raise = 0.1, fRaise = 1;
	    int p = objName.find(".");
	    if(p > 0) objName = objName.substr(0,p);
		objName += "." + key;
	  }
	  SParamValueH mode = FindExperion(html, objName.c_str(), "MODE");
	  char m = html.pipe.ValueB_H(mode);// 1 - AUTO, 0 - MAN
	  if (m == 0)
		  val = FindExperion(html, objName.c_str(), "OP");
	  else if (m == 1 && (key != "SEQ" && key != "LSel" && key != "SELH"))
	  {
		  val = FindExperion(html, objName.c_str(), aNm);
		  min = html.pipe.ValueF_H(FindExperion(html, objName.c_str(), "PVEULO"));
		  max = html.pipe.ValueF_H(FindExperion(html, objName.c_str(), "PVEUHI"));
		  Raise *= (max-min)/100, fRaise *= (max-min)/100;
	  }
		
	  double _new, _old;
	  _new = _old = html.pipe.ValueF_H( val );
	  
	  bool rez = false;
	  if (!strcmp(id, "RaiseBtn")) _new = _old + Raise, rez = true;
	  else if (!strcmp(id, "LowerBtn")) _new = _old - Raise, rez = true;
	  else if (!strcmp(id, "FastLowerBtn")) _new = _old - fRaise, rez = true;
	  else if (!strcmp(id, "FastRaiseBtn")) _new = _old + fRaise, rez = true;

	  if(!val.def) return rez;
	  if (_new <= min) _new = min;
	  if (_new >= max) _new = max, rez = true;
	  if (abs(_old - _new) < min(Raise, fRaise)/10) 
		return rez;
	  
	  SSendToModel send;
	  lstrcpy( send.szValue, val.def->name );
	  send.Set( enumValueDbl, &_old, &_new );
	  html.pipe.SendData( val.nNumber, send, true );
	  return true;
  }
  return false;
}
bool changeMvar140(CTagINPUT *_this, CHTML& html, const char * name, char value, char* title)
{
	//int k = _this->m_pPar->GetCount(html);  БЕЗ -q1 возвращает 0 
  	int k = (int)_this->m_pPar->GetSize();
    CTag** obj = _this->m_pPar->GetData();
	//for (int i = k - 1; i >= 0; i--)
    for (int i = 0; i<k; i++)
        {
		//CTag* tag = _this->m_pPar->GetAt(i);
        CTag* tag = obj[i];
		CStyle& _style = tag->Style(html);
		char* id = _style.NameID( html );
        char *pp = strstr(id, name);
		if (pp && (strcmp(pp, name)==0) && (tag->mVal.nNumber>=0))
        {
			if (IDYES == ::MessageBox(html.mWnd->m_hWnd, title, "", MB_YESNO | MB_DEFBUTTON2)){
				void *Old;
				char New = value;
				SSendToModel send;
				Old = tag->mVal.def->V(html.pipe.Sh(tag->mVal.nNumber));
				lstrcpy(send.szValue, tag->mVal.def->name);
				send.eType = tag->mVal.def->eVal;
				send.Set(Old, &New);
				html.pipe.SendData(tag->mVal.nNumber, send);
                char buf[128];
                _itoa_s(value, buf, sizeof(buf)-4, 10); 
                strcpy_s(tag->m_szScriptValue, buf);
			}
			return true;
		}
	}
	return true;
}
//
void CTagINPUT::Action( CHTML& html, HWND hWnd, bool bDown, CPoint* pt )
  {
  CStyle& style = Style(html);
  char* id = style.NameID( html );
  if ( Version == LG35_8_KF || (AM2_NAK == Version) && strstr(id, "_ScPb") )
  {
	  if (ActionLG35_8_KF(html, hWnd, bDown)) return;
	  CTag * par = this->m_pPar;
	  if (strstr(id, "pushbutton005") && !html.m_strFile.CompareNoCase("M01")){
		  SParamValueH pv = ::FindExperion(html, "SBDA6100", "FLAGB.PVFL");
		  char k = html.pipe.ValueB_H(pv);
		  if (k == 0){
			  if (::MessageBox(hWnd, "Reset? <Yes>. <No>", "Сброс", MB_YESNO | MB_DEFBUTTON2) == IDYES){
				  SSendToModel send;
				  lstrcpy( send.szValue, pv.def->name );
				  char cOld = 0, cNew = 1;
				  send.Set( enumValueChr, &cOld, &cNew );
				  html.pipe.SendData( pv.nNumber, send, true );
			  }
		  } else if (k == 1) {
			  if (::MessageBox(hWnd, "Cancel Reset? <Yes>. <No>", "Сброс", MB_YESNO | MB_DEFBUTTON2) == IDYES){
				  SSendToModel send;
				  lstrcpy( send.szValue, pv.def->name );
				  char cOld = 1, cNew = 0;
				  send.Set( enumValueChr, &cOld, &cNew );
				  html.pipe.SendData( pv.nNumber, send, true );
              }

		  }
		  return;
	  } else if (strstr(id, "ScPbDown"))
	  {
		  const char * ptr = Alias(html, par, "cp_down");
		  if (ptr && *ptr) GetAppWnd()->GiveAmm(hWnd)->GoToPage(ptr,NULL);
	      return;
	  }
	  else if (strstr(id, "ScPbUp") || strstr(id, "ScPbUP"))
	  {
		  const char * ptr = Alias(html, par, "cp_up");
		  if (ptr && *ptr) GetAppWnd()->GiveAmm(hWnd)->GoToPage(ptr,NULL);
	      return;
	  }
	  else if (strstr(id, "ScPbLeft"))
	  {
		  const char * ptr = Alias(html, par, "cp_left");
		  if (ptr && *ptr) GetAppWnd()->GiveAmm(hWnd)->GoToPage(ptr,NULL);
	      return;
	  }
	  else if (strstr(id, "ScPbRight"))
	  {
		  const char * ptr = Alias(html, par, "cp_right");
		  if (ptr && *ptr) GetAppWnd()->GiveAmm(hWnd)->GoToPage(ptr,NULL);
	      return;
	  }
	  else if (strstr(id, "_pshUT"))
	  {
		  html.m_bName140 = false;
	      return;
	  }
	  else if (strstr(id, "_pshName"))
	  {
		  html.m_bName140 = true;
	      return;
	  }
	  else if (strstr(id, "_PbVAR")  && changeMvar140(this, html, "_MODE",   5, "Режим вариатор"       )) return;
	  else if (strstr(id, "_PbSTOP") && changeMvar140(this, html, "_STOPS",  4, "Стоп вариатор"        )) return;
	  else if (strstr(id, "_PbREV")  && changeMvar140(this, html, "_REVERS", 5, "Включить режим Реверс")) return;
	  else if (strstr(id, "_PbBP")   && changeMvar140(this, html, "_MODE",   6, "Режим Байпас"         )) return;
  }

  if(AM2_NAK == Version)
  {
     if(stricmp(style.m_szTitle, "Acknowledge Page")==0)
     {
       html.AckAll();
       return;
     }
  }
  else
  if(VSB_KF == Version)
  {
     if(stricmp(m_szScriptValue, "Квит. стр.")==0)
     {
       html.AckAll();
       return;
     }
  }

  if ( lstrcmp( id, "PbAlmAck" ) == 0 )
    {
    if ( mVal.nNumber == -1 )
      mVal.nNumber = html.FindNumber(html);
    AckTag( html.pipe, mVal.nNumber );
    return;
    }
  if ( lstrcmp( id, "pshName" ) == 0 || lstrcmp( id, "shape003_pshName" ) == 0 )
    {
    html.ShowOrHide( hWnd, 2 );
    return;
    }
  if ( lstrcmp( id, "pshEU"   ) == 0 || lstrcmp( id, "shape003_pshEU"   ) == 0 )
    {
    html.ShowOrHide( hWnd, 1 );
    return;
    }
  if ( lstrcmp( id, "pshHide" ) == 0 || lstrcmp( id, "shape003_pshHide" ) == 0 )
    {
    html.ShowOrHide( hWnd, 0 );
    return;
    }
  if ( m_eInputType == inputCheckBox )
    {
		void* val = html.pipe.ParamValue_H(mVal);

		if (Version == UKR_PRP
			&& (!mVal.def || mVal.def->eVal == enumValueChr)
			&& (!this->m_pClickM && this->m_pPar->m_pClickM)
			&& this->m_pPar->IsAction(html, hWnd, bDown))
			return;

		if ( val != NULL && (mVal.def->eVal == enumValueChr || mVal.def->eVal == enumValueBol))
		{
            if(LG35_8_KF != Version)
            {//скрипты выполняют проверку возможности записи
            if(id[0]==0)
              {
                if(m_pPar && (m_pPar->GetType() == TT_SPAN) && m_pPar->m_pClickM)
                {
                   CTagSCRIPT* pClickM = m_pPar->m_pClickM;
                   if ( pClickM->m_eType == eventOnChange )
                   {
                      m_pPar->m_bCancelChange = false;
                      pClickM->RunScript();
                      if(m_pPar->m_bCancelChange)
                        return;  //отменили
                   }                    
                }
              }
            }
			SSendToModel send;
			BYTE bNew, bOld = *(char*)val;
			if (bOld > 0)
			{
				bNew = 0;
				*((double*)this->m_pPar->m_szScriptValue) = 0;
			}
			else
			{
				bNew = 1;
				*((double*)this->m_pPar->m_szScriptValue) = 1;
			}
			send.Set( mVal.def->eVal, &bOld, &bNew );
			lstrcpy( send.szValue, mVal.def->name );
			html.pipe.SendData( mVal.nNumber, send);
            AAA();
		}
		else if ( val != NULL && mVal.def->eVal == enumValueStr )
		{
            if(LG35_8_KF != Version)
            {//скрипты выполняют проверку возможности записи
            if(id[0]==0)
              {
                if(m_pPar && (m_pPar->GetType() == TT_SPAN) && m_pPar->m_pClickM)
                {
                   CTagSCRIPT* pClickM = m_pPar->m_pClickM;
                   if ( pClickM->m_eType == eventOnChange )
                   {
                      m_pPar->m_bCancelChange = false;
                      pClickM->RunScript();
                      if(m_pPar->m_bCancelChange)
                        return;  //отменили
                   }                    
                }
              }
            }
			SSendToModel send;
			char *bNew, *bOld = (char*)val;
            if ((Version==LG35_8_KF) || (mVal.def->dwLog != id_FLAG))
            {
			if ( !lstrcmpi( bOld, "ON" ))
			{
				bNew = "OFF";
				*((double*)this->m_pPar->m_szScriptValue) = 0;
			}
			else
			{
				bNew = "ON";
				*((double*)this->m_pPar->m_szScriptValue) = 1;
			}
            }
            else
             {
               char* szValue = (char *)val;
               bNew = "";
               if(mVal.def->dwLog == id_FLAG)
               {
                 CShFLAG sh;
			     if ( html.pipe.GetData( mVal.nNumber, &sh ) )
                 {
			        if(_countof(sh.STATETEXT)>=2 &&_stricmp(sh.STATETEXT[0], bOld )==0)
                    {
                       bNew = sh.STATETEXT[1];
                       *((double*)this->m_pPar->m_szScriptValue) = 1;
                    }
                    else
                    {
                        bNew = sh.STATETEXT[0];
                       *((double*)this->m_pPar->m_szScriptValue) = 0;
                    }
                 }
               }
             }

			send.Set( enumValueStr, bOld, bNew );
			lstrcpy( send.szValue, mVal.def->name );
			html.pipe.SendData( mVal.nNumber, send);
		}
		return;
	}
  //
  if ( IsAction( html, hWnd, bDown ) ) return;
  //
  SBaseDSD* dsd = Dsd(html);
  if ( dsd == NULL )
    AfxMessageBox( "Нажатие на непонятной кнопке" );
  else
  {
	  bool bIsPushButtonAction0=dsd->IsPushButtonAction0(html );
	  if (bIsPushButtonAction0)
	  {
	    SParamValueH* pVal=NULL;
        if(Version == DOTF_PM)
           dsd->ActionDSD( hWnd, html );
        else
        {//mwr по-моему тут используется какое-то случайное совпадение
		CStyle& styleNextPoint=html.Style(m_nStyle+1);
		if ( styleNextPoint.m_nPlaceDsd > 0 )
		{
			SBaseDSD* dsd=html.DSD(styleNextPoint.m_nPlaceDsd);
			if (dsd!=NULL)
				pVal = dsd->Value(html);
		}
        }
        if(pVal!=NULL)
	    dsd->PushAction0( hWnd, html, "", m_szScriptValue, pVal );
	  }
	 else
		dsd->ActionDSD( hWnd, html );
  }
  }
//
void CTag::ShowOrHide( CHTML& html, LPCTSTR tag, bool bShow )
  {
  CStyle& style = Style(html);
  char* id = style.NameID( html );
  char* fnd = strrchr( id, '_' );
  if ( fnd ) id = fnd+1;
  //
  if ( lstrcmpi( id, tag ) == 0 )
    style.m_eVisible = bShow ? enumVisible : enumHidden;
  //
	INT_PTR nCount = GetSize();
  CTag**     obj = GetData();
	for ( INT_PTR n = 0; n < nCount; n++ )
    obj[n]->ShowOrHide( html, tag, bShow );
  }
//
