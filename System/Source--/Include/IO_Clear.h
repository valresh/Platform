#undef PNT
#define PNT(Class,Val,ConnName) Val = NULL;

// Параметры
#undef TAB
#define TAB(Name,Level)

#undef ETAB
#define ETAB

#undef TABS
#define TABS(Name,S)

#undef TAB_TABLE
#define TAB_TABLE(Name,Level)

#undef DESC
#define DESC( Desc )

#undef ABSADDR
#define ABSADDR

#undef PARM_D
#define PARM_D(Val,Name,Def) \
  Val = Def;

#undef ZPARM_D
#define ZPARM_D(Val,Name,Def) \
  Val = Def;

#undef PARM_I
#define PARM_I(Val,Name,Def) \
  Val = Def;

#undef ZPARM_I
#define ZPARM_I(Val,Name,Def) \
  Val = Def;

#undef PARM_L
#define PARM_L(Val,Name,Def) \
  Val = 0;

#undef PARM_LL
#define PARM_LL(Val,Name,Def) \
	Val = 0;

#undef ZPARM_L
#define ZPARM_L(Val,Name,Def) \
  Val = 0;

#undef PARM_B
#define PARM_B(Val,Name,Def) \
  Val = Def;

#undef ZPARM_B
#define ZPARM_B(Val,Name,Def) \
  Val = Def;

#undef PARM_S
#define PARM_S(Val,Dim,Name,Def) \
	ASS( lstrlen(Def) <= Dim );\
  lstrcpy ( Val, Def );

#undef ZPARM_S
#define ZPARM_S(Val,Dim,Name,Def) \
	ASS( lstrlen(Def) <= Dim );\
  lstrcpy ( Val, Def );

#undef PARM_U
#define PARM_U(Val,Name,Def) \
	Val = Def;

#undef ZPARM_U
#define ZPARM_U(Val,Name,Def) \
	Val = Def;

#undef LEV
#define LEV(L)

#undef PARM
#define PARM(Val,Name)

#undef PARAM
#define PARAM(Val,Name)

#undef STATE
#define STATE(Val,Name)

#undef STATE_PARAM
#define STATE_PARAM(Val,Name)

#undef ZPARM
#define ZPARM(Val,Name)

#undef XPARM
#define XPARM(Ref,Val,Name)

#undef PARM_IF
#define PARM_IF(Val)

#undef PARM_END
#define PARM_END(Val)

#undef HIDE
#define HIDE
////////////////////////////////////////////////////////////////////////
#undef PROP_D
#define PROP_D(Val,Name,Def) \
  Val = Def;

#undef ZPROP_D
#define ZPROP_D(Val,Name,Def) \
  Val = Def;

#undef PROP_I
#define PROP_I(Val,Name,Def) \
  Val = Def;

#undef ZPROP_I
#define ZPROP_I(Val,Name,Def) \
  Val = Def;

#undef PROP_L
#define PROP_L(Val,Name,Def) \
  Val = 0;

#undef ZPROP_L
#define ZPROP_L(Val,Name,Def) \
  Val = 0;

#undef PROP_E
#define PROP_E(Val,Name) \
  GetDefault( Val, Name );

#undef ZPROP_E
#define ZPROP_E(Val,Name) \
  GetDefault( Val, Name );

#undef PROP_B
#define PROP_B(Val,Name,Def) \
  Val = Def;

#undef ZPROP_B
#define ZPROP_B(Val,Name,Def) \
  Val = Def;

#undef PROP_S
#define PROP_S(Val,Dim,Name,Def) \
  ASS( lstrlen(Def) <= Dim );\
  lstrcpy ( Val, Def );

#undef ZPROP_S
#define ZPROP_S(Val,Dim,Name,Def) \
  ASS( lstrlen(Def) <= Dim );\
  lstrcpy ( Val, Def );

#undef PROP_U
#define PROP_U(Val,Name,Def) \
  Val = Def;

#undef ZPROP_U
#define ZPROP_U(Val,Name,Def) \
  Val = Def;

#undef PROP
#define PROP(Val,Name)

#undef ZPROP
#define ZPROP(Val,Name)
//////////////////////////////////////////////////////////////////////
#undef DEFECT
#undef ZDEFECT
#undef OBJ_DEFECT
#undef SUB_DEFECT
#undef EVENT
#undef MODEL
#undef DEF_D	// double
#undef DEF_B	// bool	
#undef DEF_I	// int
#undef DEF_S	// char*
#undef ZDEF_D	// double
#undef ZDEF_B	// bool	
#undef ZDEF_I	// int
#undef ZDEF_S	// char*
#undef DEF_CB	// combo
// модификаторы
#undef PMIN
#undef PMAX
#undef PMINMAX

#define MODEL(ClassName,Default) \
{\
	if(pMain)\
	{\
		IBaseModel* pParentModel = this;\
		while(pParentModel->pMain)\
			pParentModel = pParentModel->pMain;\
		RootNode = pParentModel->RootNode;\
		pRecoveryDef = pParentModel->pRecoveryDef;\
	}\
	else\
	{\
		if ( ClassRootNode == 0 )\
		{\
			ClassRootNode = GiveClassRoot( $ ClassName );\
		}\
		RootNode = NewDefect( );\
		ObjRoot = GetDefect( RootNode );\
		ObjRoot->Parent = ClassRootNode;\
		lstrcpy ( ObjRoot->Name, ObjName );\
		ObjRoot->Show = false;\
		ObjRoot->Flags = CDef::Defects|CDef::Node;\
		if(Default)\
		{\
		ObjRoot->Show = true;\
		ObjRoot->Flags|=CDef::Active;\
		LastDefect = NewDefect( );\
		pRecoveryDef = GetDefect( LastDefect );\
		pRecoveryDef->Parent = RootNode;\
		lstrcpy ( pRecoveryDef->Name, RECOVERY_DEFECT_NAME);\
		lstrcpy ( pRecoveryDef->ObjName, ObjName );\
		pRecoveryDef->Flags = CDef::Defects|CDef::Defect|CDef::Active;\
		pRecoveryDef->ScriptID = 0;\
		pRecoveryDef->pModel = this;\
		this->AddDefect(pRecoveryDef);\
		}\
	}\
}

#define DEFECT(DefectVar,Comment) \
{\
	LastDefect = NewDefect( );\
	DefectVar = GetDefect( LastDefect );\
	DefectVar->Parent = RootNode;\
	lstrcpy ( DefectVar->Name, $ Comment );\
	lstrcpy ( DefectVar->ObjName, ObjName );\
	DefectVar->Flags = CDef::Defects|CDef::Defect|CDef::Active;\
	DefectVar->ScriptID = 0;\
	DefectVar->pModel = this;\
	IBaseModel* pParentModel = this;\
	while(pParentModel->pMain)\
		pParentModel = pParentModel->pMain;\
	pParentModel->AddDefect(DefectVar);\
}


#define NODE(NodeName) \
  {\
  NodeDefect = NewDefect( );\
  CDef * DefectVar = GetDefect( NodeDefect );\
  DefectVar->Parent = RootNode;\
  lstrcpy ( DefectVar->Name, NodeName );\
  lstrcpy ( DefectVar->ObjName, ObjName );\
  DefectVar->Flags = CDef::Defects|CDef::Node|CDef::Active;\
  DefectVar->ScriptID = 0;\
  DefectVar->pModel = this;\
  }

#define EVENT(DefectVar,Comment) \
	{\
	LastDefect = NewDefect( );\
	DefectVar = GetDefect( LastDefect );\
	DefectVar->Parent = RootNode;\
	lstrcpy ( DefectVar->Name,$ Comment );\
	lstrcpy ( DefectVar->ObjName, ObjName );\
  DefectVar->Flags = CDef::Events|CDef::Defect|CDef::Active;\
  DefectVar->ScriptID = 0;\
  DefectVar->pModel = this;\
	}

#define OBJ_DEFECT( Obj, Defect, Comment ) \
	Obj.Defect.Add( this ); Obj.Defect.SetName(Comment);
#define SUB_DEFECT( Obj ) Obj.AddDefects( RootNode );


#define DEF_D(Var, vName, vDef, vMin, vMax ) \
	{\
  Var = NewDefectPram( LastDefect );\
  Var->Type = CDefParam::tDbl;\
	lstrcpy(Var->Name,$ vName );\
  Var->Min = (float)vMin;\
  Var->Max = (float)vMax;\
    Var->Dbl = Var->_Dbl = (double)(vDef);\
	}

#define DEF_B(Var, vName, vDef) \
	{\
  Var = NewDefectPram( LastDefect );\
  Var->Type = CDefParam::tBool;\
	lstrcpy(Var->Name,$ vName );\
    Var->Bool = Var->_Bool = (bool)(vDef);\
	}

#define DEF_I(Var, vName, vDef, vMin, vMax ) \
	{\
  Var = NewDefectPram( LastDefect );\
  Var->Type = CDefParam::tInt;\
	lstrcpy(Var->Name,$ vName );\
  Var->Min = (float)vMin;\
  Var->Max = (float)vMax;\
    Var->Int = Var->_Int = (int)(vDef);\
	}


// модификаторы
#define PMIN(Var,Value) Var.SetMin(Value);

#define PMAX(Var,Value) Var.SetMax(Value);

#define PMINMAX(Var,Min,Max) Var.SetMinMax(Min,Max);

