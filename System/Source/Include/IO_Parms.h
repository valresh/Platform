#undef PNT
#define PNT(Class,Val,ConnName)
#define CParams QParams
#undef TAB
#define TAB(Name,Level) \
{\
	char __szTabName[MAX_PARM_NAME+1];\
	lstrcpy(__szTabName, Name);\
  {\
  static int Show = 0;\
  if ( !CParams::SaveAllParams && !CParams::ReadProps )\
    {\
    CParams & __P = CParams::Get();\
    __P.SetDefaultValue(NULL);\
    __P.Set( this, NULL, Show, 4 );\
    __P.pDefValue = (void*)Level;\
    __P.Type  = 'T';\
    __P.Class = 'P';\
    __P.SafeCopyTab( $ Name );\
    __P.Property = false;\
    }\
  }

#undef ETAB
#define ETAB \
  {\
  if ( !CParams::SaveAllParams && !CParams::ReadProps )\
    {\
    CParams & __P = CParams::Get();\
    __P.SetDefaultValue(NULL);\
    __P.Type  = 'E';\
    __P.Class = 'P';\
    __P.SafeCopyTab( $ __szTabName );\
    __P.Property = false;\
    }\
  }\
}

#undef TABO
#define TABO(Name,Level) \
  {\
  static int Show = 0;\
  if ( !CParams::SaveAllParams && !CParams::ReadProps )\
    {\
    CParams & __P = CParams::Get();\
    __P.SetDefaultValue(NULL);\
    __P.Set( this, NULL, Show, 4 );\
    __P.pDefValue = (void*)Level;\
    __P.Type  = 'T';\
    __P.Class = 'P';\
    __P.SafeCopyTab( $ Name );\
    __P.Property = false;\
    }\
  }

#undef ETABO
#define ETABO(Name)\
  {\
  if ( !CParams::SaveAllParams && !CParams::ReadProps )\
    {\
    CParams & __P = CParams::Get();\
    __P.SetDefaultValue(NULL);\
    __P.Type  = 'E';\
    __P.Class = 'P';\
    __P.SafeCopyTab( $ Name );\
    __P.Property = false;\
    }\
  }


#undef TABS
#define TABS(Name,S) \
  {\
  static int Show = 0;\
  if ( !CParams::SaveAllParams && !CParams::ReadProps )\
    {\
    CParams & __P = CParams::Get();\
    __P.SetDefaultValue(NULL);\
    __P.Set( this, NULL, Show, 4 );\
    __P.pDefValue = NULL;\
    __P.Type  = 'T';\
    __P.Class = *#S;\
    __P.SafeCopyTab( $ Name );\
    __P.Property = false;\
    }\
  }

#undef TAB_TABLE
#define TAB_TABLE(Name,Level) \
{\
	char* __szTabName = Name;\
  {\
  static int Show = 0;\
  if ( !CParams::SaveAllParams && !CParams::ReadProps )\
	{\
	CParams & __P = CParams::Get();\
	__P.SetDefaultValue(NULL);\
	__P.Set( this, NULL, Show, 4 );\
	__P.pDefValue = (void*)Level;\
	__P.Type  = 'T';\
	__P.Class = 'P';\
	__P.SafeCopyTab( $ __szTabName );\
	__P.Property = false;\
	__P.dwFlags |= CParams::FLAG_TABLE;\
	}\
  }

#undef DESC
#define DESC(Desc) \
  {\
  if ( !CParams::SaveAllParams )\
    {\
    CParams & __P = CParams::GetLast();\
    __P.pModelDescr = (char*)Desc;\
    }\
  }



#undef ABSADDR
#define ABSADDR \
  {\
  CParams & __P = CParams::GetLast();\
  __P.AbsAddr = true;\
  }

#undef PARM_D
#define PARM_D(Val,Name,Def) \
	{\
  static double default_Val = Def;\
  if ( CParams::AddParmToList( Name ) )\
    {\
    CParams & __P = CParams::Get();\
    __P.SetDefaultValue(&default_Val);\
	  __P.Set( this, $ Name, Val, sizeof ( Val ) );\
    __P.Property = false;\
    }\
	}


#undef PARM_I
#define PARM_I(Val,Name,Def) \
	{\
  static int default_Val = Def;\
  if ( CParams::AddParmToList( Name ) )\
    {\
    CParams & __P = CParams::Get();\
    __P.SetDefaultValue(&default_Val);\
	  __P.Set( this, $ Name, Val, sizeof ( Val ) );\
    __P.Property = false;\
    }\
	}

#undef PARM_B
#define PARM_B(Val,Name,Def) \
	{\
  static bool default_Val = Def;\
  if ( CParams::AddParmToList( Name ) )\
    {\
    CParams & __P = CParams::Get();\
    __P.SetDefaultValue(&default_Val);\
	  __P.Set( this, $ Name, Val, sizeof ( Val ) );\
    __P.Property = false;\
    }\
	}

#undef PARM_S
#define PARM_S(Val,Max,Name,Def) \
	{\
  static char * default_Val = $ Def;\
  if ( CParams::AddParmToList( Name ) )\
    {\
    CParams & __P = CParams::Get();\
    __P.Len = sizeof(Val);\
    __P.SetDefaultValue(default_Val);\
	__P.Set( this, $ Name, Val, __P.Len);\
    __P.Property = false;\
    }\
  }


#undef PARM_U
#define PARM_U(Val,Name,Def) \
	{\
	static char * default_Val = $ Def;\
  if ( CParams::AddParmToList( Name ) )\
    {\
  	CParams & __P = CParams::Get();\
	  __P.SetDefaultValue(default_Val);\
	  __P.Set( this, $ Name, Val );\
    __P.Property = false;\
    }\
	}

//////////////////////////////////////////////////////
#undef PROP_D
#define PROP_D(Val,Name,Def) \
  {\
  static double default_Val = Def;\
  if ( CParams::AddPropToList( Name ) )\
    {\
    CParams & __P = CParams::Get();\
    __P.SetDefaultValue(&default_Val);\
    __P.Set( this, $ Name, Val, sizeof ( Val ) );\
    __P.Property = true;\
    }\
  }

#undef PROP_I
#define PROP_I(Val,Name,Def) \
  {\
  static int default_Val = Def;\
  if ( CParams::AddPropToList( Name ) )\
    {\
    CParams & __P = CParams::Get();\
    __P.SetDefaultValue(&default_Val);\
    __P.Set( this, $ Name, Val, sizeof ( Val ) );\
    __P.Property = true;\
    }\
  }


#undef PROP_B
#define PROP_B(Val,Name,Def) \
  {\
  static bool default_Val = Def;\
  if ( CParams::AddPropToList( Name ) )\
    {\
    CParams & __P = CParams::Get();\
    __P.SetDefaultValue(&default_Val);\
    __P.Set( this, $ Name, Val, sizeof ( Val ) );\
    __P.Property = true;\
    }\
  }


#undef PROP_S
#define PROP_S(Val,Max,Name,Def) \
  {\
  static char * default_Val = Def;\
  if ( CParams::AddPropToList( Name ) )\
    {\
    CParams & __P = CParams::Get();\
    __P.Len = Max;\
    __P.SetDefaultValue(default_Val);\
    __P.Set( this, $ Name, Val, Max );\
    __P.Property = true;\
    }\
  }

#undef PROP_U
#define PROP_U(Val,Name,Def) \
  {\
  static char * default_Val = Def;\
  if ( CParams::AddPropToList( Name ) )\
    {\
    CParams & __P = CParams::Get();\
    __P.SetDefaultValue(default_Val);\
    __P.Set( this, $ Name, Val );\
    __P.Property = true;\
    }\
  }

#undef PROP
#define PROP(Val,Name) \
  {\
  if ( CParams::AddPropToList( Name ) )\
    {\
    CParams & __P = CParams::Get();\
    __P.SetDefaultValue(NULL);\
    __P.Set( this, $ Name, Val, sizeof ( Val ) );\
    __P.Property = true;\
    }\
  }


#undef PROP_L
#define PROP_L(Val,Name,Def) \
  {\
  if ( CParams::AddPropToList( Name ) )\
    {\
    CParams & __P = CParams::Get();\
    __P.pDefValue = $ Def;\
    __P.SetAddr ( this, $ Name, &Val, 'L', 4 );\
    __P.Property = true;\
    }\
  }

#undef PROP_E
#define PROP_E(Val,Name) \
  {\
  if ( CParams::AddPropToList( Name ) )\
    {\
    static char List[256];\
    if ( GetPropList( Val, Name, List ))\
      {\
      CParams & __P = CParams::Get();\
      __P.pDefValue = $ List;\
      __P.SetAddr ( this, $ Name, &Val.NumbSelect, 'L', 4 );\
      __P.Property = true;\
      }\
    }\
  }

#undef BEG_TEST_DEF
#define BEG_TEST_DEF \
  {\
  CParams & __P = CParams::Get();\
  __P.Type = 'Z';\
  }

#undef END_TEST_DEF
#define END_TEST_DEF \
  {\
  CParams & __P = CParams::Get();\
  __P.Type = 'z';\
  }

#undef HIDE
#define HIDE \
  {\
  return CParams::Params[CParams::kParams-1].Visible = -1;\
  }

#ifdef PARM_STRUCT
#undef PARM
#define PARM(Val,Name) \
  {\
  CParams & __P = CParams::Get();\
  __P.SetDefaultValue(NULL);\
  __P.Set( NULL, Name, Val, sizeof ( Val ) );\
  }

#undef PARM_L
#define PARM_L(Val,Name,Def) \
  {\
  CParams & __P = CParams::Get();\
  __P.pDefValue = Def;\
  __P.SetAddr ( NULL, Name, &Val, 'L', 4 );\
  }

#else
#undef PARM
#define PARM(Val,Name) \
  {\
  if ( CParams::AddParmToList( Name ) )\
    {\
    CParams & __P = CParams::Get();\
    __P.SetDefaultValue(NULL);\
	  __P.Set( this, $ Name, Val, sizeof ( Val ) );\
    __P.Property = false;\
    }\
  }
#undef PARAM
#define PARAM(Val,Name) \
  {\
  if ( CParams::AddParmToList( Name ) )\
	{\
	CParams & __P = CParams::Get();\
	__P.SetDefaultValue(NULL);\
	__P.Set( this, $ Name, Val, sizeof ( Val ) );\
	__P.Property = false;\
	__P.dwFlags = CParams::FLAG_PARAM;\
	}\
  }
#undef STATE
#define STATE(Val,Name) \
  {\
  if ( CParams::AddParmToList( Name ) )\
	{\
	CParams & __P = CParams::Get();\
	__P.SetDefaultValue(NULL);\
	__P.Set( this, $ Name, Val, sizeof ( Val ) );\
	__P.Property = false;\
	__P.dwFlags = CParams::FLAG_STATE;\
	}\
  }
#undef STATE_PARAM
#define STATE_PARAM(Val,Name) \
  {\
  if ( CParams::AddParmToList( Name ) )\
	{\
	CParams & __P = CParams::Get();\
	__P.SetDefaultValue(NULL);\
	__P.Set( this, $ Name, Val, sizeof ( Val ) );\
	__P.Property = false;\
	__P.dwFlags = CParams::FLAG_STATE | CParams::FLAG_PARAM;\
	}\
  }
#undef PARMU
#define PARMU(Val,Name,pUsed) \
  {\
  if ( CParams::AddParmToList( Name ) )\
    {\
    CParams & __P = CParams::Get();\
    __P.SetDefaultValue(NULL);\
    __P.Set( this, $ Name, Val, sizeof ( Val ) );\
    __P.Used = pUsed;\
    __P.Property = false;\
    }\
  }
 
#undef PARM_L
#define PARM_L(Val,Name,Def) \
  {\
  if ( CParams::AddParmToList( Name ) )\
    {\
    CParams & __P = CParams::Get();\
    __P.pDefValue = (void*)$ Def;\
    __P.SetAddr ( this, $ Name, &Val, 'L', 4 );\
    __P.Property = false;\
	  }\
  }

#undef PARM_LL
#define PARM_LL(Val,Name,Def) \
  {\
  if ( CParams::AddParmToList( Name ) )\
	{\
	CParams & __P = CParams::Get();\
	__P.pDefValue = (void*)$ Def;\
	__P.SetAddr ( this, $ Name, &Val, 'L', 4 );\
	__P.Property = false;\
	  }\
  }

#endif  
#undef PARM_IF
#define PARM_IF(Val)\
  {\
  if ( !CParams::SaveAllParams )\
    {\
    CParams & __P = CParams::Get();\
    __P.SetDefaultValue(NULL);\
    if ( Val )\
      __P.SetAddr( this, #Val, NULL, 'Y', 1 );\
    else\
      __P.SetAddr( this, #Val, NULL, 'Y', 0 );\
    }\
  }

#undef PARM_END
#define PARM_END(Val)\
  {\
  if ( !CParams::SaveAllParams )\
    {\
    CParams & __P = CParams::Get();\
    __P.SetDefaultValue(NULL);\
    __P.SetAddr( this, #Val, NULL, 'Y', -1 );\
    }\
  }

#undef TAB_S
#define TAB_S TAB
#undef PARM_D_S
#define PARM_D_S PARM_D
#undef PARM_L_S
#define PARM_L_S PARM_L
#undef PARM_I_S
#define PARM_I_S PARM_I
#undef PARM_B_S
#define PARM_B_S PARM_B
#undef PARM_S_S
#define PARM_S_S PARM_S
#undef PARM_U_S
#define PARM_U_S PARM_U
#undef PARM_A_S
#define PARM_A_S PARM_D

#undef PARM_FMT
#define PARM_FMT(Val,Fmt,...) \
  {\
  char Name[256];\
  sprintf_s ( Name, sizeof ( Name ), Fmt, __VA_ARGS__ );\
  CParams & __P = CParams::Get();\
  __P.SetDefaultValue(NULL);\
	__P.Set( this, Name, Val, sizeof ( Val ) );\
  }

///////////////////////////////////////////////////////////////
// отмена макросов
#undef MODEL
#undef DEFECT
#undef OBJ_DEFECT
#undef SUB_DEFECT
#undef EVENT
#undef DEF_D	// double
#undef DEF_B	// bool	
#undef DEF_I	// int
#undef DEF_S	// char*
#undef DEF_CB	// combo
// модификаторы
#undef PMIN
#undef PMAX
#undef PMINMAX
// 
#define DEFECT( Name, Comment ) \
  {\
  if ( Name && !CParams::SaveAllParams )\
    {\
    CParams & P = CParams::Get();\
    P.SetDefaultValue(NULL);\
    char ParmName[256];\
	  if ( Name->Flags & CDef::Active )\
		  sprintf_s ( ParmName, 256, "#+%s", $ Comment );\
    else\
		  sprintf_s ( ParmName, 256, "#-%s", $ Comment );\
	  P.SetDefect( this, ParmName, Name );\
    P.Class = 'D';\
    }\
  }


#define OBJ_DEFECT( Obj, Defect, Comment ) \
  {\
  CParams & P = CParams::Get();\
  P.SetDefaultValue(NULL);\
  char ParmName[256];\
  if ( Obj.Defect.Active )\
		sprintf_s ( ParmName, 256, "#+%s", $ Comment );\
  else\
		sprintf_s ( ParmName, 256, "#-%s", $ Comment );\
	P.Set( this, ParmName, Obj.Defect.Fire, sizeof ( Obj.Defect.Fire ) );\
  P.Class = 'D';\
  }
#define SUB_DEFECT( Obj )

#define EVENT(Name, Comment ) DEFECT( Name, Comment )
#define PMIN(Value) 
#define PMAX(Value) 
#define PMINMAX(Min,Max) 

#define DEF_D(Val,Name,Def,Min,Max) \
  if ( Val && !CParams::SaveAllParams )\
    {\
    CParams & P = CParams::Get();\
    P.SetDefaultValue(&Val->_Dbl);\
    P.m_bNoSave = true;\
	  P.Set( this, $ Name, Val->Dbl, sizeof ( Val->Dbl ) );\
    P.Class = 'D';\
    }

#define DEF_B(Val,Name,Def) \
  if ( Val && !CParams::SaveAllParams )\
    {\
    CParams & P = CParams::Get();\
    P.SetDefaultValue(&Val->_Bool);\
    P.m_bNoSave = true;\
    P.Set( this, $ Name, Val->Bool, sizeof ( Val->Bool ) );\
    P.Class = 'D';\
    }

#define DEF_I(Val,Name,Def,Min,Max) \
  if ( Val && !CParams::SaveAllParams )\
    {\
    CParams & P = CParams::Get();\
    P.SetDefaultValue(&Val->_Int);\
    P.m_bNoSave = true;\
	  P.Set( this, $ Name, Val->Int, sizeof ( Val->Int ) );\
    P.Class = 'D';\
    }


#define MODEL(Name,Default)
//defDefaultDefects = Default;
//ASS(N<MAX__PARAMS);
//Params[N].SetDefaultValue( &defDefaultDefects );
//Params[N++].Set( this, "#Возможны отказы", DefaultDefects, sizeof ( DefaultDefects ) );

#undef IF
#define IF(Cond) if ( (Cond) || CParams::SaveAllParams ) {

#undef ENDIF
#define ENDIF }
