//#include "UtilsH/StdAfx.h"
//#include "ARM.h"
//#include "../AplicFrm.h"
//#include "../AliasCSV.h"
//
//#include "HTML.h"
#include "Tag.h"
//#include "Bin.h"
//
// #include "TagTITLE.h"
// #include "TagOBJECT.h"
// #include "TagBODY.h"
// #include "TagSPAN.h"
#include "TagDIV.h"
// #include "TagIMG.h"
// #include "TagSCRIPT.h"
// #include "TagINPUT.h"
// #include "TagTEXTAREA.h"
// #include "TagA.h"
//#include "TagDetail.h"

//#include "SafetyManagerStruct.h"
#include "SmShData.h"
//#include "QuickBuilderStruct.h"
#include "QbShData.h"

#include "Scripts127.h"
#include "HTML.h"
#include "Style.h"

#include <global.h>

#undef  _static
#define _static static
//
#undef ASSERT
#define ASSERT(V)

#define ASSUNO { _static bool a = true; if ( a ) { a = false; ASSERT(0);} }
typedef char CharMP[_MAX_PATH];

#include <Profile.h>

int CTag::m_nStyle = 0;
//
CTag::CTag()
    :
    //    m_nStyle(0)
    //, m_pWnd(NULL)
    m_pPar(NULL)
    , m_bPressed(false)
    , m_pChange(NULL)
    , m_pClickM(NULL)
    , m_pMouseD(NULL)
    , m_pMouseU(NULL)
    , m_pLink(NULL)
    , m_bNoPaint(false)
    , mParam(0)
    , m_bCancelChange(false)
    , m_ScriptTextId(-1)
    //    , m_ScriptTexteType(eventUnk)
    , m_pPageComplete(NULL)
    , m_nControllerDigits(-1)
    , m_ClickType(ctFaceplate)
    , m_bIsShape(false)
{
    memset( m_szScriptValue, 0, sizeof(m_szScriptValue) );
    memset(m_szNumericDisplayValue, 0, sizeof(m_szNumericDisplayValue) );
}

CTag::~CTag(void)
{
    Clear();
}

void CTag::Clear()
{
    // Очистка всех дочерних элементов
    //НTODO
    // for (auto ptr : &this) {
    //     delete ptr;
    // }
}

void CTag::Kerdyk(HWND hWnd, CHTML &html)
{

}

void CTag::Script(HWND hWnd, CHTML &html)
{
    INT_PTR nCount = GetSize();
    CTag**  obj = GetData();
    for ( INT_PTR n = 0; n < nCount; n++ )
        obj[n]->Script( hWnd, html );
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CStyle& CTag::Style(CHTML& html)
{
    return html.Style(m_nStyle);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void CTag::AnalizStyle( CHTML& html, char* value )
{
    CStyle& style = Style(html);
    if ( !html.m_bBinData )
        style.AnalizSTYLE( html, this, value );
    style.Queue( html, this );
}

void CTag::AnalizHdx(CHTML &html, char *value)
{
    CStyle& style = Style(html);
    //if ( !html.m_bBinData )
    // style.AnalizHDX( html, value, mImg );
    //  style.AnalizHDX( html, value);
}
// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// void CTag::AnalizHdx( CHTML& html, char* value )
//   {
//   CStyle& style = Style(html);
//   if ( !html.m_bBinData )
//    // style.AnalizHDX( html, value, mImg );
//    style.AnalizHDX( html, value);
//   }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SBaseDSD* CTag::Dsd(CHTML& html)
  {
  CStyle& style = Style(html);
  if ( style.m_nPlaceDsd < 0 ) return NULL;
  return html.DSD(style.m_nPlaceDsd);
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// int CTag::GetCount(CHTML& html)
//   {
//   CStyle& style = Style(html);
//   return style.m_nKolvo;
//   }
// const char* CTag::GetName(CHTML& html,int n)
//   {
//   CStyle& style = Style(html);
//   return style.GetName(html,n);
//   }
// const char* CTag::GetValue(CHTML& html,int n)
//   {
//   CStyle& style = Style(html);
//   return style.GetValue(html,n);
//   }
const char* CTag::GetName(CHTML& html)
{
    CStyle& style = Style(html);
    return style.GetName(html);

}
const char* CTag::GetValue(CHTML& html)
{
    CStyle& style = Style(html);
    return style.GetValue(html);
}
// bool CTag::IsYou( CHTML& html, const char* szID )
//   {
//   CStyle& style = Style(html);
//   return style.IsYou( html, szID );
//   }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void CTag::NotFound( CHTML& html, cross::CString *list, int& nCounts )
{
    INT_PTR nCount = GetSize();
    CTag** obj = GetData();
    if(nCounts >= (MAX_QUQU-1))
        return;
    for ( INT_PTR n = 0; n < nCount; n++ )
    {
        obj[n]->NotFound( html, list, nCounts );
        if(nCounts >= (MAX_QUQU-1))
            return;
    }
    //
    // SBaseDSD* dsd = Dsd(html);
    // if ( dsd == NULL   ) return;
    // if ( dsd->m_nCountPip == 0 ) return;
    // SParamValueH* vars = html.Value();
    // int n = 0;
    // for ( ; n < dsd->m_nCountPip; n++ )
    //   {
    //   SParamValueH* var = dsd->Value(html,n);
    //   if ( var->nNumber == -2 )
    //     break;
    //   }
    // if ( n == dsd->m_nCountPip ) return;
    //
    // const char* name = dsd->PointName(html);
    // const char* vals = dsd->GetStr( html, "PointRefParamName" );
    // if ( !name || !*name ) return;
    // cross::CString str;
    // if ( vals && *vals )
    //   {
    //   const char* aaa = strchr( vals, '.' );
    //   if ( aaa )
    //     {
    //     str.Format( "%s.%s %s", name, vals, html.m_strFile );
    //     //char* p = strrchr( str.GetBuffer(), '.' );
    //     //if ( p ) *p = 0;
    //     //return;
    //     }
    //   else
    //     {
    //     str.Format( "%s (%s) %s", name, vals, html.m_strFile );
    //     //str.Format( "%s (%s)", name, vals );
    //     //if ( lstrcmpi( vals, "INALM" ) )
    //     //if ( lstrcmpi( vals, "NAME" ) )
    //     //return;
    //     //str = name;
    //     }
    //   }
    // else
    //   str = name;
    // //
    // for ( int n = 0; n < nCounts; n++ )
    // if ( lstrcmpi( list[n], str ) == 0 )
    //   return;
    // //ASSERT( nCounts < MAX_QUQU-1 );
    // if( nCounts < (MAX_QUQU-1))
    //   list[nCounts++] = str;
    // else AAA();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int FindColor( CHTML& html, CTag* pTag )
{
    // while ( pTag )
    //   {
    //   CStyle& style = pTag->Style(html);
    //   if ( style.m_dwFlags & HDX_CLRTEXT )
    //     return style.colorTxt;
    //   if (  style.colorTxt != 0xffffffff )
    //     return style.colorTxt;
    //   pTag = pTag->m_pPar;
    //   }
    return 0;
}
//
///*virtual*/ void CTag::Kerdyk( KERDYK )
//  {
//   SBaseDSD* dsd = Dsd(html);
//   if ( dsd != NULL )
//     {
//       char param[256];
//       UINT nType = dsd->Pipe( html, param, this );
//       if(nType)
//       {
//           strupr(param);
//           m_UserDefined.nType = nType;
//           strcpy_s(m_UserDefined.param, param);
//       }
//     }
//   //
//   INT_PTR nCount = GetSize();
//   CTag** obj = GetData();
//   for ( INT_PTR n = 0; n < nCount; n++ )
//     {
//    // obj[n]->Kerdyk( hWnd, html );
//     CStyle& style = obj[n]->Style(html);
//     if ( style.m_nBreakpointType != -1 )
//     if ( obj[n]->mVal.nNumber == -1 )
//       {
//       if ( dsd )
//         {
//         SParamValueH* var = dsd->Value(html);
//         if(var==NULL)
//           AAA();
//         else mVal = *var;
//         }
//       obj[n]->mVal = mVal;
//       }
//     }
//   //
//   CStyle& style = Style(html);
//   char* WWv = style.NameID( html );
// #ifdef _DEBUG
//   char* id = style.NameID(html);
//   if( strcmp("GrpState6", id)==0)
//      AAA();
// #endif
//   if ( (style.m_dwFlagObj & STYLE_VISIBLE) == 0 )
//     {
//     CTag* pPar = m_pPar;
//     while ( pPar )
//       {
//       CStyle& par = pPar->Style(html);
//       if ( par.m_dwFlagObj & STYLE_VISIBLE )
//         {
//         if(Version <= LG35_8_KF) //после LG35_8_KF если родитель не виден, потомки не будут видны, а в случаем видимости родителя у потомков может быть индивидуальная видимость
//           style.m_eVisible = par.m_eVisible;
//         break;
//         }
//       pPar = pPar->m_pPar;
//       }
//     }
//   //
//   if ( (style.m_dwFlags & HDX_CLRTEXT) == 0 )
//     {
//     if (  style.colorTxt != 0xffffffff )
//       style.m_clrText = style.colorTxt;
//     else
//       style.m_clrText = FindColor( html, this );
//     }
//   //
//   if ( style.mClass != clsUnknown )
//   if ( FillClass( html ) )
//     return;
//   //
//   if ( dsd )
//     {
//     if ( dsd->m_nCountPip == 1 )
//       {
//       SParamValueH* var = dsd->Value(html);
//       if ( mVal.nNumber == -1 )
//       mVal = *var;
//       }
//     }
//   if (Version == LG35_8_KF
// 		&& ( !html.m_strFile.CompareNoCase("sysdtlMOS_fp") || !html.m_strFile.CompareNoCase("sysdtlPOS_fp"))
// 		&& !strcmp(WWv, "ComCboPV") && mVal.def && mVal.def->eVal == enumValueStr)
//   {
// 	  const char * name = html.pipe.GetMnemoName(mVal.nNumber);
// 	  mVal = FindExperion(html, name, "PVFL");
//   }
//
//  }
//
void CTag::PaintThis( LOCALS )
{
    if ( m_bNoPaint )
        return;
    CStyle& style = Style(html);

    //     char* id = style.NameID( html );

    // #ifdef _DEBUG
    //     if (stricmp(id, "ConAlpPVEUHi" )==0)
    //         AAA();
    //     if (stricmp(id, "ConShpSIFL" )==0)
    //         AAA();
    // #endif

    //     if ( Version == LG35_8_KF
    //         && ((html.m_strFile.CompareNoCase("MVar1")==0) || (html.m_strFile.CompareNoCase("MVar2")==0))
    //         && (strncmp(id, "shape", sizeof("shape")-1)==0)
    //         && ((strcmp(id+sizeof("shape")+2, "_MODE") == 0) || (strcmp(id+sizeof("shape")+2, "_REVERS") == 0) || (strcmp(id+sizeof("shape")+2, "_STOPS") == 0) )
    //         )
    {//заполнение переменных для скриптов
        if (mVal.nNumber>=0)
        {
            // char *Old = (char*)mVal.def->V(html.pipe.Sh(mVal.nNumber));
            //          char buf[128];
            //          _itoa_s(*Old, buf, sizeof(buf)-4, 10);
            //          strcpy_s(m_szScriptValue, buf);
        }
    }
    //if ( style.m_dwFlags & HDX_INITcross::RECT )
    //if ( style.m_eVisible == enumHidden )
    //  return;
    //
    style.Local( hDC, html );
    Matrix( hDC, html );
    //
    EVisibility eVisible = enumVisible;
    if (Version != LG35_8_KF)  //там как-то работало
        eVisible = style.m_eVisible;

    if(html.m_nTimerCount == 0)
        eVisible = enumVisible;   //чтобы рассчитать все m_rcTruecross::RECT

    if(eVisible != enumHidden) //если что-то невидимо, то и все что в это включено тоже невидимо
    {
        INT_PTR nCount = GetSize();
        CTag**     obj = GetData();
        for ( INT_PTR n = 0; n < nCount; n++ )
        {
            if (Version != LG35_8_KF)
            {
                if(style.m_clrFill != -1 && (TYPE_PNT==TYPE_GROUP))
                {  //если у группы цвет поменян скриптом - меняем цвет дочерних элементов
                    if(style.m_bClrFillCanChangedByScript || style.m_bChangedByData)
                    {
                        CStyle& stylen = obj[n]->Style(html);
                        stylen.m_clrFill = style.m_clrFill;
                    }
                    else AAA();
                }
            }

            bool bSuppressRotate = false;
            if (Version >= L35_11600_KF)
                if ( style.m_dwFlags & HDX_ROTATE )
                {
                    // if( typeid(*obj[n]) == typeid(CTagINPUT))
                    // {  // CTagINPUT не поворачиваются
                    //     bSuppressRotate = true;
                    //     style.PostsD( hDC );
                    // }
                }
        }
    }
}



int CTag::HWBTN(CHTML& html,WORD nCode,UINT uKey)
{
    if ( m_bNoPaint )
        return 0;
    CStyle& style = Style(html);
    if ( style.m_dwTypeObj & TYPE_ALARMICON )
    {
        // auto placeholder = AckTag;
        // placeholder(html.pipe, mVal.nNumber);
    }
    //
    // if ( m_pWnd )
    //   return m_pWnd->HWBTN( nCode, uKey );
    //
    INT_PTR nCount = size();
    CTag**     obj = data();
    for ( INT_PTR n = 0; n < nCount; n++ )
        if ( obj[n]->HWBTN( html, nCode, uKey ) )
            return 1;
    return 0;
}
//
INT_PTR CTag::FullCount()
{
    INT_PTR nCount = GetSize();
    INT_PTR nFull = nCount;
    CTag**     obj = GetData();
    for ( INT_PTR n = 0; n < nCount; n++ )
        nFull += obj[n]->FullCount();
    return nFull;
}
//
bool CTag::IsPresent(CHTML& html)
{
    if ( m_bNoPaint )
        return false;
    CStyle& style = Style(html);
    if ( style.m_dwFlags & HDX_PRESENT )
        return true;
    INT_PTR nCount = GetSize();
    CTag**     obj = GetData();
    for ( INT_PTR n = 0; n < nCount; n++ )
        if ( obj[n]->IsPresent(html) )
            return true;
    return false;
}
//
CTag* CTag::Objects( CHTML& html, const char* t )
{
    char id[256];
    CStyle& style = Style(html);
    // char* name = style.NameID(html);
    // lstrcpy( id, name );
    lstrcat( id, "_" );
    lstrcat( id, t );
    return LinkScript( html, id );
}
//
CTag* CTag::LinkScript(CHTML& html,UINT id)
{
    if ( m_bNoPaint )
        return NULL;
    CStyle& style = Style(html);
    if ( style.mId == id )
        return this;
    INT_PTR nCount = GetSize();
    CTag**     obj = GetData();
    for ( INT_PTR n = 0; n < nCount; n++ )
    {
        CTag* fnd = obj[n]->LinkScript( html, id );
        if ( fnd )
            return fnd;
    }
    return NULL;
}
//
CTag* CTag::GetFlag( CHTML& html, DWORD dwFlag )
{
    if ( m_bNoPaint )
        return NULL;
    CStyle& style = Style(html);
    if ( style.GetFlag(dwFlag) )
        return this;
    INT_PTR nCount = GetSize();
    CTag**     obj = GetData();
    for ( INT_PTR n = 0; n < nCount; n++ )
    {
        CTag* fnd = obj[n]->GetFlag( html, dwFlag );
        if ( fnd )
            return fnd;
    }
    return NULL;
}
//
bool CTag::SetLink( CHTML& html, DWORD dwFlag )
{
    if ( m_pLink == NULL )
    {
        CTag* pTag = html.GetFlag( html, dwFlag );
        //if ( pTag == NULL || pTag->m_pWnd == NULL )
        if ( pTag == NULL )
        {
            ASSUNO;
            return false;
        }
        //   m_pLink = pTag->m_pWnd;
    }
    return true;
}
//
CTag* CTag::LinkScript(CHTML& html, const char* c)
{
    if ( m_bNoPaint )
        return NULL;
    CStyle& style = Style(html);
    // char* name = style.NameID(html);
    // if ( lstrcmpi( c, name ) == 0 )
    //     return this;


    INT_PTR nCount = GetSize();
    CTag**     obj = GetData();
    for ( INT_PTR n = 0; n < nCount; n++ )
    {
        CTag* fnd = obj[n]->LinkScript( html, c );
        if ( fnd )
            return fnd;
    }
    return NULL;
}
//
CTag* CTag::FindNearestTag(CHTML& html, CTag *pThisTag, const char* c)
{
    if ( m_bNoPaint )
        return NULL;
    //поиск родительского Shape
    CTag *pShapeTag = pThisTag->m_pPar;
    while(pShapeTag)
    {
        CStyle& style = pShapeTag->Style(html);
        if(style.IsShape())
        {
            CTag *pTag = pShapeTag->LinkScript(html, c);
            if(pTag)
                return pTag;
        }
        pShapeTag = pShapeTag->m_pPar;
    }
    return NULL;
}
//
// CTag* CTag::FindType(CHTML& html, const std::type_info &type)
//   {
//   if ( m_bNoPaint )
//     return NULL;

//   CStyle& style = Style(html);
//   char* name = style.NameID(html);
//   if(typeid(*this) != type)
//     return this;

//   INT_PTR nCount = GetSize();
//   CTag**     obj = GetData();
//   for ( INT_PTR n = 0; n < nCount; n++ )
//     {
//     CTag* fnd = obj[n]->FindType(html, type);
//     if ( fnd )
//       return fnd;
//     }

//   return NULL;
//   }
//
int CTag::FindNumber(CHTML& html)
{
    if ( m_bNoPaint )
        return -1;

    if ( mVal.nNumber >= 0 )
        return mVal.nNumber;
    //
    INT_PTR nCount = GetSize();
    CTag**     obj = GetData();
    for ( INT_PTR n = 0; n < nCount; n++ )
    {
        int fnd = obj[n]->FindNumber( html );
        if ( fnd != -1 )
            return fnd;
    }
    return -1;
}
//
#include "../MnemoFrm.h"
//void CTag::Action( CHTML& html, HWND hWnd, bool bDown, CPoint* pt )
void CTag::Action( CHTML& html, bool bDown, cross::CPoint* pt )
 {
 //
 if ( IsAction( html, bDown ) )
     return;
 if ( ActionSysBtn( html ) )
     return;
 //
 CStyle& style = Style(html);
 char* id = style.NameID( html );
 //
 if (IsSpecUpMenu089_Project(style,html))
 {
      GoToSpecScheme089_Project(html);
      return;
 }
 if (Version==LG35_8_KF)
 {
      if (bDown && (!lstrcmpi(id, "ConAlpDescription") || !lstrcmpi(id, "alphaTagName")))
        {
            // const char* name = html.pipe.GetMnemoName( mVal.nNumber );
            // OutputDebugString(name);
//            GetAppWnd()->Detail(name);
        }
 }

 if ( html.m_strFile == cross::CString("sysAlarmSummary") && strcmp(id, "btnSilenceAudible")==0 )
 {
//     GetAppWnd()->HoneywellKBD(0, hw_SIL);
     return;
 }

 if ( !(style.m_dwTypeObj & TYPE_FACEPLATE) ) return;
 //
 const char* name = NULL;
 // if ( mVal.nNumber != -1 && mVal.nNumber != -2 )
 //   name = html.pipe.GetMnemoName( mVal.nNumber );
 // else
 //   {
 //   SBaseDSD* dsd = Dsd(html);
 //   if ( !( dsd && dsd->m_nCountPip ) )
 //     return;
 //   //
 //   SParamValueH* var = dsd->Value(html);
 //   name = html.pipe.GetMnemoName( var->nNumber );
 //   }
 // if (Version == LG35_8_KF && name && strstr(id, "_up_face") && (html.m_strFile.CompareNoCase("HOZ_RASCH")!=0)
 //      && (html.m_strFile.CompareNoCase("M36")!=0))
 // {
 //      const char * pos = strstr(name, ".DACA");
 //      if (pos)
 //      {
 //          char _name[64];
 //          strcpy(_name, name);
 //          _name[pos-name] = 0;
 //          sprintf(_name, "%s.PIDA", _name);
 //          name = _name;
 //      }
 // }
 //
 if ( name != NULL )
   {
   CharMP szName;
   lstrcpy( szName, name );
   //AntiAlias(szName);//MIHAIL обрезалось имя
    // if (!theApp.IsAllInOne() && !theApp.m_bFourMonitors)
    //   GetAppWnd()->ChoiseFaceplate( szName );
    // else{
    //     if ( GetAppWnd()->m_wndSens && hWnd == GetAppWnd()->m_wndSens.m_wndMnemo->m_hWnd )
    //        {//для сенсорной панели
    //         GetAppWnd()->m_wndSens.ResetFaceplate();
    //         //GetAppWnd()->m_wndSens.m_wndfp->Faceplate(szName);
    //         GetAppWnd()->m_wndSens.GoFP(szName);
    //        }
    //     else {//открывается и в своём окне и на сенсорной панели
    //            CAplicFrm* appwnd = GetAppWnd();
    //           appwnd->ChoiseFaceplate( szName, GetAppWnd()->GiveAmm(hWnd, true) );
    //            if (::IsWindow(appwnd->m_wndSens.m_hWnd))
    //            {
    //                appwnd->m_wndSens.ResetFaceplate();
    //                //appwnd->m_wndSens.m_wndfp->Faceplate(szName);
    //                appwnd->m_wndSens.GoFP(szName);
    //            }
    //           }
    //     }
   }
 }
void CTag::Release()
{
    //  if ( m_pWnd != NULL ) m_pWnd->Release();
}
//
//bool CTag::IsAction( CHTML& html, HWND hWnd, bool bDown )
bool CTag::IsAction( CHTML& html, bool bDown )
{
    if ( bDown )
    {
        // if ( m_pMouseD )
        // {
        //     if ( m_pMouseD->m_eType == eventOnMouseDown )
        //     {
        //         m_pMouseD->RunScript();
        //         return true;
        //     }
        //     else
        //     {
        //         ASSERT(0);
        //     }
        // }
        // else
        //     return false;
    }
    //
    // if ( m_bNoPaint )
    //     return false;
    // if ( m_pWnd != NULL )
    // {
    //     m_pWnd->Pressed();
    //     return true;
    // }
    // //
    // if ( m_pClickM )
    // {
    //     if ( m_pClickM->m_eType == eventOnClick )
    //     {
    //         html.WINDOWS_EXTERNAL_CURRENTPAGE[0] = 0;
    //         if (m_pClickM->RunScript() == -1)
    //         {
    //             if(html.WINDOWS_EXTERNAL_CURRENTPAGE[0])
    //             {
    //                 NaviGo(html, hWnd, html.WINDOWS_EXTERNAL_CURRENTPAGE);
    //             }
    //             return true;
    //         }
    //         if ( m_pMouseU == NULL )
    //             return true;
    //     }
    //     else
    //     {
    //         ASSERT(0);
    //     }
    // }
    // //
    // if ( m_pMouseU )
    // {
    //     if ( m_pMouseU->m_eType == eventOnMouseUp )
    //     {
    //         m_pMouseU->RunScript();
    //         return true;
    //     }
    //     else
    //     {
    //         ASSERT(0);
    //     }
    // }
    //
    CStyle& style = Style(html);

    if(Version==DOTF_PM) //по-идее правильно везде
        if((style.m_dwTypeObj & (TYPE_SCHEMATIC|TYPE_FACEPLATE))==0)
            return false;

    if (m_ClickType==ctPopup && style.m_nPopup )
    {
        // char* name = style.PopupName(html);
        // std::string strNamePopupWithPoint=name;
        // char* namePoint=NULL;
        // if ( style.m_nPopupCurrentPoint )
        //     namePoint = style.CurrentPointName(html);
        // const char* pParamName = NULL;
        // if (namePoint!=NULL)
        // {
        //     strNamePopupWithPoint+="?";
        //     strNamePopupWithPoint+=namePoint;
        //     // SBaseDSD* dsd = Dsd(html);
        //     // if(dsd)
        //     //     pParamName = dsd->GetStr( html, "PointRefParamName" );
        //     if(pParamName && *pParamName)
        //     {
        //         strNamePopupWithPoint += "?";
        //         strNamePopupWithPoint += pParamName;
        //     }
        // }
        // else
        //     if(style.m_nPopupGroupParametrs)
        //     {
        //         strNamePopupWithPoint +=  "?&";
        //         strNamePopupWithPoint +=  html.Buffer(style.m_nPopupGroupParametrs);
        //     }
        //char* nameWithPoint=_strdup(strNamePopupWithPoint.c_str());
        //  const char* nameWithPoint=strNamePopupWithPoint.c_str();
        //     GetAppWnd()->SetLastActiveMnemoForPopups(hWnd);
        //  HWND hChild = (HWND)GetAppWnd()->SendMessage( nPressQueue, QUEUE_POPUP, (LPARAM)nameWithPoint );
        //  if ( style.m_ptPopup.x != -1 && style.m_ptPopup.y != -1 )
        //  if ( ::IsWindow( hChild ) )
        //    {
        //    cross::RECT rc;
        //    ::GetWindowcross::RECT( hWnd, &rc );
        //    //::ScreenToClient( hWnd, (LPPOINT)&rc );
        // if (Version == AT_PRP)
        // {
        //POINT p;
        //if (GetCursorPos(&p)) ::SetWindowPos( hChild, HWND_TOP, p.x, p.y, 0, 0, SWP_NOSIZE );
        // }
        // else
        //::SetWindowPos( hChild, HWND_TOP, rc.left+style.m_ptPopup.x, rc.top+style.m_ptPopup.y, 0, 0,/* SWP_NOZORDER|*/SWP_NOSIZE );
        //    }
        //  return true;
        //  }
        //
        if(!bDown)
        {
            const char *dspName = Alias(html, this, "DspName");
            if(dspName && strcmp(dspName, "DspName")!=0 && dspName[0] && CHTML::IsTrueMemoname(dspName))
            {
                //   NaviGo(html, hWnd, dspName);
                return true;
            }
        }
        return false;
    }
}

bool CTag::ActionSysBtn(CHTML &html)
{
    return true;
}
//
bool CTag::IsRadioButtonFromFaceplate(CStyle& style, CHTML& html)
{
    // char* id = style.NameID(html);
    // if ((lstrcmp(id,"ShpGOPState4")==0)||(lstrcmp(id,"ShpGOPState5")==0)||(lstrcmp(id,"ShpGOPState6")==0)
    //     ||(lstrcmp(id,"shpPVFL1")==0)||(lstrcmp(id,"shpPVFL2")==0))
    //     return true;
    return false;
}

CTag* CTag::PtInTag( CHTML& html, cross::POINT& pt, bool bAll, bool loadInnerFirst)
{
    if ( m_bNoPaint )
        return NULL;
    CStyle& style = Style(html);
    //     char* id = style.NameID(html);
    //     if (strcmp(id, "Page") != 0 || bAll)
    //         if ( ::PtIncross::RECT( &style.m_rcTruecross::RECT, pt ) )
    //         {
    // #ifdef _DEBUG
    //             if(stricmp(id, "hyperlink006") ==0)
    //                 AAA();
    //             if(stricmp(id, "shape045_ScTxtSelectBox") ==0)
    //                 AAA();
    // #endif
    //             if (Version == LG35_8_KF && strstr(id, "cross::RECTarg")) return this;
    //             if (loadInnerFirst) return this;
    //             if (IsRadioButtonFromFaceplate(style, html))
    //                 return this;
    //             if ( StopDebug( html, "ShpGOPState5" ) )
    //                 AAA();
    //             if (Version == LG35_8_KF && strstr(id, "ShpX"))
    //                 return this;
    //             if ((Version == LG35_8_KF) && (!lstrcmpi(id, "alphaTagName") || strstr(id, "ScTxtSelectBox") || !lstrcmpi(id, "ConAlpDescription") || strstr(id, "EnclavSE")) && style.m_eVisible != enumHidden)
    //                 return this;
    //             // if (CAplicFrm::IsOpenFaceplateByScTxtSelectBox() && strstr(id, "ScTxtSelectBox"))
    //             //     return this;
    //             if (IsSpecUpMenu089_Project(style, html))
    //                 return this;
    //             if (style.GetFlag(TYPE_CHECKBOX))
    //             {
    //                 INT_PTR nCount = GetSize();
    //                 CTag** obj = GetData();
    //                 for ( INT_PTR n = nCount-1; n >= 0; n-- )
    //                 {
    //                     CTag* find = obj[n]->PtInTag( html, pt, bAll, true );
    //                     if ( find != NULL )
    //                         return find;
    //                 }
    //             }
    //             //  Вызов VBScript-кода   Вызов окна мнемосхемы
    //             if ( m_pClickM != NULL || m_pMouseD != NULL || m_pMouseU != NULL
    //                 || ( style.m_nPopup && ((Version!=DOTF_PM) || (style.m_dwTypeObj & TYPE_SCHEMATIC)) )
    //                 || style.GetFlag(TYPE_PUSHBUTTON) )
    //             {
    //                 if ( style.m_eVisible != enumHidden )
    //                     return this;
    //             }
    //             //
    //             if ( style.m_dwTypeObj & TYPE_FACEPLATE )
    //             {
    //                 //if ( mVal.nNumber != -1 && mVal.nNumber != -2 )
    //                 // SBaseDSD* dsd = Dsd(html);
    //                 // if ( dsd && dsd->m_nCountPip )
    //                 if ( style.m_eVisible != enumHidden )
    //                     return this;
    //             }
    //             if ( bAll || // m_pWnd ||
    //                 (style.m_dwTypeObj & TYPE_SYS_BUTTON) )
    //             //Sergej 06.04 if ( bAll || m_pWnd || ((style.m_dwTypeObj & TYPE_SYS_BUTTON) && style.mClass) )
    //             {
    //                 if ( style.m_eVisible != enumHidden )
    //                     return this;
    //             }
    //             if (Version == LG35_8_KF)
    //             {
    //                 for(std::vector<Script127>::iterator it = m_vctrOnClickScripts.begin(); it != m_vctrOnClickScripts.end(); ++it)
    //                 {
    //                     if (*it == usNav_SelectBox_OnClick_me_) return this;
    //                 }
    //             }
    //         }// if ( ::PtIncross::RECT( &style.m_rcTruecross::RECT, pt ) )
    //
    if (!bAll && style.m_eVisible == enumHidden)
        return NULL;
    //
    INT_PTR nCount = GetSize();
    CTag**     obj = GetData();
#if 0
	for ( INT_PTR n = 0; n < nCount; n++ )
#else
    for ( INT_PTR n = nCount-1; n >= 0; n-- )
#endif
    {
        CTag* find = obj[n]->PtInTag( html, pt, bAll );
        if ( find != NULL )
            return find;
    }
    //
    if ( global::PtInRect( &style.m_rcTrueRect, pt ) )
    {
        if(m_ClickType == ctMnemo)
            return this;
        const char *dspName = Alias(html, this, "DspName");
        if(dspName && strcmp(dspName, "DspName")!=0 && dspName[0])
            return this;
    }
    //
    return NULL;
}
//
CTag* CTag::PtInBoundTag( CHTML& html, cross::POINT& pt)
{
    if ( m_bNoPaint )
        return NULL;

    CStyle& style = Style(html);
    if ( ( style.m_eVisible != enumHidden ) && ::PtInRect( &style.m_rcTrueRect, pt ))
    {
#ifdef _DEBUG
        //     const char* id = style.NameID( html );
#endif 
        if(mVal.nNumber >=0 )
            return this;
    }

    INT_PTR nCount = GetSize();
    CTag**  obj = GetData();
    for ( INT_PTR n = nCount-1; n >= 0; n-- )
    {
        CTag* find = obj[n]->PtInBoundTag( html, pt );
        if ( find != NULL )
            return find;
    }

    return NULL;
}
//поиск всех видимых тегов (кроме "Page") под курсором
int CTag::FindTags(CHTML& html, cross::POINT& pt, std::vector<CTag*>& tags)
{
    if ( m_bNoPaint )
        return 0;

    CStyle& style = Style(html);
    if ( ( style.m_eVisible != enumHidden ) && ::PtInRect( &style.m_rcTrueRect, pt ))
    {
        const char* id = style.NameID( html );
        if(strcmp(id, "Page") != 0)
            tags.push_back(this);
    }

    INT_PTR nCount = GetSize();
    CTag**  obj = GetData();
    for ( INT_PTR n = nCount-1; n >= 0; n-- )
    {
        obj[n]->FindTags(html, pt, tags);
    }

    return (int)tags.size();
}
//
bool CTag::FindTitle( CHTML& html, cross::POINT& pt, LPSTR title)
{
    if ( m_bNoPaint )
        return false;
    CStyle& style = Style(html);
    if ( global::PtInRect( &style.m_rcTrueRect, pt ) )
    {
        if(style.m_eVisible != enumHidden && style.m_szTitle[0])
        {
            lstrcpy(title, style.m_szTitle);
            return true;
        }
    }

    INT_PTR nCount = GetSize();
    CTag**     obj = GetData();
    for ( INT_PTR n = nCount-1; n >= 0; n-- )
    {
        if(obj[n]->FindTitle( html, pt, title ))
            return true;
    }

    return false;
}
//
bool CTag::IsSpecUpMenu089_Project(CStyle& style, CHTML& html)
{
    if (Version==Реформинг_КНПЗ)
    {
        // if (style.mClass==clsScTxtSelectBox_NotSelected)
        // {
        //     std::string strText = style.NameID( html );
        //     if (strText.find("_ScTxtSelection")!=std::string::npos)
        //         return true;
        // }
    }
    return false;
}

CTag* CTag::TabIndex( CHTML& html, cross::POINT& pt )
{
    if ( m_bNoPaint )
        return NULL;
    CStyle& style = Style(html);
    if (  style.nTabIndex != -1 )
        if ( style.m_eVisible != enumHidden )
             if ( cross::PtInRect( &style.m_rcTrueRect, pt ) )
            {
                if ( StopDebug( html, "btnAckPage" ) )
                    AAA();
                if ( mVal.nNumber >= 0 )
                    if ( mVal.def != NULL )
                        return this;
            }
    //
    INT_PTR nCount = GetSize();
    CTag**     obj = GetData();
    for ( INT_PTR n = nCount-1; n >= 0; n-- )
    {
        CTag* find = obj[n]->TabIndex( html, pt );
        if ( find != NULL )
            return find;
    }
    //
    return NULL;
}
//
void CTag::Matrix( LOCALS )
{
    CStyle& style = Style(html);
#ifdef _DEBUG
    //   const char* id = style.NameID( html );
#endif 
    // if ( style.LocalD( hDC ) ) return;
}
//
void CTag::Postik( LOCALS )
{
    CStyle& style = Style(html);
    //  style.PostsD( hDC );
}
//
typedef CTag* (*LPFuncs)();
typedef CTag* (*LPFuncsEx)(int nParam);
//
class CTagXML    : public CTag{public: CTagXML    (void){};};CTag* fnXML     () { return new CTagXML     (); }
class CTagTR     : public CTag{public: CTagTR     (void){};};CTag* fnTR      () { return new CTagTR      (); }
class CTagTD     : public CTag{public: CTagTD     (void){};};CTag* fnTD      () { return new CTagTD      (); }
class CTagTABLE  : public CTag{public: CTagTABLE  (void){};};CTag* fnTABLE   () { return new CTagTABLE   (); }
class CTagTBODY  : public CTag{public: CTagTBODY  (void){};};CTag* fnTBODY   () { return new CTagTBODY   (); }
class CTagSTYLE  : public CTag{public: CTagSTYLE  (void){};};CTag* fnSTYLE   () { return new CTagSTYLE   (); }
class CTagPARAM  : public CTag{public: CTagPARAM  (void){};};CTag* fnPARAM   () { return new CTagPARAM   (); }
//class CTagA      : public CTag{public: CTagA      (void){};};CTag* fnA       () { return new CTagA       (); }
class CTagP      : public CTag{public: CTagP      (void){};};CTag* fnP       () { return new CTagP       (); }
class CTagBR     : public CTag{public: CTagBR     (void){};};CTag* fnBR      () { return new CTagBR      (); }
class CTagMETA   : public CTag{public: CTagMETA   (void){};};CTag* fnMETA    () { return new CTagMETA    (); }
class CTagLINK   : public CTag{public: CTagLINK   (void){};};CTag* fnLINK    () { return new CTagLINK    (); }
class CTagITEM   : public CTag{public: CTagITEM   (void){};};CTag* fnITEM    () { return new CTagITEM    (); }
class CTagIFRAME : public CTag{public: CTagIFRAME (void){};};CTag* fnIFRAME  () { return new CTagIFRAME  (); }
class CTagHTML   : public CTag{public: CTagHTML   (void){};};CTag* fnHTML    () { return new CTagHTML    (); }
class CTagHEAD   : public CTag{public: CTagHEAD   (void){};};CTag* fnHEAD    () { return new CTagHEAD    (); }
class CTagDocType: public CTag{public: CTagDocType(void){};};CTag* fnDOCTYPE () { return new CTagDocType (); }
class CTagCOL    : public CTag{public: CTagCOL    (void){};};CTag* fnCOL     () { return new CTagCOL     (); }

class CTagVRoundRect: public CTag{public: CTagVRoundRect(void){};};CTag* fnv_roundrect () { return new CTagVRoundrect (); }
class CTaghn_alarm_tracker: public CTag{public: CTaghn_alarm_tracker(void){};};CTag* hn_alarm_tracker() { return new CTaghn_alarm_tracker(); }
//
// CTag* fnTITLE   () { return new CTagTITLE   (); }
// CTag* fnOBJECT  () { return new CTagOBJECT  (); }
// CTag* fnBODY    () { return new CTagBODY    (); }
// CTag* fnSPAN    () { return new CTagSPAN    (); }
CTag* fnDIV     () { return new CTagDIV     (); }
// CTag* fnIMG     () { return new CTagIMG     (); }
// CTag* fnINPUT   () { return new CTagINPUT   (); }
// CTag* fnTEXTAREA() { return new CTagTextArea(); }
// CTag* fnSCRIPT  () { return new CTagSCRIPT  (); }
// CTag* fnDetail  () { return new CTagDetail  (); }
// CTag* fnA()        { return new CTagA(); }
//
//#include "TagGrpState.h"
//#include "TagShpGPVSt.h"
// #include "TagShpGOPSt.h"
// #include "TagShpPVFLSt.h"
// #include "TagConCboMode.h"
// #include "TagConCboModeAttr.h"
// #include "TagConCboOP.h"
// #include "TagConCboPVFL.h"
// #include "TagConCboPVMan.h"
// #include "TagConAlpPV.h"
// #include "TagConAlpOP.h"
// #include "TagConAlpSP.h"
// #include "TagConAlpPVMan.h"
// #include "TagConShpAlmPr.h"
// #include "TagConAlpSelEqn.h"
// #include "TagShpX.h"
// #include "TagSpanCombo.h"
// #include "TagFaceplate.h"
// #include "TagAlarmState001.h"
// #include "TagAlarmTEXT.h"
// #include "TagConShpLstatus.h"
// #include "TagsInterlock140.h"
// #include "TagAreaPane.h"
// CTag* fnGrpState(int n) { return new CTagGrpState      (n); }
// CTag* fnShpGPVSt(int n) { return new CTagShpGPVState   (n); }
// CTag* fnShpGOPSt(int n) { return new CTagShpGOPState   (n); }
// CTag* fnCboMode (int n) { return new CTagConCboMode    (n); }
// CTag* fnCboAttr (int n) { return new CTagConCboModeAttr(n); }
// CTag* fnCboPVMan   (int n) { return new CTagConCboPVMan   (n); }
// CTag* fnCboOP   (int n) { return new CTagConCboOP      (n); }
// CTag* fnCboPVFL (int n) { return new CTagConCboPVFL      (n); }
// CTag* fnAlpPV   (int n) { return new CTagConAlpPV      (n); }
// CTag* fnAlpOP   (int n) { return new CTagConAlpOP      (n); }
// CTag* fnAlpSP   (int n) { return new CTagConAlpSP      (n); }
// CTag* fnAlpPVMan(int n) { return new CTagConAlpPVMan   (n); }
// CTag* fnAlpSelEqn(int n){ return new CTagConAlpSelEqn  (n); }
// CTag* fnScShpX  (int n) { return new CTagScShpX        (n); }
// CTag* fnShpX    (int n) { return new CTagShpX          (n); }
// CTag* fnShpAlmPr(int n) { return new CTagConShpAlmPr   (n); }
// CTag* fnCboSPAN (int n) { return new CTagSpanCombo     (n); }
// CTag* fnCboFace (int n) { return new CTagFaceplate     (n); }
// CTag* fnAlarmSt (int n) { return new CTagAlarmState001 (n); }
// CTag* fnAlarmTxt(int n) { return new CTagAlarmTEXT     (n); }
// CTag* fnShpPVFLSt(int n) { return new CTagShpPVFLState (n); }
// CTag* fnConShpLstatus(int n) { return new CTagConShpLstatus(n); }
// CTag* fnAreaPane(int n) { return new CTagAreaPane(); }
//
inline char* FindTagItem( STegScn* teg, const char* name )
{
    int n = 0;
    while ( teg[n].name )
    {
        if ( lstrcmpi( teg[n].name, name ) == 0 )
            return teg[n].value;
        n++;
    }
    return NULL;
}
//
CTag* CTag::AnalizSpecific( ANALIZ, char ID, void* func )
{
    struct SLocal { const char* name; LPFuncsEx func; char ID; int type; };
    // _static SLocal list[] =
    //     {
    //      {"ShpGPVState4"       , fnShpGPVSt, 'D',  4 },
    //      {"ShpGPVState5"       , fnShpGPVSt, 'D',  5 },
    //      {"ShpGPVState6"       , fnShpGPVSt, 'D',  6 },
    //      {"ShpGOPState4"       , fnShpGOPSt, 'D',  4 },
    //      {"ShpGOPState5"       , fnShpGOPSt, 'D',  5 },
    //      {"ShpGOPState6"       , fnShpGOPSt, 'D',  6 },
    //      {"shpPVFL1"           , fnShpPVFLSt,'D',  1 },
    //      {"shpPVFL2"           , fnShpPVFLSt,'D',  0 },
    //      {"GrpState6"          , fnGrpState, 'D',  6 },
    //      {"ConCboMode"         , fnCboMode , 'S', -1 },
    //      {"ConCboModeAttr"     , fnCboAttr , 'S', -1 },
    //      {"ConCboOP"           , fnCboOP   , 'S', -1 },
    //      {"ConCboPVMan"        , fnCboPVMan   , 'S', -1 },
    //      {"ComCboPVFL"         , fnCboPVFL   , 'S', -1 },
    //      {"ConAlpPV"           , fnAlpPV   , 'T', -1 },
    //      {"ConAlpOP"           , fnAlpOP   , 'T', -1 },
    //      {"ConAlpSP"           , fnAlpSP   , 'T', -1 },
    //      {"ConAlpPVMan"        , fnAlpPVMan, 'T', -1 },
    //      {"ConAlpSelEqn"       , fnAlpSelEqn,'T', -1 },

        //      {"ScShpX1"            , fnScShpX,   'D',  1 },
        //      {"ScShpX2"            , fnScShpX,   'D',  2 },
        //      {"ScShpX3"            , fnScShpX,   'D',  3 },
        //      {"ScShpX4"            , fnScShpX,   'D',  4 },
        //      {"ScShpX5"            , fnScShpX,   'D',  5 },
        //      {"ScShpX6"            , fnScShpX,   'D',  6 },
        //      {"ScShpX7"            , fnScShpX,   'D',  7 },
        //      {"ScShpX8"            , fnScShpX,   'D',  8 },

        //      {"ShpX1"              , fnShpX,     'D',  1 },
        //      {"ShpX2"              , fnShpX,     'D',  2 },
        //      {"ShpX3"              , fnShpX,     'D',  3 },
        //      {"ShpX4"              , fnShpX,     'D',  4 },
        //      {"ShpX5"              , fnShpX,     'D',  5 },
        //      {"ShpX6"              , fnShpX,     'D',  6 },
        //      {"ShpX7"              , fnShpX,     'D',  7 },
        //      {"ShpX8"              , fnShpX,     'D',  8 },

        //      // Сигнализационные пределы
        //      {"ConShpAlmPr"        , fnShpAlmPr, 'D',  0 },
        //      {"ConShpAlmPr_RecH"   , fnShpAlmPr, 'D',  1 },
        //      {"ConShpAlmPr_RecL"   , fnShpAlmPr, 'D',  2 },
        //      {"ConShpAlmPr_RecHH"  , fnShpAlmPr, 'D',  3 },
        //      {"ConShpAlmPr_RecLL"  , fnShpAlmPr, 'D',  4 },

        //      {"ConShpAlmPr_indpvHigh", fnShpAlmPr, 'D',  1 },
        //      {"ConShpAlmPr_indpvLow", fnShpAlmPr, 'D',  2 },
        //      {"ConShpAlmPr_indpvHighHigh", fnShpAlmPr, 'D',  3 },
        //      {"ConShpAlmPr_indpvLowLow", fnShpAlmPr, 'D',  4 },

        //      {"ConShpAlmPr001", fnShpAlmPr, 'D',  0 },
        //      {"ConShpAlmPr001_RecH", fnShpAlmPr, 'D',  1 },
        //      {"ConShpAlmPr001_RecL", fnShpAlmPr, 'D',  2 },
        //      {"ConShpAlmPr001_RecHH", fnShpAlmPr, 'D',  3 },
        //      {"ConShpAlmPr001_RecLL", fnShpAlmPr, 'D',  4 },

        //      {"ConShpAlmPr001_indpvHigh", fnShpAlmPr, 'D',  1 },
        //      {"ConShpAlmPr001_indpvLow", fnShpAlmPr, 'D',  2 },
        //      {"ConShpAlmPr001_indpvHighHigh", fnShpAlmPr, 'D',  3 },
        //      {"ConShpAlmPr001_indpvLowLow", fnShpAlmPr, 'D',  4 },

        //      // Комбобоксы на групповых мнемосхемах
        //      {"cboName"            , fnCboSPAN , 'S',  0 },
        //      {"cboView"            , fnCboSPAN , 'S',  1 },
        //      // Faceplate
        //      {"hscgroupfaceplate1" , fnCboFace , 'D',  0 },
        //      {"hscgroupfaceplate2" , fnCboFace , 'D',  1 },
        //      {"hscgroupfaceplate3" , fnCboFace , 'D',  2 },
        //      {"hscgroupfaceplate4" , fnCboFace , 'D',  3 },
        //      {"hscgroupfaceplate5" , fnCboFace , 'D',  4 },
        //      {"hscgroupfaceplate6" , fnCboFace , 'D',  5 },
        //      {"hscgroupfaceplate7" , fnCboFace , 'D',  6 },
        //      {"hscgroupfaceplate8" , fnCboFace , 'D',  7 },
        //      {"AlarmState001"      , fnAlarmSt , 'D',  0 },
        //      {"ConAlpHiAlmTypeDACA", fnAlarmTxt, 'T',  0 },
        //      {"ConAlpHiAlmTypePIDA", fnAlarmTxt, 'T',  1 },

        //      {"ConShpLstatus1"      , fnConShpLstatus, 'D', 1 },
        //      {"ConShpLstatus2"      , fnConShpLstatus, 'D', 2 },
        //      {"ConShpLstatus3"      , fnConShpLstatus, 'D', 3 },
        //      {"ConShpLstatus4"      , fnConShpLstatus, 'D', 4 },
        //      {"ConShpLstatus5"      , fnConShpLstatus, 'D', 5 },
        //      {"ConShpLstatus6"      , fnConShpLstatus, 'D', 6 },
        //      // Детальные окна
        //      {"grpFaceplate"       , fnCboFace , 'D',  9 },
        //      {"grpfacelpate"       , fnCboFace , 'D',  9 },// грамотеи
        //      {"AXFacePlate"        , fnCboFace , 'D',  9 },
        //      {"activexdoc001"      , fnCboFace , 'D',  9 },
        //      {"activexdoc002"      , fnCboFace , 'D',  9 },
        //      {"Conactivexdoc"      , fnCboFace , 'D',  9 },

        //      {"AreaPane"           , fnAreaPane, 'D',  0 },

        //      {"CM_FB_C12INVERT"    , fnCM_FB_CXXINVERT , 'T',  12 },
        //      {"CM_FB_C11INVERT"    , fnCM_FB_CXXINVERT , 'T',  11 },
        //      {"CM_FB_C10INVERT"    , fnCM_FB_CXXINVERT , 'T',  10 },
        //      {"CM_FB_C9INVERT"     , fnCM_FB_CXXINVERT , 'T',   9 },
        //      {"CM_FB_C8INVERT"     , fnCM_FB_CXXINVERT , 'T',   8 },
        //      {"CM_FB_C7INVERT"     , fnCM_FB_CXXINVERT , 'T',   7 },
        //      {"CM_FB_C6INVERT"     , fnCM_FB_CXXINVERT , 'T',   6 },
        //      {"CM_FB_C5INVERT"     , fnCM_FB_CXXINVERT , 'T',   5 },
        //      {"CM_FB_C4INVERT"     , fnCM_FB_CXXINVERT , 'T',   4 },
        //      {"CM_FB_C3INVERT"     , fnCM_FB_CXXINVERT , 'T',   3 },
        //      {"CM_FB_C2INVERT"     , fnCM_FB_CXXINVERT , 'T',   2 },
        //      {"CM_FB_C1INVERT"     , fnCM_FB_CXXINVERT , 'T',   1 },

        //      {"CM_FB_G3NUMINPUTS"  , fnCM_FB_GXNUMINPUTS , 'T',   3 },
        //      {"CM_FB_G3FIRSTCOND"  , fnCM_FB_GXFIRSTCOND , 'T',   3 },
        //      {"CM_FB_G2NUMINPUTS"  , fnCM_FB_GXNUMINPUTS , 'T',   2 },
        //      {"CM_FB_G2FIRSTCOND"  , fnCM_FB_GXFIRSTCOND , 'T',   2 },
        //      {"CM_FB_G1NUMINPUTS"  , fnCM_FB_GXNUMINPUTS , 'T',   1 },
        //      {"CM_FB_G1FIRSTCOND"  , fnCM_FB_GXFIRSTCOND , 'T',   1 },

        //      {"CM_FB_G1SO"         , fnCM_FB_GXSO        , 'T',   1 },
        //      {"CM_FB_G2SO"         , fnCM_FB_GXSO        , 'T',   2 },
        //      {"CM_FB_G3SO"         , fnCM_FB_GXSO        , 'T',   3 },
        //      {"CM_FB_G4SOFINAL"    , fnCM_FB_GXSO        , 'T',   4 },

        //      {"CM_FB_UPDETECTED12" , fnCM_FB_UPDETECTED  , 'T',   12 },
        //      {"CM_FB_UPDETECTED11" , fnCM_FB_UPDETECTED  , 'T',   11 },
        //      {"CM_FB_UPDETECTED10" , fnCM_FB_UPDETECTED  , 'T',   10 },
        //      {"CM_FB_UPDETECTED9"  , fnCM_FB_UPDETECTED  , 'T',   9 },
        //      {"CM_FB_UPDETECTED8"  , fnCM_FB_UPDETECTED  , 'T',   8 },
        //      {"CM_FB_UPDETECTED7"  , fnCM_FB_UPDETECTED  , 'T',   7 },
        //      {"CM_FB_UPDETECTED6"  , fnCM_FB_UPDETECTED  , 'T',   6 },
        //      {"CM_FB_UPDETECTED5"  , fnCM_FB_UPDETECTED  , 'T',   5 },
        //      {"CM_FB_UPDETECTED4"  , fnCM_FB_UPDETECTED  , 'T',   4 },
        //      {"CM_FB_UPDETECTED3"  , fnCM_FB_UPDETECTED  , 'T',   3 },
        //      {"CM_FB_UPDETECTED2"  , fnCM_FB_UPDETECTED  , 'T',   2 },
        //      {"CM_FB_UPDETECTED1"  , fnCM_FB_UPDETECTED  , 'T',   1 },

        //      {"CM_FB_FIRSTUP12"   , fnCM_FB_FIRSTUP      , 'T',   12 },
        //      {"CM_FB_FIRSTUP11"   , fnCM_FB_FIRSTUP      , 'T',   11 },
        //      {"CM_FB_FIRSTUP10"   , fnCM_FB_FIRSTUP      , 'T',   10 },
        //      {"CM_FB_FIRSTUP9"    , fnCM_FB_FIRSTUP      , 'T',   9  },
        //      {"CM_FB_FIRSTUP8"    , fnCM_FB_FIRSTUP      , 'T',   8  },
        //      {"CM_FB_FIRSTUP7"    , fnCM_FB_FIRSTUP      , 'T',   7  },
        //      {"CM_FB_FIRSTUP6"    , fnCM_FB_FIRSTUP      , 'T',   6  },
        //      {"CM_FB_FIRSTUP5"    , fnCM_FB_FIRSTUP      , 'T',   5  },
        //      {"CM_FB_FIRSTUP4"    , fnCM_FB_FIRSTUP      , 'T',   4  },
        //      {"CM_FB_FIRSTUP3"    , fnCM_FB_FIRSTUP      , 'T',   3  },
        //      {"CM_FB_FIRSTUP2"    , fnCM_FB_FIRSTUP      , 'T',   2  },
        //      {"CM_FB_FIRSTUP1"    , fnCM_FB_FIRSTUP      , 'T',   1  },

        //      {"CM_FB_C12SO"       , fnCM_FB_CXXSO        , 'T',   12 },
        //      {"CM_FB_C11SO"       , fnCM_FB_CXXSO        , 'T',   11 },
        //      {"CM_FB_C10SO"       , fnCM_FB_CXXSO        , 'T',   10 },
        //      {"CM_FB_C9SO"        , fnCM_FB_CXXSO        , 'T',   9 },
        //      {"CM_FB_C8SO"        , fnCM_FB_CXXSO        , 'T',   8 },
        //      {"CM_FB_C7SO"        , fnCM_FB_CXXSO        , 'T',   7 },
        //      {"CM_FB_C6SO"        , fnCM_FB_CXXSO        , 'T',   6 },
        //      {"CM_FB_C5SO"        , fnCM_FB_CXXSO        , 'T',   5 },
        //      {"CM_FB_C4SO"        , fnCM_FB_CXXSO        , 'T',   4 },
        //      {"CM_FB_C3SO"        , fnCM_FB_CXXSO        , 'T',   3 },
        //      {"CM_FB_C2SO"        , fnCM_FB_CXXSO        , 'T',   2 },
        //      {"CM_FB_C1SO"        , fnCM_FB_CXXSO        , 'T',   1 },

        //      {"CM_FB_C12FL"       , fnCM_FB_CXXFL        , 'T',   12 },
        //      {"CM_FB_C11FL"       , fnCM_FB_CXXFL        , 'T',   11 },
        //      {"CM_FB_C10FL"       , fnCM_FB_CXXFL        , 'T',   10 },
        //      {"CM_FB_C9FL"        , fnCM_FB_CXXFL        , 'T',   9 },
        //      {"CM_FB_C8FL"        , fnCM_FB_CXXFL        , 'T',   8 },
        //      {"CM_FB_C7FL"        , fnCM_FB_CXXFL        , 'T',   7 },
        //      {"CM_FB_C6FL"        , fnCM_FB_CXXFL        , 'T',   6 },
        //      {"CM_FB_C5FL"        , fnCM_FB_CXXFL        , 'T',   5 },
        //      {"CM_FB_C4FL"        , fnCM_FB_CXXFL        , 'T',   4 },
        //      {"CM_FB_C3FL"        , fnCM_FB_CXXFL        , 'T',   3 },
        //      {"CM_FB_C2FL"        , fnCM_FB_CXXFL        , 'T',   2 },
        //      {"CM_FB_C1FL"        , fnCM_FB_CXXFL        , 'T',   1 },

        //      {"CM_FB_C12BYPREQ"   , fnCM_FB_CXXByPreq    , 'T',   12 },
        //      {"CM_FB_C11BYPREQ"   , fnCM_FB_CXXByPreq    , 'T',   11 },
        //      {"CM_FB_C10BYPREQ"   , fnCM_FB_CXXByPreq    , 'T',   10 },
        //      {"CM_FB_C9BYPREQ"    , fnCM_FB_CXXByPreq    , 'T',   9 },
        //      {"CM_FB_C8BYPREQ"    , fnCM_FB_CXXByPreq    , 'T',   8 },
        //      {"CM_FB_C7BYPREQ"    , fnCM_FB_CXXByPreq    , 'T',   7 },
        //      {"CM_FB_C6BYPREQ"    , fnCM_FB_CXXByPreq    , 'T',   6 },
        //      {"CM_FB_C5BYPREQ"    , fnCM_FB_CXXByPreq    , 'T',   5 },
        //      {"CM_FB_C4BYPREQ"    , fnCM_FB_CXXByPreq    , 'T',   4 },
        //      {"CM_FB_C3BYPREQ"    , fnCM_FB_CXXByPreq    , 'T',   3 },
        //      {"CM_FB_C2BYPREQ"    , fnCM_FB_CXXByPreq    , 'T',   2 },
        //      {"CM_FB_C1BYPREQ"    , fnCM_FB_CXXByPreq    , 'T',   1 },

        //      {"CM_FB_G4ALGID"     , fnCM_FB_GXAlgId      , 'T',   4 },
        //      {"CM_FB_G3ALGID"     , fnCM_FB_GXAlgId      , 'T',   3 },
        //      {"CM_FB_G2ALGID"     , fnCM_FB_GXAlgId      , 'T',   2 },
        //      {"CM_FB_G1ALGID"     , fnCM_FB_GXAlgId      , 'T',   1 },

        //      {"al_combobox"       , fnNotBindedCombo     , 'S',   -2 },
        //      {"Bypass_Combo12"    , fnNotBindedCombo     , 'S',   12 },
        //      {"Bypass_Combo11"    , fnNotBindedCombo     , 'S',   11 },
        //      {"Bypass_Combo10"    , fnNotBindedCombo     , 'S',   10 },
        //      {"Bypass_Combo9"     , fnNotBindedCombo     , 'S',   9 },
        //      {"Bypass_Combo8"     , fnNotBindedCombo     , 'S',   8 },
        //      {"Bypass_Combo7"     , fnNotBindedCombo     , 'S',   7 },
        //      {"Bypass_Combo6"     , fnNotBindedCombo     , 'S',   6 },
        //      {"Bypass_Combo5"     , fnNotBindedCombo     , 'S',   5 },
        //      {"Bypass_Combo4"     , fnNotBindedCombo     , 'S',   4 },
        //      {"Bypass_Combo3"     , fnNotBindedCombo     , 'S',   3 },
        //      {"Bypass_Combo2"     , fnNotBindedCombo     , 'S',   2 },
        //      {"Bypass_Combo1"     , fnNotBindedCombo     , 'S',   1 },
        //      {"FinalBypass_Combo" , fnNotBindedCombo     , 'S',   -1 },

        //      {"CM_FB_C12BYPPERM"  , fnCM_FB_CXXByPerm    , 'T',   12 },
        //      {"CM_FB_C11BYPPERM"  , fnCM_FB_CXXByPerm    , 'T',   11 },
        //      {"CM_FB_C10BYPPERM"  , fnCM_FB_CXXByPerm    , 'T',   10 },
        //      {"CM_FB_C9BYPPERM"   , fnCM_FB_CXXByPerm    , 'T',   9 },
        //      {"CM_FB_C8BYPPERM"   , fnCM_FB_CXXByPerm    , 'T',   8 },
        //      {"CM_FB_C7BYPPERM"   , fnCM_FB_CXXByPerm    , 'T',   7 },
        //      {"CM_FB_C6BYPPERM"   , fnCM_FB_CXXByPerm    , 'T',   6 },
        //      {"CM_FB_C5BYPPERM"   , fnCM_FB_CXXByPerm    , 'T',   5 },
        //      {"CM_FB_C4BYPPERM"   , fnCM_FB_CXXByPerm    , 'T',   4 },
        //      {"CM_FB_C3BYPPERM"   , fnCM_FB_CXXByPerm    , 'T',   3 },
        //      {"CM_FB_C2BYPPERM"   , fnCM_FB_CXXByPerm    , 'T',   2 },
        //      {"CM_FB_C1BYPPERM"   , fnCM_FB_CXXByPerm    , 'T',   1 },


        //      }InitLocal;
        //
        char* id = FindTagItem( teg, "id" );
    if ( id != NULL )
    {
        // SLocal* find = FindLocal(id);
        // if ( find != NULL )
        //     if ( !AS( find->ID == ID ) )// На всякий случай
        //     {
        //         LPFuncsEx func = find->func;
        //         if(LG35_8_KF==Version || fnConShpLstatus != find->func)
        //             if(BERTH_ONPZ!=Version || (html.m_strFile.CompareNoCase("sysdtlSMdevctla_3SVLV_fp") != 0 &&  html.m_strFile.CompareNoCase("sysdtlSMdevctla_PUMP_fp") != 0)
        //                 || (fnShpGOPSt != find->func && fnShpGPVSt != find->func))
        //                 return (*find->func)(find->type);
        //     }
    }
    return (*((LPFuncs)func))();
}
//
bool CTag::Parser( CHTML& html, char* szBuffer, char*& ptr, CBin* bin, SCRIPT_SOURSE_DATA *pSriptData, CTag **pTag )
{
    struct SLocal { char* name; LPFuncs func; bool bClose; };
    _static SLocal list[] =
        {
         // { "!DOCTYPE", fnDOCTYPE , false },
         // { "HTML"    , fnHTML    , true  },
         // { "HEAD"    , fnHEAD    , true  },
         // { "TITLE"   , fnTITLE   , true  },
         // { "META"    , fnMETA    , false },
         // { "LINK"    , fnLINK    , false },
         // { "PARAM"   , fnPARAM   , false },
         // { "STYLE"   , fnSTYLE   , true  },
         // { "OBJECT"  , fnOBJECT  , true  },
         // { "SCRIPT"  , fnSCRIPT  , true  },
         // { "BODY"    , fnBODY    , true  },
         // { "SPAN"    , fnSPAN    , true  },
         { "DIV"     , fnDIV     , true  },
         // { "A"       , fnA       , true  },
         // { "P"       , fnP       , true  },
         // { "BR"      , fnBR      , false },
         // { "XML"     , fnXML     , true  },
         // { "IMG"     , fnIMG     , false },
         // { "ITEM"    , fnITEM    , true  },
         // { "INPUT"   , fnINPUT   , false },
         // { "TEXTAREA", fnTEXTAREA, true  },
         // { "TR"      , fnTR      , true  },
         // { "TD"      , fnTD      , true  },
         // { "TABLE"   , fnTABLE   , true  },
         // { "TBODY"   , fnTBODY   , true  },
         // { "IFRAME"  , fnIFRAME  , true  },
         // { "Detail"  , fnDetail  , true  },
         // { "COL"     , fnCOL     , true  },
         // { "v:roundcross::RECT",fnv_roundcross::RECT,true},
         // { "hn-alarm-tracker", hn_alarm_tracker, true},
         }InitLocal;
    //
    while ( *ptr )
    {
        if(Version >= RCOL_SI)
        {
            if (strncmp(ptr, "<BR>", _countof("<BR>")-1)==0)
            {
                ptr += _countof("<BR>")-2; //это просто перевод строки - для него не нужно создавать отдельный тег, т.к за ним может быть нужный текст текущего тега
                *ptr = '\n';
            }
        }
        CTag* obj = NULL;
        DWORD dwShiftPTR = DWORD(DWORD_PTR(ptr)-DWORD_PTR(szBuffer));
        int nReturn;
        STegScn* teg = theApp.ParsXML( ptr, nReturn );
        if ( nReturn > 0 ) //Все нормально
        {
            if ( *teg[0].name == '/' )
            {
                const char* dd = GetName (html);
                const char* ff = GetValue(html);
                ASSERT( IsYou( html, teg[0].name+1 ) );
                return true;
            }
            else
            {
                SLocal* find = FindLocal( teg[0].name );
                if ( find )
                {
                    if (( html.m_bR300 )||(html.GetPopup()))
                    {/*
                        if ( lstrcmpi(teg[0].name,"OBJECT") == 0 )
                            if ( lstrcmpi(teg[1].name,"class") == 0 )
                                if ( lstrcmpi(teg[1].value,"hsc.trend.1") == 0 )*/
                        //  KKK();
                        if ( lstrcmpi(teg[0].name,"DIV") == 0 ){
                            obj = AnalizSpecific( html, teg+1, 'D', find->func );
                            if (!strcmp((teg+1)->value, "AreaPane"))
                                html.m_pAreaPane = obj;
                        }
                        else
                            if ( lstrcmpi(teg[0].name,"SPAN") == 0 )
                                obj = AnalizSpecific( html, teg+1, 'S', find->func );
                            else
                                if ( lstrcmpi(teg[0].name,"TEXTAREA") == 0 )
                                    obj = AnalizSpecific( html, teg+1, 'T', find->func );
                    }
                    //
                    if ( obj == NULL )
                        obj = (*find->func)();
                }
                else
                {
                    if ( lstrcmpi( teg[0].name, "!--" ) == 0 )
                    {
                        /*
                    char* p = strstr( ptr, "-->" );
                    ASSERT( p != NULL );
                    if ( p == NULL ) return false;
                    ptr = p+3;*/
                        continue;
                    }
                    if ( lstrcmpi( teg[0].name, "![CDATA[" ) == 0 )
                    {
                        char* p = strstr( ptr, "]]>" );
                        ASSERT( p != NULL );
                        if ( p == NULL )
                            return false;
                        ptr = p+3;
                        continue;
                    }
                    ASSERT(0);
                    return false;
                }
                ASSERT( obj != NULL );
                //
                obj->InitStyle(html);
                //
                CStyle& style = obj->Style(html);
                // if ( ::IsMnemoStyle(MNEMO_EDIT) && nReturn > 1 )
                //     style.AddItems( html, nReturn-1, teg+1 );

                if(strncmp(ptr, "Combobox</SPAN>", sizeof("Combobox</SPAN>")-1)==0)
                {
                    if(style.mClass == clsUnknown)
                    {
                        //what is  clsComboBox???
                 //       style.mClass = clsComboBox;
                        ptr += sizeof("Combobox")-1;
                    }
                }

                style.SetLastName( html, find->name );
                //
                obj->m_pPar = this;
                //      obj->m_nNumber = m_nNumber;
                obj->Analiz( html, teg+1 );
                //    Add( obj );// Перенёс из-за expression
                //
                if(pTag && *pTag == NULL)
                    *pTag = obj;
                //
                CStyle& styl = obj->Style(html);
                //
                if ( styl.m_dwFlags & HDX_BINDING )
                {
                    if ( styl.m_nHDXBINDINGID >= 0 )
                        if ( styl.m_nPlaceDsd == -1 && bin )
                        {
#ifdef _DEBUG
                            char* id = styl.NameID( html );
                            if(strcmp(id, "shape072_ValveVerh")==0)
                                AAA();
#endif
                            // CDsd* dsd = bin->FindDSO( html, styl.m_nHDXBINDINGID, styl.m_bReadOnly, styl.m_bParameterFormat);
                            // if ( dsd != NULL )
                            //     styl.m_nPlaceDsd = dsd->Save(html);
                            // else
                            //     styl.m_nPlaceDsd = -2;
                        }
                }
                //
                if (Version >= L35_11600_KF)
                {
                    if(typeid(*obj) == typeid(CTagDIV) && obj->m_constValue.IsEmpty())
                    {
                        char *pp = ptr;
                        while(1)
                        {
                            while(*pp == ' ' || *pp == '\xd' || *pp == '\xa') pp++;
                            if(_strnicmp(pp, "<P>", _countof("<P>")-1)!=0)
                            {
                                ptr = pp;
                                break;
                            }
                            {
                                if(char *pendp = strstr(pp + _countof("<P>")-1, "</P>"))
                                {
                                    *pendp = 0;
                                    //if(obj->m_constValue.IsEmpty())
                                    //	 {
                                    obj->m_constValue += pp + _countof("<P>")-1;
                                    obj->m_constValue.Replace("<P>", "");
                                    obj->m_constValue.Replace("\xD\xA", "");
                                    obj->m_constValue.Replace("&amp;", "&");
                                    /* obj->m_constValue.Replace("&nbsp;", " ");
                           obj->m_constValue.Replace("<BR>", "\n");*/
                                    // }
                                    pp = pendp + _countof("</P>");
                                    *pendp = '<';
                                }
                            }
                        }
                    }
                }
                //
                if ( find->bClose || obj->Close() )
                {
                    bool bShape = styl.IsShape();

                    bool bReturn  = obj->Parser( html, szBuffer, ptr, bin );
                    if ( !bReturn )
                        return false;
                }
                //      Add( obj );
                //
                if(obj->m_ScriptTextId!=-1)
                {
                    // CStyle& styl2 = obj->Style(html);
                    // char* id = styl2.NameID( html );
                    // CTagSCRIPT *pTagSCRIPT = new CTagSCRIPT();
                    // pTagSCRIPT->m_eType = (EventType)obj->m_ScriptTexteType;
                    // pTagSCRIPT->m_eLang = langVBScript;
                    // pTagSCRIPT->InitStyle(html);
                    // CStyle& sstyle = pTagSCRIPT->Style(html);
                    // sstyle.SetLastName( html, "SCRIPT");
                    // sstyle.NameID( html, id );
                    // char *pValue = html.Buffer(obj->m_ScriptTextId);
                    // sstyle.SetLastValue( html, pValue);
                    // Add(pTagSCRIPT);
                }

                // if(pSriptData && pSriptData->eType != eventUnk && pSriptData->m_ScriptText.size()>0)
                // {
                // CStyle& styl2 = obj->Style(html);
                // char* id = styl2.NameID( html );
                // CTagSCRIPT *pTagSCRIPT = new CTagSCRIPT();
                // pTagSCRIPT->m_eType = (EventType)pSriptData->eType;
                // pTagSCRIPT->m_eLang = langVBScript;
                // pTagSCRIPT->InitStyle(html);
                // CStyle& sstyle = pTagSCRIPT->Style(html);
                // sstyle.SetLastName( html, "SCRIPT");
                // sstyle.NameID( html, id );
                // sstyle.SetLastValue( html, pSriptData->m_ScriptText.c_str());
                // pTagSCRIPT->m_pLinkS = obj;
                // Add(pTagSCRIPT);
                // pSriptData = NULL;
                //}
                //
            }
        }
        else
            if ( nReturn == -1 ) //Текстовое оформление
            {
                cross::CString str = "</"; str += teg[0].name;
                if (str == cross::CString("</BR") && !strncmp(ptr, "Loading....Please", 17))
                    str = "</DIV";
                char* p = strstr( teg[0].value, str );
                ASSERT( p );
                if ( p == 0 )
                    return false;
                str = "<"; str += teg[0].name;
                char* r = strstr( teg[0].value, str );
                if ( r != 0 )
                {
                    if ( DWORD_PTR(p) > DWORD_PTR(r) )
                    {
                        p = strstr( teg[0].value, "<" );
                        *p=0;
                    }
                }
                //
                if ( !html.m_bBinData )
                {
                    CStyle& style = Style(html);
                    *p = 0;
                    //
                    cross::CString out;
                    //const char* name = GetName (html); т.к. указатель может поменятся при перераспределении памяти под тексты
                    cross::CString name = GetName (html);
                    if ( lstrcmpi( name, "SCRIPT" ) == 0 || lstrcmpi( name, "Detail" ) == 0 )
                        out = teg[0].value;
                    else
                        AnalizText( out, teg[0].value );
                    char* b = out.GetBuffer();
                    int dl = lstrlen(b)-1;
                    while ( dl > 0 && b[dl] == ' ' ) b[dl--] = 0;
                    style.SetLastValue( html, out );
                    char* id = style.NameID( html );
                    if ( lstrcmpi( name, "SCRIPT" ) == 0 &&  lstrcmpi( id, "" ) == 0 )
                    {
                        if(LG35_8_KF!=Version)
                        {
                            html.GetSubroutines(out);
                            if(Version>=VSB_KF)
                            {
                                std::map<std::string, SUB_STRUCT>::iterator it = html.m_Subroutines.find("page_onClick");
                                if(it!=html.m_Subroutines.end())
                                {
                                    // char* id = "Page";
                                    // CTagSCRIPT *pTagSCRIPT = new CTagSCRIPT();
                                    // pTagSCRIPT->m_eType = eventOnClick;
                                    // pTagSCRIPT->m_eLang = langVBScript;
                                    // pTagSCRIPT->InitStyle(html);
                                    // CStyle& sstyle = pTagSCRIPT->Style(html);
                                    // sstyle.SetLastName( html, "SCRIPT");
                                    // sstyle.NameID( html, id );
                                    // const char *pValue = it->second.body.c_str();
                                    // sstyle.SetLastValue( html, pValue);
                                    // Add(pTagSCRIPT);
                                }
                            }
                        }
                    }
                    *p = '<';
                }
                //
                ptr = p;
            }
            else
                if ( nReturn == 0 ) //Конец документа
                {
                    return true;
                }
                else //Ошибка
                {
                    ASSERT(0);
                    return false;
                }
    }
    return true;
}
//
void CTag::InitStyle( CHTML& html )
{
    m_nStyle = html.AddStyle();
    if ( !html.m_bBinData )
    {
        //if ( m_nStyle == 0 ) return;
        CStyle  style;// Чтобы отработал конструктор по умолчанию
        CStyle& obj = html.Style(m_nStyle);
        memcpy( &obj, &style, sizeof(style) );
    }
}
//
// void CTag::DlgDebug( CDlgDebug& dlg, POINT& pt, int nPaint )
// {
//     CStyle& style = Style(dlg.m_pHtml);
//     //if ( !( style.m_dwTypeObj & (TYPE_BACKGROUND | TYPE_PAGE)) )
//     {
//         cross::RECT tmpcross::RECT = style.m_rcTruecross::RECT;
//         if (TYPE_PNT == TYPE_LINE)
//             ::Inflatecross::RECT( &tmpcross::RECT, 2, 2);
//         if ( ::PtIncross::RECT( &tmpcross::RECT, pt ) || nPaint == 2 )
//         {
//             ASSERT( dlg.m_nCount < MAX_COMBO );
//             dlg.arrTag[dlg.m_nCount++] = this;
//             const char* name  = style.GetName (dlg.m_pHtml);
//             if ( lstrcmpi( name, "DIV" ) == 0 )
//             {
//                 CTagDIV* ss = (CTagDIV*)this;
//                 if ( ss->m_numberOfShapesAnimated >= 2 )
//                     nPaint = 2;
//             }
//         }
//     }
//     //
//     INT_PTR nCount = GetSize();
//     CTag**     obj = GetData();
//     for ( INT_PTR n = 0; n < nCount; n++ )
//         obj[n]->DlgDebug( dlg, pt, nPaint );
// }

void CTag::SetMVal( CHTML& html, cross::SParamValueH val )
{
    if ( mVal.nNumber == -1 )
        mVal = val;
    INT_PTR nCount = GetSize();
    CTag**     obj = GetData();
    for ( INT_PTR n = 0; n < nCount; n++ )
        obj[n]->SetMVal( html, val );
}

void CTag::BadRect( HDC hDC, CHTML& html, COLORREF color )
{
    // if ( ::IsMnemoStyle(MNEMO_EDIT) )
    // {
    //     cross::RECT cross::RECT = Style(html).Fullcross::RECT();
    //     CMyBrush brush( hDC, color );
    //     brush.cross::RECT( cross::RECT );
    // }
}
//
void CTag::SetColor( CHTML& html, COLORREF color )
{
    CStyle& style = Style(html);
    switch ( TYPE_PNT )
    {
    case TYPE_RECT      :
    case TYPE_OVAL      :
#if 0
      if ( style.m_nFillStyle == 0 )
        style.m_clrFill = color;
      else
        style.m_clrLine = color;
#else
        if ( style.m_nFillStyle == 1 )
            style.m_nFillStyle = 0;
        style.m_clrFill = color, style.m_dwFlags |= HDX_CLRFILL;
        if ( style.m_dwFlags & HDX_CLRLINE )
        {
            if ((style.m_clrLine & 0xff000000) > 0)
                style.m_clrLine = color;
        }
        else
            if (Version == AM2_NAK && style.m_dwFlags & HDX_ALMSTATE )
            {  //т.к в этом случае, сли нет аларма цвет возьмется отсюда
                style.m_clrFillEmpty = style.m_clrFill;
            }
#endif
        break;
    case TYPE_ARC       :
    case TYPE_LINE      :
    case TYPE_BEZIER    :
    case TYPE_POLYLINE  : style.m_clrLine = color; break;
    case TYPE_ROUNDRECT :
    case TYPE_POLYGON   :
    case TYPE_10        :
    case TYPE_WEDGE     : style.m_dwFlags |= HDX_CLRFILL; style.m_clrFill = color; break;
    case TYPE_TEXT      : style.m_clrText = color; break;
    case TYPE_GROUP     : if(Version >= L35_11600_KF)
        {
            style.m_clrFill = color;
            style.m_bChangedByData = true;
        }
        break;
    }
}

bool CTag::OutDbl( CHTML& html, double val, char* str )
{
    CStyle& style = Style(html);
    style.m_dValue = val;
    if ( TYPE_PNT == TYPE_TEXT )
    {
        if(Version == AM2_NAK)
        {
            // char* filtr = style.Filtr(html);
            // if(!filtr[0] && m_nControllerDigits==-1 && !m_constValue.IsEmpty())
            // {
            //     lstrcpy( str,  m_constValue);
            //     return true;
            // }
        }
        // if ( IsNaN(val) )
        //     lstrcpy( str, "NaN" );
        // else
        ////  if ( style.mClass == clsPV )
        //   {
        // char* filtr = style.Filtr(html);
        // BYTE dbl = html.pipe.ValueB_H( mDbl );
        // if ( dbl < 8 )
        // {
        //     char flt[8] = "%.0f"; flt[2] += dbl;
        //     sprintf_s( str, 512, flt, val );
        // }
        // else
        // {
        //     sprintf_s( str, 512, style.Filtr(html), val );
        // }
        //  }
        //  else
        // if ( style.mClass == clsConAlpOP )
        // {
        //     char flt[8] = "%.0f%%";
        //     sprintf_s( str, 512, flt, val );
        // }
        // else
        // {
        //     if (Version != LG35_8_KF)
        //     {
        //         if(m_nControllerDigits != -1)
        //         {
        //             switch(m_nControllerDigits)
        //             {
        //             case 0:  sprintf_s( str, 512, "%.0f", val ); break;
        //             case 1:  sprintf_s( str, 512, "%.1f", val ); break;
        //             case 2:  sprintf_s( str, 512, "%.2f", val ); break;
        //             case 3:  sprintf_s( str, 512, "%.3f", val ); break;
        //             }
        //         }
        //     }

        //     if(m_nControllerDigits == -1)
        //     {
        //         const char* filtr = Alias( html, this, "DigitsAfterDot" );
        //         if ( lstrcmpi( filtr, "DigitsAfterDot" ) == 0 )
        //             filtr = style.Filtr(html);
        //         if (*m_szNumericDisplayValue)
        //             filtr=m_szNumericDisplayValue;
        //         if ( lstrcmpi( filtr, "%s" ) == 0 )
        //             sprintf_s( str, 512, "%.2f", val );
        //         else
        //         {
        //             if (lstrcmpi( filtr, "%6.1" ) == 0)
        //             {
        //                 char filtr_[8] = "%6.1f";
        //                 sprintf_s( str, 512, filtr_ , val );
        //             }
        //             else
        //             {
        //                 if (Version != LG35_8_KF)
        //                 {// пока так - нужно обработать скрипты устаналивающие правильное значение
        //                     if(_stricmp(filtr, "%.256f")==0)
        //                         filtr = "%.3f";
        //                 }
        //                 char bfiltr[32];
        //                 strcpy_s( bfiltr, filtr);
        //                 char *pp1 = strchr(bfiltr, '.');
        //                 if(pp1)
        //                 {
        //                     char *pp2 = strchr(pp1+1, '.');
        //                     if (pp2)
        //                     {   //из-за того что me.value - double - скрипт sFormat="%." & me.value & "f" может добавить лишнюю точку
        //                         size_t len = strlen(bfiltr);
        //                         *pp2++ = bfiltr[len-1];
        //                         *pp2=0;
        //                     }
        //                 }
        //                 sprintf_s( str, 512, bfiltr , val );
        //             }
        //         }
        //     }
        //   }
    }
    //
    if (Version==Реформинг_КНПЗ)
    {
        if (SetColorForSpecClass(html, style.mClass, val))
            return true;
    }

    if ( style.m_nBreakpointType == 1 )
    {
        if (Version!=LG35_8_KF)
        {
            if(style.m_dwFlags & HDX_CLRTEXT)
                return true;

            if(style.m_bClrFillCanChangedByScript)
            {
                return style.m_clrFill != 0xFF000000;
            }

            if( style.m_nDiscreteColors != 0)
            {
                COLORREF color = 0xFF000000;
                int ival = 0;
                if(IsNaN(val))
                    ival = 7;
                else if (val < 0.5)
                    ival = 0;
                else
                {
                    ival = int(val + 0.5)%8;
                }
                // color = style.DiscColor(html, ival);
                // SetColor(html,color);
                // return color != 0xFF000000;
            }

        }
        return false;
    }
    if ( style.m_nBreakpointType != 0 ) return true;
    //
    if (Version!=LG35_8_KF)
    {
        if(style.m_dwFlags & HDX_CLRTEXT)
            return true;
    }
    //
    // COLORREF color = style.Con_Color(html,val);
    // SetColor(html,color);
    // return color != 0xFF000000;
}
//
bool CTag::SetColorForSpecClass(CHTML& html, int nStyleClass, double dValue)
{
    CStyle& style = Style(html);
    // if (( style.mClass == clsPlgIn_RegValve_Close )||( style.mClass == clsPlgOut_RegValve_Close ))
    // //бантик регулятора
    // {
    //     if (dValue<=5)
    //         SetColor(html,0x0000ff);//red
    //     else
    //         SetColor(html,0x00ff00);//green
    //     return true;
    // }
    return false;
}

bool CTag::SetColorForSpecClass(CHTML& html, int nStyleClass, BYTE bValue)
{
    CStyle& style = Style(html);
    // if (( style.mClass == clsOvlBody_Pump_Stop )||( style.mClass == clsOvlBody_Blower_Stop )||//насос и аво devctla.gpv
    //     ( style.mClass == clsPlgOut_MOV_Close )||( style.mClass == clsPlgIn_MOV_Close ))//задвижка ZV devctla.gpv
    // {
    //     if (bValue==5)
    //         SetColor(html,0x00ff00);//green
    //     else
    //         SetColor(html,0x0000ff);//red
    //     return true;
    // }
    // else
    //     if (style.mClass ==clsWdgC_MOV_Close)//буква M на задвижке devctla.gop
    //     {
    //         SetColor(html,0xc0c0c0);//серый //MIHAIL M на задвижке пока всегда крашу серым
    //         return true;
    //     }
    //     else
    //         if (style.mClass ==clsRecAlm_MOS_off)//,блокировка
    //         {
    //             CStyle& style = Style(html);
    //             if (bValue==1)
    //             {
    //                 SetColor(html,0x808080);
    //                 style.m_clrLine=0;//Деблок темнее
    //             }
    //             else
    //             {
    //                 SetColor(html,0xdddddd);//Норма светлее
    //                 style.m_clrLine=0x9c9c9c;
    //             }
    //             return true;
    //         }
    //         else
    //             if (( style.mClass == clsPlgIn_OnOffValve_Close )||( style.mClass == clsPlgOut_OnOffValve_Close ))//клапан HV devctla.gpv
    //             {
    //                 bool bGreen=false;
    //                 // bool bS1IsOpen=IsS1Open_DEVCTLA(html,  mVal);
    //                 // if (bS1IsOpen)
    //                 // {
    //                 //     if (bValue==5)
    //                 //         bGreen=true;//окраска в зеленый по S1
    //                 // }
    //                 // else
    //                 // {
    //                 //     if (bValue==4)
    //                 //         bGreen=true;//окраска в зеленый по S0
    //                 // }
    //                 if (bGreen)
    //                     SetColor(html,0x00ff00);//green
    //                 else
    //                     SetColor(html,0x0000ff);//red
    //             }
    return false;
}

// bool CTag::IsS1Open_DEVCTLA(CHTML& html, SParamValueH& mVal)
// {
//     if( mVal.def != NULL )
//     {
//         if (mVal.def->dwLog==id_DEVCTL)
//         {
//             CShDEVCTL sh;
//             if ( html.pipe.GetData( mVal.nNumber, &sh ) )
//             {
//                 cross::CString strValue = sh.STATETEXT[5];
//                 strValue.MakeUpper();
//                 if (strValue=="ОТКР")
//                     return true;
//             }
//         }
//     }
//     return false;
// }


bool CTag::OutStr( CHTML& html, char* val, char* str  )
{
    CStyle& style = Style(html);
    if ( TYPE_PNT == 0 )
        style.AddFlag( TYPE_TEXT );
    //
    //char* filtr = style.Filtr(html);
    if ( TYPE_PNT == TYPE_TEXT
        //|| !*filtr
        )
    {
        if ( (Version==DOTF_PM) && (style.m_dwTypeObj & 0x01800000)==0) //для этих битов костантное значение на скришотах не меняется
            lstrcpy( str, m_constValue);
        else
            if((Version==L35_11600_KF) && m_nControllerDigits==-1 && !m_constValue.IsEmpty())
                lstrcpy( str, m_constValue);
            else
                lstrcpy( str, val ); //так было
    }
    else
    {
        ASSUNO;
        //        sprintf_s( str, 512, filtr, val );
    }
    return true;
}
//
bool CTag::OutChr( CHTML& html, BYTE val, char* str )
{
    CStyle& style = Style(html);
    if ( TYPE_PNT == TYPE_TEXT )
    {
        // if ( style.mClass == clsPV )
        // {
        //     if ( val == 0 )
        //         lstrcpy( str, Alias( html, this, "DescState0" ) );
        //     else
        //         if ( val == 1 )
        //             lstrcpy( str, Alias( html, this, "DescState1" ) );
        //         else
        //             ASSUNO
        // }
        // else
        //     if ( style.mClass == clsAlarmType )
        //     {
        //         const char* text = ::EnumStrH( *mVal.def, val );
        //         if ( val == 2 )// Normal
        //             lstrcpy( str, "" );
        //         else
        //             if ( text != NULL )
        //                 lstrcpy( str, text );
        //             else
        //                 ASSUNO
        //     }
        //     else
        //         if ( style.mClass == clsTxtMode )
        //         {
        //             const char* text = ::EnumStrH( *mVal.def, val );
        //             if ( text != NULL )
        //             {
        //                 lstrcpy( str, text);
        //                 str[1]='\0';
        //             }
        //             else
        //                 ASSUNO
        //         }
        //         else
        //         {
        //             char* filtr = style.Filtr(html);
        //             if ( lstrcmpi( filtr, "%s" ) )
        //             {
        //                 if ( *filtr )
        //                 {
        //                     if ( (Version<DOTF_PM) && mVal.def->dwLog && (style.m_dwTypeObj & TYPE_BREAKPOINT) == 0 )
        //                     {// D120
        //                         if ( val == 0 )
        //                             lstrcpy( str, "OFF" );
        //                         else
        //                             lstrcpy( str, "ON" );
        //                     }
        //                     else
        //                         sprintf_s( str, 512, filtr, double(val) );
        //                 }
        //                 else
        //                     lstrcpy( str, m_constValue );
        //             }
        //             else if (Version == LG35_8_KF)
        //             {
        //                 // const char* name = html.pipe.GetMnemoName( mVal.nNumber );
        //                 // if(!strnicmp(name, "FIC2701.STATE", 13)
        //                 //     && !strnicmp(mVal.def->name, "PVFL", 4) && val == 1)
        //                 //     strcpy(str, shape_get_data_value_string(html, this, (char*)name, "STATETEXT[1]"));

        //                 if(!strcmp(mVal.def->name, "CEESTATE"))
        //                     strcpy(str, EnumStrH( *mVal.def, val ));

        //                 if(!strcmp(mVal.def->name, "MODE"))
        //                 {
        //                     switch(val)
        //                     {
        //                     case 3: case 5:
        //                         if(Version == L35_11600_KF)
        //                             break;
        //                     case 0: case 1: case 2: case 4:
        //                         strcpy(str, EnumStrH( *mVal.def, val ));
        //                         break;
        //                     }
        //                 }
        //                 if(!strcmp(mVal.def->name, "HIALM.TYPE"))
        //                 {
        //                     if ( mVal.def->dwFlag != 0 ){
        //                         const char* text = ::EnumStrH( *mVal.def, val );
        //                         if ( text != NULL )
        //                             lstrcpy( str, text );
        //                     }
        //                 }
        //             }
        //             else
        //                 switch ( mVal.def->dwLog )
        //                 {
        // case id_FLAG:
        // {
        //     CShFLAG* sh = (CShFLAG*)html.pipe.Sh(mVal.nNumber);
        //     val = max( val, 0 );
        //     val = min( val, 1 );
        //     if ( sh != NULL )
        //         lstrcpy( str, sh->STATETEXT[val] );
        // }
        // break;
        // case id_DEVCTL:
        // {
        //     CShDEVCTL* sh = (CShDEVCTL*)html.pipe.Sh(mVal.nNumber);
        //     val = max( val, 0 );
        //     val = min( val, 6 );
        //     if ( sh != NULL )
        //         lstrcpy( str, sh->STATETEXT[val] );
        // }
        // break;
        // case id_INTERLOCK:
        // {
        //     sprintf(str, "%d", html.pipe.ValueB_H(mVal)/*?"TRUE":"FALSE"*/);
        // }
        // break;
        // //MIHAIL_HWL нет такой структуры
        // //case id_ST:
        // //  {
        // //  CShST* sh = (CShST*)html.pipe.Sh(mVal.nNumber);
        // //  val = max( val, 0 );
        // //  val = min( val, 7 );
        // //  if ( sh != NULL )
        // //  switch ( val )
        // //    {
        // //    case 0:lstrcpy( str, sh->UNITS  );break;
        // //    case 1:lstrcpy( str, sh->UNITS1 );break;
        // //    case 2:lstrcpy( str, sh->UNITS2 );break;
        // //    case 3:lstrcpy( str, sh->UNITS3 );break;
        // //    case 4:lstrcpy( str, sh->UNITS4 );break;
        // //    case 5:lstrcpy( str, sh->UNITS5 );break;
        // //    case 6:lstrcpy( str, sh->UNITS6 );break;
        // //    case 7:lstrcpy( str, sh->UNITS7 );break;
        // //    }
        // //  }
        // //break;
        // case id_CONTROLMODULE:
        //     if(Version != LG35_8_KF)
        //     {
        //         const char* text = ::EnumStrH( *mVal.def, val );
        //         if ( text != NULL )
        //             lstrcpy( str, text );
        //     }
        //     break;
        // case id_DATAACQ:
        // case id_PID:
        //     if ( mVal.def->dwFlag == 0 )
        //     {
        //         if(val)
        //             lstrcpy( str, "True" );
        //         else lstrcpy( str, "False" );
        //     }
        //     else
        //     {
        //         const char* text = ::EnumStrH( *mVal.def, val );
        //         if ( text != NULL )
        //             lstrcpy( str, text );
        //     }
        //     break;
        // case id_SMDI:
        // case id_SMDICOM:
        // {
        //     CShSMDISCRET* sh = (CShSMDISCRET*)html.pipe.Sh(mVal.nNumber);
        //     if(val)
        //         lstrcpy( str,  sh->STATE1TEXT );
        //     else lstrcpy( str,  sh->STATE0TEXT );
        // }
        // break;

        // case id_QBSTATUS:
        // {
        //     CShQBSTATUS* sh = (CShQBSTATUS*)html.pipe.Sh(mVal.nNumber);
        //     if(Version == T1163_ONPZ || Version == T1165P_ONPZ)
        //     {
        //         switch(val%8)
        //         {
        //         case 0:lstrcpy( str,  sh->TARGETPVFOROPSTATE0 ); break;
        //         case 1:lstrcpy( str,  sh->TARGETPVFOROPSTATE1 ); break;
        //         case 2:lstrcpy( str,  sh->TARGETPVFOROPSTATE2 ); break;
        //         case 3:lstrcpy( str,  sh->TARGETPVFOROPSTATE3 ); break;
        //         case 4:lstrcpy( str,  sh->TARGETPVFOROPSTATE4 ); break;
        //         case 5:lstrcpy( str,  sh->TARGETPVFOROPSTATE5 ); break;
        //         case 6:lstrcpy( str,  sh->TARGETPVFOROPSTATE6 ); break;
        //         case 7:lstrcpy( str,  sh->TARGETPVFOROPSTATE7 ); break;
        //         }
        //         break;
        //     }
        //     if(val)
        //         lstrcpy( str,  sh->TARGETPVFOROPSTATE1 );
        //     else lstrcpy( str,  sh->TARGETPVFOROPSTATE0 );
        // }
        // break;

        // case id_QBAcyMin:
        // {
        //     if(strcmp(mVal.def->name, "MD") == 0)
        //     {
        //         if(val)
        //             lstrcpy( str,  "А" );
        //         else lstrcpy( str,  "Р" );
        //     }
        // }
        // break;

        //         default:
        //             if ( mVal.def->dwFlag != 0 )
        //             {
        //                 const char* text = ::EnumStrH( *mVal.def, val );
        //                 if ( text != NULL )
        //                     lstrcpy( str, text );
        //             }
        //             else
        //                 ASSUNO
        //         };
        // }
    }
    //
    if (Version==Реформинг_КНПЗ)
    {
        if (SetColorForSpecClass(html, style.mClass, val))
            return true;
    }
    if ( style.m_nBreakpointType == -1 ) return true;
    if ( style.m_nBreakpointType == 0 ) ASSUNO;
    if ( style.m_nBreakpointType !=  1 )
    {
        if(DOTF_PM==Version)
        {
            if(style.m_nContinuousValues && style.m_nContinuousColors)
            {
                // COLORREF color = style.Con_Color(html,val);
                // SetColor(html,color);
            }
        }
        return true;
    }

    if (Version > DOTF_PM)
    {  //в DOTF_PM это не проверял
        if((TYPE_PNT == TYPE_TEXT) && (style.m_dwFlags & HDX_CLRTEXT) && !(style.m_dwTypeObj & TYPE_BREAKPOINT))
            return true;
    }

    if(DOTF_PM==Version)
    {
        // char* id = style.NameID( html );
        // if( !strchr(id, '_') )
        // { //только те, что внутри shape и декодированны по m_numberOfShapesAnimated
        //     CTagDIV *pPar = dynamic_cast<CTagDIV*>(m_pPar);
        //     if(pPar)
        //     {
        //         CShape *pSha = dynamic_cast<CShape*>(pPar->m_pPar);
        //         if(pSha && pSha->m_pPar)
        //         {
        //             CTagDIV *pPar2 = dynamic_cast<CTagDIV*>(pSha->m_pPar);
        //             if(pPar2->m_numberOfShapesAnimated>=2)
        //                 if( (style.m_dwFlags & HDX_CLRFILL ) && !(style.m_dwFlags & HDX_FILL_S) && (style.m_dwFlags & HDX_DISK_COLORS)
        //                     && style.m_clrFillEmpty!=0xff000000
        //                     ) //нормального признака такого поведения не нашел
        //                 {//Анимация по номеру Shape
        //                     //Click the Animation tab and enter the number of shapes you want to use in Number
        //                     //of shapes. (For example, if the parameter has only four states but the shape sequence
        //                     //contains eight shapes, type 4 to use shapes 2 to 5.)
        //                     if(val == pSha->m_nShapeNumber)
        //                     {
        //                         SetColor(html, style.m_clrFillEmpty);
        //                         return true;
        //                     }
        //                     else return false;
        //                 }
        //         }
        //     }
        // }
    }

    if ( val > 7 )
    {
        ASSUNO
            return true;
    }
    //
    // COLORREF color = style.DiscColor(html,val);
    // SetColor(html,color);
    return true;//color != 0xFF000000;
}
//
bool CTag::OutInt( CHTML& html, int val, char* str  )
{
    CStyle& style = Style(html);
    if ( TYPE_PNT == TYPE_TEXT )
    {
        // char* filtr = style.Filtr(html);
        // if(style.m_TimeFormat==2)
        // {//"HH:MM:SS"
        //     //__time64_t T = val;
        //     //struct tm Tm;
        //     //_localtime64_s( &Tm, &T );
        //     //strftime( str , 512, "%H:%M:%S", &Tm );
        //     sprintf_s( str, 512, "%d", val );
        // }
        // else
        // {
        //     if ( lstrcmpi( filtr, "%s" ) )
        //         sprintf_s( str, 512, filtr, double(val) );
        //     else
        //         sprintf_s( str, 512, "%d", val );
        // }
    }
    //
    if ( style.m_nBreakpointType == -1 ) return true;
    if ( style.m_nBreakpointType !=  1 ) return true;

    if(style.m_dwFlags & HDX_CLRTEXT)
        return true;

    ASSERT( val < 8 );
    if ( val > 7 ) return true;
    //
    // COLORREF color = style.DiscColor(html,val);
    // SetColor(html,color);
    // return color != 0xFF000000;


    return true;
}
//
bool CTag::FillText( CHTML& html, char* str, int TypePnt )
{
    CStyle& style = Style(html);
    //
    // SBaseDSD* dsd = Dsd(html);
    // if ( dsd && dsd->m_nCountPip > 1 )
    // {
    //     lstrcpy( str, "$$$$$$$$$$" );
    //     return true;
    // }
    // //
    // bool bPaint = true;
    // void* val = html.pipe.ParamValue_H(mVal);
    // const char* name = html.pipe.GetMnemoName( mVal.nNumber );


    if(SGK_ONPZ==Version)
    {
        // if(mVal.nNumber>=0 && mVal.def && mVal.def->eVal == enumValueChr && strcmp(mVal.def->name, "PV")==0)
        // {
        //     if(strcmp(name, "XLP101A")==0 || strcmp(name, "XLP101B") == 0)
        //         if(*(BYTE*)val == 1)
        //             *(BYTE*)val = 2;
        // }
    }

    // if ( val != NULL )
    // {
    //     switch ( mVal.def->eVal )
    //     {
    //     case enumValueDbl: bPaint = OutDbl( html, *(double*)val, str ); break;
    //     case enumValueInt: bPaint = OutInt( html, *(int   *)val, str ); break;
    //     case enumValueBol:
    //         lstrcpy(str, m_constValue);
    //         if ( style.m_nBreakpointType ==  1 )
    //         {
    //             if(style.m_dwTypeObj & TYPE_BREAKPOINT)
    //             {
    //                 int vall = *(int*)val ? 1 : 0;
    //                 COLORREF color = style.DiscColor(html, vall);
    //                 SetColor(html,color);
    //             }
    //             lstrcpy(str, m_constValue);
    //         }
    //         else
    //             if ( TYPE_PNT == TYPE_TEXT )
    //             {
    //                 char* filtr = style.Filtr(html);
    //                 if(strcmp(filtr, "%s")==0)
    //                 {
    //                     if(mVal.def->dwLog == id_SMDISCRET)
    //                     {
    //                         CShSMDISCRET sh;
    //                         if ( html.pipe.GetData( mVal.nNumber, &sh ) )
    //                         {
    //                             if(*(bool*)val)
    //                                 strcpy(str, sh.STATE1TEXT);
    //                             else strcpy(str, sh.STATE0TEXT);
    //                         }
    //                     }
    //                 }
    //             }
    //         break;
    //     case enumValueChr: bPaint = OutChr( html, *(BYTE  *)val, str ); break;
    //     case enumValueStr:
    //     {
    //         if (Version==LG35_8_KF)
    //         {
    //             char* szNameId=style.NameID(html);
    //             if (strstr(szNameId,"_FaultZ")!=NULL)
    //             {
    //                 //MIHAIL привязаны GFZ.PV вместо GFZ.PVFL, текстовый вместо значений
    //                 char* szValue=(char *)val;
    //                 BYTE nValue=0;
    //                 if (lstrcmp(szValue,"АВАРИЯ")==0)
    //                     nValue=1;
    //                 bPaint = OutChr( html, nValue, str );
    //                 break;
    //             }
    //         }
    //         else
    //         {
    //             if((TypePnt==TYPE_LINE)||(TypePnt==TYPE_OVAL)||(TypePnt==TYPE_cross::RECT)) //по-идее все рисуемое
    //             {
    //                 char* szValue = (char *)val;
    //                 if(mVal.def->dwLog == id_FLAG)
    //                 {
    //                     CShFLAG sh;
    //                     if ( html.pipe.GetData( mVal.nNumber, &sh ) )
    //                     {
    //                         for(int ii=0; ii<_countof(sh.STATETEXT); ii++)
    //                             if(_stricmp(sh.STATETEXT[ii], szValue)==0)
    //                             {
    //                                 bPaint = OutChr( html, ii, str );
    //                                 break;
    //                             }
    //                     }
    //                 }
    //             }
    //             else
    //                 if(TypePnt==TYPE_TEXT)
    //                 {
    //                     if (Version==AM2_NAK)
    //                         if( (style.m_nBreakpointType == 1) && (style.m_dwTypeObj & TYPE_BREAKPOINT)==0)
    //                         {
    //                             lstrcpy(str, m_constValue);
    //                             break;
    //                         }
    //                 }
    //         }
    //         bPaint = OutStr( html,  (char  *)val, str );
    //         break;
    //     }
    //     case enumValueI64: OutInt( html, *(long*)val, str ); break;
    //     default:
    //         lstrcpy( str, "##########" );
    //         break;
    //     };
    //     if(Version == LG35_8_KF && !html.m_strFile.CompareNoCase("sncdtlCPCa_fp")
    //         && !stricmp(mVal.def->name, "PVEUHI"))
    //     {
    //         char *p = strstr(str,".");
    //         if(p) *p = 0;
    //     }
    // }
    // else
    //     if(mVal.nNumber == -4 && m_UserDefined.nType && m_UserDefined.param[0])
    //     {//ищем в параметр определенный пользователем
    //         if(id_QBANALOG == m_UserDefined.nType)
    //         {
    //             char* pp = strrchr(m_UserDefined.param, '.');
    //             if(pp)
    //             {
    //                 char name[256];
    //                 int len = int(pp-m_UserDefined.param);
    //                 strncpy_s(name, m_UserDefined.param, len);
    //                 name[len] = '\0';
    //                 CShQBANALOG sh;
    //                 if(html.pipe.GetData( id_QBANALOG, name, &sh ))
    //                 {
    //                     ++pp;
    //                     for(int ii=0; ii<_countof(sh.userDefined); ii++)
    //                     {
    //                         if(stricmp(sh.userDefined[ii].paramName, pp) == 0)
    //                         {
    //                             switch(sh.userDefined[ii].dataType)
    //                             {
    //                             case  evtInt2:
    //                                 sprintf_s(str, 64, "%d", (int)sh.userDefined[ii].i2);
    //                                 break;
    //                             case  evtInt4:
    //                                 sprintf_s(str, 64, "%d", (int)sh.userDefined[ii].i4);
    //                                 break;
    //                             case evtString:
    //                                 lstrcpyn(str, sh.userDefined[ii].chr, _countof(sh.userDefined[ii].chr));
    //                             default:
    //                                 AAA();
    //                                 break;
    //                             }
    //                             break;
    //                         }
    //                     }
    //                     AAA();
    //                 }
    //             }
    //         }
    //         else
    //             if(id_QBSTATUS == m_UserDefined.nType)
    //             {
    //                 char* pp = strrchr(m_UserDefined.param, '.');
    //                 if(pp)
    //                 {
    //                     char name[256];
    //                     int len = int(pp-m_UserDefined.param);
    //                     strncpy_s(name, m_UserDefined.param, len);
    //                     name[len] = '\0';
    //                     CShQBSTATUS sh;
    //                     if(html.pipe.GetData( id_QBSTATUS, name, &sh ))
    //                     {
    //                         ++pp;
    //                         for(int ii=0; ii<_countof(sh.userDefined); ii++)
    //                         {
    //                             if(stricmp(sh.userDefined[ii].paramName, pp) == 0)
    //                             {
    //                                 switch(sh.userDefined[ii].dataType)
    //                                 {
    //                                 case  evtInt2:
    //                                     sprintf_s(str, 64, "%d", (int)sh.userDefined[ii].i2);
    //                                     break;
    //                                 case  evtInt4:
    //                                     sprintf_s(str, 64, "%d", (int)sh.userDefined[ii].i4);
    //                                     break;
    //                                 case evtString:
    //                                     lstrcpyn(str, sh.userDefined[ii].chr, _countof(sh.userDefined[ii].chr));
    //                                 default:
    //                                     AAA();
    //                                     break;
    //                                 }
    //                                 break;
    //                             }
    //                         }
    //                         AAA();
    //                     }
    //                 }
    //             }
    //             else AAA();
    //     }
    //     else
    //     {
    //         if(mVal.nNumber != -1)
    //             lstrcpy( str, "----------" );
    //         else str[0] = 0;
    //     }
    // //
    // if ( style.m_nNumberOfChars >= 0 )
    //     if ( lstrlen(str) > style.m_nNumberOfChars )
    //         str[style.m_nNumberOfChars] = 0;
    // //
    // return bPaint;
    return true;
}
//
bool CTag::StopDebug(CHTML& html,LPCTSTR str)
{
#ifndef _DEBUG
    return false;
#endif
    _static int A = 0;
    switch ( A )
    {
    case 0: // входной текст
        break;
    case 1: // текст из клипбоарда
    {
        static CharMP clip;
        //    ::TxtFromClp( SizeMP, clip );
        str = clip;
    }
    break;
    case 2: // входной текст
        break;
    };
    CStyle& style = Style(html);
    // char* id = style.NameID( html );
    // if ( str != NULL )
    //     if ( lstrcmpi( id, str ) == 0 )
    //         return true;
    return false;
}

void CTag::ShowOrHide(CHTML &html, LPCTSTR tag, bool bShow)
{

}

CTag* CTag::FindTagById(CHTML& html, const char *nameid )
{
    CStyle& style = Style(html);
    // char* id = style.NameID( html );
    // if(stricmp(id, nameid)==0)
    //     return this;
    INT_PTR nCount = GetSize();
    CTag**     obj = GetData();
    for ( INT_PTR n = 0; n < nCount; n++ )
        if(CTag *ptag = obj[n]->FindTagById(html, nameid ))
            return ptag;

    return NULL;
}
//
void CTag::SetVisibility(CHTML& html, int vis)
{
    CStyle& style = Style(html);
#ifdef _DEBUG
    //   char* id = style.NameID( html );
#endif
    style.m_eVisible = static_cast<EVisibility>(vis);
    // if(m_pWnd && ::IsWindow(m_pWnd->m_hWnd))
    // {
    //     if(vis == enumHidden)
    //         ::ShowWindow(m_pWnd->m_hWnd, SW_HIDE);
    //     else if(vis == enumVisible)
    //         ::ShowWindow(m_pWnd->m_hWnd, SW_SHOW);
    // }
    if(Version == LG35_8_KF)
    {
        INT_PTR nCount = GetSize();
        CTag**     obj = GetData();
        for ( INT_PTR n = 0; n < nCount; n++ )
            obj[n]->SetVisibility( html, vis );
    }
}
//
void CTag::GoToSpecScheme089_Project(CHTML& html)
{
    std::string strPageName=GetSpecSchemeName089_Project(html);
    //   if (strPageName!="")
    //       GetAppWnd()->ButtonOk( barPAGE, strPageName.c_str(), hWnd );
}

std::string CTag::GetSpecSchemeName089_Project(CHTML& html)
{
    std::string strPageName="";
    int nShiftUpTagsFromThis=5;
    CStyle& styleText=html.Style(m_nStyle-5);
    std::string strText=styleText.GetValue(html);
    if (strText=="БСФ")
        strPageName="200";
    else
        if (strText=="E-201")
            strPageName="250";
        else
            if (strText=="K-200")
                strPageName="251";
            else
                if (strText=="П-200")
                    strPageName="252";
                else
                    if (strText=="E-200")
                        strPageName="253";
                    else
                        if (strText=="С-201")
                            strPageName="354";
                        else
                            if (strText=="Прочее")
                                strPageName="255";
    return strPageName;
}

typedef void (*Script127Function)(CTag* oSource, HDC hDC, CHTML& html);

// const Script127Function Scripts127[] =
//     {
//         NULL, //VBSCRIPT_All_OnClick_me_,
//         VBSCRIPT_CDA_Devctl_Hialm_RedTag_OnUpdate_me_,
//         VBSCRIPT_CDA_Device3States_AlpGPV_OnUpdate_me_,
//         VBSCRIPT_CDA_DigState_AlpPV_OnUpdate_me_,
//         VBSCRIPT_CDA_DigStateButton_AlpPV_OnUpdate_me_,
//         VBSCRIPT_CDA_DigStateCColor_AlpPV_OnUpdate_me_,
//         VBSCRIPT_CDA_DigStateColor_Khabarovsk_AlpPV_OnUpdate_me_,
//         VBSCRIPT_CDA_ExecState_Alp_OnUpdate_me_,
//         VBSCRIPT_CDA_Graph_AlpError_OnUpdate_me_,
//         VBSCRIPT_CDA_Numeric_AlpPVFormat_OnUpdate_me_,
//         VBSCRIPT_CDA_OnOffValves_AlpGPV_OnUpdate_me_,
//         VBSCRIPT_CDA_RegCntl_Mode_Hialm_RedTag_EUDesc_OnUpdate_me_,
//         VBSCRIPT_CDA_RegCntlValve_AlpVal_OnUpdate_me_,
//         VBSCRIPT_CDA_RegCtlValves_RedTag_BadCtlFl_OnUpdate_me_,
//         NULL, //VBSCRIPT_Graphic_KHABAROVSK_OnMouseOver_me_,
//         NULL, //VBSCRIPT_Graphic_KHABAROVSK_OnMouseUp_me_,
//         NULL, //VBSCRIPT_Nav_SelectBox_OnClick_me_,
//         VBSCRIPT_Scd_AbsorberStatus_Khabarovsk_AlpPV_OnUpdate_me_,
//         VBSCRIPT_Scd_Device3States_AlpPV_OnUpdate_me_,
//         VBSCRIPT_Scd_DigState_AlpPV_OnUpdate_me_,
//         VBSCRIPT_Scd_DigStateButton_AlpPV_OnUpdate_me_,
//         VBSCRIPT_Scd_DigStateCColor_AlpPV_OnUpdate_me_,
//         VBSCRIPT_Scd_DigStateColor_Khabarovsk_AlpPV_OnUpdate_me_,
//         VBSCRIPT_Scd_Graph_AlpError_OnUpdate_me_,
//         VBSCRIPT_Scd_Khabarovsk_BoxLocRem_AlpPV_OnUpdate_me_,
//         VBSCRIPT_Scd_Khabarovsk_Bypass_AlpPV_OnUpdate_me_,
//         VBSCRIPT_Scd_Khabarovsk_Description_onupdate_me_,
//         VBSCRIPT_Scd_Khabarovsk_DigStateCColor_AlpPV_OnUpdate_me_,
//         VBSCRIPT_Scd_Khabarovsk_Discrepancy_AlpPV_OnUpdate_me_,
//         VBSCRIPT_Scd_Khabarovsk_Enclav_onupdate_me_,
//         VBSCRIPT_Scd_Khabarovsk_Interlock_OnUpdate_me_,
//         VBSCRIPT_Scd_KHABAROVSK_RotorkValvesLocalRemote_AlpPV_OnUpdate_me_,
//         VBSCRIPT_Scd_libKHABAROVSK_Bypass_AlpPV_OnUpdate_me_,
//         VBSCRIPT_Scd_libKHABAROVSK_Interlock_AlpPV_OnUpdate_me_,
//         VBSCRIPT_Scd_Numeric_AlpPVFormat_OnUpdate_me_,
//         VBSCRIPT_Scd_OnOffValves_AlpPV_OnUpdate_me_,
//         VBSCRIPT_Scd_RegCntl_Mode_Hialm_EUDesc_OnUpdate_me_1_,
//         VBSCRIPT_RP1516CB_TextBox_UpdateScript,
//         VBSCRIPT_Scd_Khabarovsk_InitAnalog_AlpPV_OnUpdate_me_,
//         VBSCRIPT_Scd_Khabarovsk_Actuador_AlpPV_OnUpdate_me_,
//         VBSCRIPT_Scd_Khabarovsk_InitDigital_AlpPV_OnUpdate_me_,
//         VBSCRIPT__RecPV1_0140213,
//         VBSCRIPT__RecPV2_0140213,
//         VBSCRIPT__RecPV3_0140213,
//         VBSCRIPT__RecPV1_0140220,
//         VBSCRIPT__TxtMode001,
//         VBSCRIPT_Scd_OnOffValves_Khabarovsk_AlpPV_OnUpdate_me_,
//         VBSCRIPT_Scd_RegCntlValve_Khabarovsk_AlpVal_OnUpdate_me_,
//         VBSCRIPT__HWLineHeater14_2500109,
//         VBSCRIPT__HWLineHeater13_2500109,
//         VBSCRIPT__HWLineHeater23_2500115,
//         VBSCRIPT__HWLineHeater24_2500115,
//         VBSCRIPT__HWLineHeater33_2500201,
//         VBSCRIPT_Scd_RegCntlValve_AlpVal_OnUpdate_me_,
//         VBSCRIPT_Scd_Khabarovsk_DamperValves_4State_AlpPV_OnUpdate_me_,
//         VBSCRIPT_CDA_Damper_AlpVal_OnUpdate_me_,

    //         VBSCRIPT_Cda_CEXXX_AlpPV_OnUpdate_me_,
    //         VBSCRIPT_LK2B_Cda_CEXXX_AlpPV_OnUpdate_me_,
    //         VBSCRIPT_LK2B_Cda_CEXXX_ConAlpTP_OnUpdate_me_,
    //         VBSCRIPT_LK2B_Cda_CEXXX_RecPV_OnUpdate_me_,
    //         VBSCRIPT_UZ_Cda_Graph_AlpError_OnUpdate_me_,
    //         VBSCRIPT_All_ILBPGraph_AlpError_OnUpdate_me_,
    //         VBSCRIPT_CDA_Interlock_OnUpdate_me_,
    // };

    // void CTag::Script127Exec( CTag* oSource, HDC hDC, CHTML& html )
    // {
    //     //if (!IsKhabarovsk()) return;
    //     for(std::vector<Script127>::iterator it = m_vctrUpdateScripts.begin(); it != m_vctrUpdateScripts.end(); ++it)
    //         if (Scripts127[*it])
    //             Scripts127[*it](oSource, hDC, html);
    // }

    std::map<std::string, SUB_STRUCT> CTag::m_CommonSubroutines;
void CTag::LoadCommomScripts()
{
    LoadCommomScripts("Abstract/Scripts/", "\nsub ");
    LoadCommomScripts("Abstract/Scripts/", "\npublic sub ");
    LoadCommomScripts("Abstract/DisplayScripts/", "\nsub ");
    LoadCommomScripts("Abstract/DisplayScripts/", "\npublic sub ");
}

void CTag::LoadCommomScripts(char * subpath, char *sbegin)
{
    //std::string szbasePath(::GetBasePath());
    std::string szbasePath;
    std::string path = szbasePath + subpath;
    WIN32_FIND_DATAA FD;
    HANDLE hFind = FindFirstFileA( (path + "*.vbs").c_str(), &FD );
    if ( hFind != INVALID_HANDLE_VALUE )
    {
        BOOL Res = TRUE;
        while( Res )
        {
            if( (FD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0 )
            {
                std::string filename = path + FD.cFileName;
                DWORD dwSize;
                //               char* szBuffer = ::FileToBuff( filename.c_str(), dwSize );
                // if ( szBuffer != NULL )
                // {
                //     CHTML::GetSubroutines0(m_CommonSubroutines, szBuffer, sbegin, "\nend sub", false);

                //     ::BuffFree(szBuffer);
                // }
                Res = FindNextFile(hFind, &FD);
            }
        }
        ::FindClose( hFind );
    }
}
