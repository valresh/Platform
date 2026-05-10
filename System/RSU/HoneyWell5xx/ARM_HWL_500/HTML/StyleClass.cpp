// #include "StdAfx.h"
// #include "Tag.h"
// #include "FormatScn.h"
// #include "Hdx.h"
// #include "HTML.h"

// EStyleClass GiveClass( const char* value )
//   {
//   if ( !*value ) return clsUnknown;
//   // General_Style - ??????? ?????????
//   // PointName_Style - ???
//   // Description_Style - ???????????
//   // DataEntry_Alphanumeric_Style - EditBox
//   struct SLocal { const char* name; EStyleClass type; };
//   static SLocal list[] =
//     {
//     #undef  S_CLASS
//     #define S_CLASS( Name, Text ) { Text, cls##Name },
//     #include "StyleClass.hpp"
//     // ???????
//     { "Tab_Text", clsHyperLink },
//     { "Readonly_Alpha_RightAlign_Style", clsReadOnlyR },
//     }InitLocal;
//   //
//   SLocal* find = FindLocal( value );
//   if ( find != NULL )
//   return find->type;
//   return clsUnknown;
//   }

// bool CTag::FillClass( CHTML& html )
//   {
//   CStyle& style = Style(html);
  
//   char* id = style.NameID( html );
//   if ( lstrcmp( id, "shape016_TxtEUDesc" ) == 0 )
//     AAA();
//   const char* name = Alias( html, this, "Tagname" );
//   //if ( lstrcmp( name, "Tagname" ) == 0 )
//   //  name = Alias( html, this, "Point" );
//   const char* value = NULL;
//   switch ( style.mClass )
//     {
//     case clsTagName  :
//       m_constValue = name;
//       m_constValue.MakeUpper();
//     break;
//     case clsTagEuDesc:
//       value = Alias( html, this, "cp_eudesc" );
//       if ( name != NULL && value != NULL )
//         mVal = ::FindExperion( html, name, value );
//     break;
//     case clsPV       :
//       value = Alias( html, this, "cp_pv" );
//       if ( name != NULL && value != NULL )
//         mVal = ::FindExperion( html, name, value );
//       value = Alias( html, this, "cp_pvformat" );
//       if ( name != NULL && value != NULL )
//         mDbl = ::FindExperion( html, name, value );
//     break;
//     case clsAlarmType:
//       if ( name != NULL )
//         mVal = ::FindExperion( html, name, "PVSTS" );
//     break;
//     default:
//       return false;
//     };
//   return true;
//   }

// int CTag::StyleClass( HDC hDC, CHTML& html )
//   {
//   CStyle& style = Style(html);
//   char* id = style.NameID( html );
//   if ( lstrcmp( id, "oval016" ) == 0 )
//     AAA();
//   switch ( style.mClass )
//     {
//     case clsUnknown:
//     return 1;
//     case clsSimState:
//     return -1;
//     case clsTitleBarR:
//     return clsTitleBarR;
//     case clsAlarmEnbS:
//     return clsAlarmEnbS;
//     }
//   return 1;
//   }
