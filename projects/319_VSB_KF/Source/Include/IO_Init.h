#undef PNT
#define PNT(Class,Val,Name) \
	{\
	Val = NULL;\
	::GetData( ObjName, Name, Class::TypeID, (void**)&Val );\
	if ( Val != NULL && Class::TypeID == CSensor::TypeID )\
		{\
		CSensor * pSens = (CSensor*)Val;\
		pSens -> pModel = this;\
		}\
	}
// Параметры
#undef MENU
#define MENU(Name)
#undef E_MENU
#define E_MENU(Name)
#undef TAB
#define TAB(Name,Level)
#undef ETAB
#define ETAB
#undef PARM_D
#define PARM_D(Val,Name,Def)
#undef PARM_I
#define PARM_I(Val,Name,Def)
#undef PARM_L
#define PARM_L(Val,Name,Def)
#undef PARM_B
#define PARM_B(Val,Name,Def)
#undef PARM_S
#define PARM_S(Val,Max,Name,Def)
#undef PARM
#define PARM(Val,Name)
#undef PARM_DD
#define PARM_DD(Val,Name,Def)
#undef PARM_ID
#define PARM_ID(Val,Name,Def)
#undef PARM_BD
#define PARM_BD(Val,Name,Def)
#undef PARM_SD
#define PARM_SD(Val,Dim,Name,Def)
#undef PROP_D
#define PROP_D(Val,Name,Def)
#undef PROP_I
#define PROP_I(Val,Name,Def)
#undef PROP_L
#define PROP_L(Val,Name,Def)
#undef PROP_B
#define PROP_B(Val,Name,Def)
#undef PROP_S
#define PROP_S(Val,Max,Name,Def)

#undef PARM_IF
#define PARM_IF(Val)

#undef PARM_NO
#define PARM_NO(Val)

#undef PARM_END
#define PARM_END

#undef LEV
#define LEV(L)

///
//////////////////////////////////////////////////////////////////////
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
#define MODEL(Name,Default)
#define DEFECT( Name, Comment )
#define OBJ_DEFECT( Obj, Defect, Comment )
#define SUB_DEFECT( Obj )
#define EVENT(Name, Comment )
#define PMIN(Value) 
#define PMAX(Value) 
#define PMINMAX(Min,Max) 
#define DEF_D(Var, Comment, Val, Min, Max)
#define DEF_B(Var, pname, Val)
#define DEF_I(Var, pname, Val, Min, Max)
#define DEF_S( pname, Val)
#define DEF_CB(Var, pname, Format, ID)

///////////////////////////////////////////////////////////
#undef POINT 
#define POINT(Fmt,Pnt) \
	sprintf ( Name, Fmt, ObjName );\
	if ( (*pGetObjByName)( Name, Type, (void**)&pObj ))\
		{\
	  int Addr = pObj->Addr;\
		if ( Addr > 0 )\
			Pnt.Address = Addr;\
		}\
	else\
		{\
		lstrcat ( Name, "#" );\
		if ( (*pGetObjByName)( Name, Type, (void**)&pObj ))\
			{\
			int Addr = pObj->Addr;\
			if ( Addr > 0 )\
				Pnt.Address = Addr;\
			}\
		}
