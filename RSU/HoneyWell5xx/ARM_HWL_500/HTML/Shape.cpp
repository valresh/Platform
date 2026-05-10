// #include "StdAfx.h"
// #include "../ARM.h"
// #include "HTML.h"
// #include "FileXML.h"
// #include "Shape.h"
// #include "TagDIV.h"
// #include "TagSCRIPT.h"
// //#include "..\\FindAbstract.h"
// //
// CShape::CShape()
//   {
//      m_nShapeNumber =0;
//   }
// //
// bool CTagDIV::Shape( CHTML& html )
//   {
//   CStyle& style = Style(html);
//   char* id = style.NameID( html );
// #ifdef _DEBUG
//   if ( lstrcmp( id, "PUMP1_LED_001" ) == 0 )
//      AAA();
//   if ( lstrcmp( id, "ShpGOPState5" ) == 0 )
//      AAA();
// #endif

//   m_bIsShape = true;

//   if ( !style.IsShape() ) return false;
//   char* src = style.SrcFile( html );
//   if ( !*src ) return false;

//   if(Version>=AM2_NAK)
//   {
//       if((Version==BERTH_ONPZ && (html.m_strFile.CompareNoCase("sysdtlSMdevctla_3SVLV_fp") == 0 || html.m_strFile.CompareNoCase("sysdtlSMdevctla_PUMP_fp") == 0))
//         || m_linkType != ltEmbedded)
//       {
//       if(m_numberOfShapesAnimated == 0)
//         m_numberOfShapesAnimated = -2; //прочитать из sha
//       }
//       else
//       if(Version==BERTH_ONPZ)
//       {
//           if(strstr(id, "_LED_") && m_linkType == ltEmbedded && m_numberOfShapesAnimated == 0)
//             m_numberOfShapesAnimated = -2; //прочитать из sha
//       }
//   }

//   INT_PTR n = GetSize();
//   if ( m_numberOfShapesAnimated == 0 )
//   if ( n > 0 )
//     return false;
//   //
//   if ( m_numberOfShapesAnimated > 1 )
//     {
//     ASSERT( n == 0 || n == 1 );
//     if ( n == 1 )
//       {
//       CTag** obj = GetData();
//       obj[0]->m_bNoPaint = true;
//       delete obj[0];
//       RemoveAt( 0 );
//       }
//     }
//   //
//   int nOld = m_numberOfShapesAnimated;
//   if ( m_numberOfShapesAnimated == 0 ) m_numberOfShapesAnimated++;
//   CFileXML xml;
//   CharMP szCurr,szSave;
//   UINT nReturn = xml.LoadEx(src, html.m_strFile, szCurr);
//   if ( nReturn != 0)
//   {   //сначала в подкаталоге мнемосхемы
//       CharMP szFilesSrc;
//       sprintf_s(szFilesSrc, "%s_files/%s", html.m_strFile, src);
//       nReturn = xml.LoadEx(szFilesSrc, html.m_strFile, szCurr);
//   }
//   if ( nReturn != 0 )
//     {
//     const char* find = FindAbstract(src);
//     if ( find != NULL )
//       nReturn = xml.LoadEx(find, html.m_strFile, szCurr);
//     }
//   if ( AS(nReturn == 0) ) return false;
//   //
//   ::GetCurrentDicross::RECTory( SizeMP, szSave );
//   ::SetCurrentDicross::RECTory( szCurr );
//   //
//   if ( !(style.m_dwTypeObj & TYPE_ANIMATOR) )
//   {
//       if(style.m_nHDXBINDINGID >=0)
//       {
//          style.m_nPlaceDsd = -3;  //"HDXBINDINGID:" без соответствующих признаков
//       }
//   }
//   //
//   if ( !Shape( html, xml ) )
//   {
//     if((L35_11600_KF == Version || AM2_NAK == Version) && stricmp(src, "sysBackground_1276X849_CDA.sha") == 0)
//     { //нет этого файла - чтоб не красилось оранжевым
//      m_numberOfShapesAnimated = 0;
//     }
//     else
//     m_numberOfShapesAnimated = -1;
//   }
//   else
//   if ( nOld == 0 )
//     m_numberOfShapesAnimated = 1;
//   ::SetCurrentDicross::RECTory( szSave );
//   //
//   return true;
//   }
// //
// bool CTagDIV::Shape( CHTML& html, CFileXML& xml )
//   {
//   HXML nShapeFile = xml.IsItems(INVALID_HXML,"ShapeFile");
//   if ( As(nShapeFile) ) return false;
//   HXML nShapes = xml.IsItems(nShapeFile,"Shapes");
//   if ( As(nShapes) ) return false;
//   //
//   int nRealShapes = xml.CountItem( nShapes,"Shape" );
//   if ( m_numberOfShapesAnimated == -2 )
//     m_numberOfShapesAnimated = nRealShapes;
//   else m_numberOfShapesAnimated = min(nRealShapes, m_numberOfShapesAnimated);
//   //
//   HXML nShape = xml.IsItems(nShapes,"Shape", nRealShapes );
//   if ( nShape == INVALID_HXML )
//     {
//     for(int ii=1; ii<4; ii++)
//       {
//       nShape = xml.IsItems(nShapes,"Shape", nRealShapes + ii);
//       if ( nShape != INVALID_HXML )
//           break;
//       }

//     if ( nShape == INVALID_HXML )
//        return false;
//     m_numberOfShapesAnimated++;
//     }
//   //
//   const char* text = xml.Param( nShapeFile, "useFirstShapeForBadValue" );
//   if ( text )
//     {
//     if ( lstrcmp( text, "0"  ) == 0 )  m_useFirstShapeForBadValue =  0;
//     else
//     if ( lstrcmp( text, "-1" ) == 0 )  m_useFirstShapeForBadValue = -1;
//     else
//       {
//       ASSERT(0);
//       }
//     }

//   int nShapeNumber=0;
//   do
//     {
//     CShape* shape = new CShape();
//     shape->InitStyle(html);
//     ASSERT( shape != NULL );
//     shape->m_pPar = this;
//     CStyle& style = shape->Style(html);
//     style.mcross::RECT = Style(html).mcross::RECT;
//     style.SetLastName( html, "Shape" );
//     if ( !Shape( html, xml, nShape, shape ) )
//       {
//       ASSERT(0);
//       delete shape;
//       return false;
//       }
//     shape->m_nShapeNumber = nShapeNumber++;
//     Add( shape );
//     nShape = xml.NextItem( nShape );
//     }
//   while( nShape != INVALID_HXML );

//   if(Version>=AM2_NAK)
//   {
//   INT_PTR nCount = GetSize();
//   if(nCount > 1 &&   nCount > nShapeNumber && m_linkType == ltLinked)
//   {
//       //удаляю 0левой
//       CTag** obj = GetData();
// #ifdef _DEBUG
//       CStyle& ostyle = obj[0]->Style(html);
//       char* oid = ostyle.NameID( html );
// #endif
//       obj[0]->m_bNoPaint = true;
//       delete obj[0];
//       RemoveAt( 0 );
//   }
//   }
//   return true;
//   }
// //
// bool CTagDIV::Shape( CHTML& html, CFileXML& xml, HXML nShape, CTag* shape )
//   {
//   HXML nGroup = xml.FindItem(nShape,"Group");
//   if ( nGroup == INVALID_HXML )
//     {
//     HXML nElement = xml.FindItem(nShape,"Element");
//     if ( nElement == INVALID_HXML )
//       return false;
//     return Eleme( html, xml, nElement, shape );
//     }
//   //
//   while ( nGroup != INVALID_HXML )
//     {
//     if ( !Group( html, xml, nGroup, shape ) )
//       return false;
//     nGroup = xml.NextItem(nGroup);
//     }
//   return true;
//   }
// //
// bool CTagDIV::Group( CHTML& html, CFileXML& xml, HXML nGroup, CTag* shape )
//   {
//   if ( !Eleme( html, xml, nGroup, shape ) )
//     return false;
//   //
//   INT_PTR n = shape->GetSize();
//   CTag* tag = NULL;
//   for(;n>0; n--)
//   {
//     tag = shape->GetAt(n-1);
//     if (typeid(*tag) != typeid(CTagSCRIPT))
//         break;
//     tag = NULL;
//   }
//   if(tag == NULL)
//     return false;
//   n = shape->GetSize();
//   //
//   const char* name  = shape->Style(html).GetName (html);
//   if ( lstrcmpi( name, "Shape" ) == 0 )
//     {
//     // Первый Group почему-то в абсолютных единицах
//     CStyle& style = tag->Style(html);
//     style.mcross::RECT = shape->Style(html).mcross::RECT;
//     }
//   shape = tag;
//   //
//   //
//   SItemXML& item = xml.Item( nGroup );
//   if ( item.m_nChild == INVALID_HXML )
//     return true;
//   HXML N = item.m_nChild;
//   while ( N != INVALID_HXML )
//     {
//     SItemXML& child = xml.Item( N );
//     const char* name = xml.Name(N);
//     if ( lstrcmpi( name, "Element" ) == 0 )
//       {
//       if ( !Eleme( html, xml, N, shape ) )
//         return false;
//       }
//     else
//     if ( lstrcmpi( name, "Group" ) == 0 )
//       {
//       if ( !Group( html, xml, N, shape ) )
//         return false;
//       }
//     N = child.m_nNext;
//     }
//   return true;
//   }
// //
// bool DoAlias( char* dst, const char* src )
//   {
//   if ( !(src[0] == '{' && src[1] == '%') )
//     return false;
//   int dl = lstrlen( src );
//   if ( !(src[dl-2] == '%' && src[dl-1] == '}') )
//     return false;
//   //
//   const char* p = strstr( src, "::" );
//   if ( p == NULL )
//     return false;
//   lstrcpy( dst, p + 2 );
//   char* r = strchr( dst, '%' );
//   *r = 0;
//   return true;
//   }
// //
// bool CTagDIV::Eleme( CHTML& html, CFileXML& xml, HXML nItem, CTag* shape )
//   {
//   HXML nHtml = xml.IsItems(nItem,"HTML");
//   if ( As(nHtml) ) return false;
//   HXML nContent = xml.IsItems(nHtml,"Content");
//   if ( As(nContent) ) return false;
//   HXML nData = xml.IsItems(nContent,"![CDATA[");
//   if ( As(nData) ) return false;
//   //
//   SCRIPT_SOURSE_DATA SriptData;
//   if(Version >= AM2_NAK)
//   {//скрипты прописанные прямо в описании тегов (обычно sha-файлы)
//   HXML nScripts = xml.IsItems(nHtml, "scripts");
//   if( nScripts != HXML(-1))
//     {   //по идее в цикле все скрипты - пока первый подходящий
//       HXML nScript = xml.IsItems(nScripts, "script");
//       UINT nnScripts = -1;
//       if(nScript == HXML(-1))
//         {
//         nnScripts = xml.CountItem( nScripts, "script" );
//         nScript = xml.IsItems(nScripts, "script", nnScripts );
//         }

//       int ii = 0;
//       while( nScript != HXML(-1) )
//         {
//         const char* nScriptLanguage = xml.Param( nScript, "language" );
//         if(nScriptLanguage && stricmp(nScriptLanguage, "VBScript")==0)
//           {
//            HXML nScriptData= xml.IsItems(nScript, "![CDATA[");
//            if( nScriptData != HXML(-1))
//            {
//            const char* scScriptEvent = xml.Param( nScript, "event" );
//            SriptData.eType = GetEventType(scScriptEvent);
//            if(SriptData.eType != eventUnk)
//              {
//              const char* szScriptValue = xml.Value( nScriptData );
//              if(szScriptValue)
//                SriptData.m_ScriptText = szScriptValue;
//              break;
//              }
//            }
//           }
//          nScript = xml.NextItem( nScript );
//         }
//     }
//   }
//   //
//   const char* value = xml.Value( nData );
//   cross::CString str = value;
//   char* szBuffer = str.GetBuffer();
//   char* ptr = szBuffer;
//   CTag* pCreatedTag = NULL;
//   //
// #ifdef _DEBUG
//   CStyle& sstyle = shape->Style(html);
//   char* sid = sstyle.NameID( html );
//   if(strcmp(sid, "shape013")==0)
//     AAA();
// #endif
//   //
//   if ( !shape->Parser( html, szBuffer, ptr, NULL, &SriptData, &pCreatedTag ) )
//     return false;
//   //
//   HXML nBinding = xml.IsItems(nItem,"binding");
//   if ( nBinding == INVALID_HXML ) return true;
//   HXML nDataObject = xml.IsItems(nBinding,"dataobject");
//   if ( nDataObject == INVALID_HXML ) return true;
//   HXML nDOD = xml.IsItems(nDataObject,"dataobjectdef");
//   if ( nDOD == INVALID_HXML ) return true;
//   //
//   const char* szName  = "";
//   const char* szValue = "";
//   nItem = xml.FindItem ( nDOD, "property" );
//   int n = 0;
//   while ( nItem != INVALID_HXML )
//     {
//     SItemXML& item = xml.Item( nItem );
//     const char* act = xml.Param( nItem, "name" );
//     if ( lstrcmpi( act, "PointRefPointName" ) == 0 )
//       szName = xml.Param( nItem, "value" ),n |= 1;
//     else
//     if ( lstrcmpi( act, "PointRefParamName" ) == 0 )
//       szValue = xml.Param( nItem, "value" ),n |= 2;
//     if ( lstrcmpi( act, "PointName" ) == 0 )
//       szName = xml.Param( nItem, "value" ),n |= 1;
//     else
//     if ( lstrcmpi( act, "PointParamName" ) == 0 )
//       szValue = xml.Param( nItem, "value" ),n |= 2;
//     //
//     if ( n == 3 ) break;
//     nItem = xml.NextItem ( nItem );
//     }
//   //
//   if ( !*szName || !*szValue ) return true;
//   //
//   CStyle& style = Style(html);
//   CharMP N,V;
//   if ( !DoAlias( N, szName  ) ) lstrcpy( N, szName  );
//   if ( !DoAlias( V, szValue ) ) lstrcpy( V, szValue );
//   //
//   INT_PTR nCount = shape->GetSize();
//   CTag**     obj = shape->GetData();
//   if ( nCount == 0 ) return true;
//   //
//   const char* szA = style.FindAlias( html, N );
//   const char* szB = style.FindAlias( html, V );
//   if ( lstrcmpi( szA, "{%Currentpoint%}" ) == 0 )
//     szA = html.Group();
//   if ( lstrcmpi( szA, "false" ) == 0 )
//     szA = style.FindAlias( html, N );
//   //
// #ifdef _DEBUG
//   if(stricmp(szB, "OP.OP")==0)
//      AAA();
// #endif
//   //
//   SParamValueH val = ::FindExperion( html, szA, szB );
//   //if ( val.def == NULL && val.nNumber != -1 ) ASSUNO;
//   obj[nCount-1]->mVal = val;
//   if(pCreatedTag)
//      pCreatedTag->mVal = val;
//   //
//   return true;
//   }
// //
// bool IsPidaMode(const char *pida, const char *mode)
// {
// 	pida += strlen(pida)-4;
// 	return !strnicmp(pida, "PIDA", 4) && !strnicmp(mode, "MODE", 4);
// }
// bool IsManMode(BYTE v)
// {
// 	return (v == 0);
// }
// //
// #include "Scripts127.h"
// void CTagDIV::PaintThis( LOCALS )
//   {
//   //
//   CStyle& style = Style(html);
//   char* id = style.NameID( html );
// #ifdef _DEBUG
//   if ( _stricmp(id, "PUMP1_LED_001")==0)
//     AAA();
// #endif

//   if(BERTH_ONPZ == Version)
//   {
//     if(html.m_nTimerCount == 1)
//       VBSCRIPT_Butt_Page_OnShapeLoad(this, NULL, html);
//   }

//   const char* name = html.pipe.GetMnemoName( mVal.nNumber );
//   EDataTypes  type = (EDataTypes)html.pipe.GetMnemoType( mVal.nNumber );
//   void* value = html.pipe.ParamValue_H(mVal);
//   if ( m_numberOfShapesAnimated == 0 )//|| !(style.m_dwFlags & HDX_INITcross::RECT) || val == NULL )
//     {
//     CTag::PaintThis( hDC, html );
//     return;
//     }
//   //
//   if(Version >= AM2_NAK && style.m_eVisible == enumHidden)
// 	  return;

//   if(Version >= AM2_NAK)
//   {
//   if ( style.LocalD( hDC ) )   //здесь осуществляются преобразования для поворота
//       return;
//   /*//рассчитать m_rcTruecross::RECT
//   if ( !(style.m_dwFlags & HDX_INITcross::RECT) )
//     {
//     XFORM xT;
//     ::GetWorldTransform(hDC, &xT );
//     if ( style.m_dwFlags & HDX_ROTATE )
//        ::ModifyWorldTransform( hDC, &style.m_XF, MWT_LEFTMULTIPLY );
//     style.m_dwFlags |= HDX_INITcross::RECT;
//     XFORM xf;
//     ::GetWorldTransform( hDC, &xf );
//     Gdiplus::Matrix matrix( xf.eM11, xf.eM12, xf.eM21, xf.eM22, xf.eDx, xf.eDy );
//     PointF p1( style.mcross::RECT.X, style.mcross::RECT.Y );
//     matrix.TransformPoints( &p1 );
//     PointF p2( style.mcross::RECT.X + style.mcross::RECT.Width, style.mcross::RECT.Y + style.mcross::RECT.Height );
//     matrix.TransformPoints( &p2 );
//     //
//     style.m_rcTruecross::RECT.left   = (int)( min( p1.X, p2.X ) + 0.5 );
//     style.m_rcTruecross::RECT.right  = (int)( max( p1.X, p2.X ) + 0.5 );
//     style.m_rcTruecross::RECT.top    = (int)( min( p1.Y, p2.Y ) + 0.5 );
//     style.m_rcTruecross::RECT.bottom = (int)( max( p1.Y, p2.Y ) + 0.5 );
    
//     ::SetWorldTransform(hDC, &xT );
//     }
//     */
//   }
//   //
//   INT_PTR nCount = GetSize();
//   CTag**     obj = GetData();

//   if(obj == NULL)
//   {  //проверить почему так происходит
//       Postik( hDC, html );  //здесь осуществляются возврат к преобразованию до поворота
//       return;
//   }

//   if ( m_numberOfShapesAnimated == 1 )//|| !(style.m_dwFlags & HDX_INITcross::RECT) || val == NULL )
//     {
//     if ( nCount > 0 && obj != NULL )
//       obj[0]->PaintThis( hDC, html );
//     else
//       ASSUNO;
//     Postik( hDC, html ); //здесь осуществляются возврат к преобразованию до поворота.
//     return;
//     }
//   //
//   Postik( hDC, html ); //здесь осуществляются возврат к преобразованию до поворота. Возможно нужно сдлелать ниже после рисования, но такого не было
//   //
//   EVisibility eVisible = enumVisible;
//   if (Version != LG35_8_KF)  //там как-то работало
//      eVisible = style.m_eVisible;
//   if(eVisible == enumHidden)
//      return;//если что-то невидимо, то и все что в это включено тоже невидимо
//   //
//   style.Local( hDC, html );
//   if ( style.LocalD( hDC ) ) return;
//   //
//   cross::RECT cross::RECT = style.Outscross::RECT();
//   if ( ::IsMnemoStyle(MNEMO_HIDE_SCN) )
//   //if ( (style.m_dwTypeObj & TYPE_FACEPLATE) )
//     {
//     //cross::RECT cross::RECT = style.Fullcross::RECT();
//     //CMyBrush brush( hDC, 0x8080 );
//     //brush.cross::RECT( cross::RECT );
//     //char txt[256];
//     //sprintf_s( txt, 100, "%d", m_numberOfShapesAnimated );
//     //CMyFont font( hDC, NULL, 0xff );
//     //font.Draw( cross::RECT, txt );
//     }
//   //
//   if ( mVal.nNumber >= 0 )
//     {
//     int dd = -1;
//     BYTE rr = (BYTE)dd;
// #ifdef _DEBUG
//     const char* pname = html.pipe.GetModelName(mVal.nNumber);
//     if ( lstrcmp( pname, "BZE6705.DI1" ) == 0 )
//       AAA();
// #endif
//     BYTE v = -1;
//     int vv = 0;
//     if ( mVal.def->eVal == enumValueChr )
//       v = (BYTE)html.pipe.ValueB_H( mVal );
//     else
//     if ( mVal.def->eVal == enumValueBol )
//       v = (BYTE)html.pipe.ValueBOOL_H( mVal );
//     else
//     if ( mVal.def->eVal == enumValueInt )
//       v = (BYTE)html.pipe.ValueI_H( mVal );
//     else
//     if ( mVal.def->eVal == enumValueStr )
//       {
//          char* szValue = (char*)html.pipe.ParamValue_H(mVal);
//          if(szValue)
//          {
//          if(mVal.def->dwLog == id_FLAG)
//           {
//            CShFLAG sh;
// 		   if ( html.pipe.GetData( mVal.nNumber, &sh ) )
//            {
//              for(int ii=0; ii<_countof(sh.STATETEXT); ii++)
// 		     if(_stricmp(sh.STATETEXT[ii], szValue)==0)
//                 {
//                   if(ii<nCount)
//                     v = vv = ii;
//                   break;
//                 }
//            }
//           }
//          else
//            KKK();
//          }
//       }
//     else
//     if ( mVal.def->eVal == enumValueBol )
//       {
//           //if(mVal.def->dwLog == id_SMDISCRET)
//           v = (BYTE)html.pipe.ValueB_H( mVal );
//           AAA();
//       }
//     else
//     {
//       //  v=(::GetTickCount()/1000)%nCount;
//       ASSUNO;
//     }
//     //
//     sprintf_s(m_szScriptValue, 32, "%d", v);
//     //
//     if ( lstrcmpi( mVal.def->name, "PV" ) == 0 )
//       {
//       // 0-PVSTATES (0) (Определяется STATETXT(0))
// 	    // 1-PVSTATES (1) (Определяется STATETXT(1))
// 	    // 2-PVSTATES (2) (Определяется BADPVTXT   )
// 	    // 3-PVSTATES (3) (Определяется MOVPVTXT   )
// 	    // 4-PVSTATES (4) (Определяется STATETXT(2))
//       if ( m_useFirstShapeForBadValue == -1 )
//         {
//         if(Version < AM2_NAK)
//         switch ( v )
//           {
//           case 0: v=1; break;
//           case 1: v=2; break;
//           case 2: v=0; break;
//           case 3: v=4; break;
//           case 4:
//             // Ёбаные нюансы
//             v=5;
//             if ( v >= BYTE(nCount) )
//               v = BYTE(nCount-1);
//             break;
//           }
//         else
//           {
//             if( v == 0xff)
//                 v = 0;
//              else
// 				 if(nCount > 2)
// 					 ++v;
//           }
//         }

//       if ( v == 0xff )
//         obj[vv]->PaintThis( hDC, html );
//       else
//       if ( v < nCount )
//       {
//         obj[v]->PaintThis( hDC, html );
// 		if (Version == ITH_NAK && _stricmp(id, "shpDB")==0)
// 			*m_szScriptValue = v-1;
//       }
//       else
//       if ( v != 0xff )
//         {
//         ASSERT(0);
//         }
//       }
//     else
//       {
//       if ( m_useFirstShapeForBadValue == -1 )
//         v = ( v == 0xff ) ? 0 : v+1;
//       else
//         v = ( v == 0xff ) ? 0 : v;
//       if ( nCount > 0 && obj != NULL )
// 	  {
// 		  // name = FCA2219A.PIDA
// 		  // mVal.def->name = MODE
// 		  if (Version == AT_PRP && IsPidaMode(name, mVal.def->name))
// 		  {
// 			  if (IsManMode(v)) obj[0]->PaintThis( hDC, html );
// 		  }
// 		  else
// 		  if (Version == AM2_NAK)
// 		  {
// 			obj[v%nCount]->PaintThis( hDC, html );
// 		  }
// 		  else
// 		  {
// 			obj[v%m_numberOfShapesAnimated]->PaintThis( hDC, html );
// 		  }
// 	  }
//       else
//         ASSUNO
//       }
//     }
//   else
//   if(atoi(m_szScriptValue) > 0)
//     {
//       int v = atoi(m_szScriptValue);
//       if(Version != AM2_NAK
// 			|| !((!strcmp(html.m_strFile, "307") && !strcmp(id, "shape006_shpZadv"))
// 				|| (!strcmp(html.m_strFile, "329") && !strcmp(id, "shape004_shpZadv"))
// 				|| (!strcmp(html.m_strFile, "339") && !strcmp(id, "shape005_shpZadv"))
// 				|| (!strcmp(html.m_strFile, "342") && !strcmp(id, "shape002_shpZadv"))
// 				|| (!strcmp(html.m_strFile, "343") && !strcmp(id, "shape002_shpZadv"))
// 				)
// 			)
//       if(Version!=BERTH_ONPZ || m_linkType!= ltEmbedded)
// 		v -= 1;
//       if ( v < nCount )
//         obj[v]->PaintThis( hDC, html );
//       AAA();
//     }
//   else
//   if(m_nDivValue > 0)
//   {
//      int v = m_nDivValue-1;
//      if ( v < nCount )
//         obj[v]->PaintThis( hDC, html );
//   }
//   else
//     {
//     if ( ::IsMnemoStyle(MNEMO_EDIT) )
//       {
//       cross::RECT cross::RECT = style.Fullcross::RECT();
//       CMyBrush brush( hDC, 0x80ff );
//       brush.cross::RECT( cross::RECT );
//       }
//     if ( nCount > 0 && obj != NULL )
//     obj[0]->PaintThis( hDC, html );
//     }
//   //
//   Postik( hDC, html );
//   style.Posts( hDC, html );
//   //
//   if ( m_pWnd ) m_pWnd->Timer();
//   //
//   }
// //
