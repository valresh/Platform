#include <StdAfx.h>
#include "HTML.h"
#include "TagSPAN.h"
//#include "QuickBuilderStruct.h"
#include "qbbasetype_.h"


#include "Style.h"
#include "Wnd/HTMLWndBase.h"

CTagSPAN::CTagSPAN(void)
  {
	  this->m_pAcceptCheckboxValueCallback = CTagSPAN::Accept;
      m_pCheckBoxInput = NULL;
  }
//
bool CTagSPAN::Accept(CHTML& html, CTag *_this, void *arg)
{
	if (_this && ((CTagSPAN*)_this)->m_pCheckBoxInput)
	{
		((CTagSPAN*)_this)->m_pCheckBoxInput->CheckboxCallbackFromScript(html, *((int*)arg));
		return true;
	}
	else return false;
}
void CTagSPAN::Analiz( ANALIZ )
  {
  AnalizSPAN( this, html, teg );
  }

  void CTagSPAN::Kerdyk(HWND hWnd, CHTML &html)
  {
      CStyle& style1 = Style(html);
      char* id = style1.NameID( html );
#ifdef _DEBUG
      if(stricmp(id, "ProductCmbx_1")==0)
          KKK();
#endif
      if ( Version == LG35_8_KF
          && ( !html.m_strFile.CompareNoCase("sysdtlMOS_fp")
              || !html.m_strFile.CompareNoCase("sysdtlPOS_fp")
              || !html.m_strFile.CompareNoCase("sysdtlflaga_rr_fp"))
          && !strcmp(id, "ComCboPV")) ;
      else
          if ( Version == LG35_8_KF && !strcmp(id, "shaSeqCmd_cmbCom")
              && (!html.m_strFile.CompareNoCase("sncdtlCPCa_fp")
                  || !html.m_strFile.CollateNoCase("sncdtlSPCMa_fp")) );
          else if( strnicmp(id, "combobox", sizeof("comobox")-1)==0 || strnicmp(id, "cboExternalModePermit", sizeof("cboExternalModePermit")-1)==0 ) ;
          else if( strnicmp(id, "ComboBadCtrlOpt", sizeof("ComboBadCtrlOpt")-1)==0 ) ;
          else
              if ( AS( style1.mClass != clsComboBox ))
                  if (Version < DOTF_PM)
                      return;
      CTag::Kerdyk( hWnd, html );
      CStyle& style = Style(html); //style1 может быть испорчен в Kerdyk так как там добавляются элементы и может быть перераспределена
      id = style.NameID( html );   //память на которую ссылается style1
      //
      if ( style.GetFlag(TYPE_COMBOBOX) )
      {
          if ( (Version == LG35_8_KF || Version==L35_11600_KF) &&  !strcmp(id, "shaSeqCmd_cmbCom") )
          {
              if(html.m_strFile.CompareNoCase("sncdtlCPCa_fp")==0)
              {
                  static COMBO_LIST sncdtlCPCa_cmb[] = { {"Пуск", "cLoad.PVFL", 1}, {"Стоп", "cUnload.PVFL", 1}, {"___", NULL, 0}, {NULL,NULL,0} };
                  m_pWnd = new CHTML_ENUM_LIST( html, (COMBO_LIST*)&sncdtlCPCa_cmb );
              }
              else
                  if(html.m_strFile.CollateNoCase("sncdtlSPCMa_fp")==0)
                  {
                      static COMBO_LIST sncdtlSPCMa_cmb[] = { {"Пуск", "cStart.PVFL", 1}, {"Стоп", "cStop.PVFL", 1},
                                                             {"СбросХл", "cResetCL.PVFL", 1}, {"СбросАО", "cResetESD.PVFL", 1}, {NULL,NULL,0} };

                      static COMBO_LIST sncdtlSPCMa_cmb238[] = { {"Пуск", "cStart.PVFL", 1}, {"Стоп", "cStop.PVFL", 1},
                                                                {"Сброс сч", "cResetCL.PVFL", 1}, {"Сброс АО", "cResetESD.PVFL", 1}, {NULL,NULL,0} };

                      m_pWnd = (Version == LG35_8_KF)
                                   ? new CHTML_ENUM_LIST( html, (COMBO_LIST*)&sncdtlSPCMa_cmb )
                                   : new CHTML_ENUM_LIST( html, (COMBO_LIST*)&sncdtlSPCMa_cmb238 );
                  }
          }
          else
              if ( mVal.def != NULL )
              {
                  ASSERT( mVal.def->eVal == enumValueChr );
                  if ( mVal.def->eVal == enumValueChr )
                  {
                      if ( Version == LG35_8_KF && ( !html.m_strFile.CompareNoCase("sysdtlMOS_fp") || !html.m_strFile.CompareNoCase("sysdtlPOS_fp")) && !strcmp(id, "ComCboPV")) m_pWnd = new CHTML_PVOP( html, mVal );
                      else m_pWnd = new CHTML_ENUM( html, mVal, this );
                  }
                  else if ( mVal.def->eVal == enumValueStr )
                  {
                      if ( Version == LG35_8_KF && (!html.m_strFile.CompareNoCase("sysdtlflaga_rr_fp")) && !strcmp(id, "ComCboPV")) m_pWnd = new CHTML_ENUM2( html, mVal );
                      else m_pWnd = new CHTML_ENUM( html, mVal, this );
                  }
                  else if ( mVal.def->eVal == enumValueDbl )
                  {
                      m_pWnd = new CHTML_ENUM( html, mVal, this);//html.Dsd(html) );
                  }
                  return;
              }
              else
                  if(Version == BERTH_ONPZ)
                  {
                      if(strncmp(id, "ProductCmbx_", _countof("ProductCmbx_")-1)==0 || strcmp(id, "VesselCmbx")==0)
                      {
                          std::vector<COMBO_LIST2> dataList;
                          char *pDock =  id + _countof("ProductCmbx_")-1;
                          CFileXML& xml = theApp.m_DictionaryXML;
                          HXML nMap = xml.FindItem( INVALID_HXML, "ArgosyMap" );
                          if (  nMap != INVALID_HXML )
                          {
                              if(strncmp(id, "ProductCmbx_", _countof("ProductCmbx_")-1)==0)
                              {
                                  HXML nProducts = xml.FindItem( nMap, "Products" );
                                  if ( nProducts != INVALID_HXML )
                                  {
                                      HXML nProduct = xml.FindItem(nProducts, "Product" );
                                      while ( nProduct != INVALID_HXML )
                                      {
                                          HXML nCodeArg = xml.FindItem(nProduct, "CodeArg" );
                                          HXML nNameArg = xml.FindItem(nProduct, "NameArg" );
                                          HXML nPostsAllowed = xml.FindItem(nProduct, "PostsAllowed" );
                                          if (nCodeArg != INVALID_HXML && nNameArg != INVALID_HXML && nPostsAllowed != INVALID_HXML )
                                          {
                                              HXML nPost = xml.FindItem(nPostsAllowed, "Post" );
                                              if ( nPost != INVALID_HXML )
                                              {
                                                  HXML nDock = xml.FindItem(nPost, "Dock" );
                                                  if ( nDock != INVALID_HXML )
                                                  {
                                                      const char* pcDock = xml.Param( nDock );
                                                      if(strcmp(pDock, pcDock) == 0)
                                                      {
                                                          COMBO_LIST2 ni;
                                                          cross::CString namei;
                                                          namei.Format("%s  (%s) (%s)", xml.Param(nNameArg), xml.Param(nCodeArg), xml.Param(xml.FindItem(nPost, "NameArg")));
                                                          ni.showItem = namei.GetBuffer();
                                                          namei.Format("E_DOCK%s_FuelID.PV", pDock);
                                                          ni.paramRead = namei.GetBuffer();
                                                          namei.Format("E_DOCK%s_FuelId_cmd.SP", pDock);
                                                          ni.paramWrite = namei.GetBuffer();
                                                          ni.val2set.eType = enumValueDbl;
                                                          ni.val2set.odouble =  (double)(dataList.size()+1);
                                                          dataList.push_back(ni);
                                                      }
                                                      AAA();
                                                  }
                                              }
                                          }
                                          nProduct =  xml.NextItem( nProduct );
                                      }
                                  }
                              }
                              else
                                  if(strcmp(id, "VesselCmbx")==0)
                                  {
                                      HXML nProducts = xml.FindItem( nMap, "VesselTypes" );
                                      if ( nProducts != INVALID_HXML )
                                      {
                                          HXML nProduct = xml.FindItem(nProducts, "VesselType" );
                                          while ( nProduct != INVALID_HXML )
                                          {
                                              HXML nNameArg = xml.FindItem(nProduct, "NameArg" );
                                              if (nNameArg != INVALID_HXML)
                                              {
                                                  COMBO_LIST2 ni;
                                                  ni.showItem = xml.Param(nNameArg);
                                                  const O_VAL* pov = html.GetCommomValue("DockN");
                                                  if(pov)
                                                  {
                                                      cross::CString namei;
                                                      namei.Format("E_DOCK%d_ShipType.PV", pov->oint);
                                                      ni.paramRead = namei.GetBuffer();
                                                      ni.val2set.eType = enumValueDbl;
                                                      ni.val2set.odouble =  (double)(dataList.size()+1);
                                                      dataList.push_back(ni);
                                                  }
                                              }
                                              nProduct =  xml.NextItem( nProduct );
                                          }
                                      }
                                  }

                              m_pWnd = new CHTML_ENUM_LIST2( html, &dataList, true );
                              AAA();
                          }
                      }
                  }
      }
      //
      if ( style.GetFlag( TYPE_CHECKBOX ) )
      {
          INT_PTR nCount = GetSize();
          CTag**     obj = GetData();
          ASSERT( nCount == 2 );
          if ( nCount == 2 )
          {
              CStyle& input = obj[0]->Style(html);
              CStyle& spans = obj[1]->Style(html);
              input.mRect.Width = input.mRect.Height;
              spans.mRect.X    += input.mRect.Height+2;
              spans.mRect.Width-= input.mRect.Height+6;
              input.m_hFont = style.m_hFont;
              spans.m_hFont = style.m_hFont;
              obj[0]->mVal  = mVal;
          }
          return;
      }
      //
      if ( !(style.m_dwFlags & HDX_RANGELOHI) || Version == AM2_NAK || Version == ITH_NAK/*похоже HDX_RANGELOHI не об этом*/ )
      {
          if ( mVal.nNumber >= 0 && mVal.def )
              if(mVal.def->dwLog != id_QBANALOG)
              {
                  if ( lstrcmpi( mVal.def->name, "OP" ) !=0)
                  {
                      EDataTypes t = (EDataTypes)html.pipe.GetMnemoType( mVal.nNumber );
                      mMax.def = ::NameToValueEx( t, "PVEUHI" );
                      mMin.def = ::NameToValueEx( t, "PVEULO" );
                      mMax.nNumber = mVal.nNumber;
                      mMin.nNumber = mVal.nNumber;
                  }
              }
              else
                  if ( lstrcmpi( mVal.def->name, "PV" )==0 || lstrcmpi(mVal.def->name, "SV")==0)
                  {
                      EDataTypes t = (EDataTypes)html.pipe.GetMnemoType( mVal.nNumber );
                      mMax.def = ::NameToValueEx( t, "RANGEAT100" );
                      mMin.def = ::NameToValueEx( t, "RANGEAT0" );
                      mMax.nNumber = mVal.nNumber;
                      mMin.nNumber = mVal.nNumber;
                  }
      }
      else
          if((style.m_dwFlags & HDX_RANGELOHI) && mVal.nNumber >= 0 && mVal.def->dwLog == id_QBANALOG)
          {
              if ( lstrcmpi( mVal.def->name, "PV" )==0 || lstrcmpi(mVal.def->name, "SV")==0)
              {
                  EDataTypes t = (EDataTypes)html.pipe.GetMnemoType( mVal.nNumber );
                  mMax.def = ::NameToValueEx( t, "RANGEAT100" );
                  mMin.def = ::NameToValueEx( t, "RANGEAT0" );
                  mMax.nNumber = mVal.nNumber;
                  mMin.nNumber = mVal.nNumber;
              }
          }
      //
      if ( mVal.def != NULL )
          if ( lstrcmpi( mVal.def->name, "PVP" ) == 0 || lstrcmpi( mVal.def->name, "SPP" ) == 0 )
              if ( (style.m_dwFlags & HDX_RANGELOHI) == 0 )
              {
                  style.m_dwFlags |= HDX_RANGELOHI;
                  style.m_dRangeHi = 100;
                  style.m_dRangeLo = 0.0;
              }
      //
  }
//

void DrawFrame(cross::HDC mhDC, cross::RECT rect)
{
     Graphics graphics( mhDC );
     graphics.SetSmoothingMode(SmoothingModeHighQuality);
     graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
     graphics.SetPixelOffsetMode(PixelOffsetModeHighQuality);

     cross::Gdiplus::Color stdBorderColor;   stdBorderColor.SetFromCOLORREF( 0x00aaaaaa );
	 Gdiplus::Color darkBorderColor;  darkBorderColor.SetFromCOLORREF( 0x00222222 );
	 Gdiplus::Color lightBorderColor; lightBorderColor.SetFromCOLORREF( 0x00eeeeee );

     
     Gdiplus::RECT r(cross::RECT.left, cross::RECT.top, abs(cross::RECT.right-cross::RECT.left), abs(cross::RECT.bottom-cross::RECT.top));
	 Gdiplus::Pen stdBorderPen( stdBorderColor, 2);
	 graphics.Drawcross::RECTangle(&stdBorderPen, r);

	 Gdiplus::Pen darkBorderPen( darkBorderColor, 1);	 
	 graphics.DrawLine(&darkBorderPen, r.GetLeft(), r.GetTop(), r.GetRight(), r.GetTop());
	 graphics.DrawLine(&darkBorderPen, r.GetLeft(), r.GetTop(), r.GetLeft(), r.GetBottom());

	 Gdiplus::Pen lightBorderPen( lightBorderColor, 1);	 
	 graphics.DrawLine(&lightBorderPen, r.GetRight(), r.GetBottom(), r.GetLeft(), r.GetBottom());
	 graphics.DrawLine(&lightBorderPen, r.GetRight(), r.GetBottom(), r.GetRight(), r.GetTop());
}
//
void CTagSPAN::Matrix( LOCALS )
  {
  CStyle& style = Style(html);
  char* id = style.NameID( html );
  
  if ( Version == LG35_8_KF && html.m_strFile == "sysAlarmSummary" &&  !lstrcmpi(this->m_pPar->Style(html).NameID(html), "AreaAndViewToolbar")){
	 return;
  }
  if ( Version == LG35_8_KF && html.m_strFile == "sysAlarmSummary" &&  (!lstrcmpi(id, "spnAreaPaneTitle")||!lstrcmpi(id, "spnViewLabel"))){
	  style.m_clrText = 0;
  }
  if ( style.LocalD( hDC ) ) return;
#ifdef _DEBUG
  if ( StopDebug( html, "indicator016" ) )
    AAA();
#endif

  style.GetCSSStyle();

  const char* text = style.GetValue(html);
  //
  if ( style.GetFlag(TYPE_COMBOBOX) )
    {
    if ( ::IsMnemoStyle( MNEMO_EDIT ) )
      {
      if( !m_pWnd || typeid(*m_pWnd) != typeid(CHTML_ENUM_LIST) )
       {      
           if ( mVal.def == NULL               ) 
               style.BadValue( hDC, "Надо вставить в модель!!! ");
      else if ( mVal.def->eVal != enumValueChr ) 
              style.BadValue( hDC, "Переменная имеет не тот тип!!! ");
       }
      }
    return;
    }
  //
  if ( style.GetFlag( TYPE_CHECKBOX ) )
    {
    return;
    }
  //
  if ( style.m_eVisible == enumHidden )
  {
    if(Version>=DOTF_PM) 
    if(mVal.nNumber > 0)
      {//скрипты используют невидимые поля которые должны обновлятся
         void* pval = NULL;
         if(mVal.def)
            { 
              EValueType eA = enumValueUnk;
              pval = html.pipe.ParamValue_H( mVal );
            }
         if(pval)
           switch(mVal.def->eVal)
              {
              case enumValueStr:
                lstrcpy(m_szScriptValue, (char*)pval);
                break;
               case enumValueDbl:
                sprintf_s(m_szScriptValue, "%.4f", *(double*)pval); break;
               case enumValueInt:
                sprintf_s(m_szScriptValue, "%d", *(int*)pval); break;
               case enumValueChr:
                sprintf_s(m_szScriptValue, "%d", (int)*(BYTE*)pval); break;
               case enumValueBol:
                lstrcpy(m_szScriptValue, *(bool*)pval ? "1" : "0" ); break;
              }
      }
    return;
  }
  //
  cross::RECTF rc = style.mcross::RECT;
  cross::RECT cross::RECT = { int(rc.X+0.5), int(rc.Y+0.5), int(rc.X+rc.Width+0.5), int(rc.Y+rc.Height+0.5) };
  //
  if ( style.colorBkg != 0xffffffff )
    {
    CMyBrush brush( hDC, style.colorBkg );
    brush.cross::RECT( cross::RECT );
    }  
  else
  if ( style.m_clrFill == 0xffffffff )
    {//если цвет фона здесь не задан в html - он белый
    CMyBrush brush( hDC, RGB(255,255,255) );
    brush.cross::RECT( cross::RECT );
    }
  //
  SBaseDSD* dsd = Dsd(html);
  if ( dsd && dsd->m_nCountPip > 1 )
    {
    ASSERT(0);//Этого не должно быть
    return;
    }
  //
  if ( *text )
    {
    style.TextOut( hDC, style.m_clrText, cross::RECT, text, false );
    return;
    }
  //
  double dValue = 0.5;
  COLORREF color = 0x80ff;
  void* val = html.pipe.ParamValue_H(mVal);
  const char* name = html.pipe.GetMnemoName( mVal.nNumber );
  EDataTypes  type = (EDataTypes)html.pipe.GetMnemoType( mVal.nNumber );
  if ( *m_szScriptValue )
    {
    dValue = atof(m_szScriptValue);
    color = Bindind( hDC, html, dValue );
    }
  else
  if ( val != NULL )
    {
    switch ( mVal.def->eVal )
      {
      case enumValueDbl:
        dValue = *(double*)val;
		if ( IsNaN(dValue) ){
		  style.Level( hDC, html, cross::RECT, dValue, color );
        DrawFrame(hDC, cross::RECT);
        return;
		}
        color = Bindind( hDC, html, dValue );
        break;
      case enumValueInt:
        dValue = (double)*(int*)val;
        color = Bindind( hDC, html, dValue );
        break;
      default:
       ASSUNO
       style.BadValue( hDC, "Не обрабатывается этот тип! " );
      return;
      };
    }
  else
  {
    DrawFrame(hDC, cross::RECT);
    return; //нет данных
  }

  //SetAlamColor(html, style);
  if (style.m_dwFlags & HDX_ALMSTATE)
  {
	CAlarmBase* al = html.pipe.Alarm( mVal.nNumber );
	if (al!=NULL)
	{
	 BYTE btKvit = al->notACK ? 0 : 1;
	 BYTE btPriority=al->btEHLN;
	 BYTE btOnOf = 1;
	 if ( PulsTimer() || btKvit )
	 {
      //может зависить от проекта
	  switch (btPriority)
	  {
			case 2:color=RGB(255,255,0); break;
			case 3:
			case 4:color=RGB(255,0,0); break;
	  }
	 }
	}
  }

  style.Level( hDC, html, cross::RECT, dValue, color );
  if(!(Version == LG35_8_KF 
	&& (!html.m_strFile.CompareNoCase("sncdtlCPCa_fp")
		||!html.m_strFile.CompareNoCase("sncdtlSPCMa_fp"))))
		DrawFrame(hDC, cross::RECT);
  //
  }
//
COLORREF CTagSPAN::Bindind( HDC hDC, CHTML& html, double& dValue )
  {
  CStyle& style = Style(html);
  if ( style.m_dwTypeObj & TYPE_INDICATOR )
    {
    double dMin,dMax;
    if ( style.m_dwFlags & HDX_RANGELOHI && (mMin.nNumber==-1 || mMin.def == NULL || mMax.nNumber==-1 || mMax.def == NULL) )
      {
      dMin = style.m_dRangeLo;
      dMax = style.m_dRangeHi;
      }
    else
      {
      dMin = html.pipe.ValueF_H(mMin,0.000);
      dMax = html.pipe.ValueF_H(mMax,100.0);
      }

	if(Version == LG35_8_KF 
		&& (!strcmp(mVal.def->name, "PVP") 
			|| !strcmp(mVal.def->name, "SPP")))
		dValue *= (dMax - dMin)/100;

    dValue = max( dValue, dMin );
    dValue = min( dValue, dMax );
    COLORREF color = style.m_clrFillLvl;
    if ( style.m_nBreakpointType == 0 )
      color = style.Con_Color(html,dValue);
    //
    if ( dMin >= dMax )
      dValue = 0.5, color = 0x80ff;
    else
      dValue = ( dValue - dMin ) / ( dMax - dMin );
    return color;
    }
  //
  if ( style.GetFlag(TYPE_COMBOBOX) )
    return 0xff000000;
  //ASSERT(0);  
  return 0xffff00;
  }
//
