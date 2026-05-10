#include <stdio.h>

#define DECLARE_PRIVATE_VARSINFO(ClassObject,DimV) \
  typedef ClassObject ClassObject_; \
  static ClassObject s_private; \
  static SVarInfo s_privateVarInfo[DimV]; \
  static DWORD s_kprivateVarInfo = 0; \
  static SVarInfo::tFlagType_ s_defPrivateFlag = SVarInfo::efEmpty;\
  static void InitPrivateInfo( eVarType type, tShiftType_ offset, tVarSizeType_ size, LPCSTR pszVarName ) \
  { \
      if( s_kprivateVarInfo == countof(s_privateVarInfo) )  \
        throw 1; \
      SVarInfo * pV = &(s_privateVarInfo[s_kprivateVarInfo++]); \
      pV->shift = offset; \
      pV->pszName = pszVarName; \
      pV->evtType = type; \
      pV->size = size; \
      if( sizeof(s_private) < (offset+size) ) \
        throw 2; \
      pV->flags |= s_defPrivateFlag; \
  }

#undef PrivateAnalog32
#define PrivateAnalog32( Var ) \
  InitPrivateInfo( evtAnalog32, offsetof(ClassObject_, Var), sizeof(((ClassObject_*)0)->Var), #Var ); 

#undef PrivateAnalog32Savebale
#define PrivateAnalog32Savebale( Var ) \
  s_defPrivateFlag |= SVarInfo::efSaveInState;\
  InitPrivateInfo( evtAnalog32, offsetof(ClassObject_, Var), sizeof(((ClassObject_*)0)->Var), #Var );\
  s_defPrivateFlag &= ~SVarInfo::efSaveInState;

#undef PrivateDigital32
#define PrivateDigital32( Var ) \
  InitPrivateInfo( evtDigital32, offsetof(ClassObject_, Var), sizeof(((ClassObject_*)0)->Var), #Var ); 

#undef PrivateDigital32Savebale
#define PrivateDigital32Savebale( Var ) \
  s_defPrivateFlag |= SVarInfo::efSaveInState;\
  InitPrivateInfo( evtDigital32, offsetof(ClassObject_, Var), sizeof(((ClassObject_*)0)->Var), #Var );\
  s_defPrivateFlag &= ~SVarInfo::efSaveInState;

#undef PrivateChars
#define PrivateChars(Var,Dim)\
  InitPrivateInfo( evtChars, offsetof(ClassObject_, Var), sizeof(tChars)*Dim, #Var ); 

#undef PrivateConstString
#define PrivateConstString(Var)\
  InitPrivateInfo( evtPrivConstString, offsetof(ClassObject_, Var), sizeof(((ClassObject_*)0)->Var), #Var ); 


#undef PrivateFieldStructAnalog32Savebale
#define PrivateFieldStructAnalog32Savebale( Struct, Field ) \
  s_defPrivateFlag |= SVarInfo::efSaveInState;\
  InitPrivateInfo( evtAnalog32, (tShiftType_)offsetof(ClassObject_, Struct.Field), sizeof(((ClassObject_*)0)->Struct.Field), #Struct"."#Field );\
  s_defPrivateFlag &= ~SVarInfo::efSaveInState;

#undef PrivateFieldStructDigital32Savebale
#define PrivateFieldStructDigital32Savebale( Struct, Field ) \
  s_defPrivateFlag |= SVarInfo::efSaveInState;\
  InitPrivateInfo( evtDigital32, (tShiftType_)offsetof(ClassObject_, Struct.Field), sizeof(((ClassObject_*)0)->Struct.Field), #Struct"."#Field );\
  s_defPrivateFlag &= ~SVarInfo::efSaveInState;

#ifdef _WIN32
#undef PrivateFieldStructArrAnalog32Savebale
#define PrivateFieldStructArrAnalog32Savebale( Struct, index, Field, name ) \
  s_defPrivateFlag |= SVarInfo::efSaveInState;\
  InitPrivateInfo( evtAnalog32, (tShiftType_)offsetof(ClassObject_, Struct[index].Field), sizeof(((ClassObject_*)0)->Struct[index].Field), name );\
  s_defPrivateFlag &= ~SVarInfo::efSaveInState;

#undef PrivateFieldStructArrDigital32Savebale
#define PrivateFieldStructArrDigital32Savebale( Struct, index, Field, name ) \
  s_defPrivateFlag |= SVarInfo::efSaveInState;\
  InitPrivateInfo( evtDigital32, (tShiftType_)offsetof(ClassObject_, Struct[index].Field), sizeof(((ClassObject_*)0)->Struct[index].Field), name );\
  s_defPrivateFlag &= ~SVarInfo::efSaveInState;
#else
#undef PrivateFieldStructArrAnalog32Savebale
#define PrivateFieldStructArrAnalog32Savebale(Struct, index, Field, name) \
  s_defPrivateFlag |= SVarInfo::efSaveInState; \
  InitPrivateInfo( \
    evtAnalog32, \
    (tShiftType_)(offsetof(ClassObject_, Struct[0].Field) + index * sizeof(((ClassObject_*)0)->Struct[0])), \
    (tVarSizeType_)sizeof(((ClassObject_*)0)->Struct[0].Field), \
    name ); \
  s_defPrivateFlag &= ~SVarInfo::efSaveInState;

#undef PrivateFieldStructArrDigital32Savebale
#define PrivateFieldStructArrDigital32Savebale(Struct, index, Field, name) \
  s_defPrivateFlag |= SVarInfo::efSaveInState; \
  InitPrivateInfo( \
    evtDigital32, \
    (tShiftType_)(offsetof(ClassObject_, Struct[0].Field) + index * sizeof(((ClassObject_*)0)->Struct[0])), \
    (tVarSizeType_)sizeof(((ClassObject_*)0)->Struct[0].Field), \
    name ); \
  s_defPrivateFlag &= ~SVarInfo::efSaveInState;
#endif

//////////////////////////////////////////////////////////////////////////
#define DECLARE_SHARED_VARSINFO(ClassObject,DimV) \
  typedef ClassObject ClassObjectShared_; \
  static ClassObject s_shared; \
  static SVarInfo s_sharedVarInfo[DimV]; \
  static DWORD s_ksharedVarInfo = 0; \
  static SVarInfo::tFlagType_ s_defSharedFlag = SVarInfo::efEmpty; \
  static void InitSharedInfo( eVarType type, tShiftType_ offset, tVarSizeType_ size, LPCSTR pszVarName ) \
  { \
      if( s_ksharedVarInfo == countof(s_sharedVarInfo) ) \
        throw 1; \
      SVarInfo * pV = &(s_sharedVarInfo[s_ksharedVarInfo++]); \
      pV->shift = offset; \
      pV->pszName = pszVarName; \
      pV->evtType = type; \
      pV->size = size; \
      if( sizeof(s_shared) < (offset+size) ) \
        throw 2; \
      pV->flags |= s_defSharedFlag; \
  }

#undef SharedAnalog32
#define SharedAnalog32( Var ) \
  InitSharedInfo( evtAnalog32, offsetof(ClassObjectShared_, Var), sizeof(((ClassObjectShared_*)0)->Var), #Var ); 

#undef SharedAnalog32Savebale
#define SharedAnalog32Savebale( Var ) \
  s_defSharedFlag |= SVarInfo::efSaveInState;\
  InitSharedInfo( evtAnalog32, offsetof(ClassObjectShared_, Var), sizeof(((ClassObjectShared_*)0)->Var), #Var );\
  s_defSharedFlag &= ~SVarInfo::efSaveInState;

#ifdef _WIN32
#undef SharedArrayAnalog32
#define SharedArrayAnalog32( Var, Dim ) \
{\
  static char sName[Dim][64];\
  for( int i=0; i<Dim; ++i )\
  { sprintf_s(sName[i], "%s[%d]", #Var, i);\
  InitSharedInfo( evtAnalog32, (tShiftType_)offsetof(ClassObjectShared_, Var[i]), sizeof(((ClassObjectShared_*)0)->Var[i]), sName[i] );}\
}
#else
#undef SharedArrayAnalog32
#define SharedArrayAnalog32( Var, Dim ) \
{ \
  static char sName[Dim][64]; \
  for( int i=0; i<Dim; ++i ) \
  { \
    sprintf_s(sName[i], "%s[%d]", #Var, i); \
    InitSharedInfo( \
      evtAnalog32, \
      (tShiftType_)(offsetof(ClassObjectShared_, Var[0]) + i * sizeof(((ClassObjectShared_*)0)->Var[0])), \
      (tVarSizeType_)sizeof(((ClassObjectShared_*)0)->Var[0]), \
      sName[i] ); \
  } \
}
#endif

#undef SharedDigital32
#define SharedDigital32( Var ) \
  InitSharedInfo( evtDigital32, offsetof(ClassObjectShared_, Var), sizeof(((ClassObjectShared_*)0)->Var), #Var ); 

#undef SharedDigital32Savebale
#define SharedDigital32Savebale( Var ) \
  s_defSharedFlag |= SVarInfo::efSaveInState;\
  InitSharedInfo( evtDigital32, offsetof(ClassObjectShared_, Var), sizeof(((ClassObjectShared_*)0)->Var), #Var );\
  s_defSharedFlag &= ~SVarInfo::efSaveInState;

#undef SharedDigital64
#define SharedDigital64( Var ) \
  InitSharedInfo( evtDigital64, offsetof(ClassObjectShared_, Var), sizeof(((ClassObjectShared_*)0)->Var), #Var ); 

#undef SharedChars
#define SharedChars(Var,Dim) \
  InitSharedInfo( evtChars, offsetof(ClassObjectShared_, Var), sizeof(tChars)*Dim, #Var ); 

#undef SharedCharsSavebale
#define SharedCharsSavebale(Var,Dim) \
  s_defSharedFlag |= SVarInfo::efSaveInState;\
  InitSharedInfo( evtChars, offsetof(ClassObjectShared_, Var), sizeof(tChars)*Dim, #Var ); \
  s_defSharedFlag &= ~SVarInfo::efSaveInState;

#undef SharedAnalog64
#define SharedAnalog64( Var ) \
  InitSharedInfo( evtAnalog64, offsetof(ClassObjectShared_, Var), sizeof(((ClassObjectShared_*)0)->Var), #Var ); 

#undef SharedAnalog64Savebale
#define SharedAnalog64Savebale( Var ) \
  s_defSharedFlag |= SVarInfo::efSaveInState;\
  InitSharedInfo( evtAnalog64, offsetof(ClassObjectShared_, Var), sizeof(((ClassObjectShared_*)0)->Var), #Var );\
  s_defSharedFlag &= ~SVarInfo::efSaveInState;
