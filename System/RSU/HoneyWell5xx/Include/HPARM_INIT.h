#undef LIST_PARM

#pragma warning(disable: 4101)
#ifdef _DEBUG
#define _CHECK_V_SIZE ASS( sizeof(V) < USHRT_MAX );
#else
#define _CHECK_V_SIZE
#endif

#define LIST_PARM(BaseClass,Class,DimV) \
  static Class _W;\
  typedef Class _Class;\
  static SVarInfo VarInfo[DimV];\
  static int kVarInfo = 0;\
  void BaseClass::SetParmList( )\
  {\
    if( !kVarInfo ) \
      BaseClass::InitParm();\
    ClassVarInfo = VarInfo;\
    kClassVarInfo = kVarInfo;\
    initW( W, &_W );\
    pClass_FullW = ((BYTE*)static_cast<Class*>(W)) + sizeof(CBase);\
    L_Class_FullW = sizeof (Class) - sizeof(CBase);\
    pThisAddr = (BYTE*)this;\
  }\
static SVarInfo::eFlags s_defFlag = SVarInfo::efVar;\
template<class T, class V>\
SVarInfo* InitInfo( eVarType type, T &_W, V *var, LPCSTR pszVarName, V def)\
{\
  ASS( *pszVarName )\
  *var = def;\
  ASS( kVarInfo < _countof(VarInfo) );\
  SVarInfo * pV = &(VarInfo[kVarInfo++]);\
  pV->sVar =(DWORD)((BYTE*)var - (BYTE*)&_W);\
  pV->VarName = pszVarName;\
  pV->Type = type;\
  _CHECK_V_SIZE\
  pV->varSize = sizeof(V);\
  pV->flags |= s_defFlag;\
  return pV;\
}\
template<class T, class V>\
SVarInfo* InitInfo( eVarType type, T &_W, V *var, LPCSTR pszVarName)\
{\
  ASS( *pszVarName )\
  ASS( kVarInfo < _countof(VarInfo) );\
  SVarInfo * pV = &(VarInfo[kVarInfo++]);\
  pV->sVar =(DWORD)((BYTE*)var - (BYTE*)&_W);\
  pV->VarName = pszVarName;\
  pV->Type = type;\
  _CHECK_V_SIZE\
  pV->varSize = sizeof(V);\
  pV->flags |= s_defFlag;\
  return pV;\
}\
template<class T, class V>\
SVarInfo* InitInfoEnum( eVarType type, T &_W, V *var, LPCSTR pszVarName, BYTE def, LPCSTR penumVals)\
{\
  ASS( *pszVarName )\
  *var = def;\
  ASS( kVarInfo < _countof(VarInfo) );\
  SVarInfo * pV = &(VarInfo[kVarInfo++]);\
  pV->sVar =(DWORD)((BYTE*)var - (BYTE*)&_W);\
  pV->VarName = pszVarName;\
  pV->Type = type;\
  pV->flags |= SVarInfo::efEnum;\
  pV->pszEnumVals = penumVals;\
  _CHECK_V_SIZE\
  pV->varSize = sizeof(V);\
  pV->flags |= s_defFlag;\
  return pV;\
}\

template<size_t _S>
void makeName( char (&res)[_S], LPCSTR pFormat, LPCSTR pv, LPCSTR pf )
{
  sprintf_s( res, pFormat, pv, pf );
  _Ktoupper(res);
}
template<size_t _S>
void makeName( char (&res)[_S], LPCSTR pFormat, LPCSTR pv, int i )
{
  sprintf_s( res, pFormat, pv, i );
  _Ktoupper(res);
}
template<size_t _S>
void makeName( char (&res)[_S], LPCSTR pFormat, LPCSTR pv, LPCSTR pf, int i )
{
  sprintf_s( res, pFormat, pv, pf, i );
  _Ktoupper(res);
}
template<size_t _S>
void makeName_A( char (&res)[_S], LPCSTR pFormat, LPCSTR pv, int i, LPCSTR pf )
{
  sprintf_s( res, pFormat, pv, i, pf );
  _Ktoupper(res);
}
//////////////////////////////////////////////////////////////////////////
#undef Real_A
#define Real_A(Var,Desc,Len,Def)\
  static char Var##Name[Len+1][24*4]; \
  {SVarInfo *p=InitInfo( evtDouble, _W, &_W.Var[0], #Var, (double)Def);p->varSize*=(Len+1);p->flags|=SVarInfo::efParam;}\
  for( int i=0; i<_countof(Var##Name); ++i ) \
  {\
    makeName( Var##Name[i], "%s[%d]", #Var, i );\
    InitInfo( evtDouble, _W, &_W.Var[i], Var##Name[i], (double)Def);\
  }

#undef Real
#define Real(Var,Desc,Def) InitInfo( evtDouble, _W, &_W.Var, #Var, (double)Def);
//////////////////////////////////////////////////////////////////////////
#undef Boolean
#define Boolean(Var,Desc,Def) InitInfo( evtBool, _W, &_W.Var, #Var, (BYTE)Def);

#undef BooleanEx2
#define BooleanEx2(Var,Desc,Def,Name2) InitInfo( evtBool, _W, &_W.Var, #Var, (BYTE)Def); InitInfo( evtBool, _W, &_W.Var, #Name2, (BYTE)Def);

#undef Boolean_A
#define Boolean_A(Var,Desc,Len,Def)\
  static char Var##Name[Len+1][24*4]; \
  {SVarInfo *p=InitInfo( evtBool, _W, &_W.Var[0], #Var, (BYTE)Def);p->varSize*=(Len+1);p->flags|=SVarInfo::efParam;}\
  for( int i=0; i<_countof(Var##Name); ++i ) \
  {\
    makeName( Var##Name[i], "%s[%d]", #Var, i );\
    InitInfo( evtBool, _W, &_W.Var[i], Var##Name[i], (BYTE)Def);\
  }

#undef Boolean_AA
#define Boolean_AA(Var,Desc,Len1,Len2,Def)\
  static char Var##Name[Len1+1][Len2+1][24*4]; \
  for( int i=0; i<_countof(Var##Name); ++i ) \
  {\
    for( int j=0; j<_countof(Var##Name[0]); ++j ) \
    {\
      sprintf_s( Var##Name[i][j], "%s[%d,%d]", #Var, i, j );\
      InitInfo( evtBool, _W, &_W.Var[i][j], Var##Name[i][j], (BYTE)Def);\
    }\
  }
//////////////////////////////////////////////////////////////////////////
#undef String_K
#define String_K(Var,Comm,Len) \
{SVarInfo* pV = InitInfo( evtString, _W, &_W.Var, #Var); pV->varSize = Len; }

#undef String_A
#define String_A(Var,Comm,Len,Count)\
  static char Var##Name[Count+1][24*4]; \
  {SVarInfo *p=InitInfo( evtString, _W, &_W.Var[0], #Var);p->varSize*=((Len+1)*(Count+1));p->flags|=SVarInfo::efParam;}\
  for( int i=0; i<_countof(Var##Name); ++i ) \
  {\
    makeName( Var##Name[i], "%s[%d]", #Var, i );\
    SVarInfo* pV = InitInfo( evtString, _W, &_W.Var[i], Var##Name[i]);\
    pV->varSize = Len;\
  }
//////////////////////////////////////////////////////////////////////////
#undef Integer
#define Integer(Var,Desc,Def) InitInfo( evtInt, _W, &_W.Var, #Var, (int)Def);

#undef Integer_A
#define Integer_A(Var,Desc,Len,Def)\
  static char Var##Name[Len+1][24*4]; \
  {SVarInfo *p=InitInfo( evtInt, _W, &_W.Var[0], #Var, (int)Def);p->varSize*=(Len+1);p->flags|=SVarInfo::efParam;}\
  for( int i=0; i<_countof(Var##Name); ++i ) \
  {\
    makeName( Var##Name[i], "%s[%d]", #Var, i );\
    InitInfo( evtInt, _W, &_W.Var[i], Var##Name[i], (int)Def);\
  }
//////////////////////////////////////////////////////////////////////////
#undef UInt64
#define UInt64(Var,Desc,Def) InitInfo( evtUInt64, _W, &_W.Var, #Var, (UInt64)Def);
//////////////////////////////////////////////////////////////////////////
#undef Time
#define Time(Var,Desc,Def) InitInfo( evtTime, _W, &_W.Var, #Var, (time_t)Def);

#undef Time_A
#define Time_A(Var,Desc,Len,Def)\
  static char Var##Name[Len+1][24*4]; \
  {SVarInfo *p=InitInfo( evtTime, _W, &_W.Var[0], #Var, (time_t)Def);p->varSize*=(Len+1);p->flags|=SVarInfo::efParam;}\
  for( int i=0; i<_countof(Var##Name); ++i ) \
  {\
    makeName( Var##Name[i], "%s[%d]", #Var, i );\
    InitInfo( evtTime, _W, &_W.Var[i], Var##Name[i], (time_t)Def);\
  }
//////////////////////////////////////////////////////////////////////////
#undef  Enumeration
#define Enumeration(Var,Desc,...) InitInfoEnum( evtByte, _W, &_W.Var, #Var, _Class::_##Var::Def, #__VA_ARGS__ );

#undef  Enumeration16
#define Enumeration16(Var,Desc,...) InitInfoEnum( evtUShort, _W, &_W.Var, #Var, _Class::_##Var::Def, #__VA_ARGS__ );

#undef  Enumeration_A
#define Enumeration_A(Var,Desc,Len,...)\
  static char Var##Name[Len+1][24*4]; \
  for( int i=0; i<_countof(Var##Name); ++i ) \
  {\
    makeName( Var##Name[i], "%s[%d]", #Var, i );\
    InitInfoEnum( evtByte, _W, &_W.Var[i], Var##Name[i], _Class::_##Var::Def, #__VA_ARGS__ );\
  }
//////////////////////////////////////////////////////////////////////////
#undef StructRE
#define StructRE(Var,Desc,realVal, enumVal )\
  static char Var##realVal[24*4];\
  makeName( Var##realVal, "%s.%s", #Var, #realVal );\
  InitInfo( evtDouble, _W, &_W.Var.realVal, Var##realVal, (double)0);\
  static char Var##enumVal[24*4];\
  makeName( Var##enumVal, "%s.%s", #Var, #enumVal );\
  InitInfoEnum( evtByte, _W, &_W.enumVal, Var##enumVal, _Class::_##enumVal::Def, _Class::_##enumVal::GetVals() );
//////////////////////////////////////////////////////////////////////////
#undef StructRB
#define StructRB(Var,Desc,realVal, bVal)\
  static char Var##realVal[24*4];\
  makeName( Var##realVal, "%s.%s", #Var, #realVal );\
  InitInfo( evtDouble, _W, &_W.Var.realVal, Var##realVal, (double)0);\
  static char Var##bVal[24*4];\
  makeName( Var##bVal, "%s.%s", #Var, #bVal );\
  InitInfo( evtBool, _W, &_W.Var.bVal, Var##bVal, (BYTE)0);
//////////////////////////////////////////////////////////////////////////
#undef StructBE
#define StructBE(Var,Desc,bVal, enumVal )\
  static char Var##bVal[24*4];\
  makeName( Var##bVal, "%s.%s", #Var, #bVal );\
  InitInfo( evtBool, _W, &_W.Var.bVal, Var##bVal, (BYTE)0);\
  static char Var##enumVal[24*4];\
  makeName( Var##enumVal, "%s.%s", #Var, #enumVal );\
  InitInfoEnum( evtByte, _W, &_W.enumVal, Var##enumVal, _Class::_##enumVal::Def, _Class::_##enumVal::GetVals() );
//////////////////////////////////////////////////////////////////////////
#undef Struct3B
#define Struct3B(Var,Desc,Val1,Val2,Val3 )\
  static char Var##Val1[24*4];\
  makeName( Var##Val1, "%s.%s", #Var, #Val1 );\
  InitInfo( evtBool, _W, &_W.Var.Val1, Var##Val1, (BYTE)0);\
  static char Var##Val2[24*4];\
  makeName( Var##Val2, "%s.%s", #Var, #Val2 );\
  InitInfo( evtBool, _W, &_W.Var.Val2, Var##Val2, (BYTE)0);\
  static char Var##Val3[24*4];\
  makeName( Var##Val3, "%s.%s", #Var, #Val3 );\
  InitInfo( evtBool, _W, &_W.Var.Val3, Var##Val3, (BYTE)0);
//////////////////////////////////////////////////////////////////////////
#undef Struct4B
#define Struct4B(Var,Desc,Val1,Val2,Val3,Val4 )\
  static char Var##Val1[24*4];\
  makeName( Var##Val1, "%s.%s", #Var, #Val1 );\
  InitInfo( evtBool, _W, &_W.Var.Val1, Var##Val1, (BYTE)0);\
  static char Var##Val2[24*4];\
  makeName( Var##Val2, "%s.%s", #Var, #Val2 );\
  InitInfo( evtBool, _W, &_W.Var.Val2, Var##Val2, (BYTE)0);\
  static char Var##Val3[24*4];\
  makeName( Var##Val3, "%s.%s", #Var, #Val3 );\
  InitInfo( evtBool, _W, &_W.Var.Val3, Var##Val3, (BYTE)0);\
  static char Var##Val4[24*4];\
  makeName( Var##Val4, "%s.%s", #Var, #Val4 );\
  InitInfo( evtBool, _W, &_W.Var.Val4, Var##Val4, (BYTE)0);
//////////////////////////////////////////////////////////////////////////
#undef Struct5B
#define Struct5B(Var,Desc,Val1,Val2,Val3,Val4,Val5 )\
  static char Var##Val1[24*4];\
  makeName( Var##Val1, "%s.%s", #Var, #Val1 );\
  InitInfo( evtBool, _W, &_W.Var.Val1, Var##Val1, (BYTE)0);\
  static char Var##Val2[24*4];\
  makeName( Var##Val2, "%s.%s", #Var, #Val2 );\
  InitInfo( evtBool, _W, &_W.Var.Val2, Var##Val2, (BYTE)0);\
  static char Var##Val3[24*4];\
  makeName( Var##Val3, "%s.%s", #Var, #Val3 );\
  InitInfo( evtBool, _W, &_W.Var.Val3, Var##Val3, (BYTE)0);\
  static char Var##Val4[24*4];\
  makeName( Var##Val4, "%s.%s", #Var, #Val4 );\
  InitInfo( evtBool, _W, &_W.Var.Val4, Var##Val4, (BYTE)0);\
  static char Var##Val5[24*4];\
  makeName( Var##Val5, "%s.%s", #Var, #Val5 );\
  InitInfo( evtBool, _W, &_W.Var.Val5, Var##Val5, (BYTE)0);
//////////////////////////////////////////////////////////////////////////
#undef Struct2B
#define Struct2B(Var,Desc,Val1,Val2 )\
  static char Var##Val1[24*4];\
  makeName( Var##Val1, "%s.%s", #Var, #Val1 );\
  InitInfo( evtBool, _W, &_W.Var.Val1, Var##Val1, (BYTE)0);\
  static char Var##Val2[24*4];\
  makeName( Var##Val2, "%s.%s", #Var, #Val2 );\
  InitInfo( evtBool, _W, &_W.Var.Val2, Var##Val2, (BYTE)0);
//////////////////////////////////////////////////////////////////////////
#undef Struct_BEI
#define Struct_BEI(Var,ValB,tValE,ValE,ValI)\
  static char Var##ValB[24*4];\
  makeName( Var##ValB, "%s.%s", #Var, #ValB );\
  InitInfo( evtBool, _W, &_W.Var.ValB, Var##ValB, (BYTE)0);\
  static char Var##ValE[24*4];\
  makeName( Var##ValE, "%s.%s", #Var, #ValE );\
  InitInfoEnum( evtByte, _W, &_W.Var.ValE, Var##ValE, ::__##tValE::Def, ::__##tValE::GetVals() );\
  static char Var##ValI[24*4];\
  makeName( Var##ValI, "%s.%s", #Var, #ValI );\
  InitInfo( evtInt, _W, &_W.Var.ValI, Var##ValI, (int)0);
//////////////////////////////////////////////////////////////////////////
#undef Struct_BEIII
#define Struct_BEIII(Var,ValB,tValE,ValE,ValI,ValI2,ValI3)\
  static char Var##ValB[24*4];\
  makeName( Var##ValB, "%s.%s", #Var, #ValB );\
  InitInfo( evtBool, _W, &_W.Var.ValB, Var##ValB, (BYTE)0);\
  static char Var##ValE[24*4];\
  makeName( Var##ValE, "%s.%s", #Var, #ValE );\
  InitInfoEnum( evtByte, _W, &_W.Var.ValE, Var##ValE, ::__##tValE::Def, ::__##tValE::GetVals() );\
  static char Var##ValI[24*4];\
  makeName( Var##ValI, "%s.%s", #Var, #ValI );\
  InitInfo( evtInt, _W, &_W.Var.ValI, Var##ValI, (int)0);\
  static char Var##ValI2[24*4];\
  makeName( Var##ValI2, "%s.%s", #Var, #ValI2 );\
  InitInfo( evtInt, _W, &_W.Var.ValI2, Var##ValI2, (int)0);\
  static char Var##ValI3[24*4];\
  makeName( Var##ValI3, "%s.%s", #Var, #ValI3 );\
  InitInfo( evtInt, _W, &_W.Var.ValI3, Var##ValI3, (int)0);
//////////////////////////////////////////////////////////////////////////
#undef Struct_EIE
#define Struct_EIE(Var,tValB,ValB,ValI,tValE,ValE)\
  static char Var##ValB[24*4];\
  makeName( Var##ValB, "%s.%s", #Var, #ValB );\
  InitInfoEnum( evtByte, _W, &_W.Var.ValB, Var##ValB, ::__##tValB::Def, ::__##tValB::GetVals() );\
  static char Var##ValI[24*4];\
  makeName( Var##ValI, "%s.%s", #Var, #ValI );\
  InitInfo( evtInt, _W, &_W.Var.ValI, Var##ValI, (int)0);\
  static char Var##ValE[24*4];\
  makeName( Var##ValE, "%s.%s", #Var, #ValE );\
  InitInfoEnum( evtByte, _W, &_W.Var.ValE, Var##ValE, ::__##tValE::Def, ::__##tValE::GetVals() );
//////////////////////////////////////////////////////////////////////////
#undef Struct_EEEE
#define Struct_EEEE(Var,tValE1,ValE1,tValE2,ValE2,tValE3,ValE3,tValE4,ValE4)\
  static char Var##ValE1[24*4];\
  makeName( Var##ValE1, "%s.%s", #Var, #ValE1 );\
  InitInfoEnum( evtByte, _W, &_W.Var.ValE1, Var##ValE1, ::__##tValE1::Def, ::__##tValE1::GetVals() );\
  static char Var##ValE2[24*4];\
  makeName( Var##ValE2, "%s.%s", #Var, #ValE2 );\
  InitInfoEnum( evtByte, _W, &_W.Var.ValE2, Var##ValE2, ::__##tValE2::Def, ::__##tValE2::GetVals() );\
  static char Var##ValE3[24*4];\
  makeName( Var##ValE3, "%s.%s", #Var, #ValE3 );\
  InitInfoEnum( evtByte, _W, &_W.Var.ValE3, Var##ValE3, ::__##tValE3::Def, ::__##tValE3::GetVals() );\
  static char Var##ValE4[24*4];\
  makeName( Var##ValE4, "%s.%s", #Var, #ValE4 );\
  InitInfoEnum( evtByte, _W, &_W.Var.ValE4, Var##ValE4, ::__##tValE4::Def, ::__##tValE4::GetVals() );
//////////////////////////////////////////////////////////////////////////
#undef Struct_EI
#define Struct_EI(Var,tValB,ValB,ValI)\
  static char Var##ValB[24*4];\
  makeName( Var##ValB, "%s.%s", #Var, #ValB );\
  InitInfoEnum( evtByte, _W, &_W.Var.ValB, Var##ValB, ::__##tValB::Def, ::__##tValB::GetVals() );\
  static char Var##ValI[24*4];\
  makeName( Var##ValI, "%s.%s", #Var, #ValI );\
  InitInfo( evtInt, _W, &_W.Var.ValI, Var##ValI, (int)0);
//////////////////////////////////////////////////////////////////////////
#undef Struct_EB
#define Struct_EB(Var,tValE,ValE,ValB)\
  static char Var##ValE[24*4];\
  makeName( Var##ValE, "%s.%s", #Var, #ValE );\
  InitInfoEnum( evtByte, _W, &_W.Var.ValE, Var##ValE, ::__##tValE::Def, ::__##tValE::GetVals() );\
  static char Var##ValB[24*4];\
  makeName( Var##ValB, "%s.%s", #Var, #ValB );\
  InitInfo( evtBool, _W, &_W.Var.ValB, Var##ValB, (BYTE)0);
//////////////////////////////////////////////////////////////////////////
#undef Struct_REBEIIR
#define Struct_REBEIIR(Var,R1,tE2,E2,B3,tE4,E4,I5,I6,R7)\
  static char Var##R1[24*4];\
  makeName( Var##R1, "%s.%s", #Var, #R1 );\
  InitInfo( evtDouble, _W, &_W.Var.R1, Var##R1, (double)0);\
  static char Var##E2[24*4];\
  makeName( Var##E2, "%s.%s", #Var, #E2 );\
  InitInfoEnum( evtByte, _W, &_W.Var.E2, Var##E2, ::__##tE2::Def, ::__##tE2::GetVals() );\
  static char Var##B3[24*4];\
  makeName( Var##B3, "%s.%s", #Var, #B3 );\
  InitInfo( evtBool, _W, &_W.Var.B3, Var##B3, (BYTE)0);\
  static char Var##E4[24*4];\
  makeName( Var##E4, "%s.%s", #Var, #E4 );\
  InitInfoEnum( evtByte, _W, &_W.Var.E4, Var##E4, ::__##tE4::Def, ::__##tE4::GetVals() );\
  static char Var##I5[24*4];\
  makeName( Var##I5, "%s.%s", #Var, #I5 );\
  InitInfo( evtInt, _W, &_W.Var.I5, Var##I5, (int)0);\
  static char Var##I6[24*4];\
  makeName( Var##I6, "%s.%s", #Var, #I6 );\
  InitInfo( evtInt, _W, &_W.Var.I6, Var##I6, (int)0);\
  static char Var##R7[24*4];\
  makeName( Var##R7, "%s.%s", #Var, #R7 );\
  InitInfo( evtDouble, _W, &_W.Var.R7, Var##R7, (double)0);
//////////////////////////////////////////////////////////////////////////
#undef Struct_REBEIIIR
#define Struct_REBEIIIR(Var,R1,tE2,E2,B3,tE4,E4,I5,I6,I7,R8)\
  static char Var##R1[24*4];\
  makeName( Var##R1, "%s.%s", #Var, #R1 );\
  InitInfo( evtDouble, _W, &_W.Var.R1, Var##R1, (double)0);\
  static char Var##E2[24*4];\
  makeName( Var##E2, "%s.%s", #Var, #E2 );\
  InitInfoEnum( evtByte, _W, &_W.Var.E2, Var##E2, ::__##tE2::Def, ::__##tE2::GetVals() );\
  static char Var##B3[24*4];\
  makeName( Var##B3, "%s.%s", #Var, #B3 );\
  InitInfo( evtBool, _W, &_W.Var.B3, Var##B3, (BYTE)0);\
  static char Var##E4[24*4];\
  makeName( Var##E4, "%s.%s", #Var, #E4 );\
  InitInfoEnum( evtByte, _W, &_W.Var.E4, Var##E4, ::__##tE4::Def, ::__##tE4::GetVals() );\
  static char Var##I5[24*4];\
  makeName( Var##I5, "%s.%s", #Var, #I5 );\
  InitInfo( evtInt, _W, &_W.Var.I5, Var##I5, (int)0);\
  static char Var##I6[24*4];\
  makeName( Var##I6, "%s.%s", #Var, #I6 );\
  InitInfo( evtInt, _W, &_W.Var.I6, Var##I6, (int)0);\
  static char Var##I7[24*4];\
  makeName( Var##I7, "%s.%s", #Var, #I7 );\
  InitInfo( evtInt, _W, &_W.Var.I7, Var##I7, (int)0);\
  static char Var##R8[24*4];\
  makeName( Var##R8, "%s.%s", #Var, #R8 );\
  InitInfo( evtDouble, _W, &_W.Var.R8, Var##R8, (double)0);
//////////////////////////////////////////////////////////////////////////
#undef Struct_IR
#define Struct_IR(Var,I1,R2,Def2)\
  static char Var##I1[24*4];\
  makeName( Var##I1, "%s.%s", #Var, #I1 );\
  InitInfo( evtInt, _W, &_W.Var.I1, Var##I1, (int)0);\
  static char Var##R2[24*4];\
  makeName( Var##R2, "%s.%s", #Var, #R2 );\
  InitInfo( evtDouble, _W, &_W.Var.R2, Var##R2, (double)Def2);
//////////////////////////////////////////////////////////////////////////
#undef Struct_BEIR
#define Struct_BEIR(Var,B1,tE2,E2,I3,R4)\
  static char Var##B1[24*4];\
  makeName( Var##B1, "%s.%s", #Var, #B1 );\
  InitInfo( evtBool, _W, &_W.Var.B1, Var##B1, (BYTE)0);\
  static char Var##E2[24*4];\
  makeName( Var##E2, "%s.%s", #Var, #E2 );\
  InitInfoEnum( evtByte, _W, &_W.Var.E2, Var##E2, ::__##tE2::Def, ::__##tE2::GetVals() );\
  static char Var##I3[24*4];\
  makeName( Var##I3, "%s.%s", #Var, #I3 );\
  InitInfo( evtInt, _W, &_W.Var.I3, Var##I3, (int)0);\
  static char Var##R4[24*4];\
  makeName( Var##R4, "%s.%s", #Var, #R4 );\
  InitInfo( evtDouble, _W, &_W.Var.R4, Var##R4, (double)0);
//////////////////////////////////////////////////////////////////////////
#undef Struct_BEIIa
#define Struct_BEIIa(Var,B1,tE2,E2,I3,I4arr,I4count)\
  static char Var##B1[24*4];\
  makeName( Var##B1, "%s.%s", #Var, #B1 );\
  InitInfo( evtBool, _W, &_W.Var.B1, Var##B1, (BYTE)0);\
  static char Var##E2[24*4];\
  makeName( Var##E2, "%s.%s", #Var, #E2 );\
  InitInfoEnum( evtByte, _W, &_W.Var.E2, Var##E2, ::__##tE2::Def, ::__##tE2::GetVals() );\
  static char Var##I3[24*4];\
  makeName( Var##I3, "%s.%s", #Var, #I3 );\
  InitInfo( evtInt, _W, &_W.Var.I3, Var##I3, (int)0);\
  static char Var##I4arr[I4count+1][24*4]; \
  for( int i=0; i<_countof(Var##I4arr); ++i ) \
  {\
    makeName( Var##I4arr[i], "%s.%s[%d]", #Var, #I4arr, i );\
    InitInfo( evtInt, _W, &_W.Var.I4arr[i], Var##I4arr[i], (int)0);\
  }
//////////////////////////////////////////////////////////////////////////
#undef StructA_BBEI
#define StructA_BBEI(Var,Count,B1,B2,tE3,E3,I4)\
  static char Var##B1[Count+1][24*4]; \
  for( int i=0; i<_countof(Var##B1); ++i ) \
  {\
    makeName( Var##B1[i], "%s.%s[%d]", #Var, #B1, i );\
    InitInfo( evtBool, _W, &_W.Var.B1[i], Var##B1[i], (BYTE)0);\
  }\
  static char Var##B2[Count+1][24*4]; \
  for( int i=0; i<_countof(Var##B2); ++i ) \
  {\
    makeName( Var##B2[i], "%s.%s[%d]", #Var, #B2, i );\
    InitInfo( evtBool, _W, &_W.Var.B2[i], Var##B2[i], (BYTE)0);\
  }\
  static char Var##E3[Count+1][24*4]; \
  for( int i=0; i<_countof(Var##E3); ++i ) \
  {\
    makeName( Var##E3[i], "%s.%s[%d]", #Var, #E3, i );\
    InitInfoEnum( evtByte, _W, &_W.Var.E3[i], Var##E3[i], ::__##tE3::Def, ::__##tE3::GetVals() );\
  }\
  static char Var##I4[Count+1][24*4]; \
  for( int i=0; i<_countof(Var##I4); ++i ) \
  {\
    makeName( Var##I4[i], "%s.%s[%d]", #Var, #I4, i );\
    InitInfo( evtInt, _W, &_W.Var.I4[i], Var##I4[i], (int)0);\
  }
//////////////////////////////////////////////////////////////////////////
#undef Struct_BBEI
#define Struct_BBEI(Var,B1,B2,tE3,E3,I4)\
  static char Var##B1[24*4];\
  makeName( Var##B1, "%s.%s", #Var, #B1 );\
  InitInfo( evtBool, _W, &_W.Var.B1, Var##B1, (BYTE)0);\
  static char Var##B2[24*4];\
  makeName( Var##B2, "%s.%s", #Var, #B2 );\
  InitInfo( evtBool, _W, &_W.Var.B2, Var##B2, (BYTE)0);\
  static char Var##E3[24*4];\
  makeName( Var##E3, "%s.%s", #Var, #E3 );\
  InitInfoEnum( evtByte, _W, &_W.Var.E3, Var##E3, ::__##tE3::Def, ::__##tE3::GetVals() );\
  static char Var##I4[24*4];\
  makeName( Var##I4, "%s.%s", #Var, #I4 );\
  InitInfo( evtInt, _W, &_W.Var.I4, Var##I4, (int)0);
//////////////////////////////////////////////////////////////////////////
#undef Struct_BxIx
#define Struct_BxIx(Var,B1,B1c,I2,I2c)\
  static char Var##B1[B1c+1][24*4]; \
  for( int i=0; i<_countof(Var##B1); ++i ) \
  {\
    makeName( Var##B1[i], "%s.%s[%d]", #Var, #B1, i );\
    InitInfo( evtBool, _W, &_W.Var.B1[i], Var##B1[i], (BYTE)0);\
  }\
  static char Var##I2[I2c+1][24*4]; \
  for( int i=0; i<_countof(Var##I2); ++i ) \
  {\
    makeName( Var##I2[i], "%s.%s[%d]", #Var, #I2, i );\
    InitInfo( evtInt, _W, &_W.Var.I2[i], Var##I2[i], (int)0);\
  }
//////////////////////////////////////////////////////////////////////////
#undef Struct_BxRx
#define Struct_BxRx(Var,B1,B1c,D2,D2c)\
  static char Var##B1[B1c+1][24*4]; \
  for( int i=0; i<_countof(Var##B1); ++i ) \
  {\
  makeName( Var##B1[i], "%s.%s[%d]", #Var, #B1, i );\
  InitInfo( evtBool, _W, &_W.Var.B1[i], Var##B1[i], (BYTE)0);\
  }\
  static char Var##D2[D2c+1][24*4]; \
  for( int i=0; i<_countof(Var##D2); ++i ) \
  {\
  makeName( Var##D2[i], "%s.%s[%d]", #Var, #D2, i );\
  InitInfo( evtDouble, _W, &_W.Var.D2[i], Var##D2[i], (double)0);\
  }
//////////////////////////////////////////////////////////////////////////
#undef Struct_R
#define Struct_R(Var,R2,Def2)\
  static char Var##R2[24*4];\
  makeName( Var##R2, "%s.%s", #Var, #R2 );\
  InitInfo( evtDouble, _W, &_W.Var.R2, Var##R2, (double)Def2);
//////////////////////////////////////////////////////////////////////////
#undef Struct_3R
#define Struct_3R(Var,Desc,Val1,Val2,Val3)\
  static char Var##Val1[24*4];\
  makeName( Var##Val1, "%s.%s", #Var, #Val1 );\
  InitInfo( evtDouble, _W, &_W.Var.Val1, Var##Val1, (double)0);\
  static char Var##Val2[24*4];\
  makeName( Var##Val2, "%s.%s", #Var, #Val2 );\
  InitInfo( evtDouble, _W, &_W.Var.Val2, Var##Val2, (double)0);\
  static char Var##Val3[24*4];\
  makeName( Var##Val3, "%s.%s", #Var, #Val3 );\
  InitInfo( evtDouble, _W, &_W.Var.Val3, Var##Val3, (double)0);
//////////////////////////////////////////////////////////////////////////
#undef Struct_3R_A
#define Struct_3R_A(Var,Desc,Count,Val1,Val2,Val3)\
  static char Var##V1[Count+1][24*4]; \
  static char Var##V2[Count+1][24*4]; \
  static char Var##V3[Count+1][24*4]; \
  for( int i=0; i<_countof(Var##V1); ++i ) \
  {\
    makeName_A( Var##V1[i], "%s[%d].%s", #Var, i, #Val1 );\
    InitInfo( evtDouble, _W, &_W.Var[i].Val1, Var##V1[i], (double)0);\
    makeName_A( Var##V2[i], "%s[%d].%s", #Var, i, #Val2 );\
    InitInfo( evtDouble, _W, &_W.Var[i].Val2, Var##V2[i], (double)0);\
    makeName_A( Var##V3[i], "%s[%d].%s", #Var, i, #Val3 );\
    InitInfo( evtDouble, _W, &_W.Var[i].Val3, Var##V3[i], (double)0);\
  }
//////////////////////////////////////////////////////////////////////////
#undef Struct_BERB
#define Struct_BERB(Var,B1,tE2,E2,R3,B4)\
  static char Var##B1[24*4];\
  makeName( Var##B1, "%s.%s", #Var, #B1 );\
  InitInfo( evtBool, _W, &_W.Var.B1, Var##ValB, (BYTE)0);\
  static char Var##E2[24*4];\
  makeName( Var##E2, "%s.%s", #Var, #E2 );\
  InitInfoEnum( evtByte, _W, &_W.Var.E2, Var##E2, ::__##tE2::Def, ::__##tE2::GetVals() );\
  static char Var##R3[24*4];\
  makeName( Var##R3, "%s.%s", #Var, #R3 );\
  InitInfo( evtDouble, _W, &_W.Var.R3, Var##R3, (double)0);\
  static char Var##B4[24*4];\
  makeName( Var##B4, "%s.%s", #Var, #B4 );\
  InitInfo( evtBool, _W, &_W.Var.B4, Var##B4, (BYTE)0);
//////////////////////////////////////////////////////////////////////////
#undef Struct_BIRBSIR
#define Struct_BIRBSIR(Var,Desc,Val1,Val2,Val3,Val4,szVal5,lenV5,Val6,Val7)\
  static char Var##Val1[24*4];\
  makeName( Var##Val1, "%s.%s", #Var, #Val1 );\
  InitInfo( evtBool, _W, &_W.Var.Val1, Var##Val1, (BYTE)0);\
  static char Var##Val2[24*4];\
  makeName( Var##Val2, "%s.%s", #Var, #Val2 );\
  InitInfo( evtInt, _W, &_W.Var.Val2, Var##Val2, (int)0);\
  static char Var##Val3[24*4];\
  makeName( Var##Val3, "%s.%s", #Var, #Val3 );\
  InitInfo( evtDouble, _W, &_W.Var.Val3, Var##Val3, (double)0);\
  static char Var##Val4[24*4];\
  makeName( Var##Val4, "%s.%s", #Var, #Val4 );\
  InitInfo( evtByte, _W, &_W.Var.Val4, Var##Val4, (BYTE)0);\
  static char Var##szVal5[24*4];\
  makeName( Var##szVal5, "%s.%s", #Var, #szVal5 );\
  InitInfo( evtString, _W, &_W.Var.szVal5, Var##szVal5)->varSize = lenV5;\
  static char Var##Val6[24*4];\
  makeName( Var##Val6, "%s.%s", #Var, #Val6 );\
  InitInfo( evtInt, _W, &_W.Var.Val6, Var##Val6, (int)0);\
  static char Var##Val7[24*4];\
  makeName( Var##Val7, "%s.%s", #Var, #Val7 );\
  InitInfo( evtDouble, _W, &_W.Var.Val7, Var##Val7, (double)0);
//////////////////////////////////////////////////////////////////////////
#undef Struct_ER
#define Struct_ER(Var,tE2,E2,R3)\
  static char Var##E2[24*4];\
  makeName( Var##E2, "%s.%s", #Var, #E2 );\
  InitInfoEnum( evtByte, _W, &_W.Var.E2, Var##E2, ::__##tE2::Def, ::__##tE2::GetVals() );\
  static char Var##R3[24*4];\
  makeName( Var##R3, "%s.%s", #Var, #R3 );\
  InitInfo( evtDouble, _W, &_W.Var.R3, Var##R3, (double)0);
//////////////////////////////////////////////////////////////////////////
#undef Struct_BIRBR
#define Struct_BIRBR(Var,Desc,Val1,Val2,Val3,Val4,Val5)\
  static char Var##Val1[24*4];\
  makeName( Var##Val1, "%s.%s", #Var, #Val1 );\
  InitInfo( evtBool, _W, &_W.Var.Val1, Var##Val1, (BYTE)0);\
  static char Var##Val2[24*4];\
  makeName( Var##Val2, "%s.%s", #Var, #Val2 );\
  InitInfo( evtInt, _W, &_W.Var.Val2, Var##Val2, (int)0);\
  static char Var##Val3[24*4];\
  makeName( Var##Val3, "%s.%s", #Var, #Val3 );\
  InitInfo( evtDouble, _W, &_W.Var.Val3, Var##Val3, (double)0);\
  static char Var##Val4[24*4];\
  makeName( Var##Val4, "%s.%s", #Var, #Val4 );\
  InitInfo( evtByte, _W, &_W.Var.Val4, Var##Val4, (BYTE)0);\
  static char Var##Val5[24*4];\
  makeName( Var##Val5, "%s.%s", #Var, #Val5 );\
  InitInfo( evtDouble, _W, &_W.Var.Val5, Var##Val5, (double)0);
//////////////////////////////////////////////////////////////////////////
#undef Struct_F32F64x32I32UI64
#define Struct_F32F64x32I32UI64(Var,Desc,Val1,Val2,Val3,Val4)\
  static char Var##Val1[24*4];\
  makeName( Var##Val1, "%s.%s", #Var, #Val1 );\
  InitInfo( evtFloat, _W, &_W.Var.Val1, Var##Val1, (float)0);\
  static char Var##Val2[24*4];\
  makeName( Var##Val2, "%s.%s", #Var, #Val2 );\
  InitInfo( evtDouble, _W, &_W.Var.Val2, Var##Val2, (double)0);\
  static char Var##Val3[24*4];\
  makeName( Var##Val3, "%s.%s", #Var, #Val3 );\
  InitInfo( evtInt, _W, &_W.Var.Val3, Var##Val3, (int)0);\
  static char Var##Val4[24*4];\
  makeName( Var##Val4, "%s.%s", #Var, #Val4 );\
  InitInfo( evtUInt64, _W, &_W.Var.Val4, Var##Val4, (UINT64)0);
//////////////////////////////////////////////////////////////////////////
#undef StructA_SCM_TRANS_G
#define StructA_SCM_TRANS_G(Var,Count)\
{\
  static char szALGID[Count+1][32*4]; \
  for( int i=0; i<_countof(szALGID); ++i ) \
  {\
    makeName( szALGID[i], "%s[%d].ALGID", #Var, i );\
    InitInfoEnum( evtByte, _W, &_W.Var[i].ALGID, szALGID[i], ::__ALGID::Def, ::__ALGID::GetVals() );\
  }\
  static char szFIRSTCOND[Count+1][32*4]; \
  for( int i=0; i<_countof(szFIRSTCOND); ++i ) \
  {\
    makeName( szFIRSTCOND[i], "%s[%d].FIRSTCOND", #Var, i );\
    InitInfo( evtInt, _W, &_W.Var[i].FIRSTCOND, szFIRSTCOND[i], (int)0);\
  }\
  static char szNUMINPTS[Count+1][32*4]; \
  for( int i=0; i<_countof(szNUMINPTS); ++i ) \
  {\
    makeName( szNUMINPTS[i], "%s[%d].NUMINPTS", #Var, i );\
    InitInfo( evtInt, _W, &_W.Var[i].NUMINPTS, szNUMINPTS[i], (int)0);\
  }\
  static char szSO[Count+1][32*4]; \
  for( int i=0; i<_countof(szSO); ++i ) \
  {\
    makeName( szSO[i], "%s[%d].SO", #Var, i );\
    InitInfo( evtByte, _W, &_W.Var[i].SO, szSO[i], (BYTE)0);\
  }\
}
//////////////////////////////////////////////////////////////////////////
#undef StructA_SCM_TRANS_C
#define StructA_SCM_TRANS_C(Var,Count)\
{\
  static char szBYPPERM[Count+1][32*4]; \
  for( int i=0; i<_countof(szBYPPERM); ++i ) \
  {\
    makeName( szBYPPERM[i], "%s[%d].BYPPERM", #Var, i );\
    InitInfo( evtByte, _W, &_W.Var[i].BYPPERM, szBYPPERM[i], (BYTE)0);\
  }\
  s_defFlag = SVarInfo::efVar;\
  static char szBYPREQ[Count+1][32*4]; \
  for( int i=0; i<_countof(szBYPREQ); ++i ) \
  {\
    makeName( szBYPREQ[i], "%s[%d].BYPREQ", #Var, i );\
    InitInfoEnum( evtByte, _W, &_W.Var[i].BYPREQ, szBYPREQ[i], ::__BYPREQ::Def, ::__BYPREQ::GetVals() );\
  }\
  s_defFlag = SVarInfo::efParam;\
  static char szCONFIGDESC[Count+1][32*4]; \
  for( int i=0; i<_countof(szCONFIGDESC); ++i ) \
  {\
    makeName( szCONFIGDESC[i], "%s[%d].CONFIGDESC", #Var, i );\
    InitInfo( evtString, _W, &_W.Var[i].szCONFIGDESC, szCONFIGDESC[i])->varSize = _countof(_W.Var[i].szCONFIGDESC)-1;\
  }\
  static char szDESC[Count+1][32*4]; \
  for( int i=0; i<_countof(szDESC); ++i ) \
  {\
    makeName( szDESC[i], "%s[%d].DESC", #Var, i );\
    InitInfo( evtString, _W, &_W.Var[i].szDESC, szDESC[i])->varSize = _countof(_W.Var[i].szDESC)-1;\
  }\
  static char szEXPR[Count+1][32*4]; \
  for( int i=0; i<_countof(szEXPR); ++i ) \
  {\
    makeName( szEXPR[i], "%s[%d].EXPR", #Var, i );\
    InitInfo( evtString, _W, &_W.Var[i].szEXPR, szEXPR[i])->varSize = _countof(_W.Var[i].szEXPR)-1;\
  }\
  static char szFL[Count+1][32*4]; \
  for( int i=0; i<_countof(szFL); ++i ) \
  {\
    makeName( szFL[i], "%s[%d].FL", #Var, i );\
    InitInfo( evtByte, _W, &_W.Var[i].FL, szFL[i], (BYTE)0);\
  }\
  static char szGATEASGN[Count+1][32*4]; \
  for( int i=0; i<_countof(szGATEASGN); ++i ) \
  {\
    makeName( szGATEASGN[i], "%s[%d].GATEASGN", #Var, i );\
    InitInfoEnum( evtByte, _W, &_W.Var[i].GATEASGN, szGATEASGN[i], ::__GATEASGN::Def, ::__GATEASGN::GetVals() );\
  }\
  static char szINCONNSTS1[Count+1][32*4]; \
  for( int i=0; i<_countof(szINCONNSTS1); ++i ) \
  {\
    makeName( szINCONNSTS1[i], "%s[%d].INCONNSTS1", #Var, i );\
    InitInfo( evtInt, _W, &_W.Var[i].INCONNSTS1, szINCONNSTS1[i], (int)0);\
  }\
  static char szINCONNSTS2[Count+1][32*4]; \
  for( int i=0; i<_countof(szINCONNSTS2); ++i ) \
  {\
    makeName( szINCONNSTS2[i], "%s[%d].INCONNSTS2", #Var, i );\
    InitInfo( evtInt, _W, &_W.Var[i].INCONNSTS2, szINCONNSTS2[i], (int)0);\
  }\
  static char szINCONNSTS3[Count+1][32*4]; \
  for( int i=0; i<_countof(szINCONNSTS3); ++i ) \
  {\
    makeName( szINCONNSTS3[i], "%s[%d].INCONNSTS3", #Var, i );\
    InitInfo( evtInt, _W, &_W.Var[i].INCONNSTS3, szINCONNSTS3[i], (int)0);\
  }\
  static char szINCONNSTS4[Count+1][32*4]; \
  for( int i=0; i<_countof(szINCONNSTS4); ++i ) \
  {\
    makeName( szINCONNSTS4[i], "%s[%d].INCONNSTS4", #Var, i );\
    InitInfo( evtInt, _W, &_W.Var[i].INCONNSTS4, szINCONNSTS4[i], (int)0);\
  }\
  static char szINCONNSTS5[Count+1][32*4]; \
  for( int i=0; i<_countof(szINCONNSTS2); ++i ) \
  {\
    makeName( szINCONNSTS5[i], "%s[%d].INCONNSTS5", #Var, i );\
    InitInfo( evtInt, _W, &_W.Var[i].INCONNSTS5, szINCONNSTS5[i], (int)0);\
  }\
  static char szINCONNSTS6[Count+1][32*4]; \
  for( int i=0; i<_countof(szINCONNSTS6); ++i ) \
  {\
    makeName( szINCONNSTS6[i], "%s[%d].INCONNSTS6", #Var, i );\
    InitInfo( evtInt, _W, &_W.Var[i].INCONNSTS6, szINCONNSTS6[i], (int)0);\
  }\
  static char szINVOKFL[Count+1][32*4]; \
  for( int i=0; i<_countof(szINVOKFL); ++i ) \
  {\
    makeName( szINVOKFL[i], "%s[%d].INVOKFL", #Var, i );\
    InitInfo( evtByte, _W, &_W.Var[i].INVOKFL, szINVOKFL[i], (BYTE)0);\
  }\
}
//////////////////////////////////////////////////////////////////////////
#undef Struct_SCM_STEP_SC
#define Struct_SCM_STEP_SC(Var)\
{\
  static char szBYPPERM[24*4]; \
  makeName( szBYPPERM, "%s.BYPPERM", #Var, 0 );\
  InitInfo( evtByte, _W, &_W.Var.BYPPERM, szBYPPERM, (BYTE)0);\
  static char szBYPREQ[24*4]; \
  makeName( szBYPREQ, "%s.BYPREQ", #Var, 0 );\
  InitInfoEnum( evtByte, _W, &_W.Var.BYPREQ, szBYPREQ, ::__BYPREQ::Def, ::__BYPREQ::GetVals() );\
  static char szCONFIGDESC[24*4]; \
  makeName( szCONFIGDESC, "%s.CONFIGDESC", #Var, 0 );\
  InitInfo( evtString, _W, &_W.Var.szCONFIGDESC, szCONFIGDESC)->varSize = _countof(_W.Var.szCONFIGDESC)-1;\
  static char szDESC[24*4]; \
  makeName( szDESC, "%s.DESC", #Var, 0 );\
  InitInfo( evtString, _W, &_W.Var.szDESC, szDESC)->varSize = _countof(_W.Var.szDESC)-1;\
  static char szEXPR[24*4]; \
  makeName( szEXPR, "%s.EXPR", #Var, 0 );\
  InitInfo( evtString, _W, &_W.Var.szEXPR, szEXPR)->varSize = _countof(_W.Var.szEXPR)-1;\
  static char szFL[24*4]; \
  makeName( szFL, "%s.FL", #Var, 0 );\
  InitInfo( evtByte, _W, &_W.Var.FL, szFL, (BYTE)0);\
  static char szINCONNSTS1[24*4]; \
  makeName( szINCONNSTS1, "%s.INCONNSTS1", #Var, 0 );\
  InitInfo( evtInt, _W, &_W.Var.INCONNSTS1, szINCONNSTS1, (int)0);\
  static char szINCONNSTS2[24*4]; \
  makeName( szINCONNSTS2, "%s.INCONNSTS2", #Var, 0 );\
  InitInfo( evtInt, _W, &_W.Var.INCONNSTS2, szINCONNSTS2, (int)0);\
  static char szINCONNSTS3[24*4]; \
  makeName( szINCONNSTS3, "%s.INCONNSTS3", #Var, 0 );\
  InitInfo( evtInt, _W, &_W.Var.INCONNSTS3, szINCONNSTS3, (int)0);\
  static char szINCONNSTS4[24*4]; \
  makeName( szINCONNSTS4, "%s.INCONNSTS4", #Var, 0 );\
  InitInfo( evtInt, _W, &_W.Var.INCONNSTS4, szINCONNSTS4, (int)0);\
  static char szINCONNSTS5[24*4]; \
  makeName( szINCONNSTS5, "%s.INCONNSTS5", #Var, 0 );\
  InitInfo( evtInt, _W, &_W.Var.INCONNSTS5, szINCONNSTS5, (int)0);\
  static char szINCONNSTS6[24*4]; \
  makeName( szINCONNSTS6, "%s.INCONNSTS6", #Var, 0 );\
  InitInfo( evtInt, _W, &_W.Var.INCONNSTS6, szINCONNSTS6, (int)0);\
  static char szINVOKFL[24*4]; \
  makeName( szINVOKFL, "%s.INVOKFL", #Var, 0 );\
  InitInfo( evtByte, _W, &_W.Var.INVOKFL, szINVOKFL, (BYTE)0);\
  static char szOPT[24*4]; \
  makeName( szOPT, "%s.OPT", #Var, 0 );\
  InitInfo( evtByte, _W, &_W.Var.OPT, szOPT, (BYTE)0);\
}
//////////////////////////////////////////////////////////////////////////
#undef StructA_SCM_STEP_OP
#define StructA_SCM_STEP_OP(Var,Count) \
{\
  static char szACQSTATE[Count+1][32*4]; \
  for( int i=0; i<_countof(szACQSTATE); ++i ) \
  {\
    makeName( szACQSTATE[i], "%s[%d].ACQSTATE", #Var, i );\
    InitInfoEnum( evtByte, _W, &_W.Var[i].ACQSTATE, szACQSTATE[i], ::__ACQSTATE::Def, ::__ACQSTATE::GetVals() );\
  }\
  static char szBYPREQ[Count+1][32*4]; \
  for( int i=0; i<_countof(szBYPREQ); ++i ) \
  {\
    makeName( szBYPREQ[i], "%s[%d].BYPREQ", #Var, i );\
    InitInfoEnum( evtByte, _W, &_W.Var[i].BYPREQ, szBYPREQ[i], ::__BYPREQ::Def, ::__BYPREQ::GetVals() );\
  }\
  static char CMDREQSTS[Count+1][32*4]; \
  for( int i=0; i<_countof(CMDREQSTS); ++i ) \
  {\
    makeName( CMDREQSTS[i], "%s[%d].CMDREQSTS", #Var, i );\
    InitInfo( evtInt, _W, &_W.Var[i].CMDREQSTS, CMDREQSTS[i], (int)0);\
  }\
  static char CONFIGASSOCPARAM[Count+1][32*4]; \
  for( int i=0; i<_countof(CONFIGASSOCPARAM); ++i ) \
  {\
    makeName( CONFIGASSOCPARAM[i], "%s[%d].CONFIGASSOCPARAM", #Var, i );\
    InitInfo( evtByte, _W, &_W.Var[i].CONFIGASSOCPARAM, CONFIGASSOCPARAM[i], (BYTE)0);\
  }\
  static char CONFIGDESC[Count+1][32*4]; \
  for( int i=0; i<_countof(CONFIGDESC); ++i ) \
  {\
    makeName( CONFIGDESC[i], "%s[%d].CONFIGDESC", #Var, i );\
    InitInfo( evtString, _W, &_W.Var[i].CONFIGDESC, CONFIGDESC[i])->varSize = _countof(_W.Var[i].CONFIGDESC)-1;\
  }\
  static char CURRVALTREND[Count+1][32*4]; \
  for( int i=0; i<_countof(CURRVALTREND); ++i ) \
  {\
    makeName( CURRVALTREND[i], "%s[%d].CURRVALTREND", #Var, i );\
    InitInfo( evtByte, _W, &_W.Var[i].CURRVALTREND, CURRVALTREND[i], (BYTE)0);\
  }\
  static char DELAYTIME[Count+1][32*4]; \
  for( int i=0; i<_countof(DELAYTIME); ++i ) \
  {\
    makeName( DELAYTIME[i], "%s[%d].DELAYTIME", #Var, i );\
    InitInfo( evtDouble, _W, &_W.Var[i].DELAYTIME, DELAYTIME[i], (double)0);\
  }\
  static char DELAYTIMEREM[Count+1][32*4]; \
  for( int i=0; i<_countof(DELAYTIMEREM); ++i ) \
  {\
    makeName( DELAYTIMEREM[i], "%s[%d].DELAYTIMEREM", #Var, i );\
    InitInfo( evtDouble, _W, &_W.Var[i].DELAYTIMEREM, DELAYTIMEREM[i], (double)0);\
  }\
  static char DESC[Count+1][32*4]; \
  for( int i=0; i<_countof(DESC); ++i ) \
  {\
    makeName( DESC[i], "%s[%d].DESC", #Var, i );\
    InitInfo( evtString, _W, &_W.Var[i].DESC, DESC[i])->varSize = _countof(_W.Var[i].DESC)-1;\
  }\
  static char EQUIPNAME[Count+1][32*4]; \
  for( int i=0; i<_countof(EQUIPNAME); ++i ) \
  {\
    makeName( EQUIPNAME[i], "%s[%d].EQUIPNAME", #Var, i );\
    InitInfo( evtInt, _W, &_W.Var[i].EQUIPNAME, EQUIPNAME[i], (int)0);\
  }\
  static char EXECDESC[Count+1][32*4]; \
  for( int i=0; i<_countof(EXECDESC); ++i ) \
  {\
    makeName( EXECDESC[i], "%s[%d].EXECDESC", #Var, i );\
    InitInfo( evtString, _W, &_W.Var[i].EXECDESC, EXECDESC[i])->varSize = _countof(_W.Var[i].EXECDESC)-1;\
  }\
  static char EXECSTS[Count+1][32*4]; \
  for( int i=0; i<_countof(EXECSTS); ++i ) \
  {\
    makeName( EXECSTS[i], "%s[%d].EXECSTS", #Var, i );\
    InitInfoEnum( evtByte, _W, &_W.Var[i].EXECSTS, EXECSTS[i], ::__EXECSTS::Def, ::__EXECSTS::GetVals() );\
  }\
  static char EXPRPCODE[Count+1][32*4]; \
  for( int i=0; i<_countof(EXPRPCODE); ++i ) \
  {\
    makeName( EXPRPCODE[i], "%s[%d].EXPRPCODE", #Var, i );\
    InitInfo( evtString, _W, &_W.Var[i].EXPRPCODE, EXPRPCODE[i])->varSize = _countof(_W.Var[i].EXPRPCODE)-1;\
  }\
  static char FILTERABLE[Count+1][32*4]; \
  for( int i=0; i<_countof(FILTERABLE); ++i ) \
  {\
    makeName( FILTERABLE[i], "%s[%d].FILTERABLE", #Var, i );\
    InitInfo( evtByte, _W, &_W.Var[i].FILTERABLE, FILTERABLE[i], (BYTE)0);\
  }\
  static char HIDETBLVIEW[Count+1][32*4]; \
  for( int i=0; i<_countof(HIDETBLVIEW); ++i ) \
  {\
    makeName( HIDETBLVIEW[i], "%s[%d].HIDETBLVIEW", #Var, i );\
    InitInfo( evtByte, _W, &_W.Var[i].HIDETBLVIEW, HIDETBLVIEW[i], (BYTE)0);\
  }\
  static char INCONNSTS1[Count+1][32*4]; \
  for( int i=0; i<_countof(INCONNSTS1); ++i ) \
  {\
    makeName( INCONNSTS1[i], "%s[%d].INCONNSTS1", #Var, i );\
    InitInfo( evtInt, _W, &_W.Var[i].INCONNSTS1, INCONNSTS1[i], (int)0);\
  }\
  static char INCONNSTS2[Count+1][32*4]; \
  for( int i=0; i<_countof(INCONNSTS1); ++i ) \
  {\
    makeName( INCONNSTS2[i], "%s[%d].INCONNSTS2", #Var, i );\
    InitInfo( evtInt, _W, &_W.Var[i].INCONNSTS2, INCONNSTS2[i], (int)0);\
  }\
  static char INCONNSTS3[Count+1][32*4]; \
  for( int i=0; i<_countof(INCONNSTS3); ++i ) \
  {\
    makeName( INCONNSTS3[i], "%s[%d].INCONNSTS3", #Var, i );\
    InitInfo( evtInt, _W, &_W.Var[i].INCONNSTS3, INCONNSTS3[i], (int)0);\
  }\
  static char INCONNSTS4[Count+1][32*4]; \
  for( int i=0; i<_countof(INCONNSTS4); ++i ) \
  {\
    makeName( INCONNSTS4[i], "%s[%d].INCONNSTS4", #Var, i );\
    InitInfo( evtInt, _W, &_W.Var[i].INCONNSTS4, INCONNSTS4[i], (int)0);\
  }\
  static char INCONNSTS5[Count+1][32*4]; \
  for( int i=0; i<_countof(INCONNSTS5); ++i ) \
  {\
    makeName( INCONNSTS5[i], "%s[%d].INCONNSTS5", #Var, i );\
    InitInfo( evtInt, _W, &_W.Var[i].INCONNSTS5, INCONNSTS5[i], (int)0);\
  }\
  static char INCONNSTS6[Count+1][32*4]; \
  for( int i=0; i<_countof(INCONNSTS6); ++i ) \
  {\
    makeName( INCONNSTS6[i], "%s[%d].INCONNSTS6", #Var, i );\
    InitInfo( evtInt, _W, &_W.Var[i].INCONNSTS6, INCONNSTS6[i], (int)0);\
  }\
  static char INSTRUCTCNFM[Count+1][32*4]; \
  for( int i=0; i<_countof(INSTRUCTCNFM); ++i ) \
  {\
    makeName( INSTRUCTCNFM[i], "%s[%d].INSTRUCTCNFM", #Var, i );\
    InitInfo( evtByte, _W, &_W.Var[i].INSTRUCTCNFM, INSTRUCTCNFM[i], (BYTE)0);\
  }\
  static char INSTRUCTCOMP[Count+1][32*4]; \
  for( int i=0; i<_countof(INSTRUCTCOMP); ++i ) \
  {\
    makeName( INSTRUCTCOMP[i], "%s[%d].INSTRUCTCOMP", #Var, i );\
    InitInfo( evtByte, _W, &_W.Var[i].INSTRUCTCOMP, INSTRUCTCOMP[i], (BYTE)0);\
  }\
  static char INSTRUCTPEND[Count+1][32*4]; \
  for( int i=0; i<_countof(INSTRUCTPEND); ++i ) \
  {\
    makeName( INSTRUCTPEND[i], "%s[%d].INSTRUCTPEND", #Var, i );\
    InitInfo( evtByte, _W, &_W.Var[i].INSTRUCTPEND, INSTRUCTPEND[i], (BYTE)0);\
  }\
  static char INSTRUCTTYPE[Count+1][32*4]; \
  for( int i=0; i<_countof(INSTRUCTTYPE); ++i ) \
  {\
    makeName( INSTRUCTTYPE[i], "%s[%d].INSTRUCTTYPE", #Var, i );\
    InitInfoEnum( evtByte, _W, &_W.Var[i].INSTRUCTTYPE, INSTRUCTTYPE[i], ::__INSTRUCTTYPE::Def, ::__INSTRUCTTYPE::GetVals() );\
  }\
  static char OWNERSSTS[Count+1][32*4]; \
  for( int i=0; i<_countof(OWNERSSTS); ++i ) \
  {\
    makeName( OWNERSSTS[i], "%s[%d].OWNERSSTS", #Var, i );\
    InitInfo( evtInt, _W, &_W.Var[i].OWNERSSTS, OWNERSSTS[i], (int)0);\
  }\
  static char OWNFL[Count+1][32*4]; \
  for( int i=0; i<_countof(OWNFL); ++i ) \
  {\
    makeName( OWNFL[i], "%s[%d].OWNFL", #Var, i );\
    InitInfo( evtByte, _W, &_W.Var[i].OWNFL, OWNFL[i], (BYTE)0);\
  }\
  static char RELFL[Count+1][32*4]; \
  for( int i=0; i<_countof(OWNFL); ++i ) \
  {\
    makeName( RELFL[i], "%s[%d].RELFL", #Var, i );\
    InitInfo( evtByte, _W, &_W.Var[i].RELFL, RELFL[i], (BYTE)0);\
  }\
  static char REQFL[Count+1][32*4]; \
  for( int i=0; i<_countof(REQFL); ++i ) \
  {\
    makeName( REQFL[i], "%s[%d].REQFL", #Var, i );\
    InitInfo( evtByte, _W, &_W.Var[i].REQFL, REQFL[i], (BYTE)0);\
  }\
  static char ROLE[Count+1][32*4]; \
  for( int i=0; i<_countof(ROLE); ++i ) \
  {\
    makeName( ROLE[i], "%s[%d].ROLE", #Var, i );\
    InitInfo( evtString, _W, &_W.Var[i].ROLE, ROLE[i])->varSize = _countof(_W.Var[i].ROLE)-1;\
  }\
  static char SRCEXPR[Count+1][32*4]; \
  for( int i=0; i<_countof(SRCEXPR); ++i ) \
  {\
    makeName( SRCEXPR[i], "%s[%d].SRCEXPR", #Var, i );\
    InitInfo( evtString, _W, &_W.Var[i].SRCEXPR, SRCEXPR[i])->varSize = _countof(_W.Var[i].SRCEXPR)-1;\
  }\
  static char STATE[Count+1][32*4]; \
  for( int i=0; i<_countof(STATE); ++i ) \
  {\
    makeName( STATE[i], "%s[%d].STATE", #Var, i );\
    InitInfoEnum( evtByte, _W, &_W.Var[i].STATE, STATE[i], ::__STATE::Def, ::__STATE::GetVals() );\
  }\
  static char STORESTS[Count+1][32*4]; \
  for( int i=0; i<_countof(STORESTS); ++i ) \
  {\
    makeName( STORESTS[i], "%s[%d].STORESTS", #Var, i );\
    InitInfo( evtInt, _W, &_W.Var[i].STORESTS, STORESTS[i], (int)0);\
  }\
  static char TARGETCHANGE[Count+1][32*4]; \
  for( int i=0; i<_countof(TARGETCHANGE); ++i ) \
  {\
    makeName( TARGETCHANGE[i], "%s[%d].TARGETCHANGE", #Var, i );\
    InitInfo( evtByte, _W, &_W.Var[i].TARGETCHANGE, TARGETCHANGE[i], (BYTE)0);\
  }\
  static char TYPE[Count+1][32*4]; \
  for( int i=0; i<_countof(TYPE); ++i ) \
  {\
    makeName( TYPE[i], "%s[%d].TYPE", #Var, i );\
    InitInfoEnum( evtByte, _W, &_W.Var[i].TYPE, TYPE[i], ::__TYPE::Def, ::__TYPE::GetVals() );\
  }\
}
//////////////////////////////////////////////////////////////////////////
#undef Struct_EII
#define Struct_EII(Var,tE1,E1,I2,I3)\
  static char Var##E1[24*4];\
  makeName( Var##E1, "%s.%s", #Var, #E1 );\
  InitInfoEnum( evtByte, _W, &_W.Var.E1, Var##E1, ::__##tE1::Def, ::__##tE1::GetVals() );\
  static char Var##I2[24*4];\
  makeName( Var##I2, "%s.%s", #Var, #I2 );\
  InitInfo( evtInt, _W, &_W.Var.I2, Var##I2, (int)0);\
  static char Var##I3[24*4];\
  makeName( Var##I3, "%s.%s", #Var, #I3 );\
  InitInfo( evtInt, _W, &_W.Var.I3, Var##I3, (int)0);
//////////////////////////////////////////////////////////////////////////
#undef Struct2BR
#define Struct2BR(Var,Desc,Val1,Val2,R3)\
  static char Var##Val1[24*4];\
  makeName( Var##Val1, "%s.%s", #Var, #Val1 );\
  InitInfo( evtBool, _W, &_W.Var.Val1, Var##Val1, (BYTE)0);\
  static char Var##Val2[24*4];\
  makeName( Var##Val2, "%s.%s", #Var, #Val2 );\
  InitInfo( evtBool, _W, &_W.Var.Val2, Var##Val2, (BYTE)0);\
  static char Var##R3[24*4];\
  makeName( Var##R3, "%s.%s", #Var, #R3 );\
  InitInfo( evtDouble, _W, &_W.Var.R3, Var##R3, (double)0);
//////////////////////////////////////////////////////////////////////////
#undef StructBR
#define StructBR(Var,Desc,Val1,R2)\
  static char Var##Val1[24*4];\
  makeName( Var##Val1, "%s.%s", #Var, #Val1 );\
  InitInfo( evtBool, _W, &_W.Var.Val1, Var##Val1, (BYTE)0);\
  static char Var##R2[24*4];\
  makeName( Var##R2, "%s.%s", #Var, #R2 );\
  InitInfo( evtDouble, _W, &_W.Var.R2, Var##R2, (double)0);
//////////////////////////////////////////////////////////////////////////
