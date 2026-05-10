#include "StdAfx.h"
#include "HTML.h"
#include "TagTextArea.h"


#include "Style.h"
#include "PipeClient.h"



#define ASSUNO { _static bool a = true; if ( a ) { a = false; ASSERT(0);} }

CTagTextArea::CTagTextArea(void)
{
     m_pAcceptValueCallback = NULL;
}
//
bool CTagTextArea::Accept(CHTML& html, CTag *pTag, void *arg)
{
	if ( pTag )
	{
       cross::SParamValueH mVal = pTag->mVal;
       if(mVal.nNumber >= 0)
       {
       if(mVal.def->eVal == enumValueChr)
        {
        BYTE nNew = *(BYTE*)arg;
        BYTE nOld = html.pipe.ValueB_H( mVal );
        if ( nOld == 0xff ){}
        else
        // Чтобы не занимать сеть
        if ( nOld == nNew ){}
        else
          {
          SSendToModel send;
          lstrcpy( send.szValue, mVal.def->name );
          send.Set( enumValueChr, &nOld, &nNew );
          html.pipe.SendData( mVal.nNumber, send, true );
	      return false; //false чтобы также записать значение в Value
          }
        }
       else
       if (mVal.def->eVal == enumValueDbl)
        {
        double dNew = *(double*)arg;;
        double dOld = html.pipe.ValueF_H( mVal );
        if ( dOld != dNew )
         {
         SSendToModel send;
         lstrcpy( send.szValue, mVal.def->name );
         send.Set( enumValueDbl, &dOld, &dNew );
         html.pipe.SendData( mVal.nNumber, send, true );
         return false; //false чтобы также записать значение в Value
         }
        }
      }
	}
	return false;
}
void CTagTextArea::Analiz( ANALIZ )
  {
  AnalizTextArea( this, html, teg );
  }

  void CTagTextArea::Kerdyk(HWND hWnd, CHTML &html)
  {
#ifdef _DEBUG
      CStyle& style1 = Style(html);
      char* id1 = style1.NameID(html);
      if ( strnicmp( id1, "ConAlpStateText", _countof("ConAlpStateText")-1 ) == 0)
          AAA();
      if ( stricmp( id1, "Pier_1_RDY" ) == 0)
          AAA();
#endif
      CTag::Kerdyk( hWnd, html );
      //
      CStyle& style = Style(html);
      char* id = style.NameID(html);
      ASSERT( style.mClass != clsAlphaNum );
#ifdef _DEBUG
      if ( _stricmp( id, "valOP" ) == 0)
          AAA();
#endif

      style.GetCSSStyle();

      if(AM2_NAK == Version)
      {
          if (style.m_eVisible == enumHidden && stricmp( id, "VALOP_OP" ) == 0)
          {
              if(mVal.nNumber == -2)
              { //не найден
                  lstrcpy(m_szScriptValue, "0");
                  mVal.nNumber = -1;
              }
          }
      }
      else
          if(L35_11600_KF == Version)
          {
              if ( html.m_strFile.CompareNoCase("Nav")==0)
                  if ( strcmp( id, "alpha001" ) == 0)
                  {
                      style.m_fHeight = 36;
                      style.CreateFont( html, this, style.m_fntStyle );
                  }
          }

      bool bAddEditOP=false;
      bool bAddEditOP1=false;
      bool bAddEditPV=false;
      bool bAddEditOther=false;
      if (mVal.def!=NULL)
      {
          bool bCanCheckEdit = true;
          if(Version==DOTF_PM)
          {
              if ( mVal.nNumber>=0 && lstrcmp( id, "shape001_mode" ) == 0 && html.m_strFile.CompareNoCase("mn10")==0)
              {
                  const char* name = html.pipe.GetMnemoName( mVal.nNumber );
                  mVal = FindExperion( html, name, "mode" );
              }
              if (mVal.def!=NULL)
              {
                  if(style.m_bReadOnly)
                      bCanCheckEdit = false; //по-идее правильно везде кроме исключений ниже
                  else
                      if ( (html.m_strFile.CompareNoCase("mn44")==0)
                          && (stricmp(id, "NUMERIC022")==0 || stricmp(id, "NUMERIC021")==0) //не понял отчего тут не так
                          )
                          bCanCheckEdit = false;
                      else
                          if ( (html.m_strFile.CompareNoCase("mn41")==0)
                              && (stricmp(id, "alpha034")==0 || stricmp(id, "alpha036")==0) //не понял отчего тут не так
                              )
                              bCanCheckEdit = false;
                          else
                              if ( (html.m_strFile.CompareNoCase("mn34")==0)
                                  && (stricmp(id, "NUMERIC016")==0 || stricmp(id, "NUMERIC017")==0) //не понял отчего тут не так
                                  )
                                  bCanCheckEdit = false;

                  if(style.m_bParameterFormat)
                  {//игнорировать параметры формата на мнемосхеме
                      if(mVal.def->dwLog == id_DATAACQ)
                      {
                          CShDATAACQ sh;
                          html.pipe.GetData( mVal.nNumber, &sh );
                          if ( IsTypeOK )
                          {
                              m_nControllerDigits = int(sh.PVFORMAT) % 4;
                          }
                      }
                  }
              }
          }
          else
              if(Version > DOTF_PM)
              {
                  if(style.m_bReadOnly)
                      bCanCheckEdit = false;
              }

          if(bCanCheckEdit)
          {
              bAddEditOP=(lstrcmpi(mVal.def->name,"OP")==0) && (Version != LG35_8_KF);
              bAddEditOP1=(lstrcmpi(mVal.def->name,"OP[1]")==0);
              bAddEditOP1=(lstrcmpi(mVal.def->name,"OP(1)")==0);
              bAddEditPV=((style.m_dwFlags &HDX_ALMSTATEMINUS1)&&(Version!=UKR_PRP)&&(lstrcmpi(mVal.def->name,"PV")==0));
              if(!bAddEditPV)
              {
                  bAddEditPV = (Version==LG35_8_KF) && (mVal.def->eVal == enumValueDbl) && (stricmp(mVal.def->name,"PV")==0) && (strncmp(id, "alpha", sizeof("alpha")-1)==0);
              }

              if(Version != LG35_8_KF)
              {
                  if(!bAddEditOP && !bAddEditOP1 && !bAddEditPV)
                  {
                      if((mVal.def->eVal == enumValueDbl) || (mVal.def->eVal == enumValueInt))
                          bAddEditOther = true;
                  }
              }
          }

          if(Version!=LG35_8_KF)  //
          {
              SBaseDSD* dsd = Dsd(html);
              if(dsd)
              {
                  LPCSTR pPointName = dsd->GetStr( html, "PointRefPointName" );
                  LPCSTR pParamName = dsd->GetStr( html, "PointRefParamName" );
                  if (!style.m_bReadOnly && pPointName && pPointName[0] && pParamName && pParamName[0])
                      m_pAcceptValueCallback = Accept;
              }
              else
              {
                  if (mVal.def!=NULL)
                      m_pAcceptValueCallback = Accept;
              }
          }

          if(strcmp(id, "ConAlpGOP")==0)
              m_pAcceptValueCallback = Accept; //пока только для этого случая привязка на запись
      }
      else
      {//если от сервера ничего про это нет, а тег есть и пареметр==name, то тут имя тега
          if(Version!=LG35_8_KF)  //
          {
              SBaseDSD* dsd = Dsd(html);
              if(dsd)
              {
                  LPCSTR pParamName = dsd->GetStr( html, "PointRefParamName" );
                  LPCSTR pPointName = dsd->GetStr( html, "PointRefPointName" );
                  if(pParamName && pPointName)
                      if(_stricmp(pParamName, "name")==0)
                      {
                          LPCSTR ref=NULL;
                          if((strncmp(pPointName, "{%", sizeof("{%")-1)==0) && (ref = strstr(pPointName+sizeof("{%"), "%}")))
                          {
                              if(_strnicmp( pPointName, "{%CurrentPoint%}", sizeof("{%CurrentPoint%}")-1)==0)
                                  m_constValue = html.Group().GetBuffer();
                              else
                              {
                                  char key[64]="";
                                  strncpy_s(key, pPointName+sizeof( "{%")-1, ref-pPointName-sizeof( "{%")+1);
                                  m_constValue = html.FindInDisplayDataRepository(key).c_str();
                              }
                          }
                          else m_constValue = pPointName;
                          mVal.nNumber=-1;
                      }
                      else
                          if(_stricmp(pParamName, "ITEMNAME")==0)
                          {
                              m_constValue = pPointName;
                              mVal.nNumber=-1;
                          }
              }
          }
      }
      if (bAddEditOP)
          m_pWnd =new CHTML_OP( html, style.Filtr(html) );
      else
          if (bAddEditOP1)
              m_pWnd =new CHTML_OP( html, style.Filtr(html), bAddEditOP1 );
          else
              if (bAddEditPV)
                  m_pWnd =new CHTML_PV( html, style.Filtr(html) );
              else
                  if ( m_pWnd == NULL && m_pClickM && m_pClickM->m_eType == eventOnChange )
                  {
                      m_pWnd = new CHTML_Edit( html, m_pClickM, m_szScriptValue );
                  }
                  else
                      if ( html.m_strFile.CompareNoCase("sysGrpDetail")==0 && stricmp(id, "alphaNum002")==0
                          || html.m_strFile.CompareNoCase("sysGrpTrend")==0 && stricmp(id, "alphaNum")==0
                          || html.m_strFile.CompareNoCase("sysGrpNumeric")==0 && stricmp(id, "alphaNum")==0
                          )
                      {
                          lstrcpy( m_szScriptValue, html.m_strGroup);
                          m_pWnd = new CHTML_Edit( html, m_pClickM, m_szScriptValue );
                      }
                      else
                          if ( Version == LG35_8_KF
 //                             && m_pWnd == NULL
                              && mVal.def && mVal.def->eVal == enumValueDbl
                              && html.m_strFile == "sysDtlPIDB"
                              && (style.m_dwTypeObj & TYPE_ALPHA)
                              && (style.m_dwTypeObj & TYPE_BINDING)
                              && (style.m_dwTypeObj & TYPE_TEXT)
                              && (style.m_dwTypeObj & TYPE_HOVER))
                          {
                              m_pWnd = new CHTML_LoopTuneEdit( html, &mVal );
                          }
                          else
                              if (bAddEditOther)
                                  m_pWnd =new CHTML_OTHER( html, &mVal, style.Filtr(html) );
      //
      if ( !style.HdxFlag(HDX_FILL_S) )
      {
          if ( style.HdxFlag(HDX_CLRLINE|HDX_CLRFILL) )
              style.m_nFillStyle = -2;
          else
              if ( style.HdxFlag(HDX_CLRFILL) )
                  style.m_nFillStyle = -2;//Но!
              else
                  if ( style.HdxFlag(HDX_CLRLINE) )
                      style.m_nFillStyle = -1;
                  else
                      style.m_nFillStyle = -1;
      }
      else
      {
          ASSUNO
          AAA();
      }
  }
//
// void CTagTextArea::Kerdyk( KERDYK )
//   {

//   }

void CTagTextArea::ApplyStyles(LOCALS)
{
	CStyle& style = Style(html);
	switch(style.mClass)
	{
/*
	case clsBoxLocRem_Enum0:
		style.m_clrFill  = 0x008000;
		break;
	case clsBoxLocRem_Enum1:
		style.m_clrFill  = 0x0000ff;
		break;
	case clsDiscrepancy_Alarm:
		style.m_eVisible = enumVisible;
		style.m_clrFill  = 0x00ffff;
		break;
	case clsDiscrepancy_Normal:
		style.m_eVisible = enumHidden;
		break;
	case clsEnclav_Actuador_Off:
		style.m_clrFill  = 0x808080;
		break;
	case clsEnclav_Actuador_On:
		style.m_clrFill  = 0x00A5FF;
		break;
	case clsEnclav_InitAnl_Off:
		style.m_clrFill  = 0x808080;
		break;
	case clsEnclav_InitAnl_On:
		style.m_clrFill  = 0x0000ff;
		break;
	case clsEnclav_InitDig_Off:
		style.m_clrFill  = 0x808080;
		break;
	case clsEnclav_InitDig_On:
		style.m_clrFill  = 0x0000ff;
		break;
	case clsEnclav_TxtBypass_Off:
		style.m_clrFill  = 0x808080;
		break;
	case clsEnclav_TxtBypass_On:
		style.m_clrFill  = 0xffff00;
		break;
	case clsEnclav_TxtBypass_On_Auto:
		style.m_clrFill  = 0x00ffff;
		break;
	case clsEnclav_TxtDescription:
		break;
	case clsEnclav_TxtName_Active:
		style.m_clrFill  = 0x0000ff;
		break;
	case clsEnclav_TxtName_Normal:
		style.m_clrFill  = 0x808080;
		break;
	case clsLinesArrow_Air:
		style.m_clrLine  = 0xff0000;
		break;
	case clsLinesArrow_Amine:
		break;
	case clsLinesArrow_Gas:
		style.m_clrLine  = 0xffff;
		break;
	case clsLinesArrow_LeanAmine:
		style.m_clrLine  = 0xFF00FF;
		break;
	case clsLinesArrow_Process:
		style.m_clrLine  = 0x9bff;
		break;
	case clsLinesArrow_ProcessHeavy:
		style.m_clrLine  = 0x4080;
		break;
	case clsLinesArrow_RichAmine:
		style.m_clrLine  = 0x800080;
		break;
	case clsLinesArrow_Steam:
		style.m_clrLine  = 0xff;
		break;
	case clsLinesArrow_Water:
		style.m_clrLine  = 0x8000;
		break;
	case clsLinExecState_OffScan_InActive:
		style.m_eVisible = enumVisible;
		break;
	case clsLinExecState_OnScan_Active:
		style.m_eVisible = enumHidden;
		break;
	case clsOvlBody_Motor_Bad:
		style.m_clrFill =  0x808080;
		break;  
	case clsOvlBody_Motor_Other:
		style.m_clrFill =  0xffffff;
		break;  
	case clsOvlBody_Motor_Run:
		style.m_clrFill  = 0x008000;
		break;
	case clsOvlBody_Motor_Stop:
		style.m_clrFill  = 0x0000ff;
		break;
	case clsOvlBody_Pump_Bad:
		style.m_clrFill  = 0x808080;
		break;
	case clsOvlBody_Pump_Other:
		style.m_clrFill  = 0xffffff;
		break;
	case clsOvlBody_Pump_Run:
		style.m_clrFill  = 0x008000;
		break;
	case clsOvlBody_Pump_Stop:
		style.m_clrFill  = 0x0000ff;
		break;
	case clsPlgBody_Motor_Bad:
		style.m_clrFill  = 0x808080;
		break;
	case clsPlgBody_Motor_Run:
		style.m_clrFill  = 0x008000;
		break;
	case clsPlgBody_Motor_Stop:
		style.m_clrFill  = 0x0000ff;
		break;
	case clsPlgBody_Pump_Bad:
		style.m_clrFill  = 0x808080;
		break;
	case clsPlgBody_Pump_Other:
		style.m_clrFill  = 0xffffff;
		break;	
	case clsPlgBody_Pump_Run:
		style.m_clrFill  = 0x008000;
		break; 
	case clsPlgBody_Pump_Stop:
		style.m_clrFill  = 0x0000ff;
		break; 
	case clsPlgIn_OnOffValve_Bad:
		style.m_clrFill  = 0x808080;
		break; 
	case clsPlgIn_OnOffValve_Close:
		style.m_clrFill  = 0x0000ff;
		break; 
	case clsPlgIn_OnOffValve_InBet:
		style.m_clrFill  = 0x808080;
		break;
	case clsPlgIn_OnOffValve_Open:
		style.m_clrFill  = 0x008000;
		break;
	case clsPlgIn_RegValve_Close:
		style.m_clrFill  = 0x0000ff;
		break;
	case clsPlgIn_RegValve_Open:
		style.m_clrFill  = 0x008000;
		break;
	case clsPlgIn_ValveCntlStatus_Alarm:
		style.m_clrFill  = 0xffff;
		break;
	case clsPlgIn_ValveCntlStatus_Close:
		style.m_clrFill  = 0xff;
		break;
	case clsPlgIn_ValveCntlStatus_Open:
		style.m_clrFill  = 0x8000;
		break;
	case clsPlgIn_ValveOnOffStatus_Alarm:
		style.m_clrFill  = 0xffff;
		break;
	case clsPlgIn_ValveOnOffStatus_Close:
		style.m_clrFill  = 0xff;
		break;
	case clsPlgIn_ValveOnOffStatus_Open:
		style.m_clrFill  = 0x8000;
		break;
	case clsPlgInterlock_Interlock_OFF:
		style.m_clrFill  = 0x808080;
		break;
	case clsPlgInterlock_Interlock_ON:
		style.m_clrFill  = 0x0000ff;
		break;
	case clsPlgOut_OnOffValve_Bad:
		style.m_clrFill  = 0x808080;
		break; 
	case clsPlgOut_OnOffValve_Close:
		style.m_clrFill  = 0x0000ff;
		break; 
	case clsPlgOut_OnOffValve_InBet:
		style.m_clrFill  = 0xc0c0c0;
		break; 
	case clsPlgOut_OnOffValve_Open:
		style.m_clrFill  = 0x008000;
		break;
	case clsPlgOut_RegValve_Close:
		style.m_clrFill  = 0x0000ff;
		break;
	case clsPlgOut_RegValve_Open:
		style.m_clrFill  = 0x008000;
		break;
	case clsPlgOut_ValveCntlStatus_Alarm:
		style.m_clrFill  = 0xffff;
		break;
	case clsPlgOut_ValveCntlStatus_Close:
		style.m_clrFill  = 0xff;
		break;
	case clsPlgOut_ValveCntlStatus_Open:
		style.m_clrFill  = 0x8000;
		break;
	case clsPlgOut_ValveOnOffStatus_Alarm:
		style.m_clrFill  = 0xffff;
		break;
	case clsPlgOut_ValveOnOffStatus_Close:
		style.m_clrFill  = 0xff;
		break;
	case clsPlgOut_ValveOnOffStatus_Open:
		style.m_clrFill  = 0x8000;
		break;
	case clsRecBody_Motor_Bad:
		break; 
	case clsRecBody_Motor_Run:
		style.m_clrFill  = 0x008000;
		break; 
	case clsRecBody_Motor_Stop:
		style.m_clrFill  = 0x0000ff;
		break; 
	case clsRecBody_Pump_Bad:
		style.m_clrFill  = 0x808080;
		break;
	case clsRecBody_Pump_Other:
		style.m_clrFill  = 0xffffff;
		break;
	case clsRecBody_Pump_Run:
		style.m_clrFill  = 0x008000;
		break;
	case clsRecBody_Pump_Stop:
		style.m_clrFill  = 0x0000ff;
		break; 
	case clsRecFrame_DigitalState_CYAN:
		style.m_clrFill  = 0xffff00;
		break;
	case clsRecFrame_DigitalState_GRAY:
		style.m_clrFill  = 0x808080;
		break;
	case clsRecFrame_DigitalState_GREEN:
		style.m_clrFill  = 0x008000;
		break;
	case clsRecFrame_DigitalState_RED:
		style.m_clrFill  = 0x0000ff;
		break; 
	case clsRecPV_DigitalState_GRAY:
		style.m_clrFill  = 0x808080;
		break; 
	case clsRecPV_DigitalState_GREEN:
		style.m_clrFill  = 0x008000;
		break;
	case clsRecPV_DigitalState_RED:
		style.m_clrFill  = 0x0000ff;
		break;
	case clsRecPV_Enum0:
		style.m_clrFill  = 0x0000ff;
		break; 
	case clsRecPV_Enum1:
		style.m_clrFill  = 0xc0c0c0;
		break;
	case clsRecPV_Enum2:
		style.m_clrFill  = 0xe6e6e6;
		break; 
	case clsRecPV_Enum3:
		style.m_clrFill  = 0x0;
		break; 
	case clsRecPV_Enum4:
		style.m_clrFill  = 0x008000;
		break; 
	case clsRecPV_Enum5:
		style.m_clrFill  = 0xffffff;
		break; 
	case clsRecPV_Enum6:
		style.m_clrFill  = 0xffffff;
		break; 
	case clsRecPV_Enum7:
		style.m_clrFill  = 0xaaaaaa;
		break; 
	case clsRecPV_VlvClose:
		style.m_clrFill  = 0x0000ff;
		break; 
	case clsRecPV_VlvOpen:
		style.m_clrFill  = 0x008000;
		break; 
	case clsScConAlpPV_Enum0:
		break; 
	case clsScConAlpPV_Enum1:
		break; 
	case clsScConAlpPV_Enum2:
		break; 
	case clsScConAlpPV_Enum3:
		break; 
	case clsScConAlpPV_Enum4:
		break; 
	case clsScConAlpPV_Enum5:
		break; 
	case clsScConAlpPV_Enum6:
		break; 
	case clsScConAlpPV_Enum7:
		break;
	case clsTxt_AbsorberStatus_Green:
		style.m_clrText = 0x008000;
		break; 
	case clsTxt_AbsorberStatus_Grey:
		style.m_clrText = 0x808080;
		break; 
	case clsTxtBad_Bad:
		style.m_eVisible = enumVisible;
		break; 
	case clsTxtBad_NotBad:
		style.m_eVisible = enumHidden;
		break; 
*/
	case clsTxtError_NoError:
		style.m_eVisible = enumHidden;
		break; 
/*
	case clsTxtMode_in_NMode:
		style.m_clrFill  = 0xdddddd; 
		style.m_clrText = 0x0;
		break; 
	case clsTxtMode_not_in_NMode:
		style.m_clrFill  = 0x808080;
		style.m_clrText = 0xdddddd; 
		break; 
	case clsWdgC_OnOffValve:
	case clsWdgC_OnOffValve_Close:
		style.m_clrFill  = 0xc0c0c0;
		break;
	case clsWdgC_OnOffValve_Open:
		style.m_clrFill  = 0xc0c0c0;
		break;
	case clsWdgC_OnOffValve_InBet:
		style.m_clrFill = 0xc0c0c0; 
		break; 
	case clsWdgRedTag_Off:
		style.m_eVisible = enumHidden;
		break; 
	case clsWdgRedTag_On:
		style.m_eVisible = enumVisible;
		style.m_clrFill = 0x1e69d2; 
		break;
	case clsWdgC_DamperValve_GRAY: 
		style.m_clrFill = 0x808080; 
		break;
	case clsWdgC_DamperValve_GREEN: 
		style.m_clrFill = 0x008000; 
		break;
	case clsWdgC_DamperValve_OTHER: 
		style.m_clrFill = 0x0000FF; 
		break;
	case clsWdgC_DamperValve_RED: 
		style.m_clrFill = 0x0000FF; 
		break;
	case clsWdgC_RegValve_Close: 
		style.m_clrFill = 0x0000FF; 
		break;
	case clsWdgC_RegValve_Open: 
		style.m_clrFill = 0x008000; 
		break;
*/
	case clsRecPV_LK2B_Normal:
		//style.m_clrFill  = 0x696969;
		break; 
	case clsTxtCText_LK2B_Normal:
		//style.m_clrText = 0x8c8c8c;
		break; 
	case clsRecPV_LK2B_Fail:
		//style.m_clrFill  = 0xff0000;
		break; 
	case clsTxtCText_LK2B_Fail:
		//style.m_clrText = 0x000000;
		break; 
	case clsTxtCText_Cause_Fail:
		//style.m_clrText = 0x000000;
		break;
	case clsTxtCText_Cause_Normal:
		//style.m_clrText = 0x8c8c8c;
		break;
	case clsRecPV_MO_Normal:
		style.m_clrFill  = 0x696969;
		break;		
	case clsRecPV_MO_Fail:
		style.m_clrFill  = 0xc0c0c0;
		break;		
	case clsUnknown: return;
	default:
		AAA();
	}
}
//
void CTagTextArea::Matrix( LOCALS )
  {
//  CTag::Script127Exec(this, hDC, html);
  CStyle& style = Style(html);
  char* id = style.NameID( html );
  if ( style.LocalD( hDC ) ) return;
  /*if (Version == LG35_8_KF && strstr(id, "AlpDescTxt") )
	  style.m_clrText = 0xffffff;*/
  ApplyStyles(hDC, html);
  //
#ifdef _DEBUG
  if ( stricmp( id, "ConAlpL1PVEUHi_GSH" ) == 0)
     AAA();
  if ( stricmp(id, "shape136_ScConAlpPV" )==0 )
     AAA();
#endif
  //
  //
  if ( style.m_eVisible == enumHidden )
  {
    if(Version>=DOTF_PM) 
      {//скрипты используют невидимые поля которые должны обновлятся
      char* filtr = style.Filtr(html);
      if(filtr && strcmp(filtr, "%.0f")==0 && style.m_nNumberOfChars==1)
        style.m_nNumberOfChars = 10;   //почему то так бывает
      int nInc = 0;
      if(filtr && strchr(filtr, 's') && style.m_nNumberOfChars>0)
         nInc = 3;
      style.m_nNumberOfChars += nInc;
      if(mVal.nNumber != -1)
        FillText( html, m_szScriptValue );
      style.m_nNumberOfChars -= nInc;
      }
    return;
  }
  //
  cross::RECT rect = style.FullRect();
  //
  if ( style.m_nFillStyle != -1 || Version >= SGK_ONPZ)
    style.Rect( hDC, html, rect, Version >= SGK_ONPZ );
  else if (Version == UKR_PRP && !html.m_strFile.CompareNoCase("LIGHT_TIME"))
  {
	style.m_nFillStyle = 0; 
	style.m_dwFlagObj &= 0xffff0fff;
    style.Rect( hDC, html, rect );
  }
  //
  int nClass = StyleClass( hDC, html );
  if ( nClass == -1 )
    return;
  //
#define NOFILLTEXT  "??????????" 
  char str[512] = NOFILLTEXT;
  bool bPaint = true;
  if ( nClass == 1 )
    {
    if ( Version < AM2_NAK/*наверно для всех кроме 140- т.к. m_szScriptValue текстовое */ &&  m_pClickM  && m_pClickM->m_eType == eventOnChange )
      {
      char* filtr = style.Filtr(html);
      if ( *filtr )
        {
        double val = atof(m_szScriptValue);
        sprintf_s( str, filtr, val );
        }
      else
        lstrcpy( str, m_szScriptValue );
      }
    else
    if ((mVal.nNumber == -1) && *m_szScriptValue )
      {
      lstrcpy( str, m_szScriptValue );
      }
    else
      {
      if ( m_constValue.IsEmpty() )
      {
        if((Version >= L35_11600_KF) && (mVal.nNumber==-1/*т.е не задано тега*/) )
          lstrcpy( str, m_constValue );
        else
        {
          bPaint = FillText( html, str );

          if(Version == BERTH_ONPZ)
          {
              if(html.m_strFile.CompareNoCase("PL2023")==0 && strcmp(str, "-9999")==0)
                lstrcpy(str, "----");
          }

          if((Version == LG35_8_KF || L35_11600_KF)
			&& !stricmp(html.m_strFile, "sncdtlSPCMa_fp") && mVal.def == NULL && !::IsMnemoStyle( MNEMO_EDIT )
			&& (!strcmp(id, "ConAlpL1EA" ) || !strcmp(id, "ConAlpL1SD" ) || !strcmp(id, "ConAlpTrValue" )))
				style.m_nFillStyle = -1, bPaint = false;
			
          if(strcmp(str, NOFILLTEXT) != 0)
          {
            lstrcpy(m_szScriptValue, str);
            //убираю 0и после точки и саму точку, если до неё одни 0и
            if(::IsFlt(m_szScriptValue) && strchr(m_szScriptValue, '.'))
            {
                size_t len = strlen(m_szScriptValue);
                for(size_t ii=len-1; ii>0; ii--)
                {
                  if(m_szScriptValue[ii] == '0')
                    m_szScriptValue[ii] = 0;
                  else if(m_szScriptValue[ii] == '.')
                       {
                           m_szScriptValue[ii] = 0; break;
                       }
                       else if(isdigit(m_szScriptValue[ii]))
                              break;
                }
            }
          }
        }
      }
      else
        lstrcpy( str, m_constValue );
      }
    }
  else
  switch ( nClass )
    {
    case clsTitleBarR:
      lstrcpy( str, html.Group() );
    break;
    }
  //
  style.GetCSSStyle();
  //
  if (Version == AT_PRP && !strnicmp(html.m_strFile, "Proverka plotnomerov", 20))
  {
	  __time64_t T = GiveModelTime();
	  struct tm Tm;
	  _localtime64_s( &Tm, &T );
	  
	  if (!strnicmp(id, "alpha008", 8) || !strnicmp(id, "alpha006", 8))
	  {
		  strftime( str , 8, "%H", &Tm );
	  }
	  else if (!strnicmp(id, "alpha009", 8) || !strnicmp(id, "alpha010", 8))
	  {
		  strftime( str , 8, "%M", &Tm );
	  }
	  else if (!strnicmp(id, "alpha012", 8) || !strnicmp(id, "alpha013", 8))
	  {
		  strftime( str , 8, "%S", &Tm );
	  }
  }

  bool bIgnoreBadValue=false;
  if ( (Version == LG35_8_KF || Version == L35_11600_KF) && !strnicmp(html.m_strFile, "NAV", 4))
  {
	  __time64_t T = GiveModelTime();
	  struct tm TM;
	  _localtime64_s( &TM, &T );
	  if (!strnicmp(id, "alpha002", 8))
	  {
		  strftime( str, sizeof(str), "%d.%m.%Y", &TM );
		  bIgnoreBadValue=true;
	  }
	  else
	  if (!strnicmp(id, "alpha001", 8))
	  {
		  strftime( str, sizeof(str), "%H:%M:%S", &TM );
		  bIgnoreBadValue=true;
	  }
  }

  if ( !*str ) return;
  //
   rect = style.OutsRect();
  if ( bPaint )
  if ( rect.left != rect.right )
  if ( rect.top != rect.bottom )
  if ( style.m_clrText != 0xff000000 )
  {
	  bool bDrawText=true;
	  COLORREF clrText=style.m_clrText;
	  if (mVal.nNumber>=0)
	  {
		  if (style.m_dwFlags &HDX_ALMSTATEMINUS1)
		  {
			  CAlarmBase* al = html.pipe.Alarm( mVal.nNumber );
			  if (al) // иногда al может быть нулем, обходим этот случай
			  {
				  COLORREF clr=GetColorIndicateAlarmMinus1(al->btEHLN);
				  if (clr != 0 && clr != 0xff000000)
					  clrText=clr;
				  if (( ! PulsTimer()) && (al->notACK ))
					  bDrawText=false;
			  }
		  }
	  }

	  if(Version == DMW_NAK)
	  if(html.m_strFile.CompareNoCase("WTP0201_0202") == 0
		  || html.m_strFile.CompareNoCase("WTP0203_0204") == 0)
		  if(strncmp(id, "alpha", 5) == 0)
              {style.align |= DT_WORDBREAK; rect.bottom += 100;}

      if(Version == AM2_NAK && html.m_strFile.Compare("331") == 0)
		  if(strcmp(id, "alpha010") == 0)
              {rect.top += 2; rect.bottom += 2;}

	  if (bDrawText)
        style.TextOut( hDC, clrText, rect, str, true );
  }
  //
//  if ( ::IsMnemoStyle( MNEMO_EDIT ) )
  if (!bIgnoreBadValue)
  if ( mVal.def == NULL )
  if ( mVal.nNumber != -1 && mVal.nNumber != -4)
  style.BadValue( hDC, "Надо вставить в модель!!! ");
  }
//
