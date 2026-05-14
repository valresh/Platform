#undef PNT
#define PNT(Class,Val,Name) Class * Val;
// Параметры
#undef TAB
#define TAB(Name,Level)

#undef ETAB
#define ETAB

#undef TABS
#define TABS(Name,Type)

#undef TAB_TABLE
#define TAB_TABLE(Name,Level)

#undef ABSADDR
#define ABSADDR

#undef DESC
#define DESC(Desc)

#undef PARM_CTRL
#define PARM_CTRL(Class,n,Max,Cmd,Param)

#undef PARM_D
#define PARM_D(Val,Name,Def) \
  double Val;

#undef PARM_I
#define PARM_I(Val,Name,Def) \
  int Val;

#undef PARM_L
#define PARM_L(Val,Name,Def) \
  int Val;

#undef PARM_LL
#define PARM_LL(Val,Name,Def) \

#undef PARM_B
#define PARM_B(Val,Name,Def) \
  bool Val;

#undef PARM_S
#define PARM_S(Val,Dim,Name,Def) \
  char Val[Dim*4+1];

#undef PARM_U
#define PARM_U(Val,Name,Def) \
	CStr Val;

#undef PARM
#define PARM(Val,Name)

#undef PARAM
#define PARAM(Val,Name)

#undef STATE
#define STATE(Val,Name)

#undef STATE_PARAM
#define STATE_PARAM(Val,Name)
////////////////////////////////////////////////
#undef XPARM
#define XPARM(Ref,Val,Name)

#undef PROP_D
#define PROP_D(Val,Name,Def) \
  double Val;

#undef PROP_I
#define PROP_I(Val,Name,Def) \
  int Val;

#undef PROP_L
#define PROP_L(Val,Name,Def) \
  int Val;

#undef PROP_E
#define PROP_E(Val,Name) \
  CProp_L Val;

#undef PROP_B
#define PROP_B(Val,Name,Def) \
  bool Val;

#undef PROP_S
#define PROP_S(Val,Dim,Name,Def) \
  char Val[Dim+1];

#undef PROP_U
#define PROP_U(Val,Name,Def) \
  CStr Val;

#undef PROP
#define PROP(Val,Name)
////////////////////////////////////////////////

#undef PARM_IF
#define PARM_IF(Val)

#undef PARM_END
#define PARM_END(Val)

#undef HIDE
#define HIDE

#undef __U
#define __U

#undef __I
#define __I

#undef __D
#define __D

#undef __X
#define __X

///////////////////////////////////////////////////////////
#undef TAB_S
#define TAB_S TAB
#undef PARMS
#define PARMS PARM
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
#define PARM_A_S(Val,Name,Def)

///////////////////////////////////////////////////////////////////////////////////////////
// отмена макросов
#undef DEFECT
#undef ZDEFECT
#undef OBJ_DEFECT
#undef EVENT
#undef MODEL
#undef DEF_D	// double
#undef DEF_B	// bool	
#undef DEF_I	// int
// модификаторы
#undef PMIN
#undef PMAX
#undef PMINMAX
///////////////////////////////////////////////////////////////////////////////////////////

#ifndef NO_DEFECT
// определение макросов для создания данных в классе
#define MODEL(Name,Default)  static DWORD ClassRootNode; DWORD LastDefect; CDef * ObjRoot;
#define DEFECT( Name, Comment ) CDef * Name;
#define OBJ_DEFECT( Obj, Defect, Comment )
#define EVENT( Name, Comment ) DEFECT( Name, Comment )
#define PMIN(Value) 
#define PMAX(Value) 
#define PMINMAX(Min,Max) 

#define DEF_D(Var, Comment, Val, Min, Max)	\
	CDefParam * Var;

#define DEF_B(Var, pname, Val)	\
	CDefParam * Var;

#define DEF_I(Var, pname, Val, Min, Max)	\
	CDefParam * Var;

#ifndef NO_DEFECTS
//virtual int OnDefect( struct CDef * pDefect );
//void SetDefect( double dt );
//bool defDefaultDefects;
//char CurrDefect[64];
#endif

#define IF_DEFECT(Defect,Comment) \
	if ( pDefect == NULL || pDefect == Defect )\
		{\

#define IF_EVENT(Comment) \
	if ( pDefect == NULL || pDefect == Defect )\
		{\
		if ( pDefect )\
			{\
			IsDefect = true;\
			lstrcpy ( CurrDefect, pDefect->Name );\
			}

#define END_IF \
		if ( pDefect )\
		IBaseModel::OnDefect(pDefect);\
		return 1;\
		}
#endif


#undef IF
#define IF(Cond)

#undef ENDIF
#define ENDIF
