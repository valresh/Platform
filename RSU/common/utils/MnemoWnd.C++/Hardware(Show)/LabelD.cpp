//#include "..\MnemoWnd.h"
#include "Queue.h"
#include "../Lang.h"
#include "SoftGDI.h"
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Структура надписи "Дублирование"
struct Queue_LabelD : public SPROLOZ
  {
  COLORREF color;
  COLORREF fon;
  } ;
////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//// Функция отображения
//int PaintLabelD(DefinePaint)
//  {
//  QueueP(Queue_LabelD);
//  //
//  CShBase* sh = PipeSh;
//  if (p->nSocketNumber!=-1)
//	sh=q.m_pSocket->Sh(p->nSocketNumber);
//  if ( sh == NULL ) return 1;
//  //
//  if ( p->fon != 0 )
//    {
//    if ( ::IsMnemoStyle(MNEMO_EDIT) )
//    if ( !IsTypeOk )
//    TransparentRect( hDC, p->mRect, 0x80ff, p->fon );
//    return 1;
//    }
//  //
//  char* text = NULL;
//  if ( !IsTypeOk )
//    {
//    static char szText[32] = "Нет данных";
//    text = szText;
//    }
//  else
//  if ( sh->Busy > 1 && (q.m_dwMainTimer % 2) )
//    {
//    static char szText[32] = "Дублирование";
//    text = szText;
//    }
//  //
//  if ( text != NULL )
//    {
//    COLORREF color = ::SetTextColor( hDC, p->color );
//    ::DrawText( hDC, text, lstrlen(text), &p->mRect, DT_SINGLELINE|DT_VCENTER|DT_CENTER );
//    ::SetTextColor( hDC, color );
//    }
//  //
//  return 1;
//  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AddLabelD(DefineTegs)
{
    //
    AddQueueEx(Queue_LabelD);
    //
    // Код, посылаемый в АРМ
    p->btSendType = QUEUE_NO;
    // Определение функции вывода
    //p->paint = &PaintLabelD;
    //
    p->color = 0x0060ff;
    char szObj[32] = "\0";
    p->nSocketNumber = -1;
    //
    STegObj obj[] =
    {
    { 'A', "color" , &p->color },
    { 'A', "fon"   , &p->fon   },
    { 'S', "obj"   , szObj, 31 },
    // Дополнительная информация
    EXT_QUEUE
    };
    //
    ::ParserObjScn(teg, obj, sizeof(obj) / sizeof(STegObj));
    //
    if (*teg[0].value == 0 || *szObj == 0)
    {
        p->nSocketNumber = def.GetActiveObjSocket();
        managed::RegisterElement(teg->name, p);
        return;
    }
    //
    UINT eType = TextToType(szObj);
    p->nSocketNumber = def.AddKindEx(eType, teg[0].value);
    managed::RegisterElement(teg->name, p);
}
////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//// Функция начального нажатия
//static void fnBeginPress(void* base,void* pVoid)
//  {
//  ASS( base )
//  Queue_PRS* p = (Queue_PRS*)base;
//  p->send.SetOld(pVoid);
//  p->m_bPressed = true;
//  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MaxMin( CLang& def, double& dMax, double& dMin )
  {
  int nActive = def.GetActiveObj();
  int nNumber = def.GetMnemoTags( nActive );
  if ( nNumber < 0 ) return;
  //
  UINT type = def.GetMnemoType( nActive );
  if ( type == id_Sensor || type == id_Reg )
    {
    STeg teg;
    GetTag( nNumber, teg );
    dMin = teg.dWorkMin;
    dMax = teg.dWorkMax;
    }
  else
  if ( type == id_Value )
    {
    const char* name = def.GetModelName( nActive );
    const char* find = strrchr( name, '.' );
    if ( find == NULL ) find = "";
    else                find++;
    //
    CharMP szParam;
    sprintf_s( szParam, SizeMP, "max%s", find );
    dMax = ::TegValueD( nNumber, szParam, dMax );
    sprintf_s( szParam, SizeMP, "min%s", find );
    dMin = ::TegValueD( nNumber, szParam, dMin );
    }
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#undef  STD_VAR
#define STD_VAR CLang& def, STegScn* teg, LPFuncAddP func, Queue_PRS* p,\
                  char* szImage, POINT& pt, double dScale, char* szShift
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void FillGDI( SPriborBase* d, Queue_PRS* p, char* szShift )
  {
  d->pBase   = p;
  // vladexl d->fnPress = fnBeginPress;
  if ( *szShift )
    {
     d->pVoid = FillVoid( p->send, p->pSH, szShift, p->FieldName);
     ASSERT( d->pVoid != NULL );
    }
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
HPRIBOR FillWheel( STD_VAR )
  {
  static SPriborWheel data; data.dScale = dScale;
  MaxMin( def, data.dValMax, data.dValMin );
  //
  STegObj obj[] =
    {
    { 'D', "step"  , &data.dStep    },
    { 'N', "direct", &data.direct   },
    { 'D', "scalex", &data.dScaleX  },
    { 'D', "scaley", &data.dScaleY  },
    { 'I', "clip"  , &data.nClipTop },
    { 'L', "invert", &data.bInvert  },
    { 'D', "min"   , &data.dValMin  },
    { 'D', "max"   , &data.dValMax  },
    };::ParserObjScn( teg, obj, sizeof(obj)/sizeof(STegObj) );
  //
  FillGDI( &data, p, szShift );
  (*func)( &data, p );
  return ::GiveMyPribor( szImage, pt, &data );
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
HPRIBOR FillInt( STD_VAR )
  {
  static SPriborInt data; data.dScale = dScale;
  //
  STegObj obj[] =
    {
	{ 'B', "specRichag"  , &data.bSpecRichag },
    { 'N', "direct", &data.direct },
	{ 'L', "hideCircles"  , &data.bHideCircles },
	{ 'L', "return1from2"  , &data.bReturn1From2 },
    };::ParserObjScn( teg, obj, sizeof(obj)/sizeof(STegObj) );
  //
  FillGDI( &data, p, szShift );
  (*func)( &data, p );
  return ::GiveMyPribor( szImage, pt, &data );
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
HPRIBOR FillBool( STD_VAR )
  {
  static SPriborBool data; data.dScale = dScale;
  //
  STegObj obj[] =
    {
    { 'N', "direct", &data.direct  },
    { 'L', "zalip" , &data.bZalip  },
    { 'L', "static", &data.bFirstS },
    { 'L', "invert", &data.bInvert },
    { 'I', "timer" , &data.nTimer  },
    { 'I', "set"   , &data.nSet    },
    };::ParserObjScn( teg, obj, sizeof(obj)/sizeof(STegObj) );
  //
  FillGDI( &data, p, szShift );
  (*func)( &data, p );
  return ::GiveMyPribor( szImage, pt, &data );
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
HPRIBOR FillDbl( STD_VAR )
  {
  static SPriborDbl data; data.dScale = dScale;
  MaxMin( def, data.dValMax, data.dValMin );
  //
  STegObj obj[] =
    {
    { 'I', "step"  , &data.nStep   },
    { 'N', "direct", &data.direct  },
    { 'D', "min"   , &data.dValMin },
    { 'D', "max"   , &data.dValMax },
    };::ParserObjScn( teg, obj, sizeof(obj)/sizeof(STegObj) );
  //
  //data.pbBlock = pbBlock;
  //
  FillGDI( &data, p, szShift );
  (*func)( &data, p );
  return ::GiveMyPribor( szImage, pt, &data );
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
HPRIBOR FillDouble( STD_VAR )
  {
  static SPriborDouble data; data.dScale = dScale;
  MaxMin( def, data.dValMax, data.dValMin );
  //
  STegObj obj[] =
    {
    { 'D', "step"  , &data.dStep   },
    { 'N', "direct", &data.direct  },
    { 'D', "min"   , &data.dValMin },
    { 'D', "max"   , &data.dValMax },
	{ 'B', "specRuchka", &data.bSpecRuchka},
    };::ParserObjScn( teg, obj, sizeof(obj)/sizeof(STegObj) );
  //
  //data.bDenyCtrlShift=(Version==CRBA_PR)&&( lstrcmp( CPipeClient::ms_szServer, "." ) != 0 );
  data.bDenyCtrlShift=false;
  //data.pbBlock = pbBlock;
  //
  FillGDI( &data, p, szShift );
  (*func)( &data, p );
  return ::GiveMyPribor( szImage, pt, &data );
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
HPRIBOR FillOnOff( STD_VAR )
  {
  static SPriborOnOff data; data.dScale = dScale;
  data.bZalip  = true;
  //
  STegObj obj[] =
    {
    { 'N', "direct", &data.direct },
    { 'L', "zalip" , &data.bZalip },
    };::ParserObjScn( teg, obj, sizeof(obj)/sizeof(STegObj) );
  //
  FillGDI( &data, p, szShift );
  (*func)( &data, p );
  return ::GiveMyPribor( szImage, pt, &data );
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
HPRIBOR FillArrow( STD_VAR )
  {
  static SPriborArrow data; data.dScale = dScale;
  MaxMin( def, data.dValMax, data.dValMin );
  //
  STegObj obj[] =
    {
    { 'N', "direct", &data.direct  },
    { 'D', "min"   , &data.dValMin },
    { 'D', "max"   , &data.dValMax },
    { 'A', "color" , &data.rgbColor},
    { 'I', "width" , &data.nWidth  },
    };::ParserObjScn( teg, obj, sizeof(obj)/sizeof(STegObj) );
  //
  FillGDI( &data, p, szShift );
  (*func)( &data, p );
  return ::GiveMyPribor( szImage, pt, &data );
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
HPRIBOR FillFloat( STD_VAR )
  {
  static SPriborFloat data; data.dScale = dScale;
  MaxMin( def, data.dValMax, data.dValMin );
  //
  STegObj obj[] =
    {
    { 'N', "direct", &data.direct  },
    { 'I', "angle1", &data.nAngle1 },
    { 'I', "anglew", &data.nAngleW },
    { 'D', "min"   , &data.dValMin },
    { 'D', "max"   , &data.dValMax },
    };::ParserObjScn( teg, obj, sizeof(obj)/sizeof(STegObj) );
  //
  FillGDI( &data, p, szShift );
  (*func)( &data, p );
  return ::GiveMyPribor( szImage, pt, &data );
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
HPRIBOR FillPM( STD_VAR )
  {
  static SPriborPM data; data.dScale = dScale;
  MaxMin( def, data.dValMax, data.dValMin );
  double dStep = 1.;
  lstrcpy( data.szAdd,"+" );
  lstrcpy( data.szDiv,"-" );
  bool bBool = false;
  bool bInvertColors = false;
  //
  STegObj obj[] =
    {
    { 'D', "step"  , &dStep        },
    { 'N', "direct", &data.direct  },
    { 'E', "add"   , &data.add     },
    { 'E', "div"   , &data.div     },
    { 'L', "bool"  , &bBool   },
	{ 'L', "invertColors"  , &bInvertColors   },
    { 'D', "min"   , &data.dValMin },
    { 'D', "max"   , &data.dValMax },
    { 'D', "font"  , &data.dFont   },
    { 'S', "txtA"  , data.szAdd, sizeof(data.szAdd)-1},
    { 'S', "txtD"  , data.szDiv, sizeof(data.szDiv)-1},
    };::ParserObjScn( teg, obj, sizeof(obj)/sizeof(STegObj) );
  //
  if ( bBool ) data.eType = enumValueBol;
  data.dStep = (data.dValMax-data.dValMin)/(dStep*100);
  data.clAdd = 0x000060;
  data.clDiv = 0x800000;
  if (bInvertColors)
  {
	    data.clDiv = 0x000060;
		data.clAdd = 0x800000;
  }
  //
  FillGDI( &data, p, szShift );
  (*func)( &data, p );
  return ::GiveMyPribor( szImage, pt, &data );
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
HPRIBOR FillManom( STD_VAR )
  {
  static SPriborManom data; data.dScale = dScale;
  //
  int nActive = def.GetActiveObj();
  int nTeg = def.GetMnemoTags( nActive );
  if ( nTeg >= 0 )
    {
    UINT eData = def.GetMnemoType( nActive );
    if ( eData == id_Sensor )
      {
      STeg teg;
      ::GetTag( nTeg, teg );
      data.dValMin = teg.dWorkMin;
      data.dValMax = teg.dWorkMax;
      }
    else
      {
      data.dValMin = TegValueD( nTeg, "manMin", data.dValMin );
      data.dValMax = TegValueD( nTeg, "manMax", data.dValMax );
      }
    }
  //
  STegObj obj[] =
    {
    { 'N', "direct", &data.direct  },
    { 'D', "min"   , &data.dValMin },
    { 'D', "max"   , &data.dValMax },
    { 'D', "riska" , &data.dRiska  },
    { 'D', "radius", &data.dRadius },
    { 'D', "delta" , &data.dDelta  },
    { 'D', "angle" , &data.dAngle  },
    { 'D', "denom" , &data.dDenom  },
    { 'S', "unit"  , &data.szUnit,15},
    { 'I', "ManomType", &data.nManomType },
    { 'D', "GreyMax" , &data.dGreyMax },
    { 'D', "GreyMin" , &data.dGreyMin  },
    };::ParserObjScn( teg, obj, sizeof(obj)/sizeof(STegObj) );
  //
  //if ( ::IsMnemoStyle(MNEMO_EDIT) )
  //  {
  //  int r = (int)data.dRadius;
  //  ::SetRect( &((LPSPROLOZ)p)->mRect, pt.x-r, pt.y-r, pt.x+r, pt.y+r );
  //  }
  //
  FillGDI( &data, p, szShift );
  (*func)( &data, p );
  return ::GiveMyPribor( szImage, pt, &data );
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
HPRIBOR FillScale( STD_VAR )
  {
  static SPriborScale data; data.dScale = dScale;
  //
  int nActive = def.GetActiveObj();
  int nTeg = def.GetMnemoTags( nActive );
  if ( nTeg >= 0 )
    {
    UINT eData = def.GetMnemoType( nActive );
    if ( eData == id_Sensor )
      {
      STeg teg;
      ::GetTag( nTeg, teg );
      data.dRealMin = teg.dWorkMin;
      data.dRealMax = teg.dWorkMax;
      }
    else
      {
      data.dRealMin = TegValueD( nTeg, "manMin", data.dRealMin );
      data.dRealMax = TegValueD( nTeg, "manMax", data.dRealMax );
      }
    }
  //
  STegObj obj[] =
    {
    { 'N', "direct", &data.direct  },
    { 'D', "minreal"   , &data.dRealMin },
    { 'D', "maxreal"   , &data.dRealMax},
	{ 'D', "mindraw"   , &data.dDrawMin },
    { 'D', "maxdraw"   , &data.dDrawMax},
    { 'D', "riska" , &data.dRiska  },
    { 'I', "height", &data.nScaleHeight },
	{ 'I', "width", &data.nScaleWidth },
    { 'S', "units"  , &data.szUnits,15},
    { 'I', "scaletype", &data.nScaleType },
	{ 'I', "sectors", &data.nSectors },
	{ 'I', "linesinsector", &data.nLinesInOneSector },
	{ 'L', "ramkagray", &data.bDrawGrayRamka },
	{ 'I', "ramkagrayX", &data.nGrayRamkaX },
	{ 'I', "ramkagrayY", &data.nGrayRamkaY },
	{ 'I', "unitsheight", &data.nUnitsHeight },
	{ 'I', "biglinelength", &data.nBigLinesLength },
	{ 'I', "ramkawhitex", &data.nWhiteRamkaX },
	{ 'I', "ramkawhitey", &data.nWhiteRamkaY },
	{ 'L', "horizont", &data.bHorizont },
	{ 'I', "float", &data.nFloat },
	{ 'I', "fontheight", &data.nFontHeight },
	{ 'L', "triangle", &data.bTriangle },
	{ 'L', "unitsincenter", &data.bUnitsInCenter },
	{ 'D', "trianglewidth" , &data.dTriangleWidth  },
	{ 'D', "triangleheight" , &data.dTriangleHeight  },
    };::ParserObjScn( teg, obj, sizeof(obj)/sizeof(STegObj) );
  //
    ::SetRect( &((LPSPROLOZ)p)->mRect, pt.x, pt.y, pt.x+data.nScaleWidth, pt.y+data.nScaleHeight );
  //
  FillGDI( &data, p, szShift );
  (*func)( &data, p );
  return ::GiveMyPribor( szImage, pt, &data );
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
HPRIBOR FillOnOffOn( STD_VAR )
  {
  static SPriborOnOffOn data; data.dScale = dScale;
  data.bZalip  = true;
  //
  STegObj obj[] =
    {
    { 'N', "direct", &data.direct },
    { 'L', "zalip" , &data.bZalip },
    };::ParserObjScn( teg, obj, sizeof(obj)/sizeof(STegObj) );
  //
  FillGDI( &data, p, szShift );
  (*func)( &data, p );
  return ::GiveMyPribor( szImage, pt, &data );
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
HPRIBOR AddPribors( DefineTags, Queue_PRS* p, LPFuncAddP func, EDataTypes eDataTypes )
  {
  p->pPipe = def.m_queue.m_pPipe;
  p->pSocket=def.m_queue.m_pSocket;
  //
  POINT pt = {0,0};
  char szImage[64] = "\0";
  char szShift[64] = "\0";
  char szSFcom[64] = "\0";
  double dScale = 1.0;
  //
  STegObj obj[] =
    {
    { 'S', "image", szImage, 63},
    { 'S', "shift", szShift, 63},
    { 'P', "pt"   , &pt        },
    { 'D', "scale", &dScale    },
    { 'S', "suffcommon", szSFcom, 63 },
    // Дополнительная информация
    EXT_QUEUE
    };
  ::ParserObjScn( teg, obj, sizeof(obj)/sizeof(STegObj) );
  //
  if (eDataTypes!=id_ModelData)
  {
	  if ( *szShift == 0 )
		{
		lstrcpy( szShift, teg[0].value );
		lstrcpy( teg[0].value, "." );
		p->nSocketNumber = def.GetActiveObjSocket();
		}
	  else
		if( *szSFcom ) //суффикс для имени
		  {
		  char szName[128];
		  sprintf_s( szName, sizeof(szName), "%s%s", teg[0].value, szSFcom );
		  p->nSocketNumber = def.AddKindEx( eDataTypes, szName );
		  }
		else
		  p->nSocketNumber = def.AddKindEx( eDataTypes, teg[0].value );
  }
  else
  {
	  //для DataST мы подставляем переменную сами, а не берем из CSh элемента по shift
	  lstrcpy(szShift,"");
  }
  p->send.eData = eDataTypes;
  //
  HPRIBOR hPribor = NULL;
  int nType = TypePribor( szImage );
  if ( nType >= 0 )
    {
    switch ( nType )
      {
      #undef  XIXI
      #define XIXI def, teg, func, p,  szImage, pt, dScale, szShift
      case  0: hPribor = FillDouble ( XIXI ); break;
      case  1: hPribor = FillBool   ( XIXI ); break;
      case  3: hPribor = FillOnOff  ( XIXI ); break;
      case  4: hPribor = FillArrow  ( XIXI ); break;
      case 10: hPribor = FillInt    ( XIXI ); break;
      case 11: hPribor = FillDbl    ( XIXI ); break;
      case 12: hPribor = FillWheel  ( XIXI ); break;
      case 13: hPribor = FillFloat  ( XIXI ); break;
      case 14: hPribor = FillPM     ( XIXI ); break;
      case 15: hPribor = FillManom  ( XIXI ); break;
      case 16: hPribor = FillOnOffOn( XIXI ); break;
	  case 17: hPribor = FillScale  ( XIXI ); break;
      }
    }
  else
    {
    ASSERT( 0 );
    }
  //
  if ( hPribor == NULL )
    return NULL;
  //
  // vladexl
  //SIZE sz = ::GiveMySize( hPribor );
  //sz.cx = int(sz.cx*dScale);
  //sz.cy = int(sz.cy*dScale);
  //if ( ::IsMnemoStyle(MNEMO_EDIT) && sz.cx > 0 && sz.cy > 0 )
  //::SetRect( &p->mRect, pt.x, pt.y, pt.x+sz.cx, pt.y+sz.cy );
  ////
  //if ( p->nSocketNumber == -1 )
  //  def.AddPribor( eDataTypes, teg[0].value, p, hPribor );
  //else
  //  def.AddPribor( hPribor );
  //
  lstrcpy( p->send.szName, def.GetModelName(p->nSocketNumber) );
  //
  return hPribor;
  }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
HPRIBOR AddPriborsPipe( DefineTags, Queue_PRS* p, LPFuncAddP func, EDataTypes eDataTypes)
  {
   p->pPipe = def.m_queue.m_pPipe;
   p->pSocket=def.m_queue.m_pSocket;
  //
  POINT pt = {0,0};
  char szImage[64] = "\0";
  char szShift[64] = "\0";
  char szSFcom[64] = "\0";
  double dScale = 1.0;
  //
  STegObj obj[] =
    {
    { 'S', "image", szImage, 63},
    { 'S', "shift", szShift, 63},
    { 'P', "pt"   , &pt        },
    { 'D', "scale", &dScale    },
    { 'S', "suffcommon", szSFcom, 63 },
    // Дополнительная информация
    EXT_QUEUE
    };
  ::ParserObjScn( teg, obj, sizeof(obj)/sizeof(STegObj) );

  //
  if (eDataTypes!=id_ModelData)
  {
      if (*szShift == 0)
      {
          lstrcpy(szShift, teg[0].value);
          lstrcpy(teg[0].value, ".");
          p->nNumber = def.GetActiveObj();
      }
      else
		if( *szSFcom ) //суффикс для имени
		  {
		  char szName[128];
		  sprintf_s( szName, sizeof(szName), "%s%s", teg[0].value, szSFcom );
		  p->nNumber = def.AddKindExPipe( eDataTypes, szName );
		  }
		else
		  p->nNumber = def.AddKindExPipe( eDataTypes, teg[0].value );
  }
  else
  {
	  //для DataST мы подставляем переменную сами, а не берем из CSh элемента по shift
	  lstrcpy(szShift,"");
  }
  p->send.eData = eDataTypes;
  //
  HPRIBOR hPribor = NULL;
  int nType = TypePribor( szImage );
  ASSERT(nType != 0);
  if ( nType >= 0 )
    {
    switch ( nType )
      {
      #undef  XIXI
      #define XIXI def, teg, func, p,  szImage, pt, dScale, szShift
      case  0: hPribor = FillDouble ( XIXI ); break;
      case  1: hPribor = FillBool   ( XIXI ); break;
      case  3: hPribor = FillOnOff  ( XIXI ); break;
      case  4: hPribor = FillArrow  ( XIXI ); break;
      case 10: hPribor = FillInt    ( XIXI ); break;
      case 11: hPribor = FillDbl    ( XIXI ); break;
      case 12: hPribor = FillWheel  ( XIXI ); break;
      case 13: hPribor = FillFloat  ( XIXI ); break;
      case 14: hPribor = FillPM     ( XIXI ); break;
      case 15: hPribor = FillManom  ( XIXI ); break;
      case 16: hPribor = FillOnOffOn( XIXI ); break;
	  case 17: hPribor = FillScale  ( XIXI ); break;
      }
    }
  //
  if ( hPribor == NULL )
    return NULL;
  //
  // vladexl
  //SIZE sz = ::GiveMySize( hPribor );
  //sz.cx = int(sz.cx*dScale);
  //sz.cy = int(sz.cy*dScale);
  //if ( ::IsMnemoStyle(MNEMO_EDIT) && sz.cx > 0 && sz.cy > 0 )
  //::SetRect( &p->mRect, pt.x, pt.y, pt.x+sz.cx, pt.y+sz.cy );
  //
  //if ( p->nNumber == -1 )
  //  def.AddPriborPipe( eDataTypes, teg[0].value, p, hPribor );
  //else
  //  def.AddPribor( hPribor );
  //
  lstrcpy( p->send.szName, def.GetModelNamePipe(p->nNumber) );
  //
  return hPribor;
  }