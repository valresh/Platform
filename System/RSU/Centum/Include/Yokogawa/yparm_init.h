template<typename D, typename S>
void CopyInitialValues( D &destW, S &srcStatic )
{
  size_t ds = sizeof(destW);
  size_t ss = sizeof(srcStatic);
  size_t cs = sizeof(D) - sizeof(CBase);
  memmove(  ((BYTE*)&destW) + sizeof(CBase),
            ((BYTE*)&srcStatic) + sizeof(CBase),
            sizeof(D) - sizeof(CBase) );
}

#undef LIST_PARM
#define LIST_PARM(BaseClass,Class,DimP,DimV) \
  static Class _W;\
  static CYParmInfo ParmInfo[DimP];\
  static int kParmInfo = 0;\
  static CYVarInfo VarInfo[DimV+1];\
  static int kVarInfo = 0;\
  void BaseClass::SetParmList( )\
    {\
    if( !kParmInfo && !kVarInfo ) \
      BaseClass::InitParm();\
    ClassParmInfo = ParmInfo;\
    kClassParmInfo = kParmInfo;\
    ClassVarInfo = VarInfo;\
    kClassVarInfo = kVarInfo;\
    pClassBase = (BYTE*)W;\
    Y_Type = W->ID_CLASS;\
    CopyInitialValues( *W, _W);\
    pClass_P = ((BYTE*)static_cast<Class*>(W)) + sizeof(CBase);\
    L_Class_P = sizeof (Class) - sizeof(CBase);\
    }


#undef SKIP
#define SKIP(Name) \
  {\
  ASS( kParmInfo < _countof(ParmInfo) );\
  CYParmInfo * pP = &(ParmInfo[kParmInfo++]);\
  pP->sVar_1 = 0;\
  pP->Name_1 = NULL;\
  pP->Type = " ";\
  pP->ParmName = Name;\
  pP->ParmFmt = NULL;\
  pP->szVarSize = 0;\
  pP->Read = NULL;\
  }


#undef YPARM_I
#define YPARM_I(Var,Def,Name) \
  {\
  _W.Var = Def;\
  ASS( kParmInfo < _countof(ParmInfo) );\
  CYParmInfo * pP = &(ParmInfo[kParmInfo++]);\
  pP->sVar_1 = (DWORD)((BYTE*)&_W.Var - (BYTE*)&_W);\
  pP->Name_1 = #Var;\
  pP->Type = "I";\
  pP->ParmName = Name;\
  pP->ParmFmt = "%I";\
  pP->Read = 0;\
  pP->szVarSize = sizeof(_W.Var);\
  ASS( kVarInfo < _countof(VarInfo) );\
  CYVarInfo * pV = &(VarInfo[kVarInfo++]);\
  pV->sVar = (DWORD)((BYTE*)&_W.Var - (BYTE*)&_W);\
  pV->VarName = #Var;\
  pV->Type = 'I';\
  pV->Work = 0;\
  pV->szVarSize = sizeof(_W.Var);\
  }


#undef YPARM_I_A
#define YPARM_I_A(Var,Dim,Desc) \
  {\
  for ( int n = 0; n < Dim; n++ )\
  {\
  _W.Var[n] = 0;\
  ASS( kParmInfo < _countof(ParmInfo) );\
  CYParmInfo * pP = &(ParmInfo[kParmInfo++]);\
  pP->Type = "I";\
  pP->sVar_1 = (DWORD)((BYTE*)&_W.Var[n] - (BYTE*)&_W);\
  sprintf_s(_W.Var##Name[n], "%s%02d", #Var, n + 1 );\
  pP->Name_1 = _W.Var##Name[n];\
  pP->Type = "I";\
  pP->ParmName = Desc;\
  pP->ParmFmt = "%I";\
  pP->Read = 0;\
  pP->szVarSize = sizeof(_W.Var[n]);\
  ASS( kVarInfo < _countof(VarInfo) );\
  CYVarInfo * pV = &(VarInfo[kVarInfo++]);\
  pV->sVar = (DWORD)((BYTE*)&_W.Var[n] - (BYTE*)&_W);\
  pV->VarName = _W.Var##Name[n];\
  pV->Type = 'I';\
  pV->Work = 0;\
  pV->szVarSize = sizeof(_W.Var[n]);\
  }\
  }

#undef YPARM_L
#define YPARM_L(Var,Def,Name,Fmt) \
  {\
  _W.Var = Def;\
  ASS( kParmInfo < _countof(ParmInfo) );\
  CYParmInfo * pP = &(ParmInfo[kParmInfo++]);\
  pP->sVar_1 = (DWORD)((BYTE*)&_W.Var - (BYTE*)&_W);\
  pP->Name_1 = #Var;\
  pP->Type = "I";\
  pP->ParmName = Name;\
  pP->ParmFmt = Fmt;\
  pP->Read = 0;\
  pP->szVarSize = sizeof(_W.Var);\
  ASS( kVarInfo < _countof(VarInfo) );\
  CYVarInfo * pV = &(VarInfo[kVarInfo++]);\
  pV->sVar = (DWORD)((BYTE*)&_W.Var - (BYTE*)&_W);\
  pV->VarName = #Var;\
  pV->Type = 'I';\
  pV->Work = 0;\
  pV->szVarSize = sizeof(_W.Var);\
  }

#undef YPARM_L_savable
#define YPARM_L_savable(Var,Def,Name,Fmt) \
  {\
  _W.Var = Def;\
  ASS( kParmInfo < _countof(ParmInfo) );\
  CYParmInfo * pP = &(ParmInfo[kParmInfo++]);\
  pP->sVar_1 = (DWORD)((BYTE*)&_W.Var - (BYTE*)&_W);\
  pP->Name_1 = #Var;\
  pP->Type = "I";\
  pP->ParmName = Name;\
  pP->ParmFmt = Fmt;\
  pP->Read = 0;\
  pP->szVarSize = sizeof(_W.Var);\
  ASS( kVarInfo < _countof(VarInfo) );\
  CYVarInfo * pV = &(VarInfo[kVarInfo++]);\
  pV->sVar = (DWORD)((BYTE*)&_W.Var - (BYTE*)&_W);\
  pV->VarName = #Var;\
  pV->Type = 'I';\
  pV->Work = 0;\
  pV->szVarSize = sizeof(_W.Var);\
  }

#undef YPARM_N
#define YPARM_N(Var,Def,Name,Fmt) \
  {\
  _W.Var = Def;\
  ASS( kParmInfo < _countof(ParmInfo) );\
  CYParmInfo * pP = &(ParmInfo[kParmInfo++]);\
  pP->sVar_1 = (DWORD)((BYTE*)&_W.Var - (BYTE*)&_W);\
  pP->Name_1 = #Var;\
  pP->Type = "I";\
  if( Name[0]=='#') \
    pP->ParmName = Name+1;\
  else\
    pP->ParmName = Name;\
  pP->ParmFmt = Fmt;\
  pP->Read = 0;\
  pP->szVarSize = sizeof(_W.Var);\
  ASS( kVarInfo < _countof(VarInfo) );\
  CYVarInfo * pV = &(VarInfo[kVarInfo++]);\
  pV->sVar = (DWORD)((BYTE*)&_W.Var - (BYTE*)&_W);\
  pV->VarName = #Var;\
  pV->Type = 'I';\
  pV->Work = 0;\
  pV->szVarSize = sizeof(_W.Var);\
  }

#undef YPARM_D
#define YPARM_D(Var,Def,Name) \
  {\
  _W.Var = Def;\
  ASS( kParmInfo < _countof(ParmInfo) );\
  CYParmInfo * pP = &(ParmInfo[kParmInfo++]);\
  pP->sVar_1 = (DWORD)((BYTE*)&_W.Var - (BYTE*)&_W);\
  pP->Name_1 = #Var;\
  pP->Type = "D";\
  pP->ParmName = Name;\
  pP->ParmFmt = "%D";\
  pP->Read = 0;\
  pP->szVarSize = sizeof(_W.Var);\
  ASS( kVarInfo < _countof(VarInfo) );\
  CYVarInfo * pV = &(VarInfo[kVarInfo++]);\
  pV->sVar = (DWORD)((BYTE*)&_W.Var - (BYTE*)&_W);\
  pV->VarName = #Var;\
  pV->Type = 'D';\
  pV->Work = 0;\
  pV->szVarSize = sizeof(_W.Var);\
  }

#undef YPARM_Dref
#define YPARM_Dref(Var,Def,Name) \
  {\
  _W.Var = DBL_MAX;\
  ASS( kParmInfo < _countof(ParmInfo) );\
  CYParmInfo * pP = &(ParmInfo[kParmInfo++]);\
  pP->sVar_1 = (DWORD)((BYTE*)&_W.Var - (BYTE*)&_W);\
  pP->Name_1 = #Var;\
  pP->Type = "D";\
  pP->ParmName = Name;\
  pP->ParmFmt = "%D";\
  pP->Read = 0;\
  pP->szVarSize = sizeof(_W.Var);\
  ASS( kVarInfo < _countof(VarInfo) );\
  CYVarInfo * pV = &(VarInfo[kVarInfo++]);\
  pV->sVar = (DWORD)((BYTE*)&_W.Var - (BYTE*)&_W);\
  pV->VarName = #Var;\
  pV->Type = 'D';\
  pV->Work = 0;\
  pV->szVarSize = sizeof(_W.Var);\
  }

#undef YPARM_D_A
#define YPARM_D_A(Var,Dim,Desc) \
  {\
  for ( int n = 0; n < Dim; n++ )\
  {\
  _W.Var[n] = 0;\
  ASS( kParmInfo < _countof(ParmInfo) );\
  CYParmInfo * pP = &(ParmInfo[kParmInfo++]);\
  pP->Type = "D";\
  pP->sVar_1 = (DWORD)((BYTE*)&_W.Var[n] - (BYTE*)&_W);\
  sprintf_s(_W.Var##Name[n], "%s%02d", #Var, n + 1 );\
  pP->Name_1 = _W.Var##Name[n];\
  pP->Type = "D";\
  pP->ParmName = Desc;\
  pP->ParmFmt = "%D";\
  pP->Read = 0;\
  pP->szVarSize = sizeof(_W.Var[n]);\
  ASS( kVarInfo < _countof(VarInfo) );\
  CYVarInfo * pV = &(VarInfo[kVarInfo++]);\
  pV->sVar = (DWORD)((BYTE*)&_W.Var[n] - (BYTE*)&_W);\
  pV->VarName = _W.Var##Name[n];\
  pV->Type = 'D';\
  pV->Work = 0;\
  pV->szVarSize = sizeof(_W.Var[n]);\
  }\
  }

#undef YPARM_D_savable
#define YPARM_D_savable(Var,Def,Name) \
  {\
  _W.Var = Def;\
  ASS( kParmInfo < _countof(ParmInfo) );\
  CYParmInfo * pP = &(ParmInfo[kParmInfo++]);\
  pP->sVar_1 = (DWORD)((BYTE*)&_W.Var - (BYTE*)&_W);\
  pP->Name_1 = #Var;\
  pP->Type = "D";\
  pP->ParmName = Name;\
  pP->ParmFmt = "%D";\
  pP->Read = 0;\
  pP->szVarSize = sizeof(_W.Var);\
  ASS( kVarInfo < _countof(VarInfo) );\
  CYVarInfo * pV = &(VarInfo[kVarInfo++]);\
  pV->sVar = (DWORD)((BYTE*)&_W.Var - (BYTE*)&_W);\
  pV->VarName = #Var;\
  pV->Type = 'D';\
  pV->Work = 0;\
  pV->szVarSize = sizeof(_W.Var);\
  }

#undef YPARM_D_ALT
#define YPARM_D_ALT(Var,Def,Name,VARaltNAME) \
  {\
  _W.Var = Def;\
  ASS( kParmInfo < _countof(ParmInfo) );\
  CYParmInfo * pP = &(ParmInfo[kParmInfo++]);\
  pP->sVar_1 = (DWORD)((BYTE*)&_W.Var - (BYTE*)&_W);\
  pP->Name_1 = #Var;\
  pP->Type = "D";\
  pP->ParmName = Name;\
  pP->ParmFmt = "%D";\
  pP->Read = 0;\
  pP->szVarSize = sizeof(_W.Var);\
  ASS( kVarInfo < _countof(VarInfo) );\
  CYVarInfo * pV = &(VarInfo[kVarInfo++]);\
  pV->sVar = (DWORD)((BYTE*)&_W.Var - (BYTE*)&_W);\
  pV->VarName = #Var;\
  pV->Type = 'D';\
  pV->Work = 0;\
  pV->szVarSize = sizeof(_W.Var);\
  }\
  {\
  ASS( kParmInfo < _countof(ParmInfo) );\
  CYParmInfo * pP = &(ParmInfo[kParmInfo++]);\
  pP->sVar_1 = (DWORD)((BYTE*)&_W.Var - (BYTE*)&_W);\
  pP->Name_1 = #VARaltNAME;\
  pP->Type = "D";\
  pP->ParmName = Name;\
  pP->ParmFmt = "%D";\
  pP->Read = 0;\
  pP->szVarSize = sizeof(_W.Var);\
  ASS( kVarInfo < _countof(VarInfo) );\
  CYVarInfo * pV = &(VarInfo[kVarInfo++]);\
  pV->sVar = (DWORD)((BYTE*)&_W.Var - (BYTE*)&_W);\
  pV->VarName = #VARaltNAME;\
  pV->Type = 'D';\
  pV->Work = 0;\
  pV->szVarSize = sizeof(_W.Var);\
  }\

#undef YPARM_H
#define YPARM_H(Var,Def,Name) \
  {\
  _W.Var = Def;\
  ASS( kParmInfo < _countof(ParmInfo) );\
  CYParmInfo * pP = &(ParmInfo[kParmInfo++]);\
  pP->sVar_1 = (DWORD)((BYTE*)&_W.Var - (BYTE*)&_W);\
  pP->Name_1 = #Var;\
  pP->Type = "D";\
  pP->ParmName = Name;\
  pP->ParmFmt = "%H";\
  pP->Read = 0;\
  pP->szVarSize = sizeof(_W.Var);\
  ASS( kVarInfo < _countof(VarInfo) );\
  CYVarInfo * pV = &(VarInfo[kVarInfo++]);\
  pV->sVar = (DWORD)((BYTE*)&_W.Var - (BYTE*)&_W);\
  pV->VarName = #Var;\
  pV->Type = 'D';\
  pV->Work = 0;\
  pV->szVarSize = sizeof(_W.Var);\
  }

#undef YPARM_B
#define YPARM_B(Var,Def,Name) \
  {\
  _W.Var = Def;\
  ASS( kParmInfo < _countof(ParmInfo) );\
  CYParmInfo * pP = &(ParmInfo[kParmInfo++]);\
  pP->sVar_1 = (DWORD)((BYTE*)&_W.Var - (BYTE*)&_W);\
  pP->Name_1 = #Var;\
  pP->Type = "B";\
  pP->ParmName = Name;\
  pP->ParmFmt = "%B";\
  pP->Read = 0;\
  pP->szVarSize = sizeof(_W.Var);\
  ASS( kVarInfo < _countof(VarInfo) );\
  CYVarInfo * pV = &(VarInfo[kVarInfo++]);\
  pV->sVar = (DWORD)((BYTE*)&_W.Var - (BYTE*)&_W);\
  pV->VarName = #Var;\
  pV->Type = 'B';\
  pV->Work = 0;\
  pV->szVarSize = sizeof(_W.Var);\
  }

#undef YPARM_FLAG
#define YPARM_FLAG(Var,Def,Name) \
  {\
  _W.Var = Def;\
  ASS( kParmInfo < _countof(ParmInfo) );\
  CYParmInfo * pP = &(ParmInfo[kParmInfo++]);\
  pP->sVar_1 = (DWORD)((BYTE*)&_W.Var - (BYTE*)&_W);\
  pP->Name_1 = #Var;\
  pP->Type = "W";\
  pP->ParmName = Name;\
  pP->ParmFmt = "%I";\
  pP->Read = 0;\
  pP->szVarSize = sizeof(_W.Var);\
  ASS( kVarInfo < _countof(VarInfo) );\
  CYVarInfo * pV = &(VarInfo[kVarInfo++]);\
  pV->sVar = (DWORD)((BYTE*)&_W.Var - (BYTE*)&_W);\
  pV->VarName = #Var;\
  pV->Type = 'W';\
  pV->Work = 0;\
  pV->szVarSize = sizeof(_W.Var);\
  }

#undef YPARM_S
#define YPARM_S(Var,Dim,Name) \
  {\
  memset ( _W.Var, 0, sizeof ( _W.Var ));\
  ASS( kParmInfo < _countof(ParmInfo) );\
  CYParmInfo * pP = &(ParmInfo[kParmInfo++]);\
  pP->Type = "S";\
  pP->sVar_1 = (DWORD)((BYTE*)_W.Var - (BYTE*)&_W);\
  pP->Name_1 = #Var;\
  pP->Type = "S";\
  pP->ParmName = Name;\
  pP->ParmFmt = "%S";\
  pP->Read = 0;\
  pP->sVar_2 = Dim;\
  pP->szVarSize = Dim;\
  ASS( kVarInfo < _countof(VarInfo) );\
  CYVarInfo * pV = &(VarInfo[kVarInfo++]);\
  pV->sVar = (DWORD)((BYTE*)&_W.Var - (BYTE*)&_W);\
  pV->VarName = #Var;\
  pV->Type = 'S';\
  pV->szVarSize = Dim;\
  pV->Work = 0;\
  }

#undef YPARM_S_A
#define YPARM_S_A(Var,Dim,Max,Desc) \
  {\
  for ( int n = 0; n < Dim; n++ )\
  {\
  memset ( _W.Var[n], 0, sizeof ( _W.Var[n] ));\
  ASS( kParmInfo < _countof(ParmInfo) );\
  CYParmInfo * pP = &(ParmInfo[kParmInfo++]);\
  pP->Type = "S";\
  pP->szVarSize = Max;\
  pP->sVar_1 = (DWORD)((BYTE*)_W.Var[n] - (BYTE*)&_W);\
  sprintf_s(_W.Var##Name[n], "%s%02d", #Var, n + 1 );\
  pP->Name_1 = _W.Var##Name[n];\
  pP->Type = "S";\
  pP->ParmName = Desc;\
  pP->ParmFmt = "%S";\
  pP->Read = 0;\
  pP->szVarSize = sizeof(_W.Var[n]);\
  pP->sVar_2 = Max;\
  ASS( kVarInfo < _countof(VarInfo) );\
  CYVarInfo * pV = &(VarInfo[kVarInfo++]);\
  pV->sVar = (DWORD)((BYTE*)&_W.Var[n] - (BYTE*)&_W);\
  pV->VarName = _W.Var##Name[n];\
  pV->Type = 'S';\
  pV->szVarSize = Max;\
  pV->Work = 0;\
  pV->szVarSize = sizeof(_W.Var[n]);\
  }\
  }

#undef YPARM_IID
#define YPARM_IID(Var_1,Var_2,Var_3,Def_1,Def_2,Def_3,Name,Fmt) \
  {\
  _W.Var_1 = Def_1;\
  _W.Var_2 = Def_2;\
  _W.Var_3 = DBL_MAX;\
  ASS( kParmInfo < _countof(ParmInfo) );\
  CYParmInfo * pP = &(ParmInfo[kParmInfo++]);\
  pP->Type = "IID";\
  pP->sVar_1 = (DWORD)((BYTE*)&_W.Var_1 - (BYTE*)&_W);\
  pP->sVar_2 = (DWORD)((BYTE*)&_W.Var_2 - (BYTE*)&_W);\
  pP->sVar_3 = (DWORD)((BYTE*)&_W.Var_3 - (BYTE*)&_W);\
  pP->Name_1 = #Var_1;\
  pP->Name_2 = #Var_2;\
  pP->Name_3 = #Var_3;\
  pP->ParmName = Name;\
  pP->ParmFmt = Fmt;\
  pP->Read = 0;\
  ASS( kVarInfo < _countof(VarInfo) );\
  CYVarInfo * pV = &(VarInfo[kVarInfo++]);\
  pV->sVar = (DWORD)((BYTE*)&_W.Var_1 - (BYTE*)&_W);\
  pV->VarName = #Var_1;\
  pV->Type = 'I';\
  pV->Work = 0;\
  pV->szVarSize = sizeof(_W.Var_1);\
  ASS( kVarInfo < _countof(VarInfo) );\
  pV = &(VarInfo[kVarInfo++]);\
  pV->sVar = (DWORD)((BYTE*)&_W.Var_2 - (BYTE*)&_W);\
  pV->VarName = #Var_2;\
  pV->Type = 'I';\
  pV->Work = 0;\
  pV->szVarSize = sizeof(_W.Var_2);\
  ASS( kVarInfo < _countof(VarInfo) );\
  pV = &(VarInfo[kVarInfo++]);\
  pV->sVar = (DWORD)((BYTE*)&_W.Var_3 - (BYTE*)&_W);\
  pV->VarName = #Var_3;\
  pV->Type = 'D';\
  pV->Work = 0;\
  pV->szVarSize = sizeof(_W.Var_3);\
  }

#undef YPARM_DDD
#define YPARM_DDD(Var_1,Var_2,Var_3,Def_1,Def_2,Def_3,Name,Fmt) \
  {\
  _W.Var_1 = DBL_MAX;\
  _W.Var_2 = DBL_MAX;\
  _W.Var_3 = DBL_MAX;\
  ASS( kParmInfo < _countof(ParmInfo) );\
  CYParmInfo * pP = &(ParmInfo[kParmInfo++]);\
  pP->Type = "DDD";\
  pP->sVar_1 = (DWORD)((BYTE*)&_W.Var_1 - (BYTE*)&_W);\
  pP->sVar_2 = (DWORD)((BYTE*)&_W.Var_2 - (BYTE*)&_W);\
  pP->sVar_3 = (DWORD)((BYTE*)&_W.Var_3 - (BYTE*)&_W);\
  pP->Name_1 = #Var_1;\
  pP->Name_2 = #Var_2;\
  pP->Name_3 = #Var_3;\
  pP->ParmName = Name;\
  pP->ParmFmt = Fmt;\
  pP->Read = 0;\
  ASS( kVarInfo < _countof(VarInfo) );\
  CYVarInfo * pV = &(VarInfo[kVarInfo++]);\
  pV->sVar = (DWORD)((BYTE*)&_W.Var_1 - (BYTE*)&_W);\
  pV->VarName = #Var_1;\
  pV->Type = 'D';\
  pV->Work = 0;\
  pV->szVarSize = sizeof(_W.Var_1);\
  ASS( kVarInfo < _countof(VarInfo) );\
  pV = &(VarInfo[kVarInfo++]);\
  pV->sVar = (DWORD)((BYTE*)&_W.Var_2 - (BYTE*)&_W);\
  pV->VarName = #Var_2;\
  pV->Type = 'D';\
  pV->Work = 0;\
  pV->szVarSize = sizeof(_W.Var_2);\
  ASS( kVarInfo < _countof(VarInfo) );\
  pV = &(VarInfo[kVarInfo++]);\
  pV->sVar = (DWORD)((BYTE*)&_W.Var_3 - (BYTE*)&_W);\
  pV->VarName = #Var_3;\
  pV->Type = 'D';\
  pV->Work = 0;\
  pV->szVarSize = sizeof(_W.Var_3);\
  }

#undef YPARM_4D
#define YPARM_4D(Var_1,Var_2,Var_3,Var_4,Def_1,Def_2,Def_3,Def_4,Name,Fmt) \
  {\
  _W.Var_1 = DBL_MAX;\
  _W.Var_2 = DBL_MAX;\
  _W.Var_3 = DBL_MAX;\
  _W.Var_4 = DBL_MAX;\
  ASS( kParmInfo < _countof(ParmInfo) );\
  CYParmInfo * pP = &(ParmInfo[kParmInfo++]);\
  pP->Type = "DDDD";\
  pP->sVar_1 = (DWORD)((BYTE*)&_W.Var_1 - (BYTE*)&_W);\
  pP->sVar_2 = (DWORD)((BYTE*)&_W.Var_2 - (BYTE*)&_W);\
  pP->sVar_3 = (DWORD)((BYTE*)&_W.Var_3 - (BYTE*)&_W);\
  pP->sVar_4 = (DWORD)((BYTE*)&_W.Var_4 - (BYTE*)&_W);\
  pP->Name_1 = #Var_1;\
  pP->Name_2 = #Var_2;\
  pP->Name_3 = #Var_3;\
  pP->Name_4 = #Var_4;\
  pP->ParmName = Name;\
  pP->ParmFmt = Fmt;\
  pP->Read = 0;\
  ASS( kVarInfo < _countof(VarInfo) );\
  CYVarInfo * pV = &(VarInfo[kVarInfo++]);\
  pV->sVar = (DWORD)((BYTE*)&_W.Var_1 - (BYTE*)&_W);\
  pV->VarName = #Var_1;\
  pV->Type = 'D';\
  pV->Work = 0;\
  pV->szVarSize = sizeof(_W.Var_1);\
  ASS( kVarInfo < _countof(VarInfo) );\
  pV = &(VarInfo[kVarInfo++]);\
  pV->sVar = (DWORD)((BYTE*)&_W.Var_2 - (BYTE*)&_W);\
  pV->VarName = #Var_2;\
  pV->Type = 'D';\
  pV->Work = 0;\
  pV->szVarSize = sizeof(_W.Var_2);\
  ASS( kVarInfo < _countof(VarInfo) );\
  pV = &(VarInfo[kVarInfo++]);\
  pV->sVar = (DWORD)((BYTE*)&_W.Var_3 - (BYTE*)&_W);\
  pV->VarName = #Var_3;\
  pV->Type = 'D';\
  pV->Work = 0;\
  pV->szVarSize = sizeof(_W.Var_3);\
  ASS( kVarInfo < _countof(VarInfo) );\
  pV = &(VarInfo[kVarInfo++]);\
  pV->sVar = (DWORD)((BYTE*)&_W.Var_4 - (BYTE*)&_W);\
  pV->VarName = #Var_4;\
  pV->Type = 'D';\
  pV->Work = 0;\
  pV->szVarSize = sizeof(_W.Var_4);\
  }

#undef YPARM_DDS
#define YPARM_DDS(Var_1,Var_2,Var_3,Def_1,Def_2,Dim_3,Name,Fmt) \
  {\
  _W.Var_1 = DBL_MAX;\
  _W.Var_2 = DBL_MAX;\
  memset( _W.Var_3, 0, Dim_3 );\
  ASS( kParmInfo < _countof(ParmInfo) );\
  CYParmInfo * pP = &(ParmInfo[kParmInfo++]);\
  pP->Type = "DDS";\
  pP->sVar_1 = (DWORD)((BYTE*)&_W.Var_1 - (BYTE*)&_W);\
  pP->sVar_2 = (DWORD)((BYTE*)&_W.Var_2 - (BYTE*)&_W);\
  pP->sVar_3 = (DWORD)((BYTE*)&_W.Var_3 - (BYTE*)&_W);\
  pP->Name_1 = #Var_1;\
  pP->Name_2 = #Var_2;\
  pP->Name_3 = #Var_3;\
  pP->ParmName = Name;\
  pP->ParmFmt = Fmt;\
  pP->Read = 0;\
  pP->szVarSize = Dim_3;\
  ASS( kVarInfo < _countof(VarInfo) );\
  CYVarInfo * pV = &(VarInfo[kVarInfo++]);\
  pV->sVar = (DWORD)((BYTE*)&_W.Var_1 - (BYTE*)&_W);\
  pV->VarName = #Var_1;\
  pV->Type = 'D';\
  pV->Work = 0;\
  pV->szVarSize = sizeof(_W.Var_1);\
  ASS( kVarInfo < _countof(VarInfo) );\
  pV = &(VarInfo[kVarInfo++]);\
  pV->sVar = (DWORD)((BYTE*)&_W.Var_2 - (BYTE*)&_W);\
  pV->VarName = #Var_2;\
  pV->Type = 'D';\
  pV->Work = 0;\
  pV->szVarSize = sizeof(_W.Var_2);\
  ASS( kVarInfo < _countof(VarInfo) );\
  pV = &(VarInfo[kVarInfo++]);\
  pV->sVar = (DWORD)((BYTE*)&_W.Var_3 - (BYTE*)&_W);\
  pV->VarName = #Var_3;\
  pV->Type = 'S';\
  pV->szVarSize = Dim_3;\
  pV->Work = 0;\
  }

#undef YPARM_BBB
#define YPARM_BBB(Var_1,Var_2,Var_3,Def_1,Def_2,Def_3,Name,Fmt) \
  {\
  _W.Var_1 = Def_1;\
  _W.Var_2 = Def_2;\
  _W.Var_3 = Def_3;\
  ASS( kParmInfo < _countof(ParmInfo) );\
  CYParmInfo * pP = &(ParmInfo[kParmInfo++]);\
  pP->Type = "BBB";\
  pP->sVar_1 = (DWORD)((BYTE*)&_W.Var_1 - (BYTE*)&_W);\
  pP->sVar_2 = (DWORD)((BYTE*)&_W.Var_2 - (BYTE*)&_W);\
  pP->sVar_3 = (DWORD)((BYTE*)&_W.Var_3 - (BYTE*)&_W);\
  pP->Name_1 = #Var_1;\
  pP->Name_2 = #Var_2;\
  pP->Name_3 = #Var_3;\
  pP->ParmName = Name;\
  pP->ParmFmt = Fmt;\
  pP->Read = 0;\
  ASS( kVarInfo < _countof(VarInfo) );\
  CYVarInfo * pV = &(VarInfo[kVarInfo++]);\
  pV->sVar = (DWORD)((BYTE*)&_W.Var_1 - (BYTE*)&_W);\
  pV->VarName = #Var_1;\
  pV->Type = 'B';\
  pV->Work = 0;\
  pV->szVarSize = sizeof(_W.Var_1);\
  ASS( kVarInfo < _countof(VarInfo) );\
  pV = &(VarInfo[kVarInfo++]);\
  pV->sVar = (DWORD)((BYTE*)&_W.Var_2 - (BYTE*)&_W);\
  pV->VarName = #Var_2;\
  pV->Type = 'B';\
  pV->Work = 0;\
  pV->szVarSize = sizeof(_W.Var_2);\
  ASS( kVarInfo < _countof(VarInfo) );\
  pV = &(VarInfo[kVarInfo++]);\
  pV->sVar = (DWORD)((BYTE*)&_W.Var_3 - (BYTE*)&_W);\
  pV->VarName = #Var_3;\
  pV->Type = 'B';\
  pV->Work = 0;\
  pV->szVarSize = sizeof(_W.Var_3);\
  }


#undef YPARM_DD
#define YPARM_DD(Var_1,Var_2,Def_1,Def_2,Name,Fmt) \
  {\
  _W.Var_1 = DBL_MAX;\
  _W.Var_2 = DBL_MAX;\
  ASS( kParmInfo < _countof(ParmInfo) );\
  CYParmInfo * pP = &(ParmInfo[kParmInfo++]);\
  pP->Type = "DD";\
  pP->sVar_1 = (DWORD)((BYTE*)&_W.Var_1 - (BYTE*)&_W);\
  pP->sVar_2 = (DWORD)((BYTE*)&_W.Var_2 - (BYTE*)&_W);\
  pP->Name_1 = #Var_1;\
  pP->Name_2 = #Var_2;\
  pP->ParmName = Name;\
  pP->ParmFmt = Fmt;\
  pP->Read = 0;\
  ASS( kVarInfo < _countof(VarInfo) );\
  CYVarInfo * pV = &(VarInfo[kVarInfo++]);\
  pV->sVar = (DWORD)((BYTE*)&_W.Var_1 - (BYTE*)&_W);\
  pV->VarName = #Var_1;\
  pV->Type = 'D';\
  pV->Work = 0;\
  pV->szVarSize = sizeof(_W.Var_1);\
  ASS( kVarInfo < _countof(VarInfo) );\
  pV = &(VarInfo[kVarInfo++]);\
  pV->sVar = (DWORD)((BYTE*)&_W.Var_2 - (BYTE*)&_W);\
  pV->VarName = #Var_2;\
  pV->Type = 'D';\
  pV->Work = 0;\
  pV->szVarSize = sizeof(_W.Var_2);\
  }

#undef YPARM_DD_savable
#define YPARM_DD_savable(Var_1,Var_2,Def_1,Def_2,Name,Fmt) \
  {\
  _W.Var_1 = Def_1;\
  _W.Var_2 = Def_2;\
  ASS( kParmInfo < _countof(ParmInfo) );\
  CYParmInfo * pP = &(ParmInfo[kParmInfo++]);\
  pP->Type = "DD";\
  pP->sVar_1 = (DWORD)((BYTE*)&_W.Var_1 - (BYTE*)&_W);\
  pP->sVar_2 = (DWORD)((BYTE*)&_W.Var_2 - (BYTE*)&_W);\
  pP->Name_1 = #Var_1;\
  pP->Name_2 = #Var_2;\
  pP->ParmName = Name;\
  pP->ParmFmt = Fmt;\
  pP->Read = 0;\
  ASS( kVarInfo < _countof(VarInfo) );\
  CYVarInfo * pV = &(VarInfo[kVarInfo++]);\
  pV->sVar = (DWORD)((BYTE*)&_W.Var_1 - (BYTE*)&_W);\
  pV->VarName = #Var_1;\
  pV->Type = 'D';\
  pV->Work = 0;\
  pV->szVarSize = sizeof(_W.Var_1);\
  ASS( kVarInfo < _countof(VarInfo) );\
  pV = &(VarInfo[kVarInfo++]);\
  pV->sVar = (DWORD)((BYTE*)&_W.Var_2 - (BYTE*)&_W);\
  pV->VarName = #Var_2;\
  pV->Type = 'D';\
  pV->Work = 0;\
  pV->szVarSize = sizeof(_W.Var_2);\
  }

#undef YPARM_DB
#define YPARM_DB(Var_1,Var_2,Def_1,Def_2,Name,Fmt) \
  {\
  _W.Var_1 = DBL_MAX;\
  _W.Var_2 = Def_2;\
  ASS( kParmInfo < _countof(ParmInfo) );\
  CYParmInfo * pP = &(ParmInfo[kParmInfo++]);\
  pP->Type = "DB";\
  pP->sVar_1 = (DWORD)((BYTE*)&_W.Var_1 - (BYTE*)&_W);\
  pP->sVar_2 = (DWORD)((BYTE*)&_W.Var_2 - (BYTE*)&_W);\
  pP->Name_1 = #Var_1;\
  pP->Name_2 = #Var_2;\
  pP->ParmName = Name;\
  pP->ParmFmt = Fmt;\
  pP->Read = 0;\
  ASS( kVarInfo < _countof(VarInfo) );\
  CYVarInfo * pV = &(VarInfo[kVarInfo++]);\
  pV->sVar = (DWORD)((BYTE*)&_W.Var_1 - (BYTE*)&_W);\
  pV->VarName = #Var_1;\
  pV->Type = 'D';\
  pV->Work = 0;\
  pV->szVarSize = sizeof(_W.Var_1);\
  ASS( kVarInfo < _countof(VarInfo) );\
  pV = &(VarInfo[kVarInfo++]);\
  pV->sVar = (DWORD)((BYTE*)&_W.Var_2 - (BYTE*)&_W);\
  pV->VarName = #Var_2;\
  pV->Type = 'B';\
  pV->Work = 0;\
  pV->szVarSize = sizeof(_W.Var_2);\
  }

//////////////////////////////////////////////////////
#undef  UMODE
#define UMODE \
  {\
  ASS( kVarInfo < _countof(VarInfo) );\
  CYVarInfo * pP = &(VarInfo[kVarInfo++]);\
  pP->sVar = (DWORD)((BYTE*)&_W.mode.M - (BYTE*)&_W);\
  pP->VarName = "MODE";\
  pP->Type = 'I';\
  pP->Work = 1;\
  pP->szVarSize = sizeof(_W.mode.M);\
  }

//
#undef FLAG
#define FLAG(Var,Comm) \
  {\
  ASS( kVarInfo < _countof(VarInfo) );\
  CYVarInfo * pP = &(VarInfo[kVarInfo++]);\
  pP->sVar = (DWORD)((BYTE*)&_W.Var - (BYTE*)&_W);\
  pP->VarName = #Var;\
  pP->Type = 'W';\
  pP->Work = 1;\
  pP->szVarSize = sizeof(_W.Var);\
  }
//
#undef DOUBLE
#define DOUBLE(Var,Comm) \
  {\
  ASS( kVarInfo < _countof(VarInfo) );\
  CYVarInfo * pP = &(VarInfo[kVarInfo++]);\
  pP->sVar = (DWORD)((BYTE*)&_W.Var - (BYTE*)&_W);\
  pP->sVarTune = 0;\
  pP->VarName = #Var;\
  pP->Type = 'D';\
  pP->Work = 1;\
  pP->szVarSize = sizeof(_W.Var);\
  }

#undef DOUBLE_A
#define DOUBLE_A(Var,Dim,Comm) \
  {\
  for ( int n = 0; n < Dim; n++ )\
  {\
  ASS( kVarInfo < _countof(VarInfo) );\
  CYVarInfo * pP = &(VarInfo[kVarInfo++]);\
  pP->sVar = (DWORD)((BYTE*)&_W.Var[n] - (BYTE*)&_W);\
  sprintf_s(_W.Var##Name[n], "%s%02d", #Var, n + 1 );\
  pP->VarName = _W.Var##Name[n];\
  pP->Type = 'D';\
  pP->Work = 1;\
  pP->szVarSize = sizeof(_W.Var[n]);\
  if ( Comm[0] == 'P' ) pP->Work = 0;\
  }\
  }

#undef DOUBLE2
#define DOUBLE2(Var,Var_Tune,Comm) \
  {\
  ASS( kVarInfo < _countof(VarInfo) );\
  CYVarInfo * pP = &(VarInfo[kVarInfo++]);\
  pP->sVar = (DWORD)((BYTE*)&_W.Var - (BYTE*)&_W);\
  pP->sVarTune = (DWORD)((BYTE*)&_W.Var_Tune - (BYTE*)&_W);\
  pP->VarName = #Var;\
  pP->Type = 'D';\
  pP->Work = 1;\
  pP->szVarSize = sizeof(_W.Var);\
  }

#undef INTEGER
#define INTEGER(Var,Comm) \
  {\
  ASS( kVarInfo < _countof(VarInfo) );\
  CYVarInfo * pP = &(VarInfo[kVarInfo++]);\
  pP->sVar = (DWORD)((BYTE*)&_W.Var - (BYTE*)&_W);\
  pP->VarName = #Var;\
  pP->Type = 'I';\
  pP->Work = 1;\
  pP->szVarSize = sizeof(_W.Var);\
  }

#undef INTEGER_A
#define INTEGER_A(Var,Dim,Desc) \
  {\
  for ( int n = 0; n < Dim; n++ )\
  {\
  ASS( kVarInfo < _countof(VarInfo) );\
  CYVarInfo * pP = &(VarInfo[kVarInfo++]);\
  pP->sVar = (DWORD)((BYTE*)&_W.Var[n] - (BYTE*)&_W);\
  sprintf_s(_W.Var##Name[n], "%s%02d", #Var, n + 1 );\
  pP->VarName = _W.Var##Name[n];\
  pP->Type = 'I';\
  pP->Work = 1;\
  pP->szVarSize = sizeof(_W.Var[n]);\
  if ( Desc[0] == 'P' ) pP->Work = 0;\
  }\
  }

#undef ENUM
#define ENUM(Var,Desc,...) \
  {\
  ASS( kVarInfo < _countof(VarInfo) );\
  CYVarInfo * pP = &(VarInfo[kVarInfo++]);\
  pP->sVar = (DWORD)((BYTE*)&_W.Var - (BYTE*)&_W);\
  pP->VarName = #Var;\
  pP->Type = 'W';\
  pP->Work = 1;\
  pP->bEnum = true;\
  pP->szVarSize = sizeof(_W.Var);\
  pP->pszEnumVals = #__VA_ARGS__;\
  }


#undef  STRING
#define STRING(Var,Max,Desc) \
  {\
  ASS( kVarInfo < _countof(VarInfo) );\
  CYVarInfo * pP = &(VarInfo[kVarInfo++]);\
  pP->sVar = (DWORD)((BYTE*)&_W.Var - (BYTE*)&_W);\
  pP->VarName = #Var;\
  pP->Type = 'S';\
  pP->szVarSize = Max;\
  pP->Work = 1;\
  }

#undef  STRING_skipSave
#define STRING_skipSave(Var,Max,Desc) \
  {\
  ASS( kVarInfo < _countof(VarInfo) );\
  CYVarInfo * pP = &(VarInfo[kVarInfo++]);\
  pP->sVar = (DWORD)((BYTE*)&_W.Var - (BYTE*)&_W);\
  pP->VarName = #Var;\
  pP->Type = 'S';\
  pP->szVarSize = Max;\
  pP->Work = 4;\
  }

#undef  ACK_ALARMS
#define ACK_ALARMS \
  {\
  ASS( kVarInfo < _countof(VarInfo) );\
  CYVarInfo * pP = &(VarInfo[kVarInfo++]);\
  pP->sVar = (DWORD)((BYTE*)&_W.ALRM - (BYTE*)&_W);\
  pP->VarName = "ALRM";\
  pP->Type = 'I';\
  pP->Work = 2;\
  pP->szVarSize = sizeof(_W.ALRM);\
  pP = &(VarInfo[kVarInfo++]);\
  pP->sVar = (DWORD)((BYTE*)&_W.XALRM - (BYTE*)&_W);\
  pP->VarName = "XALRM";\
  pP->Type = 'W';\
  pP->Work = 2;\
  pP->szVarSize = sizeof(_W.XALRM);\
  ASS( kVarInfo < _countof(VarInfo) );\
  pP = &(VarInfo[kVarInfo++]);\
  pP->sVar = (DWORD)((BYTE*)&_W.notACK - (BYTE*)&_W);\
  pP->VarName = "AFLS";\
  pP->Type = 'W';\
  pP->Work = 2;\
  pP->szVarSize = sizeof(_W.notACK);\
  ASS( kVarInfo < _countof(VarInfo) );\
  pP = &(VarInfo[kVarInfo++]);\
  pP->sVar = (DWORD)((BYTE*)&_W.ALRMmask - (BYTE*)&_W);\
  pP->VarName = "ALRMmask";\
  pP->Type = 'L';\
  pP->Work = 2;\
  pP->szVarSize = sizeof(_W.ALRMmask);\
  ASS( kVarInfo < _countof(VarInfo) );\
  pP = &(VarInfo[kVarInfo++]);\
  pP->sVar = (DWORD)((BYTE*)&_W.szALRM - (BYTE*)&_W);\
  strcpy_s( _W.szALRM, "NR" );\
  pP->VarName = "@ALRM";\
  pP->Type = 'S';\
  pP->Work = 2;\
  pP->szVarSize = sizeof(_W.szALRM);\
  }

#undef  STRING_A
#define STRING_A(Var,Dim,Max,Desc) \
  {\
  for ( int n = 0; n < Dim; n++ )\
  {\
  ASS( kVarInfo < _countof(VarInfo) );\
  CYVarInfo * pP = &(VarInfo[kVarInfo++]);\
  pP->sVar = (DWORD)((BYTE*)&_W.Var[n] - (BYTE*)&_W);\
  sprintf_s(_W.Var##Name[n], "%s%02d", #Var, n + 1 );\
  pP->VarName = _W.Var##Name[n];\
  pP->Type = 'S';\
  pP->szVarSize = Max;\
  pP->Work = 1;\
  }\
  }
//
#undef YPARM_VARIANT
#define YPARM_VARIANT(Name) \
  {\
  ASS( kParmInfo < _countof(ParmInfo) );\
  CYParmInfo * pP = &(ParmInfo[kParmInfo++]);\
  pP->sVar_1 = 0;\
  pP->Name_1 = Name;\
  pP->Type = "V";\
  pP->ParmName = Name;\
  pP->ParmFmt = NULL;\
  pP->Read = 0;\
  }

